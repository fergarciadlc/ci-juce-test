/*================================================================================================*/
/*
 *
 *	Copyright 2013-2015 by Avid Technology, Inc.
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
 *	\file   AAX_SliderConversions.h
 *	
 *	\brief Legacy utilities for converting parameter values to and from the normalized full-scale
 *	32-bit fixed domain that was used for RTAS/TDM plug-ins.
 *
 *	\details
 *	\legacy These utilities may be required in order to maintain settings chunk compatibility with
 *	plug-ins that were ported from the legacy RTAS/TDM format.
 *									
 *	\note %AAX does not provide facilities for converting to and from extended80 data types.  If you
 *	use these types in your plug-in settings then you must provide your own chunk data parsing 
 *	routines.
 *
 */ 
/*================================================================================================*/


#pragma once

#ifndef AAX_SLIDERCONVERSIONS_H
#define AAX_SLIDERCONVERSIONS_H

#include "AAX.h"
#include <algorithm>
#include <stdint.h>


#define AAX_LIMIT(v1,firstVal,secondVal) ( (secondVal > firstVal) ? (std::max)((std::min)(v1,secondVal),firstVal) :  (std::min)((std::max)(v1,secondVal),firstVal) )

int32_t LongControlToNewRange (int32_t aValue, int32_t rangeMin, int32_t rangeMax);

/**	\brief Convert from int32_t control value 0x80000000...0x7FFFFFFF
 *	to a int32_t ranging from rangeMin to rangeMax (linear)
 */
int32_t LongToLongControl (int32_t aValue, int32_t rangeMin, int32_t rangeMax);

/**	\brief Convert from int32_t control value 0x80000000...0x7FFFFFFF
 *	to an double ranging from firstVal to secondVal (linear)
 */
double LongControlToDouble(int32_t aValue, double firstVal, double secondVal);

/**	\brief Convert from an double ranging from firstVal to secondVal (linear)
 *	to int32_t control value 0x80000000...0x7FFFFFFF 
 */
int32_t DoubleToLongControl (double aValue, double firstVal, double secondVal);

int32_t DoubleToLongControlNonlinear(double aValue, double* minVal, double* rangePercent, int32_t numRanges);
double LongControlToDoubleNonlinear(int32_t aValue, double* minVal, double* rangePercent, int32_t numRanges);

/**	\brief Convert from int32_t control value 0x80000000...0x7FFFFFFF
 *	to an double ranging from minVal to maxVal (logarithmic)
 *	
 *	\details
 *	\note This is LOGARITHMIC, so minVal & maxVal have to be > zero!
 */
double LongControlToLogDouble(int32_t aValue, double minVal, double maxVal);

/**	\brief Convert from an double ranging from minVal to maxVal (logarithmic)
 *	to int32_t control value 0x80000000...0x7FFFFFFF 
 *	
 *	\details
 *	\note This is LOGARITHMIC, so minVal & maxVal have to be > zero!
 */
int32_t LogDoubleToLongControl(double aValue, double minVal, double maxVal);

#endif // AAX_SLIDERCONVERSIONS_H

