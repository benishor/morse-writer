#include <MorseRenderer.h>
#include <iostream>
#include <string.h> // for memset

#include <math.h>

int sizeInSamplesFor(MorseElement element, const MorseCodeSpeed& speed, int samplerate) {
    switch (element) {
    case MorseElement::Dot:
    case MorseElement::SilentDot:
        return (speed.dotSizeInMilliseconds / 1000.0) * samplerate;
    case MorseElement::Dash:
        return (speed.dashSizeInMilliseconds / 1000.0) * samplerate;
    case MorseElement::SpaceBetweenChars:
        return (speed.spaceBetweenCharsInMilliseconds / 1000.0) * samplerate;
    case MorseElement::SpaceBetweenWords:
        return (speed.spaceBetweenWordsInMilliseconds / 1000.0) * samplerate;
    default:
        return 0;
    }
}

MorseRenderer::MorseRenderer() {
}

MorseRenderer::~MorseRenderer() {
    if (dotShapingBuffer != nullptr) {
        delete [] dotShapingBuffer;
    }

    if (dashShapingBuffer != nullptr) {
        delete [] dashShapingBuffer;
    }
}

void MorseRenderer::buildShapingBuffers() {
    if (dotShapingBuffer != nullptr) {
        delete [] dotShapingBuffer;
    }

    if (dashShapingBuffer != nullptr) {
        delete [] dashShapingBuffer;
    }
    int dotSizeInSamples = sizeInSamplesFor(MorseElement::Dot, settings.speed, settings.audio.sampleRate);
    int dashSizeInSamples = sizeInSamplesFor(MorseElement::Dash, settings.speed, settings.audio.sampleRate);

    dotShapingBuffer = new double[dotSizeInSamples];
    dashShapingBuffer = new double[dashSizeInSamples];

    double shaping;
    for (int i = 0; i < dotSizeInSamples; i++) {
        shaping = sin(i * (M_PI / ((double) dotSizeInSamples - 1.0)));
        shaping = shaping * settings.punchiness;
        shaping = std::min(shaping, 1.0);
        dotShapingBuffer[i] = shaping;
    }

    const int HALF_DOT_SAMPLES = dotSizeInSamples >> 1;
    for (int i = 0; i < dashSizeInSamples; i++) {
        shaping = 1.0;
        if (i < HALF_DOT_SAMPLES) {
            shaping = sin(i * (M_PI / ((double) dotSizeInSamples - 1.0)));
        } else if (i >= (dashSizeInSamples - HALF_DOT_SAMPLES)) {
            shaping = sin((dashSizeInSamples - i - 1) * (M_PI / (double) dotSizeInSamples));
        }
        shaping = shaping * settings.punchiness;
        shaping = std::min(shaping, 1.0);

        dashShapingBuffer[i] = shaping;
    }
}


void MorseRenderer::feed(MorseDataSource& ds, MorseRendererSettings& renderSettings) {
    dataSource = ds;
    settings = renderSettings;

    oscillator.setSampleRate(settings.audio.sampleRate);
    oscillator.setFrequency(settings.frequency);

    buildShapingBuffers();
}


bool MorseRenderer::finished() const {
    return dataSource.finished() && currentElementRemainingSamples <= 0;
}

int MorseRenderer::render(short* buffer, int bufferSizeInSamples) {
    int renderedSamples = 0;
    int remainingSamples = bufferSizeInSamples;

    while (remainingSamples > 0) {

        if (currentElementRemainingSamples <= 0) {
            currentElement = dataSource.get();
            currentElementSamples = sizeInSamplesFor(currentElement, settings.speed, settings.audio.sampleRate);
            currentElementRemainingSamples = currentElementSamples;
        }

        int samplesToRenderNow = currentElement == MorseElement::None ?
                                 remainingSamples :
                                 std::min(currentElementRemainingSamples, remainingSamples);

        renderPartial(buffer + renderedSamples * settings.audio.channels, samplesToRenderNow);

        renderedSamples += samplesToRenderNow;
        remainingSamples -= samplesToRenderNow;
        currentElementRemainingSamples -= samplesToRenderNow;
    }

    return renderedSamples * settings.audio.channels;
}


void MorseRenderer::renderPartial(short* buffer, int samples) {
    switch (currentElement) {
    case MorseElement::Dot: {
        double* ampBuffer = &dotShapingBuffer[currentElementSamples - currentElementRemainingSamples];
        while (samples--) {
            double value = (short)(oscillator.tick() * 32767.0 * *ampBuffer++);
            for (int i = 0; i < settings.audio.channels; i++) {
                *buffer++ = value;
            }
        }
    }
    break;
    case MorseElement::Dash: {
        double* ampBuffer = &dashShapingBuffer[currentElementSamples - currentElementRemainingSamples];
        while (samples--) {
            double value = (short)(oscillator.tick() * 32767.0 * *ampBuffer++);
            for (int i = 0; i < settings.audio.channels; i++) {
                *buffer++ = value;
            }
        }
    }
    break;
    default:
        memset(buffer, 0, sizeof(short) * samples * settings.audio.channels);
        break;
    }
}