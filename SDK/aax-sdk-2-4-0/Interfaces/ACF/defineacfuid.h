/***********************************************************************

	The software and files contained herein are furnished under a 
	license agreement. The software: (1) is for internal use only; 
	(2) may not be redistributed; (3) may not be reverse assembled; 
	and (4) may be used or copied only in accordance with the terms 
	of the license agreement.  

	Copyright (c) 2004 Avid Technology, Inc. All rights reserved.

************************************************************************/


#ifndef defineacfuid_h
#define defineacfuid_h

/*!
    \file defineacfuid.h
	\brief Defines DEFINE_ACFUID.

    \remarks This header provides the definition for the DEFINE_ACFUID 
	macro that is used to either declare a acfUID as a forward declaration
	or initializes, defines, the acfUID symbol.
 */

#include "acfbasetypes.h"


/*!
	\def DEFINE_ACFUID(type, name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)
	\brief Defines a type of acfUID structure.

	This macro is used to both declare and define acfUID constants. If the
	symbol INITACFIDS is defined then the constant is fully defined, otherwise the
	constant is just declared.

	\note To avoid duplicate symbol definitions the symbol INITACFIDS should only
	be defined in one source file of an executable module.
*/
#ifndef DEFINE_ACFUID
#define DEFINE_ACFUID(type, name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	ACFEXTERN_C const type name
#endif // DEFINE_ACFUID


#if defined(INITACFIDS) || defined(INITAVXIDS)
#include "initacfuid.h"
#endif

#endif // defineacfuid_h

