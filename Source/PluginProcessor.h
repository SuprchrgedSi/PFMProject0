/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <array>
//==============================================================================
/**
*/

struct BufferAnalyzer : juce::Thread, juce::Timer, juce::Component
{
    BufferAnalyzer();
    ~BufferAnalyzer();
    void prepareBuffer(double sampleRate, int samplesPerBlock);
    void cloneBuffer(const juce::dsp::AudioBlock<float>& other);
    void run() override;
    void timerCallback() override;
    void paint(juce::Graphics& g) override;
private:
    std::array<juce::AudioBuffer<float>, 2> buffers;
    juce::Atomic<bool> firstBuffer{ true };
    std::array<juce::Atomic<size_t>, 2> samplesCopied;

    juce::Path fftCurve;

    enum
    {
        fftOrder = 11,
        fftSize = 1 << fftOrder,
        numPoints = 512
    };

    float fifoBuffer[fftSize];        
    float fftData[2 * fftSize]; 
    int fifoIndex = 0;          

    juce::Atomic<bool> nextFFTBlockReady = false;
    void pushNextSampleIntoFifo(float);
    float curveData[numPoints];

    juce::dsp::FFT forwardFFT{ fftOrder };
    juce::dsp::WindowingFunction<float> window{ fftSize,
                                        juce::dsp::WindowingFunction<float>::hann };
    void drawNextFrameOfSpectrum();


};

//==============================================================================
class PFMProject0AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    PFMProject0AudioProcessor();
    ~PFMProject0AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioParameterBool* shouldPlaySound = nullptr;
    juce::AudioParameterFloat* bgColor = nullptr;

    static void updateAutomatableParameter(juce::RangedAudioParameter*, float value);
    
    BufferAnalyzer leftBufferAnalyzer, rightBufferAnalyzer;

private:
    juce::AudioProcessorValueTreeState apvts;
    juce::Random r;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PFMProject0AudioProcessor)
};
