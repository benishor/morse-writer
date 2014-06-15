#pragma once

#include <MorseDataSource.h>
#include <AudioSettings.h>
#include <MorseCodeSpeed.h>
#include <Oscillator.h>
#include <vector>

struct MorseRendererSettings {
    AudioSettings audio;
    MorseCodeSpeed speed = MorseCodeSpeed::defaultSpeed();
    int frequency;
    double punchiness;
};

class MorseRenderer {
public:
    explicit MorseRenderer();
    virtual ~MorseRenderer();

    /**
     * @brief Determines whether the renderer finished rendering the data source completely or not.
     * @details [long description]
     * @return true if the renderer finished rendering the data source completely, false otherwise
     */
    bool finished() const;

    /**
     * @brief Subscribes an event listener
     * @details Subscribes the provided event listener to the existing list. Duplicates are not dealt with so if you register your listener twice, you will receive each event twice.
     * 
     * @param listener the event listener to be added to the current listener list
     */
    void addListener(MorseEventListener& listener);

    /**
     * @brief Unsubscribes an event listener
     * @details Unsubscribes the provided event listener to the existing list. Duplicates are not dealt with so if you registered your event listener twice, 
     * you must call this method twice in order to remove both subscriptions. 
     * In case the listener is not found, nothing happens.
     * 
     * @param listener [description]
     */
    void removeListener(MorseEventListener& listener);


    /**
     * @brief Feeds the renderer with the specified data source and settings.
     * @details [long description]
     * 
     * @param dataSource the data source to be rendered until completion
     * @param settings the settings used to render the provided data source
     */
    void feed(MorseDataSource&& dataSource, MorseRendererSettings& settings);
    void feed(MorseDataSource& dataSource, MorseRendererSettings& settings);

    /**
     * @brief [brief description]
     * @details [long description]
     * 
     * @param buffer [description]
     * @param bufferSizeInSamples [description]
     * 
     * @return [description]
     */
    int render(short* buffer, int bufferSizeInSamples);

private:

    std::vector<MorseEventListener*> eventListeners;

    MorseDataSource dataSource;
    MorseRendererSettings settings;

    void buildShapingBuffers();
    void selectNextElementToRender();
    void renderPartial(short* buffer, int samples);

    int currentElementRemainingSamples = 0;
    int currentElementSamples = 0;
    MorseElement currentElement = MorseElement::None;

    Oscillator oscillator;

    double* dotShapingBuffer = nullptr;
    double* dashShapingBuffer = nullptr;
};
