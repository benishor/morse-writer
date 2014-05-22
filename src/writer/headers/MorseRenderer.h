#pragma once

#include <MorseDataSource.h>
#include <AudioSettings.h>
#include <MorseCodeSpeed.h>
#include <Oscillator.h>

struct MorseRendererSettings {
    AudioSettings& audioSettings;
    MorseCodeSpeed& speed;
    int frequency;
    double punchiness;
};

class MorseRenderer {
public:
    explicit MorseRenderer(MorseDataSource& dataSource, const AudioSettings& audioSettings, int frequency, double punchiness, const MorseCodeSpeed& speed);
    virtual ~MorseRenderer();

    /**
     * @brief Determines whether the renderer finished rendering the data source completely or not.
     * @details [long description]
     * @return true if the renderer finished rendering the data source completely, false otherwise
     */
    bool finished() const;


    // void setSettings(const MorseRendererSettings& settings);
    // void setDataSource(MorseDataSource& dataSource);

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
    void buildShapingBuffers();
    void renderPartial(short* buffer, int samples);

    int currentElementRemainingSamples = 0;
    int currentElementSamples = 0;
    MorseElement currentElement = MorseElement::None;

    Oscillator oscillator;

    MorseDataSource& dataSource;
    const AudioSettings& audioSettings;
    const MorseCodeSpeed& speed;
    double punchiness;

    double* dotShapingBuffer;
    double* dashShapingBuffer;
};
