#include <MorseRenderer.h>
#include <iostream>
#include <string.h> // for memset

MorseRenderer::MorseRenderer(MorseDataSource& stream)
    : dataSource(stream) {
}

bool MorseRenderer::finished() const {
    return dataSource.finished() && currentElementRemainingSamples <= 0;
}

std::string as_string(MorseElement element) {
    switch (element) {
    case MorseElement::Dot: return "Dot";
    case MorseElement::SilentDot: return "SilentDot";
    case MorseElement::Dash: return "Dash";
    case MorseElement::SpaceBetweenChars: return "SpaceBetweenChars";
    case MorseElement::SpaceBetweenWords: return "SpaceBetweenWords";
    default: return "unknown";
    }
}


int MorseRenderer::render(short* buffer, int maxSamples) {
    int renderedSamples = 0;
    int remainingSamples = maxSamples;

    while (remainingSamples > 0) {

        if (currentElementRemainingSamples <= 0) {
            currentElement = dataSource.get();
            currentElementRemainingSamples = speed.sizeInSamplesFor(currentElement);
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