cmake_minimum_required(VERSION 3.19)
set(PROJECT_NAME NiallsOwnDelay)
project(${PROJECT_NAME} VERSION 0.1.0)
set(JUCE_PLUGIN_NAME "Nialls Own Delay")

if(APPLE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13")
endif()

# By default we don't want Xcode schemes to be made for modules, etc
set(CMAKE_XCODE_GENERATE_SCHEME OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS YES)

# Create a /Modules directory in the IDE with the JUCE Module code
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS
       "Show all module sources in IDE projects" ON)

add_subdirectory(third-party/JUCE)
add_subdirectory(third-party/react-juce)

set(PLUG_FORMATS AUv3 VST3 Standalone)

if(APPLE)
  set(VST3_DIR "$ENV{HOME}/Library/Audio/Plug-Ins/VST3")
  set(AU_DIR "$ENV{HOME}/Library/Audio/Plug-Ins/Components")
elseif(WIN32)
  set(VST3_DIR "$PROGRAMFILES64\\\\Common Files\\\\VST3")
endif()

juce_add_plugin(
  ${PROJECT_NAME}
  VERSION
  ${PROJECT_VERSION} # Set this if the plugin version is different to the
                     # project version
  # ICON_BIG ...                              # ICON_* arguments specify a path
  # to an image file to use as an icon for the Standalone ICON_SMALL ...
  COMPANY_NAME
  "WCCharles"
  BUNDLE_ID
  com.wccharles.niallsowndelay
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
  WCCh
  PLUGIN_CODE
  WC01
  FORMATS
  ${PLUG_FORMATS}
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

# C++17 please
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W3)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror)
endif()

if(NOT XCODE)
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fsanitize=thread")
endif()

juce_generate_juce_header(${PROJECT_NAME})

set(PLUGIN_SOURCES
    ${CMAKE_SOURCE_DIR}/Source/PluginProcessor.h
    ${CMAKE_SOURCE_DIR}/Source/PluginProcessor.cpp
    ${CMAKE_SOURCE_DIR}/Source/Delay.h
    ${CMAKE_SOURCE_DIR}/Source/Delay.cpp
    ${CMAKE_SOURCE_DIR}/Source/StereoProcessor.h
    ${CMAKE_SOURCE_DIR}/Source/FeedbackDelayNetwork.h
    ${CMAKE_SOURCE_DIR}/Source/FeedbackDelayNetwork.cpp
    ${CMAKE_SOURCE_DIR}/Source/STFT.h
    ${CMAKE_SOURCE_DIR}/Source/STFT.cpp
    ${CMAKE_SOURCE_DIR}/Source/Modulation.h
    ${CMAKE_SOURCE_DIR}/Source/Params/ParamsData.h
    ${CMAKE_SOURCE_DIR}/Source/Params/ParamsData.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${PLUGIN_SOURCES})

set(PLUGIN_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/Source ${CMAKE_SOURCE_DIR}/Source/Constants
    ${CMAKE_SOURCE_DIR}/Source/Params)

target_include_directories(${PROJECT_NAME} PRIVATE ${PLUGIN_INCLUDE_DIRS})

# No, we don't want our source buried in extra nested folders
set_target_properties("${PROJECT_NAME}" PROPERTIES FOLDER "")

# The Xcode source tree should uhhh, still look like the source tree, yo
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source
  PREFIX ""
  FILES ${PLUGIN_SOURCES})

# This cleans up the folder organization on Xcode. It tucks the Plugin varieties
# into a "Targets" folder and generate an Xcode Scheme manually Xcode scheme
# generation is turned off globally to limit noise from other targets The
# non-hacky way of doing this is via the global PREDEFINED_TARGETS_FOLDER
# property However that doesn't seem to be working in Xcode Not all plugin types
# (au, vst) available on each build type (win, macos, linux)
foreach(target ${PLUG_FORMATS} "All")
  if(TARGET ${PROJECT_NAME}_${target})
    set_target_properties(
      ${PROJECT_NAME}_${target}
      PROPERTIES
        # Tuck the actual plugin targets into a folder where they won't bother
        # us
        FOLDER "Targets"
        # MacOS only: Sets the default executable that Xcode will open on build
        # For this exact path to to work, manually build the
        # AudioPluginHost.xcodeproj in the JUCE subdir
        # XCODE_SCHEME_EXECUTABLE
        # "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/extras/AudioPluginHost/Builds/MacOSX/build/Debug/AudioPluginHost.app"
        # Let us build the target in Xcode
        XCODE_GENERATE_SCHEME ON
        XCODE_SCHEME_THREAD_SANITIZER ON)
  endif()
endforeach()

target_compile_definitions(
  ${PROJECT_NAME} PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_USE_SIMD=1
                         JUCE_VST3_CAN_REPLACE_VST2=0 JUCER_ENABLE_GPL_MODE=1)

set(JUCE_LINK_LIBRARIES
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_dsp
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
    react_juce)

target_link_libraries(${PROJECT_NAME} PRIVATE ${JUCE_LINK_LIBRARIES})
