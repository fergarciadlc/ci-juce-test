//
//  AAXValidator_Types.h
//  AAXValidator
//
//  Created by Rob Majors on 10/5/13.
//  Copyright 2014 by Avid Technology, Inc.
//

#ifndef AAXValidator_AAXValidator_Types_h
#define AAXValidator_AAXValidator_Types_h

/** @file
 
 @brief Type definitions and constants used by the AAXValidator API
 */


/* C99 Includes */
#include <stdint.h>

#if defined(_MSC_VER)
#pragma warning( disable : 4068 )
#endif // _MSC_VER

/* --------------------------------------------------------------------------- */
#pragma mark Macros

#define AAXVal_GLOBAL static const


/* --------------------------------------------------------------------------- */
#pragma mark Type definitions
/** @name Type definitions
 */
/**@{*/
/** @brief AAX Validator return type
 
 @details
 This is the return type for all AAX Validator C API calls
 */
typedef int32_t AAXVal_Result;
/** @brief Fixed-width boolean type
 
 @details
 This type is used for all boolean values that cross the library boundary
 */
typedef uint8_t AAXVal_TBoolean;
/**	@brief Format identifier
 */
typedef int32_t AAXVal_TFormat;
/**	@brief Connection port number type
 
 @details Connection port is a number in range [0, 65535]
 */
typedef int32_t AAXVal_TPortNumber;
/**@}end Type definitions */



/* --------------------------------------------------------------------------- */
#pragma mark Constants
/** @name Constants
 */
/**@{*/
AAXVal_GLOBAL uint64_t kAAXVal_IDUnset = 0xFFFFFFFFFFFFFFFF;								/**< Generic unset ID value */

AAXVal_GLOBAL int32_t kAAXVal_TimeoutDefault = 30;											/**< Convenience definition for test timeouts */

/* The following constants are defined here rather than in enums due to the undefined
 size of enum values in C */

/* Structured data format selectors */
AAXVal_GLOBAL AAXVal_TFormat kAAXVal_Format_Protobuf_Data = 0;								/**< Raw Protobuf data (default) */
AAXVal_GLOBAL AAXVal_TFormat kAAXVal_Format_Protobuf_Text = 1;								/**< Protobuf formatted text */
AAXVal_GLOBAL AAXVal_TFormat kAAXVal_Format_XML = 2;										/**< XML formatted text */
AAXVal_GLOBAL AAXVal_TFormat kAAXVal_Format_JSON = 3;										/**< JSON formatted text */
AAXVal_GLOBAL AAXVal_TFormat kAAXVal_Format_MinStringFormat = kAAXVal_Format_Protobuf_Text;	/**< Minimum AAXVal_TFormat string-formatted value */
AAXVal_GLOBAL AAXVal_TFormat kAAXVal_Format_MaxStringFormat = kAAXVal_Format_JSON;			/**< Maximum AAXVal_TFormat string-formatted value */

/* Configuration macro strings */
AAXVal_GLOBAL char* const kAAXVal_Macro_Dir_Resources = "AAXVAL_DIR_RESOURCES";				/**< Configuration macro: Absolute path to the Validator resources directory */
AAXVal_GLOBAL char* const kAAXVal_Macro_Dir_Tools = "AAXVAL_DIR_TOOLS";						/**< Configuration macro: Absolute path to the Validator tools directory */
AAXVal_GLOBAL char* const kAAXVal_Macro_Dir_Temp = "AAXVAL_DIR_TEMP";						/**< Configuration macro: Absolute path to the Validator temp directory */
AAXVal_GLOBAL char* const kAAXVal_Macro_Param_AAXPlugIn_Path = "AAXVAL_PARAM_AAXPLUGIN";	/**< Configuration macro: Absolute path to the .aaxplugin bundle under test */
AAXVal_GLOBAL char* const kAAXVal_Macro_Param_Unique_ID = "AAXVAL_PARAM_UNIQ_ID";			/**< Configuration macro: Unique Connection Identifier*/
AAXVal_GLOBAL char* const kAAXVal_Macro_TCP_Addr = "AAXVAL_TCP_ADDR";						/**< Configuration macro: Validator TCP client's address for test results */
AAXVal_GLOBAL char* const kAAXVal_Macro_TCP_Port = "AAXVAL_TCP_PORT";						/**< Configuration macro: Validator TCP client's port for test results */

AAXVal_GLOBAL char* const kAAXVal_Default_Connection_Address = "127.0.0.1";					/**< Default validator server address */
AAXVal_GLOBAL uint16_t kAAXVal_Default_Connection_Port = 63036;								/**< Default validator server port */

/**@}end Constants */

#endif
