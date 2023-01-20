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
 *	\file  AAX_IACFComponentDescriptor.h
 *
 *	\brief	Versioned description interface for an %AAX plug-in algorithm callback
 *
 */ 
/*================================================================================================*/


#ifndef _AAX_IACFCOMPONENTDESCRIPTOR_H_
#define _AAX_IACFCOMPONENTDESCRIPTOR_H_

#include "AAX.h"
#include "AAX_Callbacks.h"
#include "AAX_IDma.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "acfunknown.h"


/** \brief	Versioned description interface for an %AAX plug-in algorithm callback
 */
class AAX_IACFComponentDescriptor : public IACFUnknown
{
public:
	virtual AAX_Result	Clear () = 0;	///< \copydoc AAX_IComponentDescriptor::Clear()
	virtual AAX_Result	AddReservedField ( AAX_CFieldIndex inFieldIndex, uint32_t inFieldType ) = 0;	///< \copydoc AAX_IComponentDescriptor::AddReservedField()
    virtual AAX_Result  AddAudioIn ( AAX_CFieldIndex inFieldIndex ) = 0;	///< \copydoc AAX_IComponentDescriptor::AddAudioIn()
    virtual AAX_Result  AddAudioOut ( AAX_CFieldIndex inFieldIndex ) = 0;	///< \copydoc AAX_IComponentDescriptor::AddAudioOut()
    virtual AAX_Result  AddAudioBufferLength ( AAX_CFieldIndex inFieldIndex ) = 0;	///< \copydoc AAX_IComponentDescriptor::AddAudioBufferLength()
    virtual AAX_Result  AddSampleRate ( AAX_CFieldIndex inFieldIndex ) = 0;	///< \copydoc AAX_IComponentDescriptor::AddSampleRate()
    virtual AAX_Result  AddClock ( AAX_CFieldIndex inFieldIndex ) = 0;	///< \copydoc AAX_IComponentDescriptor::AddClock()
    virtual AAX_Result  AddSideChainIn ( AAX_CFieldIndex inFieldIndex ) = 0;	///< \copydoc AAX_IComponentDescriptor::AddSideChainIn()
	virtual AAX_Result	AddDataInPort ( AAX_CFieldIndex inFieldIndex, uint32_t inPacketSize, AAX_EDataInPortType inPortType) = 0;	///< \copydoc AAX_IComponentDescriptor::AddDataInPort()
	virtual AAX_Result	AddAuxOutputStem ( AAX_CFieldIndex inFieldIndex, int32_t inStemFormat, const char inNameUTF8[]) = 0;	///< \copydoc AAX_IComponentDescriptor::AddAuxOutputStem()
	virtual AAX_Result	AddPrivateData ( AAX_CFieldIndex inFieldIndex, int32_t inDataSize, uint32_t inOptions = AAX_ePrivateDataOptions_DefaultOptions ) = 0;	///< \copydoc AAX_IComponentDescriptor::AddPrivateData()
	virtual AAX_Result	AddDmaInstance ( AAX_CFieldIndex inFieldIndex, AAX_IDma::EMode inDmaMode ) = 0;	///< \copydoc AAX_IComponentDescriptor::AddDmaInstance()
	virtual AAX_Result	AddMIDINode ( AAX_CFieldIndex inFieldIndex, AAX_EMIDINodeType inNodeType, const char inNodeName[], uint32_t channelMask ) = 0;	///< \copydoc AAX_IComponentDescriptor::AddMIDINode()

	virtual AAX_Result	AddProcessProc_Native (	
		AAX_CProcessProc inProcessProc,
		IACFUnknown * inProperties,
		AAX_CInstanceInitProc inInstanceInitProc,
		AAX_CBackgroundProc inBackgroundProc,
		AAX_CSelector * outProcID) = 0;	///< \copydoc AAX_IComponentDescriptor::AddProcessProc_Native()
	virtual AAX_Result	AddProcessProc_TI ( 
		const char inDLLFileNameUTF8 [], 
		const char inProcessProcSymbol [],
		IACFUnknown * inProperties,
		const char inInstanceInitProcSymbol [],
		const char inBackgroundProcSymbol [],
		AAX_CSelector * outProcID) = 0;	///< \copydoc AAX_IComponentDescriptor::AddProcessProc_TI()
	
	virtual AAX_Result	AddMeters ( AAX_CFieldIndex inFieldIndex, const AAX_CTypeID* inMeterIDs, const uint32_t inMeterCount ) = 0;	///< \copydoc AAX_IComponentDescriptor::AddMeters()
};

/** \brief	Versioned description interface for an %AAX plug-in algorithm callback
 */
class AAX_IACFComponentDescriptor_V2 : public AAX_IACFComponentDescriptor
{
public:
    virtual AAX_Result  AddTemporaryData( AAX_CFieldIndex inFieldIndex, uint32_t inDataElementSize) = 0;	///< \copydoc AAX_IComponentDescriptor::AddTemporaryData()
};

/** \brief	Versioned description interface for an %AAX plug-in algorithm callback
 */
class AAX_IACFComponentDescriptor_V3 : public AAX_IACFComponentDescriptor_V2
{
public:
    virtual AAX_Result AddProcessProc (
		IACFUnknown * inProperties,
		AAX_CSelector* outProcIDs,
		int32_t inProcIDsSize) = 0;	///< \copydoc AAX_IComponentDescriptor::AddProcessProc()
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // #ifndef _AAX_IACFCOMPONENTDESCRIPTOR_H_
