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

#ifndef AAXLibrary_AAX_IFeatureInfo_h
#define AAXLibrary_AAX_IFeatureInfo_h

#include "AAX.h"


class AAX_IPropertyMap;


class AAX_IFeatureInfo
{
public:
	virtual ~AAX_IFeatureInfo() {}
	
public: // AAX_IACFFeatureInfo
	/** Determine the level of support for this feature by the host
	 
	 \note The host will not provide an underlying \ref AAX_IACFFeatureInfo interface for features which it does not
	 recognize at all, resulting in a \ref AAX_ERROR_NULL_OBJECT error code
	 */
	virtual AAX_Result SupportLevel(AAX_ESupportLevel& oSupportLevel) const = 0;
	
	/** Additional properties providing details of the feature support
	 
	 See the feature's UID for documentation of which features provide additional properties
	 
	 Ownership of the returned object is passed to the caller; the caller is responsible for destroying the object, e.g. by
	 capturing the returned object in a smart pointer.
	 
	 \code
	 // AAX_IFeatureInfo* featureInfo
	 std::unique_ptr<const AAX_IPropertyMap> featurePropertiesPtr(featureInfo->AcquireProperties();
	 \endcode
	 
	 \return An \ref AAX_IPropertyMap interface with access to the host's properties for this feature.
	 \return \c NULL if the desired feature was not found or if an error occurred
	 
	 \note May return an \ref AAX_IPropertyMap object with limited method support, which would return an error such as
	 \ref AAX_ERROR_NULL_OBJECT or \ref AAX_ERROR_UNIMPLEMENTED to interface calls.
	 
	 */
	virtual const AAX_IPropertyMap* AcquireProperties() const = 0;
	
public: // AAX_IFeatureInfo
	/** Returns the ID of the feature which this object represents
	 */
	virtual const AAX_Feature_UID& ID() const = 0;
};


#endif
