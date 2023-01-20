//
/*
	AAXHostShell_Operations.h
	AAXHostShell
	
	Created by Rob Majors on 2/1/14.
	Copyright 2014 by Avid Technology, Inc.
*/

#ifndef AAXHostShell_AAXHostShell_Operations_h
#define AAXHostShell_AAXHostShell_Operations_h

/** @file
 
 @brief Header for the AAXHostShell direct operation API
 */

/** @defgroup AAXHostShell_Operations AAXHostShell direct operations
 @ingroup AAXHostShell_API
 
 @brief Operations that can be performed on the AAXHostShell
 
 @details
 <B>Conforms to the conventions of the @ref AAXHostShell_API </B>
 
 This group of AAXHostShell API methods is comprised of "operations". Each operation represents
 a single action that may be performed by the framework.
 */

/* AAXHostShell Includes */
#include "AAXHostShell_API.h"
#include "AAXHostShell_Types.h"
/* C99 Includes */
#include <stdint.h>
#include <stddef.h>

/* --------------------------------------------------------------------------- */
#pragma mark - Environment
/** @name Environment operations
 
 @details
 These methods are dependent on the application environment
 */
/**@{*/
/** @brief Set the current host context
 
 @details
 @sa AAXHShell_InstantiateEffect()
 
 @param[in]		inHostContext
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult)  AAXHShell_SetHostContext(const AAXHShell_SHostContext* inHostContext);
/** @brief Get the current host context
 
 @details
 @param[out]	outHostContext
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetHostContext(AAXHShell_SHostContext* outHostContext);
/** @brief Set the current debug level
 
 @details
 By default, the debug level will be \ref kAAXHShell_DebugBits_Default
 
 @sa AAXHShell_InstantiateEffect()
 
 @param[in]		inDebugBits
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_SetDebugLevel(int32_t inDebugBits);
/** @brief Get the current debug level
 
 @details
 @param[out]	outDebugBits
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetDebugLevel(int32_t* outDebugBits);
/** @brief Perform main-thread idle tasks
 
 @details
 To satisfy the AAX host threading specification, this method must be called from the main application thread
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(void) AAXHShell_DoMainThreadIdle();
/** @brief Copy a C string representing a system path to the default AAX plug-ins directory
 
 @details
 @param[out]	outDefaultPlugInsDirectoryCString
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 @param[in]		inSize
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetDefaultPlugInsDirectory(char* outDefaultPlugInsDirectoryCString, int32_t inSize);
/**@}end Environment */


/* --------------------------------------------------------------------------- */
#pragma mark - Macro operations
/** @name Macro operations
 
 @details
 These convenience methods batch together a series of operations to perform one larger task
 */
/**@{*/
/** @brief Find and instantiate the first AAX Native effect in a plug-in
 
 @details
 @param[in]		inID
 @param[out]	outInstanceID
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_InstatiateFirstNativeEffect(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID* outInstanceID);
/**@}end Macro operations */


/* --------------------------------------------------------------------------- */
#pragma mark - Plug-in
/** @name Plug-in operations
 */
