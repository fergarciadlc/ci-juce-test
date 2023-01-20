/***********************************************************************

	The software and files contained herein are furnished under a 
	license agreement. The software: (1) is for internal use only; 
	(2) may not be redistributed; (3) may not be reverse assembled; 
	and (4) may be used or copied only in accordance with the terms 
	of the license agreement.  

	Copyright (c) 2004 Avid Technology, Inc. All rights reserved.

************************************************************************/

#ifndef initacfuid_h
#define initacfuid_h

#include "acfbasetypes.h"

/*!
    \file
	\brief Include initacfuid.h to enable acfUID initialization.  This must be done 
    once per exe/dll.  After this file, include one or more of the acfUID definition files.    
*/

#ifdef DEFINE_ACFUID
#undef DEFINE_ACFUID
#endif

/*!
	Defines a uid structure of type \ref acfUID and initializes member data.
	Generally you will want to use this macro to define all uid's within a definition.
	\warning The uid must be unique. \n
	\e Example: \n 
	\verbatim DEFINE_ACFUID(acfUID, ACF_MyEffectUID, 0x51b30d8a, 0xf54, 0x4092, 0xba, 0x41, 0x46, 0xdc, 0x91, 0xee, 0xf7, 0xf4); \endverbatim
 */
#define DEFINE_ACFUID(type, name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    ACFEXTERN_C const type name = { l, w1, w2, {b1, b2,  b3,  b4,  b5,  b6,  b7,  b8} }

#endif // initacfuid_h
