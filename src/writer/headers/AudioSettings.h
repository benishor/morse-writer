#pragma once

struct AudioSettings {
    int sampleRate = 44100;
    int channels = 1;

    int bytesPerSample() const {
        return sizeof(short);
    }
};
