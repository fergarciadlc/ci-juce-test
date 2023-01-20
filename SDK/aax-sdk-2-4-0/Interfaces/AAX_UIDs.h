/*================================================================================================*/
/*
 *
 *	Copyright 2013-2017, 2019-2021 by Avid Technology, Inc.
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
 *	\file  AAX_UIDs.h
 *
 *	\brief Unique identifiers for AAX/ACF interfaces
 *
 */ 
/*================================================================================================*/

/// @cond ignore
#ifndef AAX_UIDS_H
#define AAX_UIDS_H
/// @endcond

#include "acfbasetypes.h"
#include "defineacfuid.h"

// Pull in the declarations of all standard ACF UIDs
#include "acfuids.h"


/** @name %AAX Host interface IDs
 *
 */
//@{
/// ACF component ID for \ref AAX_IHostServices components
DEFINE_ACFUID(acfIID, AAXCompID_HostServices, 0x88882c2d, 0xebbc, 0x42ef, 0xc0, 0xab, 0x89, 0x81, 0xb0, 0xbd, 0x0c, 0xca);
/// ACF interface ID for \ref AAX_IACFHostServices
DEFINE_ACFUID(acfIID, IID_IAAXHostServicesV1, 0x96d42c2d, 0xebbc, 0x41ef, 0xb0, 0xab, 0x99, 0x91, 0xa0, 0xed, 0x0c, 0xca);
/// ACF interface ID for \ref AAX_IACFHostServices_V2
DEFINE_ACFUID(acfIID, IID_IAAXHostServicesV2, 0xa207ee9e, 0xb442, 0x11e4, 0xa7, 0x1e, 0x12, 0xe3, 0xf5, 0x12, 0xa3, 0x38);
/// ACF interface ID for \ref AAX_IACFHostServices_V3
DEFINE_ACFUID(acfIID, IID_IAAXHostServicesV3, 0x12bea399, 0x9a4f, 0x4353, 0x80, 0x98, 0x39, 0x16, 0xfa, 0x71, 0x89, 0x8d);

/// ACF component ID for \ref AAX_ICollection components
DEFINE_ACFUID(acfIID, AAXCompID_AAXCollection, 0x89882c2d, 0x77bc, 0x42ef, 0x70, 0x7b, 0x79, 0x81, 0xb7, 0xbd, 0x0c, 0xca);
/// ACF interface ID for \ref AAX_IACFCollection
DEFINE_ACFUID(acfIID, IID_IAAXCollectionV1, 0x96d42c2d, 0xebbc, 0x41df, 0xb1, 0xab, 0x99, 0x91, 0xa2, 0xee, 0x0c, 0xca);

/// ACF component ID for \ref AAX_IEffectDescriptor components
DEFINE_ACFUID(acfIID, AAXCompID_AAXEffectDescriptor, 0x89872c2d, 0x75bc, 0x423f, 0x40, 0x1b, 0xf9, 0xa1, 0xba, 0xad, 0x0c, 0xca);
/// ACF interface ID for \ref AAX_IACFEffectDescriptor
DEFINE_ACFUID(acfIID, IID_IAAXEffectDescriptorV1, 0x96d42c2d, 0xebbc, 0x41ef, 0xd1, 0xcb, 0x49, 0x94, 0x42, 0xe4, 0x0f, 0xda);
/// ACF interface ID for \ref AAX_IACFEffectDescriptor_V2
DEFINE_ACFUID(acfIID, IID_IAAXEffectDescriptorV2, 0x41eccc52, 0x416b, 0x4072, 0x84, 0xbd, 0x40, 0xb0, 0x52, 0x10, 0xa7, 0x4c);

