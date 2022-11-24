#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible (textEditor);
    textEditor.setColour (juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
    textEditor.setColour (juce::TextEditor::backgroundColourId, juce::Colours::black);
    textEditor.setMultiLine (true);
    textEditor.setReadOnly (true);
    textEditor.setCaretVisible (false);
    textEditor.setPopupMenuEnabled (false);
    textEditor.setLineSpacing (1.1f);
    textEditor.setJustification (juce::Justification::centredLeft);
    textEditor.setFont (juce::Font (15.0f));
    textEditor.setColour (juce::TextEditor::textColourId, juce::Colours::red);
    textEditor.insertTextAtCaret ("Left channel: non-oversampled sawtooth - audible & visible aliasing\n");
    textEditor.setColour (juce::TextEditor::textColourId, juce::Colours::green);
    textEditor.insertTextAtCaret ("Right channel: oversampled sawtooth - inaudible aliasing");
    
    addAndMakeVisible (fftScope);
    fftScope.assignFftProcessor (&fftProcessor);
    fftScope.setFreqMin (1000.0f);

    // Make sure you set the size of the component after you add any child components.
    setSize (800, 450);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    const auto numChannels = deviceManager.getCurrentAudioDevice()->getActiveOutputChannels().countNumberOfSetBits();
    const juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlockExpected), static_cast<juce::uint32> (numChannels) };
    const juce::dsp::ProcessSpec specOs { sampleRate * oversamplingFactor, static_cast<juce::uint32> (samplesPerBlockExpected * oversamplingFactor), static_cast<juce::uint32> (numChannels) };
    sampleRate = spec.sampleRate;
    maxBlockSize = spec.maximumBlockSize;
    
    if (spec.numChannels > 0)
    {
        hiir.prepare (oversamplingFactor, numChannels, samplesPerBlockExpected);

        fftProcessor.prepare (spec);
        fftScope.prepare (spec);

        const auto nyquist = static_cast<float> (round (sampleRate / 2.0));
        sweepStartFrequency = nyquist * 0.25f;
        sweepEndFrequency = nyquist;
        calculateNumSweepSteps();
        sweepStepIndex = 0;
        sweepStepDelta = 1;
        oscillator1.setFrequency (static_cast<float> (currentFrequency));
        oscillator1.prepare (spec);
        oscillator2.setFrequency (static_cast<float> (currentFrequency));
        oscillator2.prepare (specOs);
    }
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    const auto numChannels = bufferToFill.buffer->getNumChannels();
    const auto numSamples = bufferToFill.numSamples;
    
    oscillator1.setFrequency (static_cast<float> (getSweepFrequency()));
    oscillator2.setFrequency (static_cast<float> (getSweepFrequency()));

    // Upsample input before we change the buffer content
    // Note that we're going to replace this with an oscillator source anyhow, but if you wanted to process the input, this is how you'd do it!
    hiir.setSize (numSamples);
    hiir.upSample (*bufferToFill.buffer);

    // Generate oscillator1 into both channels of non-oversampled block
    juce::dsp::AudioBlock<float> outputBlock (*bufferToFill.buffer, static_cast<size_t>(bufferToFill.startSample));
    const auto context = juce::dsp::ProcessContextReplacing<float> (outputBlock);
    oscillator1.process (context);

    // Generate oscillator2 into both channels of the oversampled block
    const auto numSamplesOS = hiir.getOversampledSize();
    juce::dsp::AudioBlock<float> audioBlockOS (hiir.getOverSampledData(), numChannels, numSamplesOS);
    const auto contextOs = juce::dsp::ProcessContextReplacing<float> (audioBlockOS);
    oscillator2.process (contextOs);

    // Downsample oscillator2 into the right channel of the non-oversampled block
    if (numChannels > 1) {
        hiir.downSample (1, outputBlock.getChannelPointer (1));
    }
    
    // Step the frequency sweep
    if (sweepStepIndex >= numSweepSteps) sweepStepDelta = -1;
    else if (sweepStepIndex <= 0) sweepStepDelta = 1;
    sweepStepIndex += sweepStepDelta;
    
    // Perform FFT for scope
    for (size_t ch = 0; ch < outputBlock.getNumChannels(); ++ch)
    {
        const auto chNum = static_cast<int> (ch);
        const auto* audioData = outputBlock.getChannelPointer (ch);
        fftProcessor.appendData (chNum, numSamples, audioData);
    }
    
    // Reduce the gain of the output to protect our ears!
    const auto gain = juce::Decibels::decibelsToGain<float> (-30.0f);
    outputBlock.multiplyBy (gain);
}

void MainComponent::releaseResources()
{
    hiir.release();
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    auto fftBounds = getLocalBounds().reduced (20);
    const auto txtBounds = fftBounds.removeFromTop (60);
    textEditor.setBounds (txtBounds);
    fftScope.setBounds (fftBounds);
}

double MainComponent::getSweepFrequency() const
{
    //f(x) = 10^(log(span)/n*x) + fStart
    //where:
    //    x = the number of the sweep point
    //    n = total number of sweep points
    const auto span = sweepEndFrequency - sweepStartFrequency;
    return pow (10, log10 (span) / numSweepSteps * sweepStepIndex) + sweepStartFrequency;
}

void MainComponent::calculateNumSweepSteps()
{
    numSweepSteps = static_cast<long> (sweepDuration * sampleRate / static_cast<double> (maxBlockSize));
}
