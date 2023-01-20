/*================================================================================================*/
/*
 *	Copyright 2010-2017 by Avid Technology, Inc.
 *	All rights reserved.
 *	
 *	CONFIDENTIAL: This document contains confidential information.  Do not 
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
 *	OF Avid Technology, INC.
 *
 */
/*================================================================================================*/

#include "AAX_CHostProcessor.h"
#include "AAX_IEffectParameters.h"
#include "AAX_VHostProcessorDelegate.h"
#include "AAX_VController.h"
#include "AAX_IEffectParameters.h"
#include "AAX_UIDs.h"

// ***************************************************************************
// METHOD:	AAX_CHostProcessor
// ***************************************************************************
AAX_CHostProcessor::AAX_CHostProcessor (void) :
	mController( NULL ),
	mHostProcessingDelegate ( NULL ),
	mEffectParameters( NULL )
{		
	mSrcStart			= 0;
	mSrcEnd				= 0;
	mDstStart			= 0;
	mDstEnd				= 0;	
	mLocation			= 0;
}

// ***************************************************************************
// METHOD:	~AAX_CHostProcessor
// ***************************************************************************
AAX_CHostProcessor::~AAX_CHostProcessor ()
{
	Uninitialize();	//<DMT> Just try to double check that this was all torn down.
}

// ***************************************************************************
// METHOD:	Initialize
// ***************************************************************************
AAX_Result		AAX_CHostProcessor::Initialize(IACFUnknown* iController)
{
	mController = new AAX_VController(iController);
	if (iController == 0 || mController == 0)
		return AAX_ERROR_NOT_INITIALIZED;

	mHostProcessingDelegate = new AAX_VHostProcessorDelegate(iController);
	if (iController == 0 || mHostProcessingDelegate == 0)
		return AAX_ERROR_NOT_INITIALIZED;
	
	if ( iController )
	{
		iController->QueryInterface(IID_IAAXEffectParametersV1, (void **)&mEffectParameters);		
	}	
	
	return AAX_SUCCESS;
}
		
// *******************************************************************************
// METHOD:	Uninitialize
// *******************************************************************************
AAX_Result		AAX_CHostProcessor::Uninitialize ( )
{
	if (mEffectParameters)
	{
		mEffectParameters->Release();
		mEffectParameters = 0;
	}
	
	if ( mHostProcessingDelegate )
	{
		delete ( mHostProcessingDelegate );
		mHostProcessingDelegate = 0;
	}
	
	if ( mController )
	{
		delete ( mController );
		mController = 0;
	}	
	return AAX_SUCCESS;
}
		

// ***************************************************************************
// METHOD:	InitOutputBounds
// ***************************************************************************
AAX_Result AAX_CHostProcessor::InitOutputBounds ( int64_t iSrcStart, int64_t iSrcEnd, int64_t * oDstStart, int64_t * oDstEnd )
{
	AAX_Result result = AAX_SUCCESS;
	if (oDstStart && oDstEnd)
	{
		mSrcStart = iSrcStart;
		mSrcEnd = iSrcEnd;

		this->TranslateOutputBounds( mSrcStart, mSrcEnd, *oDstStart, *oDstEnd );

		mDstStart = *oDstStart;
		mDstEnd = *oDstEnd;
	}
	else
	{
		result = AAX_ERROR_PLUGIN_NULL_PARAMETER;
	}
		
	return result;
}

// ***************************************************************************
// METHOD:	TranslateOutputBounds
// ***************************************************************************
AAX_Result AAX_CHostProcessor::TranslateOutputBounds ( int64_t iSrcStart, int64_t iSrcEnd, int64_t& oDstStart, int64_t& oDstEnd )
{
	oDstStart = iSrcStart;
	oDstEnd = iSrcEnd;
	return AAX_SUCCESS;
}

// ***************************************************************************
// METHOD:	SetLocation
// ***************************************************************************
AAX_Result AAX_CHostProcessor::SetLocation ( int64_t iSample )
{
	mLocation = iSample;
	return AAX_SUCCESS;
}

// ***************************************************************************
// METHOD:	RenderAudio
// ***************************************************************************
AAX_Result AAX_CHostProcessor::RenderAudio ( const float * const /*inAudioIns*/ [], int32_t /*inAudioInCount*/, float * const /*iAudioOuts*/ [], int32_t /*iAudioOutCount*/, int32_t * /*ioWindowSize*/ )
{
	return AAX_ERROR_UNIMPLEMENTED;
}

// ***************************************************************************
// METHOD:	PreRender
// ***************************************************************************
AAX_Result AAX_CHostProcessor::PreRender ( int32_t /*inAudioInCount*/, int32_t /*iAudioOutCount*/, int32_t /*iWindowSize*/ )
{
	return AAX_SUCCESS;
}

// ***************************************************************************
// METHOD:	PostRender
// ***************************************************************************
AAX_Result AAX_CHostProcessor::PostRender ()
{
	return AAX_SUCCESS;
}

// ***************************************************************************
// METHOD:	AnalyzeAudio
// ***************************************************************************
AAX_Result AAX_CHostProcessor::AnalyzeAudio ( const float * const /*inAudioIns*/ [], int32_t /*inAudioInCount*/, int32_t * /*ioWindowSize*/ )
{
	return AAX_ERROR_UNIMPLEMENTED;
}

// ***************************************************************************
// METHOD:	PreAnalyze
// ***************************************************************************
AAX_Result AAX_CHostProcessor::PreAnalyze ( int32_t /*inAudioInCount*/, int32_t /*iWindowSize*/ )
{
	return AAX_SUCCESS;
}

// ***************************************************************************
// METHOD:	PostAnalyze
// ***************************************************************************
AAX_Result AAX_CHostProcessor::PostAnalyze ()
{
	return AAX_SUCCESS;
}

// ***************************************************************************
// METHOD:	GetClipNameSuffix
// ***************************************************************************
AAX_Result AAX_CHostProcessor::GetClipNameSuffix ( int32_t /*inMaxLength*/, AAX_IString* /*outString*/ ) const
{
    return AAX_ERROR_UNIMPLEMENTED;
}

// ***************************************************************************
// METHOD:	GetAudio
// ***************************************************************************
AAX_Result AAX_CHostProcessor::GetAudio ( const float * const inAudioIns [], int32_t inAudioInCount, int64_t inLocation, int32_t * ioNumSamples )
{
	return mHostProcessingDelegate->GetAudio( inAudioIns, inAudioInCount, inLocation, ioNumSamples );
}

// ***************************************************************************
// METHOD:	GetSideChainInputNum
// ***************************************************************************
int32_t AAX_CHostProcessor::GetSideChainInputNum ()
{
	return mHostProcessingDelegate->GetSideChainInputNum();
}
