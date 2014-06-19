#pragma once
#include <MorseRenderer.h>
#include <MorseEventDelayer.h>

class MorsePlayer : public MorseEventListener {
public:
    MorsePlayer();
    explicit MorsePlayer(const AudioSettings& audioSettings);

    MorsePlayer& setSampleRate(int sr);
    MorsePlayer& setFrequency(int frequency);
    MorsePlayer& setSpeed(int wpm);
    MorsePlayer& setPunchiness(double punchiness);
    MorsePlayer& setLatencyInMilliseconds(int latency);

    MorsePlayer& addListener(MorseEventListener& listener);
    MorsePlayer& removeListener(MorseEventListener& listener);

    void onMorseEvent(const MorseEvent& event) override;

    MorsePlayer& play(std::string content);
    bool finished() const;

    const AudioSettings& audioSettings() const;
    MorsePlayer& render(short int* buffer, int samplesInBuffer);

private:
    const int DEFAULT_SPEED_WPM = 20;
    const int DEFAULT_FREQUENCY = 600;
    const int DEFAULT_SAMPLERATE = 44100;

    bool finishedPlaying = true;

    MorseDictionary dictionary = MorseDictionary::defaultDictionary();
    MorseCodeStyle style;
    MorseRendererSettings settings;
    MorseEventDelayer delayer;
    MorseRenderer renderer;
};
