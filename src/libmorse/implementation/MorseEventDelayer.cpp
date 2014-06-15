#include <MorseEventDelayer.h>
#include <chrono>

MorseEventDelayer::MorseEventDelayer()
    : currentTime(0),
      delayInMilliseconds(0),
      timeToDie(false) {
    startDispatcher();
}

MorseEventDelayer::MorseEventDelayer(int delayMs)
    : currentTime(0),
      delayInMilliseconds(delayMs),
      timeToDie(false) {
    startDispatcher();
}

void MorseEventDelayer::startDispatcher() {
    delayThread = std::move(std::thread([this]() {
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        while (!timeToDie) {
            std::this_thread::sleep_for(std::chrono::microseconds(500));
            end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsedSeconds = end - start;
            currentTime = static_cast<int>(elapsedSeconds.count() * 1000.0);
            this->dispatchEvents();
        }
    }));
}

void MorseEventDelayer::stopDispatcher() {
    timeToDie = true;
    delayThread.join();
}

MorseEventDelayer::~MorseEventDelayer() {
    stopDispatcher();
}

void MorseEventDelayer::setDelayInMilliseconds(int delayMs) {
    delayInMilliseconds = delayMs;
}

void MorseEventDelayer::addListener(MorseEventListener& listener) {
    listeners.push_back(&listener);
}

void MorseEventDelayer::removeListener(MorseEventListener& listener) {
    auto it = std::begin(listeners);
    while (it != std::end(listeners)) {
        if (*it == &listener) {
            listeners.erase(it);
            return;
        }
    }
}

void MorseEventDelayer::onMorseEvent(const MorseEvent& event) {
    std::lock_guard<std::mutex> _(mtx);
    queue.push(std::move(DelayedMorseEvent(currentTime + delayInMilliseconds, event)));
}

void MorseEventDelayer::dispatchEvents() {
    std::lock_guard<std::mutex> _(mtx);
    while (!queue.empty() && queue.front().time <= currentTime) {
        notifyListeners(queue.front());
        queue.pop();
    }
}

void MorseEventDelayer::notifyListeners(DelayedMorseEvent& delayedEvent) {
    for (auto listener : listeners)
        listener->onMorseEvent(delayedEvent.event);
}
