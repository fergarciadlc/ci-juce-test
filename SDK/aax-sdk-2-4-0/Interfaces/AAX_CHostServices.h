/*================================================================================================*/
/*
 *
 *	Copyright 2014-2015, 2018 by Avid Technology, Inc.
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
 *	\file  AAX_CHostServices.h
 *
 *	\brief Concrete implementation of the AAX_IHostServices interface.
 *
 */ 
/*================================================================================================*/


#ifndef AAX_CHOSTSERVICES_H
#define AAX_CHOSTSERVICES_H

#include "AAX.h"
#include "AAX_Enums.h"


class IACFUnknown;

/**	@brief Method access to a singleton implementation of the \ref AAX_IHostServices interface
 */
class AAX_CHostServices
{
public:
	static void Set ( IACFUnknown * pUnkHost );
	
	static AAX_Result HandleAssertFailure ( const char * iFile, int32_t iLine, const char * iNote, /* AAX_EAssertFlags */ int32_t iFlags = AAX_eAssertFlags_Default ); ///< \copydoc AAX_IHostServices::HandleAssertFailure()
	static AAX_Result Trace ( AAX_ETracePriorityHost iPriority, const char * iMessage, ... ); ///< \copydoc AAX_IHostServices::Trace()
	static AAX_Result StackTrace ( AAX_ETracePriorityHost iTracePriority, AAX_ETracePriorityHost iStackTracePriority, const char * iMessage, ... ); ///< \copydoc AAX_IHostServices::StackTrace()
};


#endif
