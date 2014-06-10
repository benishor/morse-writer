#include <MorseRenderer.h>
#include <RtAudio.h>

#include <iostream>
#include <cstdlib>
#include <math.h>

#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>

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
        case MorseEventType::StartChar:
            return "StartChar";
        case MorseEventType::EndChar:
            return "EndChar";
        case MorseEventType::Eof:
            return "EOF";
        default:
            return "unknown event";
    }
}

class DelayedMorseEvent {
public:
    DelayedMorseEvent(int timeToPost, const MorseEvent& eventToPost);

    int time;
    MorseEvent event;
};


std::ostream& operator << (std::ostream& out, const MorseEvent& event) {
    out << "MorseEvent{" << "type: " << eventName(event.type) << ", letter: " << event.letter << "}";
    return out;
}

std::ostream& operator << (std::ostream& out, const DelayedMorseEvent& delayedEvent) {
    out << "DelayedMorseEvent{" << "time: " << delayedEvent.time << ", event: " << delayedEvent.event << "}";
    return out;
}


DelayedMorseEvent::DelayedMorseEvent(int timeToPost, const MorseEvent& eventToPost)
    : time(timeToPost),
      event(eventToPost) {
}


class DelayedListener : public MorseEventListener {
public:

    DelayedListener(int delayMs) : delayInMilliseconds(delayMs) {
        delayThread = new std::thread([this]() {
            std::chrono::time_point<std::chrono::system_clock> start, end;
            start = std::chrono::system_clock::now();
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                end = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsedSeconds = end - start;
                this->update(static_cast<int>(elapsedSeconds.count() * 1000.0));
                start = end;
            }
        });
    }

    virtual ~DelayedListener() {
    }

    void addListener(MorseEventListener& listener) {
        listeners.push_back(&listener);
    }

    void removeListener(MorseEventListener& listener) {
        auto it = std::begin(listeners);
        while (it != std::end(listeners)) {
            if (*it == &listener) {
                listeners.erase(it);
                return;
            }
        }
    }

    void onMorseEvent(const MorseEvent& event) override {
        std::lock_guard<std::mutex> _(mtx);
        queue.push(new DelayedMorseEvent(currentTime + delayInMilliseconds, event));
    }

    void update(int dt) {
        std::lock_guard<std::mutex> _(mtx);
        currentTime += dt;
        while (!queue.empty() && queue.front()->time <= currentTime) {
            notifyListeners(*queue.front());
            queue.pop();
        }
    }

    void notifyListeners(DelayedMorseEvent& delayedEvent) {
        for (auto listener : listeners)
            listener->onMorseEvent(delayedEvent.event);
    }

private:
    std::mutex mtx;
    std::thread* delayThread;
    int delayInMilliseconds;
    int currentTime;
    std::queue<DelayedMorseEvent*> queue;
    std::vector<MorseEventListener*> listeners;
};


class MorseListener : public MorseEventListener {
public:
    void setContent(std::string c) {
        content = c;
        renderedCharacters = 0;
        showRenderedOutput();
    }

    void onMorseEvent(const MorseEvent& event) override {
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

private:
    std::atomic<int> renderedCharacters;
    std::string content;
};

const int BUFFER_FRAMES = 256;
const int SPEED_WPM = 50;

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <text to render>" << std::endl;
        exit(1);
    }


    MorseDictionary dictionary = MorseDictionary::defaultDictionary();
    MorseCodeStyle style;
    MorseCodeSpeed speed = MorseCodeSpeed::fromFarnsworthAndStyle(SPEED_WPM, SPEED_WPM, style);
    MorseDataSource dataSource = MorseDataSource(argv[1], dictionary);

    MorseRendererSettings settings;
    settings.audio.sampleRate = 48000;
    settings.audio.channels = 2;
    settings.speed = speed;
    settings.frequency = 600;
    settings.punchiness = 0.8;

    MorseRenderer renderer;
    renderer.feed(dataSource, settings);

    MorseListener listener;
    listener.setContent(dataSource.getContent());

    DelayedListener delayer((BUFFER_FRAMES * 4 * 1000) / settings.audio.sampleRate);
    renderer.addListener(delayer);

    delayer.addListener(listener);

    RtAudio dac;

    // for (int i = 0; i < dac.getDeviceCount(); i++) {
    //     RtAudio::DeviceInfo info = dac.getDeviceInfo(i);
    //     std::cout << "Device info [" << info.name << "], output channels: " << info.outputChannels << ", isDefaultOutput: "  << info.isDefaultOutput << std::endl;
    //     for (auto samplerate : info.sampleRates) {
    //         std::cout << "\tsamplerate: " << samplerate << std::endl;
    //     }
    // }

    RtAudio::StreamParameters oParams;
    oParams.deviceId = dac.getDefaultOutputDevice();
    oParams.nChannels = settings.audio.channels;
    oParams.firstChannel = 0;

    unsigned int bufferFrames = BUFFER_FRAMES;

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
