/*
 AAXHostShell_API.h
 AAXHostShell
 
 Created by Rob Majors on 10/4/13.
 Copyright 2014 by Avid Technology, Inc.
 */

#ifndef AAXHostShell_AAXHostShell_API_h
#define AAXHostShell_AAXHostShell_API_h

/** @file
 
 @brief Macros used for defining the AAXHostShell ABI
 
 @details
 @internal These macros are based on the macros defined in AAXH_API.h
 */

/** @def AAXHShell_CDECL
 Sets the @c __cdecl calling convention for a symbol when applicable to the compiling environment
 */

/** @def AAXHShell_EXTERN_C
 Defines 'C' linkage for a symbol when applicable to the compiling environment
 */

/** @def AAXHShell_ATTR_API_DLL_EXPORT
 Sets the appropriate properties to export a symbol from the current DLL
 */

/** @def AAXHShell_ATTR_API_DLL_IMPORT
 Sets the appropriate properties to import a symbol into the current DLL
 */

/** @def AAXHShell_API
 Defines a symbol as part of the exported AAXHostShell API
 */


#if		defined(_MSC_VER)
#define AAXHShell_CDECL __cdecl
#else	/* GCC */
#define AAXHShell_CDECL
#endif

#if defined (__cplusplus)
#define AAXHShell_EXTERN_C extern "C"
#else
#define AAXHShell_EXTERN_C
#endif

#if defined (__GNUC__)
#if 4 <= __GNUC__
/* symbol exposure */
#define AAXHShell_ATTR_API_DLL_EXPORT(rettype) AAXHShell_EXTERN_C __attribute__((visibility("default"))) rettype
#define AAXHShell_ATTR_API_DLL_IMPORT(rettype) AAXHShell_EXTERN_C __attribute__((visibility("default"))) rettype
#endif /* if 4 <= __GNUC__ */
#else
/* symbol exposure */
#define AAXHShell_ATTR_API_DLL_EXPORT(rettype) AAXHShell_EXTERN_C __declspec(dllexport) rettype AAXHShell_CDECL
#define AAXHShell_ATTR_API_DLL_IMPORT(rettype) AAXHShell_EXTERN_C __declspec(dllimport) rettype AAXHShell_CDECL
#endif

#if defined(AAXHShell_DLL)
/* building DLL */
#define AAXHShell_API(rettype)					AAXHShell_ATTR_API_DLL_EXPORT(rettype)
#else
/* using DLL */
#define AAXHShell_API(rettype)					AAXHShell_ATTR_API_DLL_IMPORT(rettype)
#endif

#endif
