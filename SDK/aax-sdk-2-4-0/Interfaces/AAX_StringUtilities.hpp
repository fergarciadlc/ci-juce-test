/*================================================================================================*/
/*
 *	Copyright 2014-2017, 2019-2021 by Avid Technology, Inc.
 *	All rights reserved.
 *
 *	CONFIDENTIAL: This document contains confidential information.  Do not 
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
 *	OF Avid Technology, INC.
 */
/*================================================================================================*/

#ifndef AAXLibrary_AAX_StringUtilities_hpp
#define AAXLibrary_AAX_StringUtilities_hpp

#include "AAX_IString.h"
#include "AAX_Errors.h"
#include "AAX_Assert.h"
#include <stdlib.h>

#include <algorithm>
#include <vector>
#include <string>
#include <cstring>

//===========================================================================
//
//	FloatToString:	Convert the given floating point number to a pascal string.
//
//===========================================================================
/*
void	FloatToString(float aNumber, StringPtr aString)
{
	Str255			MantissaStr;
	double		aDouble;
	StringPtr		tempStr;
	int32_t			mantissa,tens,hundreds;
	int16_t			count;

	aDouble = (double) aNumber;
	if (aNumber < 0.0)  // take abs value
		aDouble = -aDouble;
	
	aDouble += 0.005;		// perform rounding by adding 1/2 of the hundreths digit
	
	mantissa = aDouble;
	tens = (aDouble * 10.0) - (mantissa * 10.0);
	hundreds = (aDouble * 100.0) - (mantissa * 100.0) - (tens * 10.0);

	NumToString(mantissa, MantissaStr);
	
	// set up string length
	if (aNumber < 0.0)
		*aString++ = (char) (1 + 3 + *MantissaStr);
	else
		*aString++ = (char) (3 + *MantissaStr);
	
	tempStr = MantissaStr;
	
	// copy mantissa first
	count = *tempStr++;
	
	if (aNumber < 0.0)
		*aString++ = '-';

	while (count--)
		*aString++ = *tempStr++;

	*aString++ = '.';
	*aString++ = (char) (tens + '0');
	*aString++ = (char) (hundreds + '0');
}
*/

///==============================================================
//
//  GetCStringOfLength
//
// A routine for selecting a string based on the size passed in
// by the client.  If none of the strings are short enough then
// the shortest string is truncated to fit.
//
// stringIn="A Very Nice String\nA String\nAStrng\nStr\n";
//
// Submitted from Erik Gavriluk of BombFactory (Free of Charge)
// Debugged and separator character changed by Frederick Umminger
//===============================================================

void AAX::GetCStringOfLength(char *s_out, const char* s_in, int32_t aMaxChars)
{
	AAX_ASSERT(0 < aMaxChars);
	
	const char kSeparator = '\n';
	
	if(s_in)
	{
		const char* s_begin = s_in;
		const char* s_end = s_begin;
		while(s_begin)
		{
			// Count characters in current substring
			while((*s_end != kSeparator) && (*s_end != '\0'))
			{
				s_end++;
			};
			
			// If substring is less than or equal to aMaxChars then use it.
			if((s_end-s_begin <= aMaxChars) || (*s_end=='\0'))
			{
				break;
			}
			
			s_begin = ++s_end;
		}
		// We don't use strncpy in order to make sure a '\0' gets put on the end of s_out
		*s_out = '\0';
		const int32_t length = int32_t(s_end-s_begin);
		if (0 < length && 0 < aMaxChars)
		{
			std::strncat(s_out, s_begin, static_cast<std::size_t>(std::max<int32_t>(0, std::min<int32_t>(aMaxChars,length))));
		}
	}
	else if (0 < aMaxChars)
	{
		strncpy(s_out, "", static_cast<size_t>(aMaxChars));
	};
}

int32_t AAX::Caseless_strcmp(const char* cs, const char* ct)
{
	if(cs)
	{
		if(ct)
		{
			while(*cs && *ct)
			{
				int32_t cmp = toupper(*ct++) - toupper(*cs++);
				if(cmp) return cmp;
			};
			if(*cs)
			{
				return -1;
			}
			else
			{
				if(*ct)
				{
					return 1;
				}
				else
				{
					return 0;
				};
			};
		}
		else
		{
			return -1;
		};
	}
	else
	{
		if(ct)
			return 1;
		else
			return 0;
	}
	
}


