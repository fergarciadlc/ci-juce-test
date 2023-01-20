/*================================================================================================*/
/*
 *	Copyright 2014-2016, 2019 by Avid Technology, Inc.
 *	All rights reserved.
 *
 *	CONFIDENTIAL: This document contains confidential information.  Do not 
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
 *	OF Avid Technology, INC.
 */

/**  
 *	\file AAX_CMonolithicParameters.h
 *
 *	\brief A convenience class extending \ref AAX_CEffectParameters for monolithic instruments
 *
 */ 
/*================================================================================================*/


#ifndef AAX_CMONOLITHICPARAMETERS_H
#define AAX_CMONOLITHICPARAMETERS_H

// AAX headers
//
// Parent class
#include "AAX_CEffectParameters.h"
//
// Describe
#include "AAX_IEffectDescriptor.h"
#include "AAX_IComponentDescriptor.h"
#include "AAX_IPropertyMap.h"
//
// Utilities
#include "AAX_CAtomicQueue.h"
#include "AAX_IParameter.h"
#include "AAX_IMIDINode.h"
#include "AAX_IString.h"

// Standard Includes
#include <set>
#include <list>
#include <utility>


// Max number of additional midi nodes is 15, for a grand total of 16 input midi nodes.  We're not aware of any plug-in that uses more.  
#define kMaxAdditionalMIDINodes     15

// You can increase this if you need, it is not a system limitation.
#define kMaxAuxOutputStems     32

// You can increase this if you need; it should be set to a value greater than or equal to the number of synchronized parameters in your plug-in
#define kSynchronizedParameterQueueSize     32


/** \brief Information used to describe the instrument
 *
 *	\details
 *  \sa \ref AAX_CMonolithicParameters::StaticDescribe()
 */
struct AAX_SInstrumentSetupInfo
{
    //Global MIDI Nodes
	bool				mNeedsGlobalMIDI;           ///< Does the instrument use a global MIDI input node?  \sa \ref additionalFeatures_MIDI
	const char*			mGlobalMIDINodeName;        ///< Name of the global MIDI node, if used  \sa \ref additionalFeatures_MIDI
	uint32_t			mGlobalMIDIEventMask;       ///< Global MIDI node event mask of \ref AAX_EMidiGlobalNodeSelectors, if used  \sa \ref additionalFeatures_MIDI
	
    //Input MIDI Nodes
	bool				mNeedsInputMIDI;                ///< Does the instrument use a local MIDI input node?  \sa \ref additionalFeatures_MIDI
	const char*			mInputMIDINodeName;             ///< Name of the MIDI input node, if used  \sa \ref additionalFeatures_MIDI
	uint32_t			mInputMIDIChannelMask;          ///< MIDI input node channel mask, if used  \sa \ref additionalFeatures_MIDI
	int32_t             mNumAdditionalInputMIDINodes;   ///< Number of additional input MIDI Nodes.  These will all share the same channelMask and base MIDINodeName, but the names will be appended with numbers 2,3,4,...  \sa \ref additionalFeatures_MIDI
    
    //Transport
	bool				mNeedsTransport;            ///< Does the instrument use the transport interface? \sa \ref AAX_ITransport
	const char*			mTransportMIDINodeName;     ///< Name of the MIDI transport node, if used \sa \ref AAX_ITransport
	
    //Meters
	int32_t				mNumMeters;					///< Number of meter taps used by the instrument.  Must match the size of \ref mMeterIDs \sa \ref AdditionalFeatures_Meters
	const AAX_CTypeID*	mMeterIDs;					///< Array of meter IDs  \sa \ref AdditionalFeatures_Meters
    
    //Aux Output Stems Feature.
    int32_t             mNumAuxOutputStems;         ///< Number of aux output stems for the plug-in. \sa \ref additionalFeatures_AOS
    const char*         mAuxOutputStemNames[kMaxAuxOutputStems];     ///< Names of the aux output stems. \sa \ref additionalFeatures_AOS
    AAX_EStemFormat     mAuxOutputStemFormats[kMaxAuxOutputStems];      ///< Stem formats for the output stems. \sa \ref additionalFeatures_AOS
	
