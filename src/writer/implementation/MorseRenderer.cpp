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

MorseRenderer::MorseRenderer(MorseDataSource& stream, const AudioSettings& audioSet, int frequency, double punch, const MorseCodeSpeed& spd)
    : dataSource(stream),
      audioSettings(audioSet),
      speed(spd),
      punchiness(punch) {

    oscillator.setSampleRate(audioSettings.sampleRate);
    oscillator.setFrequency(frequency);

    buildShapingBuffers();
}

MorseRenderer::~MorseRenderer() {
    if (dotShapingBuffer != NULL) {
        delete [] dotShapingBuffer;
    }

    if (dashShapingBuffer != NULL) {
        delete [] dashShapingBuffer;
    }
}

void MorseRenderer::buildShapingBuffers() {
    int dotSizeInSamples = sizeInSamplesFor(MorseElement::Dot, speed, audioSettings.sampleRate);
    int dashSizeInSamples = sizeInSamplesFor(MorseElement::Dash, speed, audioSettings.sampleRate);

    dotShapingBuffer = new double[dotSizeInSamples];
    dashShapingBuffer = new double[dashSizeInSamples];

    double shaping;
    for (int i = 0; i < dotSizeInSamples; i++) {
        shaping = sin(i * (M_PI / ((double) dotSizeInSamples - 1.0)));
        shaping = shaping * punchiness;
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
        shaping = shaping * punchiness;
        shaping = std::min(shaping, 1.0);

        dashShapingBuffer[i] = shaping;
    }
}

bool MorseRenderer::finished() const {
    return dataSource.finished() && currentElementRemainingSamples <= 0;
}

int MorseRenderer::render(short* buffer, int maxSamples) {
    int renderedSamples = 0;
    int remainingSamples = maxSamples;

    while (remainingSamples > 0) {

        if (currentElementRemainingSamples <= 0) {
            currentElement = dataSource.get();
            currentElementSamples = sizeInSamplesFor(currentElement, speed, audioSettings.sampleRate);
            currentElementRemainingSamples = currentElementSamples;
        }

        int samplesToRenderNow = currentElement == MorseElement::None ?
                                 remainingSamples :
                                 std::min(currentElementRemainingSamples, remainingSamples);

        renderPartial(buffer + renderedSamples, samplesToRenderNow);

        renderedSamples += samplesToRenderNow;
        remainingSamples -= samplesToRenderNow;
        currentElementRemainingSamples -= samplesToRenderNow;
    }

    return renderedSamples;
}


void MorseRenderer::renderPartial(short* buffer, int samples) {
    switch (currentElement) {
    case MorseElement::Dot: {
        double* ampBuffer = &dotShapingBuffer[currentElementSamples - currentElementRemainingSamples];
        while (samples--) {
            *buffer++ = (short)(oscillator.tick() * 32767.0 * *ampBuffer++);
        }
    }
    break;
    case MorseElement::Dash: {
        double* ampBuffer = &dashShapingBuffer[currentElementSamples - currentElementRemainingSamples];
        while (samples--) {
            *buffer++ = (short)(oscillator.tick() * 32767.0 * *ampBuffer++);
        }
    }
    break;
    default:
        memset(buffer, 0, sizeof(short) * samples);
        break;
    }
}