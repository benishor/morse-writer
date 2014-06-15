#pragma once

#include <MorseCodeStyle.h>

struct MorseCodeSpeed {
    double dotSizeInMilliseconds;
    double dashSizeInMilliseconds;
    double spaceBetweenCharsInMilliseconds;
    double spaceBetweenWordsInMilliseconds;

    static MorseCodeSpeed fromParisWpmAndStyle(int wpm, const MorseCodeStyle& style);
    static MorseCodeSpeed fromFarnsworthAndStyle(int characterWpm, int feelsLikeWpm, const MorseCodeStyle& style);
    static MorseCodeSpeed defaultSpeed();
};
