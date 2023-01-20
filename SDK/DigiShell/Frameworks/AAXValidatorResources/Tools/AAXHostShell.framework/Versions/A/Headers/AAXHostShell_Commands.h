/*
	AAXHostShell_Commands.h
	AAXHostShell
	
	Created by Rob Majors on 2/1/14.
	Copyright 2014 by Avid Technology, Inc.
 */

#ifndef AAXHostShell_AAXHostShell_Commands_h
#define AAXHostShell_AAXHostShell_Commands_h

/** @file
 
 @brief Header for the AAXHostShell Command API
 */

/** @defgroup AAXHostShell_Commands AAXHostShell command interface
 @ingroup AAXHostShell_API
 
 @brief Command methods for the AAXHostShell 
 
 @details
 <B>Conforms to the conventions of the @ref AAXHostShell_API </B>
 
 This group of AAXHostShell API methods is comprised of "commands". Each command represents the
 logic for both executing and undoing a particular action.
 
 Each command is executed or undone as part of a <B>command list</B>. A command list is a list of
 one or more chained commands that may be executed or undone with a single method call.
 
 The client may allocate memory within a command list for storage of intermediate values. This is
 convenient, for example, when feeding an output parameter from one command to an input parameter
 of another command. Storage of this data internally within the command list allows the list to be
 self-contained and to not rely on or modify externally allocated memory when computing intermediate
 values.
 
 The following example demonstrates construction, execution, and clean-up of a command list. The
 command list in this example can be used to instantiate or de-instantiate a plug-in. While the
 plug-in is instantiated, its internal data (such as the instance ID) is available via
 the command list's @ref AAXHShell_TCommandListDataID values. These values may be hidden from
 clients to keep the command list's data internal (like @c effectIndexIDDataID) or made available
 to clients to allow access to the command list data (like @c instanceIDDataID.)
 
 @note The pure C command API can be unweildy, especially when managing allocations of command
 list data. There are no plans to extend the command API to support additional operations. Instead,
 we recommend wrapping the operations interface in language-specific command classes as needed.
 
 @code
	 // These IDs will be made available to the client
	 AAXHShell_TCommandListID instantiateCommandListID = kAAXHShell_IDUnset;
	 AAXHShell_TCommandListDataID instanceIDDataID = kAAXHShell_IDUnset;
	 
	 // Create and populate the command list
	 bool result = AAXHShell_CreateCommandList(&instantiateCommandListID);
	 if (true == result)
	 {
		 // Allocate memory for parameters that will be passed within the command list
		 AAXHShell_TCommandListDataID plugInIDDataID = kAAXHShell_IDUnset;
		 AAXHShell_TCommandListDataID plugInPathDataID = kAAXHShell_IDUnset;
		 AAXHShell_TCommandListDataID effectIndexIDDataID = kAAXHShell_IDUnset;
		 const char* const pathCStr = "path/to/MyPlugIn.aaxplugin";
		 uint32_t pathByteLength = 27; // Incl. terminating null char
		 
		 result = result && AAXHShell_AddCommandListData(instantiateCommandListID, 0, sizeof(AAXHShell_TPlugInID), &plugInIDDataID);
		 result = result && AAXHShell_AddCommandListData(instantiateCommandListID, 0, sizeof(AAXHShell_TInstanceID), &instanceIDDataID);
		 result = result && AAXHShell_AddCommandListData(instantiateCommandListID, 0, sizeof(int32_t), &effectIndexIDDataID);
		 result = result && AAXHShell_AddCommandListData(instantiateCommandListID, pathCStr, pathByteLength, &plugInPathDataID);
		 
		 // Construct the command list using the addresses of its internally-allocated data
		 result = result && AAXHShell_PushCommandLoadPlugIn(instantiateCommandListID, plugInPathDataID, plugInIDDataID);
		 result = result && AAXHShell_PushCommandFindFirstNativeEffectIndex(instantiateCommandListID, plugInIDDataID, effectIndexIDDataID);
		 result = result && AAXHShell_PushCommandInstantiateEffect(instantiateCommandListID, plugInIDDataID, effectIndexIDDataID, instanceIDDataID);
	 }
	 
	 //
	 // ...
	 //
	 
	 // Execute the command list (can be from a different thread, object, etc.)
	 result = AAXHShell_ExecuteCommandList(instantiateCommandListID);
	 
	 // Retrieve the instance ID
	 AAXHShell_TInstanceID instanceID = kAAXHShell_IDUnset;
	 result = result && AAXHShell_GetCommandListData(instantiateCommandListID, instanceIDDataID, &instanceID, sizeof(AAXHShell_TInstanceID));
	 
	 //
	 // ...
	 //
	 
	 // Clean up by undoing the instantiation and de-allocating all command list data.
	 // Note that the command list is self-contained and no additional parameters are
	 // required for clean-up.
	 result = AAXHShell_UndoAndDeleteCommandList(instantiateCommandListID);
 @endcode
 
 */

