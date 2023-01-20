/***********************************************************************

	The software and files contained herein are furnished under a 
	license agreement. The software: (1) is for internal use only; 
	(2) may not be redistributed; (3) may be used or copied only in
	accordance with the terms of the license agreement.  

	Copyright (c) 2004 Avid Technology, Inc. All rights reserved.

************************************************************************/
#ifndef acfMessageCheck_h
#define acfMessageCheck_h


 /*!
 \file acfcheckm.h
 \brief Utility to report ACFRESULTS as a string and display the result in a Message box.
 \remarks
	The code within acfcheckm are designed to aid debugging. The code may be freely modified. 
	

 \author Todd Kelly
 \date March 2005
 \note This file is intended to help debugging of ACFRESULTS
 It is a starting point for ACF developers. You may freely 
 modify acfCheckM.h and acfCheckM.cpp to  
 meet your needs. The acfMessageCheck files are intended to 
 suplement acfcheck( ).  In Windows you will need to ling to User32.lib
 in order to use the MessageBox functionality.

 */


 
#include "acfbasetypes.h"
#include "acfresult.h"



//////////////////////////////////
// Build Options

// REPORT_RESULT_AS_STRING -
// Create message only in a debug environment
#ifdef _DEBUG
	#ifdef _WIN32
		// Currently windows only
		#define REPORT_RESULT_AS_STRING 
	#endif
#else 
	#define THROWACFRESULT
#endif

#ifdef NDEBUG
#define NO_MESSAGE			
#endif

/*!
 Defining THROWACFRESULT, allows or supress acfcheckmM
to throwing a failed ACFRESULT.
 The defualt prevents throwing in a debug build.
 Because a message box is enough warning when
 debugging...
 A defualt release build will throw any result that
 represents failure.
 
  Enabling provides for behaviour similar to acfcheck
 which does throw a result. 
 In a release build the default behaviour is to throw
*/

#ifdef THROWACFRESULT 
	#define THROWRESULT throw
#else
	#define THROWRESULT
#endif


#ifdef _WIN32
#ifndef UNICODE
	#pragma message ( "Unicode should be defined" )
#endif
#endif



#ifndef WIDEN2
#define WIDEN2(x) L ## x
#endif

#ifndef WIDEN
#define WIDEN(x) WIDEN2(x)
#endif

#ifndef __FUNCTION__
	#define __FUNCTION__  ""
#endif


/*!
	AcfResultToString
	Given an ACF Result, the result is converted to a 
	a message string that easier for a human to read.
	example return would be "ACF_E_NOTIMPL"
*/
const wchar_t* ACFResultToString( ACFRESULT result );


#undef  acfcheckM

/*!
	\def acfcheckM( _ACF_result_ )
		In a debug build:
		In windows - acfcheckM an ACFRESULT in a message box with the 
		result code as a human readable string along with the file, function 
		and line that the error occurred in.

		In release build: acfcheckM  should do nothing or throw a failed result, depending
		on the definition of THROWACFRESULT
*/

#ifdef  REPORT_RESULT_AS_STRING  
	ACFRESULT _acfcheckM( ACFRESULT result, const wchar_t* file , const wchar_t* functioname, int line );
	#define acfcheckM(_result_) (_acfcheckM( _result_, WIDEN(__FILE__) , WIDEN(__FUNCTION__) , __LINE__ ))
#else
	#ifdef THROWACFRESULT
		inline void acfcheckM( ACFRESULT _result_) { if (ACFFAILED(_result_)) THROWRESULT ACFRESULT(_result_);}
	#else
		#define  acfcheckM( _result_ ) ( _result_)
	#endif 
#endif  /* NDEBUG */


	

#endif //acfMessageCheck_h

