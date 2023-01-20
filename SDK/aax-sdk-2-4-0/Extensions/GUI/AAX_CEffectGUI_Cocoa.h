/*================================================================================================*/
/*
 *	Copyright 2010-2015, 2019 by Avid Technology, Inc.
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
 *  \file AAX_CEffectGUI_Cocoa.h
 *
 */
/*================================================================================================*/

#ifndef AAX_CEFFECTGUI_COCOA_H
#define AAX_CEFFECTGUI_COCOA_H

#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#else
class NSWindow;
class NSViewController;
class NSView;
#endif

#include "AAX_CEffectGUI.h"

#include <string>
#include <vector>

class AAX_CEffectGUI_Cocoa : public AAX_CEffectGUI
{
public:
	AAX_CEffectGUI_Cocoa ();
	~AAX_CEffectGUI_Cocoa () AAX_OVERRIDE;

protected:
	AAX_Result	GetViewSize ( AAX_Point * oEffectViewSize ) const AAX_OVERRIDE;
	void		CreateViewContainer () AAX_OVERRIDE;
	void		DeleteViewContainer () AAX_OVERRIDE;

	void		SetViewController ( NSViewController * iViewController ) { mViewController = iViewController; }
	void		AddViewContents ();

protected:
	NSView *			mNSView;	
	NSViewController *	mViewController;
};

#endif //AAX_CEFFECTGUI_COCOA_H
