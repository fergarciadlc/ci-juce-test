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
 *  \file AAX_PlugInBundleLocation.h
 *
 *	\brief	Utilities for interacting with the host OS
 */
/*================================================================================================*/
/// @cond ignore
#ifndef AAX_PLUGINBUNDLELOCATION_H
#define AAX_PLUGINBUNDLELOCATION_H
/// @endcond

namespace AAX
{
	/** @name Filesystem utilities
	 */
	//@{
    /** @brief Retrieve the file path of the .aaxplugin bundle
     
     @details
     @param[in] iBundleName
     - OS X: The @c CFBundleIdentifier value set in the plug-in's .plist file
     - Other: This parameter is ignored
     @param[in] iMaxLength
     @param[out] oModuleName
     A preallocated buffer of size @c iMaxLength
     
     @ingroup OtherExtensions
     */
    bool GetPathToPlugInBundle (const char* iBundleName, int iMaxLength, char* oModuleName);
    //@} Filesystem utilities
}

/// @cond ignore
#endif
/// @endcond
