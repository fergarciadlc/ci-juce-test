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
 *	\file  AAX_IACFHostProcessor.h
 *
 *	\brief The host processor interface that is exposed to the host application
 *
 */ 
/*================================================================================================*/


#ifndef AAX_IACFHOSTPROCESSOR_H
#define AAX_IACFHOSTPROCESSOR_H

#include "AAX.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "acfunknown.h"

class AAX_IHostProcessorDelegate;
class AAX_IEffectParameters;
class AAX_IString;

/**	
 *	\brief Versioned interface for an %AAX host processing component
 *
 *	\details
 *	\note This interface gets exposed to the host application. See
 *	\ref AAX_CHostProcessor for method documentation.
 *
 *	\legacy	This interface provides offline processing features
 *  analogous to the legacy AudioSuite plug-in architecture
 *
 *	\ingroup AuxInterface_HostProcessor
 *
 */ 
class AAX_IACFHostProcessor : public IACFUnknown
{
public:
	virtual AAX_Result		Initialize(IACFUnknown* iController) = 0;	///< \copydoc AAX_CHostProcessor::Initialize()
	virtual AAX_Result		Uninitialize() = 0;	///< \copydoc AAX_CHostProcessor::Uninitialize()
	
	virtual AAX_Result		InitOutputBounds ( int64_t iSrcStart, int64_t iSrcEnd, int64_t * oDstStart, int64_t * oDstEnd ) = 0;	///< \copydoc AAX_CHostProcessor::InitOutputBounds()
	virtual AAX_Result		SetLocation ( int64_t iSample ) = 0;	///< \copydoc AAX_CHostProcessor::SetLocation()

	virtual AAX_Result		RenderAudio ( const float * const inAudioIns [], int32_t inAudioInCount, float * const iAudioOuts [], int32_t iAudioOutCount, int32_t * ioWindowSize ) = 0;	///< \copydoc AAX_CHostProcessor::RenderAudio()
	virtual AAX_Result		PreRender ( int32_t inAudioInCount, int32_t iAudioOutCount, int32_t iWindowSize ) = 0;	///< \copydoc AAX_CHostProcessor::PreRender()
	virtual AAX_Result		PostRender () = 0;	///< \copydoc AAX_CHostProcessor::PostRender()

	virtual AAX_Result		AnalyzeAudio ( const float * const inAudioIns [], int32_t inAudioInCount, int32_t * ioWindowSize ) = 0;	///< \copydoc AAX_CHostProcessor::AnalyzeAudio()
	virtual AAX_Result		PreAnalyze ( int32_t inAudioInCount, int32_t iWindowSize ) = 0;	///< \copydoc AAX_CHostProcessor::PreAnalyze()
	virtual AAX_Result		PostAnalyze () = 0;	///< \copydoc AAX_CHostProcessor::PostAnalyze()
};

/** 
 *  \brief Supplemental interface for an %AAX host processing component
 *
 *  \details
 *	\note This interface gets exposed to the host application. See
 *	\ref AAX_CHostProcessor for method documentation.
 */
class AAX_IACFHostProcessor_V2 : public AAX_IACFHostProcessor
{
public:
    virtual AAX_Result		GetClipNameSuffix ( int32_t inMaxLength, AAX_IString* outString ) const = 0; ///< \copydoc AAX_CHostProcessor::GetClipNameSuffix()
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif //AAX_IACFHOSTPROCESSOR_H
