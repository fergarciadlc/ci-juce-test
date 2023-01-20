# Cmake minimum version
cmake_minimum_required (VERSION 3.21)

# Plugin name
set (PROJECT_NAME "ci-cmake-juce")

# Set version
set (CURRENT_VERSION "0.0.1")

# Project name
project (${PROJECT_NAME} VERSION ${CURRENT_VERSION})

# Plugin formats
set (FORMATS AU VST3 VST AAX Standalone)
# set (FORMATS VST3)

# By default we don't want Xcode schemes to be made for modules, etc ******
set (CMAKE_XCODE_GENERATE_SCHEME OFF)

# Architecture and version for MacOS
set (CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
set (CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Support macOS down to High Sierra" FORCE)

# Adds all the module sources so they appear correctly in the IDE ********
set_property (GLOBAL PROPERTY USE_FOLDERS YES)

# Create a /Modules directory in the IDE with the JUCE Module code ********
option (JUCE_ENABLE_MODULE_SOURCE_GROUPS "Show all module sources in IDE projects" ON)

#add_subdirectory (Modules/JUCE)

include (FetchContent)

macro (get_repository name repository branch)
    FetchContent_Declare(${name}
            GIT_REPOSITORY ${repository}
            GIT_TAG ${branch}
            GIT_PROGRESS 1
            GIT_SHALLOW 1)

    FetchContent_GetProperties(${name})

    if (NOT ${name}_POPULATED)
        message("Fetching ${name}...")
        FetchContent_Populate(${name})
        message("Fetch of ${name} finished")
    endif()
endmacro()

get_repository (JUCE https://github.com/juce-framework/JUCE.git origin/master)
add_subdirectory (${juce_SOURCE_DIR})

# Add SKD
juce_set_vst2_sdk_path (SDK/VST2_SDK)
juce_set_aax_sdk_path  (SDK/aax-sdk-2-4-0)

# Plug-in data
juce_add_plugin (${PROJECT_NAME}
    COMPANY_NAME Ear_Candy_Technologies
    BUNDLE_ID com.earCandyTech.ci-cmake-juce
    VERSION ${CURRENT_VERSION}  # Change this if the plugin version is different to the project version
    IS_SYNTH FALSE                       
    NEEDS_MIDI_INPUT FALSE               
    NEEDS_MIDI_OUTPUT FALSE              
    IS_MIDI_EFFECT FALSE                
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE    
    COPY_PLUGIN_AFTER_BUILD TRUE 		 
    PLUGIN_MANUFACTURER_CODE Ects        
    PLUGIN_CODE ci01                     
    FORMATS "${FORMATS}"
    PRODUCT_NAME ${PROJECT_NAME}
)

juce_generate_juce_header (${PROJECT_NAME})

# Set C++ version for a given target
target_compile_features (${PROJECT_NAME} PRIVATE cxx_std_17)

# Add all source files
set (SourceFiles
    Source/PluginEditor.h
    Source/PluginEditor.cpp
    Source/PluginProcessor.h
    Source/PluginProcessor.cpp
	Source/Helpers/Parameters.h
	Source/Helpers/Sizes.h
)

target_sources (${PROJECT_NAME} PRIVATE ${SourceFiles})

# No, we don't want our source buried in extra nested folders *********
set_target_properties (${PROJECT_NAME} PROPERTIES FOLDER "")

# The Xcode source tree should uhhh, still look like the source tree, yo ********
source_group (TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "" FILES ${SourceFiles})

# If need to add binary data
# juce_add_binary_data (Assets SOURCES pamplejuce.png)

# Generate targets
foreach (target ${FORMATS} "All")
    if (TARGET ${PROJECT_NAME}_${target})
        set_target_properties(${PROJECT_NAME}_${target} PROPERTIES
            
			# Tuck the actual plugin targets into a folder where they won't bother us
            FOLDER "Targets"
            # Let us build the target in Xcode
            XCODE_GENERATE_SCHEME ON)

    endif()
endforeach()

# set_target_properties (Assets PROPERTIES FOLDER "Targets")

target_compile_definitions (${PROJECT_NAME}
    PUBLIC
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_MODAL_LOOPS_PERMITTED=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# We'll need to link to these from our plugin as well as our tests
set (JUCE_DEPENDENCIES 
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_osc
)

target_link_libraries (${PROJECT_NAME}
    PRIVATE
    #Assets
    ${JUCE_DEPENDENCIES}
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

# Required for ctest (which is just easier for cross-platform CI)
# enable_testing()

# "GLOBS ARE BAD" is brittle and silly UX, sorry CMake!
# CONFIGURE_DEPENDS / Clion's CMake integration makes globbing absolutely fine
# file (GLOB_RECURSE TestFiles CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Tests/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Tests/*.h")
# source_group (TREE ${CMAKE_CURRENT_SOURCE_DIR}/Tests PREFIX "" FILES ${TestFiles})

# Use Catch2 v3 on the devel branch
# Include (FetchContent)
# FetchContent_Declare(
#     Catch2
#     GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#     GIT_PROGRESS TRUE
#     GIT_SHALLOW TRUE
#     GIT_TAG v3.1.0)
# FetchContent_MakeAvailable(Catch2) # find_package equivalent

# Setup the test executable, again C++ 20 please
# add_executable(Tests ${TestFiles})
# target_compile_features(Tests PRIVATE cxx_std_20)

# Our test executable also wants to know about our plugin code...
# target_include_directories(Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source)
# target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain "${PROJECT_NAME}" ${JUCE_DEPENDENCIES})

# Make an Xcode Scheme for the test executable so we can run tests in the IDE
# set_target_properties (Tests PROPERTIES XCODE_GENERATE_SCHEME ON)

# Organize the test source in the Tests/ folder in the IDE
# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Tests PREFIX "" FILES ${TestFiles})

# Load and use the .cmake file provided by Catch2
# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md
# We have to manually provide the source directory here for now
# include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)
# catch_discover_tests(Tests)

# Color our warnings and errors
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   add_compile_options (-fcolor-diagnostics)
endif ()
