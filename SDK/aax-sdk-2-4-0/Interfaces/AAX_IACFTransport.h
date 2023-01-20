/*================================================================================================*/
/*
 *
 *	Copyright 2013-2015, 2020-2021 by Avid Technology, Inc.
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
 *	\file  AAX_IACFTransport.h
 *
 *	\brief Interface for the %AAX Transport data access functionality.
 */ 
/*================================================================================================*/

#ifndef AAX_IACFTRANSPORT_H
#define AAX_IACFTRANSPORT_H

#include "AAX.h"
#include "AAX_Enums.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "acfunknown.h"

/**	\brief Versioned interface to information about the host's transport state
 */
class AAX_IACFTransport : public IACFUnknown
{
public:

	virtual AAX_Result	GetCurrentTempo ( double* TempoBPM ) const = 0;	///< \copydoc AAX_ITransport::GetCurrentTempo()
	virtual AAX_Result	GetCurrentMeter ( int32_t* MeterNumerator, int32_t* MeterDenominator ) const = 0;	///< \copydoc AAX_ITransport::GetCurrentMeter()
	virtual AAX_Result	IsTransportPlaying ( bool* isPlaying ) const = 0;	///< \copydoc AAX_ITransport::IsTransportPlaying()
	virtual AAX_Result	GetCurrentTickPosition ( int64_t* TickPosition ) const = 0;	///< \copydoc AAX_ITransport::GetCurrentTickPosition()
	virtual AAX_Result	GetCurrentLoopPosition ( bool* bLooping, int64_t* LoopStartTick, int64_t* LoopEndTick ) const = 0;	///< \copydoc AAX_ITransport::GetCurrentLoopPosition()
	virtual AAX_Result	GetCurrentNativeSampleLocation ( int64_t* SampleLocation ) const = 0;	///< \copydoc AAX_ITransport::GetCurrentNativeSampleLocation()
	virtual AAX_Result	GetCustomTickPosition ( int64_t* oTickPosition, int64_t iSampleLocation ) const = 0;	///< \copydoc AAX_ITransport::GetCustomTickPosition()
	virtual AAX_Result	GetBarBeatPosition ( int32_t* Bars, int32_t* Beats, int64_t* DisplayTicks, int64_t SampleLocation ) const = 0;	///< \copydoc AAX_ITransport::GetBarBeatPosition()
	virtual AAX_Result	GetTicksPerQuarter ( uint32_t* ticks ) const = 0;	///< \copydoc AAX_ITransport::GetTicksPerQuarter()
	virtual AAX_Result	GetCurrentTicksPerBeat ( uint32_t* ticks ) const = 0;	///< \copydoc AAX_ITransport::GetCurrentTicksPerBeat()

};

/**	\brief Versioned interface to information about the host's transport state
 */
class AAX_IACFTransport_V2 : public AAX_IACFTransport
{
public:

	virtual AAX_Result	GetTimelineSelectionStartPosition( int64_t* oSampleLocation ) const = 0;	///< \copydoc AAX_ITransport::GetTimelineSelectionStartPosition()
	virtual AAX_Result	GetTimeCodeInfo( AAX_EFrameRate* oFrameRate, int32_t* oOffset ) const = 0;	///< \copydoc AAX_ITransport::GetTimeCodeInfo() 
	virtual AAX_Result	GetFeetFramesInfo( AAX_EFeetFramesRate* oFeetFramesRate, int64_t* oOffset ) const =  0;	///< \copydoc AAX_ITransport::GetFeetFramesInfo()
	virtual AAX_Result	IsMetronomeEnabled ( int32_t* isEnabled ) const = 0;	///< \copydoc AAX_ITransport::IsMetronomeEnabled()
};

/**	\brief Versioned interface to information about the host's transport state
 */
class AAX_IACFTransport_V3 : public AAX_IACFTransport_V2
{
public:
	
	virtual AAX_Result	GetHDTimeCodeInfo( AAX_EFrameRate* oHDFrameRate, int64_t* oHDOffset ) const = 0;	///< \copydoc AAX_ITransport::GetHDTimeCodeInfo()
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // AAX_IACFTRANSPORT_H

