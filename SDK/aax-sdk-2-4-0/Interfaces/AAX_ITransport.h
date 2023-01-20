/*================================================================================================*/
/*
 *
 *	Copyright 2013-2015, 2018, 2020-2021 by Avid Technology, Inc.
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
 *	\file  AAX_ITransport.h
 *
 *	\brief The interface for query ProTools transport information
 *
 *	\note To use this interface plug-in must describe AAX_eProperty_UsesMIDI property
 */
/*================================================================================================*/


#ifndef AAX_ITRANSPORT_H
#define AAX_ITRANSPORT_H

#include "AAX.h"
#include "AAX_Enums.h"

/**	\brief Interface to information about the host's transport state
	
	\details
	\hostimp
	
	Plug-ins that use this interface should describe \ref AAX_eProperty_UsesTransport as 1
	
	Acquire this interface using \ref AAX_IMIDINode::GetTransport(). Classes that inherit
	from \ref AAX_CEffectParameters or \ref AAX_CEffectGUI can also use
	\ref AAX_CEffectParameters::Transport() / \ref AAX_CEffectGUI::Transport().
	
*/
class AAX_ITransport
{
public:
	
	/** \brief Virtual destructor
	 *
	 *	\note This destructor MUST be virtual to prevent memory leaks.
	 */
	virtual	~AAX_ITransport()	{ }
	
	/*!
	 *	\brief CALL: Gets the current tempo
	 *
	 *	Returns the tempo corresponding to the current position of the transport counter
	 *
	 *	\note The resolution of the tempo returned here is based on the host's tempo resolution, so it will match the tempo displayed in the host.
	 *	Use \ref AAX_ITransport::GetCurrentTicksPerBeat() "GetCurrentTicksPerBeat()" to calculate the tempo resolution note.
	 *
	 *	\param[out] TempoBPM The current tempo in beats per minute	
	 */
	virtual	AAX_Result	GetCurrentTempo ( double* TempoBPM ) const = 0;
	
	/*!
	 *	\brief CALL: Gets the current meter
	 *
	 *	Returns the meter corresponding to the current position of the transport counter
	 *
	 *	\param[out] MeterNumerator The numerator portion of the meter
	 *	\param[out] MeterDenominator The denominator portion of the meter
	 */
	virtual	AAX_Result	GetCurrentMeter ( int32_t* MeterNumerator, int32_t* MeterDenominator ) const = 0;
	
	/*!
	 *	\brief CALL: Indicates whether or not the transport is playing back
	 *
	 *	\param[out] isPlaying \c true if the transport is currently in playback 
	 */
	virtual	AAX_Result	IsTransportPlaying ( bool* isPlaying ) const = 0;
	
	/*!
	 *  \brief CALL: Gets the current tick position
	 *
	 *	Returns the current tick position corresponding to the current transport position. One
	 *	"Tick" is represented here as 1/960000 of a quarter note. That is, there are 960,000 of
	 *	these ticks in a quarter note.
	 *
	 *	\compatibility The tick resolution here is different than that of the tick displayed in
	 *	Pro Tools. "Display ticks" (as they are called) are 1/960 of a quarter note.
	 *
	 *	\param[out] TickPosition The tick position value
	 */		
	virtual	AAX_Result	GetCurrentTickPosition ( int64_t* TickPosition ) const = 0;
	
	/*!
	 *	\brief CALL: Gets current information on loop playback
	 *	
	 *	\compatibility This does not indicate anything about the status of the "Loop Record" option. Even
	 *	when the host is configured to loop playback, looping may not occur if certain conditions are not
	 *	met (i.e. the length of the selection is too short)
	 *
	 *	\param[out] bLooping \c true if the host is configured to loop playback
	 *	\param[out] LoopStartTick The starting tick position of the selection being looped (see \ref AAX_ITransport::GetCurrentTickPosition() "GetCurrentTickPosition()")
	 *	\param[out] LoopEndTick The ending tick position of the selection being looped (see \ref AAX_ITransport::GetCurrentTickPosition() "GetCurrentTickPosition()")
	 */		
	virtual	AAX_Result	GetCurrentLoopPosition ( bool* bLooping, int64_t* LoopStartTick, int64_t* LoopEndTick ) const = 0;
	/*!
	*	\brief CALL: Gets the current playback location of the native audio engine
	*	
	*	When called from a ProcessProc render callback, this method will provide the absolute sample location at the
	*	beginning of the callback's audio buffers.
	*	
	*	When called from \ref AAX_IEffectParameters::RenderAudio_Hybrid(), this method will provide the absolute
	*	sample location for the samples in the method's <b>output</b> audio buffers. To calculate the absolute sample
	*	location for the sampels in the method's input buffers (i.e. the timelin location where the samples
	*	originated) subtract the value provided by \ref AAX_IController::GetHybridSignalLatency() from this value.
	*
	*	When called from a non-real-time thread, this method will provide the current location of the samples being
	*	processed by the plug-in's ProcessProc on its real-time processing thread.
	*
	*	\note This method only returns a value during playback. It cannot be used to determine, e.g., the location of
	*	the timeline selector while the host is not in playback.
	*
	*	\param[out] SampleLocation Absolute sample location of the first sample in the current native processing buffer      
	*/
	virtual	AAX_Result	GetCurrentNativeSampleLocation ( int64_t* SampleLocation ) const = 0;
	
