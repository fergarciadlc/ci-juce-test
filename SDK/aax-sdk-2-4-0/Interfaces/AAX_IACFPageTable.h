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

#ifndef AAXLibrary_AAX_IACFPageTable_h
#define AAXLibrary_AAX_IACFPageTable_h

// AAX Includes
#include "AAX.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

// ACF Includes
#include "acfunknown.h"

// Forward declarations
class AAX_IString;

/**	\brief Versioned interface to the host's representation of a plug-in instance's page table
 */
class AAX_IACFPageTable : public IACFUnknown
{
public:
	virtual AAX_Result Clear() = 0; ///< \copydoc AAX_IPageTable::Clear()
	virtual AAX_Result Empty(AAX_CBoolean& oEmpty) const = 0; ///< \copydoc AAX_IPageTable::Empty()
	virtual AAX_Result GetNumPages(int32_t& oNumPages) const = 0; ///< \copydoc AAX_IPageTable::GetNumPages()
	virtual AAX_Result InsertPage(int32_t iPage) = 0; ///< \copydoc AAX_IPageTable::InsertPage()
	virtual AAX_Result RemovePage(int32_t iPage) = 0; ///< \copydoc AAX_IPageTable::RemovePage()
	virtual AAX_Result GetNumMappedParameterIDs(int32_t iPage, int32_t& oNumParameterIdentifiers) const = 0; ///< \copydoc AAX_IPageTable::GetNumMappedParameterIDs()
	virtual AAX_Result ClearMappedParameter(int32_t iPage, int32_t iIndex) = 0; ///< \copydoc AAX_IPageTable::ClearMappedParameter()
	virtual AAX_Result GetMappedParameterID(int32_t iPage, int32_t iIndex, AAX_IString& oParameterIdentifier) const = 0; ///< \copydoc AAX_IPageTable::GetMappedParameterID()
	virtual AAX_Result MapParameterID(AAX_CParamID iParameterIdentifier, int32_t iPage, int32_t iIndex) = 0; ///< \copydoc AAX_IPageTable::MapParameterID()
};

/**	\brief Versioned interface to the host's representation of a plug-in instance's page table
 */
class AAX_IACFPageTable_V2 : public AAX_IACFPageTable
{
public:
	virtual AAX_Result GetNumParametersWithNameVariations(int32_t& oNumParameterIdentifiers) const = 0; ///< \copydoc AAX_IPageTable::GetNumParametersWithNameVariations()
	virtual AAX_Result GetNameVariationParameterIDAtIndex(int32_t iIndex, AAX_IString& oParameterIdentifier) const = 0; ///< \copydoc AAX_IPageTable::GetNameVariationParameterIDAtIndex()
	virtual AAX_Result GetNumNameVariationsForParameter(AAX_CPageTableParamID iParameterIdentifier, int32_t& oNumVariations) const = 0; ///< \copydoc AAX_IPageTable::GetNumNameVariationsForParameter()
	virtual AAX_Result GetParameterNameVariationAtIndex(AAX_CPageTableParamID iParameterIdentifier, int32_t iIndex, AAX_IString& oNameVariation, int32_t& oLength) const = 0; ///< \copydoc AAX_IPageTable::GetParameterNameVariationAtIndex()
	virtual AAX_Result GetParameterNameVariationOfLength(AAX_CPageTableParamID iParameterIdentifier, int32_t iLength, AAX_IString& oNameVariation) const = 0; ///< \copydoc AAX_IPageTable::GetParameterNameVariationOfLength()
	virtual AAX_Result ClearParameterNameVariations() = 0; ///< \copydoc AAX_IPageTable::ClearParameterNameVariations()
	virtual AAX_Result ClearNameVariationsForParameter(AAX_CPageTableParamID iParameterIdentifier) = 0; ///< \copydoc AAX_IPageTable::ClearNameVariationsForParameter()
	virtual AAX_Result SetParameterNameVariation(AAX_CPageTableParamID iParameterIdentifier, const AAX_IString& iNameVariation, int32_t iLength) = 0; ///< \copydoc AAX_IPageTable::SetParameterNameVariation()
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif


#endif // AAXLibrary_AAX_IACFPageTable_h