	//AAX Hybrid
	AAX_EStemFormat     mHybridInputStemFormat;     ///< \ref AAX_eProperty_HybridInputStemFormat "Hybrid input stem format" \details A plug-in that defines this value must also define \c mHybridOutputStemFormat and implement \ref AAX_IEffectParameters::RenderAudio_Hybrid() \sa \ref additionalFeatures_Hybrid
	AAX_EStemFormat     mHybridOutputStemFormat;    ///< \ref AAX_eProperty_HybridOutputStemFormat "Hybrid output stem format" \details A plug-in that defines this value must also define \c mHybridInputStemFormat and implement \ref AAX_IEffectParameters::RenderAudio_Hybrid() \sa \ref additionalFeatures_Hybrid
	
	
	//General Properties
	AAX_EStemFormat	mInputStemFormat;               ///< \ref AAX_eProperty_InputStemFormat "Input stem format"
	AAX_EStemFormat	mOutputStemFormat;              ///< \ref AAX_eProperty_OutputStemFormat "Output stem format"
    bool            mUseHostGeneratedGUI;           ///< Allow Pro Tools or other host to generate a generic GUI. This can be useful for early development. \sa \ref AAX_eProperty_UsesClientGUI
	bool			mCanBypass;                     ///< Can this instrument be bypassed? \sa \ref AAX_eProperty_CanBypass
	AAX_CTypeID		mManufacturerID;                ///< \ref AAX_eProperty_ManufacturerID "Manufacturer ID"
	AAX_CTypeID		mProductID;                     ///< \ref AAX_eProperty_ProductID "Product ID"
	AAX_CTypeID		mPluginID;                      ///< \ref AAX_eProperty_PlugInID_Native "Plug-In (Type) ID"
	AAX_CTypeID     mAudiosuiteID;                  ///< \ref AAX_eProperty_PlugInID_AudioSuite "AudioSuite ID"
	
	/** Multi-mono support
	 
	 \note It is recommended to un-set the \c mMultiMonoSupport flag for
	 VIs and other plug-ins which rely on non-global MIDI input. For more
	 information see \ref AAX_eProperty_Constraint_MultiMonoSupport
	 */
	AAX_CBoolean    mMultiMonoSupport;
	
	
	/** \brief Default constructor
     *
     *  Use this constructor if you want to enable a sub-set of features
     *  and don't need to fill out the whole struct.
     */ 
	AAX_SInstrumentSetupInfo()
	{
		mNeedsGlobalMIDI = false;
		mGlobalMIDINodeName = "GlobalMIDI";
		mGlobalMIDIEventMask = 0xffff;
		mNeedsInputMIDI = false;
		mInputMIDINodeName = "InputMIDI";
		mInputMIDIChannelMask = 0xffff;
        mNumAdditionalInputMIDINodes = 0;
		mNeedsTransport = false;
		mTransportMIDINodeName = "Transport";
		mNumMeters = 0;
		mMeterIDs = 0;
		mInputStemFormat = AAX_eStemFormat_Mono;
		mOutputStemFormat = AAX_eStemFormat_Mono;
        mUseHostGeneratedGUI = false;
		mCanBypass = true;
		mManufacturerID = 'none';
		mProductID = 'none';
		mPluginID = 'none';
		mAudiosuiteID = 'none';
		mMultiMonoSupport = true;
        
        mNumAuxOutputStems = 0;
        for (int32_t i=0; i<kMaxAuxOutputStems; i++)
        {
            mAuxOutputStemNames[i] = 0;
            mAuxOutputStemFormats[i] = AAX_eStemFormat_Mono;
        }
		
		mHybridInputStemFormat = AAX_eStemFormat_None;
		mHybridOutputStemFormat = AAX_eStemFormat_None;
	}
};

class AAX_CMonolithicParameters;	//predefined for AAX_SInstrumentPrivateData
/** \brief Utility struct for \ref AAX_CMonolithicParameters
 *
 *  This is an implementation detail of \ref AAX_CMonolithicParameters; you
 *  should never need to interact with this structure directly.
 */
struct AAX_SInstrumentPrivateData
{
    /** \brief A pointer to the instrument's data model
     *
     *  You should never need to use this since the data model is
     *  available directly from within the virtual
     *  \ref AAX_CMonolithicParameters::RenderAudio() function.
     */
	AAX_CMonolithicParameters*	mMonolithicParametersPtr;
    
    //<DMT>  Removed mOutputStemFormat. Adding it to this structure was not intentional.  Please call Controller()->GetOutputStemFormat() from your RenderAudio call if you need this info.
    //<DMT>  Please note, nothing else should be added to this struct.  All host information is accessible through the AAX_IController in the RenderAudio call.
};


/** \brief Information used to parameterize \ref AAX_CMonolithicParameters::RenderAudio()
 *
 */
struct AAX_SInstrumentRenderInfo
{
	float**						mAudioInputs;			///< Audio input buffers
	float**						mAudioOutputs;			///< Audio output buffers, including any aux output stems.
	int32_t*					mNumSamples;			///< Number of samples in each buffer.  Bounded as per \ref AAE_EAudioBufferLengthNative.  The exact value can vary from buffer to buffer.
    AAX_CTimestamp*             mClock;                 ///< Pointer to the global running time clock.

