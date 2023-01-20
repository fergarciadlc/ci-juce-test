/*================================================================================================*/
/*
 *	Copyright 2016-2017 by Avid Technology, Inc.
 *	All rights reserved.
 *
 *	CONFIDENTIAL: This document contains confidential information.  Do not
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT
 *	OF Avid Technology, INC.
 */

#ifndef AAXLibrary_AAX_IACFDescriptionHost_h
#define AAXLibrary_AAX_IACFDescriptionHost_h


#include "AAX.h"

class AAX_IACFFeatureInfo;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "acfbaseapi.h"
#include "acfunknown.h"

/** Interface to host services provided during plug-in description
 */
class AAX_IACFDescriptionHost : public IACFUnknown
{
public:
	// NOTE: Documentation is not copied directly from AAX_IDescriptionHost due to an adaptation of parameter types (IACFUnknown to AAX_IFeatureInfo)
	/**
	 *	\copybrief AAX_IDescriptionHost::AcquireFeatureProperties()
	 *
	 *	\p outFeatureProperties must support \ref AAX_IACFFeatureInfo const methods
	 *
	 *	\sa AAX_IDescriptionHost::AcquireFeatureProperties()
	 */
	virtual AAX_Result AcquireFeatureProperties(const AAX_Feature_UID& inFeatureID, IACFUnknown** outFeatureProperties) = 0;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif
