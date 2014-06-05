#include <MorseRenderer.h>
#include <RtAudio.h>

#include <iostream>
#include <cstdlib>
#include <math.h>

#if defined( __WINDOWS_ASIO__ ) || defined( __WINDOWS_DS__ ) || defined( __WINDOWS_WASAPI__ )
#include <windows.h>
#define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds )
#else // Unix variants
#include <unistd.h>
#define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif

int outputCallback(void* outputBuffer,
                   void* /*inputBuffer*/,
                   unsigned int bufferFrames,
                   double /*streamTime*/,
                   RtAudioStreamStatus /*status*/,
                   void* userData) {

    short int* buffer = static_cast<short int*>(outputBuffer);

    MorseRenderer& morseRenderer = (MorseRenderer&)(*(MorseRenderer*)userData);
    morseRenderer.render(buffer, bufferFrames);

    return morseRenderer.finished() ? 1 : 0;
}


static std::string eventName(const MorseEventType& type) {
    switch (type) {
    case MorseEventType::StartChar: return "StartChar";
    case MorseEventType::EndChar: return "EndChar";
    case MorseEventType::Eof: return "EOF";
    default: return "unknown event";
    }
}


class MorseListener : public MorseEventListener {
public:
    void setContent(std::string c) {
        content = c;
        renderedCharacters = 0;
        showRenderedOutput();
    }

    void onMorseEvent(const MorseEvent& event) {
        if (event.type == MorseEventType::EndChar) {
            renderedCharacters++;
            showRenderedOutput();
        } else if (event.type == MorseEventType::Eof) {
            std::cout << "\e[0;37m" << std::flush;
            std::cout << std::endl;
        }
    }

    void showRenderedOutput() {
        std::cout << "\r" << std::flush;
        int i = 0;
        for (auto c : content) {
            if (i < renderedCharacters) {
                std::cout << "\e[0;32m" << std::flush;
            } else if (i > renderedCharacters) {
                std::cout << "\e[0;37m" << std::flush;
            } else {
                std::cout << "\e[0;31m" << std::flush;
            }
            std::cout << c << std::flush;
            i++;
        }
    }

private:
    int renderedCharacters;
    std::string content;
};

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <text to render>" << std::endl;
        exit(1);
    }


    MorseDictionary dictionary = MorseDictionary::defaultDictionary();
    MorseCodeStyle style;
    MorseCodeSpeed speed = MorseCodeSpeed::fromFarnsworthAndStyle(20, 20, style);
    MorseDataSource dataSource = MorseDataSource(argv[1], dictionary);

    MorseRendererSettings settings;
    settings.audio.sampleRate = 48000;
    settings.audio.channels = 2;
    settings.speed = speed;
    settings.frequency = 600;
    settings.punchiness = 1;

    MorseRenderer renderer;
    renderer.feed(dataSource, settings);

    MorseListener listener;
    listener.setContent(dataSource.getContent());

    renderer.addListener(listener);

    RtAudio dac;

    // for (int i = 0; i < dac.getDeviceCount(); i++) {
    //     RtAudio::DeviceInfo info = dac.getDeviceInfo(i);
    //     std::cout << "Device info [" << info.name << "], output channels: " << info.outputChannels << ", isDefaultOutput: "  << info.isDefaultOutput << std::endl;
    // }

    RtAudio::StreamParameters oParams;
    oParams.deviceId = dac.getDefaultOutputDevice();
    oParams.nChannels = settings.audio.channels;
    oParams.firstChannel = 0;

    unsigned int bufferFrames = 32;

    try {
        dac.openStream(&oParams, NULL, RTAUDIO_SINT16, settings.audio.sampleRate, &bufferFrames, &outputCallback, &renderer);
        dac.startStream();

        while (dac.isStreamRunning()) {
            SLEEP(100);
        }

    } catch (RtAudioError& e) {
        std::cout << '\n' << e.getMessage() << '\n' << std::endl;
    }

    if (dac.isStreamOpen()) {
        dac.closeStream();
    }

    return 0;
}
