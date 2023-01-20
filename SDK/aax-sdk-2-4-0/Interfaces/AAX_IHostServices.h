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
 *	\file  AAX_IHostServices.h
 *
 *	\brief Various host services
 *
 */ 
/*================================================================================================*/


#ifndef AAX_IHOSTSERVICES_H
#define AAX_IHOSTSERVICES_H

#include "AAX.h"

/**	\brief Interface to diagnostic and debugging services provided by the %AAX host
	
	\details
	\hostimp
	
	\sa \ref AAX_IACFHostServices
*/
class AAX_IHostServices
{
public:
	
	virtual ~AAX_IHostServices() {}
	
	/**	\brief Handle an assertion failure
	 
	 \details
	 Use this method to delegate assertion failure handling to the host
	 
	 Use \p inFlags to request that specific behavior be included when handling the
	 failure. This request may not be fulfilled by the host, and absence of a flag
	 does not preclude the host from using that behavior when handling the failure.
	 
	 \param[in] iFile
	 The name of the file containing the assert check. Usually \c __FILE__
	 \param[in] iLine
	 The line number of the assert check. Usually \c __LINE__
	 \param[in] iNote
	 Text to display related to the assert. Usually the condition which failed
	 \param[in] iFlags
	 Bitfield of \ref AAX_EAssertFlags to request specific handling behavior
	 */
	virtual AAX_Result HandleAssertFailure ( const char * iFile, int32_t iLine, const char * iNote, /* AAX_EAssertFlags */ int32_t iFlags ) const = 0;
	/**	\brief Log a trace message
	 
	 \param[in] iPriority
	 Priority of the trace, used for log filtering. One of \ref kAAX_Trace_Priority_Low, \ref kAAX_Trace_Priority_Normal, \ref kAAX_Trace_Priority_High
	 \param[in] iMessage
	 Message string to log
	 */
	virtual AAX_Result Trace ( int32_t iPriority, const char * iMessage ) const = 0;
	/** \brief Log a trace message or a stack trace
	 
	 If the logging output filtering is set to include logs with
	 \p iStackTracePriority then both the logging message and a stack trace will
	 be emitted, regardless of \p iTracePriority.
	 
	 If the logging output filtering is set to include logs with \p iTracePriority
	 but to exclude logs with \p iStackTracePriority then this will emit a normal
	 log with no stack trace.
	 
	 \param[in] iTracePriority
	 Priority of the trace, used for log filtering. One of \ref kAAX_Trace_Priority_Low, \ref kAAX_Trace_Priority_Normal, \ref kAAX_Trace_Priority_High
	 \param[in] iStackTracePriority
	 Priority of the stack trace, used for log filtering. One of \ref kAAX_Trace_Priority_Low, \ref kAAX_Trace_Priority_Normal, \ref kAAX_Trace_Priority_High
	 \param[in] iMessage
	 Message string to log
	 */
	virtual AAX_Result StackTrace ( int32_t iTracePriority, int32_t iStackTracePriority, const char * iMessage ) const = 0;
};

#endif // #ifndef AAX_IHOSTSERVICES_H
