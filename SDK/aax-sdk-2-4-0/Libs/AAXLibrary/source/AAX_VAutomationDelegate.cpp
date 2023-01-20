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

#include "AAX_VAutomationDelegate.h"
#include "AAX_UIDs.h"

// ******************************************************************************************
// METHOD:	AAX_VAutomationDelegate
// ******************************************************************************************
AAX_VAutomationDelegate::AAX_VAutomationDelegate( IACFUnknown * pUnknown )
{
	if ( pUnknown )
	{
		pUnknown->QueryInterface(IID_IAAXAutomationDelegateV1, (void **)&mIAutomationDelegate);
	}
}

// ******************************************************************************************
// METHOD:	~AAX_VAutomationDelegate
// ******************************************************************************************
AAX_VAutomationDelegate::~AAX_VAutomationDelegate()
{
}

// ******************************************************************************************
// METHOD:	RegisterControl
// ******************************************************************************************
AAX_Result AAX_VAutomationDelegate::RegisterParameter ( AAX_CParamID iParameterID )
{
	if ( mIAutomationDelegate )
		return mIAutomationDelegate->RegisterParameter ( iParameterID );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	UnregisterControl
// ******************************************************************************************
AAX_Result AAX_VAutomationDelegate::UnregisterParameter ( AAX_CParamID iParameterID )
{
	if ( mIAutomationDelegate )
		return mIAutomationDelegate->UnregisterParameter ( iParameterID );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	PostSetValueRequest
// ******************************************************************************************
AAX_Result AAX_VAutomationDelegate::PostSetValueRequest ( AAX_CParamID iParameterID, double iNormalizedValue ) const
{
	if ( mIAutomationDelegate )
		return mIAutomationDelegate->PostSetValueRequest ( iParameterID, iNormalizedValue );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	PostCurrentValue
// ******************************************************************************************
AAX_Result AAX_VAutomationDelegate::PostCurrentValue ( AAX_CParamID iParameterID, double iNormalizedValue ) const
{
	if ( mIAutomationDelegate )
		return mIAutomationDelegate->PostCurrentValue ( iParameterID, iNormalizedValue );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	PostTouchRequest
// ******************************************************************************************
AAX_Result AAX_VAutomationDelegate::PostTouchRequest ( AAX_CParamID iParameterID )
{
	if ( mIAutomationDelegate )
		return mIAutomationDelegate->PostTouchRequest ( iParameterID );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	PostReleaseRequest
// ******************************************************************************************
AAX_Result AAX_VAutomationDelegate::PostReleaseRequest ( AAX_CParamID iParameterID )
{
	if ( mIAutomationDelegate )
		return mIAutomationDelegate->PostReleaseRequest ( iParameterID );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetTouchState
// ******************************************************************************************
AAX_Result AAX_VAutomationDelegate::GetTouchState ( AAX_CParamID iParameterID, AAX_CBoolean * outTouched )
{
	if ( mIAutomationDelegate )
		return mIAutomationDelegate->GetTouchState ( iParameterID, outTouched );
	
	return AAX_ERROR_NULL_OBJECT;
}
