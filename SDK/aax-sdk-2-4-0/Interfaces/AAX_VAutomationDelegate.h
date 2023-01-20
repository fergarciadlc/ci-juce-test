/*================================================================================================*/
/*
 *
 *	Copyright 2014-2017, 2019 by Avid Technology, Inc.
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
 *	\file  AAX_VAutomationDelegate.h
 *
 *	\brief Version-managed concrete AutomationDelegate  class
 *
 */ 
/*================================================================================================*/

#ifndef AAX_VAUTOMATIONDELEGATE_H
#define AAX_VAUTOMATIONDELEGATE_H

#include "AAX_IAutomationDelegate.h"
#include "AAX_IACFAutomationDelegate.h"
#include "ACFPtr.h"

class AAX_IACFAutomationDelegate;
class IACFUnknown;

/**
 *	\brief Version-managed concrete \ref AAX_IAutomationDelegate "automation delegate" class
 *
 */
class AAX_VAutomationDelegate : public AAX_IAutomationDelegate
{
public:
	AAX_VAutomationDelegate( IACFUnknown * pUnknown );
	~AAX_VAutomationDelegate() AAX_OVERRIDE;
	
	IACFUnknown*	GetUnknown() const { return mIAutomationDelegate; }
	
	AAX_Result		RegisterParameter ( AAX_CParamID iParameterID ) AAX_OVERRIDE;
	AAX_Result		UnregisterParameter ( AAX_CParamID iParameterID ) AAX_OVERRIDE;
	AAX_Result		PostSetValueRequest ( AAX_CParamID iParameterID, double iNormalizedValue ) const AAX_OVERRIDE;
	AAX_Result		PostCurrentValue ( AAX_CParamID iParameterID, double iNormalizedValue ) const AAX_OVERRIDE;
	AAX_Result		PostTouchRequest ( AAX_CParamID iParameterID ) AAX_OVERRIDE;
	AAX_Result		PostReleaseRequest ( AAX_CParamID iParameterID ) AAX_OVERRIDE;
	AAX_Result		GetTouchState ( AAX_CParamID iParameterID, AAX_CBoolean * outTouched ) AAX_OVERRIDE;
	
private:
	ACFPtr<AAX_IACFAutomationDelegate>	mIAutomationDelegate;
};



#endif //AAX_IAUTOMATIONDELEGATE_H
