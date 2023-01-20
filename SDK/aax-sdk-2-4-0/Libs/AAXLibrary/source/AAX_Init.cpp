/*================================================================================================*/
/*
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
/*================================================================================================*/

#include "AAX_Init.h"
#include "AAX_VCollection.h"
#include "AAX_CHostServices.h"

#include "AAX_Exception.h"
#include "AAX_Assert.h"
#include "AAX_Version.h"
#include "ACFPtr.h"
#include "CACFClassFactory.h"

// this function must be implemented somewhere in your plug-in
//   inCollection: The host-provided collection interface for describing this plug-in. Never NULL.
extern AAX_Result GetEffectDescriptions( AAX_ICollection * outCollection );

AAX_Result AAXRegisterPlugin(IACFUnknown * pUnkHost, IACFPluginDefinition **ppPluginDefinition)
{
	AAX_Result result = AAX_SUCCESS;
	
	try
	{
		AAX_VCollection	collection ( pUnkHost );
		AAX_CheckedResult checkedResult(GetEffectDescriptions ( &collection ));
		
		*ppPluginDefinition = 0;
		if ( collection.GetIUnknown()->QueryInterface(IID_IACFPluginDefinition, (void**)ppPluginDefinition) != ACF_OK )
			checkedResult = AAX_ERROR_NULL_OBJECT;
	}
	catch (const AAX::Exception::ResultError& ex)
	{
		result = ex.Result();
		AAX_TRACE_RELEASE(kAAX_Trace_Priority_High, "AAXRegisterPlugin exception caught: %s", ex.What().c_str());
	}
	catch (const AAX::Exception::Any& ex)
	{
		result = AAX_ERROR_UNKNOWN_EXCEPTION;
		AAX_TRACE_RELEASE(kAAX_Trace_Priority_High, "AAXRegisterPlugin exception caught: %s", ex.What().c_str());
	}
	catch (const std::exception& ex)
	{
		result = AAX_ERROR_UNKNOWN_EXCEPTION;
		AAX_TRACE_RELEASE(kAAX_Trace_Priority_High, "AAXRegisterPlugin exception caught: %s", ex.what());
	}
	catch (...)
	{
		result = AAX_ERROR_UNKNOWN_EXCEPTION;
		AAX_TRACE_RELEASE(kAAX_Trace_Priority_High, "AAXRegisterPlugin exception caught: unknown");
	}
	
	return result;
}	

AAX_Result AAXRegisterComponent (IACFUnknown * /*pUnkHost*/, acfUInt32 /*index*/, IACFComponentDefinition **ppComponentDefinition)
{
	*ppComponentDefinition = NULL;
	return AAX_SUCCESS;
}

AAX_Result AAXGetClassFactory (IACFUnknown * /*pUnkHost*/, const acfCLSID& /*clsid*/, const acfIID& /*iid*/, void** ppOut)
{
	*ppOut = NULL;	
	return AAX_ERROR_UNIMPLEMENTED;
}

AAX_Result AAXCanUnloadNow(IACFUnknown* /*pUnkHost*/)
{
	return static_cast<AAX_Result>(CACFUnknown::GetActiveObjectCount());
}

AAX_Result AAXStartup(IACFUnknown* pUnkHost)
{
	AAX_CHostServices::Set ( pUnkHost );
	return AAX_SUCCESS;
}

AAX_Result AAXShutdown(IACFUnknown* /*pUnkHost*/)
{
	AAX_CHostServices::Set(NULL);	
	return AAX_SUCCESS;
}

AAX_Result AAXGetSDKVersion( acfUInt64* oSDKVersion )
{
    //Upper 32 bit uint is for SDK Version
    *oSDKVersion = acfUInt64(AAX_SDK_VERSION) << 32;
    
    //Lower 32 bit uint is for revision number.
    *oSDKVersion += acfUInt64(AAX_SDK_CURRENT_REVISION);
	return AAX_SUCCESS;
}
