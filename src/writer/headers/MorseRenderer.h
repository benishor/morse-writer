#pragma once

#include <MorseDataSource.h>
#include <AudioSettings.h>
#include <MorseCodeSpeed.h>

#define _USE_MATH_DEFINES
#include <math.h>


class Oscillator {
public:
    explicit Oscillator(double sr)
        : sampleRate(sr) {
        setFrequency(500);
    }

    void setFrequency(double freq) {
        frequency = freq;
        phaseAdd = (2.0 * M_PI * frequency) / sampleRate;
    }

    double tick() {
        phase += phaseAdd;
        return sin(phase);
    }

private:
    double frequency;
    double sampleRate;
    double phaseAdd;
    double phase;
};

class MorseRenderer {
public:
    explicit MorseRenderer(MorseDataSource& dataSource, const AudioSettings& audioSettings, const MorseCodeSpeed& speed);

    bool finished() const;
    int render(short* buffer, int maxSamples);

private:

    void renderPartial(short* buffer, int samples);

    int currentElementRemainingSamples = 0;
    MorseElement currentElement = MorseElement::None;

    Oscillator oscillator {44100.0};

    MorseDataSource& dataSource;
    const AudioSettings& audioSettings;
    const MorseCodeSpeed& speed;
};