/**@{*/
/** @brief Load a plug-in bundle from the filesystem
 
 @details
 @sa AAXHShell_TPlugInID
 
 @param[in]		inPlugInPathCString
 @param[out]	outID
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_LoadPlugIn(const char* inPlugInPathCString, AAXHShell_TPlugInID* outID);
/** @brief Unload a plug-in bundle
 
 @details
 <b>Precondition</b>
 Once an instance of an effect within a plug-in has been created, the plug-in must remain loaded until all of
 its effects' instances have been removed.
 
 @sa AAXHShell_TPlugInID
 
 @param[in]		inID
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_UnloadPlugIn(AAXHShell_TPlugInID inID);
/** @brief Get a list of all loaded plug-ins
 
 @details
 If the full array of plug-in IDs cannot fit into @c inSize bytes then this method will return @c false and
 no output parameters will be changed.
 
 @sa AAXHShell_TPlugInID
 
 @param[out]	outIDs
 An array of @ref AAXHShell_TPlugInID elements no longer than @c inSize bytes
 @param[out]	outNumElems
 The number of @ref AAXHShell_TPlugInID elements in \c outIDs
 @param[in]		inSize
 The size of the pre-allocated @c outIDs buffer, in bytes
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetLoadedPlugIns(AAXHShell_TPlugInID outIDs[], int32_t* outNumElems, int32_t inSize);
/** @brief Get the system file path to a loaded plug-in
 
 @details
 If the full path string cannot fit into @c inSize bytes then this method will return @c false and
 no output parameters will be changed.
 
 @sa AAXHShell_TPlugInID
 
 @param[in]		inID
 @param[out]	outPlugInPathCString
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 @param[in]		inSize
 The size of the pre-allocated @c outPlugInPathCString buffer, in bytes
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetPlugInPath(AAXHShell_TPlugInID inID, char* outPlugInPathCString, int32_t inSize);
/** @brief Get the Manufacturer Name
 
 @details
 If the full path string cannot fit into @c inSize bytes then this method will return @c false and
 no output parameters will be changed.
 
 @sa AAXHShell_TPlugInID
 
 @param[in]		inID
 @param[out]	outManufacturerName
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 @param[in]		inSize
 The size of the pre-allocated @c outPlugInPathCString buffer, in bytes
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetManufacturerName(AAXHShell_TPlugInID inID, char* outManufacturerName, int32_t inSize);
/** @brief Get the package (bundle) Name
 
 @details
 If the full path string cannot fit into @c inSize bytes then this method will return @c false and
 no output parameters will be changed.
 
 @sa AAXHShell_TPlugInID
 
 @param[in]		inID
 @param[out]	outPakageName
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 @param[in]		inSize
 The size of the pre-allocated @c outPlugInPathCString buffer, in bytes
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetPackageName(AAXHShell_TPlugInID inID, char* outPakageName, int32_t inSize);


/** @brief Get validity information for a plug-in's description
 
 @details
 This method scans the complete plug-in description and caches information about any warnings or
 errors that are encountered. To retrieve information about specific warnings or errors, call
 @ref AAXHShell_GetDescriptionValidationMessage
 
 @param[in]		inID
 @param[in]		inResultType
 Type of string to retrieve. Must be either @ref kAAXHShell_ResultType_Warning or @ref kAAXHShell_ResultType_Error
 @param[out]	outNumResults
 The number of results of the given type that were encountered
 @param[out]	outResultMessageCStringMaxSize
 The size of the longest result message c-string, in bytes. This parameter may be NULL.
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetDescriptionValidationInfo(AAXHShell_TPlugInID inID, int32_t inResultType, int32_t* outNumResults, int32_t* outResultMessageCStringMaxSize);
/** @brief Get a description validation warning or error string
 
 @details
 @ref AAXHShell_GetDescriptionValidationInfo must be called before this method.
 
 @param[in]		inID
 @param[in]		inResultType
 Type of string to retrieve. Must be one of @ref kAAXHShell_ResultType_Success, @ref kAAXHShell_ResultType_Warning, or @ref kAAXHShell_ResultType_Error
 @param[in]		inIndex
 Index of the string to retrieve. An error is returned if this value is too large.
 @param[out]	outResultCode
 An @c AAX_Result value
 @param[out]	outResultMessageCString
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 @param[in]		inSize
 The size of the pre-allocated @c outMessageCString buffer, in bytes
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetDescriptionValidationMessage(AAXHShell_TPlugInID inID, int32_t inResultType, int32_t inIndex, int32_t* outResultCode, char* outResultMessageCString, int32_t inSize);
/**@}end Plug-in operations*/


/* --------------------------------------------------------------------------- */
#pragma mark - GUI
/** @name GUI operations
 */
/**@{*/
/** @brief Create the Effect GUI module for a plug-in instance
 
 @details
 @param[in]		inID
 @param[in]		inInstanceID
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_CreateEffectGUI(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID);
/** @brief Delete a plug-in instance's Effect GUI module
 
 @details
 @param[in]		inID
 @param[in]		inInstanceID
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_DeleteEffectGUI(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID);
/** @brief Create an AAX view container and register it with the AAXHostShell framework
 
 @details
 @sa AAXHShell_TViewID
 
 @details
 @param[in]		inViewPtr
 @param[in]		outViewID
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_CreateViewContainer(void* inViewPtr, AAXHShell_TViewID* outViewID);
/** @brief Delete an AAX view container and unregister it from the AAXHostShell framework
 
 @details
 <b>Precondition</b>
 If the view container has been associated with an effect instance then the client must
 disassociate the view container via @ref AAXHShell_RemoveEffectInstanceView() before deleting the view
 container.
 
 @sa AAXHShell_TViewID
 
 @param[in]		inViewID
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_DeleteViewContainer(AAXHShell_TViewID inViewID);
/** @brief Associate a registered view container with an specific effect instance
 
 @details
 @param[in]		inID
 @param[in]		inInstanceID
 @param[in]		inViewID
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_SetEffectInstanceView(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, AAXHShell_TViewID inViewID);
/** @brief Disassociate a registered view container from an specific effect instance
 
 @details
 <b>Precondition</b>
 If the view container has been set on the effect instance's Effect GUI module then the client
 must remove the view container via @ref AAXHShell_RemoveViewFromEffectGUI() before disassocating
 the view container from the instance.
 
 @param[in]		inID
 @param[in]		inInstanceID
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_RemoveEffectInstanceView(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID);
/** @brief Set an effect instance's associated view container on its Effect GUI module
 
 @details
 @param[in]		inID
 @param[in]		inInstanceID
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_SetCurrentViewOnEffectGUI(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID);
/** @brief Remove an effect instance's view container
 
 @details
 <b>Precondition</b>
 If the instance's Effect GUI module is currently running then the client must pause the Effect
 GUI via @ref AAXHShell_StopEffectGUI() before removing the instance's view container.
 
 @param[in]		inID
 @param[in]		inInstanceID
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_RemoveViewFromEffectGUI(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID);
/** @brief Begin idle callbacks and event handling for an instance's Effect GUI module
 
 @details
 @param[in]		inID
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_RunEffectGUI(AAXHShell_TPlugInID inID);
/** @brief Pause idle callbacks and event handling for an instance's Effect GUI module
 
 @details
 @param[in]		inID
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_StopEffectGUI(AAXHShell_TPlugInID inID);
/**@}end GUI operations */


