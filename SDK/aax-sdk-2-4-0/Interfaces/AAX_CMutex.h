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

#ifndef AAX_CMUTEX_H
#define AAX_CMUTEX_H

/**  
 *	\file  AAX_CMutex.h
 *
 *	\brief Mutex
 *
 */ 
/*================================================================================================*/

/**	\brief Mutex with try lock functionality
 */
class AAX_CMutex
{
public:
	AAX_CMutex();
	~AAX_CMutex();
	
    bool Lock();
    void Unlock();
    bool Try_Lock();
	
private:
    AAX_CMutex(const AAX_CMutex&);
    AAX_CMutex& operator=(const AAX_CMutex&);

	typedef struct opaque_aax_mutex_t * aax_mutex_t;	
	aax_mutex_t		mMutex;	
};

/** \brief Helper class for working with mutex
 */
class AAX_StLock_Guard
{
public:
    explicit AAX_StLock_Guard(AAX_CMutex& iMutex) : mMutex(iMutex) { mNeedsUnlock = mMutex.Lock(); }
    ~AAX_StLock_Guard() { if (mNeedsUnlock) mMutex.Unlock(); }
	
private:	
    AAX_StLock_Guard(AAX_StLock_Guard const&);
    AAX_StLock_Guard& operator=(AAX_StLock_Guard const&);

	AAX_CMutex &	mMutex;
	bool			mNeedsUnlock;
};

#endif // AAX_CMUTEX_H

