#pragma once

#include <MorseDataSource.h>
#include <AudioSettings.h>
#include <MorseCodeSpeed.h>
#include <Oscillator.h>

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


    void feed(MorseDataSource& dataSource, MorseRendererSettings& settings);

    /**
     * @brief [brief description]
     * @details [long description]
     * 
     * @param buffer [description]
     * @param maxSamples [description]
     * 
     * @return [description]
     */
    int render(short* buffer, int bufferSizeInSamples);

private:

    MorseDataSource dataSource;
    MorseRendererSettings settings;

    void buildShapingBuffers();
    void renderPartial(short* buffer, int samples);

    int currentElementRemainingSamples = 0;
    int currentElementSamples = 0;
    MorseElement currentElement = MorseElement::None;

    Oscillator oscillator;

    double* dotShapingBuffer = nullptr;
    double* dashShapingBuffer = nullptr;
};
