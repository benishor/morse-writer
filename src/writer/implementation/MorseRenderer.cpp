#include <MorseRenderer.h>
#include <iostream>
#include <string.h> // for memset

int sizeInSamplesFor(MorseElement element, const MorseCodeSpeed& speed, int samplerate) {
    switch (element) {
        case MorseElement::Dot:
        case MorseElement::SilentDot:
            return (speed.dotSizeInMilliseconds/1000.0) * samplerate;
        case MorseElement::Dash:
            return (speed.dashSizeInMilliseconds/1000.0) * samplerate;
        case MorseElement::SpaceBetweenChars:
            return (speed.spaceBetweenCharsInMilliseconds/1000.0) * samplerate;
        case MorseElement::SpaceBetweenWords:
            return (speed.spaceBetweenWordsInMilliseconds/1000.0) * samplerate;
        default:
            return 0;
    }
}

MorseRenderer::MorseRenderer(MorseDataSource& stream, const AudioSettings& audioSet, const MorseCodeSpeed& spd)
    : dataSource(stream),
      audioSettings(audioSet),
      speed(spd) {
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
            currentElementRemainingSamples = sizeInSamplesFor(currentElement, speed, audioSettings.sampleRate);
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
    case MorseElement::Dot:
        while (samples--) {
            *buffer++ = (short)(oscillator.tick() * 32767.0);
        }
        break;
    case MorseElement::Dash:
        while (samples--) {
            *buffer++ = (short)(oscillator.tick() * 32767.0);
        }
        break;
    default:
        memset(buffer, 0, sizeof(short) * samples);
        break;
    }
}