/* --------------------------------------------------------------------------- */
#pragma mark - Effect data retrieval
/** @name Effect data retrieval operations
 
 @details
 An effect represents a single user-facing "product"
 */
/**@{*/
/** @brief Get the number of effects in a plug-in bundle
 
 @details
 @param[in]		inID
 @param[out]	outNumEffects
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetNumberOfEffects(AAXHShell_TPlugInID inID, int32_t* outNumEffects);
/** @brief Get an effect's ID as a C string
 
 @details
 If the provided char buffer is too small then this returns false
 
 @param[in]		inID
 @param[in]		inEffectIndex
 @param[out]	outEffectID
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 @param[in]		inSize
 The size of the pre-allocated @c outEffectID buffer, in bytes.
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectIDByIndex(AAXHShell_TPlugInID inID, int32_t inEffectIndex, char* outEffectID, int32_t inSize);
/** @brief Get an effect's name as a C string
 
 @details
 An appropriate abbreviated name will be given, if it has been provided by the plug-in,
 depending on the value of \c inSize. The longest available abbreviated name which will
 fit into the buffer will be used. If the shortest available abbreviated name is too
 long to fit into the buffer then it will be truncated to fit.
 
 If the provided char buffer is too small then this returns false
 
 @param[in]		inID
 @param[in]		inEffectIndex
 @param[out]	outEffectName
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 @param[in]		inSize
 The size of the pre-allocated @c outEffectID buffer, in bytes.
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectNameByIndex(AAXHShell_TPlugInID inID, int32_t inEffectIndex, char* outEffectName, int32_t inSize);
/** @brief Determine whether a particular effect supports the current host context
 
 @details
 @warning Currently the host context input and output stem formats are ignored
 
 @sa AAXHShell_SetHostContext()
 
 @param[in]		inID
 @param[in]		inEffectIndex
 @param[out]	outItDoes
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectSupportsCurrentHostContext(AAXHShell_TPlugInID inID, int32_t inEffectIndex, AAXHShell_TBoolean* outItDoes);
/** @brief Determine whether a particular effect supports a given host context
 
 @details
 @warning Currently the host context input and output stem formats are ignored
 
 @param[in]		inID
 @param[in]		inEffectIndex
 @param[in]		inHostContext
 @param[out]	outItDoes
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectSupportsHostContext(AAXHShell_TPlugInID inID, int32_t inEffectIndex, const AAXHShell_SHostContext* inHostContext, AAXHShell_TBoolean* outItDoes);
/** @brief Determine whether a particular effect supports Native host contexts
 
 @details
 @param[in]		inID
 @param[in]		inEffectIndex
 @param[out]	outItDoes
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectSupportsNative(AAXHShell_TPlugInID inID, int32_t inEffectIndex, AAXHShell_TBoolean* outItDoes);
/** @brief Determine whether a particular effect supports TI DSP host contexts
 
 @details
 @param[in]		inID
 @param[in]		inEffectIndex
 @param[out]	outItDoes
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectSupportsTI(AAXHShell_TPlugInID inID, int32_t inEffectIndex, AAXHShell_TBoolean* outItDoes);
/** @brief Returns a property for an effect
 
 @details
 The expected behavior if the property is defined but has the wrong size 
 (function returns true and sets outHasProperty to zero)
 @sa @ref AAXHShell_GetEffectTypeProperty @ref AAXHShell_GetEffectProperty64
 
 @param[in]		inID
 @param[in]		inEffectIndex
 @param[in]		inProperty
 @param[out]	outHasProperty
 Set to 0x0 if the effect does not include the specified property
 @param[out]	outValue
 Left unchanged if the effect does not include the specified property
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectProperty(AAXHShell_TPlugInID inID, int32_t inEffectIndex, int64_t inProperty, AAXHShell_TBoolean* outHasProperty, int32_t* outValue);
/** @brief Returns a property for an effect
 
 @details
 The expected behavior if the property is defined but has the wrong size
 (function returns true and sets outHasProperty to zero)
 @sa @ref AAXHShell_GetEffectTypeProperty @ref AAXHShell_GetEffectProperty
 
 @param[in]		inID
 @param[in]		inEffectIndex
 @param[in]		inProperty
 @param[out]	outHasProperty
 Set to 0x0 if the effect does not include the specified property
 @param[out]	outValue
 Left unchanged if the effect does not include the specified property
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectProperty64(AAXHShell_TPlugInID inID, int32_t inEffectIndex, int64_t inProperty, AAXHShell_TBoolean* outHasProperty, int64_t* outValue);
/** @brief Get the byte size of an effect's description C string
 
 @details
 If the description size is larger than @c INT32_MAX then this returns false
 
 @details
 @sa AAXHShell_GetEffectDescription()
 
 @param[in]		inID
 @param[in]		inEffectIndex
 @param[out]	outSize
 The byte length of the plug-in's description C string (including the null terminating character.)
 
 @ingroup AAXHostShell_Operations
 */

AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectDescriptionSize(AAXHShell_TPlugInID inID, int32_t inEffectIndex, int32_t* outSize);
/** @brief Get an effect's description as a C string
 
 @details
 If the provided char buffer is too small then this returns false
 
 @note The full description string can be very long.
 
 @sa AAXHShell_GetEffectDescriptionSize
 
 @param[in]		inID
 @param[in]		inEffectIndex
 @param[out]	outEffectDescription
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 @param[in]		inSize
 The size of the pre-allocated @c outEffectID buffer, in bytes.
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectDescription(AAXHShell_TPlugInID inID, int32_t inEffectIndex, char* outEffectDescription, int32_t inSize);
/** @brief Get an Processor Ptr with type inProcID
 @details
 If Processor does not support the given \c inProcID then the output ptr will be set to null
 @param[in] inID
 @param[in] inEffectIndex
 @param[in] inProcID
 @param[out] outProcPtr
 The type of outProcPtr is void*
 
 @ingroup AAXHostShell_Operations
*/
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetProcPtr(AAXHShell_TPlugInID inID, int32_t inEffectIndex, AAXHShel_ProcType inProcID, void** outProcPtr);

// Meter data retrieval (needs documentation)
AAXHShell_API(AAXHShell_TResult)  AAXHShell_GetNumMeterIDs(AAXHShell_TPlugInID inID, int32_t inEffectIndex, int32_t* outNumMeters);
AAXHShell_API(AAXHShell_TResult)  AAXHShell_GetMeterID(AAXHShell_TPlugInID inID, int32_t inEffectIndex, int32_t inMeterIndex, uint32_t* outMeterId);
AAXHShell_API(AAXHShell_TResult)  AAXHShell_GetMeterNameForID(AAXHShell_TPlugInID inID, int32_t inEffectIndex, uint32_t inMeterTypeId,char* outMeterName, int32_t inSize);
AAXHShell_API(AAXHShell_TResult)  AAXHShell_GetMeterPropertyByMeterId(AAXHShell_TPlugInID inID, int32_t inEffectIndex, uint32_t inMeterTypeID, int64_t inProperty, AAXHShell_TBoolean* outHasProperty, int32_t* outValue) ;
AAXHShell_API(AAXHShell_TResult)  AAXHShell_GetEffectSupportsGainReductionMetering(AAXHShell_TPlugInID inID, int32_t inEffectIndex, AAXHShell_TBoolean* outItDoes);

/**@} end Effect data retrieval operations */

/* --------------------------------------------------------------------------- */
#pragma mark - Effect type data retrieval
/**@{*/
/** @name Effect type data retrieval operations
 
 @details
 An effect type represents a specific configuration of an effect
 */
