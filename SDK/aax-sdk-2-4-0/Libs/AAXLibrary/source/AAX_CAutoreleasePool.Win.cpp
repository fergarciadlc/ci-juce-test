/*================================================================================================*/
/*
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
 *	\file  AAX_CAutoreleasePool.Win.cpp
 *	
 *	\brief Windows implementation of a basic Autorelease pool helper utility
 *
 */ 
/*================================================================================================*/

#include "AAX_CAutoreleasePool.h"
#include "stddef.h"

AAX_CAutoreleasePool::AAX_CAutoreleasePool() : mAutoreleasePool(NULL)
{
	// Nothing to do on Windows yet...maybe someday if Apple ever opens objective-C libs on the Windows platform.
}

AAX_CAutoreleasePool::~AAX_CAutoreleasePool()
{
	// Nothing to do on Windows yet...maybe someday if Apple ever opens objective-C libs on the Windows platform.
}
