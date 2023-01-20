/*================================================================================================*/
/*
 *	Copyright 2010-2015 by Avid Technology, Inc.
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

#include "AAX_IHostProcessor.h"
#include "AAX_UIDs.h"
#include "acfextras.h"

ACFMETHODIMP AAX_IHostProcessor::InternalQueryInterface(const acfIID & riid, void **ppvObjOut)
{
    if ((riid == IID_IAAXHostProcessorV2) || (riid == IID_IAAXHostProcessorV1))
    { 
		*ppvObjOut = static_cast<IACFUnknown *>(this);
        ( static_cast<IACFUnknown *>(*ppvObjOut))->AddRef();
        return ACF_OK;
    }
    
	return this->CACFUnknown::InternalQueryInterface(riid, ppvObjOut);	
}