std::string AAX::Binary2String(uint32_t value, int32_t numBits)
{
	std::string s;
	
	uint32_t currentBitMask = (static_cast<uint32_t>(0x1) << (numBits-1));
	
	while (currentBitMask != 0)
	{
		if (currentBitMask & value)
		{
			s += "1";
		}
		else
		{
			s += "0";
		};
		currentBitMask >>= 1;
	}
	return s;
}

uint32_t AAX::String2Binary(const AAX_IString& s)
{
	uint32_t value = 0;
	
	const char* const cS = s.Get();
	int32_t length = int32_t(s.Length());
	for(int32_t i = 0; i < length ; i++)
	{
		switch(cS[i])
		{
			case '0':
				break;
			case '1':
				value |= (0x1 << (length-1-i));
				break;
			default:
				AAX_ASSERT('0' == cS[i] || '1' == cS[i]);
		};
	};
	
	return value;
}

bool AAX::IsASCII(char inChar)
{
	return (0x20 <= inChar) && (0x7E >= inChar);
}

bool AAX::IsFourCharASCII(uint32_t inFourChar)
{
	const uint32_t oneCharMask = 0x000000FF;
	const size_t oneCharNumBits = 8;
	
	bool result = true;
	for (uint16_t i = 3; true == result /* i value checked within loop */; --i)
	{
		const char curChar = static_cast<const char>((inFourChar >> (i*oneCharNumBits)) & oneCharMask);
		result = result && IsASCII(curChar);
		if (0 == i) { break; }
	}
	return result;
}

std::string AAX::AsStringFourChar(uint32_t inFourChar)
{
	const uint32_t oneCharMask = 0x000000FF;
	const int16_t oneCharNumBits = 8;
	const std::string unknownChar = "(?)";
	
	std::string resultStr("");
	for (int16_t i = 3; i >= 0; --i)
	{
		const char curChar = static_cast<char>((inFourChar >> (i*oneCharNumBits)) & oneCharMask);
		resultStr += IsASCII(curChar) ? std::string(1, curChar) : unknownChar;
	}
	return resultStr;
}

std::string AAX::AsStringPropertyValue(AAX_EProperty inProperty, AAX_CPropertyValue inPropertyValue)
{
	// 63 characters should be plenty for any of the formats below, but
	// increase this value if the formats are expanded in the future.
	const size_t kBufferSize = 64;
	char buffer[kBufferSize];
	
	// Attempt to infer a sensible way to print the property
	if (AAX_eProperty_SampleRate == inProperty ||
		AAX_eProperty_Constraint_Location == inProperty)
	{
		// Print specific properties' values as bitfield
		
		// We want the exact bits, so we memcpy to avoid any potential issues
		// with casting from signed to unsigned
		uint32_t bitfield;
		memcpy(&bitfield, &inPropertyValue, sizeof(uint32_t));
		
		const int32_t maxNumBitsToShow = 8; // Currently there are no bitfield properties with more than 8 possible flags
		sprintf(buffer, "%s", AAX::Binary2String(bitfield, maxNumBitsToShow).c_str());
	}
	else if (true == AAX::IsFourCharASCII(static_cast<uint32_t>(inPropertyValue)))
	{
		// Print values in ASCII range as four-char
		sprintf(buffer, "'%s'", AAX::AsStringFourChar(static_cast<uint32_t>(inPropertyValue)).c_str());
	}
	else if (0x00FFFFFF < abs(inPropertyValue))
	{
		// Print values with most bits used as hex
		sprintf(buffer, "%#x", inPropertyValue);
	}
	else
	{
		// Otherwise, print as simple decimal
		sprintf(buffer, "%ld", (long int)inPropertyValue);
	}
	
	buffer[kBufferSize-1] = '\0'; // Just in case
	return std::string(buffer);
}

std::string	AAX::AsStringInt32(int32_t inInt32)
{
	const size_t kBufferSize = 16;
	char buffer[kBufferSize];
	sprintf(buffer, "%ld", (long int)inInt32);
	return std::string(buffer);
}

std::string	AAX::AsStringUInt32(uint32_t inUInt32)
{
	const size_t kBufferSize = 16;
	char buffer[kBufferSize];
	sprintf(buffer, "%lu", (unsigned long)inUInt32);
	return std::string(buffer);
}

