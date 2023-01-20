/*================================================================================================*/
/*
 *	Copyright 2013-2017 by Avid Technology, Inc.
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

#ifdef WINDOWS_VERSION
#include "AAX_CEffectGUI_Win32.h"
#endif

#if JUCE_WINDOWS
	#include <windows.h>
#endif

#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#endif

#ifdef WINDOWS_VERSION

static HINSTANCE sInstance = 0;

// *******************************************************************************
// ROUTINE:	DllMain
// *******************************************************************************
extern "C" BOOL WINAPI DllMain ( HINSTANCE iInstance, DWORD iSelector, LPVOID iReserved )
{
	try
	{
		if ( iSelector == DLL_PROCESS_ATTACH )
		{
			sInstance = iInstance;
		}	
	}
	catch(...)
	{
		return false;
	}

	return true;
}

#endif

// *******************************************************************************
// METHOD:	AAX_JuceContentView::AAX_JuceContentView
// *******************************************************************************
AAX_JuceContentView::AAX_JuceContentView ( const juce::Colour & inBackColour, AAX_CEffectGUI * inEffectGUI )
	: juce::Component()
{
	this->SetEffectGUI ( inEffectGUI );
	mViewContainer = 0;
	
	setBackgroundColour (inBackColour);
	mBackgroundImage = 0;
}

// *******************************************************************************
// METHOD:	AAX_JuceContentView::~AAX_JuceContentView
// *******************************************************************************
AAX_JuceContentView::~AAX_JuceContentView()
{
}

// *******************************************************************************
// METHOD:	AAX_JuceContentView::getBackgroundColour
// *******************************************************************************
const juce::Colour AAX_JuceContentView::getBackgroundColour() const
{
	return findColour (backgroundColourId, false);
}

// *******************************************************************************
// METHOD:	AAX_JuceContentView::setBackgroundColour
// *******************************************************************************
void AAX_JuceContentView::setBackgroundColour (const juce::Colour& newColour)
{
	this->setColour (backgroundColourId, newColour);
}

// *******************************************************************************
// METHOD:	AAX_JuceContentView::setBackgroundColour
// *******************************************************************************
void AAX_JuceContentView::setBackgroundImage( juce::Image * inImage, bool inResize )
{
	mBackgroundImage = inImage;
	if ( mBackgroundImage )
	{
		if ( inResize )
			this->setSize ( mBackgroundImage->getWidth (), mBackgroundImage->getHeight () );
	}
}

// *******************************************************************************
// METHOD:	AAX_JuceContentView::SetParameters
// *******************************************************************************
//void AAX_JuceContentView::SetParameters(AAX_IEffectParameters *inParameters)
//{
//	mEffectParameters = inParameters;
//}

// *******************************************************************************
// METHOD:	AAX_JuceContentView::SetController
// *******************************************************************************
//void AAX_JuceContentView::SetController(AAX_IControlle *inController)
//{
//	mController = inController;
//}

// *******************************************************************************
// METHOD:	AAX_JuceContentView::GetEffectGUI
// *******************************************************************************
void AAX_JuceContentView::SetEffectGUI ( AAX_CEffectGUI * inEffectGUI )
{
	mEffectGUI = inEffectGUI;
	if ( mEffectGUI )
	{
		mController = mEffectGUI->GetController ();
		mEffectParameters = mEffectGUI->GetEffectParameters ();
	}
	else 
	{
		mController = 0;
		mEffectParameters = 0;
	}
}

// *******************************************************************************
// METHOD:	AAX_JuceContentView::GetEffectGUI
// *******************************************************************************
AAX_CEffectGUI* AAX_JuceContentView::GetEffectGUI()
{
	return mEffectGUI;
}


// *******************************************************************************
// METHOD:	AAX_JuceContentView::GetParameters
// *******************************************************************************
AAX_IEffectParameters* AAX_JuceContentView::GetParameters()
{
	return mEffectParameters;
}


// *******************************************************************************
// METHOD:	AAX_JuceContentView::GetController
// *******************************************************************************
AAX_IController*	AAX_JuceContentView::GetController()
{
	return mController;
}


// *******************************************************************************
// METHOD:	AAX_JuceContentView::GetViewContainer
// *******************************************************************************
AAX_IViewContainer * AAX_JuceContentView::GetViewContainer()
{
	return mViewContainer;
}


// *******************************************************************************
// METHOD:	AAX_JuceContentView::paint
// *******************************************************************************
void AAX_JuceContentView::paint ( juce::Graphics & inGraphics )
{
	const int width = this->getWidth();
	const int height = this->getHeight();
	if (0 < width && 0 < height)
	{
		if ( mBackgroundImage )
		{
			const int bgWidth = mBackgroundImage->getWidth();
			const int bgHeight = mBackgroundImage->getHeight();
			if (0 < bgWidth && 0 < bgHeight)
			{
				inGraphics.drawImage ( *mBackgroundImage, 0, 0, width, height, 0, 0, mBackgroundImage->getWidth (), mBackgroundImage->getHeight () );
			}
		}
		else
		{
			inGraphics.fillAll(this->getBackgroundColour());
		}
	}
}

// *******************************************************************************
// METHOD: mouseMove
// *******************************************************************************
void AAX_JuceContentView::mouseMove ( const juce::MouseEvent & inMouseEvent )
{
	// Force the cursor to be drawn when we enter the plug-in.
#if JUCE_MAJOR_VERSION < 3
	inMouseEvent.source.forceMouseCursorUpdate();
#else
	const_cast<juce::MouseInputSource*>(&inMouseEvent.source)->forceMouseCursorUpdate();
#endif
}

#pragma mark -

juce::TooltipWindow * AAX_CEffectGUI_Juce::sTooltipWindow = NULL;

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_Juce::AAX_CEffectGUI_Juce
// *******************************************************************************
AAX_CEffectGUI_Juce::AAX_CEffectGUI_Juce()
{
	mViewComponent = 0;
	
#if WINDOWS_VERSION
	juce::PlatformUtilities::setCurrentModuleInstanceHandle ( sInstance );
#endif
	juce::initialiseJuce_GUI();

	if (sTooltipWindow == NULL)
		sTooltipWindow = new juce::TooltipWindow( 0, 2000 );
}

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_Juce::~AAX_CEffectGUI_Juce
// *******************************************************************************
AAX_CEffectGUI_Juce::~AAX_CEffectGUI_Juce()
{
	JUCE_AUTORELEASEPOOL
	
	if ( mViewComponent )
		deleteAndZero( mViewComponent );
	
	// about to shutdown juce
	if (sTooltipWindow && AAX_JuceCount::sJuceUseCount == 1)
	{
		deleteAndZero( sTooltipWindow );
		sTooltipWindow = NULL;
	}
}

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_Juce::GetViewSize
// *******************************************************************************
AAX_Result AAX_CEffectGUI_Juce::GetViewSize ( AAX_Point *oEffectViewSize ) const
{
	if ( mViewComponent )
	{
		const juce::Rectangle<int>& viewRect = mViewComponent->getBounds();
		oEffectViewSize->horz = (float) viewRect.getWidth();
		oEffectViewSize->vert = (float) viewRect.getHeight();		
		return AAX_SUCCESS;
	}
	
	return AAX_ERROR_NULL_OBJECT;
}

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_Juce::TimerWakeup
// *******************************************************************************
AAX_Result AAX_CEffectGUI_Juce::TimerWakeup ()
{ 
	if ( mViewComponent ) 
		mViewComponent->TimerWakeup (); 

	return AAX_SUCCESS; 
}

#if JUCE_WINDOWS

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_Juce::CreateViewContainer
// *******************************************************************************
void AAX_CEffectGUI_Juce::CreateViewContainer ()
{
	HWND nativeWindowToAttachTo = NULL;
	
	if ( this->GetViewContainerType () == AAX_eViewContainer_Type_HWND )
	{
		nativeWindowToAttachTo = (HWND) this->GetViewContainerPtr ();
	}
	
	if (nativeWindowToAttachTo && mViewComponent)
	{
		mViewComponent->SetViewContainer ( this->GetViewContainer () );
		mViewComponent->addToDesktop( 0 );

		HWND	plugWnd = (HWND) mViewComponent->getWindowHandle();

		DWORD val = ::GetWindowLong (plugWnd, GWL_STYLE);
		val &= ~WS_POPUP;
		val |= WS_CHILD;
		::SetWindowLong (plugWnd, GWL_STYLE, val);
		::SetParent( plugWnd, nativeWindowToAttachTo );
	}
}

// *******************************************************************************
// ROUTINE:	AAX_CEffectGUI_Juce::DeleteViewContainer
// *******************************************************************************
void AAX_CEffectGUI_Juce::DeleteViewContainer ()
{
	if (mViewComponent)
	{
		mViewComponent->SetViewContainer ( 0 );
		mViewComponent->removeFromDesktop ();
	}
}

#endif	// JUCE_WINDOWS

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_Juce::CreateViewContents
// *******************************************************************************
void AAX_CEffectGUI_Juce::CreateViewContents ()
{	
	mViewComponent = new AAX_JuceContentView ( juce::Colours::lightgrey, this );
}

// *******************************************************************************
// *******************************************************************************
// CLASS:	AAX_JuceCount
// *******************************************************************************
// *******************************************************************************

int32_t AAX_JuceCount::sJuceUseCount = 0;

// *******************************************************************************
// METHOD:	AAX_JuceCount
// *******************************************************************************
AAX_JuceCount::AAX_JuceCount()
{
	AAX_JuceCount::IncrementJuceUseCout ();
}

// *******************************************************************************
// METHOD:	~AAX_JuceCount
// *******************************************************************************
AAX_JuceCount::~AAX_JuceCount()
{
	JUCE_AUTORELEASEPOOL
	
	if ( AAX_JuceCount::DecrementJuceUseCout () )
	{
		juce::shutdownJuce_GUI();
	}
}

// *******************************************************************************
// ROUTINE:	IncrementJuceUseCout
// *******************************************************************************
bool AAX_JuceCount::IncrementJuceUseCout ()
{
	bool	result = (sJuceUseCount++ == 0);
	return result;
}

// *******************************************************************************
// ROUTINE:	DecrementJuceUseCout
// *******************************************************************************
bool AAX_JuceCount::DecrementJuceUseCout ()
{
	bool	result = (--sJuceUseCount == 0);
	return result;
}