/* AAXHostShell Includes */
#include "AAXHostShell_API.h"
#include "AAXHostShell_Types.h"

/* C99 Includes */
#include <stdint.h>

/* --------------------------------------------------------------------------- */
#pragma mark - Commands
/** @name Command list handling operations
 
 @details
 @sa AAXHShell_TCommandListID
 */
/**@{*/
/** @brief Create a new command list
 
 @details
 @sa AAXHShell_TCommandListID
 
 @param[out]	outCommandListID
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_CreateCommandList(AAXHShell_TCommandListID* outCommandListID);
/** @brief Delete a command list and all associated data
 
 @details
 Returns true even if there is no command list with the given ID
 
 @sa AAXHShell_TCommandListID
 
 @param[in]		inCommandListID
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_DeleteCommandList(AAXHShell_TCommandListID inCommandListID);
/** @brief Get the number of commands in a command list
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetCommandListLength(AAXHShell_TCommandListID inCommandListID, uint64_t* outSize);
/** @brief Execute all commands in a command list in order
 
 @details
 If any command fails, this operation undoes the failing command and all previous commands and returns @c false.
 
 @param[in]		inCommandListID
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_ExecuteCommandList(AAXHShell_TCommandListID inCommandListID);
/** @brief Execute all commands in a command list in order, then delete the command list and associated memory
 
 @details
 If any command fails, this operation undoes the failing command and all previous commands and returns @c false.
 If @c false is returned, the command list has not been deleted.
 
 @param[in]		inCommandListID
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_ExecuteAndDeleteCommandList(AAXHShell_TCommandListID inCommandListID);
/** @brief Undo all commands in a command list in order
 
 @details
 If any command fails, this operation re-executes the failing command and all previous commands and returns @c false.
 
 @param[in]		inCommandListID
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_UndoCommandList(AAXHShell_TCommandListID inCommandListID);
/** @brief Undo all commands in a command list in order, then delete the command list and associated memory
 
 @details
 If any command fails, this operation re-executes the failing command and all previous commands and returns @c false.
 If @c false is returned, the command list has not been deleted.
 
 @param[in]		inCommandListID
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_UndoAndDeleteCommandList(AAXHShell_TCommandListID inCommandListID);
/** @brief Remove the last command from a command list
 
 @param[in]		inCommandListID
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_PopCommand(AAXHShell_TCommandListID inCommandListID);
/**@}end Command list handling operations */

/** @name Command list data handling operations
 
 @details
 @sa AAXHShell_TCommandListDataID
 */
/**@{*/
/** @brief Allocate a buffer for internal command list data and optionally copy external data into the buffer
 
 @details
 @sa AAXHShell_TCommandListDataID
 
 @param[in]		inCommandListID
 @param[in]		inData
 @param[in]		inDataSize
 @param[out]	outCommandListDataID
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_AddCommandListData(AAXHShell_TCommandListDataID inCommandListID, const void* inData, uint32_t inDataSize, AAXHShell_TCommandListDataID* outCommandListDataID);
/** @brief Copy data from an internal command list data buffer to an external (client) buffer
 
 @details
 @param[in]		inCommandListID
 @param[in]		inCommandListDataID
 @param[in,out]	ioData
 @param[in]		inDataSize
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_GetCommandListData(AAXHShell_TCommandListDataID inCommandListID, AAXHShell_TCommandListDataID inCommandListDataID, void* ioData, uint32_t inDataSize);
/**@}end Command list data handling operations */