std::string AAX::AsStringIDTriad(const AAX_SPlugInIdentifierTriad& inIDTriad)
{
	std::string result = "(";
	
	result += "man: '" + AAX::AsStringFourChar(inIDTriad.mManufacturerID) + "', ";
	result += "prod: '" + AAX::AsStringFourChar(inIDTriad.mProductID) + "', ";
	result += "type: '" + AAX::AsStringFourChar(inIDTriad.mPlugInID) + "'";
	
	result += ")";
	return result;
}

std::string AAX::AsStringStemFormat(AAX_EStemFormat inStemFormat, bool inAbbreviate)
{
	switch (inStemFormat)
	{
		case AAX_eStemFormat_Mono: { return std::string("Mono"); break; }
		case AAX_eStemFormat_Stereo: { return std::string(inAbbreviate ? "St" : "Stereo"); break; }
		case AAX_eStemFormat_LCR: { return std::string("LCR"); break; }
		case AAX_eStemFormat_LCRS: { return std::string("LCRS"); break; }
		case AAX_eStemFormat_Quad: { return std::string("Quad"); break; }
		case AAX_eStemFormat_5_0: { return std::string("5.0"); break; }
		case AAX_eStemFormat_5_1: { return std::string("5.1"); break; }
		case AAX_eStemFormat_6_0: { return std::string("6.0"); break; }
		case AAX_eStemFormat_6_1: { return std::string("6.1"); break; }
		case AAX_eStemFormat_7_0_SDDS: { return std::string(inAbbreviate ? "7.0 S" : "7.0 SDDS"); break; }
		case AAX_eStemFormat_7_1_SDDS: { return std::string(inAbbreviate ? "7.1 S" : "7.1 SDDS"); break; }
		case AAX_eStemFormat_7_0_DTS: { return std::string("7.0"); break; }
		case AAX_eStemFormat_7_1_DTS: { return std::string("7.1"); break; }
		case AAX_eStemFormat_7_0_2: {return std::string("7.0.2"); break; }
		case AAX_eStemFormat_7_1_2: { return std::string("7.1.2"); break; }
		case AAX_eStemFormat_Ambi_1_ACN: { return std::string(inAbbreviate ? "Amb1" : "Ambisonics (1st Order)"); break; }
		case AAX_eStemFormat_Ambi_2_ACN: { return std::string(inAbbreviate ? "Amb2" : "Ambisonics (2nd Order)"); break; }
		case AAX_eStemFormat_Ambi_3_ACN: { return std::string(inAbbreviate ? "Amb3" : "Ambisonics (3rd Order)"); break; }
			
		case AAX_eStemFormat_Reserved_1: { return std::string(inAbbreviate ? "Res1" : "Reserved (1)"); break; }
		case AAX_eStemFormat_Reserved_2: { return std::string(inAbbreviate ? "Res2" : "Reserved (2)"); break; }
		case AAX_eStemFormat_Reserved_3: { return std::string(inAbbreviate ? "Res3" : "Reserved (3)"); break; }
			
		case AAX_eStemFormat_None: { return std::string("None"); break; }
		case AAX_eStemFormat_Any: { return std::string("Any"); break; }
			
		case AAX_eStemFormat_INT32_MAX:
		case AAX_eStemFormatNum:
		default: { return std::string(inAbbreviate ? "unk" : "unknown stem format"); break; }
	}
}

