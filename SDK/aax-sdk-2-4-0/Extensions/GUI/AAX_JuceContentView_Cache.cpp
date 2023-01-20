/*================================================================================================*/
/*
 *	Copyright 2011-2015, 2017 by Avid Technology, Inc.
 *	All rights reserved.
 *
 *	CONFIDENTIAL: This document contains confidential information.  Do not 
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
 *	OF Avid Technology, INC.
 */
/*================================================================================================*/
#include "AAX_JuceContentView_Cache.h"
#include "AAX_CEffectGUI_Juce.h"

#include "AAX_IController.h"

static AAX_JuceContentView_Cache *	sCache = 0;

// *******************************************************************************
// METHOD: ~AAX_JuceContentView_Cache
// *******************************************************************************
AAX_JuceContentView_Cache::~AAX_JuceContentView_Cache ()
{
	JUCE_AUTORELEASEPOOL
	
	sCache = 0;
	TContentMap::iterator iter = mContentViews.begin ();
	for ( ; iter != mContentViews.end (); ++iter )
	{
		AAX_JuceContentView * contentView = iter->second;
		deleteAndZero ( contentView );
	}

	if (AAX_CEffectGUI_Juce::sTooltipWindow)
	{
		delete AAX_CEffectGUI_Juce::sTooltipWindow;
		AAX_CEffectGUI_Juce::sTooltipWindow = NULL;
	}
}

// *******************************************************************************
// METHOD: add
// *******************************************************************************
bool AAX_JuceContentView_Cache::add ( AAX_CEffectGUI * inEffectGUI, AAX_JuceContentView * inContentView )
{
	if ( ! sCache )
		sCache = new AAX_JuceContentView_Cache;
	
	if ( sCache )
	{
		if ( inEffectGUI )
		{
			AAX_EStemFormat	inputStemFormat, outputStemFormat;
			if ( sCache->getStemFormats ( inEffectGUI, & inputStemFormat, & outputStemFormat ) )
			{
				sCache->doAdd ( inputStemFormat, outputStemFormat, inContentView );
				inContentView->SetEffectGUI ( 0 );
				return true;
			}
		}
	}
	
	return false;
}

// *******************************************************************************
// METHOD: getStemFormats
// *******************************************************************************
AAX_JuceContentView * AAX_JuceContentView_Cache::get ( AAX_CEffectGUI * inEffectGUI )
{
	if ( sCache )
	{
		if ( inEffectGUI )
		{
			AAX_EStemFormat	inputStemFormat, outputStemFormat;
			if ( sCache->getStemFormats ( inEffectGUI, & inputStemFormat, & outputStemFormat ) )
				return sCache->doGet ( inputStemFormat, outputStemFormat );
		}
	}
	
	return 0;
}
	
// *******************************************************************************
// METHOD: getStemFormats
// *******************************************************************************
void AAX_JuceContentView_Cache::doAdd ( AAX_EStemFormat inInputStemFormat, AAX_EStemFormat inOutputStemFormat, AAX_JuceContentView * inContentView )
{
	TStemPair	stemPair ( inInputStemFormat, inOutputStemFormat );
	mContentViews.insert ( std::pair<TStemPair,AAX_JuceContentView *> ( stemPair, inContentView ) );
}
	
// *******************************************************************************
// METHOD: getStemFormats
// *******************************************************************************
AAX_JuceContentView * AAX_JuceContentView_Cache::doGet ( AAX_EStemFormat inInputStemFormat, AAX_EStemFormat inOutputStemFormat )
{
	AAX_JuceContentView *	result = 0;
	TStemPair	stemPair ( inInputStemFormat, inOutputStemFormat );
	TContentMap::iterator iter = mContentViews.find ( stemPair );
	if ( iter != mContentViews.end () )
	{
		result = iter->second;
		result->clearControlHighlightInfo ();
		
		mContentViews.erase ( iter );
	}
	
	return result;
}

// *******************************************************************************
// METHOD: getStemFormats
// *******************************************************************************
bool AAX_JuceContentView_Cache::getStemFormats ( AAX_CEffectGUI * inEffectGUI, AAX_EStemFormat * outInputStemFormat, AAX_EStemFormat * outOutputStemFormat )
{
	if ( inEffectGUI )
	{
		AAX_IController * controller = inEffectGUI->GetController ();
		if ( controller )
		{
			if ( controller->GetInputStemFormat ( outInputStemFormat ) != AAX_SUCCESS )
				return false;
			
			if ( controller->GetOutputStemFormat ( outOutputStemFormat ) != AAX_SUCCESS )
				return false;
			
			return true;
		}
	}
	
	return false;
}
