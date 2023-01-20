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
 *	\file  AAX_VPrivateDataAccess.h
 *
 *	\brief Version-managed concrete PrivateDataAccess class
 *
 */ 
/*================================================================================================*/
#ifndef AAX_VPRIVATEDATAACCESS_H
#define AAX_VPRIVATEDATAACCESS_H

#include "AAX_IPrivateDataAccess.h"
#include "AAX_IACFPrivateDataAccess.h"
#include "ACFPtr.h"


class IACFUnknown;

/**
 *	\brief Version-managed concrete \ref AAX_IPrivateDataAccess class
 *
 */
class AAX_VPrivateDataAccess : public AAX_IPrivateDataAccess
{
public:
	AAX_VPrivateDataAccess( IACFUnknown* pUnknown );
	~AAX_VPrivateDataAccess() AAX_OVERRIDE;

	// Direct access methods
	AAX_Result	ReadPortDirect( AAX_CFieldIndex inFieldIndex, const uint32_t inOffset, const uint32_t inSize, void* outBuffer ) AAX_OVERRIDE; ///< \copydoc AAX_IPrivateDataAccess::ReadPortDirect()
	AAX_Result	WritePortDirect( AAX_CFieldIndex inFieldIndex, const uint32_t inOffset, const uint32_t inSize, const void* inBuffer ) AAX_OVERRIDE; ///< \copydoc AAX_IPrivateDataAccess::WritePortDirect()


private:
	AAX_IACFPrivateDataAccess*	mIPrivateDataAccess;
};


#endif //AAX_VPRIVATEDATAACCESS_H