std::string AAX::AsStringStemChannel(AAX_EStemFormat inStemFormat, uint32_t inChannelIndex, bool inAbbreviate)
{
	switch (inStemFormat)
	{
		case AAX_eStemFormat_Mono:
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "M" : "Audio"); }
			break;
		case AAX_eStemFormat_Stereo:
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "L" : "Left"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "R" : "Right"); }
			break;
		case AAX_eStemFormat_LCR:
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "L" : "Left"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "C" : "Center"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "R" : "Right"); }
			break;
		case AAX_eStemFormat_LCRS:
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "L" : "Left"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "C" : "Center"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "R" : "Right"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "S" : "Surround"); }
			break;
		case AAX_eStemFormat_Quad:
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "L" : "Left"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "R" : "Right"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Ls" : "Left Surround"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rs" : "Right Surround"); }
			break;
		case AAX_eStemFormat_5_0:
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "L" : "Left"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "C" : "Center"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "R" : "Right"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Ls" : "Left Surround"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rs" : "Right Surround"); }
			break;
		case AAX_eStemFormat_5_1:
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "L" : "Left"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "C" : "Center"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "R" : "Right"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Ls" : "Left Surround"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rs" : "Right Surround"); }
			if (0 == inChannelIndex--) { return std::string("LFE"); }
			break;
		case AAX_eStemFormat_6_0:
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "L" : "Left"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "C" : "Center"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "R" : "Right"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Ls" : "Left Surround"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Cs" : "Center Surround"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rs" : "Right Surround"); }
			break;
		case AAX_eStemFormat_6_1:
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "L" : "Left"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "C" : "Center"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "R" : "Right"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Ls" : "Left Surround"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Cs" : "Center Surround"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rs" : "Right Surround"); }
			if (0 == inChannelIndex--) { return std::string("LFE"); }
			break;
		case AAX_eStemFormat_7_0_SDDS:
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "L" : "Left"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Lc" : "Left Center"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "C" : "Center"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rc" : "Right Center"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "R" : "Right"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Ls" : "Left Surround"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rs" : "Right Surround"); }
			break;
		case AAX_eStemFormat_7_1_SDDS:
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "L" : "Left"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Lc" : "Left Center"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "C" : "Center"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rc" : "Right Center"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "R" : "Right"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Ls" : "Left Surround"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rs" : "Right Surround"); }
			if (0 == inChannelIndex--) { return std::string("LFE"); }
			break;
		case AAX_eStemFormat_7_0_DTS:
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "L" : "Left"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "C" : "Center"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "R" : "Right"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Lss" : "Left Surround Side"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rss" : "Right Surround Side"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Lsr" : "Left Surround Rear"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rsr" : "Right Surround Rear"); }
			break;
		case AAX_eStemFormat_7_1_DTS:
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "L" : "Left"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "C" : "Center"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "R" : "Right"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Lss" : "Left Surround Side"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rss" : "Right Surround Side"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Lsr" : "Left Surround Rear"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rsr" : "Right Surround Rear"); }
			if (0 == inChannelIndex--) { return std::string("LFE"); }
			break;
        case AAX_eStemFormat_7_0_2:
            if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "L" : "Left"); }
            if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "C" : "Center"); }
            if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "R" : "Right"); }
            if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Lss" : "Left Surround Side"); }
            if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rss" : "Right Surround Side"); }
            if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Lsr" : "Left Surround Rear"); }
            if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rsr" : "Right Surround Rear"); }
            if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "LTS" : "Left Top Surround"); }
            if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "RTS" : "Right Top Surround"); }
            break;
		case AAX_eStemFormat_7_1_2:
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "L" : "Left"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "C" : "Center"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "R" : "Right"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Lss" : "Left Surround Side"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rss" : "Right Surround Side"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Lsr" : "Left Surround Rear"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "Rsr" : "Right Surround Rear"); }
			if (0 == inChannelIndex--) { return std::string("LFE"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "LTS" : "Left Top Surround"); }
			if (0 == inChannelIndex--) { return std::string(inAbbreviate ? "RTS" : "Right Top Surround"); }
			break;
		case AAX_eStemFormat_Ambi_1_ACN:
			if (0 == inChannelIndex--) { return std::string("1"); }
			if (0 == inChannelIndex--) { return std::string("2"); }
			if (0 == inChannelIndex--) { return std::string("3"); }
			if (0 == inChannelIndex--) { return std::string("4"); }
			break;
		case AAX_eStemFormat_Ambi_2_ACN:
			if (0 == inChannelIndex--) { return std::string("1"); }
			if (0 == inChannelIndex--) { return std::string("2"); }
			if (0 == inChannelIndex--) { return std::string("3"); }
			if (0 == inChannelIndex--) { return std::string("4"); }
			if (0 == inChannelIndex--) { return std::string("5"); }
			if (0 == inChannelIndex--) { return std::string("6"); }
			if (0 == inChannelIndex--) { return std::string("7"); }
			if (0 == inChannelIndex--) { return std::string("8"); }
			if (0 == inChannelIndex--) { return std::string("9"); }
			break;
		case AAX_eStemFormat_Ambi_3_ACN:
			if (0 == inChannelIndex--) { return std::string("1"); }
			if (0 == inChannelIndex--) { return std::string("2"); }
			if (0 == inChannelIndex--) { return std::string("3"); }
			if (0 == inChannelIndex--) { return std::string("4"); }
			if (0 == inChannelIndex--) { return std::string("5"); }
			if (0 == inChannelIndex--) { return std::string("6"); }
			if (0 == inChannelIndex--) { return std::string("7"); }
			if (0 == inChannelIndex--) { return std::string("8"); }
			if (0 == inChannelIndex--) { return std::string("9"); }
			if (0 == inChannelIndex--) { return std::string("10"); }
			if (0 == inChannelIndex--) { return std::string("11"); }
			if (0 == inChannelIndex--) { return std::string("12"); }
			if (0 == inChannelIndex--) { return std::string("13"); }
			if (0 == inChannelIndex--) { return std::string("14"); }
			if (0 == inChannelIndex--) { return std::string("15"); }
			if (0 == inChannelIndex--) { return std::string("16"); }
			break;
			
		case AAX_eStemFormat_Reserved_1:
			break;
		case AAX_eStemFormat_Reserved_2:
			break;
		case AAX_eStemFormat_Reserved_3:
			break;
			
		case AAX_eStemFormat_None:
			break;
		case AAX_eStemFormat_Any:
			break;

		case AAX_eStemFormat_INT32_MAX:
		case AAX_eStemFormatNum:
		default:
			break;
	}
	
	return std::string(inAbbreviate ? "?" : "unknown");
}

