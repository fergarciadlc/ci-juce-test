/*================================================================================================*/
/*
 *	Copyright 2015 by Avid Technology, Inc.
 *	All rights reserved.
 *
 *	CONFIDENTIAL: This document contains confidential information.  Do not 
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
 *	OF Avid Technology, INC.
 */
/*================================================================================================*/

#include "AAX_PlugInBundleLocation.h"
#include <intrin.h>
#include <windows.h>
#pragma intrinsic(_ReturnAddress)

bool AAX::GetPathToPlugInBundle (const char* iBundleName, int iMaxLength, char* oModuleName)
{
	if (!oModuleName)
		return false;

	HMODULE module = NULL;
	int success = GetModuleHandleEx( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)_ReturnAddress(), &module);

	if (success)
	{
		if ((0 == GetModuleFileName(module,oModuleName,iMaxLength)) ||
			(ERROR_SUCCESS != GetLastError()))
			return false;
	}
	else
		return false;

	int len = strlen(oModuleName);
	int goUpLevels = 3;
	for (int i = len-1; goUpLevels && i >= 0; --i)
	{
		if (oModuleName[i] == '\\' || oModuleName[i] == '/')
			--goUpLevels;

		oModuleName[i] = '\0';
	}

	if (goUpLevels) // something went wrong
		return false;

	return true;
}
