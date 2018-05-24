#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#define LPFREQ_ID "lpFreq"
#define LPFREQ_NAME "LPFreq"
#define LPQ_ID "lpQ"
#define LPQ_NAME "LPQ"
#define HPFREQ_ID "hpFreq"
#define HPFREQ_NAME "HPFreq"
#define HPQ_ID "hpQ"
#define HPQ_NAME "HPQ"
#define LSFREQ_ID "lsFreq"
#define LSFREQ_NAME "LSFreq"
#define LSG_ID "lsG"
#define LSG_NAME "LSG"
#define HSFREQ_ID "hsFreq"
#define HSFREQ_NAME "HSFreq"
#define HSG_ID "hsG"
#define HSG_NAME "HSG"
#define PFREQ_ID "pFreq"
#define PFREQ_NAME "PFreq"
#define PQ_ID "pQ"
#define PQ_NAME "PQ"
#define PG_ID "pG"
#define PG_NAME "PG"
#define IV_ID "volIn"
#define IV_NAME "VolIn"
#define OV_ID "volOut"
#define OV_NAME "VolOut"
#include "Biquad.h"


//==============================================================================
/**
*/

class EqAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    EqAudioProcessor();
    ~EqAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    
    // LowPass params
    float lpFreq;
    float lpQ;
    
    // HighPass params
    float hpFreq;
    float hpQ;
    
    // LowShelf params
    float lsFreq;
    float lsG;
    
    // HighShelf params
    float hsFreq;
    float hsG;
    
    // HighShelf params
    float pFreq;
    float pG;
    float pQ;
    
    float lastOut;
    
    int numpases, numpases2;

    float inputVolume;
    float outputVolume;
    
    static const int kChannels = 2;
    
    Biquad lpFilt[kChannels], hpFilt[kChannels], lsFilt[kChannels], hsFilt[kChannels], pFilt[kChannels];

    AudioProcessorValueTreeState treeState;


private:
    //==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqAudioProcessor)
};
