/*================================================================================================*/
/*
 *	Copyright 2009-2015 by Avid Technology, Inc.
 *	All rights reserved.
 *
 *	CONFIDENTIAL: This document contains confidential information.  Do not 
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
 *	OF Avid Technology, INC.
 */

/**  
 *	\file  AAX_Alignment.h
 *
 *	\brief Alignment malloc and free methods for optimization
 *
 */ 
/*================================================================================================*/

#ifndef AAX_ALIGNMENT_H
#define AAX_ALIGNMENT_H

#include <stddef.h>

namespace AAX
{

	inline void alignFree(void *p)
	{  	
		char** aTempPtr=reinterpret_cast<char**>(p);
		aTempPtr--;							//backup 4 bytes past the beginning of the buffer
		char* aRealPtr = aTempPtr[0];		//Get the real address

		if(aRealPtr)
			::delete[] aRealPtr;
	}

	template <class T> 
	T* alignMalloc(int iArraySize, int iAlignment)
	{
		// We can seriously mess ourselves up if alignment is not a power of 2
		if ((iAlignment != 2) && (iAlignment != 4) && (iAlignment != 8) && (iAlignment != 16) && (iAlignment != 32)) {
			return 0;
		}
		// We can't allocate a negative-size array
		if (iArraySize <= 0) {
			return 0;
		}

		const unsigned int cSizeOfPointer = sizeof(char*);
		// Over-allocate memory by the maximum offset we could be from our requested alignment
		char* aRealPtr = ::new char[iArraySize*sizeof(T) + iAlignment + cSizeOfPointer];
		if (!aRealPtr) {
			return 0;
		}
		char* p=aRealPtr;
		p+=cSizeOfPointer;	//Skip four bytes (we store the real base address here)
		size_t mod = size_t(p) & (iAlignment-1);
		if (mod)
			p += (iAlignment - mod);
		*reinterpret_cast<char**>(p-cSizeOfPointer)=aRealPtr;	//Save the real address.  We'll need it for delete.
		return (T*) p;
	}
} // namespace AAX

#endif //AAX_ALIGNMENT_H
