/*
	AAXValidator.h
	AAXValidator
	
	Created by Rob Majors on 10/5/13.
	Copyright 2014 by Avid Technology, Inc.
 */

#ifndef AAXValidator_AAXValidator_h
#define AAXValidator_AAXValidator_h

/** @file
	
 @brief Unified header for AAXValidator clients
 
 @details 
 This header includes the full AAXValidator framework API and all required definitions
 */

/** @defgroup AAXValidator_API Framework C API
 
 @brief C API for the AAXHostAppliation framework
 
 @details
 Applications that wish to run tests on AAX plug-ins may link to the AAXValidator framework and
 use this API.
 
 <b>Interface compliance</b>
 This is a pure C interface. Client applications may be written in any C99 compliant language.
 
 <b>Error handling</b>
 All operations in this API return a @ref AAXVal_Result.
 
 <b>Lifecycle</b>
 Clients must call @ref AAXVal_Initialize() before calling any other methods in this API, and must
 call @ref AAXVal_Teardown() after completing all other API method execution.
 */

/* AAXValidator Includes */
#include "AAXValidator_API.h"
#include "AAXValidator_Types.h"

/* C99 Includes */
#include <stdint.h>


/* --------------------------------------------------------------------------- */
#pragma mark - Lifecycle management
/** @name Lifecycle management
 
 @details
 These methods are used to manage the life cycle of the AAXValidator framework internals.
 */
/**@{*/
/** @brief Initialize the AAXValidator framework
 
 @details
 <b>Precondition</b>
 The client application must call this method before calling any other methods in the
 @ref AAXValidator_API.
 
 @ingroup AAXValidator_API
 */
AAXVal_API(void) AAXVal_Initialize();
/**	@brief Teardown the AAXValidator framework
 
 @details
 <b>Precondition</b>
 The client appliation must call this method after all other @ref AAXValidator_API calls
 have been completed.
 
 @ingroup AAXValidator_API
 */
AAXVal_API(void) AAXVal_Teardown();
/**@}end Lifecycle management */


/* --------------------------------------------------------------------------- */
#pragma mark -
/** @name Filesystem
 */
/**@{*/
/** @brief Searches for AAX plug-ins in a directory and returns the number of plug-ins found
 
 @details
 Calling this method refreshes an internal cache for the given path/recursion pair. This method must be called prior to
 @ref AAXVal_GetAAXPlugInPath or @ref AAXVal_GetMaxAAXPlugInPathSize for this path/recursion pair.
 
 @param[in]		inDirPath
 Root path to search. Returned paths will be relative to this path.
 @param[in]		inRecursive
 If set to a non-zero value, @c inDirPath will be searched recursively.
 @param[out]	outNumPlugIns
 The number of plug-in paths concatenated in @c outPaths.
 
 @ingroup AAXValidator_API
 */
AAXVal_API(AAXVal_Result) AAXVal_GetNumAAXPlugIns(const char* inDirPath, AAXVal_TBoolean inRecursive, int32_t* outNumPlugIns);
/** @brief Returns the byte size of the longest plug-in path with the given search path/recursion pair
 
 @details
 @sa @ref AAXVal_GetAAXPlugInPath
 
 @pre @ref AAXVal_GetNumAAXPlugIns has been called for this search path/recursion pair
 
 @param[in]		inDirPath
 Root path to search.
 @param[in]		inRecursive
 If set to a non-zero value, @c inDirPath will be searched recursively.
 @param[out]	outSize
 The byte size of the longest path that could be returned from @ref AAXVal_GetAAXPlugInPath for this search path/recursion pair
 
 @ingroup AAXValidator_API
 */
AAXVal_API(AAXVal_Result) AAXVal_GetMaxAAXPlugInPathSize(const char* inDirPath, AAXVal_TBoolean inRecursive, int32_t* outSize);
/** @brief Searches for AAX plug-ins in a directory and returns their relative paths
 
 @details
 @sa @ref AAXVal_GetMaxAAXPlugInPathSize
 
 @pre @ref AAXVal_GetNumAAXPlugIns has been called for this search path/recursion pair
 
 @param[in]		inDirPath
 Root path to search. Returned paths will be relative to this path.
 @param[in]		inRecursive
 If set to a non-zero value, @c inDirPath will be searched recursively.
 @param[in]		inPlugInIndex
 Index of the requested plug-in path. Must be less than the value provided by @ref AAXVal_GetNumAAXPlugIns for this search path/recursion pair
 @param[out]	outPath
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 @param[in]		inSize
 The size of the pre-allocated @c outPath buffer, in bytes.
 
 @ingroup AAXValidator_API
 */