	AAX_IMIDINode*              mInputNode;             ///< Buffered local MIDI input node. Used for incoming MIDI messages directed to the instrument.
	AAX_IMIDINode*              mGlobalNode;            ///< Buffered global MIDI input node. Used for global events like beat updates in metronomes.
	AAX_IMIDINode*              mTransportNode;         ///< Transport MIDI node.  Used for querying the state of the MIDI transport.
    AAX_IMIDINode*              mAdditionalInputMIDINodes[kMaxAdditionalMIDINodes];  ///< List of additional input MIDI nodes, if your plugin needs them.

	AAX_SInstrumentPrivateData*	mPrivateData;			///< Struct containing private data relating to the instance.  You should not need to use this data.

	float**						mMeters;				///< Array of meter taps.  One meter value should be entered per tap for each render call.
	
	int64_t*					mCurrentStateNum;		///< State counter
};

/** @brief Extension of the \ref AAX_CEffectParameters class for monolithic VIs and effects
 
 This extension to \ref AAX_CEffectParameters adds some conveniences for Virtual
 Instrument (VI) plug-ins and for other plug-ins that use a monolithic processing object,
 i.e. an object that combines state data with the audio render routine in a single object.
 
    \li The \ref AAX_CMonolithicParameters::RenderAudio() "RenderAudio" method provides a
        direct audio processing callback within the data model object.  Perform all
        audio processing in this method.
    \li The \ref AAX_CMonolithicParameters::StaticDescribe() "StaticDescribe" method
        establishes a generic MIDI processing context for the Effect.  Call this method
        from the plug-in's \ref CommonInterface_Describe implementation.
    \li The \ref AAX_CMonolithicParameters::AddSynchronizedParameter() "AddSynchronizedParameter"
        method provides a mechanism for synchronizing parameter updates with the real-time
        thread, allowing deterministic, accurate automation playback. For more information
        abou this feature, see \ref parameterUpdateTiming_sharedData
 
 \note This convenience class assumes a monolithic processing environment (i.e.
 \ref AAX_eConstraintLocationMask_DataModel .)  This precludes the use of
 \ref AAX_CMonolithicParameters -derived Effects in distributed-processing formats
 such as %AAX DSP.
 
 \ingroup AAXLibraryFeatures_MonolithicParameters
 */
class AAX_CMonolithicParameters : public AAX_CEffectParameters
{
public: ///////////////////////////////////////////////////////////////////////////// constructor/destructor
	AAX_CMonolithicParameters (void);
	~AAX_CMonolithicParameters (void) AAX_OVERRIDE;
			
protected: ///////////////////////////////////////////////////////////////////////////// AAX_CMonolithicParameters
	typedef std::pair<AAX_CParamID const, const AAX_IParameterValue*> TParamValPair;
	
	/** @name Real-time functions
	 * Virtual functions called on the real-time thread
	 */
	//@{
	/** Perform audio render
	 *	
	 *	@param[in,out] ioRenderInfo
	 *	  State data for the current render buffer
	 *	@param[in] inSynchronizedParamValues
	 *	  The parameter values which should be applied for the current render buffer
	 *	  \sa \ref AAX_CMonolithicParameters::AddSynchronizedParameter() "AddSynchronizedParameter"
	 *	@param[in] inNumSynchronizedParamValues
	 *	  The number of parameter values provided in \c inSynchronizedParamValues
	 *
	 */
	virtual void		RenderAudio(AAX_SInstrumentRenderInfo* ioRenderInfo, const TParamValPair* inSynchronizedParamValues[], int32_t inNumSynchronizedParamValues) {}
	//@}end Real-time functions
	
