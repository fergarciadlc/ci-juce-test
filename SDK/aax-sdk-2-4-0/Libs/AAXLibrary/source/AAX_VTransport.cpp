/*================================================================================================*/
/*
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
/*================================================================================================*/

#include "AAX_VTransport.h"
#include "AAX_UIDs.h"

// ******************************************************************************************
// METHOD:	AAX_VTransport
// ******************************************************************************************
AAX_VTransport::AAX_VTransport( IACFUnknown* pUnknown )
{
	if ( pUnknown )
	{
		pUnknown->QueryInterface(IID_IAAXTransportV1, (void **)&mITransport);
		pUnknown->QueryInterface(IID_IAAXTransportV2, (void **)&mITransportV2);
		pUnknown->QueryInterface(IID_IAAXTransportV3, (void **)&mITransportV3);
	}
}

// ******************************************************************************************
// METHOD:	~AAX_VTransport
// ******************************************************************************************
AAX_VTransport::~AAX_VTransport()
{
}

// ******************************************************************************************
// METHOD:	GetCurrentTempo
// ******************************************************************************************
AAX_Result AAX_VTransport::GetCurrentTempo ( double* TempoBPM ) const
{
	if ( mITransport )
		return mITransport->GetCurrentTempo( TempoBPM );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetCurrentMeter
// ******************************************************************************************
AAX_Result AAX_VTransport::GetCurrentMeter ( int32_t* MeterNumerator, int32_t* MeterDenominator ) const
{
	if ( mITransport )
		return mITransport->GetCurrentMeter ( MeterNumerator, MeterDenominator );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	IsTransportPlaying
// ******************************************************************************************
AAX_Result AAX_VTransport::IsTransportPlaying ( bool* isPlaying ) const
{
	if ( mITransport )
		return mITransport->IsTransportPlaying ( isPlaying );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetCurrentTickPosition
// ******************************************************************************************
AAX_Result AAX_VTransport::GetCurrentTickPosition ( int64_t* TickPosition ) const
{
	if ( mITransport )
		return mITransport->GetCurrentTickPosition ( TickPosition );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetCurrentLoopPosition
// ******************************************************************************************
AAX_Result AAX_VTransport::GetCurrentLoopPosition ( bool* bLooping, int64_t* LoopStartTick, int64_t* LoopEndTick ) const
{
	if ( mITransport )
		return mITransport->GetCurrentLoopPosition ( bLooping, LoopStartTick, LoopEndTick );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetCurrentNativeSampleLocation
// ******************************************************************************************
AAX_Result AAX_VTransport::GetCurrentNativeSampleLocation ( int64_t* SampleLocation ) const
{
	if ( mITransport )
		return mITransport->GetCurrentNativeSampleLocation ( SampleLocation );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetCustomTickPosition
// ******************************************************************************************
AAX_Result AAX_VTransport::GetCustomTickPosition( int64_t* oTickPosition, int64_t iSampleLocation) const
{
	if ( mITransport )
		return mITransport->GetCustomTickPosition( oTickPosition, iSampleLocation);
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetBarBeatPosition
// ******************************************************************************************
AAX_Result AAX_VTransport::GetBarBeatPosition(int32_t* Bars, int32_t* Beats, int64_t* DisplayTicks, int64_t SampleLocation) const
{
	if ( mITransport )
		return mITransport->GetBarBeatPosition( Bars, Beats, DisplayTicks, SampleLocation);
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetTicksPerQuarter
// ******************************************************************************************
AAX_Result AAX_VTransport::GetTicksPerQuarter ( uint32_t* ticks ) const
{
	if ( mITransport )
		return mITransport->GetTicksPerQuarter ( ticks );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetCurrentTicksPerBeat
// ******************************************************************************************
AAX_Result AAX_VTransport::GetCurrentTicksPerBeat ( uint32_t* ticks ) const
{
	if ( mITransport )
		return mITransport->GetCurrentTicksPerBeat ( ticks );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetTimelineSelectionStartPosition
// ******************************************************************************************
AAX_Result AAX_VTransport::GetTimelineSelectionStartPosition ( int64_t* oSampleLocation ) const
{
	if ( mITransportV2 )
		return mITransportV2->GetTimelineSelectionStartPosition ( oSampleLocation );

    return AAX_ERROR_UNIMPLEMENTED;
}

// ******************************************************************************************
// METHOD:	GetTimeCodeInfo
// ******************************************************************************************
AAX_Result AAX_VTransport::GetTimeCodeInfo( AAX_EFrameRate* oFrameRate, int32_t* oOffset ) const
{
	if ( mITransportV2 )
		return mITransportV2->GetTimeCodeInfo( oFrameRate, oOffset );

    return AAX_ERROR_UNIMPLEMENTED;
}

// ******************************************************************************************
// METHOD:	GetFeetFramesInfo
// ******************************************************************************************
AAX_Result AAX_VTransport::GetFeetFramesInfo( AAX_EFeetFramesRate* oFeetFramesRate, int64_t* oOffset ) const
{
	if ( mITransportV2 )
		return mITransportV2->GetFeetFramesInfo( oFeetFramesRate, oOffset );

    return AAX_ERROR_UNIMPLEMENTED;
}

// ******************************************************************************************
// METHOD:	IsMetronomeEnabled
// ******************************************************************************************
AAX_Result AAX_VTransport::IsMetronomeEnabled ( int32_t* isEnabled ) const
{
	if ( mITransportV2 )
		return mITransportV2->IsMetronomeEnabled( isEnabled );

    return AAX_ERROR_UNIMPLEMENTED;
}

// ******************************************************************************************
// METHOD:	GetHDTimeCodeInfo
// ******************************************************************************************
AAX_Result AAX_VTransport::GetHDTimeCodeInfo( AAX_EFrameRate* oHDFrameRate, int64_t* oHDOffset ) const
{
	if ( mITransportV3 )
		return mITransportV3->GetHDTimeCodeInfo( oHDFrameRate, oHDOffset );
	
	return AAX_ERROR_UNIMPLEMENTED;
}
