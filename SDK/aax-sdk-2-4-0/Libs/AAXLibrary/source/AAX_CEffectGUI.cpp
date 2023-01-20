/*================================================================================================*/
/*
 *	Copyright 2013-2017, 2019, 2021 by Avid Technology, Inc.
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
/*================================================================================================*/

#include "AAX_CEffectGUI.h"
#include "AAX_VController.h"
#include "AAX_VTransport.h"
#include "AAX_VViewContainer.h"
#include "AAX_IEffectParameters.h"
#include "AAX_IParameter.h"
#include "AAX_UIDs.h"
#include "AAX_CString.h"

// *******************************************************************************
// METHOD:	AAX_CEffectGUI
// *******************************************************************************
AAX_CEffectGUI::AAX_CEffectGUI() :
	mController(NULL),
	mEffectParameters(NULL),
	mViewContainer(),
	mTransport(NULL)
{

}

// *******************************************************************************
// METHOD:	~AAX_CEffectGUI
// *******************************************************************************
AAX_CEffectGUI::~AAX_CEffectGUI()
{	
	this->Uninitialize();	//<DMT>  Just to guarantee that all these things get broken down...
}

// *******************************************************************************
// METHOD:	Initialize
// *******************************************************************************
AAX_Result AAX_CEffectGUI::Initialize ( IACFUnknown * iController )
{
	mController = new AAX_VController(iController);
	if (iController == 0 || mController == 0)
		return AAX_ERROR_NOT_INITIALIZED;
		
	if ( iController )
	{
		iController->QueryInterface(IID_IAAXEffectParametersV1, (void **)&mEffectParameters);
	}
	
	mTransport = new AAX_VTransport(iController);
		
	this->CreateViewContents ();
	return AAX_SUCCESS;
}

// *******************************************************************************
// METHOD:	Uninitialize
// *******************************************************************************
AAX_Result		AAX_CEffectGUI::Uninitialize (void)
{
	if ( nullptr != this->GetViewContainer() )
	{
		this->SetViewContainer(nullptr);
	}
	
	if (mEffectParameters)
	{
		mEffectParameters->Release();
		mEffectParameters = 0;
	}
	
	if ( mController )
	{
		delete ( mController );
		mController = 0;
	}
	if ( mTransport )
	{
		delete mTransport;
		mTransport = NULL;
	}
	
	return AAX_SUCCESS;
}

// *******************************************************************************
// METHOD:	NotificationReceived
// *******************************************************************************
AAX_Result		AAX_CEffectGUI::NotificationReceived(AAX_CTypeID /*inNotificationType*/, const void * /*inNotificationData*/, uint32_t	/*inNotificationDataSize*/)
{
	//Base implementation doesn't need to know any of these right now.
	return AAX_SUCCESS;
}



// *******************************************************************************
// METHOD:	SetViewContainer
// *******************************************************************************
AAX_Result AAX_CEffectGUI::SetViewContainer ( IACFUnknown * inViewContainer )
{
	if ( !inViewContainer )
	{
		this->DeleteViewContainer ();
		mViewContainer.reset();
	}
	else
	{
		mViewContainer.reset(new AAX_VViewContainer ( inViewContainer ));
		this->CreateViewContainer ();
		this->UpdateAllParameters ();
	}

	return AAX_SUCCESS;
}

// *******************************************************************************
// METHOD:	GetViewContainerType
// *******************************************************************************
AAX_EViewContainer_Type AAX_CEffectGUI::GetViewContainerType ()
{
	AAX_EViewContainer_Type	result = AAX_eViewContainer_Type_NULL;
	if ( AAX_IViewContainer* const viewContainer = this->GetViewContainer() ) {
		result = static_cast<AAX_EViewContainer_Type>(viewContainer->GetType());
	}
	
	return result;
}

// *******************************************************************************
// METHOD:	GetViewContainerPtr
// *******************************************************************************
void * AAX_CEffectGUI::GetViewContainerPtr ()
{
	void *	result = nullptr;
	if ( AAX_IViewContainer* const viewContainer = this->GetViewContainer() ) {
		result = viewContainer->GetPtr ();
	}
	
	return result;
}

// *******************************************************************************
// METHOD:	ParameterUpdated (Called from Host on main thread)
// *******************************************************************************
AAX_Result	AAX_CEffectGUI::ParameterUpdated(AAX_CParamID /*inParamID*/)
{
	return AAX_SUCCESS;
}

// *******************************************************************************
// METHOD:	UpdateAllParameters
// *******************************************************************************
void AAX_CEffectGUI::UpdateAllParameters ()
{
	if ( mEffectParameters )
	{
		int32_t	numControls = 0;
		if ( mEffectParameters->GetNumberOfParameters( &numControls ) == 0 )
		{
			for ( int32_t index = 0; index <= numControls; ++index )
			{
				AAX_CString	paramID;
				if ( mEffectParameters->GetParameterIDFromIndex( index, &paramID ) == 0 )
						this->ParameterUpdated( (AAX_CParamID)paramID.CString() );
			}
		}
	}
}

// *******************************************************************************
// METHOD:	GetPlugInString
// *******************************************************************************
AAX_Result	AAX_CEffectGUI::GetCustomLabel ( AAX_EPlugInStrings /*inSelector*/, AAX_IString * /*outString*/ ) const
{	
	return AAX_ERROR_NULL_OBJECT;
}

// *******************************************************************************
// METHOD:	GetController
// *******************************************************************************
AAX_IController* AAX_CEffectGUI::GetController (void)
{
    return mController;
}
const AAX_IController* AAX_CEffectGUI::GetController (void) const
{
    return mController;
}

// *******************************************************************************
// METHOD:	GetEffectParameters
// *******************************************************************************
AAX_IEffectParameters* AAX_CEffectGUI::GetEffectParameters (void)
{
    return mEffectParameters;
}
const AAX_IEffectParameters* AAX_CEffectGUI::GetEffectParameters (void) const
{
    return mEffectParameters;
}

// *******************************************************************************
// METHOD:	GetViewContainer
// *******************************************************************************
AAX_IViewContainer* AAX_CEffectGUI::GetViewContainer (void)
{
    return mViewContainer.get();
}
const AAX_IViewContainer* AAX_CEffectGUI::GetViewContainer (void) const
{
    return mViewContainer.get();
}


// *******************************************************************************
// METHOD:	Transport
// *******************************************************************************
AAX_ITransport* AAX_CEffectGUI::Transport()
{
	return mTransport;
}
const AAX_ITransport* AAX_CEffectGUI::Transport() const
{
	return mTransport;
}
