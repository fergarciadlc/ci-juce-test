/***********************************************************************

	The software and files contained herein are furnished under a 
	license agreement. The software: (1) is for internal use only; 
	(2) may not be redistributed; (3) may not be reverse assembled; 
	and (4) may be used or copied only in accordance with the terms 
	of the license agreement.  

	Copyright (c) 2004, 2021 Avid Technology, Inc. All rights reserved.

************************************************************************/


/*!
	\file CACFClassFactory.cpp
	\brief Implementation for the CACFClassFactory class.
*/

#include "CACFClassFactory.h"
#include "acfextras.h"
#include "acfassert.h"


ACFRESULT CACFClassFactory::Create(ACFCreateObjectProc pfnCreate, const acfIID& iid, void** ppOut)
{
    ACFRESULT result = ACF_OK;
    
	if (!pfnCreate || !ppOut)
		return ACF_E_INVALIDARG;
	*ppOut = 0;
		
	CACFClassFactory * classFactory = new (std::nothrow) CACFClassFactory(pfnCreate);
	if (!classFactory)
		return ACF_E_OUTOFMEMORY;
	
	// Take ownership of the classFactory (all objects are internally created with a starting
	// reference count of 0.
	classFactory->AddRef();
		
	// See if the factory supports the requested interface. If this succeeds
	// then the reference count will be two. If it fails the reference
	// count will be one and the factory object will be deleted when the classFactory
	// Release method is called below.
	result = classFactory->QueryInterface(iid, ppOut);
	
	// Release ownership of classFactory.
	classFactory->Release();

    return result;
}

// Implementation
CACFClassFactory::CACFClassFactory(ACFCreateObjectProc pfnCreate)
	: CACFUnknown(0),
	m_pfnCreate(pfnCreate)
{
	ACFASSERT (m_pfnCreate);
}

CACFClassFactory::~CACFClassFactory()
{
	m_pfnCreate = 0;
}

ACFRESULT CACFClassFactory::InternalQueryInterface 
(
	const acfIID & riid,
	void **ppvObj)
{
	if (!ppvObj)
		return ACF_E_INVALIDARG;

    if (riid == IID_IACFClassFactory) 
    { 
        *ppvObj = (IACFClassFactory *)this; 
        ( static_cast<IACFUnknown *>(*ppvObj) )->AddRef();
        return ACF_OK;
    }

	// Always delegate back to base implementation.
	return CACFUnknown::InternalQueryInterface(riid, ppvObj);
}


// Object creation is delegated to the callback function passed into
// the constructor.
ACFMETHODIMP CACFClassFactory::CreateInstance
(
	IACFUnknown * pUnkHost,
	IACFUnknown * pUnkOuter,
	const acfIID & riid,
	void * * ppvObj)
{
	ACFRESULT result = ACF_OK;
	
	if (!m_pfnCreate) // this instance was not initialized properly.
		return ACF_E_UNEXPECTED;
	if (!pUnkHost || !ppvObj)
		return ACF_E_INVALIDARG;

	*ppvObj = 0;
	
	// \note "Note that the non delegating versions of QueryInteface, 
	// AddRef, and Re lase are used. If a stand-alone identity is
	// being created, this is certainly appropriate. If an aggregate
	// is being created, this is necessary to ensure that the inner
	// object is Addrefed, not the outer object. Also note that 
	// the outer object must request IUnknown as the initial 
	// interface. This is mandated by the COM Specification.
	// If the outer object could request any initial interface, then
	// the inner object would essentially need to keep two duplicate
	// sets of vptrs, one set that delegated its QueryInterface, 
	// AddRef, and Release implementations and another set that did
	// not. By restricting the initial interface to IUnknown, the 
	// object implementor needs to isolate only the one vptr to act
	// as the non delegating IUnknown." (p. 194, "Essential COM", by
	// Don Box, Addison Wesley, 2nd Printing Feb. 1998)

	if (pUnkOuter && (IID_IACFUnknown != riid))
		return ACF_CLASS_E_NOAGGREGATION;

	// Ask the callback function to create the object instance.
	result = (*m_pfnCreate)(pUnkHost, pUnkOuter, riid, ppvObj);
	return result;
}
