/*================================================================================================*/
/*
 *
 *	Copyright 2013-2017, 2020-2021 by Avid Technology, Inc.
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
 *	\file  AAX_IEffectParameters.h
 *
 *	\brief The interface for an %AAX Plug-in's data model
 *
 */ 
/*================================================================================================*/


#ifndef AAX_IEFFECTPARAMETERS_H
#define AAX_IEFFECTPARAMETERS_H

#include "AAX_IACFEffectParameters.h"
#include "AAX.h"
#include "CACFUnknown.h"

/** @brief	The interface for an %AAX Plug-in's data model.
	
	@details
	@pluginimp
	
	The interface for an instance of a plug-in's data model. A plug-in's implementation
	of this interface is responsible for creating the plug-in's set of parameters and
	for defining how the plug-in will respond when these parameters are changed via
	control updates or preset loads.  In order for information to be routed from the
	plug-in's data model to its algorithm, the parameters that are created here must be
	registered with the host in the plug-in's
	\ref CommonInterface_Describe "Description callback".

	At \ref AAX_IACFEffectParameters::Initialize() "initialization", the host provides
	this interface with a reference to AAX_IController, which provides access from the
	data model back to the host. This reference provides a means of querying information
	from the host such as stem format or sample rate, and is also responsible for
	communication between the data model and the plug-in's (decoupled) algorithm. See
	\ref CommonInterface_Algorithm.

	You will most likely inherit your implementation of this interface from 
	\ref AAX_CEffectParameters, a default implementation that provides basic data model
	functionality such as adding custom parameters, setting control values, restoring
	state, generating coefficients, etc., which you can override and customize as
	needed.

	The following tags appear in the descriptions for methods of this class and its
	derived classes:
		\li \c CALL: Components in the plug-in should call this method to get / set
		data in the data model.

	\note
		\li This class always inherits from the latest version of the interface and thus
		requires any subclass to implement all the methods in the latest version of the
		interface. The current version of \ref AAX_CEffectParameters provides a convenient
		default implementation for all methods in the latest interface.
		\li Except where noted otherwise, the parameter values referenced by the methods in
		this interface are normalized values.  See \ref AAXLibraryFeatures_ParameterManager
		for more information.

	\legacy In the legacy plug-in SDK, these methods were found in CProcess and
	\c CEffectProcess.  For additional \c CProcess methods, see \ref AAX_IEffectGUI.

	\section AAX_IEffectParameters_relclass Related classes
	\dotfile aax_ieffectparams_related.dot "Classes related to AAX_IEffectParameters by inheritance or composition"
	\dotfile aax_ieffectparams_contained.dot "Classes owned as member objects of AAX_CEffectParameters"

	\ingroup CommonInterface_DataModel
*/
class AAX_IEffectParameters :	public AAX_IACFEffectParameters_V4
								, public CACFUnknown
{
public:
	ACF_DECLARE_STANDARD_UNKNOWN()
	
	ACFMETHOD(InternalQueryInterface)(const acfIID & riid, void **ppvObjOut) override;
	
	// CACFUnknown does not support operator=()
	AAX_DELETE(AAX_IEffectParameters& operator= (const AAX_IEffectParameters&));
};

#endif // AAX_IEFFECTPARAMETERS_H
