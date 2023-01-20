/***********************************************************************

	The software and files contained herein are furnished under a 
	license agreement. The software: (1) is for internal use only; 
	(2) may not be redistributed; (3) may not be reverse assembled; 
	and (4) may be used or copied only in accordance with the terms 
	of the license agreement.  

	Copyright (c) 2004 Avid Technology, Inc. All rights reserved.

************************************************************************/




/*!
 \file acfassert.h
	
 \brief Wrapper for AVX2 assertions. 
 \remarks The default definition uses
 ansi assert so it does not require c++ exceptions to be
 enabled.
 This also adds guards to assert.h so that it can safely
 be included in a header file (CW bug).
*/
#ifndef ACFASSERT

#ifndef assert

#if defined(__MACH__) && !defined(__GNUC__)
// This configuration for /usr/include/gcc/darwin/2.5.2/assert.h uses
// prinf and abort WITHOUT including the definitions. transdel-2001-SEPT-21.
#include <stdio.h>    // printf()
#include <stdlib.h>   // abort()
#endif

#include "assert.h"
#endif

#define ACFASSERT assert

#endif
