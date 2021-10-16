#include "PluginProcessor.h"

ChordProcessor::ChordProcessor()
    : AudioProcessor(BusesProperties()) {}

ChordProcessor::~ChordProcessor() { stopTimer(); }

const String ChordProcessor::getName() const { return JucePlugin_Name; }

bool ChordProcessor::acceptsMidi() const { return true; }
bool ChordProcessor::producesMidi() const { return false; }
bool ChordProcessor::isMidiEffect() const { return false; }

double ChordProcessor::getTailLengthSeconds() const { return 0.0; }
int ChordProcessor::getNumPrograms() { return 1; }
int ChordProcessor::getCurrentProgram() { return 0; }
void ChordProcessor::setCurrentProgram(int) {}
const String ChordProcessor::getProgramName(int) { return {}; }
void ChordProcessor::changeProgramName(int, const String&) {}
void ChordProcessor::prepareToPlay(double, int) {}
void ChordProcessor::releaseResources() {}

bool ChordProcessor::isBusesLayoutSupported(const BusesLayout&) const { return false; }

void ChordProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {
    ScopedNoDenormals noDenormals;

    buffer.clear();

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

bool ChordProcessor::hasEditor() const { return true; }

AudioProcessorEditor* ChordProcessor::createEditor() {
    File exeDir = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory();
    File bundle = exeDir.getChildFile("js/main.js");

    auto* editor = new reactjuce::GenericEditor(*this, bundle);

    short width = 740;
    short height = 840;
    short updateInterval = 25;

    editor->setResizable(true, true);
    editor->setResizeLimits(width, height, width * 2, height * 2);
    editor->getConstrainer()->setFixedAspectRatio(width / height);
    editor->setSize(width, height);

    startTimer(updateInterval);

    return editor;
}

void ChordProcessor::timerCallback() {
    if (auto* editor = dynamic_cast<reactjuce::GenericEditor*>(getActiveEditor())) {
        std::string multi;

        for (const short& x : notes) multi += std::to_string(x) + ",";

        editor->getReactAppRoot().dispatchEvent("activeNotesChanged", static_cast<String>(multi));
    }
}

void ChordProcessor::getStateInformation(MemoryBlock&) {}
void ChordProcessor::setStateInformation(const void*, int) {}

AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new ChordProcessor(); }
