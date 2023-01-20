/*================================================================================================*/
/*
 *	Copyright 2013-2016, 2019 by Avid Technology, Inc.
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
#include "AAX_IViewContainer.h"
#include "AAX_CAutoreleasePool.h"

#if WINDOWS_VERSION
#include <windows.h>
#include "lib/platform/win32/win32frame.h"
#endif
#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#endif


#if WINDOWS

void* hInstance = NULL;
// *******************************************************************************
// ROUTINE:	DllMain
// *******************************************************************************
extern "C" BOOL WINAPI DllMain ( HINSTANCE iInstance, DWORD iSelector, LPVOID iReserved )
{
	if ( iSelector == DLL_PROCESS_ATTACH )
	{
		hInstance = (HINSTANCE) iInstance;
	}
	return true;
}
#endif

// *******************************************************************************
// METHOD:	VSTGUI_ContentView::VSTGUI_ContentView
// *******************************************************************************
VSTGUI_ContentView::VSTGUI_ContentView(void *pSystemWindow, AAX_IEffectParameters* pParams, VSTGUI::VSTGUIEditorInterface *pEditor)
	: VSTGUI::CFrame(VSTGUI::CRect (0,0,0,0), pEditor)
	, mEffectParameters(pParams)
{
	this->open(pSystemWindow, VSTGUI::kNSView);
}

// *******************************************************************************
// METHOD:	VSTGUI_ContentView::~VSTGUI_ContentView
// *******************************************************************************
VSTGUI_ContentView::~VSTGUI_ContentView()
{
}

// *******************************************************************************
// METHOD:	VSTGUI_ContentView::setPosition
// *******************************************************************************
bool VSTGUI_ContentView::setPosition (VSTGUI::CCoord x, VSTGUI::CCoord y)
{
#ifdef WINDOWS
	HWND hwnd = (HWND)getFrame ()->getPlatformFrame ()->getPlatformRepresentation ();
	::SetWindowPos (hwnd, HWND_TOP, (int)x, (int)y, 0, 0, SWP_NOSIZE);
	return true;
#else
	return CFrame::setPosition(x, y);
#endif
}

// *******************************************************************************
// METHOD:	VSTGUI_ContentView::setSize
// *******************************************************************************
bool VSTGUI_ContentView::setSize (CCoord width, CCoord height)
{
	bool result = CFrame::setSize(width, height);

#if WINDOWS
	VSTGUI::CRect viewSize(0, 0, width, height);
	CViewContainer::setViewSize(viewSize);
#endif

	return result; 
}

// *******************************************************************************
// METHOD:	VSTGUI_ContentView::getSize
// *******************************************************************************
bool VSTGUI_ContentView::getSize (VSTGUI::CRect& pRect) const
{
	return this->getSize(&pRect);
}

// *******************************************************************************
// METHOD:	VSTGUI_ContentView::getSize
// *******************************************************************************
bool VSTGUI_ContentView::getSize (CRect* pRect) const
{
	if (!isVisible ())
		return false;

#if WINDOWS
	HWND wnd = (HWND)getFrame ()->getPlatformFrame ()->getPlatformRepresentation ();
	HWND wnAAXrent = GetParent (wnd);

	RECT  rctWnd;
	GetWindowRect (wnd, &rctWnd);
	
	POINT point;
	point.x = rctWnd.left;
	point.y = rctWnd.top;

	MapWindowPoints (HWND_DESKTOP, wnAAXrent, &point, 1);

	pRect->left   = (CCoord) point.x;
	pRect->top    = (CCoord) point.y;
	pRect->right  = (CCoord) pRect->left + rctWnd.right - rctWnd.left;
	pRect->bottom = (CCoord) pRect->top  + rctWnd.bottom - rctWnd.top;
	return true;
#else
	return VSTGUI::CFrame::getSize(pRect);
#endif
}

// *******************************************************************************
// METHOD:	VSTGUI_ContentView::SetParameters
// *******************************************************************************
void VSTGUI_ContentView::setParameters(AAX_IEffectParameters *inParameters)
{
	mEffectParameters = inParameters;
}

// *******************************************************************************
// METHOD:	VSTGUI_ContentView::GetParameters
// *******************************************************************************
AAX_IEffectParameters* VSTGUI_ContentView::getParameters()
{
	return mEffectParameters;
}

// *******************************************************************************
// METHOD:	VSTGUI_ContentView::onDrop
// *******************************************************************************
bool VSTGUI_ContentView::onDrop (TDragContainer* drag, const VSTGUI::CPoint& where)
{
	VSTGUI::CRect viewSize(0, 0, 0, 0);
	this->getSize(viewSize);
	
	VSTGUI::CPoint where1 (where);
#if MAC_CARBON
   	VSTGUI::CPoint topLeft (viewSize.getTopLeft());
	where1.offset (-topLeft.x, -topLeft.y);
#endif

	return VSTGUI::CFrame::onDrop(drag, where1);
}

// *******************************************************************************
// METHOD:	VSTGUI_ContentView::onDragEnter
// *******************************************************************************
void VSTGUI_ContentView::onDragEnter (TDragContainer* drag, const VSTGUI::CPoint& where)
{
	VSTGUI::CRect viewSize(0, 0, 0, 0);
	this->getSize(viewSize);
	
	VSTGUI::CPoint where1 (where);
#if MAC_CARBON
   	VSTGUI::CPoint topLeft (viewSize.getTopLeft());
	where1.offset (-topLeft.x, -topLeft.y);
#endif
	
	VSTGUI::CFrame::onDragEnter(drag, where1);
}

// *******************************************************************************
// METHOD:	VSTGUI_ContentView::onDragMove
// *******************************************************************************
void VSTGUI_ContentView::onDragMove (TDragContainer* drag, const VSTGUI::CPoint& where)
{
	VSTGUI::CRect viewSize(0, 0, 0, 0);
	this->getSize(viewSize);
	
	VSTGUI::CPoint where1 (where);
#if MAC_CARBON
   	VSTGUI::CPoint topLeft (viewSize.getTopLeft());
	where1.offset (-topLeft.x, -topLeft.y);
#endif
	
	VSTGUI::CFrame::onDragMove(drag, where1);
}

// *******************************************************************************
// METHOD:	VSTGUI_ContentView::onDragLeave
// *******************************************************************************
void VSTGUI_ContentView::onDragLeave (TDragContainer* drag, const VSTGUI::CPoint& where)
{
	VSTGUI::CRect viewSize(0, 0, 0, 0);
	this->getSize(viewSize);
	
	VSTGUI::CPoint where1 (where);
#if MAC_CARBON
   	VSTGUI::CPoint topLeft (viewSize.getTopLeft());
	where1.offset (-topLeft.x, -topLeft.y);
#endif
	
	VSTGUI::CFrame::onDragLeave(drag, where1);
}

#if WINDOWS_VERSION
// *******************************************************************************
// needed for platformOnKeyDown
// *******************************************************************************
unsigned char translateVirtualKeyWin (WPARAM winVKey)
{
	switch (winVKey)
	{
		case VKEY_BACK: return VK_BACK;
		case VKEY_TAB: return VK_TAB;
		case VKEY_CLEAR: return VK_CLEAR;
		case VKEY_RETURN: return VK_RETURN;
		case VKEY_PAUSE: return VK_PAUSE;
		case VKEY_ESCAPE: return VK_ESCAPE;
		case VKEY_SPACE: return VK_SPACE;
		case VKEY_END: return VK_END;
		case VKEY_HOME: return VK_HOME;
		case VKEY_LEFT: return VK_LEFT;
		case VKEY_RIGHT: return VK_RIGHT;
		case VKEY_UP: return VK_UP;
		case VKEY_DOWN: return VK_DOWN;
		case VKEY_PAGEUP: return VK_PRIOR;
		case VKEY_PAGEDOWN: return VK_NEXT;
		case VKEY_SELECT: return VK_SELECT;
		case VKEY_PRINT: return VK_PRINT;
		case VKEY_SNAPSHOT: return VK_SNAPSHOT;
		case VKEY_INSERT: return VK_INSERT;
		case VKEY_DELETE: return VK_DELETE;
		case VKEY_HELP: return VK_HELP;
		case VKEY_NUMPAD0: return VK_NUMPAD0;
		case VKEY_NUMPAD1: return VK_NUMPAD1;
		case VKEY_NUMPAD2: return VK_NUMPAD2;
		case VKEY_NUMPAD3: return VK_NUMPAD3;
		case VKEY_NUMPAD4: return VK_NUMPAD4;
		case VKEY_NUMPAD5: return VK_NUMPAD5;
		case VKEY_NUMPAD6: return VK_NUMPAD6;
		case VKEY_NUMPAD7: return VK_NUMPAD7;
		case VKEY_NUMPAD8: return VK_NUMPAD8;
		case VKEY_NUMPAD9: return VK_NUMPAD9;
		case VKEY_MULTIPLY: return VK_MULTIPLY;
		case VKEY_ADD: return VK_ADD;
		case VKEY_SEPARATOR: return VK_SEPARATOR;
		case VKEY_SUBTRACT: return VK_SUBTRACT;
		case VKEY_DECIMAL: return VK_DECIMAL;
		case VKEY_DIVIDE: return VK_DIVIDE;
		case VKEY_F1: return VK_F1;
		case VKEY_F2: return VK_F2;
		case VKEY_F3: return VK_F3;
		case VKEY_F4: return VK_F4;
		case VKEY_F5: return VK_F5;
		case VKEY_F6: return VK_F6;
		case VKEY_F7: return VK_F7;
		case VKEY_F8: return VK_F8;
		case VKEY_F9: return VK_F9;
		case VKEY_F10: return VK_F10;
		case VKEY_F11: return VK_F11;
		case VKEY_F12: return VK_F12;
		case VKEY_NUMLOCK: return VK_NUMLOCK;
		case VKEY_SCROLL: return VK_SCROLL;
		case VKEY_SHIFT: return VK_SHIFT;
		case VKEY_CONTROL: return VK_CONTROL;
		case VKEY_ALT: return VK_MENU;
		case VKEY_EQUALS: return VKEY_EQUALS;
	}
	return 0;
}
#endif

// *******************************************************************************
// OVERRIDE of CFrame::platformOnKeyDown
// fix for PTSW-186182
// emulation of juce-like behavior
// *******************************************************************************
bool VSTGUI_ContentView::platformOnKeyDown(VstKeyCode& keyCode)
{
#if WINDOWS_VERSION
	CBaseObjectGuard bog (this);
	int result = onKeyDown (keyCode);

	if (result != 1)
	{
		Win32Frame* win32frame = reinterpret_cast<Win32Frame*>(this->getPlatformFrame());
		HWND parentH = win32frame->getParentPlatformWindow();

		if (parentH != 0)
		{
			UINT   message = WM_KEYDOWN;
			WPARAM wParam  = translateVirtualKeyWin(keyCode.virt);
			LPARAM lParam  = NULL;
			result = (PostMessage (parentH, message, wParam, lParam) != 0);
		}
	}

	return 0 != result;
#else
	return CFrame::platformOnKeyDown(keyCode);
#endif
}

#pragma mark -
// *******************************************************************************
// METHOD:	AAX_CEffectGUI_VST::AAX_CEffectGUI_VST
// *******************************************************************************
AAX_CEffectGUI_VST::AAX_CEffectGUI_VST()
{
	mViewComponent = NULL;
}

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_VST::~AAX_CEffectGUI_VST
// *******************************************************************************
AAX_CEffectGUI_VST::~AAX_CEffectGUI_VST()
{
	AAX_CAutoreleasePool autorelease;
	
	if (mViewComponent)
	{
		mViewComponent->forget();
		mViewComponent = NULL;
	}
}

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_VST::GetViewSize
// *******************************************************************************
AAX_Result AAX_CEffectGUI_VST::GetViewSize ( AAX_Point *oEffectViewSize ) const
{
	if (mViewComponent)
	{
		VSTGUI::CRect viewSize(0,0,0,0);
		mViewComponent->getViewSize(viewSize);

		oEffectViewSize->horz = (float) viewSize.getWidth();
		oEffectViewSize->vert = (float) viewSize.getHeight();
	}
	
	return AAX_SUCCESS;	
}

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_VST::Draw
// *******************************************************************************
AAX_Result AAX_CEffectGUI_VST::Draw ( AAX_Rect * iDrawRect )
{
	if ( mViewComponent && iDrawRect)
	{
		VSTGUI::CRect invRect( VSTGUI::CPoint( (VSTGUI::CCoord) iDrawRect->left, (VSTGUI::CCoord) iDrawRect->top),
							  VSTGUI::CPoint( (VSTGUI::CCoord) iDrawRect->width, (VSTGUI::CCoord) iDrawRect->height) );
		mViewComponent->drawRect(NULL, invRect);
	}
	
	return AAX_SUCCESS;	
}

#if WINDOWS

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_VST::CreateViewContainer
// *******************************************************************************
void AAX_CEffectGUI_VST::CreateViewContainer ()
{
	if ( this->GetViewContainerType () == AAX_eViewContainer_Type_HWND )
	{
		this->CreateEffectView ( this->GetViewContainerPtr () );
	}
}

#endif	// WINDOWS


// *******************************************************************************
// ROUTINE:	AAX_CEffectGUI_VST::DeleteViewContainer
// *******************************************************************************
void AAX_CEffectGUI_VST::DeleteViewContainer ()
{
	AAX_CAutoreleasePool autorelease;

	if (mViewComponent)
	{
		mViewComponent->forget();
		mViewComponent = NULL;
	}
}

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_VST::CreateVSTFrame
// *******************************************************************************
void AAX_CEffectGUI_VST::CreateEffectView (void *inSystemWindow)
{
	mViewComponent = new VSTGUI_ContentView(inSystemWindow, this->GetEffectParameters(), this);
	if (mViewComponent)
	{
		mViewComponent->setBackgroundColor(VSTGUI::kGreyCColor);
	}
}



//==============================================================================
// CAAXButtonState
//==============================================================================

static uint32_t SynthesizeModifiers(const VSTGUI::CButtonState& inButtonState, AAX_IViewContainer* inViewContainer)
{
	uint32_t aax_mods = 0;

	if (inButtonState & VSTGUI::kAlt) { aax_mods |= AAX_eModifiers_Option; }
	if (inButtonState & VSTGUI::kApple) { aax_mods |= AAX_eModifiers_Control; } // VSTGUI modifier definitions are flipped on Mac.
	if (inButtonState & VSTGUI::kControl) { aax_mods |= AAX_eModifiers_Command; } // VSTGUI modifier definitions are flipped on Mac.
	if (inButtonState & VSTGUI::kShift) { aax_mods |= AAX_eModifiers_Shift; }
	if (inButtonState & VSTGUI::kRButton) { aax_mods |= AAX_eModifiers_SecondaryButton; }

	// It is best practice to always query the host as
	// well, since the host's key handler may have
	// prevented some modifier key states from reaching
	// the plug-in.
	if (inViewContainer)
	{
		uint32_t aaxViewMods = 0;
		inViewContainer->GetModifiers (&aaxViewMods);
		aax_mods |= aaxViewMods;
	}

	return aax_mods;
}

static VSTGUI::CButtonState SynthesizeButtonState(const VSTGUI::CButtonState& inButtonState, uint32_t inModifiers)
{
	VSTGUI::CButtonState buttons(inButtonState);

	if (AAX_eModifiers_Shift & inModifiers) { buttons |= VSTGUI::kShift; }
	if (AAX_eModifiers_Control & inModifiers) { buttons |= VSTGUI::kApple; } // VSTGUI modifier definitions are flipped on Mac.
	if (AAX_eModifiers_Option & inModifiers) { buttons |= VSTGUI::kAlt; }
	if (AAX_eModifiers_Command & inModifiers) { buttons |= VSTGUI::kControl; } // VSTGUI modifier definitions are flipped on Mac.
	if (AAX_eModifiers_SecondaryButton & inModifiers) { buttons |= VSTGUI::kRButton; }

	return buttons;
}


AAX_CVSTGUIButtonState::AAX_CVSTGUIButtonState(const VSTGUI::CButtonState& inButtonState, AAX_IViewContainer* inViewContainer)
: mModifiers(SynthesizeModifiers(inButtonState, inViewContainer))
, mButtonState(SynthesizeButtonState(inButtonState, mModifiers))
{
}
