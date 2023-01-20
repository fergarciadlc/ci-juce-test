/*================================================================================================*/
/*
 *	Copyright 2010-2015 by Avid Technology, Inc.
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
 *	\file   TI_Shell_Tracing.h
 *
 *  \brief  Tracing facilities for TI Plug-Ins.
 *
 *  \author Paul Vercellotti
 */ 
/*================================================================================================*/

#ifndef TI_SHELL_TRACING_H
#define TI_SHELL_TRACING_H

#include "AAX_Enums.h"

#ifdef  _TMS320C6X	// TI-only

#ifndef TI_SHELL_API_H
#include "TI_Shell_API.h"
#endif

#define TISHELLTRACE_MAX_ARGS 8

// Routine to do the tracing.  Don't use this directly; use TISHELLTRACE macro to call it.
// Note that all strings, including inFmtStr must be constant buffers, as this
// call processes formatting asynchronously.  String pointers need to be there
// for an unknown amount of time after this call returns.
TI_SHELL_API void _DoTrace( AAX_ETracePriorityDSP inPriority, const char *inFmtStr, unsigned int nArgs, ... );

// Faster version that only takes a simple string message
TI_SHELL_API void _DoTrace( AAX_ETracePriorityDSP inPriority, const char *inMessage );

/* Below, is the implementation of TISHELLTRACE( Priority, Str, ... ) that
 * allows printf-like output to be sent to host it takes more cycles than AAX_TRACE() 
 * so it should be used sparingly when you need to print out some variables
 * The PP_NARG macro returns the number of arguments that have been 
* passed to it. 
*/ 



#define _ARG16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define HAS_COMMA(...) _ARG16(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define HAS_NO_COMMA(...) _ARG16(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)
#define _TRIGGER_PARENTHESIS_(...) ,

#define HAS_ZERO_OR_ONE_ARGS(...)                                                    \
_HAS_ZERO_OR_ONE_ARGS(                                                               \
          /* test if there is just one argument, eventually an empty    \
             one */                                                     \
          HAS_COMMA(__VA_ARGS__),                                       \
          /* test if _TRIGGER_PARENTHESIS_ together with the argument   \
             adds a comma */                                            \
          HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__),                 \
          /* test if the argument together with a parenthesis           \
             adds a comma */                                            \
          HAS_COMMA(__VA_ARGS__ (~)),                                   \
          /* test if placing it between _TRIGGER_PARENTHESIS_ and the   \
             parenthesis adds a comma */                                \
          HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__ (~))              \
          )
 
#define PASTE5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define _HAS_ZERO_OR_ONE_ARGS(_0, _1, _2, _3) HAS_NO_COMMA(PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))
#define _IS_EMPTY_CASE_0001 ,
 
#define PP_NARG(...)  PP_NARG_(__VA_ARGS__, PP_RSEQ_N(__VA_ARGS__) )
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)


#define _VA0(...) HAS_ZERO_OR_ONE_ARGS(__VA_ARGS__)
#define _VA1(...) HAS_ZERO_OR_ONE_ARGS(__VA_ARGS__)
#define _VA2(...) 2
#define _VA3(...) 3
#define _VA4(...) 4
#define _VA5(...) 5
#define _VA6(...) 6
#define _VA7(...) 7
#define _VA8(...) 8
#define _VA9(...) 9
#define _VA10(...) 10
#define _VA11(...) 11
#define _VA12(...) 12
#define _VA13(...) 13
#define _VA14(...) 14
#define _VA15(...) 15
#define _VA16(...) 16

#define PP_ARG_N( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
	_11,_12,_13,_14,_15,_16,N,...) N 

#define PP_RSEQ_N(...) \
	_VA16(__VA_ARGS__),_VA15(__VA_ARGS__),_VA14(__VA_ARGS__),_VA13(__VA_ARGS__),\
	_VA12(__VA_ARGS__),_VA11(__VA_ARGS__),_VA10(__VA_ARGS__), _VA9(__VA_ARGS__),\
	_VA8(__VA_ARGS__),_VA7(__VA_ARGS__),_VA6(__VA_ARGS__),_VA5(__VA_ARGS__),\
	_VA4(__VA_ARGS__),_VA3(__VA_ARGS__),_VA2(__VA_ARGS__),_VA1(__VA_ARGS__),\
	_VA0(__VA_ARGS__) 