/// ACF component ID for \ref AAX_IComponentDescriptor components
DEFINE_ACFUID(acfIID, AAXCompID_AAXComponentDescriptor, 0x94872c3d, 0x95bc, 0x413d, 0xd0, 0xdb, 0xd9, 0xb1, 0x2a, 0xad, 0x0c, 0xca);
/// ACF interface ID for \ref AAX_IACFComponentDescriptor
DEFINE_ACFUID(acfIID, IID_IAAXComponentDescriptorV1, 0x96e42c2d, 0xe2bc, 0x51ef, 0x61, 0xc7, 0x48, 0x99, 0x4a, 0xeb, 0x0c, 0xda);
/// ACF interface ID for \ref AAX_IACFComponentDescriptor_V2
DEFINE_ACFUID(acfIID, IID_IAAXComponentDescriptorV2, 0x1895259e, 0xaaa9, 0x4f0f, 0xa9, 0x85, 0x14, 0x98, 0x37, 0xb7, 0x6f, 0x89);
/// ACF interface ID for \ref AAX_IACFComponentDescriptor_V3
DEFINE_ACFUID(acfIID, IID_IAAXComponentDescriptorV3, 0x979cfcd4, 0x2bcb, 0x43ae, 0x9c, 0xd0, 0x2d, 0x0e, 0xcd, 0x2a, 0xdc, 0xd5);


/// ACF component ID for \ref AAX_IPropertyMap components
DEFINE_ACFUID(acfIID, AAXCompID_AAXPropertyMap, 0xa587ad3d, 0xd53c, 0x4adc, 0xd0, 0xdd, 0xd9, 0xd1, 0x2d, 0xdd, 0xdc, 0xda);
/// ACF interface ID for \ref AAX_IACFPropertyMap
DEFINE_ACFUID(acfIID, IID_IAAXPropertyMapV1, 0x96ee2c2d, 0xeecc, 0x5eff, 0xe2, 0xd7, 0xe8, 0x49, 0xe3, 0xe2, 0xee, 0xee);
/// ACF interface ID for \ref AAX_IACFPropertyMap_V2
DEFINE_ACFUID(acfIID, IID_IAAXPropertyMapV2, 0x7177df80, 0x7c9c, 0x11e2, 0xb9, 0x2a, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66);
/// ACF interface ID for \ref AAX_IACFPropertyMap_V3
DEFINE_ACFUID(acfIID, IID_IAAXPropertyMapV3, 0x6d4ab208, 0xd34b, 0x4368, 0xb4, 0xf1, 0x58, 0xbc, 0x24, 0x3f, 0x45, 0xc9);

/// ACF component ID for \ref AAX_IHostProcessorDelegate components
DEFINE_ACFUID(acfIID, AAXCompID_HostProcessorDelegate, 0xab933d9d, 0x5434, 0x25dc, 0x19, 0x0b, 0x09, 0x23, 0x2d, 0xdd, 0x38, 0x8a);
/// ACF interface ID for \ref AAX_IACFHostProcessorDelegate
DEFINE_ACFUID(acfIID, IID_IAAXHostProcessorDelegateV1, 0x9d4e3d3d, 0x43dc, 0x5eda, 0x82, 0x27, 0xe2, 0xf2, 0xf8, 0xd5, 0x6e, 0x8e);
/// ACF interface ID for \ref AAX_IACFHostProcessorDelegate_V2
DEFINE_ACFUID(acfIID, IID_IAAXHostProcessorDelegateV2, 0xfb6de2c9, 0x29d0, 0x4683, 0xb3, 0x48, 0xc7, 0x78, 0xf4, 0xcd, 0x62, 0x5b);
/// ACF interface ID for \ref AAX_IACFHostProcessorDelegate_V3
DEFINE_ACFUID(acfIID, IID_IAAXHostProcessorDelegateV3, 0x5dfef2b3, 0x7027, 0x46b5, 0xae, 0x3a, 0x27, 0x94, 0xc6, 0xe0, 0xa8, 0xa0);

