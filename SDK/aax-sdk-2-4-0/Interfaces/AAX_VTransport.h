/*================================================================================================*/
/*
 *
 *	Copyright 2013-2017, 2019-2020 by Avid Technology, Inc.
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
 *	\file  AAX_VTransport.h
 *
 *	\brief Version-managed concrete Transport class
 *
 */ 
/*================================================================================================*/

#ifndef AAX_VTRANSPORT_H
#define AAX_VTRANSPORT_H

#include "AAX_ITransport.h"
#include "AAX_IACFTransport.h"
#include "ACFPtr.h"

/**
 *	\brief Version-managed concrete \ref AAX_ITransport class
 *
 */
class AAX_VTransport : public AAX_ITransport
{
public:
	AAX_VTransport( IACFUnknown* pUnknown );
	~AAX_VTransport() AAX_OVERRIDE;
	
	// Transport Information Getters
	// AAX_IACFTransport
	AAX_Result	GetCurrentTempo ( double* TempoBPM ) const AAX_OVERRIDE; ///< \copydoc AAX_ITransport::GetCurrentTempo()
	AAX_Result	GetCurrentMeter ( int32_t* MeterNumerator, int32_t* MeterDenominator ) const AAX_OVERRIDE; ///< \copydoc AAX_ITransport::GetCurrentMeter()
	AAX_Result	IsTransportPlaying ( bool* isPlaying ) const AAX_OVERRIDE; ///< \copydoc AAX_ITransport::IsTransportPlaying()
	AAX_Result	GetCurrentTickPosition ( int64_t* TickPosition ) const AAX_OVERRIDE; ///< \copydoc AAX_ITransport::GetCurrentTickPosition()
	AAX_Result	GetCurrentLoopPosition ( bool* bLooping, int64_t* LoopStartTick, int64_t* LoopEndTick ) const AAX_OVERRIDE; ///< \copydoc AAX_ITransport::GetCurrentLoopPosition()
	AAX_Result	GetCurrentNativeSampleLocation ( int64_t* SampleLocation ) const AAX_OVERRIDE; ///< \copydoc AAX_ITransport::GetCurrentNativeSampleLocation()
	AAX_Result	GetCustomTickPosition( int64_t* oTickPosition, int64_t iSampleLocation) const AAX_OVERRIDE; ///< \copydoc AAX_ITransport::GetCustomTickPosition()
	AAX_Result	GetBarBeatPosition(int32_t* Bars, int32_t* Beats, int64_t* DisplayTicks, int64_t SampleLocation) const AAX_OVERRIDE; ///< \copydoc AAX_ITransport::GetBarBeatPosition()
	AAX_Result	GetTicksPerQuarter ( uint32_t* ticks ) const AAX_OVERRIDE; ///< \copydoc AAX_ITransport::GetTicksPerQuarter()
	AAX_Result	GetCurrentTicksPerBeat ( uint32_t* ticks ) const AAX_OVERRIDE; ///< \copydoc AAX_ITransport::GetCurrentTicksPerBeat()

	// AAX_IACFTransport_V2
	AAX_Result	GetTimelineSelectionStartPosition ( int64_t* oSampleLocation ) const AAX_OVERRIDE; ///< \copydoc AAX_ITransport::GetTimelineSelectionStartPosition()
	AAX_Result	GetTimeCodeInfo( AAX_EFrameRate* oFrameRate, int32_t* oOffset ) const AAX_OVERRIDE; ///< \copydoc AAX_ITransport::GetTimeCodeInfo()
	AAX_Result	GetFeetFramesInfo( AAX_EFeetFramesRate* oFeetFramesRate, int64_t* oOffset ) const AAX_OVERRIDE; ///< \copydoc AAX_ITransport::GetFeetFramesInfo()
	AAX_Result	IsMetronomeEnabled ( int32_t* isEnabled ) const AAX_OVERRIDE; ///< \copydoc AAX_ITransport::IsMetronomeEnabled()
	
	// AAX_IACFTransport_V3
	AAX_Result	GetHDTimeCodeInfo( AAX_EFrameRate* oHDFrameRate, int64_t* oHDOffset ) const AAX_OVERRIDE; ///< \copydoc AAX_ITransport::GetHDTimeCodeInfo()
	
private:
	ACFPtr<AAX_IACFTransport>		mITransport;
	ACFPtr<AAX_IACFTransport_V2>	mITransportV2;
	ACFPtr<AAX_IACFTransport_V3>	mITransportV3;
};

#endif // AAX_VTRANSPORT_H

