#pragma once
#include <MorseEvent.h>
#include <queue>
#include <vector>
#include <mutex>
#include <thread>

class DelayedMorseEvent {
public:
    DelayedMorseEvent(int timeToPost, const MorseEvent& eventToPost)
        : time(timeToPost),
          event(eventToPost) {
    }

    int time;
    MorseEvent event;
};

class MorseEventDelayer : public MorseEventListener {
public:

    MorseEventDelayer();
    MorseEventDelayer(int delayMs);
    virtual ~MorseEventDelayer();


    void setDelayInMilliseconds(int delayMs);
    void addListener(MorseEventListener& listener);
    void removeListener(MorseEventListener& listener);
    void onMorseEvent(const MorseEvent& event) override;

private:

    void startDispatcher();
    void stopDispatcher();

    void dispatchEvents();
    void notifyListeners(DelayedMorseEvent& delayedEvent);

    bool timeToDie;
    std::mutex mtx;
    std::thread delayThread;
    int delayInMilliseconds;
    int currentTime;
    std::queue<DelayedMorseEvent> queue;
    std::vector<MorseEventListener*> listeners;
};