/// ACF component ID for \ref AAX_IAutomationDelegate components
DEFINE_ACFUID(acfIID, AAXCompID_AutomationDelegate, 0xab943d9d, 0x5534, 0x26dc, 0x29, 0xab, 0xc9, 0xb3, 0x2d, 0x2d, 0x28, 0x8a);
/// ACF interface ID for \ref AAX_IACFAutomationDelegate
DEFINE_ACFUID(acfIID, IID_IAAXAutomationDelegateV1, 0x9d5e3d3d, 0x42dc, 0x5efa, 0x22, 0x17, 0xee, 0xe2, 0xe8, 0xe5, 0x3e, 0x2e);

/// ACF component ID for \ref AAX_IController components
DEFINE_ACFUID(acfIID, AAXCompID_Controller, 0xab944d4d, 0x15c4, 0xc61c, 0x3d, 0x3b, 0xf9, 0xbf, 0x1d, 0x20, 0x18, 0x4a);
/// ACF interface ID for \ref AAX_IACFController
DEFINE_ACFUID(acfIID, IID_IAAXControllerV1, 0x9d5e3e3d, 0x52dc, 0x5efb, 0x20, 0x18, 0xde, 0x1d, 0xe2, 0xe6, 0x3f, 0x4e);
/// ACF interface ID for \ref AAX_IACFController_V2
DEFINE_ACFUID(acfIID, IID_IAAXControllerV2, 0x4c59aa0e, 0xd7c0, 0x4205, 0x8b, 0x6c, 0x32, 0x46, 0x8d, 0x42, 0xd2, 0x02);
/// ACF interface ID for \ref AAX_IACFController_V3
DEFINE_ACFUID(acfIID, IID_IAAXControllerV3, 0xdd6f168c, 0xda86, 0x44f8, 0xb8, 0x64, 0xd6, 0xcd, 0x22, 0x19, 0x26, 0xe7);

/// ACF component ID for %AAX page table controller components
DEFINE_ACFUID(acfIID, AAXCompID_PageTableController, 0x63355d80, 0xbfe1, 0x4291, 0xa6, 0x27, 0xc6, 0x5c, 0xb9, 0x58, 0x91, 0x40);
/// ACF interface ID for \ref AAX_IACFPageTableController
DEFINE_ACFUID(acfIID, IID_IAAXPageTableController, 0x2e9d35fb, 0xbacc, 0x4b2c, 0xb5, 0xd7, 0xc6, 0xe8, 0x51, 0xf5, 0x69, 0xbd);
/// ACF interface ID for \ref AAX_IACFPageTableController_V2
DEFINE_ACFUID(acfIID, IID_IAAXPageTableControllerV2, 0x6c6b83e, 0x9d87, 0x4938, 0x8a, 0x38, 0xf8, 0xe4, 0x5b, 0x10, 0xa2, 0x4a);

/// ACF component ID for \ref AAX_IPrivateDataAccess components
DEFINE_ACFUID(acfIID, AAXCompID_PrivateDataAccess, 0xab945d4d, 0x15c6, 0xc61c, 0x3f, 0x3f, 0xf9, 0xbf, 0x1d, 0x20, 0x18, 0x4c);
/// ACF interface ID for \ref AAX_IACFPrivateDataAccess
DEFINE_ACFUID(acfIID, IID_IAAXPrivateDataAccessV1, 0x9d5e6e3f, 0x52de, 0x5efd, 0x22, 0x18, 0xdf, 0x1f, 0xe3, 0xe8, 0x3f, 0x4c);

