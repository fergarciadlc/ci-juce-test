/*================================================================================================*/
/*
 *	Copyright 2013-2015 by Avid Technology, Inc.
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

#include "AAX_CEffectGUI_Cocoa.h"
#include "AAX_IParameter.h"

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_Cocoa
// *******************************************************************************
AAX_CEffectGUI_Cocoa::AAX_CEffectGUI_Cocoa()
:   mNSView(0),
    mViewController(0)
{
}

// *******************************************************************************
// METHOD:	~AAX_CEffectGUI_Cocoa
// *******************************************************************************
AAX_CEffectGUI_Cocoa::~AAX_CEffectGUI_Cocoa()
{
	this->DeleteViewContainer ();
}

// *******************************************************************************
// METHOD:	CreateViewContainer
// *******************************************************************************
void AAX_CEffectGUI_Cocoa::CreateViewContainer ()
{
	if ( this->GetViewContainerType ()  == AAX_eViewContainer_Type_NSView )
	{
		mNSView = (NSView *) this->GetViewContainerPtr ();
		this->AddViewContents ();
	}
}

// *******************************************************************************
// ROUTINE:	AddViewContents
// *******************************************************************************
void AAX_CEffectGUI_Cocoa::AddViewContents ()
{	
	if ( mNSView && mViewController )
	{
		if ( mViewController && mViewController.view )
			[mNSView addSubview:mViewController.view];
	}
}

// *******************************************************************************
// ROUTINE:	DeleteViewContainer
// *******************************************************************************
void AAX_CEffectGUI_Cocoa::DeleteViewContainer ()
{	
	if ( mNSView && mViewController && mViewController.view )
	{
		[mViewController.view removeFromSuperview];
		[mViewController release];
		mViewController = 0;
	}
}	

// *******************************************************************************
// METHOD:	GetViewSize
// *******************************************************************************
AAX_Result AAX_CEffectGUI_Cocoa::GetViewSize ( AAX_Point *oEffectViewSize ) const
{
	if ( mViewController && mViewController.view )
	{
		oEffectViewSize->horz = mViewController.view.frame.size.width;
		oEffectViewSize->vert = mViewController.view.frame.size.height;
	}

	return AAX_SUCCESS;
}
