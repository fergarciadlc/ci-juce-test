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

#include "AAX_CEffectGUI_Win32.h"
#include "AAX_IParameter.h"

HINSTANCE AAX_CEffectGUI_Win32::sInstance = 0;

#define AAX_ORIG_WINDOW_PROC_PROP	"AAX_OrigWinProc_Prop"
#define AAX_EFFECTGUI_PROP			"AAX_EffectGUI_Prop"

// *******************************************************************************
// ROUTINE:	DllMain
// *******************************************************************************
extern "C" BOOL WINAPI DllMain ( HINSTANCE iInstance, DWORD iSelector, LPVOID iReserved )
{
	try
	{
		if ( iSelector == DLL_PROCESS_ATTACH )
		{
			AAX_CEffectGUI_Win32::SetInstance( iInstance );
		}	
	}
	catch(...)
	{
		return false;
	}

	return true;
}

// *******************************************************************************
// METHOD:	AAX_CEffectGUI_Win32
// *******************************************************************************
AAX_CEffectGUI_Win32::AAX_CEffectGUI_Win32()
{
	mHWND = 0;
	mPlugInHWND = 0;
	mLastTextResult = AAX_eEventResult_PassEventToSystem;
}

// *******************************************************************************
// METHOD:	~AAX_CEffectGUI_Win32
// *******************************************************************************
AAX_CEffectGUI_Win32::~AAX_CEffectGUI_Win32()
{
	this->DeleteViewContainer();
}

// *******************************************************************************
// METHOD:	GetViewSize
// *******************************************************************************
AAX_Result AAX_CEffectGUI_Win32::GetViewSize ( AAX_Point* oEffectViewSize ) const
{
	if ( mPlugInHWND )
	{
		RECT	rect;
		GetWindowRect( mPlugInHWND, &rect );
		oEffectViewSize->vert = (float) rect.bottom - rect.top;
		oEffectViewSize->horz = (float) rect.right - rect.left;
	}
	else
	{
		oEffectViewSize->vert = 0;
		oEffectViewSize->horz = 0;
	}

	return AAX_SUCCESS;
}

// *******************************************************************************
// ROUTINE:	CreateViewContents
// *******************************************************************************
void AAX_CEffectGUI_Win32::CreateViewContents ( )
{
	// Create the view contents if the plug-in HWND does not yet exist
	if ( ( NULL != GetParentHWND () ) && ( NULL == GetPlugInHWND () ) )
	{
		const WORD plugInHWNDResourceID = GetPlugInHWNDResourceID ();
		CreatePlugInHWND ( plugInHWNDResourceID );
		InitPlugInHWNDContents ();
	}
}

// *******************************************************************************
// ROUTINE:	CreateViewContainer
// *******************************************************************************
void AAX_CEffectGUI_Win32::CreateViewContainer ( )
{	
	if ( this->GetViewContainerType () == AAX_eViewContainer_Type_HWND )
	{
		// Set the provided HWND as the parent HWND
		SetParentHWND( (HWND) this->GetViewContainerPtr () );
		if ( NULL != GetParentHWND () )
		{
			// Create the view contents if the plug-in HWND does not yet exist
			if ( NULL == GetPlugInHWND () )
			{
				const WORD plugInHWNDResourceID = GetPlugInHWNDResourceID ();
				CreatePlugInHWND ( plugInHWNDResourceID );
				InitPlugInHWNDContents ();
			}
		
			// Set the view contents on the parent
			if ( NULL != GetPlugInHWND () )
			{
				::SetParent( GetPlugInHWND (), GetParentHWND () );
				::ShowWindow ( GetPlugInHWND (), SW_SHOW );
			}
		}
	}
}

// *******************************************************************************
// ROUTINE:	DeleteViewContainer
// *******************************************************************************
void AAX_CEffectGUI_Win32::DeleteViewContainer ()
{	
	if ( mPlugInHWND )
	{
		DestroyWindow( mPlugInHWND );
		mPlugInHWND = 0;
	}
}	

// *******************************************************************************
// METHOD:	DoTextWindowProc
// *******************************************************************************
AAX_EEventResult AAX_CEffectGUI_Win32::DoTextWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	AAX_EEventResult	textResult = AAX_eEventResult_PassEventToSystem;

	if ( hwnd == GetFocus() )
	{
		if ( uMsg == WM_KEYDOWN )
		{
			mLastTextResult = this->HandleKeyDown( hwnd, wParam );
			textResult = mLastTextResult;
			if ( mLastTextResult == AAX_eEventResult_CancelFocus ) 
				this->ForceParameterUpdateForHWND( hwnd );
		}
		else if ( uMsg == WM_CHAR )
		{
			textResult = mLastTextResult;
			mLastTextResult = AAX_eEventResult_PassEventToSystem;

			if ( textResult == AAX_eEventResult_ClearFocus ||
				textResult == AAX_eEventResult_CancelFocus )
			{
				SetFocus( 0 );
			}
		}
//		else if ( uMsg == WM_KEYUP )
//		{
//			if ( wParam == VK_RETURN || wParam == VK_ESCAPE )
//				return true;
//		}
	}

	return textResult;
}

