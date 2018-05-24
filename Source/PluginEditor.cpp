#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EqAudioProcessorEditor::EqAudioProcessorEditor (EqAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{

    
    lpFreqAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, LPFREQ_ID, lpFreqSlider);
    hpFreqAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, HPFREQ_ID, hpFreqSlider);
    lsFreqAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, LSFREQ_ID, lsFreqSlider);
    hsFreqAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, HSFREQ_ID, hsFreqSlider);
    pFreqAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, PFREQ_ID, pFreqSlider);
    
    lpQAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, LPQ_ID, lpQSlider);
    hpQAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, HPQ_ID, hpQSlider);
    pQAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, PQ_ID, pQSlider);
    
    lsGAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, LSG_ID, lsGSlider);
    hsGAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, HSG_ID, hsGSlider);
    pGAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, PG_ID, pGSlider);
    
    inVolAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, IV_ID, inVolSlider);
    outVolAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, OV_ID, outVolSlider);

    
    // I/O Gain Sliders & label
    addAndMakeVisible(inVolSlider);
    inVolSlider.setTextValueSuffix(" dB");
    inVolSlider.addListener(this);
    
    addAndMakeVisible(inVolLabel);
    inVolLabel.setText("Input Gain", dontSendNotification);
    inVolLabel.attachToComponent(&inVolSlider, true);
    inVolLabel.setColour(Label::textColourId, Colours::antiquewhite);
    inVolLabel.setJustificationType(Justification::centred);
    
    addAndMakeVisible(outVolSlider);
    outVolSlider.setTextValueSuffix(" dB");
    outVolSlider.addListener(this);
    
    addAndMakeVisible(outVolLabel);
    outVolLabel.setText("Output Gain", dontSendNotification);
    outVolLabel.attachToComponent(&outVolSlider, true);
    outVolLabel.setColour(Label::textColourId, Colours::antiquewhite);
    outVolLabel.setJustificationType(Justification::centred);
    
    
    
    
    // LP Freq slider & label settings
    addAndMakeVisible(lpFreqSlider); // Freq slider
    lpFreqSlider.setTextValueSuffix (" Hz");
    lpFreqSlider.addListener(this);
    
    addAndMakeVisible (lpTypeLabel); // Freq label
    lpTypeLabel.setText ("Low Pass", dontSendNotification);
    lpTypeLabel.attachToComponent(&lpFreqSlider, true);
    lpTypeLabel.setColour(Label::textColourId, Colours::black);
    lpTypeLabel.setJustificationType(Justification::centred);
    
    // LP Q slider & label settings
    addAndMakeVisible(lpQSlider); // Q slider
    lpQSlider.addListener(this);
    
    
    
    
    // HP Freq slider & label settings
    addAndMakeVisible(hpFreqSlider); // Freq slider
    hpFreqSlider.setTextValueSuffix (" Hz");
    hpFreqSlider.addListener(this);
    
    addAndMakeVisible (hpTypeLabel); // Freq label
    hpTypeLabel.setText ("High Pass", dontSendNotification);
    hpTypeLabel.attachToComponent (&hpFreqSlider, true);
    hpTypeLabel.setColour(Label::textColourId, Colours::black);
    hpTypeLabel.setJustificationType(Justification::centred);
    
    // HP Q slider & label settings
    addAndMakeVisible(hpQSlider); // Q slider
    hpQSlider.addListener(this);
    
    
    
    
    // LS Freq slider & label settings
    addAndMakeVisible(lsFreqSlider); // Freq slider
    lsFreqSlider.setTextValueSuffix (" Hz");
    lsFreqSlider.addListener(this);
    
    addAndMakeVisible (lsTypeLabel); // Freq label
    lsTypeLabel.setText ("Low Shelf", dontSendNotification);
    lsTypeLabel.attachToComponent (&lsFreqSlider, true);
    lsTypeLabel.setColour(Label::textColourId, Colours::black);
    lsTypeLabel.setJustificationType(Justification::centred);
    
    // LS G slider & label settings
    addAndMakeVisible(lsGSlider); // Q slider
    lsGSlider.addListener(this);
    
    
    
    
    // HS Freq slider & label settings
    addAndMakeVisible(hsFreqSlider); // Freq slider
    hsFreqSlider.setTextValueSuffix (" Hz");
    hsFreqSlider.addListener(this);
    
    addAndMakeVisible (hsTypeLabel); // Freq label
    hsTypeLabel.setText ("High Shelf", dontSendNotification);
    hsTypeLabel.attachToComponent (&hsFreqSlider, true);
    hsTypeLabel.setColour(Label::textColourId, Colours::black);
    hsTypeLabel.setJustificationType(Justification::centred);
    
    // HS G slider & label settings
    addAndMakeVisible(hsGSlider); // Q slider
    hsGSlider.addListener(this);
    
    
    
    // Peak Freq slider & label settings
    addAndMakeVisible(pFreqSlider); // Freq slider
    pFreqSlider.setTextValueSuffix (" Hz");
    pFreqSlider.addListener(this);
    
    addAndMakeVisible (pTypeLabel); // Freq label
    pTypeLabel.setText ("Peak", dontSendNotification);
    pTypeLabel.attachToComponent (&pFreqSlider, true);
    pTypeLabel.setColour(Label::textColourId, Colours::black);
    pTypeLabel.setJustificationType(Justification::centred);
    
    // Peak G slider & label settings
    addAndMakeVisible(pGSlider); // Q slider
    pGSlider.addListener(this);
    
    // Peak Q slider & label settings
    addAndMakeVisible(pQSlider); // Q slider
    pQSlider.addListener(this);
    
    
    
    // General Freq. Gain & Q labels
    addAndMakeVisible (freqLabel1); // Freq label
    freqLabel1.setText ("Frequency", dontSendNotification);
    freqLabel1.setColour(Label::textColourId, Colours::antiquewhite);
    freqLabel1.setJustificationType(Justification::centred);
    addAndMakeVisible (freqLabel2); // Freq label
    freqLabel2.setText ("Frequency", dontSendNotification);
    freqLabel2.setColour(Label::textColourId, Colours::antiquewhite);
    freqLabel2.setJustificationType(Justification::centred);
    addAndMakeVisible (freqLabel3); // Freq label
    freqLabel3.setText ("Frequency", dontSendNotification);
    freqLabel3.setColour(Label::textColourId, Colours::antiquewhite);
    freqLabel3.setJustificationType(Justification::centred);
    addAndMakeVisible (freqLabel4); // Freq label
    freqLabel4.setText ("Frequency", dontSendNotification);
    freqLabel4.setColour(Label::textColourId, Colours::antiquewhite);
    freqLabel4.setJustificationType(Justification::centred);
    addAndMakeVisible (freqLabel5); // Freq label
    freqLabel5.setText ("Frequency", dontSendNotification);
    freqLabel5.setColour(Label::textColourId, Colours::antiquewhite);
    freqLabel5.setJustificationType(Justification::centred);
    
    
    addAndMakeVisible (qLabel1); // Freq label
    qLabel1.setText ("Q Factor", dontSendNotification);
    qLabel1.setColour(Label::textColourId, Colours::antiquewhite);
    qLabel1.setJustificationType(Justification::centred);
    addAndMakeVisible (qLabel2); // Freq label
    qLabel2.setText ("Q Factor", dontSendNotification);
    qLabel2.setColour(Label::textColourId, Colours::antiquewhite);
    qLabel2.setJustificationType(Justification::centred);
    addAndMakeVisible (qLabel3); // Freq label
    qLabel3.setText ("Q Factor", dontSendNotification);
    qLabel3.setColour(Label::textColourId, Colours::antiquewhite);
    qLabel3.setJustificationType(Justification::centred);
    
    addAndMakeVisible (gLabel1); // Freq label
    gLabel1.setText ("Gain", dontSendNotification);
    gLabel1.setColour(Label::textColourId, Colours::antiquewhite);
    gLabel1.setJustificationType(Justification::centred);
    addAndMakeVisible (gLabel2); // Freq label
    gLabel2.setText ("Gain", dontSendNotification);
    gLabel2.setColour(Label::textColourId, Colours::antiquewhite);
    gLabel2.setJustificationType(Justification::centred);
    addAndMakeVisible (gLabel3); // Freq label
    gLabel3.setText ("Gain", dontSendNotification);
    gLabel3.setColour(Label::textColourId, Colours::antiquewhite);
    gLabel3.setJustificationType(Justification::centred);
    
    
    
    // Slider style
    setLookAndFeel(&otherLookAndFeel);
    
    lpFreqSlider.setSliderStyle(Slider::Rotary);
    lpQSlider.setSliderStyle(Slider::Rotary);
    hpFreqSlider.setSliderStyle(Slider::Rotary);
    hpQSlider.setSliderStyle(Slider::Rotary);
    lsFreqSlider.setSliderStyle(Slider::Rotary);
    lsGSlider.setSliderStyle(Slider::Rotary);
    hsFreqSlider.setSliderStyle(Slider::Rotary);
    hsGSlider.setSliderStyle(Slider::Rotary);
    pFreqSlider.setSliderStyle(Slider::Rotary);
    pGSlider.setSliderStyle(Slider::Rotary);
    pQSlider.setSliderStyle(Slider::Rotary);
    inVolSlider.setSliderStyle(Slider::LinearVertical);
    outVolSlider.setSliderStyle(Slider::LinearVertical);

    
    
    // ComboBox slope
    addAndMakeVisible(slope1);
    slope1.addItem("12 dB/octave", 1);
    slope1.addItem("24 dB/octave", 2);
    slope1.addItem("48 dB/octave", 3);
    slope1.setSelectedId(1);
    slope1.addListener(this);
    
    addAndMakeVisible(slope2);
    slope2.addItem("12 dB/octave", 1);
    slope2.addItem("24 dB/octave", 2);
    slope2.addItem("48 dB/octave", 3);
    slope2.setSelectedId(1);
    slope2.addListener(this);
    
    addAndMakeVisible (slopeLabel1); // LP Slope Label
    slopeLabel1.setText ("LP Slope:", dontSendNotification);
    slopeLabel1.setColour(Label::textColourId, Colours::antiquewhite);
    slopeLabel1.setJustificationType(Justification::centred);
    slopeLabel1.attachToComponent(&slope1, true);
    
    addAndMakeVisible (slopeLabel2); // HP Slope Label
    slopeLabel2.setText ("HP Slope:", dontSendNotification);
    slopeLabel2.setColour(Label::textColourId, Colours::antiquewhite);
    slopeLabel2.setJustificationType(Justification::centred);
    slopeLabel2.attachToComponent(&slope2, true);
    
    
    // Text box dim
    lpFreqSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    lpQSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    hpFreqSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    hpQSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    lsFreqSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    lsGSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    hsFreqSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    hsGSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    pFreqSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    pGSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    pQSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    inVolSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    outVolSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    
    
    
    // Change of slider scale
    lpFreqSlider.setSkewFactorFromMidPoint (2000);
    hpFreqSlider.setSkewFactorFromMidPoint (2000);
    lsFreqSlider.setSkewFactorFromMidPoint (2000);
    hsFreqSlider.setSkewFactorFromMidPoint (2000);
    pFreqSlider.setSkewFactorFromMidPoint (2000);
    
    
    
    // Slider colour
    lpFreqSlider.setLookAndFeel(&otherLookAndFeel);
    lpQSlider.setLookAndFeel(&otherLookAndFeel);
    hpFreqSlider.setLookAndFeel(&otherLookAndFeel);
    hpQSlider.setLookAndFeel(&otherLookAndFeel);
    lsFreqSlider.setLookAndFeel(&otherLookAndFeel);
    lsGSlider.setLookAndFeel(&otherLookAndFeel);
    hsFreqSlider.setLookAndFeel(&otherLookAndFeel);
    hsGSlider.setLookAndFeel(&otherLookAndFeel);
    pFreqSlider.setLookAndFeel(&otherLookAndFeel);
    pGSlider.setLookAndFeel(&otherLookAndFeel);
    pQSlider.setLookAndFeel(&otherLookAndFeel);
    inVolSlider.setLookAndFeel(&otherLookAndFeel);
    outVolSlider.setLookAndFeel(&otherLookAndFeel);

    
    setSize (800, 400);


}