std::string AAX::AsStringResult(AAX_Result inResult)
{
#ifdef DEFINE_AAX_ERROR_STRING
#undef DEFINE_AAX_ERROR_STRING
#endif
#define DEFINE_AAX_ERROR_STRING(X) if (X == inResult) { return std::string(#X); }
	
	DEFINE_AAX_ERROR_STRING(AAX_SUCCESS);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_INVALID_PARAMETER_ID);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_INVALID_STRING_CONVERSION);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_INVALID_METER_INDEX);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_NULL_OBJECT);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_OLDER_VERSION);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_INVALID_CHUNK_INDEX);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_INVALID_CHUNK_ID);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_INCORRECT_CHUNK_SIZE);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_UNIMPLEMENTED);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_INVALID_PARAMETER_INDEX);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_NOT_INITIALIZED);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_ACF_ERROR);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_INVALID_METER_TYPE);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_CONTEXT_ALREADY_HAS_METERS);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_NULL_COMPONENT);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_PORT_ID_OUT_OF_RANGE);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_FIELD_TYPE_DOES_NOT_SUPPORT_DIRECT_ACCESS);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_DIRECT_ACCESS_OUT_OF_BOUNDS);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_FIFO_FULL);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_INITIALIZING_PACKET_STREAM_THREAD);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_POST_PACKET_FAILED);
	DEFINE_AAX_ERROR_STRING(AAX_RESULT_PACKET_STREAM_NOT_EMPTY);
	DEFINE_AAX_ERROR_STRING(AAX_RESULT_ADD_FIELD_UNSUPPORTED_FIELD_TYPE);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_MIXER_THREAD_FALLING_BEHIND);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_INVALID_FIELD_INDEX);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_MALFORMED_CHUNK);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_TOD_BEHIND);
	DEFINE_AAX_ERROR_STRING(AAX_RESULT_NEW_PACKET_POSTED);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_PLUGIN_NOT_AUTHORIZED);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_PLUGIN_NULL_PARAMETER);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_NOTIFICATION_FAILED);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_INVALID_VIEW_SIZE);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_SIGNED_INT_OVERFLOW);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_NO_COMPONENTS);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_DUPLICATE_EFFECT_ID);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_DUPLICATE_TYPE_ID);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_EMPTY_EFFECT_NAME);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_UNKNOWN_PLUGIN);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_PROPERTY_UNDEFINED);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_INVALID_PATH);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_UNKNOWN_ID);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_UNKNOWN_EXCEPTION);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_INVALID_ARGUMENT);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_NULL_ARGUMENT);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_INVALID_INTERNAL_DATA);
	DEFINE_AAX_ERROR_STRING(AAX_ERROR_ARGUMENT_BUFFER_OVERFLOW);
	
	if (AAX_ERROR_PLUGIN_BEGIN >= inResult && AAX_ERROR_PLUGIN_END <= inResult)
		return std::string("plug-in defined error");
	
	return std::string("<unknown error code>");
}

#endif
