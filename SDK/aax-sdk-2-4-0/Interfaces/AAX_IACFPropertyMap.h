/*================================================================================================*/
/*
 *
 *	Copyright 2013-2017 by Avid Technology, Inc.
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
 *	\file  AAX_IACFPropertyMap.h
 *
 *	\brief Versioned interface for an AAX_IPropertyMap
 *
 */ 
/*================================================================================================*/


#ifndef AAX_IACFPROPERTYMAP_H
#define AAX_IACFPROPERTYMAP_H

#include "AAX.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "acfunknown.h"

/**	\brief Versioned interface for an \ref AAX_IPropertyMap
 */ 
class AAX_IACFPropertyMap : public IACFUnknown
{
public:
	virtual AAX_CBoolean		GetProperty ( AAX_EProperty inProperty, AAX_CPropertyValue * outValue ) const = 0;	///< \copydoc AAX_IPropertyMap::GetProperty()
	virtual AAX_Result			AddProperty ( AAX_EProperty inProperty, AAX_CPropertyValue inValue ) = 0;	///< \copydoc AAX_IPropertyMap::AddProperty()
	virtual AAX_Result			RemoveProperty ( AAX_EProperty inProperty ) = 0;	///< \copydoc AAX_IPropertyMap::RemoveProperty()
};

/**	\brief Versioned interface for an \ref AAX_IPropertyMap
 */ 
class AAX_IACFPropertyMap_V2 : public AAX_IACFPropertyMap
{
public:
    virtual AAX_Result      AddPropertyWithIDArray ( AAX_EProperty inProperty, const AAX_SPlugInIdentifierTriad* inPluginIDs, uint32_t inNumPluginIDs) = 0;	///< \copydoc AAX_IPropertyMap::AddPropertyWithIDArray()
    virtual AAX_CBoolean    GetPropertyWithIDArray ( AAX_EProperty inProperty, const AAX_SPlugInIdentifierTriad** outPluginIDs, uint32_t* outNumPluginIDs) const = 0;	///< \copydoc AAX_IPropertyMap::GetPropertyWithIDArray()
};

/**	\brief Versioned interface for an \ref AAX_IPropertyMap
 */
class AAX_IACFPropertyMap_V3 : public AAX_IACFPropertyMap_V2
{
public:
	virtual AAX_CBoolean		GetProperty64 ( AAX_EProperty inProperty, AAX_CPropertyValue64 * outValue ) const = 0;	///< \copydoc AAX_IPropertyMap::GetProperty()
	virtual AAX_Result			AddProperty64 ( AAX_EProperty inProperty, AAX_CPropertyValue64 inValue ) = 0;	///< \copydoc AAX_IPropertyMap::AddProperty()
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // AAX_IACFPROPERTYMAP_H