AAXVal_API(AAXVal_Result) AAXVal_GetAAXPlugInPath(const char* inDirPath, AAXVal_TBoolean inRecursive, int32_t inPlugInIndex, char* outPath, int32_t inSize);
/**@}end Filesystem operations */


/* --------------------------------------------------------------------------- */
#pragma mark -
/** @name Configuration
 */
/**@{*/
/** @brief Returns the byte size of a formatted C string representing the full AAX Validator configuration
 
 @details
 The size provided is for the currently selected configuration data format
 
 @details
 @sa @ref AAXVal_GetConfiguration
 
 @param[in]		inFormat
 @param[out]	outSize
 The byte length of the configuration's C string (including the null terminating character in the case of a C string format.)
 
 @ingroup AAXValidator_API
 */
AAXVal_API(AAXVal_Result) AAXVal_GetConfigurationSize(AAXVal_TFormat inFormat, int32_t* outSize);
/** @brief Returns a formatted C string representing the full AAX Validator configuration
 
 @details
 @sa @ref AAXVal_GetConfigurationSize
 
 @param[in]		inFormat
 @param[out]	outConfiguration
 A data buffer no longer than @c inSize bytes (including the null terminating character in the case of a C string format.)
 @param[in]		inSize
 The size of the pre-allocated @c outEffectID buffer, in bytes.
 
 @ingroup AAXValidator_API
 */
AAXVal_API(AAXVal_Result) AAXVal_GetConfiguration(AAXVal_TFormat inFormat, char* outConfiguration, int32_t inSize);
/** @brief Refreshes the AAX Validator configuration
 
 @details
 The configuration is automatically refreshed during initialization; clients
 only need to call this method if changes are made to the configuration after
 initialization.
 
 @ingroup AAXValidator_API
 */
AAXVal_API(AAXVal_Result) AAXVal_RefreshConfiguration();

/* NOTE: The comment blocks for the following APIs are un-doxified so that Doxygen
   does not get confused when it tries to parse this header. If we re-add these APIs
   then we must also add the extra '*' or '!' character to each comment block so
   that Doxygen will parse the block as the function documentation.
 */
/* @brief Returns the number of tests included in the current configuration
 
 @details
 @todo We don't need this kind of detail for the initial API; the full config dump should be sufficient unless it becomes unweildy for the client
 
 @param[out]	outNumTestConfigs
 The number of tests in the current configuration.
 
 @sa @ref AAXVal_GetTestConfigID
 
 @ingroup AAXValidator_API
 */
//AAXVal_API AAXVal_Result AAXVal_GetNumTestConfigs(int32_t* outNumTestConfigs);
/* @brief Returns the byte size of the longest test configuration ID
 
 @details
 @todo We don't need this kind of detail for the initial API; the full config dump should be sufficient unless it becomes unweildy for the client
 
 @sa @ref AAXVal_GetTestConfigID
 
 @param[out]	outSize
 The byte size of the longest path that could be returned from @ref AAXVal_GetTestConfigID for the current configuration
 
 @ingroup AAXValidator_API
 */
//AAXVal_API AAXVal_Result AAXVal_GetMaxTestConfigIDSize(int32_t* outSize);
/* @brief Returns the ID string of a test in the current configuration
 
 @details
 @todo We don't need this kind of detail for the initial API; the full config dump should be sufficient unless it becomes unweildy for the client
 
 @sa @ref AAXVal_GetNumTestConfigs
 
 @param[in]	inTestIndex
 @param[out]	outID
 A UTF-8 c-string no longer than @c inSize bytes (including the null terminating character.)
 @param[in]		inSize
 The size of the pre-allocated @c outID buffer, in bytes.
 
 @ingroup AAXValidator_API
 */
//AAXVal_API AAXVal_Result AAXVal_GetTestConfigID(int32_t inTestIndex, char* outID, int32_t inSize);
/**@}end Configuration operations */


/* --------------------------------------------------------------------------- */
#pragma mark -
/** @name Test invocation
 */
/**@{*/
/** @brief Runs tests for required plug-ins
 
 @details
 @sa @ref AAXValidator_Server_Communication_Protocol
 
 This operation clears and replaces the cache of test results
 
 @param[in]		inTestIDs
 Pointer to the array of a C strings representing the test Id.
 @param[in]		inNumTests
 The size of array @c inTestIDs.
 @param[in]		inPlugInPaths
 Pointer to the array of a C strings representing the plug-in bundle path.
 @param[in]		inNumPlugIns
 The size of array @c inPlugInPaths.
 @param[in]		inTimeoutSeconds
 The maximum number of seconds between invoking a test process and receiving a process identification message. If no identification message is received within this interval the test will be aborted.
 
 @ingroup AAXValidator_API
 */
