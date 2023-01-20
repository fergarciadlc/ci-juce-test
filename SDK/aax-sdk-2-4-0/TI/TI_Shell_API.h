/*================================================================================================*/
/*
 *	Copyright 2010-2015 by Avid Technology, Inc.
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
 *	\file   TI_Shell_API.h
 *
 *  \brief  DLL import / export interface defines
 *
 *  \author Paul Vercellotti
 */ 
/*================================================================================================*/
#ifndef TI_SHELL_API_H
#define TI_SHELL_API_H

#ifdef  _TMS320C6X	// TI-only

#include <elf_linkage.h>

#if TISHELL

#define TI_SHELL_API far __declspec(dllexport)

#else

#define TI_SHELL_API far __declspec(dllimport)

#endif // TI_SHELL_BUILD

#endif // _TMS320C6X

#endif