/**@{*/
/** @brief Get the number of effect types in a plug-in
 
 @details
 @param[in]		inID
 @param[out]	outNumTypes
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetNumberOfTypes(AAXHShell_TPlugInID inID, int32_t* outNumTypes);
/** @brief Determine whether a particular effect type is Native
 
 @details
 @param[in]		inID
 @param[in]		inTypeIndex
 @param[out]	outItIs
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectTypeIsNative(AAXHShell_TPlugInID inID, int32_t inTypeIndex, AAXHShell_TBoolean* outItIs);
/** @brief Determine whether a particular effect type is AudioSuite
 
 @details
 @param[in]		inID
 @param[in]		inTypeIndex
 @param[out]	outItIs
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectTypeIsAudioSuite(AAXHShell_TPlugInID inID, int32_t inTypeIndex, AAXHShell_TBoolean* outItIs);
/** @brief Determine whether a particular effect type is TI DSP
 
 @details
 @param[in]		inID
 @param[in]		inTypeIndex
 @param[out]	outItIs
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectTypeIsTI(AAXHShell_TPlugInID inID, int32_t inTypeIndex, AAXHShell_TBoolean* outItIs);
/** @brief Get the effect ID (C string) for the effect that corresponds to an effect type
 
 @details
 If the provided char buffer is too small then this returns false
 
 @param[in]		inID
 @param[in]		inTypeIndex
 @param[out]	outEffectID
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 @param[in]		inSize
 The size of the pre-allocated @c outEffectID buffer, in bytes.
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectTypeEffectID(AAXHShell_TPlugInID inID, int32_t inTypeIndex, char* outEffectID, int32_t inSize);
/** @brief Returns a property for an effect type
 
 @details
 The expected behavior if the property is defined but has the wrong size
 (function returns true and sets outHasProperty to zero)
 @sa @ref AAXHShell_GetEffectProperty @ref AAXHShell_GetEffectTypeProperty64
 
 @param[in]		inID
 @param[in]		inTypeIndex
 @param[in]		inProperty
 @param[out]	outHasProperty
 Set to 0x0 if the effect type does not include the specified property
 @param[out]	outValue
 Left unchanged if the effect type does not include the specified property
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectTypeProperty(AAXHShell_TPlugInID inID, int32_t inTypeIndex, int64_t inProperty, AAXHShell_TBoolean* outHasProperty, int32_t* outValue);
/** @brief Returns a property for an effect type
 
 @details
 The expected behavior if the property is defined but has the wrong size
 (function returns true and sets outHasProperty to zero)
 @sa @ref AAXHShell_GetEffectProperty @ref AAXHShell_GetEffectTypeProperty
 
 @param[in]		inID
 @param[in]		inTypeIndex
 @param[in]		inProperty
 @param[out]	outHasProperty
 Set to 0x0 if the effect type does not include the specified property
 @param[out]	outValue
 Left unchanged if the effect type does not include the specified property
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectTypeProperty64(AAXHShell_TPlugInID inID, int32_t inTypeIndex, int64_t inProperty, AAXHShell_TBoolean* outHasProperty, int64_t* outValue);
/** @brief Returns triad for the particular effect type
 
 @details
 @param[in]		inID
 @param[in]		inTypeIndex
 @param[out]	outManufacturerID
 @param[out]	outProductID
 @param[out]	outEffectTypeID
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectTypeTriad(AAXHShell_TPlugInID inID, int32_t inTypeIndex, uint32_t* outManufacturerID, uint32_t* outProductID, uint32_t* outEffectTypeID);
/** @brief Determine whether particular effect types have the same manufaturerID/productID/plugInID
 
 @details
 @param[in]		inIDFirst
 @param[in]		inTypeIndexFirst
 @param[in]		inIDSecond
 @param[in]		inTypeIndexSecond
 @param[out]	outEqual
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectTypeTriadsAreEqual(AAXHShell_TPlugInID inIDFirst, int32_t inTypeIndexFirst, AAXHShell_TPlugInID inIDSecond, int32_t inTypeIndexSecond, AAXHShell_TBoolean* outEqual);
/** @brief Returns name for the particular effect type as a C string
 
 @details
 @param[in]		inID
 @param[in]		inTypeIndex
 @param[out]	outEffectTypeName
 A UTF-8 C-string no longer than @c inSize bytes (including the null terminating character).
 @param[in]		inSize
 The desired name length, including the null terminating character. Must be no longer than the size of the pre-allocated @p outEffectTypeName buffer, in bytes.
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectTypeName(AAXHShell_TPlugInID inID, int32_t inTypeIndex, char* outEffectTypeName, int32_t inSize);
/**@} end Effect type data retrieval operations */


/* --------------------------------------------------------------------------- */
#pragma mark - Effect instance loading
/**@{*/
/** @name Effect instance loading operations
 */
