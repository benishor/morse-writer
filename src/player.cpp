#include <MorsePlayer.h>
#include <RtAudio.h>

#include <iostream>
#include <cstdlib>
#include <math.h>

#include <atomic>

class DumpToConsoleEventListener : public MorseEventListener {
public:

    void onMorseEvent(const MorseEvent& event) override {
        if (event.type == MorseEventType::NewContent) {
            setContent(event.content);
        } else if (event.type == MorseEventType::EndChar) {
            renderedCharacters++;
            showRenderedOutput();
        } else if (event.type == MorseEventType::Eof) {
            std::cout << "\e[0;37m" << std::flush;
            std::cout << std::endl;
        }
    }

private:
    void showRenderedOutput() {
        std::cout << "\r" << std::flush;
        int i = 0;
        int howMuchIsRendered = renderedCharacters.load();
        for (auto c : content) {
            if (i < howMuchIsRendered) {
                std::cout << "\e[0;32m" << std::flush;
            } else if (i > howMuchIsRendered) {
                std::cout << "\e[0;37m" << std::flush;
            } else {
                std::cout << "\e[0;31m" << std::flush;
            }
            std::cout << c << std::flush;
            i++;
        }
    }

    void setContent(std::string c) {
        content = c;
        renderedCharacters = 0;
        showRenderedOutput();
    }

    std::atomic<int> renderedCharacters;
    std::string content;
};

class SoundServer {
public:
    SoundServer(MorsePlayer& plr) : player(plr) {
        RtAudio::StreamParameters oParams;
        oParams.deviceId = dac.getDefaultOutputDevice();
        oParams.nChannels = player.audioSettings().channels;
        oParams.firstChannel = 0;

        unsigned int bufferFrames = BUFFER_FRAMES;
        try {
            dac.openStream(&oParams, NULL, RTAUDIO_SINT16, player.audioSettings().sampleRate, &bufferFrames, &renderCallback, &player);
            player.setLatencyInMilliseconds((bufferFrames * 4 * 1000) / player.audioSettings().sampleRate);
            dac.startStream();
        } catch (RtAudioError& e) {
            throw e;
        }
    };

    virtual ~SoundServer() {
        if (dac.isStreamOpen())
            dac.closeStream();
    };

    static int renderCallback(void* outputBuffer,
                              void* /*inputBuffer*/,
                              unsigned int bufferFrames,
                              double /*streamTime*/,
                              RtAudioStreamStatus /*status*/,
                              void* userData) {

        short int* buffer = static_cast<short int*>(outputBuffer);
        MorsePlayer& player = (MorsePlayer&)(*(MorsePlayer*)userData);
        player.render(buffer, bufferFrames);
        return 0;
    }

private:
    const int BUFFER_FRAMES = 4096;
    MorsePlayer& player;
    RtAudio dac;
};


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <text to render>" << std::endl;
        exit(1);
    }

    AudioSettings audioSettings;
    audioSettings.sampleRate = 48000;
    audioSettings.channels = 2;

    MorsePlayer player(audioSettings);
    SoundServer server(player);

    DumpToConsoleEventListener listener;
    player.addListener(listener);
    player.setSpeed(60);
    player.setFrequency(500);

    player.play(argv[1]);
    while (!player.finished()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    std::cout << "Finished playing" << std::endl;
    return 0;
}