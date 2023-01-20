/*================================================================================================*/
/*
 *	Copyright 2009-2015, 2019 by Avid Technology, Inc.
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
 *	\file   AAX_Map.h
 *
 *	\author Mykola Kryvonos
 *
 */ 
/*================================================================================================*/

#pragma once

#ifndef AAX_MAP_H
#define AAX_MAP_H

#include "AAX.h"

#include AAX_ALIGN_FILE_BEGIN
#include AAX_ALIGN_FILE_ALG
#include AAX_ALIGN_FILE_END

//==============================================================================
class AAX_Map
{
public:
	AAX_Map() {};
	~AAX_Map() {};

	void SetCoefficients(int aSize, double* aInpX, double* aInpY);
	void GetCoefficient(int aIndex, double* aOutX, double* aOutY);
	int GetUpperBoundIndex(double inp);
	inline double GetX(int aIndex) {return mX[aIndex];};
	inline double GetY(int aIndex) {return mY[aIndex];};
	inline double GetFirstX() {return mX[0];};
	inline double GetFirstY() {return mY[0];};
	inline double GetLastX() {return mX[mSize - 1];};
	inline double GetLastY() {return mY[mSize - 1];};
	inline int GetSize() {return mSize;};

private:

	static const int mMaxSize = 13;

	int mSize;

	double mX[mMaxSize];
	double mY[mMaxSize];
};

#include AAX_ALIGN_FILE_BEGIN
#include AAX_ALIGN_FILE_RESET
#include AAX_ALIGN_FILE_END

#endif //AAX_MAP_H
