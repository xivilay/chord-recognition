#include <JuceHeader.h>

class ChordProcessor : public AudioProcessor, private Timer {
   public:
    ChordProcessor() : AudioProcessor(BusesProperties()) {}
    ~ChordProcessor() { stopTimer(); }

    const String getName() const { return JucePlugin_Name; }

    bool acceptsMidi() const { return true; }
    bool producesMidi() const { return false; }
    bool isMidiEffect() const { return false; }

    double getTailLengthSeconds() const { return 0.0; }
    int getNumPrograms() { return 1; }
    int getCurrentProgram() { return 0; }
    void setCurrentProgram(int) {}
    const String getProgramName(int) { return {}; }
    void changeProgramName(int, const String&) {}
    void prepareToPlay(double, int) {}
    void releaseResources() {}

    bool isBusesLayoutSupported(const BusesLayout&) const { return false; }

    void getStateInformation(MemoryBlock&) {}
    void setStateInformation(const void*, int) {}

    void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {
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

    bool hasEditor() const { return true; }
    AudioProcessorEditor* createEditor() {
        File exeDir = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory();
        File bundle = exeDir.getChildFile("js/main.js");

        auto* editor = new reactjuce::GenericEditor(*this, bundle);

        short updateInterval = 25;

        editor->setResizable(true, true);
        editor->setResizeLimits(WIDTH, HEIGHT, WIDTH * 2, HEIGHT * 2);
        editor->getConstrainer()->setFixedAspectRatio(WIDTH / HEIGHT);
        editor->setSize(WIDTH, HEIGHT);

        startTimer(updateInterval);

        return editor;
    }

    void timerCallback() {
        if (auto* editor = dynamic_cast<reactjuce::GenericEditor*>(getActiveEditor())) {
            std::string multi;

            for (const short& x : notes) multi += std::to_string(x) + ",";

            editor->getReactAppRoot().dispatchEvent("activeNotesChanged", static_cast<String>(multi));
        }
    }

   private:
    std::unordered_set<short> notes;

    const short WIDTH = 740;
    const short HEIGHT = 840;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChordProcessor)
};