/**@{*/
/** @brief Instantiate an effect within a loaded plug-in bundle using the current host context
 
 @details
 @sa AAXHShell_TInstanceID
 @sa AAXHShell_SetHostContext()
 
 @param[in]		inID
 @param[in]		inEffectIndex
 The index of the desired <B>effect</B> within the plug-in bundle
 @param[out]	outInstanceID
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_InstantiateEffect(AAXHShell_TPlugInID inID, int32_t inEffectIndex, AAXHShell_TInstanceID* outInstanceID);
/** @brief Instantiate an effect within a loaded plug-in bundle using the given host context
 
 @details
 @sa AAXHShell_TInstanceID
 
 @param[in]		inID
 @param[in]		inEffectIndex
 The index of the desired <B>effect</B> within the plug-in bundle
 @param[in]		inHostContext
 @param[out]	outInstanceID
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_InstantiateEffectForContext(AAXHShell_TPlugInID inID, int32_t inEffectIndex, const AAXHShell_SHostContext* inHostContext, AAXHShell_TInstanceID* outInstanceID);
/** @brief Instantiate a specific effect type within a loaded plug-in bundle
 
 @details
 @sa AAXHShell_TInstanceID
 
 @param[in]		inID
 @param[in]		inTypeIndex
 The index of the desired <B>effect type</B> within the plug-in bundle
 @param[out]	outInstanceID
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_InstantiateEffectOfType(AAXHShell_TPlugInID inID, int32_t inTypeIndex, AAXHShell_TInstanceID* outInstanceID);
/** @brief Deinstantiate an effect instance
 
 @details
 @sa AAXHShell_TInstanceID
 
 @param[in]		inID
 @param[in]		inInstanceID
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_DeinstantiateEffect(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID);
/**@} end Effect instance loading operations */


/* --------------------------------------------------------------------------- */
#pragma mark - Effect instance
/**@{*/
/** @name Effect instance operations
 */
/**@{*/
/** @brief Get an instance's effect ID as a C string
 
 @details
 If the provided char buffer is too small then this returns false
 
 @param[in]		inID
 @param[in]		inInstanceID
 @param[out]	outEffectID
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 @param[in]		inSize
 The size of the pre-allocated @c outEffectID buffer, in bytes.
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetInstanceEffectID(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, char* outEffectID, int32_t inSize);


AAXHShell_API(AAXHShell_TResult) AAXHShell_ResetAlgorithmContext(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID);
AAXHShell_API(AAXHShell_TResult) AAXHShell_RenderAudio(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, const float* const inAudioIns[], int32_t inAudioInCount, float* const inAudioOuts[], int32_t inAudioOutCount, int32_t inAudioBufferLength);
AAXHShell_API(AAXHShell_TResult) AAXHShell_RenderAudioHybrid(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, const float* const inAudioIns[], int32_t inAudioInCount, float* const inAudioOuts[], int32_t inAudioOutCount, int32_t inAudioBufferLength, int64_t inRunningTime, int32_t* outResult);

AAXHShell_API(AAXHShell_TResult) AAXHShell_WriteSettingsToFile(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, const char* inSettingsFileCString);
AAXHShell_API(AAXHShell_TResult) AAXHShell_SetSettingsFromFile(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, const char* inSettingsFileCString);
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetStateMatchesPreset(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, AAXHShell_TBoolean* outStateMatches);
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetNumberOfChanges(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, int32_t* outNumChanges);

/*
 Don't know what to do with MIDI methods...
 AAXHShell_API(AAXHShell_TResult) AAXHShell_DoMIDITransfers(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID);
 AAXHShell_API(AAXHShell_TResult) AAXHShell_UpdateMIDINodes(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, );
 AAXHShell_API(AAXHShell_TResult) AAXHShell_UpdateControlMIDINodes(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, );
 */



