/*================================================================================================*/
/*
 *
 *	Copyright 2013-2017, 2019 by Avid Technology, Inc.
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
 *	\file AAX_CLogTaperDelegate.h
 *
 *	\brief A log taper delegate.
 */ 
/*================================================================================================*/


#ifndef	AAX_CLOGTAPERDELEGATE_H
#define AAX_CLOGTAPERDELEGATE_H

#include "AAX_ITaperDelegate.h"
#include "AAX_UtilsNative.h"
#include "AAX.h"	//for types

#include <cmath>	//for floor(), log()



/** \brief A logarithmic taper conforming to AAX_ITaperDelegate
	
	\details
	This taper spaces a parameter's real values between its minimum and maximum bounds, with a
	natural logarithmic mapping between the parameter's real and normalized values.

	\par RealPrecision
	In addition to its type templatization, this taper includes a precision template parameter.
	RealPrecision is a multiplier that works in conjunction with the round()
	function to limit the precision of the real values provided by this taper.  For example, if
	RealPrecision is 1000, it will round to the closest 0.001 when doing any
	sort of value conversion.  If RealPrecision is 1, it will round to the nearest integer.
	If RealPrecision is 1000000, it will round to the nearest 0.000001.  This
	is particularly useful for preventing things like 1.9999999 truncating down to 1 instead of
	rounding up to 2.
	
	To accomplish this behavior, the taper multiplies its unrounded parameter values by
	RealPrecision, rounds the result to the nearest valid value, then divides RealPrecision
	back out.
	
	Rounding will be disabled if RealPrecision is set to a value less than 1

	\ingroup TaperDelegates

 */
template <typename T, int32_t RealPrecision=1000>
class AAX_CLogTaperDelegate : public AAX_ITaperDelegate<T>
{
public: 
	/** \brief Constructs a Log Taper with specified minimum and maximum values.  
	 *
	 *	\note The parameter's default value should lie within the min to max range.
	 *
	 *	\param[in] minValue
	 *	\param[in] maxValue
	 */
	AAX_CLogTaperDelegate(T minValue=0, T maxValue=1);
	
	//Virtual Overrides
	AAX_CLogTaperDelegate<T, RealPrecision>*	Clone() const AAX_OVERRIDE;
	T		GetMinimumValue()	const AAX_OVERRIDE				{ return mMinValue; }
	T		GetMaximumValue()	const AAX_OVERRIDE				{ return mMaxValue; }
	T		ConstrainRealValue(T value)	const AAX_OVERRIDE;
	T		NormalizedToReal(double normalizedValue) const AAX_OVERRIDE;
	double	RealToNormalized(T realValue) const AAX_OVERRIDE;
	
protected:
	T	Round(double iValue) const;

private:
	T	mMinValue;
	T	mMaxValue;
};

template <typename T, int32_t RealPrecision>
T	AAX_CLogTaperDelegate<T, RealPrecision>::Round(double iValue) const
{
	double precision = RealPrecision;
	if (precision > 0)
		return static_cast<T>(floor(iValue * precision + 0.5) / precision);
    return static_cast<T>(iValue);
}

template <typename T, int32_t RealPrecision>
AAX_CLogTaperDelegate<T, RealPrecision>::AAX_CLogTaperDelegate(T minValue, T maxValue)  :  AAX_ITaperDelegate<T>(),
	mMinValue(minValue),
	mMaxValue(maxValue)
{

}

template <typename T, int32_t RealPrecision>
AAX_CLogTaperDelegate<T, RealPrecision>*		AAX_CLogTaperDelegate<T, RealPrecision>::Clone() const
{
	return new AAX_CLogTaperDelegate(*this);
}

template <typename T, int32_t RealPrecision>
T		AAX_CLogTaperDelegate<T, RealPrecision>::ConstrainRealValue(T value)	const
{
	if (mMinValue == mMaxValue)
		return mMinValue;
	
	if (RealPrecision)
		value = Round(value);		//reduce the precision to get proper rounding behavior with integers.
	
	const T& highValue = mMaxValue > mMinValue ? mMaxValue : mMinValue;
	const T& lowValue = mMaxValue > mMinValue ? mMinValue : mMaxValue;
	
	if (value > highValue)
		return highValue;
	if (value < lowValue)
		return lowValue;
	
	return value;
}

template <typename T, int32_t RealPrecision>
T		AAX_CLogTaperDelegate<T, RealPrecision>::NormalizedToReal(double normalizedValue) const
{
	double minLog = AAX::SafeLog(double(mMinValue));
	double maxLog = AAX::SafeLog(double(mMaxValue));

	double doubleRealValue = exp(normalizedValue * (maxLog - minLog) + minLog);
	T realValue = (T) doubleRealValue;
	
	return ConstrainRealValue(realValue);
}

template <typename T, int32_t RealPrecision>
double	AAX_CLogTaperDelegate<T, RealPrecision>::RealToNormalized(T realValue) const
{
	double minLog = AAX::SafeLog(double(mMinValue));
	double maxLog = AAX::SafeLog(double(mMaxValue));

	realValue = ConstrainRealValue(realValue);
	double normalizedValue = (maxLog == minLog) ? 0.5 : (AAX::SafeLog(double(realValue)) - minLog) / (maxLog - minLog);
	return normalizedValue;
}
	
#endif // AAX_CLOGTAPERDELEGATE_H