EqAudioProcessorEditor::~EqAudioProcessorEditor()
{
   
    lpFreqSlider.removeListener(this);
    hpFreqSlider.removeListener(this);
    lsFreqSlider.removeListener(this);
    hsFreqSlider.removeListener(this);
    pFreqSlider.removeListener(this);
    lpQSlider.removeListener(this);
    hpQSlider.removeListener(this);
    pQSlider.removeListener(this);
    hsGSlider.removeListener(this);
    lsGSlider.removeListener(this);
    pGSlider.removeListener(this);
    inVolSlider.removeListener(this);
    outVolSlider.removeListener(this);
    
    lpFreqAttach.~ScopedPointer();
    hpFreqAttach.~ScopedPointer();
    lsFreqAttach.~ScopedPointer();
    hsFreqAttach.~ScopedPointer();
    pFreqAttach.~ScopedPointer();
    lpQAttach.~ScopedPointer();
    hpQAttach.~ScopedPointer();
    pQAttach.~ScopedPointer();
    lsGAttach.~ScopedPointer();
    hsGAttach.~ScopedPointer();
    pGAttach.~ScopedPointer();
    inVolAttach.~ScopedPointer();
    outVolAttach.~ScopedPointer();
    
}

//==============================================================================
void EqAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::lightgrey);
    g.setFont (15.0f);
    
    
    auto centralArea = getLocalBounds().toFloat().reduced (10.0f);
    g.drawRoundedRectangle (centralArea, 5.0f, 3.0f);
    Array<Colour> colours { Colours::beige, Colours::beige, Colours::beige, Colours::beige, Colours::beige, Colours::beige, Colours::beige };
    auto colourBarArea = centralArea.reduced (4.0f).withHeight (20.0f);
    auto colourArea = colourBarArea.withWidth (colourBarArea.getWidth() / colours.size());
    for (auto colour : colours)
    {
        g.setColour (colour);
        g.fillRect (colourArea);
        colourArea.translate (colourArea.getWidth(), 0.0f);
    }

}

