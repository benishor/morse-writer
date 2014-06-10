#include <Oscillator.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

Oscillator::Oscillator()
    : sampleRate(44100) {
    setFrequency(500);
}

void Oscillator::setSampleRate(int sr) {
    sampleRate = sr;
    setFrequency(frequency);
}

void Oscillator::setFrequency(double f) {
    frequency = f;
    phaseAdd = (2.0 * M_PI * frequency) / (double)sampleRate;
}

double Oscillator::tick() {
    phase += phaseAdd;
    return sin(phase);
}
