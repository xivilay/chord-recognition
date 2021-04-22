cmake_minimum_required(VERSION 3.15)

set(version 0.1.0)
set(pluginName MidiChordRecognition)
set(sources src/PluginProcessor.cpp)

project(${pluginName} VERSION ${version})

################################################
#        copied from juce-react/cmake          #
################################################
option(JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)

set(REACTJUCE_JS_LIBRARY DUKTAPE CACHE STRING "The JS Engine to use: either HERMES or DUKTAPE")

add_subdirectory(juce-react/ext/juce)
juce_add_module(juce-react/react_juce)

if (REACTJUCE_JS_LIBRARY STREQUAL "HERMES")
    add_subdirectory(juce-react/react_juce/hermes)
    target_compile_definitions(INTERFACE react_juce REACTJUCE_USE_HERMES=1)
    target_include_directories(
        INTERFACE
        juce-react/react_juce
        juce-react/react_juce/hermes/API/
        juce-react/react_juce/hermes/public/
    )
    target_link_libraries(INTERFACE react_juce hermesapi)
elseif (REACTJUCE_JS_LIBRARY STREQUAL "DUKTAPE")
    target_compile_definitions(react_juce INTERFACE REACTJUCE_USE_DUKTAPE=1)
endif()
################################################
#                   end                        #
################################################

juce_add_plugin(${pluginName}
    VERSION ${version}                          # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    COMPANY_NAME XivilaY                        # Specify the name of the plugin's author
    # IS_SYNTH TRUE/FALSE                       # Is this a synth or an effect?
    NEEDS_MIDI_INPUT TRUE                       # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE                     # Does the plugin need midi output?
    # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE Xivy               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Xiv7                            # A unique four-character plugin id with at least one upper-case character
    FORMATS VST3 Standalone                     # The formats to build. Other valid formats are: AAX Unity VST VST3 AU AUv3
    PRODUCT_NAME "${pluginName}")               # The name of the final executable, which can differ from the target name

juce_generate_juce_header(${pluginName})

target_sources(${pluginName} PRIVATE ${sources})
target_include_directories(${pluginName} PRIVATE react_juce/)
target_compile_definitions(${pluginName} PRIVATE
    SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(${pluginName} PRIVATE
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