	/*!
	*	\brief CALL: Given an absolute sample position, gets the corresponding tick position
	*
	*	\compatibility There is a minor performance cost associated with using this API in Pro Tools. It should not be
	*	used excessively without need.
	*
	*	\param[out] oTickPosition the timeline tick position corresponding to \c iSampleLocation
	*	\param[in] iSampleLocation An absolute sample location (see \ref AAX_ITransport::GetCurrentNativeSampleLocation() "GetCurrentNativeSampleLocation()")
	*/
	virtual	AAX_Result	GetCustomTickPosition( int64_t* oTickPosition, int64_t iSampleLocation) const = 0;
	
	/*!
	*	\brief CALL: Given an absolute sample position, gets the corresponding bar and beat position
	*
	*	\compatibility There is a minor performance cost associated with using this API in Pro Tools. It should not be
	*	used excessively without need.
	*
	*	\param[out] Bars The bar corresponding to \c SampleLocation
	*	\param[out] Beats The beat corresponding to \c SampleLocation
	*	\param[out] DisplayTicks The ticks corresponding to \c SampleLocation
	*	\param[in] SampleLocation An absolute sample location (see \ref AAX_ITransport::GetCurrentNativeSampleLocation() "GetCurrentNativeSampleLocation()")
	*/
	virtual	AAX_Result	GetBarBeatPosition(int32_t* Bars, int32_t* Beats, int64_t* DisplayTicks, int64_t SampleLocation) const = 0;
	
	/*!
	 *	\brief CALL: Retrieves the number of ticks per quarter note
	 *	
	 *	\param[out] ticks
	 */
	virtual	AAX_Result	GetTicksPerQuarter ( uint32_t* ticks ) const = 0;
	
	/*!
	 *	\brief CALL: Retrieves the number of ticks per beat
	 *
	 *	\param[out] ticks
	 */
	virtual	AAX_Result	GetCurrentTicksPerBeat ( uint32_t* ticks ) const = 0;
	
	/*!
	 *	\brief CALL: Retrieves the current absolute sample position of the beginning of the current transport selection
	 *
	 *	\note This method is part of the \ref AAX_IACFTransport_V2 "version 2 transport interface"
	 *
	 *	\param[out] oSampleLocation
	 */
	virtual	AAX_Result	GetTimelineSelectionStartPosition ( int64_t* oSampleLocation ) const = 0;
	
	/*!
	 *	\brief CALL: Retrieves the current time code frame rate and offset
	 *
	 *	\note This method is part of the \ref AAX_IACFTransport_V2 "version 2 transport interface"
	 *
	 *	\param[out] oFrameRate
	 *	\param[out] oOffset
	 */
	virtual	AAX_Result	GetTimeCodeInfo( AAX_EFrameRate* oFrameRate, int32_t* oOffset ) const = 0;
	
	/*!
	 *	\brief CALL: Retrieves the current timecode feet/frames rate and offset
	 *
	 *	\note This method is part of the \ref AAX_IACFTransport_V2 "version 2 transport interface"
	 *
	 *	\param[out] oFeetFramesRate
	 *	\param[out] oOffset
	 */
	virtual	AAX_Result	GetFeetFramesInfo( AAX_EFeetFramesRate* oFeetFramesRate, int64_t* oOffset ) const = 0;
	
	/*!
	 *	\brief Sets isEnabled to true if the metronome is enabled
	 *	
	 *	\note This method is part of the \ref AAX_IACFTransport_V2 "version 2 transport interface"
	 *
	 *	\param[out] isEnabled
	 */
	virtual AAX_Result	IsMetronomeEnabled ( int32_t* isEnabled ) const = 0;
	
	/*!
	 *	\brief CALL: Retrieves the current HD time code frame rate and offset
	 *
	 *	\note This method is part of the \ref AAX_IACFTransport_V3 "version 3 transport interface"
	 *
	 *	\param[out] oHDFrameRate
	 *	\param[out] oHDOffset
	 */
	virtual	AAX_Result	GetHDTimeCodeInfo( AAX_EFrameRate* oHDFrameRate, int64_t* oHDOffset ) const = 0;
};

#endif	//	AAX_ITRANSPORT_H

