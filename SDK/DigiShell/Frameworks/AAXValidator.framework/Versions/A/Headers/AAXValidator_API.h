/*
 AAXValidator_API.h
 AAXValidator
 
 Created by Rob Majors on 10/4/13.
 Copyright 2014 by Avid Technology, Inc.
 */

#ifndef AAXValidator_AAXValidator_API_h
#define AAXValidator_AAXValidator_API_h

/** @file
 
 @brief Macros used for defining the AAXValidator ABI
 
 @details
 @internal These macros are based on the macros defined in AAXH_API.h
 */

/** @def AAXVal_CDECL
 Sets the @c __cdecl calling convention for a symbol when applicable to the compiling environment
 */

/** @def AAXVal_EXTERN_C
 Defines 'C' linkage for a symbol when applicable to the compiling environment
 */

/** @def AAXVal_ATTR_API_DLL_EXPORT
 Sets the appropriate properties to export a symbol from the current DLL
 */

/** @def AAXVal_ATTR_API_DLL_IMPORT
 Sets the appropriate properties to import a symbol into the current DLL
 */

/** @def AAXVal_API
 Defines a symbol as part of the exported AAXValidator API
 */


#if		defined(_MSC_VER)
#define AAXVal_CDECL __cdecl
#else	/* GCC */
#define AAXVal_CDECL
#endif

#if defined (__cplusplus)
#define AAXVal_EXTERN_C extern "C"
#else
#define AAXVal_EXTERN_C
#endif

#if defined (__GNUC__)
#if 4 <= __GNUC__
/* symbol exposure */
#define AAXVal_ATTR_API_DLL_EXPORT(returntype) AAXVal_EXTERN_C __attribute__((visibility("default"))) returntype
#define AAXVal_ATTR_API_DLL_IMPORT(returntype) AAXVal_EXTERN_C __attribute__((visibility("default"))) returntype
#endif /* if 4 <= __GNUC__ */
#else
/* symbol exposure */
#define AAXVal_ATTR_API_DLL_EXPORT(returntype) AAXVal_EXTERN_C __declspec(dllexport) returntype AAXVal_CDECL
#define AAXVal_ATTR_API_DLL_IMPORT(returntype) AAXVal_EXTERN_C __declspec(dllimport) returntype AAXVal_CDECL
#endif

#if defined(AAXVal_DLL)
/* building DLL */
#define AAXVal_API(returntype)					AAXVal_ATTR_API_DLL_EXPORT(returntype)
#else
/* using DLL */
#define AAXVal_API(returntype)					AAXVal_ATTR_API_DLL_IMPORT(returntype)
#endif

#endif
