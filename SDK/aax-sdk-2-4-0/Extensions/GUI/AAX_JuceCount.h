/*================================================================================================*/
/*
 *	Copyright 2011-2015 by Avid Technology, Inc.
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
 *  \file AAX_JuceCount.h
 *
 */
/*================================================================================================*/

#ifndef AAX_JuceCount_H
#define AAX_JuceCount_H

class AAX_JuceCount
{
public:
	AAX_JuceCount ();
	virtual ~AAX_JuceCount ();
	
protected:
	static bool			IncrementJuceUseCout ();
	static bool			DecrementJuceUseCout ();
	
protected:
	static int32_t			sJuceUseCount;
};

#endif
