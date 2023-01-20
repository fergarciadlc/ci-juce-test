#pragma once

// Basic
static juce::String PLUGIN_NAME = "chorusEarCandy";
static bool LICENSE_ACTIVATION = false;

// Chorus
static juce::String TIME  = "Time";
static juce::String RATE  = "Rate";
static juce::String DEPTH = "Depth";

// General
static juce::String DRY_WET = "DryWet";
static juce::String VOLUMEN = "Volumen";

// Sweetness
static juce::String FLAVOR = "Flavor";

static std::vector<EarCandy::Utilities::Parameter> parameters =
{
    //************************** Chorus *************************//
    { TIME, TIME, 2.0f, 30.0f, 0.01f, 1.0f, 5.0f },
    { RATE, RATE, 0.01f, 3.0f, 0.01f, 1.0f, 3.0f },
    // Depth chorus: [0.5, 1.5] default 0.5, mapped on params.
    { DEPTH, DEPTH, 0.0f, 100.0f, 1.0f, 1.0f, 100.0f },
    
    //************************** General *************************//
    { DRY_WET, DRY_WET, 0.0f, 100.0f, 1.0f, 1.0f, 100.0f },
    { VOLUMEN, VOLUMEN, -60.0f, 24.0f, 1.0f, 1.0f, 0.0f },
    
    //************************** Flavor **************************//
    { FLAVOR, FLAVOR, -1.0f, 1.0f, 0.1f, 1.0f, 0.0f },
};
