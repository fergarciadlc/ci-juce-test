/*================================================================================================*/
/*
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
/*================================================================================================*/

#include "AAX_IEffectParameters.h"
#include "AAX_UIDs.h"
#include "acfextras.h"

ACFMETHODIMP AAX_IEffectParameters::InternalQueryInterface(const acfIID & riid, void **ppvObjOut)
{
    if (riid == IID_IAAXEffectParametersV1 ||
		riid == IID_IAAXEffectParametersV2 ||
		riid == IID_IAAXEffectParametersV3 ||
		riid == IID_IAAXEffectParametersV4)
    { 
		*ppvObjOut = static_cast<IACFUnknown *>(this);
        ( static_cast<IACFUnknown *>(*ppvObjOut))->AddRef();
        return ACF_OK;
    }
	
	return this->CACFUnknown::InternalQueryInterface(riid, ppvObjOut);	
}
