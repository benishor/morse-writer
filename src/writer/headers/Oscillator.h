#pragma once

class Oscillator {
public:
    explicit Oscillator();

    void setSampleRate(int sampleRate);
    void setFrequency(double freq);
    double tick();

private:
    double frequency;
    int sampleRate;
    double phaseAdd;
    double phase;
};

