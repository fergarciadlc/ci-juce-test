/*================================================================================================*/
/*
 *
 *	Copyright 2014-2015, 2018 by Avid Technology, Inc.
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
 *	\file  AAX_CParameterManager.h
 *
 *	\brief A container object for plug-in parameters
 *
 */ 
/*================================================================================================*/


#ifndef AAX_CPARAMETERMANAGER_H
#define AAX_CPARAMETERMANAGER_H

#include "AAX_CParameter.h"
#include "AAX.h"

#include <vector>
#include <map>




class AAX_IAutomationDelegate;

/**	\brief A container object for plug-in parameters
	
	\details
	This implementation uses a STL vector to store a plug-in's set of parameters.  This class
	contains a real implementation of the \ref AAXLibraryFeatures_ParameterManager (as opposed to a proxy.)
	
	For more information, see \ref AAXLibraryFeatures_ParameterManager.

	\todo Should the Parameter Manager return error codes?

	\ingroup AAXLibraryFeatures_ParameterManager
 */
class AAX_CParameterManager
{
public:
	AAX_CParameterManager();
	~AAX_CParameterManager();

	/*!
	 *  \brief Initialize the parameter manager
	 *  
	 *  Called when plug-in instance is first instantiated.  This method will initialize the
	 *	plug-in's automation delegate, among other set-up tasks.
	 *
	 *	\param[in] iAutomationDelegateUnknown
	 *		A reference to the plug-in's AAX_IAutomationDelegate interface
	 */	
	void			Initialize(AAX_IAutomationDelegate* iAutomationDelegateUnknown);
	
	/*!
	 *  \brief Returns the number of parameters in this instance of the parameter manager
	 *
	 */	
	int32_t		NumParameters()	const;	
	
	/*!
	 *  \brief Removes a parameter from the manager
	 *
	 *	\todo Should this method return success/failure code?
	 *
	 *	\param[in] identifier
	 *		ID of the parameter that will be removed
	 */	
	void			RemoveParameterByID(AAX_CParamID identifier);
	
	/*!
	 *  \brief Removes all parameters from the manager
	 *
	 *	\todo Should this method return success/failure code?
	 */	
	void			RemoveAllParameters();

	/*!
	 *  \brief Given a parameter ID, retrieves a reference to the requested parameter
	 *
	 *	\param[in] identifier
	 *		ID of the parameter that will be retrieved
	 */	
	AAX_IParameter*			GetParameterByID(AAX_CParamID  identifier);
	
	/*!
	 *  \brief Given a parameter ID, retrieves a const reference to the requested parameter
	 *
	 *	\param[in] identifier
	 *		ID of the parameter that will be retrieved
	 */	
	const AAX_IParameter*	GetParameterByID(AAX_CParamID  identifier) const;
	
	/*!
	 *  \brief Given a parameter name, retrieves a reference to the requested parameter
	 *
	 *	\note Parameter names may be ambiguous
	 *
	 *	\param[in] name
	 *		Name of the parameter that will be retrieved
	 */
	AAX_IParameter*			GetParameterByName(const char*  name);
	
	/*!
	 *  \brief Given a parameter name, retrieves a const reference to the requested parameter
	 *
	 *	\note Parameter names may be ambiguous
	 *
	 *	\param[in] name
	 *		ID of the parameter that will be retrieved
	 */
	const AAX_IParameter*	GetParameterByName(const char*  name) const;
	
	/*!
	 *  \brief Given a parameter index, retrieves a reference to the requested parameter
	 *
	 *	Parameter indices are incremented in the order that parameters are added to the manager.
	 *	See AddParameter().
	 *
	 *	\param[in] index
	 *		Index of the parameter that will be retrieved
	 */	
	AAX_IParameter*			GetParameter(int32_t index);

	/*!
	 *  \brief Given a parameter index, retrieves a const reference to the requested parameter
	 *
	 *	Parameter indices are incremented in the order that parameters are added to the manager.
	 *	See AddParameter().
	 *
	 *	\param[in] index
	 *		Index of the parameter that will be retrieved
	 */	
	const AAX_IParameter*	GetParameter(int32_t index) const;
	
	/** Given a parameter ID, retrieves the index for the specified parameter
	 *
	 *	\param[in] identifier
	 *		ID of the parameter that will be retrieved
	 */	
	int32_t					GetParameterIndex(AAX_CParamID identifier) const;

	/** Adds a parameter to the manager
	 *
	 *	\todo Should this method return success/failure code?
	 *
	 *	\param[in] param
	 *		Reference to the parameter that will be added
	 */	
	void					AddParameter(AAX_IParameter*	param);
	
	/** Removes a parameter to the manager
	 *
	 *	\todo Should this method return success/failure code?
	 *
	 *	\param[in] param
	 *		Reference to the parameter that will be removed
	 */	
	void					RemoveParameter(AAX_IParameter*	param);
	
protected:
    
	AAX_IAutomationDelegate* 		mAutomationDelegate;        //This object is not ref-counted here.  Do not delete it.  It is ref counted by this object's parent.
	std::vector<AAX_IParameter*>	mParameters;
    std::map<std::string, AAX_IParameter*> mParametersMap;
};




#endif // AAX_CPARAMETERMANAGER_H