// *******************************************************************************
// METHOD:	TextWindowProc
// *******************************************************************************
LRESULT CALLBACK AAX_CEffectGUI_Win32::TextWindowProc( __in HWND hwnd, __in UINT uMsg, __in WPARAM wParam, __in LPARAM lParam )
{
	AAX_EEventResult textResult = AAX_eEventResult_PassEventToSystem;

	AAX_CEffectGUI_Win32 * effectGUI = (AAX_CEffectGUI_Win32 *) GetProp( hwnd, AAX_EFFECTGUI_PROP );
	if ( effectGUI )
	{
		textResult = effectGUI->DoTextWindowProc( hwnd, uMsg, wParam, lParam );
		if ( textResult == AAX_eEventResult_PassEventToClient )
			SendMessage( effectGUI->GetParentHWND(), uMsg, wParam, lParam );
	}

	if ( textResult == AAX_eEventResult_PassEventToSystem )
	{
		WNDPROC origWindowProc = (WNDPROC) GetProp( hwnd, AAX_ORIG_WINDOW_PROC_PROP );
		if ( origWindowProc )
			return CallWindowProc( origWindowProc, hwnd, uMsg, wParam, lParam );
	}

	return 1;
}

// *******************************************************************************
// METHOD:	SetupForKeyboardInput
// *******************************************************************************
void AAX_CEffectGUI_Win32::SetupForKeyboardInput ( HWND iEditText )
{
	if ( iEditText )
	{
		WNDPROC oldTextWindowProc = (WNDPROC) GetWindowLongPtr( iEditText, GWLP_WNDPROC );
		SetProp( iEditText, AAX_ORIG_WINDOW_PROC_PROP, oldTextWindowProc );
		SetProp( iEditText, AAX_EFFECTGUI_PROP, this );
		SetWindowLongPtr( iEditText, GWLP_WNDPROC, (LONG_PTR) TextWindowProc );
	}
}

// *******************************************************************************
// METHOD:	DoMouseWindowProc
// *******************************************************************************
AAX_EEventResult AAX_CEffectGUI_Win32::DoMouseWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return AAX_eEventResult_PassEventToSystem;
}

// *******************************************************************************
// METHOD:	MouseInputWindowProc
// *******************************************************************************
LRESULT CALLBACK AAX_CEffectGUI_Win32::MouseInputWindowProc( __in HWND hwnd, __in UINT uMsg, __in WPARAM wParam, __in LPARAM lParam )
{
	LRESULT result = 1;
	AAX_EEventResult mouseResult = AAX_eEventResult_PassEventToSystem;

	AAX_CEffectGUI_Win32 * effectGUI = (AAX_CEffectGUI_Win32 *) GetProp( hwnd, AAX_EFFECTGUI_PROP );
	if ( effectGUI )
	{
		switch (uMsg)
		{
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_LBUTTONDOWN:
			case WM_MOUSEMOVE:
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
#if (_WIN32_WINNT >= 0x0500)
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
#endif
			{
				mouseResult = effectGUI->DoMouseWindowProc( hwnd, uMsg, wParam, lParam );
				break;
			}
		}

		if (mouseResult == AAX_eEventResult_PassEventToClient)
		{
			SendMessage( effectGUI->GetParentHWND(), uMsg, wParam, lParam );
		}
	}

	if (mouseResult == AAX_eEventResult_PassEventToSystem)
	{
		WNDPROC origWindowProc = (WNDPROC) GetProp( hwnd, AAX_ORIG_WINDOW_PROC_PROP );
		if ( origWindowProc )
		{
			result = CallWindowProc( origWindowProc, hwnd, uMsg, wParam, lParam );
		}
	}

	return result;
}

// *******************************************************************************
// METHOD:	SetupForMouseInput
// *******************************************************************************
void AAX_CEffectGUI_Win32::SetupForMouseInput ( HWND iWindow )
{
	if ( iWindow )
	{
		WNDPROC oldWindowProc = (WNDPROC) GetWindowLongPtr( iWindow, GWLP_WNDPROC );
		SetProp( iWindow, AAX_ORIG_WINDOW_PROC_PROP, oldWindowProc );
		SetProp( iWindow, AAX_EFFECTGUI_PROP, this );
		SetWindowLongPtr( iWindow, GWLP_WNDPROC, (LONG_PTR) MouseInputWindowProc );
	}
}
 
// *******************************************************************************
// METHOD:	DialogProc
// *******************************************************************************
INT_PTR CALLBACK AAX_CEffectGUI_Win32::DialogProc( __in  HWND hwnd, __in  UINT uMsg, __in  WPARAM wParam, __in  LPARAM lParam )
{
	AAX_CEffectGUI_Win32 * effectGUI = (AAX_CEffectGUI_Win32 *) GetProp( hwnd, AAX_EFFECTGUI_PROP );
	if ( effectGUI )
		return effectGUI->HandleMessage( uMsg, wParam, lParam );

	return false;
}

// *******************************************************************************
// METHOD:	CreatePlugInHWND
// *******************************************************************************
HWND AAX_CEffectGUI_Win32::CreatePlugInHWND ( WORD iResourceID )
{
	HWND plugInHWND = mPlugInHWND;
	
	if ((NULL != GetParentHWND()) && (NULL == plugInHWND))
	{
		plugInHWND = ::CreateDialog( this->GetInstance(), MAKEINTRESOURCE( iResourceID ), GetParentHWND(), DialogProc );
		if ( NULL != plugInHWND )
		{
			SetProp( plugInHWND, AAX_EFFECTGUI_PROP, this );
			this->SetPlugInHWND( plugInHWND );
		}
	}

	return plugInHWND;
}
