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

/**	
 *      \file   AAX_PlugInBundleLocation.mm
 *
 *	\brief	Utilities for interacting with the host OS
 */
/*================================================================================================*/
#include "AAX_PlugInBundleLocation.h"

#import <Foundation/Foundation.h>

bool AAX::GetPathToPlugInBundle (const char* iBundleName, int iMaxLength, char* oModuleName)
{
    if (!oModuleName || !iBundleName)
        return false;
    
    @autoreleasepool
    {
        NSString * bundleIdentifier = [NSString stringWithFormat:@"%s", iBundleName];;
        NSBundle * bundle = [ NSBundle bundleWithIdentifier: bundleIdentifier ];
        NSString * bundlePath = [bundle bundlePath];
        strncpy(oModuleName, bundlePath.UTF8String, iMaxLength);
    }
    
    if (!strlen(oModuleName))
        return false;
    
    return true;
}
