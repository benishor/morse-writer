#include <MorseCodeSpeed.h>

const int       NUMBER_OF_DOTS_IN_PARIS = 10;
const int       NUMBER_OF_SILENT_DOTS_IN_PARIS = 9;
const int       NUMBER_OF_DASHES_IN_PARIS = 4;
const double    SECONDS_IN_A_MINUTE = 60.0;
const double    MILLISECONDS_IN_A_SECOND = 1000.0;
const int       NUMBER_OF_SPACES_BETWEEN_CHARS_IN_PARIS = 4;

static double spaceImpulsesInParis(const MorseCodeStyle& style) {
    return NUMBER_OF_SPACES_BETWEEN_CHARS_IN_PARIS * style.impulsesBetweenCharacters + 
           style.impulsesBetweenWords;
}

static double nonSpaceImpulsesInParis(const MorseCodeStyle& style) {
    return NUMBER_OF_DOTS_IN_PARIS + 
           NUMBER_OF_SILENT_DOTS_IN_PARIS + 
           NUMBER_OF_DASHES_IN_PARIS * style.dashDotRatio;
}

MorseCodeSpeed::MorseCodeSpeed() {
}

MorseCodeSpeed MorseCodeSpeed::fromParisWpmAndStyle(int wpm, const MorseCodeStyle& style) {
    double impulsesPerMinute = wpm * (nonSpaceImpulsesInParis(style) + spaceImpulsesInParis(style));

    MorseCodeSpeed speed;
    speed.dotSizeInMilliseconds = (SECONDS_IN_A_MINUTE / impulsesPerMinute) * MILLISECONDS_IN_A_SECOND;
    speed.dashSizeInMilliseconds = speed.dotSizeInMilliseconds * style.dashDotRatio;
    speed.spaceBetweenCharsInMilliseconds = speed.dotSizeInMilliseconds * style.impulsesBetweenCharacters;
    speed.spaceBetweenWordsInMilliseconds = speed.dotSizeInMilliseconds * style.impulsesBetweenWords;

    return speed;
}


MorseCodeSpeed MorseCodeSpeed::fromFarnsworthAndStyle(int characterWpm, int feelsLikeWpm, const MorseCodeStyle& style) {
    MorseCodeSpeed speed = MorseCodeSpeed::fromParisWpmAndStyle(characterWpm, style);
    double totalNonspaceTimePerMinuteInMs = nonSpaceImpulsesInParis(style)
                                            * feelsLikeWpm
                                            * speed.dotSizeInMilliseconds;

    double totalSpaceTimePerMinuteInMs = (SECONDS_IN_A_MINUTE * MILLISECONDS_IN_A_SECOND) - 
                                         totalNonspaceTimePerMinuteInMs;
    double pauseImpulseLengthInMs = totalSpaceTimePerMinuteInMs / (spaceImpulsesInParis(style) * feelsLikeWpm);

    speed.spaceBetweenCharsInMilliseconds = style.impulsesBetweenCharacters * pauseImpulseLengthInMs;
    speed.spaceBetweenWordsInMilliseconds = style.impulsesBetweenWords * pauseImpulseLengthInMs;

    return speed;
}