/// ACF component ID for \ref AAX_IViewContainer components
DEFINE_ACFUID(acfIID, AAXCompID_ViewContainer, 0xdede24bd, 0xc2ff, 0x467a, 0xae, 0x2d, 0x5f, 0x29, 0x1d, 0x19, 0x22, 0x2b);
/// ACF interface ID for \ref AAX_IACFViewContainer
DEFINE_ACFUID(acfIID, IID_IAAXViewContainerV1, 0x22da0bbc, 0xd550, 0x4d5e, 0x8c, 0xc6, 0x73, 0x44, 0x83, 0xb8, 0x83, 0x7f);
/// ACF interface ID for \ref AAX_IACFViewContainer_V2
DEFINE_ACFUID(acfIID, IID_IAAXViewContainerV2, 0x9143a0be, 0x7a79, 0x4d02, 0xae, 0x25, 0xaa, 0xdb, 0xa7, 0x6a, 0x50, 0xb2);

/// ACF component ID for \ref AAX_ITransport components
DEFINE_ACFUID(acfIID, AAXCompID_Transport, 0x8a9fa236, 0x2176, 0x49e1, 0xb6, 0x24, 0x82, 0x7d, 0x2b, 0x43, 0x31, 0x5c);
/// ACF interface ID for \ref AAX_IACFTransport
DEFINE_ACFUID(acfIID, IID_IAAXTransportV1, 0x5cee4ef4, 0x6337, 0x4359, 0xb6, 0x3b, 0xfe, 0x58, 0xdc, 0x36, 0x54, 0x3a);
/// ACF interface ID for \ref AAX_IACFTransport_V2
DEFINE_ACFUID(acfIID, IID_IAAXTransportV2, 0x203cbd9f, 0x982c, 0x4fe6, 0xa8, 0x27, 0x7, 0x48, 0x2, 0x57, 0xae, 0xc3);
/// ACF interface ID for \ref AAX_IACFTransport_V3
DEFINE_ACFUID(acfIID, IID_IAAXTransportV3, 0xaf79e815, 0xecfe, 0x1fb4, 0x8a, 0x2e, 0x24, 0xab, 0x0e, 0xc0, 0x8e, 0xf0);


/// ACF component ID for \ref AAX_IPageTable components
DEFINE_ACFUID(acfIID, AAXCompID_PageTable, 0xdbc22879, 0xa24e, 0x4ac6, 0x97, 0x21, 0x93, 0x8b, 0x72, 0xd8, 0xe8, 0x1b);
/// ACF interface ID for \ref AAX_IACFPageTable
DEFINE_ACFUID(acfIID, IID_IAAXPageTableV1, 0x33c9e5be, 0x1ce3, 0x4085, 0x91, 0xa7, 0x09, 0xd6, 0xf8, 0xee, 0x4b, 0x64);
/// ACF interface ID for \ref AAX_IACFPageTable_V2
DEFINE_ACFUID(acfIID, IID_IAAXPageTableV2, 0xd0f25d1b, 0x9c5b, 0x4d2e, 0x8f, 0x1f, 0x45, 0xbc, 0x93, 0x47, 0x32, 0xf7);


/// ACF component ID for \ref AAX_IDescriptionHost components
DEFINE_ACFUID(acfIID, AAX_CompID_DescriptionHost, 0x84e184ce, 0x353c, 0x4928, 0x80, 0x61, 0x04, 0x60, 0x06, 0xb3, 0x1b, 0x52);
/// ACF interface ID for \ref AAX_IACFDescriptionHost
DEFINE_ACFUID(acfIID, IID_IAAXDescriptionHostV1, 0xe5bc71df, 0x4c1f, 0x4cc4, 0x81, 0x4a, 0x5a, 0x7d, 0xd0, 0xe7, 0x0e, 0xf5);

/// ACF component ID for \ref AAX_IFeatureInfo components
DEFINE_ACFUID(acfIID, AAX_CompID_FeatureInfo, 0x617d2e4f, 0x3556, 0x483b, 0xb4, 0xde, 0x05, 0x3c, 0xc3, 0x92, 0x17, 0x53);
/// ACF interface ID for \ref AAX_IACFFeatureInfo
DEFINE_ACFUID(acfIID, IID_IAAXFeatureInfoV1, 0x24545609, 0xa7c4, 0x44d4, 0xab, 0xb8, 0xcf, 0x13, 0xea, 0x9d, 0x0b, 0xdf);
//@}end AAX host interface IDs

