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

#ifndef AAXLibrary_AAX_VPageTable_h
#define AAXLibrary_AAX_VPageTable_h

#include "AAX_IPageTable.h"
#include "AAX_IACFPageTable.h"
#include "ACFPtr.h"

/**
 *	\brief Version-managed concrete \ref AAX_IPageTable class
 *
 */
class AAX_VPageTable : public AAX_IPageTable
{
public:
	AAX_VPageTable( IACFUnknown* pUnknown );
	~AAX_VPageTable() AAX_OVERRIDE;
	
	// AAX_IACFPageTable
	AAX_Result Clear() AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::Clear()
	AAX_Result Empty(AAX_CBoolean& oEmpty) const AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::Empty()
	AAX_Result GetNumPages(int32_t& oNumPages) const AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::GetNumPages()
	AAX_Result InsertPage(int32_t iPage) AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::InsertPage()
	AAX_Result RemovePage(int32_t iPage) AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::RemovePage()
	AAX_Result GetNumMappedParameterIDs(int32_t iPage, int32_t& oNumParameterIdentifiers) const AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::GetNumMappedParameterIDs()
	AAX_Result ClearMappedParameter(int32_t iPage, int32_t iIndex) AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::ClearMappedParameter()
	AAX_Result GetMappedParameterID(int32_t iPage, int32_t iIndex, AAX_IString& oParameterIdentifier) const AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::GetMappedParameterID()
	AAX_Result MapParameterID(AAX_CParamID iParameterIdentifier, int32_t iPage, int32_t iIndex) AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::MapParameterID()
	
	// AAX_IACFPageTable_V2
	AAX_Result GetNumParametersWithNameVariations(int32_t& oNumParameterIdentifiers) const AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::GetNumParametersWithNameVariations()
	AAX_Result GetNameVariationParameterIDAtIndex(int32_t iIndex, AAX_IString& oParameterIdentifier) const AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::GetNameVariationParameterIDAtIndex()
	AAX_Result GetNumNameVariationsForParameter(AAX_CPageTableParamID iParameterIdentifier, int32_t& oNumVariations) const AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::GetNumNameVariationsForParameter()
	AAX_Result GetParameterNameVariationAtIndex(AAX_CPageTableParamID iParameterIdentifier, int32_t iIndex, AAX_IString& oNameVariation, int32_t& oLength) const AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::GetParameterNameVariationAtIndex()
	AAX_Result GetParameterNameVariationOfLength(AAX_CPageTableParamID iParameterIdentifier, int32_t iLength, AAX_IString& oNameVariation) const AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::GetParameterNameVariationOfLength()
	AAX_Result ClearParameterNameVariations() AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::ClearParameterNameVariations()
	AAX_Result ClearNameVariationsForParameter(AAX_CPageTableParamID iParameterIdentifier) AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::ClearNameVariationsForParameter()
	AAX_Result SetParameterNameVariation(AAX_CPageTableParamID iParameterIdentifier, const AAX_IString& iNameVariation, int32_t iLength) AAX_OVERRIDE; ///< \copydoc AAX_IPageTable::SetParameterNameVariation()
	
	// AAX_VPageTable
	
	/** Returns the latest supported versioned ACF interface (e.g. an \ref AAX_IACFPageTable) which
	 is wrapped by this \ref AAX_IPageTable
	 */
	const IACFUnknown* AsUnknown() const
	{
		return mIPageTable.inArg();
	}
	
	/** \copydoc AAX_VPageTable::AsUnknown() const
	 */
	IACFUnknown* AsUnknown()
	{
		return mIPageTable.inArg();
	}
	
	bool IsSupported() const { return !mIPageTable.isNull(); }
	
private:
	ACFPtr<AAX_IACFPageTable>		mIPageTable;
	ACFPtr<AAX_IACFPageTable_V2>	mIPageTable2;
};

#endif
