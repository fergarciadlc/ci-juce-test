/*================================================================================================*/
/*
 *	Copyright 2014-2016, 2019, 2021 by Avid Technology, Inc.
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
 *	\file  AAX_Denormal.h
 *
 *	\brief Signal processing utilities for denormal/subnormal floating point numbers
 *
 */ 
/*================================================================================================*/
#pragma once

#ifndef AAX_DENORMAL_H
#define AAX_DENORMAL_H


// AAX Includes
#include "AAX.h"
#include "AAX_PlatformOptimizationConstants.h"


#if ! defined( _TMS320C6X )
#include <limits>
#include <math.h>
#endif // ! defined( _TMS320C6X )



/** \def AAX_SCOPE_DENORMALS_AS_ZERO()
 \brief Sets the run-time environment to treat denormal floats as zero within the scope
 of this macro
 
 \details The host sets the denormal policy for all AAX threads and may already use
 settings which treat denormal float values as zero (DAZ+FZ). This macro forces DAZ
 behavior.
 */

/** \def AAX_SCOPE_COMPUTE_DENORMALS()
 \brief Sets the run-time environment to handle denormal floats within the scope of this
 macro
 
 \details The host sets the denormal policy for all AAX threads and may use settings which
 treat denormal float values as zero (DAZ+FZ). This macro forces non-DAZ behavior.
 */

#if defined( _TMS320C6X )

// TODO: Currently noop on TI
#define AAX_SCOPE_COMPUTE_DENORMALS() do {} while(0)
#define AAX_SCOPE_DENORMALS_AS_ZERO() do {} while(0)

#elif ((defined _MSC_VER) || defined(WINDOWS_VERSION))
// Visual Studio

// Note: In Pro Tools 11 and higher for Windows, DAZ is turned on for all plug-in processing threads

// FIXME: These macros are currently noop on Windows because of compiler problems when intrin.h is included. Investigate using _controlfp(_DN_FLUSH, _MCW_DN) instead
#define AAX_SCOPE_COMPUTE_DENORMALS() do {} while(0)
#define AAX_SCOPE_DENORMALS_AS_ZERO() do {} while(0)
//#define AAX_SCOPE_COMPUTE_DENORMALS() AAX_StDenormalAsZeroFlagsOff computeDenormalFlagsScope ## __LINE__ ; do {} while (0)
//#define AAX_SCOPE_DENORMALS_AS_ZERO() AAX_StDenormalAsZeroFlagsOn denormalAsZeroFlagsScope ## __LINE__ ; do {} while (0)
//
//#include <Windows.h>
//#include <mmintrin.h>
//
//// Flags for the following bits of the MXCSR (Intel SSE status and control) register:
//// 06: Denormals Are Zeros: all denormal source operands are
////     converted to zero before any computations on them.
//// 15: Flush to Zero:  the processor returns zero when any
////     operation results in an underflow condition.
//const unsigned gMXCSRFlags_DAZFZ = 0x00008040;
//
//// set FPU/SSE denormal state
//struct AAX_StDenormalAsZeroFlagsOn
//{
//public:
//	AAX_StDenormalAsZeroFlagsOn()
//	: mSSESupported(IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
//	{
//		if (mSSESupported) { set_flags(); }
//	}
//	
//	~AAX_StDenormalAsZeroFlagsOn()
//	{
//		if (mSSESupported) { reset_flags(); }
//	}
//	
//private:
//	void set_flags()
//	{
//		mPrevFlags = _mm_getcsr();
//		unsigned newFlags = mPrevFlags | gMXCSRFlags_DAZFZ;
//		_mm_setcsr(newFlags);
//	}
//	
//	void reset_flags()
//	{
//		_mm_setcsr(mPrevFlags);
//	}
//	
//	unsigned mPrevFlags;
//	const bool mSSESupported;
//};
//
//// unset FPU/SSE denormal state
//struct AAX_StDenormalAsZeroFlagsOff
//{
//public:
//	AAX_StDenormalAsZeroFlagsOff()
//	: mSSESupported(IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
//	{
//		if (mSSESupported) { set_flags(); }
//	}
//	
//	~AAX_StDenormalAsZeroFlagsOff()
//	{
//		if (mSSESupported) { reset_flags(); }
//	}
//	
//private:
//	void set_flags()
//	{
//		mPrevFlags = _mm_getcsr();
//		unsigned newFlags = mPrevFlags & (~gMXCSRFlags_DAZFZ);
//		_mm_setcsr(newFlags);
//	}
//	
//	void reset_flags()
//	{
//		_mm_setcsr(mPrevFlags);
//	}
//	
//	unsigned mPrevFlags;
//	const bool mSSESupported;
//};


#elif defined(LINUX_VERSION)
// Linux

// TODO: Currently noop on Linux
#define AAX_SCOPE_COMPUTE_DENORMALS() do {} while(0)
#define AAX_SCOPE_DENORMALS_AS_ZERO() do {} while(0)

#elif (defined (__GNUC__) || defined(MAC_VERSION))
// GCC / OS X

// Note: In Pro Tools 11 through Pro Tools 12.6 on OS X, DAZ is turned off for all plug-in processing threads

