#include <MorsePlayer.h>

MorsePlayer::MorsePlayer() {
    settings.audio.sampleRate = DEFAULT_SAMPLERATE;
    settings.audio.channels = 2;
    settings.speed = MorseCodeSpeed::defaultSpeed();
    settings.frequency = DEFAULT_FREQUENCY;
    settings.punchiness = 0.8;

    delayer.addListener(*this);
    renderer.addListener(delayer);
}

MorsePlayer::MorsePlayer(const AudioSettings& audioSettings)
    : MorsePlayer() {
    settings.audio = audioSettings;
}

MorsePlayer& MorsePlayer::setSampleRate(int sr) {
    settings.audio.sampleRate = sr;
    return *this;
}

MorsePlayer& MorsePlayer::setFrequency(int frequency) {
    settings.frequency = frequency;
    return *this;
}

MorsePlayer& MorsePlayer::setSpeed(int wpm) {
    settings.speed = MorseCodeSpeed::fromParisWpmAndStyle(wpm, style);
    return *this;
}

MorsePlayer& MorsePlayer::setPunchiness(double punchiness) {
    settings.punchiness = punchiness;
    return *this;
}

MorsePlayer& MorsePlayer::setLatencyInMilliseconds(int latency) {
    delayer.setDelayInMilliseconds(latency);
    return *this;
}

MorsePlayer& MorsePlayer::render(short int* buffer, int samplesInBuffer) {
    renderer.render(buffer, samplesInBuffer);
    return *this;
}

MorsePlayer& MorsePlayer::play(std::string content) {
    finishedPlaying = false;
    renderer.feed(MorseDataSource(content, dictionary), settings);
    return *this;
}

MorsePlayer& MorsePlayer::addListener(MorseEventListener& listener) {
    delayer.addListener(listener);
    return *this;
}

MorsePlayer& MorsePlayer::removeListener(MorseEventListener& listener) {
    delayer.removeListener(listener);
    return *this;
}

const AudioSettings& MorsePlayer::audioSettings() const {
    return settings.audio;
}

void MorsePlayer::onMorseEvent(const MorseEvent& event) {
    if (event.type == MorseEventType::Eof) {
        finishedPlaying = true;
    }
}

bool MorsePlayer::finished() const {
    return finishedPlaying;
}
