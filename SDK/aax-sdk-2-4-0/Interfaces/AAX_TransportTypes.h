/*================================================================================================*/
/*
 *
 *	Copyright 2020-2021 by Avid Technology, Inc.
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
 *	\file  AAX_TransportTypes.h
 *
 *	\brief Structures, enums and other definitions used in transport
 *
 */ 
/*================================================================================================*/

#ifndef AAX_TransportTypes_h_
#define AAX_TransportTypes_h_
#pragma once

// AAX Includes
#include "AAX.h"

// Standard Library Includes
#include <string>
#include <sstream>

#include AAX_ALIGN_FILE_BEGIN
#include AAX_ALIGN_FILE_HOST
#include AAX_ALIGN_FILE_END

/**
 Helper structure for payload data described transport state information.
 */
struct AAX_TransportStateInfo_V1
{
	AAX_ETransportState	mTransportState;
	AAX_ERecordMode	mRecordMode;
	AAX_CBoolean mIsRecordEnabled;
	AAX_CBoolean mIsRecording;
	AAX_CBoolean mIsLoopEnabled;

	AAX_TransportStateInfo_V1() :
		mTransportState(AAX_eTransportState_Unknown),
		mRecordMode(AAX_eRecordMode_Unknown), 
		mIsRecordEnabled(false), 
		mIsRecording(false), 
		mIsLoopEnabled(false)
	{
		static_assert(sizeof(AAX_TransportStateInfo_V1) == 12, "Invalid size of AAX_TransportStateInfo_V1 struct during compilation!");
	}

	inline std::string ToString() const
	{
		std::stringstream ss;

		ss << "{" << std::endl;
		ss << "\"transport_state\": " << mTransportState << "," << std::endl;
		ss << "\"record_mode\": " << mRecordMode << "," << std::endl;
		ss << "\"is_record_enabled\": " << mIsRecordEnabled << "," << std::endl;
		ss << "\"is_recording\": " << mIsRecording << "," << std::endl;
		ss << "\"is_loop_enabled\": " << mIsLoopEnabled << std::endl;
		ss << "}";

		return ss.str();
	}
};

#include AAX_ALIGN_FILE_BEGIN
#include AAX_ALIGN_FILE_RESET
#include AAX_ALIGN_FILE_END

inline bool operator==(const AAX_TransportStateInfo_V1& state1, const AAX_TransportStateInfo_V1& state2)
{
	return (state1.mTransportState == state2.mTransportState) && (state1.mRecordMode == state2.mRecordMode) &&
		(state1.mIsRecordEnabled == state2.mIsRecordEnabled) && (state1.mIsRecording == state2.mIsRecording) &&
		(state1.mIsLoopEnabled == state2.mIsLoopEnabled);
}

inline bool operator!=(const AAX_TransportStateInfo_V1& state1, const AAX_TransportStateInfo_V1& state2)
{
	return !(state1 == state2);
}

#endif // #ifndef AAX_TransportTypes_h_
