/*
	AAXHostShell.h
	AAXHostShell
	
	Created by Rob Majors on 10/5/13.
	Copyright 2014 by Avid Technology, Inc.
 */

#ifndef AAXHostShell_AAXHostShell_h
#define AAXHostShell_AAXHostShell_h

/** @file
	
 @brief Unified header for AAXHostShell clients
 
 @details 
 This header includes the full AAXHostShell framework API and all required definitions
 */

/** @defgroup AAXHostShell_API AAXHostShell framework API
 
 @brief API for the AAXHostAppliation framework
 
 @details
 A pure C interface exposed to AAXHostAppliation framework clients. Applications that wish
 to host AAX plug-ins may link to the AAXHostAppliation framework and use this API to load, run, and
 display plug-ins.
 
 <b>Interface compliance</b>
 This is a pure C interface. Client applications may be written in any C99 compliant language.
 
 <b>Error handling</b>
 The operations in this API return a @c bool indicating success (@c true) or failure (@c false). As
 a rule, if an operation returns @c false then it will leave the system in the same state as before
 it was called.
 
 @todo Create a system of error codes for the API, to be supplied via an optional output argument on
 each function. Error codes should at least indicate whether there was successful recovery from an
 error when @c false is returned. Alternatively, provide a single MS-style "get last error info"
 function.
 
 <b>Lifecycle</b>
 Clients must call @ref AAXHShell_Initialize() before calling any other methods in this API, and must
 call @ref AAXHShell_Teardown() after completing all other API method execution.
 
 <b>Other conventions</b>
 - All methods in this API return a @c bool to indicate success or failure, as described above.
 - In this API documentation, term "operation" refers to a one-time action, while the term "command"
 refers to a doable or undoable action which may be loaded into and executed via a command list. See
 the @ref AAXHostShell_Operations and @ref AAXHostShell_Commands documentation for further
 information.
 */

/* AAXHostShell API Groups */
#include "AAXHostShell_Operations.h"
#include "AAXHostShell_Commands.h"

/* AAXHostShell Includes */
#include "AAXHostShell_API.h"
#include "AAXHostShell_Types.h"

/* C99 Includes */
#include <stdint.h>

/* --------------------------------------------------------------------------- */
#pragma mark - Lifecycle management
/** @name Lifecycle management operations
 
 @details
 These methods are used to manage the life cycle of the AAXHostShell framework internals.
 */
/**@{*/
/** @brief Initialize the AAXHostShell framework
 
 @details
 <b>Precondition</b>
 The client application must call this method before calling any other methods in the
 @ref AAXHostShell_API.
 
 @ingroup AAXHostShell_API
 */
AAXHShell_API(void) AAXHShell_Initialize();
/**	@brief Teardown the AAXHostShell framework
 
 @details
 <b>Precondition</b>
 The client appliation must call this method after all other @ref AAXHostShell_API calls
 have been completed.
 
 @ingroup AAXHostShell_API
 */
AAXHShell_API(void) AAXHShell_Teardown();
/**@}end Lifecycle management */

#endif
