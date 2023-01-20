/*================================================================================================*/
/*
 *	Copyright 2013-2015, 2018 by Avid Technology, Inc.
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
 *	\file  AAX_MiscUtils.h
 *
 *	\brief Miscellaneous signal processing utilities
 *
 */ 
/*================================================================================================*/
#pragma once

#ifndef AAX_MISCUTILS_H
#define AAX_MISCUTILS_H

#include "AAX_PlatformOptimizationConstants.h"
#include "AAX_Constants.h"

#if defined(_MSC_VER)
	#define DECLARE_ALIGNED(t,v,x)      __declspec(align(x)) t v
//	#define DECLARE_ALIGNED(t,v,x)      t v
#elif defined (__GNUC__)
	#define DECLARE_ALIGNED(t,v,x)      t v __attribute__ ((aligned (x)))
//	#define DECLARE_ALIGNED(t,v,x)      t v
#elif defined (_TMS320C6X)
	#define DECLARE_ALIGNED(t,v,x)      t v
	#warn "DECLARE_ALIGNED macro does not currently align data on TI"
#else
	#error "DigiError: Please port the DECLARE_ALIGNED macro to this platform"
#endif


/*!
 *  \brief Currently only functional on TI, these word alignments will provide better
 *	performance on TI.
 *
 */	
#ifdef _TMS320C6X
#define AAX_ALIGNMENT_HINT(a,b)		std::_nassert(((int)(a) % b) == 0)
#define AAX_WORD_ALIGNED_HINT(a)		AAX_ALIGNMENT_HINT(a,4)
#define AAX_DWORD_ALIGNED_HINT(a)		AAX_ALIGNMENT_HINT(a,8)
#else
#define AAX_ALIGNMENT_HINT(a,b)
#define AAX_WORD_ALIGNED_HINT(a)
#define AAX_DWORD_ALIGNED_HINT(a)
#endif

/*!
 *  \brief These macros are used on TI to convert 2 single words accesses to one double word access
 *	to provide additional optimization
 *
 */	
#ifdef _TMS320C6X
#define AAX_LO(x)                 (_itof(_lo((_amemd8_const(&x)))))
#define AAX_HI(x)                 (_itof(_hi((_amemd8_const(&x)))))
#define AAX_INT_LO(x)             (_lo((_amemd8_const(&x))))
#define AAX_INT_HI(x)             (_hi((_amemd8_const(&x))))
#else	//for non-TI systems increment pointer by 4-bytes to simulate hi-word access
#define AAX_LO(x)                 x
#define AAX_HI(x)                 *((const_cast<float*>(&x))+1)
#define AAX_INT_LO(x)             x
#define AAX_INT_HI(x)             *((const_cast<int32_t*>(reinterpret_cast<const int32_t*>(&x)))+1)
#endif




