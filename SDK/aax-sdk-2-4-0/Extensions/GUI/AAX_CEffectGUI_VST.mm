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
/*================================================================================================*/
#include "AAX_CEffectGUI_VST.h"

#import <Cocoa/Cocoa.h>

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_VST::CreateViewContainer
// *******************************************************************************
void AAX_CEffectGUI_VST::CreateViewContainer ()
{
	if ( this->GetViewContainerType () == AAX_eViewContainer_Type_NSView )
	{
		NSView * theView = (NSView *) this->GetViewContainerPtr ();
		if ( theView )
		{
#if MAC_COCOA && MAC_CARBON
			CFrame::setCocoaMode ( true );
#endif
			this->CreateEffectView ( theView );
		}
	}
}
