/*
  ==============================================================================

    Biquad.h
    Created: 30 Apr 2018 4:28:05pm
    Author:  Alberto García Ruíz

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#ifndef Biquad_h
#define Biquad_h

enum {
    lowpass = 0,
    highpass,
    peak,
    lowshelf,
    highshelf
};

class Biquad {
public:
    Biquad();
    Biquad(int type, double Fc, double Q, double peakGainDB);
    ~Biquad();
    void setType(int type);
    void setQ(double Q);
    void setFc(double Fc);
    void setPeakGain(double peakGainDB);
    void setBiquad(int type, double Fc, double Q, double peakGain);
    float process(float currentSample);
protected:
    void calcBiquad(void);
    /*float delayedSample1;
    float delayedSample2;
    float delayedOut1;
    float delayedOut2;*/
    int type;
    float a0, a1, a2, b0, b1, b2;
    //float A0, A1, A2, B1, B2;
    double Fc, Q, peakGain;
    //float d1, d2;
    float z1, z2;
};

inline float Biquad::process(float currentSample) {
    
    double out = currentSample * a0 + z1;
    z1 = currentSample * a1 + z2 - b1 * out;
    z2 = currentSample * a2 - b2 * out;
    return out;
     
}

#endif // Biquad_h