/** @name Plug-in loading commands
 */
/**@{*/
/**	@brief Load or unload a plug-in
 
 @param[in]		inCommandListID
 @param[in]		inPlugInPathCStringID
 ID for a char[] data buffer. The buffer must contain a C string representing a system path to the plug-in.
 @param[in]		outEffectTypeID
 ID for an AAXHShell_TPlugInID buffer. Upon success, the buffer's contents will be overwritten with the ID of the loaded plug-in.
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_PushCommandLoadPlugIn(AAXHShell_TCommandListID inCommandListID, AAXHShell_TCommandListDataID inPlugInPathCStringID, AAXHShell_TCommandListDataID outEffectTypeID);
/**	@brief Instantiate or deinstantate a plug-in
 
 @details When executed, this command uses the current host context to select an appropriate effect type from the plug-in.
 
 @param[in]		inCommandListID
 @param[in]		inPlugInID
 ID for an AAXHShell_TPlugInID buffer containing the ID of the plug-in.
 @param[in]		inEffectIndex
 ID for an \c int32_t buffer contining the index of the plug-in Effect that should be instantiated.
 @param[in]		outInstanceID
 ID for an AAXHShell_TInstanceID buffer. Upon success, the buffer's contents will be overwritten with the ID of the new instance.
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_PushCommandInstantiateEffect(AAXHShell_TCommandListID inCommandListID, AAXHShell_TCommandListDataID inPlugInID, AAXHShell_TCommandListDataID inEffectIndex, AAXHShell_TCommandListDataID outInstanceID);
/**	@brief Instantiate or deinstantate a plug-in
 
 @details When executed, this command uses the current host context to select an appropriate effect type from the plug-in.
 
 @param[in]		inCommandListID
 @param[in]		inPlugInID
 ID for an AAXHShell_TPlugInID buffer containing the ID of the plug-in.
 @param[in]		inTypeIndexID
 ID for an \c int32_t buffer contining the index of the plug-in Type that should be instantiated.
 @param[in]		outInstanceID
 ID for an AAXHShell_TInstanceID buffer. Upon success, the buffer's contents will be overwritten with the ID of the new instance.
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_PushCommandInstantiateEffectByTypeIndex(AAXHShell_TCommandListID inCommandListID, AAXHShell_TCommandListDataID inPlugInID, AAXHShell_TCommandListDataID inTypeIndexID, AAXHShell_TCommandListDataID outInstanceID);
/**	@brief Instantiate or deinstantate a plug-in
 
 @details When executed, this command uses the given host context to select an appropriate effect type from the plug-in.
 
 @param[in]		inCommandListID
 @param[in]		inPlugInID
 ID for an AAXHShell_TPlugInID buffer containing the ID of the plug-in.
 @param[in]		inEffectIndexID
 ID for an \c int32_t buffer contining the index of the plug-in Effect that should be instantiated.
 @param[in]		inHostContextID
 ID for an \ref AAXHShell_SHostContext buffer containing the host context that will be used for effect type selection
 @param[in]		outInstanceID
 ID for an AAXHShell_TInstanceID buffer. Upon success, the buffer's contents will be overwritten with the ID of the new instance.
 
 @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_PushCommandInstantiateEffectForContext(AAXHShell_TCommandListID inCommandListID, AAXHShell_TCommandListDataID inPlugInID, AAXHShell_TCommandListDataID inEffectIndexID, AAXHShell_TCommandListDataID inHostContextID, AAXHShell_TCommandListDataID outInstanceID);
/**	@brief Find the first Native Effect index in a plug-in
 
 @param[in]		inCommandListID
 @param[in]		inPlugInID
 ID for an AAXHShell_TPlugInID buffer containing the ID of the plug-in.
 @param[out]	outEffectIndexID
 ID for an @c int32_t command list data buffer. Upon success, the buffer's contents will be overwritten with the index of the first effect within the plug-in that supports native processing.
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_PushCommandFindFirstNativeEffectIndex(AAXHShell_TCommandListID inCommandListID, AAXHShell_TCommandListDataID inPlugInID, AAXHShell_TCommandListDataID outEffectIndexID);
/**	@brief Find the first Native Effect Type index in a plug-in
 
 @param[in]		inCommandListID
 @param[in]		inPlugInID
 ID for an AAXHShell_TPlugInID buffer containing the ID of the plug-in.
 @param[out]	outTypeIndex
 ID for an @c int32_t command list data buffer. Upon success, the buffer's contents will be overwritten with the index of the first native effect type within the plug-in.
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_PushCommandFindFirstNativeTypeIndex(AAXHShell_TCommandListID inCommandListID, AAXHShell_TCommandListDataID inPlugInID, AAXHShell_TCommandListDataID outTypeIndex);
/**@}end Plug-in loading commands */

