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

#include "AAX_VHostProcessorDelegate.h"

#include "AAX_UIDs.h"

// ******************************************************************************************
// METHOD:	AAX_VHostProcessorDelegate
// ******************************************************************************************
AAX_VHostProcessorDelegate::AAX_VHostProcessorDelegate( IACFUnknown* pUnknown )
	: mIHostProcessorDelegate(NULL)
	, mIHostProcessorDelegateV2(NULL)
	, mIHostProcessorDelegateV3(NULL)
{
	if ( pUnknown )
	{
		pUnknown->QueryInterface(IID_IAAXHostProcessorDelegateV3, (void **)&mIHostProcessorDelegateV3);
		if (mIHostProcessorDelegateV3)
		{
			mIHostProcessorDelegate = mIHostProcessorDelegateV2 = mIHostProcessorDelegateV3;
		}
		else {
			pUnknown->QueryInterface(IID_IAAXHostProcessorDelegateV2, (void **)&mIHostProcessorDelegateV2);
			if (mIHostProcessorDelegateV2)
			{
				mIHostProcessorDelegate = mIHostProcessorDelegateV2;
			}
			else {
				pUnknown->QueryInterface(IID_IAAXHostProcessorDelegateV1, (void **)&mIHostProcessorDelegate);
			}
		}
	}
}

// ******************************************************************************************
// METHOD:	GetAudio
// ******************************************************************************************
AAX_Result AAX_VHostProcessorDelegate::GetAudio ( const float * const inAudioIns [], int32_t inAudioInCount, int64_t inLocation, int32_t * ioNumSamples )
{
	if ( mIHostProcessorDelegate )
		return mIHostProcessorDelegate->GetAudio( inAudioIns, inAudioInCount, inLocation, ioNumSamples );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetSideChainInputNum
// ******************************************************************************************
int32_t AAX_VHostProcessorDelegate::GetSideChainInputNum ()
{
	int32_t	result = 0;
	if ( mIHostProcessorDelegate )
		return mIHostProcessorDelegate->GetSideChainInputNum();
	
	return result;
}

// ******************************************************************************************
// METHOD:	ForceAnalyze
// ******************************************************************************************
AAX_Result AAX_VHostProcessorDelegate::ForceAnalyze ()
{
	if ( mIHostProcessorDelegateV2 )
		return mIHostProcessorDelegateV2->ForceAnalyze();

	return AAX_ERROR_UNIMPLEMENTED;
}

// ******************************************************************************************
// METHOD:	ForceProcess
// ******************************************************************************************
AAX_Result AAX_VHostProcessorDelegate::ForceProcess ()
{
	if ( mIHostProcessorDelegateV3 )
		return mIHostProcessorDelegateV3->ForceProcess();

	return AAX_ERROR_UNIMPLEMENTED;
}
