#include "ChordProcessor.h"

AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new ChordProcessor(); }