/** @brief Get curve data for an effect instance
 
 @details
 This method returns @c true if the plug-in instance was found, even if the instance returned a non-successful
 result. Clients must check the value of @c outResult before checking any other output parameters
 
 @param[in]		inID
 @param[in]		inInstanceID
 @param[in]		inCurveType
 One of @c AAX_ECurveType
 @param[in]		inValues
 @c float array of input values
 @param[in]		inNumValues
 The number of @c float elements in the @c inValues array
 @param[out]	outResult
 @c AAX_Result returned from the plug-in instance
 @param[out]	outValues
 @c float array that will be populated with @c inNumValues curve points. The allocated size of this array
 must be at least 32 * @c inNumValues bits.
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetInstanceCurveData(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, uint32_t inCurveType, const float* inValues, uint32_t inNumValues, int32_t* outResult, float* outValues);
/** @brief Indicates which meters correspond to the X and Y axes of the EQ or Dynamics graph
 
 @details
 This method returns @c true if the plug-in instance was found, even if the instance returned a non-successful
 result. Clients must check the value of @c outResult before checking any other output parameters
 
 @param[in]		inID
 @param[in]		inInstanceID
 @param[in]		inCurveType
 One of @c AAX_ECurveType
 @param[out]	outResult
 @c AAX_Result returned from the plug-in instance
 @param[out]	outXMeterId
 Id of the X-axis meter
 @param[out]	outYMeterId
 Id of the Y-axis meter
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetInstanceCurveDataMeterIds(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, uint32_t inCurveType, int32_t* outResult, uint32_t *outXMeterId, uint32_t *outYMeterId);
/** @brief Determines the range of the graph shown by the plug-in
 
 @details
 This method returns @c true if the plug-in instance was found, even if the instance returned a non-successful
 result. Clients must check the value of @c outResult before checking any other output parameters
 
 @param[in]		inID
 @param[in]		inInstanceID
 @param[in]		inCurveType
 One of @c AAX_ECurveType
 @param[out]	outResult
 @c AAX_Result returned from the plug-in instance
 @param[out]	outXMin
 Min value of X-axis range
 @param[out]	outXMax
 Max value of X-axis range
 @param[out]	outYMin
 Min value of Y-axis range
 @param[out]	outYMax
 Max value of Y-axis range
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetInstanceCurveDataDisplayRange(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, uint32_t inCurveType, int32_t* outResult, float *outXMin, float *outXMax, float *outYMin, float *outYMax);
/**@} end Effect instance operations */


/* --------------------------------------------------------------------------- */
#pragma mark - Parameters
/** @name Parameter operations
 */
