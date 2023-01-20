/*================================================================================================*/
/*
 *	Copyright 2011-2015, 2018 by Avid Technology, Inc.
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
 *	\file   AAX_CPacketDispatcher.cpp
 *	
 *	\author Viktor Iarovyi
 *
 */ 
/*================================================================================================*/
#include "AAX_CPacketDispatcher.h"
#include "AAX_IEffectParameters.h"
#include "AAX_IParameter.h"

#include <vector>

////////////////////////////////////////////////////////////////////////////////
// class AAX_CPacket implementation

const size_t kDefaultPacketSize = 8;

AAX_CPacket::SPacketData::SPacketData()
{
	mData = new std::vector<char>(kDefaultPacketSize);
}

AAX_CPacket::SPacketData::~SPacketData()
{
	std::vector<char>* storage = static_cast<std::vector<char>*>(mData);
	delete storage;
}

const void*
AAX_CPacket::SPacketData::Get() const
{
	std::vector<char>* storage = static_cast<std::vector<char>*>(mData);
	return &storage->at(0);
}

void*
AAX_CPacket::SPacketData::Get(size_t maxSize) const
{
	std::vector<char>* storage = static_cast<std::vector<char>*>(mData);
	storage->resize(maxSize);
	return &storage->at(0);
}


////////////////////////////////////////////////////////////////////////////////
// class AAX_CPacketHandler

// *******************************************************************************
// METHOD:	AAX_CPacketDispatcher
// *******************************************************************************
AAX_CPacketDispatcher::AAX_CPacketDispatcher()
:	mPacketsHolder(),
	mPacketsHandlers(),
	mController(NULL),
	mEffectParameters(NULL),
	mLockGuard()
{
}

// *******************************************************************************
// METHOD:	~AAX_CPacketDispatcher
// *******************************************************************************
AAX_CPacketDispatcher::~AAX_CPacketDispatcher()
{
	AAX_StLock_Guard guard(this->mLockGuard); 

	// delete registered packets
	for (PacketsHolder::iterator each = mPacketsHolder.begin();  each != mPacketsHolder.end (); each++) 
	{
		AAX_CPacket* packet = each->second;
		delete packet;
	}
		
	// delete registered handlers
	for (PacketsHandlersMap::iterator each = mPacketsHandlers.begin();  each != mPacketsHandlers.end (); each++) 
	{
		AAX_IPacketHandler* handler = each->second.second;
		delete handler;
	}
}

// *******************************************************************************
// METHOD:	Initialize
// *******************************************************************************
void AAX_CPacketDispatcher::Initialize( AAX_IController* inController, AAX_IEffectParameters* inEffectParameters)
{
	mController = inController;
	mEffectParameters = inEffectParameters;
}

// *******************************************************************************
// METHOD:	RegisterTarget
// *******************************************************************************
AAX_Result AAX_CPacketDispatcher::RegisterPacket( AAX_CParamID paramID, AAX_CFieldIndex portID, const AAX_IPacketHandler* inHandler)
{
	AAX_StLock_Guard guard(this->mLockGuard); 
	
	AAX_CPacket* packet = NULL;
	
	// find whether the packet was registered
	if (0 <= portID)
	{
		PacketsHolder::iterator found (mPacketsHolder.find (portID));
		if (found == mPacketsHolder.end())
		{
			packet = new AAX_CPacket(portID);
			mPacketsHolder.insert(std::make_pair(portID, packet));
		}
		else
		{
			packet = found->second;
		}
	}

	// register handler for parameter
	std::pair<AAX_CPacket*, AAX_IPacketHandler* > myPair( packet, inHandler->Clone() );
	mPacketsHandlers.insert(std::make_pair(std::string(paramID), myPair));
	
	return AAX_SUCCESS;
}
	
// *******************************************************************************
// METHOD:	SetDirty
// *******************************************************************************
AAX_Result AAX_CPacketDispatcher::SetDirty(AAX_CParamID paramID, bool inDirty)
{
	AAX_StLock_Guard guard(this->mLockGuard); 

	PacketsHandlersMap::iterator found (mPacketsHandlers.find (paramID));
	for (PacketsHandlersMap::iterator each = found;  each != mPacketsHandlers.end (); each++) 
	{
		if (each->first != paramID)
			break;
		
		AAX_CPacket* packet = each->second.first;
		packet->SetDirty(inDirty);
	}
	
	return AAX_SUCCESS;
}
	
// *******************************************************************************
// METHOD:	Dispatch
// *******************************************************************************
AAX_Result AAX_CPacketDispatcher::Dispatch()
{
	AAX_StLock_Guard guard(this->mLockGuard); 

	AAX_Result	result = AAX_SUCCESS;
	
	for (PacketsHandlersMap::iterator each = mPacketsHandlers.begin();  each != mPacketsHandlers.end (); each++) 
	{
		AAX_CPacket* packet = each->second.first;
		if ( packet->IsDirty() )
		{
			AAX_IPacketHandler* handler = each->second.second;
			if (AAX_SUCCESS == handler->Call( each->first.c_str(), *packet ))
			{	   
				result = mController->PostPacket( packet->GetID(), packet->GetPtr<const void>(), packet->GetSize() );
			}
				
			packet->SetDirty(false);
		}
	}
	
	return result;
}


// *******************************************************************************
// METHOD:	GenerateSingleValuePacket  (simple convenience for single values packets)
// *******************************************************************************
AAX_Result AAX_CPacketDispatcher::GenerateSingleValuePacket( AAX_CParamID inParam, AAX_CPacket& ioPacket)
{
	AAX_IParameter*		parameter;
	AAX_Result			result = mEffectParameters->GetParameter ( inParam, &parameter );

	if ((result == AAX_SUCCESS) && (parameter != 0))
	{
		bool boolValue;
		if(parameter->GetValueAsBool(&boolValue))
		{	
			*ioPacket.GetPtr<int32_t>() = int32_t(boolValue);
			return AAX_SUCCESS;
		}
		
		float floatValue;
		if (parameter->GetValueAsFloat(&floatValue))
		{	
			*ioPacket.GetPtr<float>() = floatValue;
			return AAX_SUCCESS;
		}
		
		int32_t intValue;
		if(parameter->GetValueAsInt32(&intValue))
		{			
			*ioPacket.GetPtr<int32_t>() = intValue;
			return AAX_SUCCESS;
		}
		
		double doubleValue;
		if(parameter->GetValueAsDouble(&doubleValue))
		{			
			*ioPacket.GetPtr<double>() = doubleValue;
			return AAX_SUCCESS;
		}
	}
	
	return AAX_ERROR_NULL_OBJECT;
}

