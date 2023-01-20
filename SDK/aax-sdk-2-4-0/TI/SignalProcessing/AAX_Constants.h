/*================================================================================================*/
/*
 *	Copyright 2009-2015 by Avid Technology, Inc.
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
 *	\file  AAX_Constants.h
 *
 *	\brief Signal processing constants
 *
 */ 
/*================================================================================================*/
#pragma once

#ifndef AAX_CONSTANTS_H
#define AAX_CONSTANTS_H


/* the following lines were re-introduced on 6/11/09 because
	the FFmt project still uses SInt32 types	*/
#ifdef _TMS320C6X
	typedef   signed int  SInt32;
#else
//	#include "DigiPublicTypes.h"
#endif
/* end 6/11/09 changes */


// Standard headers
#include <cmath>

namespace AAX
{

#if __BIG_ENDIAN__
	const int cBigEndian=1;
	const int cLittleEndian=0;
#else
	const int cBigEndian=0;
	const int cLittleEndian=1;
#endif

const double cPi			= 3.1415926535897932384626433832795;
const double cTwoPi			= 6.2831853071795862319959269370884;  //2.0*3.1415926535897932384626433832795;
const double cHalfPi		= 1.5707963267948965579989817342721;  //0.5*3.1415926535897932384626433832795;
const double cQuarterPi		= 0.78539816339744827899949086713605; //0.25*3.1415926535897932384626433832795;
const double cRootTwo		= 1.4142135623730950488016887242097;
const double cOneOverRootTwo= 0.70710678118654752440084436210485;


//Obviously these numbers are are not exact.
const double cPos3dB=1.4142135623730950488016887242097;
const double cNeg3dB=0.70710678118654752440084436210485;
const double cPos6dB=2.0;
const double cNeg6dB=0.5;

const double cNormalizeLongToAmplitudeOneHalf	= 0.00000000023283064365386962890625; //1.0/double(1LL<<32LL);
const double cNormalizeLongToAmplitudeOne		= 1.0/double(1<<31); //-0.0000000004656612873077392578125;  

const double cMilli=0.001;
const double cMicro=0.001*0.001;
const double cNano=0.001*0.001*0.001;
const double cPico=0.001*0.001*0.001*0.001;

const double cKilo=1000.0;
const double cMega=1000.0*1000.0;
const double cGiga=1000.0*1000.0*1000.0;

enum ESampleRates
{
	e44100SampleRate = 44100,
	e48000SampleRate = 48000,
	e88200SampleRate = 88200,
	e96000SampleRate = 96000,
	e176400SampleRate = 176400,
	e192000SampleRate = 192000
};

} // namespace AAX

#endif // AAX_CONSTANTS_H

