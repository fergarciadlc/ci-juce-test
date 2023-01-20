/*================================================================================================*/
/*
 *	Copyright 2016-2017, 2019 by Avid Technology, Inc.
 *	All rights reserved.
 *
 *	CONFIDENTIAL: This document contains confidential information.  Do not
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT
 *	OF Avid Technology, INC.
 */

// Self include
#include "AAX_VDescriptionHost.h"

// AAX includes
#include "AAX_VFeatureInfo.h"
#include "AAX_IACFDescriptionHost.h"
#include "AAX_UIDs.h"

// ACF includes
#include "acfbaseapi.h"

// Standard includes
#include <memory>


AAX_VDescriptionHost::AAX_VDescriptionHost( IACFUnknown* pUnknown )
{
	if ( pUnknown )
	{
		pUnknown->QueryInterface(IID_IAAXDescriptionHostV1, (void **)&mDescriptionHost);
		pUnknown->QueryInterface(IID_IACFDefinition, (void**)&mHostInformation);
	}
}

AAX_VDescriptionHost::~AAX_VDescriptionHost()
{
}

const AAX_IFeatureInfo* AAX_VDescriptionHost::AcquireFeatureProperties(const AAX_Feature_UID& inFeatureID) const
{
	AAX_UNIQUE_PTR(const AAX_IFeatureInfo) acquiredFeatureProperties;
	
	if ( mDescriptionHost )
	{
		IACFUnknown* featureInfo = NULL;
		
		// const cast is OK here because we are ultimately storing the result of the non-const acquire call in a const object
		AAX_IACFDescriptionHost* descHost = const_cast<AAX_IACFDescriptionHost*>(mDescriptionHost.inArg());
		if (AAX_SUCCESS == descHost->AcquireFeatureProperties(inFeatureID, &featureInfo) && (NULL != featureInfo))
		{
			acquiredFeatureProperties.reset(new AAX_VFeatureInfo(featureInfo, inFeatureID));
		}
	}
	
	return acquiredFeatureProperties.release();
}




