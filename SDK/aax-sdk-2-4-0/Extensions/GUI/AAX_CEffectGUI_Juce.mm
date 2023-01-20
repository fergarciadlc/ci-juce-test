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

#include "AAX_CEffectGUI_Juce.h"

#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#endif

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_Juce::CreateViewContainer
// *******************************************************************************
void AAX_CEffectGUI_Juce::CreateViewContainer ()
{
	void* nativeViewToAttachTo = NULL;
	
	if ( this->GetViewContainerType () == AAX_eViewContainer_Type_NSView )
	{
		nativeViewToAttachTo = ((NSView *) this->GetViewContainerPtr () );
	}
	
	if ( nativeViewToAttachTo && mViewComponent )
	{
		mViewComponent->SetViewContainer ( this->GetViewContainer () );
		mViewComponent->addToDesktop ( 0, nativeViewToAttachTo );
	}
}

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_Juce::DeleteViewContainer
// *******************************************************************************
void AAX_CEffectGUI_Juce::DeleteViewContainer ()
{
	NSAutoreleasePool* autoreleasePool = [[NSAutoreleasePool alloc] init];
	if (mViewComponent)
	{
		mViewComponent->SetViewContainer ( 0 );
		mViewComponent->removeFromDesktop ();
	}
	
	[autoreleasePool release];
}
