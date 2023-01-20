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
 *	\file  AAX_PlatformOptimizationConstants.h
 *
 *	\brief Constants descriptor...
 *
 */ 
/*================================================================================================*/
#pragma once

#ifndef AAX_PLATFORMOPTIMIZATIONCONSTANTS_H
#define AAX_PLATFORMOPTIMIZATIONCONSTANTS_H

// Set up our platform-specific optimization defines
#if USE_PLATFORM_OPTIMIZATION
	#if defined( WINDOWS_VERSION )
			#define USE_INTEL_IPP (1)		// Windows
			#define __SSE__ (1)				// Manually define the __SSE__ flag
	#elif defined( MAC_VERSION )
		#if defined(__ppc__)
			#define USE_ALTIVEC_VDSP (1)	// PPC
		#elif defined(__i386__) or defined(__x86_64__)
			#define USE_INTEL_IPP (1)		// MacTel
		#else
			#error "Unsupported platform for optimizations!"
		#endif // __i386__ or __ppc__
	#else
		#error "Unsupported platform for optimizations!"
	#endif // WINDOWS_VERSION
#endif // USE_PLATFORM_OPTIMIZATION

#endif // AAX_PLATFORMOPTIMIZATIONCONSTANTS_H

