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
/*================================================================================================*/

#include "AAX_VPrivateDataAccess.h"
#include "AAX_UIDs.h"

// ******************************************************************************************
// METHOD:	AAX_VPrivateDataAccess
// ******************************************************************************************
AAX_VPrivateDataAccess::AAX_VPrivateDataAccess( IACFUnknown* pUnknown )
:	mIPrivateDataAccess(NULL)
{
	if ( pUnknown )
	{
		pUnknown->QueryInterface(IID_IAAXPrivateDataAccessV1, (void **)&mIPrivateDataAccess);
	}	
}

// ******************************************************************************************
// METHOD:	~AAX_VPrivateDataAccess
// ******************************************************************************************
AAX_VPrivateDataAccess::~AAX_VPrivateDataAccess()
{
}

// ******************************************************************************************
// METHOD:	ReadPortDirect
// ******************************************************************************************
AAX_Result AAX_VPrivateDataAccess::ReadPortDirect( AAX_CFieldIndex inFieldIndex, const uint32_t inOffset, const uint32_t inSize, void* outBuffer )
{
	if ( mIPrivateDataAccess )
		return mIPrivateDataAccess->ReadPortDirect ( inFieldIndex, inOffset, inSize, outBuffer );

	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	WritePortDirect
// ******************************************************************************************
AAX_Result AAX_VPrivateDataAccess::WritePortDirect( AAX_CFieldIndex inFieldIndex, const uint32_t inOffset, const uint32_t inSize, const void* inBuffer )
{
	if ( mIPrivateDataAccess )
		return mIPrivateDataAccess->WritePortDirect ( inFieldIndex, inOffset, inSize, inBuffer );

	return AAX_ERROR_NULL_OBJECT;
}







