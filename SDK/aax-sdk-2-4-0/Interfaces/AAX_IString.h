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
 *	\file   AAX_IString.h
 *	
 *	\brief	An %AAX string interface
 *
 */ 
/*================================================================================================*/


#ifndef AAX_ISTRING_H
#define AAX_ISTRING_H

#include "AAX.h"	//for types


/** 
*	\brief	A simple string container that can be passed across a binary boundary.  This class, for simplicity, is not versioned and thus can never change.
*
*	\details
*	For a real string implementation, see AAX_CString, which inherits from this interface, but provides a much richer string interface.
*
*   This object is not versioned with ACF for a variety of reasons, but the biggest implication of that is that THIS INTERFACE CAN NEVER CHANGE!
*
*/
class AAX_IString
{
public:
	/** Virtual Destructor */
	virtual ~AAX_IString ()	{}
	
	/** Length methods */
	virtual uint32_t		Length () const = 0;
	virtual uint32_t		MaxLength () const = 0;
	
	/** C string methods */
	virtual const char *	Get ()	const = 0;
	virtual void			Set ( const char * iString ) = 0;		
	
	/** Assignment operators */
	virtual AAX_IString &	operator=(const AAX_IString & iOther) = 0;
	virtual AAX_IString &	operator=(const char * iString) = 0;			
};




#endif //AAX_ISTRING_H
