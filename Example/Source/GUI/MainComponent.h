#pragma once

#include <JuceHeader.h>
#include "../../../Source/ob_HIIR.h"
#include "../Processing/PolyBLEP.h"
#include "../GUI/FftScope.h"
#include "../Processing/FftProcessor.h"

class MainComponent  : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    juce::TextEditor textEditor;
    
    const int oversamplingFactor = 4;
    ob::HIIR hiir;
    
    FftProcessor<10> fftProcessor;
    FftScope<10> fftScope;

    juce::dsp::PolyBlepOscillator<float> oscillator1 {juce::dsp::PolyBlepOscillator<float>::saw};
    juce::dsp::PolyBlepOscillator<float> oscillator2 {juce::dsp::PolyBlepOscillator<float>::saw};
    double currentFrequency = 440.0;
    long numSweepSteps = 0;
    long sweepStepIndex = 0;
    int sweepStepDelta = 1;
    double sweepStartFrequency = 1.0;
    double sweepEndFrequency = 22050.0;
    double sweepDuration = 5.0;
    double sampleRate = 44100.0;
    juce::uint32 maxBlockSize = 0;
    double getSweepFrequency() const;
    void calculateNumSweepSteps();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
