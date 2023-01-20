/*================================================================================================*/
/*
 *
 *	Copyright 2014-2015 by Avid Technology, Inc.
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
 *	\file  AAX_IDisplayDelegate.h
 *
 *	\brief Defines the display behavior for a parameter
 *
 */ 
/*================================================================================================*/


#ifndef AAX_IDISPLAYDELETGATE_H
#define AAX_IDISPLAYDELETGATE_H

#include "AAX.h"


//Forward declarations
class AAX_CString;

/** \brief Defines the display behavior for a parameter.
	
	\details
	This interface represents a delegate class to be used in conjunction with AAX_IParameter.
	AAX_IParameter delegates all conversion operations between strings and real parameter
	values to classes that meet this interface.  You can think of AAX_ITaperDelegate subclasses
	as simple string serialization routines that enable a specific string conversions for an
	arbitrary parameter.

	For more information about how parameter delegates operate, see the AAX_ITaperDelegate and
	\ref AAXLibraryFeatures_ParameterManager documentation.
	
	\note This class is \em not part of the %AAX ABI and must not be passed between the plug-in
	and the host.

	\ingroup AAXLibraryFeatures_ParameterManager_DisplayDelegates

*/
class AAX_IDisplayDelegateBase
{
public:
	/** \brief Virtual destructor
	 *
	 *	\note This destructor MUST be virtual to prevent memory leaks.
	 */
	virtual ~AAX_IDisplayDelegateBase()		{ }
};

/** Display delegate interface template
 
 \copydoc AAXLibraryFeatures_ParameterManager_DisplayDelegates
 \ingroup AAXLibraryFeatures_ParameterManager_DisplayDelegates
 */
template <typename T>
class AAX_IDisplayDelegate : public AAX_IDisplayDelegateBase
{
public:

	/** \brief Constructs and returns a copy of the display delegate
	 *
	 *	In general, this method's implementation can use a simple copy constructor:
	 
		\code
			template <typename T>
			AAX_CSubclassDisplayDelegate<T>*	AAX_CSubclassDisplayDelegate<T>::Clone() const
			{
				return new AAX_CSubclassDisplayDelegate(*this);
			}
		\endcode

	 */
	virtual AAX_IDisplayDelegate*	Clone() const = 0;
	
	/** \brief Converts a real parameter value to a string representation
	 *	
	 *	\param[in] value
	 *		The real parameter value that will be converted
	 *	\param[out] valueString
	 *		A string corresponding to value
	 *
	 *	\retval true	The string conversion was successful
	 *	\retval false	The string conversion was unsuccessful
	 */
	virtual bool		ValueToString(T value, AAX_CString* valueString) const = 0;

	/** \brief Converts a real parameter value to a string representation using a size hint, useful for control surfaces and other character limited displays.
	 *	
	 *	\param[in] value
	 *		The real parameter value that will be converted
	 *	\param[in] maxNumChars
	 *		Size hint for the desired maximum number of characters in the string (not including null termination)
	 *	\param[out] valueString
	 *		A string corresponding to value
	 *
	 *	\retval true	The string conversion was successful
	 *	\retval false	The string conversion was unsuccessful
	 */
	virtual bool		ValueToString(T value, int32_t maxNumChars, AAX_CString* valueString) const = 0;
	
	/** \brief Converts a string to a real parameter value
	 *	
	 *	\param[in] valueString
	 *		The string that will be converted
	 *	\param[out] value
	 *		The real parameter value corresponding to valueString
	 *
	 *	\retval true	The string conversion was successful
	 *	\retval false	The string conversion was unsuccessful
	 */
	virtual bool		StringToValue(const AAX_CString& valueString, T* value) const = 0;
};



#endif //AAX_IDISPLAYDELETGATE_H
