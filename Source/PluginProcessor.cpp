#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EqAudioProcessor::EqAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
        treeState(*this, nullptr)

#endif
{
    
    // Freq. params
    NormalisableRange<float> freqRange (20.0f, 20000.0f);
    treeState.createAndAddParameter(LPFREQ_ID, LPFREQ_NAME, LPFREQ_NAME, freqRange, 20000.0f, nullptr, nullptr);
    treeState.createAndAddParameter(HPFREQ_ID, HPFREQ_NAME, HPFREQ_NAME, freqRange, 20.0f, nullptr, nullptr);
    treeState.createAndAddParameter(LSFREQ_ID, LSFREQ_NAME, LSFREQ_NAME, freqRange, 100.0f, nullptr, nullptr);
    treeState.createAndAddParameter(HSFREQ_ID, HSFREQ_NAME, HSFREQ_NAME, freqRange, 18000.0f, nullptr, nullptr);
    treeState.createAndAddParameter(PFREQ_ID, PFREQ_NAME, PFREQ_NAME, freqRange, 500.0f, nullptr, nullptr);
    
    // Q params
    NormalisableRange<float> qRange (0.707f, 15.0f);
    treeState.createAndAddParameter(LPQ_ID, LPQ_NAME, LPQ_NAME, qRange, 0.707f, nullptr, nullptr);
    treeState.createAndAddParameter(HPQ_ID, HPQ_NAME, HPQ_NAME, qRange, 0.707f, nullptr, nullptr);
    treeState.createAndAddParameter(PQ_ID, PQ_NAME, PQ_NAME, qRange, 0.707f, nullptr, nullptr);
    
    // Gain params
    NormalisableRange<float> gainRange (-24.0f, 24.0f);
    treeState.createAndAddParameter(LSG_ID, LSG_NAME, LSG_NAME, gainRange, 0.0f, nullptr, nullptr);
    treeState.createAndAddParameter(HSG_ID, HSG_NAME, HSG_NAME, gainRange, 0.0f, nullptr, nullptr);
    treeState.createAndAddParameter(PG_ID, PG_NAME, PG_NAME, gainRange, 0.0f, nullptr, nullptr);
    
    // I/O Volume params
    NormalisableRange<float> volumeRange (-48.0f, 6.0f);
    treeState.createAndAddParameter(IV_ID, IV_NAME, IV_NAME, volumeRange, 0.0f, nullptr, nullptr);
    treeState.createAndAddParameter(OV_ID, OV_NAME, OV_NAME, volumeRange, 0.0f, nullptr, nullptr);
    
    treeState.state = ValueTree("Presets");
    


}

EqAudioProcessor::~EqAudioProcessor()
{
}

//==============================================================================
const String EqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EqAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EqAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EqAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EqAudioProcessor::setCurrentProgram (int index)
{
}

const String EqAudioProcessor::getProgramName (int index)
{
    return {};
}

void EqAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void EqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //sampleRate = getSampleRate();
    
    lastOut = 0.0f;
    
    lpFreq = 20000.0f;
    lpQ = 0.707f;
    
    hpFreq = 20.0f;
    hpQ = 0.707f;
    
    lsFreq = 100.0f;
    lsG = 0.0f;
    
    hsFreq = 18000.0f;
    hsG = 0.0f;
    
    pFreq = 500.0f;
    pG = 0.0f;
    pQ = 0.707f;
    
     for(int i=0; i<kChannels; ++i){
        
        lpFilt[i].setBiquad(lowpass, lpFreq/sampleRate, lpQ, 0);
    
        hpFilt[i].setBiquad(highpass, hpFreq/sampleRate, hpQ, 0);
    
        lsFilt[i].setBiquad(lowshelf, lsFreq/sampleRate, 0, lsG);
    
        hsFilt[i].setBiquad(highshelf, hsFreq/sampleRate, 0, hsG);
    
        pFilt[i].setBiquad(peak, pFreq/sampleRate, pQ, pG);
    }
    
    reset();
    
}

void EqAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EqAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void EqAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    const int numSamples = buffer.getNumSamples();
    
    for(int i=0; i<kChannels; ++i){
        
        lpFilt[i].setBiquad(lowpass, lpFreq/getSampleRate(), lpQ, 0);
        
        hpFilt[i].setBiquad(highpass, hpFreq/getSampleRate(), hpQ, 0);
        
        lsFilt[i].setBiquad(lowshelf, lsFreq/getSampleRate(), 0, lsG);
        
        hsFilt[i].setBiquad(highshelf, hsFreq/getSampleRate(), 0, hsG);
        
        pFilt[i].setBiquad(peak, pFreq/getSampleRate(), pQ, pG);
    }
    
    for (int chan = 0; chan<buffer.getNumChannels();++chan){
        
        //const float *inputDataL = buffer.getReadPointer(chan);
        float *outputData = buffer.getWritePointer(chan);
        
        for (int sample = 0; sample<numSamples; ++sample)
        {

            float inputSample = buffer.getSample(chan,sample) * pow(10,inputVolume/20);
            
            for (int i = 0; i < numpases; ++i)
            {
                outputData[sample] = lpFilt[chan].process(inputSample);
                inputSample = outputData[sample];
            }
            
            outputData[sample] =  lsFilt[chan].process(inputSample);
            lastOut = outputData[sample];
            outputData[sample] = pFilt[chan].process(lastOut);
            lastOut = outputData[sample];
            outputData[sample] =   hsFilt[chan].process(lastOut);
            lastOut = outputData[sample];

            for (int i = 0; i < numpases2; ++i)
            {
                outputData[sample] =  hpFilt[chan].process(lastOut);
                lastOut = outputData[sample];
            }
            
            lastOut = lastOut * pow(10,outputVolume/20);
        }
    }
}

//==============================================================================
bool EqAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* EqAudioProcessor::createEditor()
{
    return new EqAudioProcessorEditor (*this);
}

//==============================================================================
void EqAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    
    auto state = treeState.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
    
    MemoryOutputStream (destData, true).writeFloat (lpFreq);
    MemoryOutputStream (destData, true).writeFloat (lpQ);
    MemoryOutputStream(destData, true).writeFloat (numpases);
    
    MemoryOutputStream (destData, true).writeFloat (hpFreq);
    MemoryOutputStream (destData, true).writeFloat (hpQ);
    MemoryOutputStream(destData, true).writeFloat (numpases2);
    
    MemoryOutputStream (destData, true).writeFloat (lsFreq);
    MemoryOutputStream (destData, true).writeFloat (lsG);
    
    MemoryOutputStream (destData, true).writeFloat (hsFreq);
    MemoryOutputStream (destData, true).writeFloat (hsG);
    
    MemoryOutputStream (destData, true).writeFloat (pFreq);
    MemoryOutputStream (destData, true).writeFloat (pG);
    MemoryOutputStream (destData, true).writeFloat (pQ);
    
    
    
    /*// Create an outer XML element..
    XmlElement xml2 ("EQPRESETS");
    xml2.setAttribute ("Lowpass Freq.", lpFreq);
    xml2.setAttribute("Lowpass Q", lpQ);
    xml2.setAttribute("Highpass Freq.", hpFreq);
    xml2.setAttribute("Highpass Q", hpQ);
    xml2.setAttribute("Lowshelf Freq.", lsFreq);
    xml2.setAttribute("Lowshelf G", lsG);
    xml2.setAttribute("Highshelf Freq.", hsFreq);
    xml2.setAttribute("Highshelf G", hsG);
    xml2.setAttribute("Peak Freq.", pFreq);
    xml2.setAttribute("Peak Q", pQ);
    xml2.setAttribute("Peak G", pG);

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml2, destData);*/

}

void EqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    
    std::unique_ptr<XmlElement> params (getXmlFromBinary(data, sizeInBytes));
    if (params.get() != nullptr)
        if (params->hasTagName(treeState.state.getType()))
            treeState.replaceState(ValueTree::fromXml(*params));
    
    lpFreq = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
    lpQ = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
    numpases = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
    
    hpFreq = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
    hpQ = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
    numpases2 = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
    
    lsFreq = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
    lsG = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
    
    hsFreq = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
    hsG = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
    
    pFreq = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
    pG = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
    pQ = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();

    
    /*ScopedPointer<XmlElement> presets (getXmlFromBinary (data, sizeInBytes));
    
    if (presets != 0)
    {
        // make sure that it's actually our type of XML object..
        if (presets->hasTagName ("EQPRESETS"))
        {
            lpFreq  = (float) presets->getDoubleAttribute ("Lowpass Freq.", lpFreq);
            lpQ = (float) presets->getDoubleAttribute ("Lowpass Q", lpQ);
            
            hpFreq  = (float) presets->getDoubleAttribute ("Highpass Freq.", hpFreq);
            hpQ = (float) presets->getDoubleAttribute ("Highpass Q", hpQ);
            
            lsFreq  = (float) presets->getDoubleAttribute ("Lowshelf Freq.", lsFreq);
            lsG = (float) presets->getDoubleAttribute ("Lowshelf G", lsG);
            
            hsFreq  = (float) presets->getDoubleAttribute ("Highahelf Freq.", hsFreq);
            hsG = (float) presets->getDoubleAttribute ("Highahelf G", hsG);

            pFreq  = (float) presets->getDoubleAttribute ("Peak Freq.", pFreq);
            pG = (float) presets->getDoubleAttribute ("Peak G", pG);
            pQ = (float) presets->getDoubleAttribute ("Peak Q", pQ);
        }
    }*/
    

}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EqAudioProcessor();
}