#define AAX_SCOPE_COMPUTE_DENORMALS() AAX_StDenormalAsZeroFlagsOff computeDenormalFlagsScope ## __LINE__ ; do {} while (0)
#define AAX_SCOPE_DENORMALS_AS_ZERO() AAX_StDenormalAsZeroFlagsOn denormalAsZeroFlagsScope ## __LINE__ ; do {} while (0)

#include <fenv.h>
struct AAX_StDenormalAsZeroFlagsOn
{
public:
	AAX_StDenormalAsZeroFlagsOn() { set_flags(); }
	~AAX_StDenormalAsZeroFlagsOn() { reset_flags(); }
	
private:
#if !defined(__arm64__)
	void set_flags() { fegetenv(&mPrevFlags); fesetenv(FE_DFL_DISABLE_SSE_DENORMS_ENV); }
#else
    void set_flags() { fegetenv(&mPrevFlags); fesetenv(FE_DFL_DISABLE_DENORMS_ENV); }
#endif
	void reset_flags() { fesetenv(&mPrevFlags); }
	fenv_t mPrevFlags;
};
struct AAX_StDenormalAsZeroFlagsOff
{
public:
	AAX_StDenormalAsZeroFlagsOff() { set_flags(); }
	~AAX_StDenormalAsZeroFlagsOff() { reset_flags(); }
	
private:
	void set_flags() { fegetenv(&mPrevFlags); fesetenv(FE_DFL_ENV); } // assuming that FE_DFL_ENV are the flags that we want here
	void reset_flags() { fesetenv(&mPrevFlags); }
	fenv_t mPrevFlags;
};



#else

#error AAX_SCOPE_COMPUTE_DENORMALS is not defined for this compiler
#error AAX_SCOPE_DENORMALS_AS_ZERO is not defined for this compiler

// Just for Doxygen
#define AAX_SCOPE_COMPUTE_DENORMALS() do {} while(0)
#define AAX_SCOPE_DENORMALS_AS_ZERO() do {} while(0)

#endif // End compiler selection for AAX_SCOPE_COMPUTE_DENORMALS / AAX_SCOPE_DENORMALS_AS_ZERO definition




namespace AAX
{
	const double cDenormalAvoidanceOffset=3.0e-34;
	const float cFloatDenormalAvoidanceOffset=3.0e-20f;	//This number is emprically derived with minimal trial and error.

#if ! defined( _TMS320C6X )
	static const float cMinimumNormalFloat = std::numeric_limits<float>::min();
#endif
	
	/*!
	 *  \brief Clamps very small floating point values to zero
	 *
	 *  \details On Pentiums and Pentium IIs the generation of denormal floats causes enormous
	 *	performance losses. This routine removes denormals by clamping very small values to
	 *	zero. The clamping threshold is very small, but is not the absolute minimum. If
	 *  absolute minimum clamping is desired, use \ref AAX::DeDenormalFine()
	 *
	 */
	inline void DeDenormal(double &iValue)
	{
#if defined(WINDOWS_VERSION) || defined(MAC_VERSION) || defined(LINUX_VERSION)
		if(iValue < cDenormalAvoidanceOffset && iValue > -cDenormalAvoidanceOffset) iValue=0.0;
#endif
	}
	
	/*! \copydoc AAX::DeDenormal(double&)
	 */
	inline void DeDenormal(float &iValue)
	{
#if defined(WINDOWS_VERSION) || defined(MAC_VERSION) || defined(LINUX_VERSION)
		if(iValue < cFloatDenormalAvoidanceOffset && iValue > -cFloatDenormalAvoidanceOffset) iValue=0.0f;
#endif 
	}
	
#if AAX_CPP11_SUPPORT
	/*! \copydoc AAX::DeDenormal(double&)
	 */
	inline float DeDenormal(float &&iValue)
	{
#if defined(WINDOWS_VERSION) || defined(MAC_VERSION) || defined(LINUX_VERSION)
		return (iValue < cFloatDenormalAvoidanceOffset && iValue > -cFloatDenormalAvoidanceOffset) ? 0.f : iValue;
#endif
	}
#endif // AAX_CPP11_SUPPORT
	
	/*!
	 * Similar to \ref AAX::DeDenormal(), but uses the minimum possible normal float value as
	 * the clamping threshold
	 */
	inline void DeDenormalFine(float &iValue)
	{
#if ! defined( _TMS320C6X )
		if(iValue < cMinimumNormalFloat && iValue > -cMinimumNormalFloat) iValue=0.0f;
#endif
	}
	
	/*!
	 *  \brief Round all denormal/subnormal samples in a buffer to zero
	 *
	 *
	 *  \param[in] inSamples
	 *		Samples to convert
	 *  \param[in] inLength
	 *		Number of samples in inSamples
	 *
	 */
	inline void FilterDenormals(float* inSamples, int32_t inLength)
	{
		//TODO: Implement optimized versions for TI and SSE
#if ! defined( _TMS320C6X ) // Not yet implemented for TI
		for( int32_t i = 0; i < inLength; ++i )
		{
			float curFloat = *inSamples;
			if( fabsf(curFloat) < cMinimumNormalFloat )
				curFloat = 0.0f;
			*(inSamples++) = curFloat;
		}
#endif
	}

} // namespace AAX

#endif // AAX_QUANTIZE_H
