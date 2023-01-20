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
 *	\file AAX_CStringDisplayDelegate.h
 *
 *	\brief A string display delegate.
 *
 */ 
/*================================================================================================*/


#ifndef AAX_CSTRINGDISPLAYDELEGATE_H
#define AAX_CSTRINGDISPLAYDELEGATE_H

#include "AAX_IDisplayDelegate.h"
#include <sstream>
#include <map>


/**	\brief A string, or list, display format conforming to AAX_IDisplayDelegate
	
	\details
	This display delegate uses a string map to associate parameter values with specific
	strings.  This kind of display delegate is most often used for control string or
	list parameters, which would internally use an integer parameter type.  The int value
	would then be used as a lookup into this delegate, which would return a string for each
	valid int value.
	
	\ingroup DisplayDelegates

 */
template <typename T>
class AAX_CStringDisplayDelegate : public AAX_IDisplayDelegate<T>
{
public:
	/** \brief Constructor
	 *
	 *	Constructs a String Display Delegate with a provided string map.
	 *
	 *	\note The string map should
	 *	already be populated with value-string pairs, as this constructor will copy the provided
	 *	map into the delegate object's own memory.
	 *
	 *	\param[in] stringMap
	 *		A populated map of value-string pairs
	 */
	AAX_CStringDisplayDelegate(const std::map<T,AAX_CString>& stringMap);
	
	//Virtual Overrides
	AAX_CStringDisplayDelegate<T>*	Clone() const AAX_OVERRIDE;
	bool		ValueToString(T value, AAX_CString* valueString) const AAX_OVERRIDE;
	bool		ValueToString(T value, int32_t maxNumChars, AAX_CString* valueString) const AAX_OVERRIDE;
	bool		StringToValue(const AAX_CString& valueString, T* value) const AAX_OVERRIDE;
	
protected:
	std::map<T, AAX_CString>			mStringMap;
	std::map<AAX_CString, T>			mInverseStringMap;
};



template <typename T>
AAX_CStringDisplayDelegate<T>::AAX_CStringDisplayDelegate(const std::map<T,AAX_CString>& stringMap)  :
	AAX_IDisplayDelegate<T>(),
	mStringMap(stringMap),
	mInverseStringMap()
{
	//Construct an inverse string map from our already copied internal copy of the string map.
	//This inverse map is used for stringToValue conversion.
	typename std::map<T,AAX_CString>::iterator valueStringIterator = mStringMap.begin();
	while ( valueStringIterator != mStringMap.end() )
	{
		mInverseStringMap.insert(std::pair<AAX_CString, T>(valueStringIterator->second, valueStringIterator->first));
		valueStringIterator++;
	}
}

template <typename T>
AAX_CStringDisplayDelegate<T>*		AAX_CStringDisplayDelegate<T>::Clone() const
{
	return new AAX_CStringDisplayDelegate(*this);
}

template <typename T>
bool		AAX_CStringDisplayDelegate<T>::ValueToString(T value, AAX_CString* valueString) const
{
	typename std::map<T,AAX_CString>::const_iterator mapPairIterator = mStringMap.find(value);
	if( mapPairIterator != mStringMap.end() ) 
	{
		*valueString = mapPairIterator->second;
		return true;
	}
	*valueString = AAX_CString("String Not Found");
	return false;
}

template <typename T>
bool		AAX_CStringDisplayDelegate<T>::ValueToString(T value, int32_t /*maxNumChars*/, AAX_CString* valueString) const
{
	// First, get the full length string.
	bool result = this->ValueToString(value, valueString);
	
	//<DMT> TODO: Shorten the string based on the number of characters...  
	
	return result;
}

template <typename T>
bool		AAX_CStringDisplayDelegate<T>::StringToValue(const AAX_CString& valueString, T* value) const
{
	typename std::map<AAX_CString, T>::const_iterator mapPairIterator = mInverseStringMap.find(valueString);
	if( mapPairIterator != mInverseStringMap.end() ) 
	{
		*value = mapPairIterator->second;
		return true;
	}
	*value = 0;
	return false;
}




#endif //AAX_CSTRINGDISPLAYDELEGATE_H