namespace AAX
{

template<class GFLOAT>
inline GFLOAT ClampToZero(GFLOAT iValue, GFLOAT iClampThreshold)
{
	return (iValue < iClampThreshold && iValue > -iClampThreshold) ? 0.0 : iValue;
}

/*template<class GFLOAT>
class SmoothingFilter
{
public:
	SmoothingFilter() { Reset(); }
	void Reset(void) { mYnm1=0.0; }
	void Reset(double iInitialState) { mYnm1=iInitialState; }
	void SetSmothingRate(double iFrequency, double iSampleRate) { mZeroCoef = 1.0-std::exp(iFrequency*-cTwoPi/iSampleRate); }
	void SetSmoothingCoeff(GFLOAT b0) { mZeroCoef = b0; }
	inline GFLOAT Smooth(GFLOAT iInput) 
    { 
        mYnm1 += mZeroCoef * (iInput - mYnm1);
        DeDenormal(mYnm1); 
        return (GFLOAT)mYnm1;
    }

private:
	double mYnm1; // y[n-1]
	GFLOAT mZeroCoef;
};*/

inline void ZeroMemory(void* iPointer, int iNumBytes)
{
	char* aCharPointer=static_cast<char*>(iPointer);

	for(int aIndex=0; aIndex<iNumBytes; aIndex++)
	{
		*aCharPointer=0;
		aCharPointer++;
	}
}

//Warning: the number of bytes passed in must be multiple of 4
inline void ZeroMemoryDW(void* iPointer, int iNumBytes)
{
	int* aDWPointer=static_cast<int*>(iPointer);

	int numDWords=iNumBytes/sizeof(int);
	for(int aIndex=0; aIndex<numDWords; aIndex++)
	{
		*aDWPointer=0;
		aDWPointer++;
	}
}

template<typename T, int N> void Fill( T *iArray, const T* iVal )
{
//	std::fill_n( iArray, N, iVal );
	for (int i = 0; i != N; ++i)
	{
		*(iArray + i) = *iVal;
	}
}

template< typename T, int M, int N > inline void  Fill( T *iArray, const T* iVal )//Fill( T (&iArray)[M][N], const T& iVal )
{
	for ( int i = 0; i != M; ++i )
	{
		Fill( iArray + i, iVal );
	}
}

template< typename T, int L, int M, int N > inline void  Fill( T *iArray, const T* iVal ) //Fill( T (&iArray)[L][M][N], const T& iVal )
{
	for ( int i = 0; i != L; ++i )
	{
		Fill( iArray + i, iVal );
	}
}

static const int cSignBitWord = cLittleEndian;

double 
inline fabs(double iVal)
{
	//On Windows this version is slower than std::fabs so use that instead.
#if defined(MAC_VERSION)
	double aVal = iVal;
	int* tempptr = (reinterpret_cast<int*>(&aVal))+cSignBitWord;
	*tempptr &= 0x7fffffff;

	return aVal;
#else
	return std::fabs(iVal);
#endif
}

float 
inline fabs(float iVal)
{
	// Call intrinsic if on TI c6727. fabs is about the same speed as std::fabs,
	// although metrowerks v9.5 MSL libraries are much slower so for the time 
	// being were using this.
	int temp = (*(reinterpret_cast<int*>(&iVal)) & 0x7fffffff);
	return *reinterpret_cast<float*>(&temp);
}

float 
inline fabsf(float iVal)
{
	return AAX::fabs (iVal);
}

template <class T> 
inline T AbsMax(const T& iValue, const T& iMax)
{
	return std::fabs(iValue) < std::fabs(iMax) ? iMax : iValue;
}

template <class T> 
inline T MinMax(const T& iValue, const T& iMin, const T& iMax)
{
	return iValue > iMax ? iMax : (iValue < iMin ? iMin : iValue);
}

template <class T> 
inline T Max(const T& iValue1, const T& iValue2)
{
	return iValue1 > iValue2 ? iValue1 : iValue2;
}
	
template <class T> 
inline T Min(const T& iValue1, const T& iValue2)
{
	return iValue1 < iValue2 ? iValue1 : iValue2;
}

template <class T> 
inline T Sign(const T& iValue)
{
	return iValue < (T)(0.0) ? (T)(-1.0) : (T)(1.0);
}

//Polynomial evaluation.
inline double PolyEval(double x, const double* coefs, int numCoefs)
{
	// Coefs are ordered from highest degree to lowest (Matlab convention)
	if(numCoefs < 1) return 0.0;
	
	double result = coefs[0];
	for(int i = 1; i < numCoefs; ++i)
	{
		result = result*x + coefs[i];
	};

	return result;
}

inline double CeilLog2(double iValue)
{
	return std::pow(2.0f, (float)(std::ceil(std::log(iValue)/std::log(2.0f))));
}

inline void SinCosMix(float aLinearMix, float &aSinMix, float &aCosMix)
{
	aSinMix=static_cast< float >( std::sin(aLinearMix*cHalfPi) );
	aCosMix=static_cast< float >( std::cos(aLinearMix*cHalfPi) );
}


} // namespace AAX


// Some methods have been broken off into AAX_Denormal.h; including
// AAX_Denormal.h here for compatibility with projects that have not
// yet been updated to include this new header.
#include "AAX_Denormal.h"

#endif // AAX_MISCUTILS_H
