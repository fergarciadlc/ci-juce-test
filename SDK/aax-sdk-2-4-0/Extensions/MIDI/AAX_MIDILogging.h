/*================================================================================================*/
/*
 *	Copyright 2015 by Avid Technology, Inc.
 *	All rights reserved.
 *
 *	CONFIDENTIAL: This document contains confidential information.  Do not 
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
 *	OF Avid Technology, INC.
 */

/**
 *  \file AAX_MIDILogging.h
 *
 *	\brief	Utilities for logging MIDI data
 */
/*================================================================================================*/

/// @cond ignore
#ifndef AAX_MIDILOGGING_H
#define AAX_MIDILOGGING_H
/// @endcond

// AAX Includes
#include "AAX.h"

namespace AAX
{
	/** @name MIDI logging utilities
	 */
	//@{
	/** Print a MIDI stream as a C-string
		
		@details
		Sets an empty string in release builds
		
		@ingroup OtherExtensions
	*/
	void AsStringMIDIStream_Debug(const AAX_CMidiStream& inStream, char* outBuffer, int32_t inBufferSize);
	//@} MIDI logging utilities
}

/// @cond ignore
#endif // AAX_MIDILOGGING_H
/// @endcond
