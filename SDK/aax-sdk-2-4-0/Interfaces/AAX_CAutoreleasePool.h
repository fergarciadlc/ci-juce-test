/*================================================================================================*/
/*

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
 *	\file   AAX_CAutoreleasePool.h
 *	
 *	\brief Autorelease pool helper utility
 */ 
/*================================================================================================*/


#pragma once

#ifndef _AAX_CAUTORELEASEPOOL_H_
#define _AAX_CAUTORELEASEPOOL_H_


/*	\brief Creates an autorelease pool for the scope of the stack based class
	to clearn up any autoreleased memory that was allocated in the lifetime of
	the pool.
	
	\details
	This may be used on either Mac or Windows platforms and will not pull in
	any Cocoa dependencies.
	
	usage:
\code
{
	AAX_CAutoreleasePool myAutoReleasePool
	delete myCocoaObject;

	// Pool is released when the AAX_CAutoreleasePool is destroyed
}
\endcode
 */
class AAX_CAutoreleasePool
{
	public:
		AAX_CAutoreleasePool();
		~AAX_CAutoreleasePool();

	private:
		AAX_CAutoreleasePool (const AAX_CAutoreleasePool&);
		AAX_CAutoreleasePool& operator= (const AAX_CAutoreleasePool&);

	private:
		void* mAutoreleasePool; //!< Opaque pool instance
 };


#endif // #ifndef _AAX_CAUTORELEASEPOOL_H_
