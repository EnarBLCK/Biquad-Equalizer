#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Biquad.h"


//==============================================================================
/**
*/

class OtherLookAndFeel  : public LookAndFeel_V4
{
    public:
    OtherLookAndFeel()
    {
        setColour(Slider::rotarySliderOutlineColourId, Colours::ghostwhite);
        setColour(Slider::rotarySliderFillColourId, Colours::darkorchid);
        setColour(Slider::thumbColourId, Colours::ghostwhite);
    }

    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        auto radius = jmin (width / 2, height / 2) - 4.0f;
        auto centreX = x + width  * 0.5f;
        auto centreY = y + height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        
        // fill
        g.setColour (Colours::darkorchid);
        g.fillEllipse (rx, ry, rw, rw);
        
        // outline
        g.setColour (Colours::ghostwhite);
        g.drawEllipse (rx, ry, rw, rw, 1.0f);
        
        Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 2.0f;
        p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform (AffineTransform::rotation (angle).translated (centreX, centreY));
        
        // pointer
        g.setColour (Colours::ghostwhite);
        g.fillPath (p);
    }
};


class EqAudioProcessorEditor  : public AudioProcessorEditor,
                                public Slider::Listener,
                                public ComboBox::Listener
{
public:
    EqAudioProcessorEditor (EqAudioProcessor&);
    ~EqAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider *slider) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> lpFreqAttach;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> hpFreqAttach;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> lsFreqAttach;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> hsFreqAttach;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> pFreqAttach;
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> lpQAttach;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> hpQAttach;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> pQAttach;
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> lsGAttach;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> hsGAttach;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> pGAttach;
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> inVolAttach;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> outVolAttach;


private:

    EqAudioProcessor& processor;
    
    OtherLookAndFeel otherLookAndFeel;
    
    // Lowpass slider
    Slider lpFreqSlider;
    Label lpTypeLabel;
    Slider lpQSlider;
    
    // Highpass slider
    Slider hpFreqSlider;
    Label hpTypeLabel;
    Slider hpQSlider;
    
    // LowShelf slider
    Slider lsFreqSlider;
    Label lsTypeLabel;
    Slider lsGSlider;
    
    // HighShelf slider
    Slider hsFreqSlider;
    Label hsTypeLabel;
    Slider hsGSlider;
    
    // Peak slider
    Slider pFreqSlider;
    Label pTypeLabel;
    Slider pGSlider;
    Slider pQSlider;
    
    // Freq, Q and Gain labels
    Label freqLabel1;
    Label freqLabel2;
    Label freqLabel3;
    Label freqLabel4;
    Label freqLabel5;

    Label qLabel1;
    Label qLabel2;
    Label qLabel3;
    
    Label gLabel1;
    Label gLabel2;
    Label gLabel3;
    
    // Slope combobox
    ComboBox slope1;
    ComboBox slope2;
    Label slopeLabel1;
    Label slopeLabel2;
    
    
    // I/O Vol Gains
    Slider inVolSlider;
    Label inVolLabel;
    Slider outVolSlider;
    Label outVolLabel;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqAudioProcessorEditor)
};