/** @name %AAX plug-in interface IDs
 *
 */
//@{
/// ACF component ID for \ref AAX_IEffectParameters components
DEFINE_ACFUID(acfIID, AAXCompID_EffectParameters, 0xab97bd9d, 0x9b3c, 0x4bdc, 0xb9, 0x9b, 0x59, 0x51, 0xbd, 0x5d, 0x48, 0x4a);
/// ACF interface ID for \ref AAX_IACFEffectParameters
DEFINE_ACFUID(acfIID, IID_IAAXEffectParametersV1, 0x964e333d, 0x334c, 0x533f, 0xc2, 0xc7, 0x38, 0x34, 0xc3, 0xc2, 0x3e, 0x3e);
/// ACF interface ID for \ref AAX_IACFEffectParameters_V2
DEFINE_ACFUID(acfIID, IID_IAAXEffectParametersV2, 0xf1f47d06, 0x308f, 0x4cc5, 0x9c, 0x7c, 0x50, 0xa8, 0x3f, 0x8a, 0xb8, 0x13);
/// ACF interface ID for \ref AAX_IACFEffectParameters_V3
DEFINE_ACFUID(acfIID, IID_IAAXEffectParametersV3, 0xd2540e9d, 0x9163, 0x42bb, 0xa6, 0xfd, 0x81, 0xe1, 0xe,  0xa3, 0x24, 0x98);
/// ACF interface ID for \ref AAX_IACFEffectParameters_V4
DEFINE_ACFUID(acfIID, IID_IAAXEffectParametersV4, 0x2e485536, 0x31a3, 0x4697, 0x9c, 0x16, 0xe5, 0x9b, 0xf6, 0xb2, 0x8a, 0x41);

/// ACF component ID for \ref AAX_IHostProcessor components
DEFINE_ACFUID(acfIID, AAXCompID_HostProcessor, 0xab953d9d, 0x5b34, 0x45dc, 0x49, 0x3b, 0x29, 0x53, 0xcd, 0xdd, 0x48, 0x4a);
/// ACF interface ID for \ref AAX_IACFHostProcessor
DEFINE_ACFUID(acfIID, IID_IAAXHostProcessorV1, 0x964e3f3d, 0x434c, 0x5e3a, 0xa2, 0xe7, 0xe8, 0xf4, 0xf3, 0xd2, 0x2e, 0x2e);
/// ACF interface ID for \ref AAX_IACFHostProcessor_V2
DEFINE_ACFUID(acfIID, IID_IAAXHostProcessorV2, 0x457546c0, 0xf6bc, 0x4af9, 0xbf, 0xf7, 0xeb, 0xdd, 0xc0, 0x5e, 0x56, 0xde);

/// ACF component ID for \ref AAX_IEffectGUI components
DEFINE_ACFUID(acfIID, AAXCompID_EffectGUI, 0xab94339d, 0x3b34, 0x35dc, 0x29, 0x32, 0x19, 0x23, 0x1d, 0x1d, 0x48, 0x2a);
/// ACF interface ID for \ref AAX_IACFEffectGUI
DEFINE_ACFUID(acfIID, IID_IAAXEffectGUIV1, 0x964e323d, 0x424c, 0x5e1a, 0x22, 0x27, 0x28, 0x24, 0x23, 0x22, 0x2e, 0x1e);

