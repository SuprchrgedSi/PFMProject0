/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PFMProject0AudioProcessorEditor::PFMProject0AudioProcessorEditor (PFMProject0AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    update();
    setSize (400, 300);

    startTimerHz(20);
}

PFMProject0AudioProcessorEditor::~PFMProject0AudioProcessorEditor()
{
    PFMProject0AudioProcessor::updateAutomatableParameter(audioProcessor.shouldPlaySound, false);
}

void PFMProject0AudioProcessorEditor::update()
{
    cachedBgColor = audioProcessor.bgColor->get();
}

void PFMProject0AudioProcessorEditor::timerCallback()
{
    update();
    repaint();
}

//==============================================================================
void PFMProject0AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId).interpolatedWith(juce::Colours::red, cachedBgColor));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void PFMProject0AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void PFMProject0AudioProcessorEditor::mouseDown(const juce::MouseEvent& e) 
{
    lastClickPosition = e.getPosition();
}

void PFMProject0AudioProcessorEditor::mouseUp(const juce::MouseEvent& e)
{
    //PFMProject0AudioProcessor::updateAutomatableParameter(audioProcessor.shouldPlaySound, !audioProcessor.shouldPlaySound->get());
}

void PFMProject0AudioProcessorEditor::mouseDrag(const juce::MouseEvent& e)
{
    auto clickPos = e.getPosition();

    auto difY = juce::jlimit(-1.0, 1.0, -(clickPos.y - lastClickPosition.y) / 200.0);

    difY = juce::jmap(difY, -1.0, 1.0, 0.0, 1.0);

    DBG( "difY: " << difY);
    
    PFMProject0AudioProcessor::updateAutomatableParameter(audioProcessor.bgColor, difY);
    update();

    repaint();

    
}
