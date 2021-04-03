#include "PluginProcessor.h"

AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    AudioProcessorValueTreeState::ParameterLayout params (
        std::make_unique<AudioParameterFloat>(
            "MainGain",
            "Gain",
            NormalisableRange<float>(0.0, 1.0),
            0.8,
            String(),
            AudioProcessorParameter::genericParameter,
            [](float value, int /* maxLength */) {
                return String(Decibels::gainToDecibels(value), 1) + "dB";
            },
            nullptr
        ),
        std::make_unique<AudioParameterBool>(
            "MainMute",
            "Mute",
            false
       )
    );

    return params;
}

//==============================================================================
ChordProcessor::ChordProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                       .withOutput ("Output", AudioChannelSet::stereo(), true)),
       params(*this, nullptr, JucePlugin_Name, createParameterLayout())
{
}

ChordProcessor::~ChordProcessor()
{
    stopTimer();
}

//==============================================================================
const String ChordProcessor::getName() const {
    return JucePlugin_Name;
}

bool ChordProcessor::acceptsMidi() const {
    #if JucePlugin_WantsMidiInput
        return true;
    #else
        return false;
    #endif
}

bool ChordProcessor::producesMidi() const {
    #if JucePlugin_ProducesMidiOutput
        return true;
    #else
        return false;
    #endif
}

bool ChordProcessor::isMidiEffect() const {
    #if JucePlugin_IsMidiEffect
        return true;
    #else
        return false;
    #endif
}

double ChordProcessor::getTailLengthSeconds() const { return 0.0; }
int ChordProcessor::getNumPrograms() { return 1; }
int ChordProcessor::getCurrentProgram() { return 0; }
void ChordProcessor::setCurrentProgram (int /* index */) {}
const String ChordProcessor::getProgramName (int /* index */) { return {}; }
void ChordProcessor::changeProgramName (int /* index */, const String& /* newName */) {}

//==============================================================================
void ChordProcessor::prepareToPlay(double sampleRate, int /* samplesPerBlock */) {}
void ChordProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ChordProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const {
    #if JucePlugin_IsMidiEffect
        ignoreUnused(layouts);
        return true;
    #else
        // This is the place where you check if the layout is supported.
        // In this template code we only support mono or stereo.
        if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono() &&
            layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
            return false;

                // This checks if the input layout matches the output layout
            #if !JucePlugin_IsSynth
        if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
            return false;
            #endif

        return true;
    #endif
}
#endif

void ChordProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    for (const auto metadata : midiMessages) {
        auto message = metadata.getMessage();
        short num = message.getNoteNumber();

        if (message.isNoteOn()) {
            notes.insert(num);
        } else if (message.isNoteOff()) {
            notes.erase(num);
        }
    }
}

//==============================================================================
bool ChordProcessor::hasEditor() const { return true; }

AudioProcessorEditor* ChordProcessor::createEditor() {
    File sourceDir = File(SOURCE_DIR);
    File bundle = sourceDir.getChildFile("jsui/build/js/main.js");

    auto* editor = new reactjuce::GenericEditor(*this, bundle);

    editor->setResizable(true, true);
    editor->setResizeLimits(400, 240, 400 * 2, 240 * 2);
    editor->getConstrainer()->setFixedAspectRatio(400.0 / 240.0);
    editor->setSize(400, 240);

    startTimer(100);

    return editor;
}

void ChordProcessor::timerCallback() {
    if (auto* editor = dynamic_cast<reactjuce::GenericEditor*>(getActiveEditor())) {

        std::string multi;

        for (const short& x : notes) multi += std::to_string(x) + ",";

        editor->getReactAppRoot().dispatchEvent("activeNotesChanged", static_cast<String>(multi));
    }
}

//==============================================================================
void ChordProcessor::getStateInformation(MemoryBlock& /* destData */) {}
void ChordProcessor::setStateInformation(const void* /* data */, int /* sizeInBytes */) {}

AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new ChordProcessor();
}
