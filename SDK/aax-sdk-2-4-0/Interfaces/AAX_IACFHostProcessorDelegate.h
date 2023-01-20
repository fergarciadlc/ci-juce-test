/*================================================================================================*/
/*
 *
 *	Copyright 2013-2017 by Avid Technology, Inc.
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

/**  
 *	\file  AAX_IACFHostProcessorDelegate.h
 *
 */ 
/*================================================================================================*/


#ifndef AAX_IACFHOSTPROCESSORDELEGATE_H
#define AAX_IACFHOSTPROCESSORDELEGATE_H

#include "AAX.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "acfunknown.h"


/** @brief Versioned interface for host methods specific to offline processing
 */
class AAX_IACFHostProcessorDelegate : public IACFUnknown
{
public:
	virtual AAX_Result	GetAudio ( const float * const inAudioIns [], int32_t inAudioInCount, int64_t inLocation, int32_t * ioNumSamples ) = 0;	///< \copydoc AAX_IHostProcessorDelegate::GetAudio()
	virtual int32_t		GetSideChainInputNum () = 0;	///< \copydoc AAX_IHostProcessorDelegate::GetSideChainInputNum()
};


/** @brief Versioned interface for host methods specific to offline processing
 */
class AAX_IACFHostProcessorDelegate_V2 : public AAX_IACFHostProcessorDelegate
{
public:
	virtual AAX_Result	ForceAnalyze () = 0;	///< \copydoc AAX_IHostProcessorDelegate::ForceAnalyze()
};

/** @brief Versioned interface for host methods specific to offline processing
 */
class AAX_IACFHostProcessorDelegate_V3 : public AAX_IACFHostProcessorDelegate_V2
{
public:
	virtual AAX_Result	ForceProcess () = 0;	///< \copydoc AAX_IHostProcessorDelegate::ForceProcess()
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // #ifndef AAX_IACFHOSTPROCESSORDELEGATE_H
