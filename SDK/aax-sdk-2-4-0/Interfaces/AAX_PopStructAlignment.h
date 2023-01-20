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
 *	\file   AAX_PopStructAlignment.h
 *	
 *	\brief Resets (pops) the struct alignment to its previous value
 *
 *	\details
 *	\sa AAX_ALIGN_HOST
 *	\sa AAX_ALIGN_ALG
 *	\sa AAX_ALIGN_RESET
 *
 *	\note Inclusion of this file is mandatory after any 'push' inclusion.
 *	\note Some compilers do not properly "pop" alignment, so nesting
 *	      push/pop inclusions is not allowed.
 *		
 *	\sa \ref AAX_Push2ByteStructAlignment.h
 *	\sa \ref AAX_Push4ByteStructAlignment.h
 *	\sa \ref AAX_Push8ByteStructAlignment.h
 *		
 *	\internal
 *		NOTE: we don't use include guards for this file because it *is* possible to 
 *		include this file multiple times in the same file.	
 *	\endinternal
 *
 */ 
/*================================================================================================*/

// Nesting of struct alignment headers is not allowed
#ifndef __AAX_CUSTOM_STRUCT_ALIGN_IS_SET__
#error "No AAX struct alignment has been set. Cannot undo."
#else
#undef __AAX_CUSTOM_STRUCT_ALIGN_IS_SET__
#endif

#ifdef _TMS320C6X
// Do nothing for TI
#elif defined (_MSC_VER)
#pragma pack(pop)
#elif defined (__GNUC__)
// Uncomment this warning suppression if you really want to apply packing to a virtual data
// structure, but note that there is no guarantee of cross-platform compatibility for such
// a structure. For more information, see the AAX_ALIGN_FILE_ALG macro documentation
//	#ifdef __clang__
//		#pragma clang diagnostic push
//		#pragma clang diagnostic ignored "-Wno-incompatible-ms-struct"
//	#endif
#pragma ms_struct off
//	#ifdef __clang__
//		#pragma clang diagnostic pop
//	#endif
#pragma pack(pop)
#elif defined (__MWERKS__)
#pragma options align=reset
#else
#error "You need to supply a pragma here to pop structure packing"
#endif
