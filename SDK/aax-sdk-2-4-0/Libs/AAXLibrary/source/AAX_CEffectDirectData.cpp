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
 *
 */
/*================================================================================================*/

#include "AAX_CEffectDirectData.h"

#include "AAX_IEffectParameters.h"
#include "AAX_VPrivateDataAccess.h"
#include "AAX_VController.h"
#include "AAX_UIDs.h"

AAX_CEffectDirectData::AAX_CEffectDirectData() :
	mController(NULL),
	mEffectParameters(NULL)
{

}

AAX_CEffectDirectData::~AAX_CEffectDirectData()
{	
	//<DMT> Double check to make sure it was uninited.
	this->Uninitialize();
}

AAX_Result AAX_CEffectDirectData::Initialize ( IACFUnknown * iController )
{
	mController = new AAX_VController(iController);
	if (iController == 0 || mController == 0)
		return AAX_ERROR_NOT_INITIALIZED;
		
	if ( iController )
	{
		iController->QueryInterface(IID_IAAXEffectParametersV1, (void **)&mEffectParameters);
	}
	
	return Initialize_PrivateDataAccess();
}

AAX_Result AAX_CEffectDirectData::Uninitialize (void)
{
	if (mEffectParameters)
	{
		mEffectParameters->Release(); // Is this the correct way to handle this member?
		mEffectParameters = NULL;
	}
	
	if ( mController )
	{
		delete ( mController ); // Is this the correct way to handle this member?
		mController = NULL;
	}
	return AAX_SUCCESS;
}

AAX_Result AAX_CEffectDirectData::TimerWakeup (IACFUnknown * inDataAccessInterface )
{
	AAX_Result result = AAX_SUCCESS;
	
	AAX_VPrivateDataAccess dataAccess( inDataAccessInterface );
	result = TimerWakeup_PrivateDataAccess (&dataAccess);
	
	return result;
}

AAX_IController* AAX_CEffectDirectData::Controller(void)
{
	return mController;
}

AAX_IEffectParameters* AAX_CEffectDirectData::EffectParameters(void)
{
	return mEffectParameters;
}

AAX_Result AAX_CEffectDirectData::Initialize_PrivateDataAccess()
{
	return AAX_SUCCESS;
}

AAX_Result AAX_CEffectDirectData::TimerWakeup_PrivateDataAccess(AAX_IPrivateDataAccess*)
{
	return AAX_SUCCESS;
}

AAX_Result AAX_CEffectDirectData::NotificationReceived( AAX_CTypeID ,const void * ,uint32_t )
{
    return AAX_SUCCESS;
}





