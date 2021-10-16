#include <JuceHeader.h>
#include "PluginProcessor.h"

AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new ChordProcessor(); }