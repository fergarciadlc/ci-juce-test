/*================================================================================================*/
/*
 *	Copyright 2011-2015 by Avid Technology, Inc.
 *	All rights reserved.
 *
 *	CONFIDENTIAL: This document contains confidential information.  Do not 
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
 *	OF Avid Technology, INC.
 */
/*================================================================================================*/

#include "AAX_CAutoreleasePool.h"

#import "Cocoa/Cocoa.h"

AAX_CAutoreleasePool::AAX_CAutoreleasePool()
{
	mAutoreleasePool = [[NSAutoreleasePool alloc] init];
}

AAX_CAutoreleasePool::~AAX_CAutoreleasePool()
{
	[((NSAutoreleasePool*) mAutoreleasePool) release];
}