AAXVal_API(AAXVal_Result) AAXVal_DoTestsOnPlugIns(const char * inTestIDs[], int32_t inNumTests, const char * inPlugInPaths[], int32_t inNumPlugIns, int32_t inTimeoutSeconds);
/** @brief Returns the byte size of the longest cached test result
 
 @details
 @sa @ref AAXVal_GetTestResult
 
 @param[in]		inFormat
 @param[out]	outSize
 The byte size of the longest test result that could be returned from @ref AAXVal_GetTestResult.
 
 @ingroup AAXValidator_API
 */
AAXVal_API(AAXVal_Result) AAXVal_GetMaxTestResultSize(AAXVal_TFormat inFormat, int32_t * outSize);
/** @brief Retrieves a single test result
 
 @details
 @sa @ref AAXVal_GetMaxTestResultSize
 
 Only the set of results from the most recent execution of
 \ref AAXVal_DoTestsOnPlugIns is available.
 
 If the previous execution of \ref AAXVal_DoTestsOnPlugIns succeeded then the
 number of available test results will be \p inNumTests * \p inNumPlugIns
 
 @param[in]		inFormat
 @param[in]		inResultIndex
 Index of the requested test result. Must be less than the value (inNumTests * inNumPlugIns) provided in @ref AAXVal_DoTestsOnPlugIns.
 @param[out]	outResult
 A UTF-8 C-string no longer than @c inSize bytes (including the null terminating character in the case of a C string format.)
 @param[in]		inSize
 The size of the pre-allocated @c outResult buffer, in bytes.
 
 @ingroup AAXValidator_API
 */
AAXVal_API(AAXVal_Result) AAXVal_GetTestResult(AAXVal_TFormat inFormat, int32_t inResultIndex, char * outResult, int32_t inSize);
/**@}end Test invocation operations */

/* --------------------------------------------------------------------------- */
#pragma mark - Connection port management
/** @name Connection port management
 
 @details
 These methods are used to manage the connection port numbers
 */
/**@{*/
/** @brief Adds a possible port number
 
 @details
 @sa @ref AAXVal_RemoveConnectionPortNumber
 
 @param[in] inPortNumber
 
 @ingroup AAXValidator_API
 */
AAXVal_API(AAXVal_Result) AAXVal_AddConnectionPortNumber(AAXVal_TPortNumber inPortNumber);
/** @brief Removes a possible port number
 
 @details
 Success return value indicates that the number is no longer in the current list,
 so returns success even if the given number was not found
 
 @sa @ref AAXVal_AddConnectionPortNumber
 @sa @ref AAXVal_ClearConnectionPortNumbers
 
 @param[in] inPortNumber
 
 @ingroup AAXValidator_API
 */
AAXVal_API(AAXVal_Result) AAXVal_RemoveConnectionPortNumber(AAXVal_TPortNumber inPortNumber);
/** @brief Provides the number of port numbers that are currently in the list
 
 @details
 @sa @ref AAXVal_GetConnectionPortNumbers
 
 @param[out] outNumPortNumbers
 
 @ingroup AAXValidator_API
 */
AAXVal_API(AAXVal_Result) AAXVal_GetNumConnectionPortNumbers(int32_t* outNumPortNumbers);
/** @brief Gets array with port numbers

 
 @details
 @sa @ref AAXVal_GetNumConnectionPortNumbers
 
 @param[out] outNumPortNumbers
 Indicates how many port numbers were written into the array
 @param[in] inSize
 Indicates the number of elements in the pre-allocated outPortNumbers array
 @param[out] outPortNumbers
 Array with port numbers
 
 @ingroup AAXValidator_API
 */
AAXVal_API(AAXVal_Result) AAXVal_GetConnectionPortNumbers(AAXVal_TPortNumber outPortNumbers[], int32_t inSize, int32_t* outNumPortNumbers);
/** @brief Clears the port number list
 
 @details
 @sa @ref AAXVal_RemoveConnectionPortNumber
 
 @ingroup AAXValidator_API
 */
AAXVal_API(AAXVal_Result) AAXVal_ClearConnectionPortNumbers(void);
/**@}end Connection port management */

#endif
