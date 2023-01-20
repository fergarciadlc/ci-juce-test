/*================================================================================================*/
/*
 *
 *	Copyright 2014-2017, 2020-2021 by Avid Technology, Inc.
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
 *	\file  AAX_IEffectGUI.h
 *
 *	\brief The interface for a %AAX Plug-in's user interface
 *
 */ 
/*================================================================================================*/


#ifndef AAX_IEFFECTGUI_H
#define AAX_IEFFECTGUI_H

#include "AAX_IACFEffectGUI.h"
#include "AAX.h"
#include "CACFUnknown.h"


/** @brief	The interface for a %AAX Plug-in's user interface.
	
	@details
	@pluginimp

	This is the interface for an instance of a plug-in's GUI that gets exposed to 
	the host application.  You need to inherit from this interface and override all 
	of the virtual functions to create a plug-in GUI. 

	To create the GUI for an %AAX plug-in it is required that you inherit from 
	this interface and override all of the virtual functions from 
	\ref AAX_IACFEffectGUI.  In nearly all cases you will be able to take advantage of 
	the implementations in the %AAX library's \ref AAX_CEffectGUI class and only override the 
	few specific methods that you want to explicitly customize.
	
	\note This class always inherits from the latest version of the interface and thus requires any 
	subclass to implement all the methods in the latest version of the interface.
	
	\ingroup CommonInterface_GUI
*/
class AAX_IEffectGUI :	public AAX_IACFEffectGUI,
						public CACFUnknown
{	
public:
	ACF_DECLARE_STANDARD_UNKNOWN()
	
	ACFMETHOD(InternalQueryInterface)(const acfIID & riid, void **ppvObjOut) override;
	
	// CACFUnknown does not support operator=()
	AAX_DELETE(AAX_IEffectGUI& operator= (const AAX_IEffectGUI&));
};

#endif //AAX_IEFFECTGUI_H
