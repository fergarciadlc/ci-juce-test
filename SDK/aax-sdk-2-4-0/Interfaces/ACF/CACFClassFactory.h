/***********************************************************************

	The software and files contained herein are furnished under a 
	license agreement. The software: (1) is for internal use only; 
	(2) may not be redistributed; (3) may not be reverse assembled; 
	and (4) may be used or copied only in accordance with the terms 
	of the license agreement.  

	Copyright (c) 2004, 2018, 2021 Avid Technology, Inc. All rights reserved.

************************************************************************/




#ifndef CACFClassFactory_h
#define CACFClassFactory_h

#ifndef CACFUnknown_h
#include "CACFUnknown.h"
#endif

/*!
	\file CACFClassFactory.h
	\brief Define the object creation callback function that should be
	implemented as a static method for every concrete ACF object.
*/

typedef ACFRESULT (*ACFCreateObjectProc)(IACFUnknown *, IACFUnknown *, const acfIID& iid, void** ppOut); 

class CACFClassFactory : 
	public IACFClassFactory,
	public CACFUnknown
{
public:
    // Default factory method for creating class factories.
    static ACFRESULT Create (ACFCreateObjectProc pfnCreate, const acfIID& iid, void** ppOut);

	// IACFUnknown methods  
	ACF_DECLARE_STANDARD_UNKNOWN()
	
protected:
	CACFClassFactory(ACFCreateObjectProc createProc);

	virtual ~CACFClassFactory();

	// ACFUnknown override
	ACFMETHOD(InternalQueryInterface)(const acfIID & riid, void **ppv) ACF_OVERRIDE;

public:
	// IACFClassFactory methods 
	ACFMETHOD(CreateInstance)(IACFUnknown * pUnkHost, IACFUnknown * pUnkOuter, const acfIID & riid, void ** ppv) ACF_OVERRIDE;
  
private:
    CACFClassFactory(); // Default constructor is prohibited.
    CACFClassFactory(const CACFClassFactory&); // non-copyable

	ACFCreateObjectProc m_pfnCreate;
};


#endif // CACFClassFactory_h
