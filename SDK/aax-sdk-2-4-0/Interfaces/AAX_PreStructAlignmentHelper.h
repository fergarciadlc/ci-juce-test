/*================================================================================================*/
/*
 *
 *	Copyright 2018 by Avid Technology, Inc.
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
 *	\file   AAX_PreStructAlignmentHelper.h
 *
 *	\brief Helper file for data alignment macros
 */ 
/*================================================================================================*/

#if defined (AAX_SDK__PRE_STRUCT_ALIGNMENT_HELPER_DID_PUSH_A_CHANGE)
	#warning nested struct alignment directives are not tested
#endif

#if defined (__clang__)
	#if __has_warning ("-Wpragma-pack")
		#define AAX_SDK__PRE_STRUCT_ALIGNMENT_HELPER_DID_PUSH_A_CHANGE 1
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wpragma-pack"
	#endif
#endif
