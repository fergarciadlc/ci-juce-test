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
 *	\file  AAX_Init.h
 *
 *	\brief %AAX library implementations of required plug-in initialization, registration,
 *	and tear-down methods.
 *
 */ 
/*================================================================================================*/


#include "AAX.h"
#include "acfbasetypes.h"

class IACFUnknown;
class IACFPluginDefinition;
class IACFComponentDefinition;


/** @copybrief ACFRegisterPlugin()
	
	@details		
	This method determines the number of components defined in the dll. The
	implementation of this method in the %AAX library calls the following function, 
	which must be implemented somewhere in your plug-in:
	
	\code
		extern AAX_Result GetEffectDescriptions( AAX_ICollection * outCollection );
	\endcode
	
	<EM>Wrapped by \ref ACFRegisterPlugin() </EM>

	\ingroup CommonInterface_Describe
*/
AAX_Result AAXRegisterPlugin(IACFUnknown * pUnkHost, IACFPluginDefinition **ppPluginDefinition);

/** @copybrief ACFRegisterComponent()

	@details		
	The implementation of this method in the %AAX library simply sets
	\c *ppComponentDefinition to NULL and returns \ref AAX_SUCCESS.

	<EM>Wrapped by \ref ACFRegisterComponent() </EM>
*/
AAX_Result AAXRegisterComponent (IACFUnknown * pUnkHost, acfUInt32 index, IACFComponentDefinition **ppComponentDefinition);

/** @copybrief ACFGetClassFactory()

	@details		
	This method is required by ACF but is not supported by AAX. Therefore the
	implementation of this method in the %AAX library simply sets \c *ppOut to NULL
	and returns \ref AAX_ERROR_UNIMPLEMENTED.

	<EM>Wrapped by \ref ACFGetClassFactory() </EM>
*/
AAX_Result AAXGetClassFactory (IACFUnknown * pUnkHost, const acfCLSID& clsid, const acfIID& iid, void** ppOut);

/** @copybrief ACFCanUnloadNow()
	
	@details		
	The implementation of this method in the %AAX library returns the result of
	\c GetActiveObjectCount() as an \ref AAX_Result, with zero active objects
	interpreted as \ref AAX_SUCCESS (see CACFUnknown.h)

	<EM>Wrapped by \ref ACFCanUnloadNow() </EM>
*/
AAX_Result AAXCanUnloadNow(IACFUnknown* pUnkHost);

/** @copybrief ACFStartup()

	@details		
	Called once at init time. The implementation of this method in the %AAX library
	uses \c pUnkHost as an \c IACFComponentFactory to initialize global services
	(see acfbaseapi.h)

	<EM>Wrapped by \ref ACFStartup() </EM>
*/
AAX_Result AAXStartup(IACFUnknown* pUnkHost);

/** @copybrief ACFShutdown()

	@details		
	Called once before unloading the DLL. The implementation of this method in the %AAX
	library tears down any globally initialized state and releases any globally
	retained resources.

	<EM>Wrapped by \ref ACFShutdown() </EM>
*/
AAX_Result AAXShutdown(IACFUnknown* pUnkHost);

/** @copybrief ACFGetSDKVersion()
	
	@details		
	The implementation of this method in the %AAX library provides a 64-bit value in
	which the upper 32 bits represent the SDK version and the lower 32 bits represent
	the revision number of the SDK. See \ref AAX_Version.h

	<EM>Wrapped by \ref ACFGetSDKVersion() </EM>
*/
AAX_Result AAXGetSDKVersion( acfUInt64 *oSDKVersion );
