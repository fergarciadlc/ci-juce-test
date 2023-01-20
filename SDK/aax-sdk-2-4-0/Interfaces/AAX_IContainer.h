/*================================================================================================*/
/*
 *
 *	Copyright 2015 by Avid Technology, Inc.
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
 *  \file AAX_IContainer.h
 *
 *	\brief Abstract container interface
 *
 */
/*================================================================================================*/
/// @cond ignore
#ifndef AAX_ICONTAINER_H
#define AAX_ICONTAINER_H
/// @endcond


/** Abstract container interface
 */
class AAX_IContainer
{
public:
	virtual ~AAX_IContainer() {}
	
public:
	enum EStatus
	{
		eStatus_Success = 0          ///< Operation succeeded
		,eStatus_Overflow = 1        ///< Internal buffer overflow
		,eStatus_NotInitialized = 2  ///< Uninitialized container
		,eStatus_Unavailable = 3     ///< An internal resource was not available
		,eStatus_Unsupported = 4     ///< Operation is unsupported
	};
	
public:
	/** Clear the container
	 */
	virtual void Clear() = 0;
};

/// @cond ignore
#endif /* defined(AAX_ICONTAINER_H) */
/// @endcond
