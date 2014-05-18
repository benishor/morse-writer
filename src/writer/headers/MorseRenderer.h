#pragma once

#include <MorseDataSource.h>
#include <AudioSettings.h>
#include <MorseCodeSpeed.h>
#include <Oscillator.h>

class MorseRenderer {
public:
    explicit MorseRenderer(MorseDataSource& dataSource, const AudioSettings& audioSettings, int frequency, const MorseCodeSpeed& speed);

    bool finished() const;
    int render(short* buffer, int maxSamples);

private:

    void renderPartial(short* buffer, int samples);

    int currentElementRemainingSamples = 0;
    MorseElement currentElement = MorseElement::None;

    Oscillator oscillator;

    MorseDataSource& dataSource;
    const AudioSettings& audioSettings;
    const MorseCodeSpeed& speed;
};
