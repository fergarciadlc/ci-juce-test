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
 *	\file AAX_CNumberDisplayDelegate.h
 *
 *	\brief A number display delegate.
 *
 */ 
/*================================================================================================*/


#ifndef AAX_CNUMBERDISPLAYDELEGATE_H
#define AAX_CNUMBERDISPLAYDELEGATE_H

#include "AAX_IDisplayDelegate.h"
#include "AAX_CString.h"


/**	\brief A numeric display format conforming to AAX_IDisplayDelegate
	
	\details
	This display delegate converts a parameter value to a numeric string using a specified
	precision.

	\ingroup DisplayDelegates

 */
template <typename T, uint32_t Precision=2, uint32_t SpaceAfter=0>
class AAX_CNumberDisplayDelegate : public AAX_IDisplayDelegate<T>
{
public:
	//Virtual Overrides
	AAX_CNumberDisplayDelegate*	Clone() const AAX_OVERRIDE;
	bool		ValueToString(T value, AAX_CString* valueString) const AAX_OVERRIDE;
	bool		ValueToString(T value, int32_t maxNumChars, AAX_CString* valueString) const AAX_OVERRIDE;
	bool		StringToValue(const AAX_CString& valueString, T* value) const AAX_OVERRIDE;
};




template <typename T, uint32_t Precision, uint32_t SpaceAfter>
AAX_CNumberDisplayDelegate<T,Precision,SpaceAfter>*		AAX_CNumberDisplayDelegate<T,Precision,SpaceAfter>::Clone() const
{
	return new AAX_CNumberDisplayDelegate(*this);
}

template <typename T, uint32_t Precision, uint32_t SpaceAfter>
bool	AAX_CNumberDisplayDelegate<T,Precision,SpaceAfter>::ValueToString(T value, AAX_CString* valueString) const
{
	valueString->Clear();
	valueString->AppendNumber(value, Precision);
	if (SpaceAfter != 0)
		valueString->Append(" ");		//Added a space after the number for easier display of units.
	return true;
}

template <typename T, uint32_t Precision, uint32_t SpaceAfter>
bool	AAX_CNumberDisplayDelegate<T,Precision,SpaceAfter>::ValueToString(T value, int32_t maxNumChars, AAX_CString* valueString) const
{
	valueString->Clear();
	valueString->AppendNumber(value, Precision);
	uint32_t strlen = valueString->Length();
	const uint32_t maxNumCharsUnsigned = (0 <= maxNumChars) ? static_cast<uint32_t>(maxNumChars) : 0;
	if (strlen > maxNumCharsUnsigned)
	{
		valueString->Erase(maxNumCharsUnsigned, strlen-maxNumCharsUnsigned);
		strlen = valueString->Length();
	}
	
	if ( 0 < maxNumCharsUnsigned && strlen == maxNumCharsUnsigned && (*valueString)[maxNumCharsUnsigned-1] == '.')	//<DMT> Edge case when the decimal point is the last character, we probably shouldn't show it.
	{
		valueString->Erase(maxNumCharsUnsigned-1, 1);
		strlen = valueString->Length();
	}

	if ((SpaceAfter != 0) && (maxNumCharsUnsigned > strlen) && (maxNumCharsUnsigned-strlen > 2))			//<DMT> Kind of a random threshold for dropping the space after, but seems reasonable for our control surfaces.  (allows dB and Unit prefixes)
		valueString->Append(" ");		//Added a space after the number for easier display of units.
	return true;
}

template <typename T, uint32_t Precision, uint32_t SpaceAfter>
bool	AAX_CNumberDisplayDelegate<T,Precision,SpaceAfter>::StringToValue(const AAX_CString& valueString, T* value) const
{
	double dValue;
	if (valueString.ToDouble(&dValue))
	{
		*value = static_cast<T> (dValue);
		return true;	
	}
	*value = 0;
	return false;
}




#endif //AAX_CNUMBERDISPLAYDELEGATE_H
