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


int main() {

    MorseDictionary dictionary = MorseDictionary::defaultDictionary();
    AudioSettings audioSettings {44800, 2};
    MorseCodeStyle style;
    MorseCodeSpeed speed = MorseCodeSpeed::fromFarnsworthAndStyle(50, 50, style);
    MorseDataSource dataSource = MorseDataSource("cq cq cq de yo6ssw yo6ssw yo6ssw pse k", dictionary);
    MorseRenderer renderer = MorseRenderer(dataSource, audioSettings, 600, 1, speed);

    RtAudio dac;

    RtAudio::StreamParameters oParams;
    oParams.deviceId = dac.getDefaultOutputDevice();
    oParams.nChannels = audioSettings.channels;
    oParams.firstChannel = 0;

    unsigned int bufferFrames = 128;

    try {
        dac.openStream(&oParams, NULL, RTAUDIO_SINT16, audioSettings.sampleRate, &bufferFrames, &outputCallback, &renderer);
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
