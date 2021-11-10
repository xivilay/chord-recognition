cmake_minimum_required(VERSION 3.15)

set(version 0.1.0)
set(pluginName MidiChordRecognition)
set(sources src/Main.cpp)
set(targets VST3)

set(REACT_JUCE "react_juce")
set(REACT_JUCE_DIR "juce-react/${REACT_JUCE}")
set(JUCE_DIR "juce-react/ext/juce")

project(${pluginName} VERSION ${version})

add_subdirectory(${JUCE_DIR} "juce")

juce_add_module(${REACT_JUCE_DIR})

target_compile_definitions(${REACT_JUCE} INTERFACE REACTJUCE_USE_DUKTAPE=1)

juce_add_plugin(${pluginName}
    VERSION ${version}
    COMPANY_NAME XivilaY
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE Xivy
    PLUGIN_CODE Xiv7
    FORMATS ${targets}
    PRODUCT_NAME "${pluginName}"
)

juce_add_binary_data(BinaryData SOURCES
    "jsui/build/js/main.js"
)

juce_generate_juce_header(${pluginName})

target_sources(${pluginName} PRIVATE ${sources})

target_compile_definitions(${pluginName} PRIVATE
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(${pluginName} PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
    
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_graphics
    juce::juce_gui_basics

    ${REACT_JUCE}
    BinaryData
)