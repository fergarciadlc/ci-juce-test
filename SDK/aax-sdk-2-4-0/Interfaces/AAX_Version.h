/*================================================================================================*/
/*

 *	Copyright 2013-2017, 2019 by Avid Technology, Inc.
 *	All rights reserved.
 *	
 *	CONFIDENTIAL: This document contains confidential information.  Do not 
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
 *	OF Avid Technology, INC.
 *
 */

/**  
 *	\file   AAX_Version.h
 *	
 *	\brief Version stamp header for the %AAX SDK
 *
 *  This file defines a unique number that can be used to identify the version of the %AAX SDK
 */ 
/*================================================================================================*/


#pragma once

#ifndef _AAX_VERSION_H_
#define _AAX_VERSION_H_


/** \brief The SDK's version number
 *  
 *	\details
 *  This version number is generally updated only when changes have been
 *  made to the %AAX binary interface
 *  
 *  - The first byte is the major version number
 *  - The second byte is the minor version number
 *
 *  For example:
 *  - SDK 1.0.5  > \c 0x0100
 *  - SDK 10.2.1 > \c 0x0A02
 *
 *  \internal Update this value manually for all major SDK updates
 *
 */
#define AAX_SDK_VERSION ( 0x0204 )

/** \brief An atomic revision number for the source included in this SDK
 */
#define AAX_SDK_CURRENT_REVISION ( 20204000 )


#define AAX_SDK_1p0p1_REVISION ( 3712639 )
#define AAX_SDK_1p0p2_REVISION ( 3780585 )
#define AAX_SDK_1p0p3_REVISION ( 3895859 )
#define AAX_SDK_1p0p4_REVISION ( 4333589 )
#define AAX_SDK_1p0p5_REVISION ( 4598560 )
#define AAX_SDK_1p0p6_REVISION ( 5051497 )
#define AAX_SDK_1p5p0_REVISION ( 5740047 )
#define AAX_SDK_2p0b1_REVISION ( 6169787 )
#define AAX_SDK_2p0p0_REVISION ( 6307708 )
#define AAX_SDK_2p0p1_REVISION ( 6361692 )
#define AAX_SDK_2p1p0_REVISION ( 7820991 )
#define AAX_SDK_2p1p1_REVISION ( 8086416 )
#define AAX_SDK_2p2p0_REVISION ( 9967334 )
#define AAX_SDK_2p2p1_REVISION ( 10693954 )
#define AAX_SDK_2p2p2_REVISION ( 11819832 )
#define AAX_SDK_2p3p0_REVISION ( 12546840 )
#define AAX_SDK_2p3p1_REVISION ( 13200373 )
#define AAX_SDK_2p3p2_REVISION ( 14017972 )
#define AAX_SDK_2p4p0_REVISION ( 20204000 )
//CURREVSTAMP < do not remove this comment



#endif // #ifndef _AAX_VERSION_H_
