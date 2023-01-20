//
//  AAXHostShell_Types.h
//  AAXHostShell
//
//  Created by Rob Majors on 10/5/13.
//  Copyright 2014 by Avid Technology, Inc.
//

#ifndef AAXHostShell_AAXHostShell_Types_h
#define AAXHostShell_AAXHostShell_Types_h

#include <stdint.h>

#ifdef WIN32
#pragma warning( disable : 4068 )
#endif

/** @file
 
 @brief Type definitions and constants used by the AAXHostShell API
 */

/* --------------------------------------------------------------------------- */
#pragma mark Type definitions
/** @name Type definitions
 */
/**@{*/
/** @brief Fixed-width boolean type
 
 @details
 This type is used for all boolean values that cross the library boundary
 */
typedef uint32_t AAXHShell_TBoolean;
/** @brief AAXHostShell API return value type
 */
typedef AAXHShell_TBoolean AAXHShell_TResult;
/**	@brief Plug-in identifier
 
 @details
 A plug-in represents the collection of AAX elements that are contained within
 a single .aaxplugin bundle. A plug-in may contain one or more effects, and
 each effect may include a variety of configurations (types) that support
 different host environments.
 
 An single effect type may be instantiated multiple times.
 
 - To load a plug-in and acquire its ID use @ref AAXHShell_LoadPlugIn()
 - To unload a plug-in and retire its ID use @ref AAXHShell_UnloadPlugIn()
 */
typedef uint64_t AAXHShell_TPlugInID;
/**	@brief Effect instance identifier
 
 @details
 Identifier for a specific instance of a plug-in effect.
 
 - To create a new instance for the current host context and acquire its ID use @ref AAXHShell_InstantiateEffect()
 - To create a new instance of a particular type (regardless of the current host context) use @ref AAXHShell_InstantiateEffectOfType()
 - To deinstantiate an instance and retire its ID use @ref AAXHShell_DeinstantiateEffect()
 */
typedef uint64_t AAXHShell_TInstanceID;

/**	@brief View container identifier
 
 @details
 - To create a new view container and acquire its ID use @ref AAXHShell_CreateViewContainer()
 - To delete a view container and retire its ID use @ref AAXHShell_DeleteViewContainer()
 */
typedef uint64_t AAXHShell_TViewID;
/**	@brief Command list identifier
 
 @details
 - To allocate a new command list and acquire its ID use @ref AAXHShell_CreateCommandList()
 - To delete a command list and retire its ID use @ref AAXHShell_DeleteCommandList()
 */
typedef uint64_t AAXHShell_TCommandListID;
/**	@brief Command list internal data identifier
 
 @details
 - To allocate a new block of internal command list data and acquire its ID use @ref AAXHShell_AddCommandListData()
 - Command list data blocks cannot be deleted individually
 - Deleting a command list with @ref AAXHShell_DeleteCommandList() also deletes all data
 associated with the command list and retires all data identifiers that are associated with
 the command list.
 */
typedef uint64_t AAXHShell_TCommandListDataID;
/**	@brief Callbacks IDs 
 
 @details
 - AAX_IEffectDescriptor has  map with map of procedures
 - This type define type for getting procedure via the \ref AAXHShell_GetProcPtr function. 
 */
typedef int32_t AAXHShel_ProcType;
/**@}end Type definitions */

/** @name Data Structures
 */
/**@{*/
/** @brief Host context
 */
typedef struct AAXHShell_SHostContext
{
	int32_t mPlatform;			/**< Platform specifier. Must be one of: \ref kAAXHShell_Platform_Native; \ref kAAXHShell_Platform_TI */
	int32_t mHost;				/**< Host specifier. Must be either \ref kAAXHShell_Host_Generic or one of the defined host types such as \ref kAAXHShell_Host_ProTools */
	int32_t mHostVersion;		/**< Host version specifier */
	uint32_t mInputStemFormat;	/**< Input stem format specifier. The value must be one of \c AAX_EStemFormat, defined in AAX_Enums.h. */
	uint32_t mOutputStemFormat;	/**< Output stem format specifier. The value must be one of \c AAX_EStemFormat, defined in AAX_Enums.h. */
	int32_t mSampleRate;		/**< Sample rate. Set to \ref kAAXHShell_SampleRate_Any for any sample rate */
	AAXHShell_TBoolean mSupportsProcessing;	/**< Enable processing on instances in this host. Set to @c false to disable processing */
} AAXHShell_SHostContext;

/**@}end Data Structures */


/* --------------------------------------------------------------------------- */
#pragma mark Constants
/** @name Constants
 */
/**@{*/
static const AAXHShell_TResult kAAXHShell_Success = 0x1; /**< Success */
static const AAXHShell_TResult kAAXHShell_Fail = 0x0; /**< Failure */

static const uint64_t kAAXHShell_IDUnset = 0xFFFFFFFFFFFFFFFF;	/**< Generic unset ID value */
static const char* kAAXHShell_CategoryBypassParameterIDUnset = "UNSET_BYPASS_PARAMETER_ID";

/* The following constants are defined here rather than in enums due to the undefined size of enum values in C */