	/** @name Configuration methods
	 */
	//@{
	/** Add a parameter for state synchronization
	 *
	 *	A parameter should be added for synchronization if:
	 *	- It is important for the parameter's automation to be applied at the correct point on the timeline
	 *	- It is possible to quickly update the plug-in's state to reflect a parameter change
	 *	
	 *	See \ref advancedTopics_parameterUpdateTiming for more information
	 *
	 *	@param[in] inParameter
	 *	  The parameter to be synchronized. This string will be copied internally and is not required to persist
	 */
	void				AddSynchronizedParameter(const AAX_IParameter& inParameter);
	//@}
	
public: ///////////////////////////////////////////////////////////////////////////// Convenience Layer Methods
    /** @name Convenience Layer Methods
     *
     *  \note You should not need to override these methods, but if you do, make sure to call into the base class.
     *
     */
	//@{
	// Overrides from \ref AAX_CEffectParameters
	AAX_Result	UpdateParameterNormalizedValue(AAX_CParamID iParamID, double aValue, AAX_EUpdateSource inSource) AAX_OVERRIDE;
	AAX_Result	GenerateCoefficients() AAX_OVERRIDE;
	AAX_Result	ResetFieldData (AAX_CFieldIndex iFieldIndex, void * oData, uint32_t iDataSize) const AAX_OVERRIDE;
	AAX_Result	TimerWakeup() AAX_OVERRIDE;
	/**
	 *	Static description callback
	 *
     *  This method performs all of the basic context setup and pointer passing work
	 *	
	 *	@param[in,out] ioDescriptor
	 *	@param[in] setupInfo
     */
    static	AAX_Result	StaticDescribe (AAX_IEffectDescriptor * ioDescriptor, const AAX_SInstrumentSetupInfo & setupInfo);
	/**
     *  Static RenderAudio (Called by the host)
	 *
	 *	Plug-ins should override \ref AAX_CMonolithicParameters::RenderAudio()
	 *
	 *	@param[in] inInstancesBegin
	 *	@param[in] inInstancesEnd
     */
	static	void	AAX_CALLBACK	StaticRenderAudio(AAX_SInstrumentRenderInfo* const	inInstancesBegin [], const void* inInstancesEnd);	
    //@}end Convenience Layer Methods
	
private:
	// This structure is used on the render thread to set up the contiguous array of TParamValPair* values
	// which is passed to RenderAudio(). The values are drawn from lists of parameter value updates which
	// were queued during GenerateCoefficients()
	struct SParamValList
	{
		// Using 4x the preset queue size: the buffer must be large enough to accommodate the maximum
		// number of updates that we expect to be queued between/before executions of the render callback.
		// The maximum queuing that will likely ever occur is during a preset change (i.e. a call to
		// SetChunk()), in which updates to all parameters may be queued in the same state frame. It is
		// possible that the host would call SetChunk() on the plug-in more than once before the render
		// callback executes, but probably not more than 2-3x. Therefore 4x seems like a safe upper limit
		// for the capacity of this buffer.
		static const int32_t sCap = 4*kSynchronizedParameterQueueSize;
		
		TParamValPair* mElem[sCap];
		int32_t mSize;
		
		SParamValList()
		{
			Clear();
		}
		
		void Add(TParamValPair* inElem)
		{
			AAX_ASSERT(sCap > mSize);
			if (sCap > mSize)
			{
				mElem[mSize++] = inElem;
			}
		}
		
		void Append(const SParamValList& inOther)
		{
			AAX_ASSERT(sCap >= mSize + inOther.mSize);
			for (int32_t i = 0; i < inOther.mSize; ++i)
			{
				Add(inOther.mElem[i]);
			}
		}
		
		void Append(const std::list<TParamValPair*>& inOther)
		{
			AAX_ASSERT(sCap >= mSize + (int64_t)inOther.size());
			for (std::list<TParamValPair*>::const_iterator iter = inOther.begin(); iter != inOther.end(); ++iter)
			{
				Add(*iter);
			}
		}
		
		void Merge(AAX_IPointerQueue<TParamValPair>& inOther)
		{
			do
			{
				TParamValPair* const val = inOther.Pop();
				if (NULL == val) { break; }
				Add(val);
			} while (1);
		}
		
		void Clear() ///< Zeroes out the mElem array; does not destroy any elements
		{
			std::memset(mElem, 0x0, sizeof(mElem));
			mSize = 0;
		}
	};
	
	typedef std::set<const AAX_IParameter*> TParamSet;
	typedef std::pair<int64_t, std::list<TParamValPair*> > TNumberedParamStateList;
	typedef AAX_CAtomicQueue<TNumberedParamStateList, 256> TNumberedStateListQueue;
	typedef AAX_CAtomicQueue<const TParamValPair, 16*kSynchronizedParameterQueueSize> TParamValPairQueue;
	
	
	SParamValList GetUpdatesForState(int64_t inTargetStateNum);
	void DeleteUsedParameterChanges();
	
private:
	std::set<std::string>					mSynchronizedParameters;
	int64_t									mStateCounter;
	TParamSet								mDirtyParameters;
	TNumberedStateListQueue 				mQueuedParameterChanges;
	TNumberedStateListQueue 				mFinishedParameterChanges; // Parameter changes ready for deletion
	TParamValPairQueue						mFinishedParameterValues; // Parameter values ready for deletion
};

	



#endif // AAX_CMONOLITHICPARAMETERS_H
