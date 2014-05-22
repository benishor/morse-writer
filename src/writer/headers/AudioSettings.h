#pragma once

struct AudioSettings {
    int sampleRate = 44100;
    int channels = 1;

    AudioSettings() {};
    AudioSettings(int sr, int chans) : sampleRate(sr), channels(chans) {}

    int bytesPerSample() const {
        return sizeof(short);
    }
};
