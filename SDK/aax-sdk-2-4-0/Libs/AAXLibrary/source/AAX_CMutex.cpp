/*================================================================================================*/
/*
 *	Copyright 2011-2015, 2019 by Avid Technology, Inc.
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
 *	\file   AAX_CMutex.cpp
 *	
 *	\author Viktor Iarovyi
 *
 */ 
/*================================================================================================*/

#include "AAX_CMutex.h"

#if defined(__GNUC__)
#include <pthread.h>
#include <errno.h>
#elif defined(WIN32)
#include <windows.h>
#else
#error AAX_CMutex not implemented
#endif

struct opaque_aax_mutex_t
{
#if defined(__GNUC__)
	pthread_t		mOwner;
	pthread_mutex_t	mSysMutex;	
#elif defined(WIN32)
	DWORD			mOwner;
	HANDLE			mSysMutex;	
#endif
};

// ******************************************************************************************
// METHOD:	AAX_CMutex
// ******************************************************************************************
AAX_CMutex::AAX_CMutex()
{
	mMutex = new opaque_aax_mutex_t;
	mMutex->mOwner = 0;
#if defined(__GNUC__)
	if (::pthread_mutex_init(&mMutex->mSysMutex, NULL) != 0)
#elif defined(WIN32)			
	mMutex->mSysMutex = ::CreateMutex(NULL, false, NULL);
	if (0 == mMutex->mSysMutex)
#endif
	{
		delete mMutex;
		mMutex = 0;
	}
}
	
// ******************************************************************************************
// METHOD:	~AAX_CMutex
// ******************************************************************************************
AAX_CMutex::~AAX_CMutex()
{
	if (mMutex)
	{
#if defined(__GNUC__)
		::pthread_mutex_destroy(&mMutex->mSysMutex);
#elif defined(WIN32)	
		::CloseHandle(mMutex->mSysMutex);
#endif
		delete mMutex;		
		mMutex = 0;
	}
}

// ******************************************************************************************
// METHOD:	Lock
// ******************************************************************************************
bool AAX_CMutex::Lock()
{
	bool result = false;
	if (mMutex)
	{			
#if defined(__GNUC__)
		pthread_t curThread = ::pthread_self();
		if(! ::pthread_equal(curThread, mMutex->mOwner))
		{
			::pthread_mutex_lock(&mMutex->mSysMutex);
			mMutex->mOwner = curThread;
			result = true;
		}
#elif defined(WIN32)
		DWORD curThread = ::GetCurrentThreadId();
		if(mMutex->mOwner != curThread)
		{
			::WaitForSingleObject(mMutex->mSysMutex, INFINITE);
			mMutex->mOwner = curThread;
			result = true;
		}
#endif
	}
	return result;
}

// ******************************************************************************************
// METHOD:	Unlock
// ******************************************************************************************
void AAX_CMutex::Unlock()
{
	if (mMutex)
	{					
#if defined(__GNUC__)
		if(::pthread_equal(::pthread_self(), mMutex->mOwner))
		{
			mMutex->mOwner = 0;
			::pthread_mutex_unlock(&mMutex->mSysMutex);
		}
#elif defined(WIN32)
		if(mMutex->mOwner == ::GetCurrentThreadId())
		{
			mMutex->mOwner = 0;
			::ReleaseMutex(mMutex->mSysMutex);
		}
#endif
	}
}
	
// ******************************************************************************************
// METHOD:	Try_Lock
// ******************************************************************************************
bool AAX_CMutex::Try_Lock()
{
	bool result = false;
	
	if (mMutex)
	{							
#if defined(__GNUC__)
		pthread_t curThread = ::pthread_self();
		if(! ::pthread_equal(curThread, mMutex->mOwner))
		{
			// current thread doesn't own the Lock - try lock to see if we can lock it
			int err = ::pthread_mutex_trylock(&mMutex->mSysMutex);
			if (0 == err)
			{
				// we locked the lock
				mMutex->mOwner = curThread;
				result = true;
			}
			else if (EBUSY == err)
			{
				// the Lock was already locked by another thread
				result = false;
			}
			else
			{
				// it's bad
				result = false;
			}
		}
		else
		{
			// current thread already owns the lock
			result = true;
		}
#elif defined(WIN32)
		if(mMutex->mOwner != ::GetCurrentThreadId())
		{
			// try to acquire the mutex
			DWORD err = ::WaitForSingleObject(mMutex->mSysMutex, 0);
			if (WAIT_OBJECT_0 == err)
			{
				// we locked the lock
				mMutex->mOwner = ::GetCurrentThreadId();
				result = true;
			}
			else if (WAIT_TIMEOUT == err)
			{
				// lock was already locked by another thread
				result = false;
			}
			else
			{
				// it's bad
				result = false;
			}
		}
		else
		{
			// current thread already owns the lock
			result = true;
		}
#endif	
	}
	return result;
}
