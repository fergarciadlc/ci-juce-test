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
 *  \file AAX_MIDIUtilities.h
 *
 *	\brief Utilities for managing MIDI data
 *
 */
/*================================================================================================*/
/// @cond ignore
#ifndef AAX_MIDIUtilities_h
#define AAX_MIDIUtilities_h
/// @endcond

// AAX Includes
#include "AAX.h"

namespace AAX
{
	//
	// Some MIDI defines
	//
	
	/// Values for the status nibble in a MIDI packet
	enum EStatusNibble
	{
		 eStatusNibble_NoteOff = 0x80
		,eStatusNibble_NoteOn = 0x90
		,eStatusNibble_KeyPressure = 0xA0
		,eStatusNibble_ControlChange = 0xB0
		,eStatusNibble_ChannelMode = 0xB0
		,eStatusNibble_ProgramChange = 0xC0
		,eStatusNibble_ChannelPressure = 0xD0
		,eStatusNibble_PitchBend = 0xE0
		,eStatusNibble_SystemCommon = 0xF0
		,eStatusNibble_SystemRealTime = 0xF0
	};
	
	/// Values for the status byte in a MIDI packet
	enum EStatusByte
	{
		 eStatusByte_SysExBegin = 0xF0
		,eStatusByte_MTCQuarterFrame = 0xF1
		,eStatusByte_SongPosition = 0xF2
		,eStatusByte_SongSelect = 0xF3
		,eStatusByte_TuneRequest = 0xF6
		,eStatusByte_SysExEnd = 0xF7
		,eStatusByte_TimingClock = 0xF8
		,eStatusByte_Start = 0xFA
		,eStatusByte_Continue = 0xFB
		,eStatusByte_Stop = 0xFC
		,eStatusByte_ActiveSensing = 0xFE
		,eStatusByte_Reset = 0xFF
	};
	
	/// Values for the first data byte in a Channel Mode Message MIDI packet
	enum EChannelModeData
	{
		 eChannelModeData_AllSoundOff = 120
		,eChannelModeData_ResetControllers = 121
		,eChannelModeData_LocalControl = 122
		,eChannelModeData_AllNotesOff = 123
		,eChannelModeData_OmniOff = 124
		,eChannelModeData_OmniOn = 125
		,eChannelModeData_PolyOff = 126
		,eChannelModeData_PolyOn = 127
	};
	
	/// Special message data for the first data byte in a message
	enum ESpecialData
	{
		 eSpecialData_AccentedClick = 0x00 ///< For use when the high status nibble is \ref eStatusNibble_NoteOn and the low status nibble is zero
		,eSpecialData_UnaccentedClick = 0x01 ///< For use when the high status nibble is \ref eStatusNibble_NoteOn and the low status nibble is zero
	};
	
	
	//
	// Basic MIDI utility functions
	//
	
	/// Returns true if \c inPacket is a Note On message
	inline bool IsNoteOn(const AAX_CMidiPacket* inPacket)
	{
		if (!inPacket) { return false; }
		const uint8_t sn = (inPacket->mData[0] & 0xF0); // status nibble
		const uint8_t data2 = inPacket->mData[2];
		return ((eStatusNibble_NoteOn == sn) &&
				(0x00 != data2));
	}
	
	/// Returns true if \c inPacket is a Note Off message, or a Note On message with velocity zero
	inline bool IsNoteOff(const AAX_CMidiPacket* inPacket)
	{
		if (!inPacket) { return false; }
		const uint8_t sn = (inPacket->mData[0] & 0xF0); // status nibble
		const uint8_t data2 = inPacket->mData[2];
		return ((eStatusNibble_NoteOff == sn) || ((eStatusNibble_NoteOn == sn) && (0x00 == data2)));
	}
	
	/// Returns true if \c inPacket is an All Sound Off or All Notes Off message
	inline bool IsAllNotesOff(const AAX_CMidiPacket* inPacket)
	{
		if (!inPacket) { return false; }
		const uint8_t sn = (inPacket->mData[0] & 0xF0); // status nibble
		const uint8_t data1 = inPacket->mData[1];
		const uint8_t data2 = inPacket->mData[2];
		if (eStatusNibble_ChannelMode == sn)
		{
			if (eChannelModeData_PolyOff == data1)
			{
				return true;
			}
			else if ((eChannelModeData_AllSoundOff == data1) ||
					 (eChannelModeData_AllNotesOff == data1) ||
					 (eChannelModeData_OmniOff == data1) ||
					 (eChannelModeData_OmniOn == data1) ||
					 (eChannelModeData_PolyOn == data1))
			{
				return (0x00 == data2);
			}
		}
		
		return false;
	}
	
	/// Returns true if \c inPacket is a special Pro Tools accented click message
	inline bool IsAccentedClick(const AAX_CMidiPacket* inPacket)
	{
		return ((inPacket) &&
				(eStatusNibble_NoteOn == (inPacket->mData[0] & 0xF0)) &&
				(0x00 == (inPacket->mData[0] & 0x0F)) &&
				(eSpecialData_AccentedClick == inPacket->mData[1]));
	}
	
	/// Returns true if \c inPacket is a special Pro Tools unaccented click message
	inline bool IsUnaccentedClick(const AAX_CMidiPacket* inPacket)
	{
		return ((inPacket) &&
				(eStatusNibble_NoteOn == (inPacket->mData[0] & 0xF0)) &&
				(0x00 == (inPacket->mData[0] & 0x0F)) &&
				(eSpecialData_UnaccentedClick == inPacket->mData[1]));
	}
	
	/// Returns true if \c inPacket is a special Pro Tools click message
	inline bool IsClick(const AAX_CMidiPacket* inPacket)
	{
		return (IsAccentedClick(inPacket) || IsUnaccentedClick(inPacket));
	}
} // namespace AAX

/// @cond ignore
#endif // AAX_MIDIUtilities_h
/// @endcond
