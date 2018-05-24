# Biquad-Equalizer
Biquad equalizer plugin developed by UPF student, Alberto García Ruiz.

<b>WHAT IS THIS</b>

Biquad filter plugin developed with Juce (C++ coded). Provides AU and VST demo plugin. Three main classes: pluginProcessor, pluginEditor, biquad.

- pluginProcessor: process the input audio from the buffer through the biquad filter methods. 
- pluginEditor: created GUI knobs, sliders and dropdown menus.
- biquad: filters function from http://www.earlevel.com/main/2012/11/26/biquad-c-source-code/. Provided low pass, low shelf, peak, high shelf and high pass filters. Realization using Direct Form II.

<b>PLUGIN SPECS</b>

- 5 freq. bands: high pass, low shelf, peak, high shelf and low pass.
- 5 freq. knobs, 3 Q factor knobs (for hp, lp & peak), 3 filters gain knobs (for ls, hs & peak), 2 slope dropdown menus (for hp & lp) and master gain slider.