void EqAudioProcessorEditor::resized()
{
    
    const int border = 5;
    const int border2 = 50;
    const int freqSliderWidth = getWidth()/4 - (border2/2);
    const int freqSliderHeight = getHeight()/2 - (border2/2) - 100;
    
    hpFreqSlider.setBounds(border+80, border2, freqSliderWidth, freqSliderHeight);
    freqLabel1.setBounds(border+80, (getHeight()/4), freqSliderWidth, freqSliderHeight);
    hpTypeLabel.setBounds(border+80,border-20,freqSliderWidth, freqSliderHeight);
    hpQSlider.setBounds(border+80, (getHeight()/4)+70, freqSliderWidth, freqSliderHeight);
    qLabel1.setBounds(border+80, (getHeight()/4)+120, freqSliderWidth, freqSliderHeight);
    slope2.setBounds(border+120, (getHeight()/2)+120, freqSliderWidth/2, freqSliderHeight/2);


    
    lsFreqSlider.setBounds((getWidth()/7)+80, border2, freqSliderWidth, freqSliderHeight);
    freqLabel2.setBounds((getWidth()/7)+80, (getHeight()/4), freqSliderWidth, freqSliderHeight);
    lsTypeLabel.setBounds((getWidth()/7)+80,border-20,freqSliderWidth, freqSliderHeight);
    lsGSlider.setBounds((getWidth()/7)+80, (getHeight()/4)+70, freqSliderWidth, freqSliderHeight);
    gLabel1.setBounds((getWidth()/7)+80, (getHeight()/4)+120, freqSliderWidth, freqSliderHeight);

    
    pFreqSlider.setBounds((getWidth()/3.6)+80, border2, freqSliderWidth, freqSliderHeight);
    freqLabel3.setBounds((getWidth()/3.6)+80, (getHeight()/4), freqSliderWidth, freqSliderHeight);
    pTypeLabel.setBounds((getWidth()/3.6)+80,border-20,freqSliderWidth, freqSliderHeight);
    pGSlider.setBounds((getWidth()/3.6)+80, (getHeight()/2)+90, freqSliderWidth, freqSliderHeight);
    gLabel2.setBounds((getWidth()/3.6)+80, (getHeight()/2)+140, freqSliderWidth, freqSliderHeight);
    pQSlider.setBounds((getWidth()/3.6)+80, (getHeight()/4)+70, freqSliderWidth, freqSliderHeight);
    qLabel2.setBounds((getWidth()/3.6)+80, (getHeight()/4)+120, freqSliderWidth, freqSliderHeight);

    
    hsFreqSlider.setBounds((getWidth()/2.4)+80, border2, freqSliderWidth, freqSliderHeight);
    freqLabel4.setBounds((getWidth()/2.4)+80, (getHeight()/4), freqSliderWidth, freqSliderHeight);
    hsTypeLabel.setBounds((getWidth()/2.4)+80,border-20,freqSliderWidth, freqSliderHeight);
    hsGSlider.setBounds((getWidth()/2.4)+80, (getHeight()/4)+70, freqSliderWidth, freqSliderHeight);
    gLabel3.setBounds((getWidth()/2.4)+80, (getHeight()/4)+120, freqSliderWidth, freqSliderHeight);
    
    
    lpFreqSlider.setBounds((getWidth()/1.8)+80, border2, freqSliderWidth, freqSliderHeight);
    freqLabel5.setBounds((getWidth()/1.8)+80, (getHeight()/4), freqSliderWidth, freqSliderHeight);
    lpTypeLabel.setBounds((getWidth()/1.8)+80,border-20,freqSliderWidth, freqSliderHeight);
    lpQSlider.setBounds((getWidth()/1.8)+80, (getHeight()/4)+70, freqSliderWidth, freqSliderHeight);
    qLabel3.setBounds((getWidth()/1.8)+80, (getHeight()/4)+120, freqSliderWidth, freqSliderHeight);
    slope1.setBounds((getWidth()/1.8)+120, (getHeight()/2)+120, freqSliderWidth/2, freqSliderHeight/2);
    
    
    inVolSlider.setBounds(border-120, border2+15, freqSliderWidth*2, freqSliderHeight*3);
    inVolLabel.setBounds(border-120, border2-100, freqSliderWidth*2, freqSliderHeight*3);
    outVolSlider.setBounds(border2+510, border2+10, freqSliderWidth*2, freqSliderHeight*3);
    outVolLabel.setBounds(border2+510, border2-100, freqSliderWidth*2, freqSliderHeight*3);


    
}

void EqAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    if(slider == &lpFreqSlider)
    {
        processor.lpFreq = lpFreqSlider.getValue();
    }
    
    if(slider == &lpQSlider)
    {
        processor.lpQ = lpQSlider.getValue();
    }
    
    
    
    if(slider == &hpFreqSlider)
    {
        processor.hpFreq = hpFreqSlider.getValue();
    }
    
    if(slider == &lpQSlider)
    {
        processor.hpQ = hpQSlider.getValue();
    }
    
    
    
    if(slider == &lsFreqSlider)
    {
        processor.lsFreq = lsFreqSlider.getValue();
    }
    
    if(slider == &lsGSlider)
    {
        processor.lsG = lsGSlider.getValue();
    }
    
    
    
    if(slider == &hsFreqSlider)
    {
        processor.hsFreq = hsFreqSlider.getValue();
    }
    
    if(slider == &hsGSlider)
    {
        processor.hsG = hsGSlider.getValue();
    }
    
    
    
    if(slider == &pFreqSlider)
    {
        processor.pFreq = pFreqSlider.getValue();
    }
    
    if(slider == &lpQSlider)
    {
        processor.pQ = pQSlider.getValue();
    }
    
    if(slider == &pGSlider)
    {
        processor.pG = pGSlider.getValue();
    }
    
    
    if(slider == &inVolSlider)
    {
        processor.inputVolume = inVolSlider.getValue();
    }
    
    if(slider == &outVolSlider)
    {
        processor.outputVolume = outVolSlider.getValue();
    }
    
    
}

void EqAudioProcessorEditor::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    if(comboBoxThatHasChanged == &slope1)
        switch (slope1.getSelectedId())
        {
            case 1: processor.numpases = 4;  break;
            case 2: processor.numpases = 5;   break;
            case 3: processor.numpases = 6; break;
        }
    
    if(comboBoxThatHasChanged == &slope2)
        switch (slope2.getSelectedId())
    {
        case 1: processor.numpases2 = 4;  break;
        case 2: processor.numpases2 = 5;   break;
        case 3: processor.numpases2 = 6; break;
    }
}



