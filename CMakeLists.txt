cmake_minimum_required(VERSION 3.15)

set(version 0.1.0)
set(pluginName MidiChordRecognition)
set(sources src/PluginProcessor.cpp)
set(targets VST3 Standalone) # AAX AU AUv3 Standalone Unity VST VST3

set(REACT_JUCE_DIR "juce-react/react_juce")
set(JUCE_DIR "juce-react/ext/juce")

project(${pluginName} VERSION ${version})

################################################
#        copied from juce-react/cmake          #
################################################
option(JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)

set(REACTJUCE_JS_LIBRARY DUKTAPE CACHE STRING "The JS Engine to use: either HERMES or DUKTAPE")

add_subdirectory(${JUCE_DIR} "juce")
juce_add_module(${REACT_JUCE_DIR})

if (REACTJUCE_JS_LIBRARY STREQUAL "HERMES")
    add_subdirectory("${REACT_JUCE_DIR}/hermes")
    target_compile_definitions(INTERFACE react_juce REACTJUCE_USE_HERMES=1)
    target_include_directories(
        INTERFACE
        ${REACT_JUCE_DIR}
        ${REACT_JUCE_DIR}/hermes/API/
        ${REACT_JUCE_DIR}/hermes/public/
    )
    target_link_libraries(INTERFACE react_juce hermesapi)
elseif (REACTJUCE_JS_LIBRARY STREQUAL "DUKTAPE")
    target_compile_definitions(react_juce INTERFACE REACTJUCE_USE_DUKTAPE=1)
endif()
################################################
#                   end                        #
################################################
#juce_set_vst2_sdk_path(sdk/VST2_SDK)

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

juce_generate_juce_header(${pluginName})

target_sources(${pluginName} PRIVATE ${sources})
target_include_directories(${pluginName} PRIVATE react_juce/)
target_compile_definitions(${pluginName} PRIVATE
    SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_COPY_PLUGIN_AFTER_BUILD=TRUE
    JUCE_PLUGIN_COPY_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(${pluginName} PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
    
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_graphics
    juce::juce_gui_basics

    react_juce
)