// Version with commas
#define _VAC0(N)
#define _VAC1(N) (, N)
#define _VAC2(N) (, N)
#define _VAC3(N) (, N)
#define _VAC4(N) (, N)
#define _VAC5(N) (, N)
#define _VAC6(N) (, N)
#define _VAC7(N) (, N)
#define _VAC8(N) (, N)
#define _VAC9(N) (, N)
#define _VAC10(N) (, N)
#define _VAC11(N) (, N)
#define _VAC12(N) (, N)
#define _VAC13(N) (, N)
#define _VAC14(N) (, N)
#define _VAC15(N) (, N)
#define _VAC16(N) (, N)

#define PP_NARGC(...)  APPLY( CAT( _VAC, PP_NARG(__VA_ARGS__) ), (PP_NARG(__VA_ARGS__)) )

#define _GEN_ARGS0( ... ) 
#define _GEN_ARGS1( _OneArg, ... ) , sizeof(_OneArg), _OneArg
#define _GEN_ARGS2( _OneArg, ... ) , sizeof(_OneArg), _OneArg _GEN_ARGS1(__VA_ARGS__)
#define _GEN_ARGS3( _OneArg, ... ) , sizeof(_OneArg), _OneArg _GEN_ARGS2(__VA_ARGS__)
#define _GEN_ARGS4( _OneArg, ... ) , sizeof(_OneArg), _OneArg _GEN_ARGS3(__VA_ARGS__)
#define _GEN_ARGS5( _OneArg, ... ) , sizeof(_OneArg), _OneArg _GEN_ARGS4(__VA_ARGS__)
#define _GEN_ARGS6( _OneArg, ... ) , sizeof(_OneArg), _OneArg _GEN_ARGS5(__VA_ARGS__)
#define _GEN_ARGS7( _OneArg, ... ) , sizeof(_OneArg), _OneArg _GEN_ARGS6(__VA_ARGS__)
#define _GEN_ARGS8( _OneArg, ... ) , sizeof(_OneArg), _OneArg _GEN_ARGS7(__VA_ARGS__)

#define CAT(x, y) CAT_I(x, y) 
#define CAT_I(x, y) x ## y 
#define APPLY(macro, args) APPLY_I(macro, args) 
#define APPLY_I(macro, args) macro args 
#define STRIP_PARENS(x) EVAL((STRIP_PARENS_I x), x) 
#define STRIP_PARENS_I(...) 1,1 
#define EVAL(test, x) EVAL_I(test, x) 
#define EVAL_I(test, x) MAYBE_STRIP_PARENS(TEST_ARITY test, x) 
#define TEST_ARITY(...) APPLY(TEST_ARITY_I, (__VA_ARGS__, 2, 1, 0)) 
#define TEST_ARITY_I(a,b,c,...) c 
#define MAYBE_STRIP_PARENS(cond, x) MAYBE_STRIP_PARENS_I(cond, x) 
#define MAYBE_STRIP_PARENS_I(cond, x) CAT(MAYBE_STRIP_PARENS_, cond)(x) 
#define MAYBE_STRIP_PARENS_1(x) x 
#define MAYBE_STRIP_PARENS_2(x) APPLY(MAYBE_STRIP_PARENS_2_I, x) 
#define MAYBE_STRIP_PARENS_2_I(...) __VA_ARGS__ 

#define TISHELLTRACE( Priority, Str, ... ) _DoTrace( Priority, Str STRIP_PARENS(PP_NARGC(__VA_ARGS__)) APPLY( CAT( _GEN_ARGS, PP_NARG(__VA_ARGS__)), (##__VA_ARGS__) ) )


#endif // _TMS320C6X

#endif
