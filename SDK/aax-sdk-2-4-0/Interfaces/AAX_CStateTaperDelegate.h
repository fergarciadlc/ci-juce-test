/*================================================================================================*/
/*
 *
 *	Copyright 2014-2017, 2019 by Avid Technology, Inc.
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
 *	\file  AAX_CStateTaperDelegate.h
 *
 *	\brief A state taper delegate (similar to a linear taper delegate.)
 *
 */ 
/*================================================================================================*/


#ifndef	AAX_CSTATETAPERDELEGATE_H
#define AAX_CSTATETAPERDELEGATE_H

#include "AAX_ITaperDelegate.h"
#include "AAX.h"	//for types

#include <cmath>	//for floor()


/** \brief A linear taper conforming to AAX_ITaperDelegate
	
	\details
	This taper spaces a parameter's real values evenly between its minimum and maximum, with a
	linear mapping between the parameter's real and normalized values.  It is essentially a
	version of AAX_CLinearTaperDelegate without that class' additional RealPrecision
	templatization.

	\ingroup TaperDelegates

 */
template <typename T>
class AAX_CStateTaperDelegate : public AAX_ITaperDelegate<T>
{
public: 
	/** \brief Constructs a State Taper with specified minimum and maximum values.  
	 *
	 *	\note The parameter's default value should lie within the min to max range.
	 *
	 *	\param[in] minValue
	 *	\param[in] maxValue
	 */
	AAX_CStateTaperDelegate(T minValue=0, T maxValue=1);
	
	//Virtual Overrides
	AAX_CStateTaperDelegate<T>*	Clone() const AAX_OVERRIDE;
	T		GetMinimumValue()	const AAX_OVERRIDE			{ return mMinValue; }
	T		GetMaximumValue()	const AAX_OVERRIDE			{ return mMaxValue; }
	T		ConstrainRealValue(T value)	const AAX_OVERRIDE;
	T		NormalizedToReal(double normalizedValue) const AAX_OVERRIDE;
	double	RealToNormalized(T realValue) const AAX_OVERRIDE;
	
private:
	T	mMinValue;
	T	mMaxValue;
};

template <typename T>
AAX_CStateTaperDelegate<T>::AAX_CStateTaperDelegate(T minValue, T maxValue)  :  AAX_ITaperDelegate<T>(),
	mMinValue(minValue),
	mMaxValue(maxValue)
{

}

template <typename T>
AAX_CStateTaperDelegate<T>*		AAX_CStateTaperDelegate<T>::Clone() const
{
	return new AAX_CStateTaperDelegate(*this);
}

template <typename T>
T		AAX_CStateTaperDelegate<T>::ConstrainRealValue(T value)	const
{
	if (mMinValue == mMaxValue)
		return mMinValue;
		
	const T& highValue = mMaxValue > mMinValue ? mMaxValue : mMinValue;
	const T& lowValue = mMaxValue > mMinValue ? mMinValue : mMaxValue;
	
	if (value > highValue)
		return highValue;
	if (value < lowValue)
		return lowValue;
	
	return value;
}

template <typename T>
T		AAX_CStateTaperDelegate<T>::NormalizedToReal(double normalizedValue) const
{
	double doubleRealValue = normalizedValue * (double(mMaxValue) - double(mMinValue)) + double(mMinValue);
	if ( doubleRealValue >= 0 )
		doubleRealValue += 0.5;
	else doubleRealValue -= 0.5;
	return ConstrainRealValue(static_cast<T>(doubleRealValue));
}

template <typename T>
double	AAX_CStateTaperDelegate<T>::RealToNormalized(T realValue) const
{
	realValue = ConstrainRealValue(realValue);
	double normalizedValue = (mMaxValue == mMinValue) ? 0.5 : (double(realValue) - double(mMinValue)) / (double(mMaxValue) - double(mMinValue));
	return normalizedValue;
}
	



#endif //AAX_CSTATETAPERDELEGATE_H
