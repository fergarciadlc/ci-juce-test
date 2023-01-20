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
 *	\file  AAX_FastPow.h
 *
 *	\brief Set of functions to optimize pow
 *
 *	To use:
 *	\code
 *	 const int kPowTableExtent = 9;		// Lower values are less precise. 9 is the maximum
 *	 float powTableH[kPowTableSize];
 *	 float powTableL[kPowTableSize];
 *	 int radix = 2;						// This should be whatever radix you want. Ie: radix ^ exp
 *	 PowFastSetTable( powTableH, kPowExtent, kPowExtent, false ); // Set the high table
 *	 PowFastSetTable( powTableL, kPowExtent*2, kPowExtent, true );// Set the low table
 *	 ..
 *	 result = powFastLookup(exp, log(2) / log(radix), powTableH, powTableL);
 *	\endcode
 *
 */ 
/*================================================================================================*/




#pragma once

#ifndef _AAX_FASTPOW_H_
#define _AAX_FASTPOW_H_

#include <cmath> 
#if defined(MAC_VERSION) || defined(LINUX_VERSION)
namespace std  {using ::powf;using ::log10f;using ::fabsf;}
#endif
#include "AAX.h"	// For AAX_RESTRICT

namespace AAX
{
static const float _2p23 = 8388608.0f;
const unsigned int kPowExtent = 9;	// 9 results in ~-80dB cancellation
const unsigned int kPowTableSize = 1 << kPowExtent;
	
/*!
 *  \brief Initialize powFast lookup table
 *
 *	\sa \ref PowFastLookup()
 *  
 *  \param[in] pTable
 *		length must be 2 ^ precision
 *  \param[in] precision
 *		Number of mantissa bits used, >=0 and <= 18
 *  \param[in] extent
 *		Table size, >=0 and <= 9
 *  \param[in] isRound
 *		\todo Fill in...is this the lower rounding table?? Truncate/round table...
 */	
static inline void PowFastSetTable(
	float* const AAX_RESTRICT pTable,
	const unsigned int precision,
	const unsigned int extent,
	const bool isRound)
{
   // step along table elements and x-axis positions
   float zeroToOne = !isRound ?
      0.0f : (1.0f / (static_cast<float>(1 << precision) * 2.0f));
   for( int i = 0;  i < (1 << extent);  ++i )
   {
      // make y-axis value for table element
      pTable[i] = std::powf( 2.0f, zeroToOne );

      zeroToOne += 1.0f / static_cast<float>(1 << precision);
   }
}
	
/*!
 *  \brief Get pow (fast!)
 *
 *	Example use of this method to perform efficient dB-to-linear conversion:
 *	\code
 *	//
 *	// Constants
 *	//
 *
 *	// Tables generated using AAX::PowFastSetTable() with precision of 9 mantissa bits
 *	// remember to use AAX_ALIGN_FILE_ALG when defining tables like these
 *	const float gmPowTableH[1 << AAX::kPowExtent];
 *	const float gmPowTableL[1 << AAX::kPowExtent];
 *
 *	// Example initialization (this can be done at or before compilation)
 *	AAX::PowFastSetTable(gmPowTableH, AAX::kPowExtent, AAX::kPowExtent, false );
 *	AAX::PowFastSetTable(gmPowTableL, AAX::kPowExtent*2, AAX::kPowExtent, true );
 *
 *	// Other constants
 *	const float ilog2Div20 = 3.32192809488736f * 0.05f;
 *
 *
 *	//
 *	// Inside the alg processing callback...
 *	//
 *
 *	// Sample-by-sample meter processing
 *	{
 *	// Convert from dB to linear
 *	float compressionGainLinear = AAX::PowFastLookup(compressionGainInDB, ilog2Div20, gmPowTableH, gmPowTableL);
 *
 *	// Subtract from 1 so it is 0 -> 1 based instead of 1 -> 0
 *	meterValCLGain = AAX::Max(1.0f – compressionGainLinear, meterValCLGain);
 *	}
 *	\endcode
 *  
 *  \param[in] val
 *		The power to raise radix to
 *  \param[in] ilog2
 *		Document: How to change base  dynamics_algproc.cpp  One over log, to required radix, of two
 *  \param[in] tableH
 *		A call to powfastsettable must be made to generate high table (see top comment).  The length must be 2 ^ 9
 *  \param[in] tableL
 *		A call to powfastsettable must be made to generate low table (see top comment).   The length must be 2 ^ 9
 */	
static inline float PowFastLookup(
	const float         val,
	const float         ilog2,
	const float* const AAX_RESTRICT tableH,
	const float* const AAX_RESTRICT tableL)
{

	// build float bits
	const int i = static_cast<int>( (val * (_2p23 * ilog2)) + (127.0f * _2p23) );

	// replace mantissa with combined lookups
	const float t  = tableH[(i >> 14) & 0x1FF] * tableL[(i >> 5) & 0x1FF];
	const int   it = (i & 0xFF800000) |
	  (*reinterpret_cast<const int*>( &t ) & 0x7FFFFF);

	// convert bits to float
	return *reinterpret_cast<const float*>( &it );

}

/*!
 *  \brief Get 2.0f^x (fast!)
 *
 *  
 *  \param[in] val
 *		The power to raise 2 to
 *  \param[in] tableH
 *		The length must be 2 ^ 9
 *  \param[in] tableL
 *		The length must be 2 ^ 9
 */	
static inline float Pow2FastLookup(
	const float         val,
	const float* const AAX_RESTRICT tableH,
	const float* const AAX_RESTRICT tableL)
{

	// build float bits
	const int i = static_cast<int>((val + 127.0f) * _2p23);

	// replace mantissa with combined lookups
	const float t  = tableH[(i >> 14) & 0x1FF] * tableL[(i >> 5) & 0x1FF];
	const int   it = (i & 0xFF800000) |
	  (*reinterpret_cast<const int*>( &t ) & 0x007FFFFF);

	// convert bits to float
	return *reinterpret_cast<const float*>( &it );

}
//@}end Fast power functions


static const int kMantissaBitSize = 23;
static const int kExpBias = 127;
static const int kLogMantissaMSBs = 9;
static const int kLogTableSize = (1 << kLogMantissaMSBs) + 1;// 513;	

/*!
 *  \brief Description
 *
 *		Description
 *  
 *  \param[in] logTable
 *		\todo Describe
 *  \param[in] tableSize
 *		\todo Describe
 */	
static inline void LogFastSetTable(
	float* const AAX_RESTRICT logTable, 
	int tableSize = kLogTableSize)
{
	const float kInv = 1.0f/float(tableSize - 1); // 0.001953125
	float mantissaVal = 1.0f;	//Start here.
	const float invLog10Base2 = 1.0f / std::log10f(2.0f);
	for (int j = 0; j < tableSize; j++)
	{
		logTable[j] = std::log10f(mantissaVal) * invLog10Base2;	//By log equivalency: log2(x) = log10(x)/log10(2)
		mantissaVal += kInv;
		
	}

	return;
}

/*!
 *  \brief Description
 *
 *		Description
 *  
 *  \param[in] number
 *		\todo Describe
 *  \param[in] outExp
 *		\todo Describe
 *  \param[in] outMant
 *		\todo Describe
 *  \param[in] fract
 *		\todo Describe
 *
 *	\note Log processing code like LogFloatToExpMantissa() can 
 *	be copied into a plug-in's algorithm directly (mystery optimization)
 *	\note Not called directly (see LogFastLookup)
 */	
template <int kMantMSBs>
static inline void LogFloatToExpMantissa(
	float number, 
	int * const AAX_RESTRICT outExp, 
	int * const AAX_RESTRICT outMant, 
	float* const AAX_RESTRICT fract)
{	
	const int mantissaSize = 1 << (kMantissaBitSize - kMantMSBs);
	const float invMantissaSize = 1.0f/(float)mantissaSize;
	const int intEquiv = *reinterpret_cast<const int*>(&number);

	//bool isNegative = (result & 0x80000000) != 0; //Sign bit
	const int exponent      = (intEquiv & 0x7f800000) >> 23; //Exponent bits
	const int mantissa      =  intEquiv & 0x007fffff; //Mantissa bits.

	// 0 000|0 000|0 1 010| 0000 0000 0000 0000 0000
	// 0111 1111 1000 0000 0000 0000 0000 0000 = 7f800000 (Exp)											   
	// 0000 0000 0111 1111 1111 1111 1111 1111 = 007fffff (Mant)

	*outExp = exponent - kExpBias;			//Compensate for exponent bias for caller.
	//To do: add halfLSBShifted to mantissa before shifting; *outMant = (mantissa + halfLSBShifted) >> (kMantissaBitSize - mantMSBs)
	*outMant = mantissa >> (kMantissaBitSize - kMantMSBs);		//Shift to provide only mantMSBs for log LUT.
	// Provide a linear interpolation fraction
	*fract = (float)(mantissa & (mantissaSize - 1)) * invMantissaSize;

	return;
}	  

/*!
 *  \brief Fast lookup for log(x)
 *
 *		Description
 *  
 *  \param[in] num
 *		
 *  \param[in] logTable
 *		Log lookup table
 *
 *	\note Log processing code like LogFloatToExpMantissa() can 
 *	be copied into a plug-in's algorithm directly (mystery optimization)
 */	
template <int kPrecision>
static inline float LogFastLookup(
	float num, 
	const float* const AAX_RESTRICT logTable)
{

	int exponent, mantissa;
	float fract;

	AAX::LogFloatToExpMantissa<kPrecision>(num, &exponent, &mantissa, &fract);

	const float mantLog1 = logTable[mantissa];
	const float mantLog2 = logTable[mantissa+1];

	const float logOfNum = (mantLog1 * (1.0f - fract) + fract * mantLog2) + exponent;
	return logOfNum;
}  



} // namespace AAX

#endif // #ifndef _AAX_FASTPOW_H_