/// ACF component ID for \ref AAX_IEffectDirectData components
DEFINE_ACFUID(acfIID, AAXCompID_EffectDirectData, 0xaafe80ab, 0x5b34, 0x4522, 0x49, 0x3b, 0x29, 0x53, 0xcd, 0xdd, 0x48, 0x4b);
/// ACF interface ID for \ref AAX_IACFEffectDirectData
DEFINE_ACFUID(acfIID, IID_IAAXEffectDirectDataV1, 0x964e80ab, 0x434c, 0x5e22, 0xa2, 0xe7, 0xe8, 0xf4, 0xf3, 0xd2, 0x2e, 0x2f);
// ACF interface ID for \ref AAX_IACFEffectDirectData_V2
DEFINE_ACFUID(acfIID, IID_IAAXEffectDirectDataV2, 0x156ea622, 0xbd2e, 0x11e9, 0x9c, 0xb5, 0x2a, 0x2a, 0xe2, 0xdb, 0xcc, 0xe4);

//@}end AAX plug-in interface IDs



/** @name %AAX Feature UIDs
 *
 */
//@{

/** Identifier for %AAX features
 
 See \ref AAX_IDescriptionHost::AcquireFeatureProperties() and \ref AAX_IFeatureInfo
 */
typedef acfUID AAX_Feature_UID;

/** \var AAXATTR_ClientFeature_StemFormat
 
 \brief Client stem format feature support
 
 \details
 To determine the client's support for specific stem formats, use the property map
 
 <b>Property map contents</b>
 Key: \ref AAX_EStemFormat values
 Value: \ref AAX_ESupportLevel value; if undefined then no information is available
 */
DEFINE_ACFUID(AAX_Feature_UID, AAXATTR_ClientFeature_StemFormat, 0x729dd3e6, 0xd3dc, 0x484c, 0x91, 0x69, 0xf0, 0x64, 0xa0, 0x12, 0x60, 0x1d);

/** \var AAXATTR_ClientFeature_AuxOutputStem
 
 \brief Client \ref additionalFeatures_AOS "Auxiliary Output Stem" feature support
 
 Plug-ins must detect when a host does not support AOS in order to avoid running off the end of the output audio buffer list in the
 audio algorithm.
 
 \ref AAX_IComponentDescriptor::AddAuxOutputStem() "AddAuxOutputStem()" will return an error for hosts that do not support this feature, so typically
 a feature support query using this \ref AAX_Feature_UID is not required.
 */
DEFINE_ACFUID(AAX_Feature_UID, AAXATTR_ClientFeature_AuxOutputStem, 0x5bea3f7a, 0x2be8, 0x4fe1, 0x83, 0xb2, 0x94, 0xec, 0x91, 0x31, 0xb8, 0x52);

/** \var AAXATTR_ClientFeature_AuxOutputStem
 
 \brief Client \ref additionalFeatures_Sidechain "Side Chain" feature support
 */
DEFINE_ACFUID(AAX_Feature_UID, AAXATTR_ClientFeature_SideChainInput, 0x98b0a514, 0x2b96, 0x4e1f, 0x87, 0x81, 0x99, 0x08, 0xc9, 0xe3, 0xe6, 0x8b);

/** \var AAXATTR_ClientFeature_MIDI
 
 \brief Client \ref additionalFeatures_MIDI "MIDI" feature support
 */
DEFINE_ACFUID(AAX_Feature_UID, AAXATTR_ClientFeature_MIDI, 0xf5b0816c, 0x5768, 0x49c2, 0xae, 0x3e, 0x85, 0x0d, 0xe3, 0x42, 0xeb, 0x07);

//@}end AAX Feature UIDs


/** @name %AAX host attributes
 *
 */
//@{

/** \var AAXATTR_Client_Level
 
 \brief Client application level
 
 \details
 Type: \c uint32_t (\c ACFTypeID_UInt32)
 Value: one of \ref AAX_EHostLevel
 
 Query using the host's \ref IACFDefinition
 */
DEFINE_ACFUID(acfUID, AAXATTR_Client_Level, 0xe550868e, 0x1e6a, 0x482b, 0xb5, 0x86, 0x73, 0xf1, 0x24, 0x6e, 0x12, 0x6b);

//@}end AAX host attributes



/// @cond ignore
#endif
/// @endcond