/** @name GUI commands
 */
/**@{*/
/**	@brief Create or destroy the GUI module for a plug-in instance
 
 @param[in]		inCommandListID
 @param[in]		inID
 ID for an AAXHShell_TPlugInID buffer containing the ID of the plug-in.
 @param[in]		inInstanceID
 ID for an AAXHShell_TInstanceID buffer containing the ID of the new instance.
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_PushCommandCreateEffectGUI(AAXHShell_TCommandListID inCommandListID, AAXHShell_TCommandListDataID inID, AAXHShell_TCommandListDataID inInstanceID);
/**	@brief Create or destroy a plug-in view container
 
 @param[in]		inCommandListID
 @param[in]		inViewPtrID
 ID for an \c void* buffer containing the address of an OS-specific view (e.g. an NSView* or HWND)
 @param[in]		outViewID
 ID for an AAXHShell_TViewID buffer. Upon success, the buffer's contents will be overwritten with the ID of the new view.
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_PushCommandCreateViewContainer(AAXHShell_TCommandListID inCommandListID, AAXHShell_TCommandListDataID inViewPtrID, AAXHShell_TCommandListDataID outViewID);
/**	@brief Associates or disaccociates a view with a plug-in instance
 
 @param[in]		inCommandListID
 @param[in]		inID
 ID for an AAXHShell_TPlugInID buffer containing the ID of the plug-in.
 @param[in]		inInstanceID
 ID for an AAXHShell_TInstanceID buffer containing the ID of the new instance.
 @param[in]		inViewID
 ID for an AAXHShell_TViewID buffer containing the ID of the new view.
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_PushCommandSetEffectInstanceView(AAXHShell_TCommandListID inCommandListID, AAXHShell_TCommandListDataID inID, AAXHShell_TCommandListDataID inInstanceID, AAXHShell_TCommandListDataID inViewID);
/**	@brief Sets a plug-in instance's associated view onto, or removes it from, the plug-in's GUI module
 
 @param[in]		inCommandListID
 @param[in]		inID
 ID for an AAXHShell_TPlugInID buffer containing the ID of the plug-in.
 @param[in]		inInstanceID
 ID for an AAXHShell_TInstanceID buffer containing the ID of the new instance.
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_PushCommandSetCurrentViewOnEffectGUI(AAXHShell_TCommandListID inCommandListID, AAXHShell_TCommandListDataID inID, AAXHShell_TCommandListDataID inInstanceID);
/**	@brief Runs or stops a plug-in's GUI module
 
 @param[in]		inCommandListID
 @param[in]		inID
 ID for an AAXHShell_TPlugInID buffer containing the ID of the plug-in.
 
  @ingroup AAXHostShell_Commands
 */
AAXHShell_API(AAXHShell_TResult) AAXHShell_PushCommandRunEffectGUI(AAXHShell_TCommandListID inCommandListID, AAXHShell_TCommandListDataID inID);
/**@}end GUI commands */


#endif
