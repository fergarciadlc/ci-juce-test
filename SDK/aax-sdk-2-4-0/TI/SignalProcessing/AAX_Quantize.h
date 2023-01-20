/*================================================================================================*/
/*
 *	Copyright 2013-2015 by Avid Technology, Inc.
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
 *	\file  AAX_Quantize.h
 *
 *	\brief Quantization utilities
 *
 */ 
/*================================================================================================*/
#pragma once

#ifndef AAX_QUANTIZE_H
#define AAX_QUANTIZE_H

#include "AAX.h"
#include "AAX_PlatformOptimizationConstants.h"
#include "AAX_Constants.h"

#if ! defined( _TMS320C6X )

#if _MSC_VER && !defined(__INTEL_COMPILER)
#define _MM_FUNCTIONALITY
#include <intrin.h>
#elif LINUX_VERSION
#elif TARGET_OS_IPHONE
#elif defined(__arm__)
#else
// GNU or INTEL:
#include <xmmintrin.h>
#include <pmmintrin.h>
#include <tmmintrin.h>
#endif

#endif

namespace AAX
{

//This assumes the floating point processor is in 64 bit IEEE mode.
#if ! defined( _TMS320C6X )
    
// For double->Int32 conversion
static const double cDouble2IntBias = ldexpf(1,52)*1.5;
static const double cOneHalfOffset = 0.5;
static const int32_t cMantisaWord = cBigEndian;

// For double->Int64 conversion
static const double	kExponentMagicDelta			= 1.5e-8;							//1e^(number of exp bit)
static const double	kBigMantissaMagicFloat		= 6755399441055744.0;			    //2^52 * 1.5,  uses limited precisicion to floor
static const int64_t		kBigMantissaMagicMask		= 0x1fffffffffffffLL;			    //2^52 * 1.5 mask, 
static const int64_t		kBigMantissaMagicInt		= 0x18000000000000LL;			    //2^52 * 1.5,  uses limited precisicion to floor
#endif

/*===================================================================================================*/
	
/*!
 *  \brief Round to Int32
 *  
 *  
 *  \param[in] iVal
 *		Value to convert
 *
 */	
inline int32_t FastRound2Int32(double iVal)
{
#if defined( _TMS320C6X )
	// rounding mode set by the CSR register
	const int32_t r = _dpint(iVal); 
	return r;
#else
	iVal += cDouble2IntBias;
	return (reinterpret_cast<int32_t*>(&iVal))[cMantisaWord];
#endif
}

/*!
 *  \brief Round to Int32
 *  
 *  
 *  \param[in] iVal
 *		Value to convert
 *
 */	
inline int32_t FastRound2Int32(float iVal)
{
#if defined( _TMS320C6X )
	// rounding mode set by the CSR register
	const int32_t r = _spint(iVal); 
	return r;
#else
	return FastRound2Int32(double(iVal));
#endif
}


/** \deprecated
 */
inline int32_t FastRndDbl2Int32(double iVal)
{
	return FastRound2Int32(iVal);
}

/*!
 *  \brief Float to Int conversion with truncation
 *  
 *  \param[in] iVal
 *		Value to convert
 *
 *	\note This truncation is NOT identical to C style casting.  Because the Intel (and I would 
 *	assume PowerPC) processors use convergent rounding by default, exactly whole odd numbers 
 *	will truncate down by 1.0 (e.g. 0.0->0, 1.0->0, 2.0->2, 3.0->2).  Surprisingly, even with 
 *	these limitations this fast float to int conversion is often very useful in practice, as 
 *	long as one is aware of these issues.
	 */	
inline int32_t FastTrunc2Int32(double iVal)
{
#if defined( _TMS320C6X )
	// rounding mode set by the CSR register
	const int32_t r = _dpint(iVal - 0.5); 
	return r;
#else
	return FastRound2Int32(iVal - 0.5);
#endif
}

/*!
 *  \brief Float to Int conversion with truncation
 *  
 *  
 *  \param[in] iVal
 *		Value to convert
 *
 */	
inline int32_t FastTrunc2Int32(float iVal)
{
#if defined( _TMS320C6X )
	// rounding mode set by the CSR register
	const int32_t r = _spint(iVal - 0.5f); 
	return r;
#elif _MSC_VER
	int32_t r;
	r = _mm_cvtt_ss2si( _mm_load_ss(&iVal) );
	return r;
#else
	return static_cast<int32_t>(iVal);
#endif
}
    
/*!
 *  \brief Round to Int64
 *  
 *  Taken from Paul V's implementation in Sys_VecUtils.  This only works on values smaller than 2^52.
 *  
 *  \param[in] iVal
 *		Value to convert
 *
 */	
inline int64_t FastRound2Int64(double iVal)
{
#if defined( _TMS320C6X )
    return (int64_t)(iVal > 0.0 ? iVal + 0.5 : iVal - 0.5);
#else
    iVal += kExponentMagicDelta; // Round to nearest
    iVal += kBigMantissaMagicFloat; // Normalize to integer
    int64_t result =  *reinterpret_cast<int64_t*>(&iVal);
    result &= kBigMantissaMagicMask; // Mask out upper bits of float (exponent, sign)
    result -= kBigMantissaMagicInt; // Normalize to integer
    return result;
#endif
}

} // namespace AAX

#endif // AAX_QUANTIZE_H
