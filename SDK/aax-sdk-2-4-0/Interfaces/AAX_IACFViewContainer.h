/*================================================================================================*/
/*
 *
 *	Copyright 2013-2017 by Avid Technology, Inc.
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
 *	\file  AAX_IACFViewContainer.h
 *
 *	\brief Interface for the %AAX host's view of a single instance of an 
 *	effect.  Used by both clients of the AAXHost and by effect components.
 *
 */ 
/*================================================================================================*/


#ifndef _AAX_IACFVIEWCONTAINER_H_
#define _AAX_IACFVIEWCONTAINER_H_

#include "AAX_GUITypes.h"
#include "AAX.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "acfunknown.h"

/** \brief Interface for the %AAX host's view of a single instance of an 
 *	effect.  Used by both clients of the host app and by effect components.
 *	
 *	\details
 *	\sa \ref AAX_IViewContainer
 */
class AAX_IACFViewContainer : public IACFUnknown
{
public:	
	/** @name View and GUI state queries
	 */
	//@{
	virtual int32_t		GetType () = 0;	///< \copydoc AAX_IViewContainer::GetType()
    virtual void *		GetPtr () = 0;	///< \copydoc AAX_IViewContainer::GetPtr()
	virtual AAX_Result	GetModifiers ( uint32_t * outModifiers ) = 0;	///< \copydoc AAX_IViewContainer::GetModifiers()
	//@}end View and GUI state queries

	/** @name View change requests
	 */
	//@{
    virtual AAX_Result	SetViewSize ( AAX_Point & inSize ) = 0;	///< \copydoc AAX_IViewContainer::SetViewSize()
	//@}end View change requests
	
	/** @name Host event handlers
	 */
	//@{	
	virtual AAX_Result	HandleParameterMouseDown	( AAX_CParamID inParamID, uint32_t inModifiers ) = 0;	///< \copydoc AAX_IViewContainer::HandleParameterMouseDown()
	virtual AAX_Result	HandleParameterMouseDrag	( AAX_CParamID inParamID, uint32_t inModifiers ) = 0;	///< \copydoc AAX_IViewContainer::HandleParameterMouseDrag()
	virtual AAX_Result	HandleParameterMouseUp		( AAX_CParamID inParamID, uint32_t inModifiers ) = 0;	///< \copydoc AAX_IViewContainer::HandleParameterMouseUp()
	//@}end Host event handlers
};


/** \brief Supplemental interface for the %AAX host's view of a single instance of an 
 *	effect.  Used by both clients of the host app and by effect components.
 *	
 *	\details
 *	\sa \ref AAX_IViewContainer
 */
class AAX_IACFViewContainer_V2 : public AAX_IACFViewContainer
{
public:	
	/** @name Host event handlers
	 */
	//@{
	virtual AAX_Result	HandleMultipleParametersMouseDown ( const AAX_CParamID* inParamIDs, uint32_t inNumOfParams, uint32_t inModifiers ) = 0;	  ///< \copydoc AAX_IViewContainer::HandleMultipleParametersMouseDown()
	virtual AAX_Result	HandleMultipleParametersMouseDrag ( const AAX_CParamID* inParamIDs, uint32_t inNumOfParams, uint32_t inModifiers ) = 0;	  ///< \copydoc AAX_IViewContainer::HandleMultipleParametersMouseDrag()
	virtual AAX_Result	HandleMultipleParametersMouseUp   ( const AAX_CParamID* inParamIDs, uint32_t inNumOfParams, uint32_t inModifiers ) = 0;    ///< \copydoc AAX_IViewContainer::HandleMultipleParametersMouseUp()
	//@}end Host event handlers
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif
