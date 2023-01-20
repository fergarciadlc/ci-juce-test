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
 *	\file AAX_CBinaryTaperDelegate.h
 *
 *	\brief A binary taper delegate.
 *
 */ 
/*================================================================================================*/


#ifndef	AAX_CBINARYTAPERDELEGATE_H
#define AAX_CBINARYTAPERDELEGATE_H

#include "AAX_ITaperDelegate.h"


/** \brief A binary taper conforming to \ref AAX_ITaperDelegate
	
	\details
	This taper maps positive real values to 1 and negative or zero real values to 0.  This is
	the standard taper used on all bool parameters.
	
	When this taper is constructed with a bool template type, its normalized values are
	automatically typecast to the proper boolean value.

	\ingroup TaperDelegates
	
*/
template <typename T>
class AAX_CBinaryTaperDelegate : public AAX_ITaperDelegate<T>
{
public:

	/** \brief Constructs a Binary Taper 
	 *
	 */
	AAX_CBinaryTaperDelegate( );
	
	//Virtual Overrides
	AAX_ITaperDelegate<T>*	Clone() const AAX_OVERRIDE;
	T						GetMaximumValue() const AAX_OVERRIDE;
	T						GetMinimumValue() const AAX_OVERRIDE;
	T						ConstrainRealValue(T value)	const AAX_OVERRIDE;
	T						NormalizedToReal(double normalizedValue) const AAX_OVERRIDE;
	double					RealToNormalized(T realValue) const AAX_OVERRIDE;
};






template <typename T>
AAX_CBinaryTaperDelegate<T>::AAX_CBinaryTaperDelegate( )	:   
	AAX_ITaperDelegate<T>()
{
}

template <typename T>
AAX_ITaperDelegate<T>*	AAX_CBinaryTaperDelegate<T>::Clone() const
{
	return new AAX_CBinaryTaperDelegate(*this);
}

template <typename T>
T		AAX_CBinaryTaperDelegate<T>::GetMinimumValue() const
{
	return false;
}

template <typename T>
T		AAX_CBinaryTaperDelegate<T>::GetMaximumValue() const
{
	return true;
}

template <typename T>
T		AAX_CBinaryTaperDelegate<T>::ConstrainRealValue(T value)	const
{
	return value;		
}

template <typename T>
T		AAX_CBinaryTaperDelegate<T>::NormalizedToReal(double normalizedValue) const
{
	if (normalizedValue > 0.0f)
		return (T)(1);		//should construct true for bool
	return (T)(0);			//should construct false for bool
}

template <typename T>
double	AAX_CBinaryTaperDelegate<T>::RealToNormalized(T realValue) const
{
	if (realValue > (T)(0))
		return 1.0f;
	return 0.0f;
}




#endif //AAX_CBINARYTAPERDELEGATE_H