/**@{*/
/** @brief Returns the number of parameters in particular effect instance
 
 @details
 @param[in]		inID
 @param[in]		inInstanceID
 @param[out]	outNumParameters
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetNumberOfParameters(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, int32_t* outNumParameters);
/** @brief Gets a parameter's ID string
 
 If the provided char buffer is too small then this returns false
 
 @details
 @param[in]		inID
 @param[in]		inInstanceID
 @param[in]	    inParameterIndex
 @param[out]	outParamID
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 @param[in]		inSize
 The size of the pre-allocated @c outParamID buffer, in bytes.
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetParameterID(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, int32_t inParameterIndex, char* outParamID, int32_t inSize);
/** @brief Gets a parameter's \b full name
 
 If the provided char buffer is too small then this returns false
 
 \note Usually you should call \ref AAXHShell_GetParameterNameOfLength rather than this method
 
 @details
 @param[in]		inID
 @param[in]		inInstanceID
 @param[in]	    inParameterIndex
 @param[out]	outParamName
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 @param[in]		inSize
 The size of the pre-allocated @c outParamID buffer, in bytes.
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetParameterName(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, int32_t inParameterIndex, char* outParamName, int32_t inSize);
/** @brief Gets a parameter's name abbreviated to a particular maximum length
 
 @details
 @note The semantics of this method are slightly different from the corresponding AAX_IEffectParameters
 method. Here, inNameLength is the size of the full buffer including its NULL terminating character, not
 the number of characters in the string.
 
 @param[in]		inID
 @param[in]		inInstanceID
 @param[in]	    inParameterIndex
 @param[in]		inNameLength
 @param[out]	outParamName
 A UTF-8 c-string no longer than @c inNameLength bytes (including the null terminating character.)
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetParameterNameOfLength(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, int32_t inParameterIndex, int32_t inNameLength, char* outParamName);
/** @brief Gets number of steps for parameter
 
 @details
 @param[in]		inID
 @param[in]		inInstanceID
 @param[in]	    inParameterIndex
 @param[out]	outNumOfSteps
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetParameterNumOfSteps(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, int32_t inParameterIndex, int32_t* outNumOfSteps);
/** @brief Checks whether parameter is automatable
 
 @details
 @param[in]		inID
 @param[in]		inInstanceID
 @param[in]	    inParameterIndex
 @param[out]	outIsAutomatable
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetParameterIsAutomatable(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, int32_t inParameterIndex, AAXHShell_TBoolean* outIsAutomatable);
/** @brief Checks whether parameter is discrete or continious
 
 @details
 @param[in]		inID
 @param[in]		inInstanceID
 @param[in]	    inParameterIndex
 @param[out]	outType
 
  @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetParameterType(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, int32_t inParameterIndex, int32_t* outType);
/** @brief Gets the parameter's current value as a C string
 
 @details
 @param[in]		inID
 @param[in]		inInstanceID
 @param[in]	    inParameterIndex
 @param[out]	outParamValueCString
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 @param[in]		inSize
 The size of the pre-allocated @c outParamValueCString buffer, in bytes
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetParameterValueString(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, int32_t inParameterIndex, char* outParamValueCString, int32_t inSize);
/** @brief Sets the parameter's value
 
 @details
 @param[in]		inID
 @param[in]		inInstanceID
 @param[in]	    inParameterIndex
 @param[out]	inParamValueCString
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_SetParameterValueFromString(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, int32_t inParameterIndex, const char* inParamValueCString);
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetParameterNormalizedValue(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, int32_t inParameterIndex, double* outValue);
AAXHShell_API(AAXHShell_TResult) AAXHShell_SetParameterNormalizedValue(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, int32_t inParameterIndex, double inValue);
AAXHShell_API(AAXHShell_TResult) AAXHShell_TouchParameter(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, int32_t inParameterIndex);
AAXHShell_API(AAXHShell_TResult) AAXHShell_ReleaseParameter(AAXHShell_TPlugInID inID, AAXHShell_TInstanceID inInstanceID, int32_t inParameterIndex);



AAXHShell_API(AAXHShell_TResult) AAXHShell_LoadPageTable(AAXHShell_TPlugInID inID, int32_t inTypeIndex); ///< @returns kAAXHShell_Fail if the page table is not loaded successfully
AAXHShell_API(AAXHShell_TResult) AAXHShell_IsPageTableLoaded(AAXHShell_TPlugInID inID, int32_t inTypeIndex, AAXHShell_TBoolean* outResult);
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetPageTablesPathSize(AAXHShell_TPlugInID inID, int32_t inTypeIndex, int32_t* outPathSize); ///< \p outPathSize includes null terminating char
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetPageTablesPath(AAXHShell_TPlugInID inID, int32_t inTypeIndex, int32_t inPathSize, char* outPath);

AAXHShell_API(AAXHShell_TResult) AAXHShell_HasCustomizedPageTable(AAXHShell_TPlugInID inID, int32_t inTypeIndex,
														 uint32_t inPageTableType, int32_t inPageTableSize, AAXHShell_TBoolean* outResult);
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetPlugInControlForPage(AAXHShell_TPlugInID inID, int32_t inTypeIndex,
														  uint32_t inPageTableType, int32_t inPageTableSize,
														  int16_t inPageIndex, int16_t inControllerIndex,
														  int32_t buffSize, char* outControl);

AAXHShell_API(AAXHShell_TResult) AAXHShell_GetNumSupportedPageTableTypes(AAXHShell_TPlugInID inID, int32_t inTypeIndex, int32_t* outTableTypesNum);

AAXHShell_API(AAXHShell_TResult) AAXHShell_GetNumPages(AAXHShell_TPlugInID inID, int32_t inTypeIndex, uint32_t inPageTableType, int32_t inPageTableSize, int32_t* outNumPages);
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetStandardNumControlsForPageTableType(uint32_t inPageTableType, int32_t* outNumControls);
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetControlForPageTableType(AAXHShell_TPlugInID inID, int32_t inTypeIndex, uint32_t inPageTableType, int32_t inPageTableSize, int32_t inPageIndex,
																		int32_t inControlIndex, int32_t inControlNameSize, char* outControlName);

AAXHShell_API(AAXHShell_TResult) AAXHShell_GetNumPageTableControlIdentifiers(AAXHShell_TPlugInID inID, int32_t inTypeIndex, int32_t* outNumControlNames);
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetPageTableControlIdentifierByIndex(AAXHShell_TPlugInID inID, int32_t inTypeIndex, int32_t inControlIndex, int32_t inControlNameSize, char* outControlName);

/** @brief Gets supported page table type by index
 
 @details
 @param[in]		inID
 @param[in]		inTypeIndex
 @param[in]		inPageTableTypesIndex
 @param[out]	outPageTableType
 @param[out]	outPageTableSize
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetSupportedPageTableType(AAXHShell_TPlugInID inID, int32_t inTypeIndex, int32_t inPageTableTypesIndex, uint32_t* outPageTableType, int32_t* outPageTableSize);

/** @brief Gets the mask that indicates an affiliation to category
 
 @details
 @param[in]		inID
 @param[in]		inEffectId
 @param[out]	outCategoryBits
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetEffectCategoryBits(AAXHShell_TPlugInID inID, const char* inEffectId, uint32_t* outCategoryBits);
/** @brief Gets the bypassable parameter ID by category bit mask
 
 @details
 @param[in]		inID
 @param[in]		inEffectId
 @param[in]		inCategoryBits
 Mask must have only one bit set when getting the category bypass parameter ID, otherwise the method will fail with a false return value
 @param[in]		inSize
 @param[out]	outParamID
 
 @ingroup AAXHostShell_Operations
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetCategoryBypassParameterID(AAXHShell_TPlugInID inID, const char* inEffectId, uint32_t inCategoryBits, int32_t inSize, char* outParamID);

/**@}end Parameter operations */


#endif