/* Debug bitfield flags */
static const int32_t kAAXHShell_DebugBits_None = 0x0;					/**< Specifies no debug behavior */
static const int32_t kAAXHShell_DebugBits_AssertFail = 0x1;				/**< Specifies an assertion on any API method failure */
static const int32_t kAAXHShell_DebugBits_TraceFail = 0x1 << 1;			/**< Specifies a DigiTrace statement on every API method failure */
static const int32_t kAAXHShell_DebugBits_TraceAll = 0x1 << 2;			/**< Specifies a DigiTrace statement on every API method call */
static const int32_t kAAXHShell_DebugBits_StackTraceFail = (0x1 << 1) | (0x1 << 3);	/**< Specifies a DigiTrace statement on every API method failure, with a stacktrace added at verbose logging levels */
static const int32_t kAAXHShell_DebugBits_Default = (0x1 << 1) | (0x1 << 3);		/**< The default debug level, matches \ref kAAXHShell_DebugBits_StackTraceFail */
static const int32_t kAAXHShell_DebugBits_MaxTrace = 0x1 | (0x1 << 1) | (0x1 << 2) | (0x1 << 3);	/**< A maximum tracing level, matches \ref kAAXHShell_DebugBits_AssertFail | \ref kAAXHShell_DebugBits_TraceAll | \ref kAAXHShell_DebugBits_StackTraceFail */

/* Result message semantic selectors */
static const int32_t kAAXHShell_ResultType_Success = 0x0;		/**< Indicates a success result */
static const int32_t kAAXHShell_ResultType_Warning = 0x1;		/**< Indicates a warning result */
static const int32_t kAAXHShell_ResultType_Error = 0x2;			/**< Indicates an error result */

/* Platform identifiers */
static const int32_t kAAXHShell_Platform_Native = 1;			/**< Specifies a native platform. \sa AAXHShell_SHostContext */
static const int32_t kAAXHShell_Platform_TI = 2;				/**< Specifies a TI platform. \sa AAXHShell_SHostContext */
static const int32_t kAAXHShell_Platform_None = 0;				/**< Used with an \ref AAXHShell_SHostContext to specify a context with no platform support (usually this is simply a failure case, e.g. when no platform support can be determined.) */
static const int32_t kAAXHShell_Platform_Any = -1;				/**< Used with an \ref AAXHShell_SHostContext to specify a platform-agnostic context */
static const int32_t kAAXHShell_Platform_Min = 1;		    	/**< The minimum real platform specifier value, matches \ref kAAXHShell_Platform_Native */
static const int32_t kAAXHShell_Platform_Max = 2;       		/**< The maximum real platform specifier value, matches \ref kAAXHShell_Platform_TI */

/* Host identifiers */
static const int32_t kAAXHShell_Host_Generic = 0;				/**< Host type is generic, unknown, or unavailable */
static const int32_t kAAXHShell_Host_ProTools = 1;				/**< Host type is Pro Tools */
static const int32_t kAAXHShell_Host_MediaComposer = 2;			/**< Host type is Media Composer */
static const int32_t kAAXHShell_Host_VENUE_S3LX = 3;			/**< Host type is VENUE S3L-X */
static const int32_t kAAXHShell_Host_VENUE_S6L = 4;				/**< Host type is VENUE S6L */

/* Proc ptr identifiers*/
static const AAXHShel_ProcType kAAXHShell_ProcPtrID_Create_EffectParameters = 0;		///< AAX_IEffectParameters creation procedure
static const AAXHShel_ProcType kAAXHShell_ProcPtrID_Create_EffectGUI = 1;			///< AAX_IEffectGUI creation procedure
static const AAXHShel_ProcType kAAXHShell_ProcPtrID_Create_EffectGUI_Icon =2;		///< <em>Used only by Eleven Rack GUI plug-ins.</em>
static const AAXHShel_ProcType kAAXHShell_ProcPtrID_Create_HostProcessor = 3;		///< AAX_IHostProcessor creation procedure
static const AAXHShel_ProcType kAAXHShell_ProcPtrID_Create_MixerParameters = 4;		///< AAX_IMixerParameters creation procedure, used by PIs that want ramped timestamp control changes
static const AAXHShel_ProcType kAAXHShell_ProcPtrID_Create_EffectDirectData = 5;		///< AAX_IEffectDirectData creation procedure, used by PIs that want direct access to their alg memor

/* Host version identifiers */
static const int32_t kAAXHShell_HostVersion_Any = 0x7FFFFFFF;

/* Stem format identifiers */
static const uint32_t kAAXHShell_StemFormat_Any = 0xFFFF0000U;	/**< Equivalent to \c AAX_eStemFormat_Any */

/* Sample rate identifiers */
static const int32_t kAAXHShell_SampleRate_Any = 0x7FFFFFFF;	/**< Equivalent to \c AAX_eSampleRateMask_All \sa AAXHShell_SHostContext */

/* Data structures */
static const struct AAXHShell_SHostContext AAXHShell_kHostContext_Any = {
	-1,          /**< Matches to \ref kAAXHShell_Platform_Any */
	0,           /**< Matches to \ref kAAXHShell_Host_Generic */
	0x7FFFFFFF,  /**< Matches to \ref kAAXHShell_HostVersion_Any */
	0xFFFF0000U, /**< Matches to \ref kAAXHShell_StemFormat_Any */
	0xFFFF0000U, /**< Matches to \ref kAAXHShell_StemFormat_Any */
	0x7FFFFFFF,  /**< Matches to \ref kAAXHShell_SampleRate_Any */
	0
};

/* Page table constants */
static const int32_t kAAXHShell_PageTableSize_Any = -1;

/**@}end Constants */

#endif
