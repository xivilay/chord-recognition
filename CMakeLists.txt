cmake_minimum_required(VERSION 3.15)

set(VERSION 0.1.0)
set(PROJECT_NAME MidiChordRecognition)
set(SOURCES src/Main.cpp)
set(FORMATS VST3)

set(REACT_JUCE "react_juce")
set(REACT_JUCE_DIR "juce-react/${REACT_JUCE}")
set(JUCE_DIR "juce-react/ext/juce")

project(${PROJECT_NAME} VERSION ${VERSION})

add_subdirectory(${JUCE_DIR} "juce")

juce_add_module(${REACT_JUCE_DIR})

# add_subdirectory(${REACT_JUCE_DIR}/hermes)
#     target_compile_definitions(${REACT_JUCE} INTERFACE REACTJUCE_USE_HERMES=1)
#     target_include_directories(
#         INTERFACE
#         ${REACT_JUCE_DIR}/hermes/API/
#         ${REACT_JUCE_DIR}/hermes/public/
#     )
#     target_link_libraries(${REACT_JUCE} INTERFACE hermesapi)

target_compile_definitions(${REACT_JUCE} INTERFACE REACTJUCE_USE_DUKTAPE=1)

juce_add_plugin(${PROJECT_NAME}
    VERSION ${VERSION}
    COMPANY_NAME XivilaY
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE Xivy
    PLUGIN_CODE Xiv7
    FORMATS ${FORMATS}
    PRODUCT_NAME "${PROJECT_NAME}"
)

juce_add_binary_data(BinaryData SOURCES
    "jsui/build/js/main.js"
)

juce_generate_juce_header(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(${PROJECT_NAME} PUBLIC
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