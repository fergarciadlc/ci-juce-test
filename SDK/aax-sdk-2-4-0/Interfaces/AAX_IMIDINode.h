/*================================================================================================*/
/*
 *	Copyright 2014-2017 by Avid Technology, Inc.
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
 *  \file  AAX_IMIDINode.h
 *
 *	\brief Declaration of the base MIDI Node interface.
 *
 *	\author by Andriy Goshko
 */
/*================================================================================================*/
/// @cond ignore
#pragma once
#ifndef AAX_IMIDINODE_H
#define AAX_IMIDINODE_H
/// @endcond

#include "AAX.h"
#include "AAX_ITransport.h"

/** \brief Interface for accessing information in a MIDI node
 *	
 *	\details
 *	\hostimp
 *	
 *	\sa AAX_IComponentDescriptor::AddMIDINode
 */
class AAX_IMIDINode 
{
public:
	virtual	~AAX_IMIDINode() {}

	/** \brief Returns a MIDI stream data structure
	 *
	 *	
	 */
	virtual AAX_CMidiStream*		GetNodeBuffer () = 0;
	
	/** \brief Posts an \ref AAX_CMidiPacket to an output MIDI node
	 *
	 *	\compatibility Pro Tools supports the following MIDI events from plug-ins:
	 *	- NoteOn
	 *	- NoteOff
	 *	- Pitch bend
	 *	- Polyphonic key pressure
	 *	- Bank select (controller #0)
	 *	- Program change (no bank)
	 *	- Channel pressure
	 *
	 *
	 *	\param[in] packet
	 *		The MIDI packet to be pushed to a MIDI output node
	 */
	virtual AAX_Result			PostMIDIPacket (AAX_CMidiPacket *packet) = 0;
	
	/** \brief Returns a transport object
	 *
	 */
	virtual AAX_ITransport*		GetTransport () = 0;
};


/// @cond ignore
#endif // AAX_IMIDINODE_H
/// @endcond
