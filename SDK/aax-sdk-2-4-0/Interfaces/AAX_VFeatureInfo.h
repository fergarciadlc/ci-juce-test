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

#ifndef AAXLibrary_AAX_VFeatureInfo_h
#define AAXLibrary_AAX_VFeatureInfo_h

#include "AAX_IFeatureInfo.h"

#include "ACFPtr.h"
#include "acfbasetypes.h"


class AAX_IPropertyMap;
class AAX_IACFFeatureInfo;


/** Concrete implementation of \ref AAX_IFeatureInfo, which provides a version-controlled
 interface to host feature information
 */
class AAX_VFeatureInfo : public AAX_IFeatureInfo
{
public:
	explicit AAX_VFeatureInfo( IACFUnknown* pUnknown, const AAX_Feature_UID& inFeatureID );
	~AAX_VFeatureInfo() AAX_OVERRIDE;
	
public: // AAX_IFeatureInfo
	AAX_Result SupportLevel(AAX_ESupportLevel& oSupportLevel) const AAX_OVERRIDE; ///< \copydoc AAX_IFeatureInfo::SupportLevel()
	const AAX_IPropertyMap* AcquireProperties() const AAX_OVERRIDE; ///< \copydoc AAX_IFeatureInfo::AcquireProperties()
	const AAX_Feature_UID& ID() const AAX_OVERRIDE; ///< \copydoc AAX_IFeatureInfo::ID()
	
private:
	AAX_Feature_UID mFeatureID;
	ACFPtr<AAX_IACFFeatureInfo> mIFeature;
};


#endif
