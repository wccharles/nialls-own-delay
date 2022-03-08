cmake_minimum_required(VERSION 3.15)
project(ModDelay VERSION 1.0.0)
set(JUCE_PLUGIN_NAME "Modulation Delay")

enable_testing()
include(GoogleTest)

add_subdirectory(third-party/JUCE)
add_subdirectory(third-party/react-juce)

if(APPLE)
  set(VST_DIR "$ENV{HOME}/Library/Audio/Plug-Ins/VST")
  set(VST3_DIR "$ENV{HOME}/Library/Audio/Plug-Ins/VST3")
  set(AU_DIR "$ENV{HOME}/Library/Audio/Plug-Ins/Components")
elseif(WIN32)
  set(VST_DIR "%CommonProgramFiles%\VST2" "%CommonProgramFiles(x86)%\VST2")
  set(VST3_DIR "%CommonProgramFiles%\VST3" "%CommonProgramFiles(x86)%\VST3")
endif()

# juce_set_vst2_sdk_path ($ENV{HOME}/SDKs/vstsdk2.4)

juce_add_plugin(
  ModDelay
  VERSION
  ${PROJECT_VERSION} # Set this if the plugin version is different to the
                     # project version
  # ICON_BIG ...                              # ICON_* arguments specify a path
  # to an image file to use as an icon for the Standalone ICON_SMALL ...
  COMPANY_NAME
  "WCCharles"
  COMPANY_EMAIL
  "C.walker10@edu.salford.ac.uk"
  COMPANY_WEBSITE
  "github.com/wccharles"
  IS_SYNTH
  FALSE
  NEEDS_MIDI_INPUT
  FALSE
  NEEDS_MIDI_OUTPUT
  FALSE
  IS_MIDI_EFFECT
  FALSE
  EDITOR_WANTS_KEYBOARD_FOCUS
  FALSE
  COPY_PLUGIN_AFTER_BUILD
  TRUE
  PLUGIN_MANUFACTURER_CODE
  ModD
  PLUGIN_CODE
  WC1M
  FORMATS
  Standalone
  VST3
  AUv3
  kPlugCategEffect
  VST3_CATEGORIES
  Delay
  Filter
  Fx
  Modulation
  OnlyRT
  Pitch
  Shift
  Reverb
  Stereo
  Up-Downmix
  VST3_COPY_DIR
  ${VST3_DIR}
  AU_COPY_DIR
  ${AU_DIR}
  AU_MAIN_TYPE
  kAudioUnitType_MusicEffect
  PRODUCT_NAME
  ${JUCE_PLUGIN_NAME}
  PLUGIN_NAME
  ${JUCE_PLUGIN_NAME}
  BACKGROUND_AUDIO_ENABLED
  TRUE
  BACKGROUND_BLE_ENABLED
  TRUE
  MICROPHONE_PERMISSION_ENABLED
  TRUE)

juce_generate_juce_header(ModDelay)

target_sources(
  ModDelay
  PRIVATE Source/PluginProcessor.cpp Source/PluginEditor.cpp Source/Delay.cpp
          Source/Modulation.h Source/Params/ParamsData.cpp)

target_include_directories(ModDelay PRIVATE Source/Params)

target_compile_definitions(
  ModDelay
  PUBLIC PLUGIN_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" JUCE_WEB_BROWSER=0
         JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0 JUCER_ENABLE_GPL_MODE=1)

set(JUCE_LINK_LIBRARIES
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_dsp
    juce::juce_graphics
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_processors
    juce::juce_graphics
    juce::juce_gui_basics
    react_juce)

target_link_libraries(ModDelay PRIVATE ${JUCE_LINK_LIBRARIES})

add_subdirectory(tests)
