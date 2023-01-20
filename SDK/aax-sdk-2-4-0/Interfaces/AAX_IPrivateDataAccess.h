/*================================================================================================*/
/*
 *
 *	Copyright 2014-2017 by Avid Technology, Inc.
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
 *	\file  AAX_IPrivateDataAccess.h
 *
 *	\brief Interface to data access provided by host to plug-in
 *
 */ 
/*================================================================================================*/


#ifndef AAX_IPRIVATEDATAACCESS_H
#define AAX_IPRIVATEDATAACCESS_H

#include "AAX.h"			


/** @brief Interface to data access provided by host to plug-in.
	
	@details
	@hostimp
	
	WARNING: AAX_IPrivateDataAccess objects are not reference counted and
			 are not guaranteed to exist beyond the scope of the method(s)
			 they are passed into.
	
	\sa AAX_IACFEffectDirectData::TimerWakeup
	
	\ingroup AuxInterface_DirectData
	
*/
class AAX_IPrivateDataAccess
{
public:	
	virtual ~AAX_IPrivateDataAccess() {}

	/** @brief	Read data directly from DSP at the given port.
	
	\note Blocking

	@param[in]	inFieldIndex
		The port to read from.
	@param[in]	inOffset
		Offset into data to start reading.
	@param[in]	inSize
		Amount of data to read (in bytes).
	@param[out]	outBuffer
		Pointer to storage for data to be read into.

	*/
	virtual	AAX_Result				ReadPortDirect( AAX_CFieldIndex inFieldIndex, const uint32_t inOffset, const uint32_t inSize, void* outBuffer ) = 0;

	/** @brief	Write data directly to DSP at the given port.

	\note Blocking

	@param[in]	inFieldIndex
		The port to write to.
	@param[in]	inOffset
		Offset into data to begin writing.
	@param[in]	inSize
		Amount of data to write (in bytes).
	@param[in]	inBuffer
		Pointer to data being written.
	*/
	virtual	AAX_Result				WritePortDirect( AAX_CFieldIndex inFieldIndex, const uint32_t inOffset, const uint32_t inSize, const void* inBuffer ) = 0;
};

#endif //AAX_IPRIVATEDATAACCESS_H
