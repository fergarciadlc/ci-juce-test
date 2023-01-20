/*================================================================================================*/
/*
 *	Copyright 2015 by Avid Technology, Inc.
 *	All rights reserved.
 *
 *	CONFIDENTIAL: This document contains confidential information.  Do not 
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
 *	OF Avid Technology, INC.
 */
/*================================================================================================*/


// Self Include
#include "AAX_MIDILogging.h"

// AAX Includes
#include "AAX_CString.h"
#include "AAX_Assert.h"

// Standard Includes
#include <map>
#include <vector>
#include <algorithm>


#if defined(_DEBUG)
static uint8_t PV_StatusBitsToChannel(uint8_t inStatusBits)
{
	return static_cast<uint8_t>(inStatusBits & 0x0F);
}
#endif

static bool PV_DataBufferMatchesMask(uint32_t inLength, const uint8_t* inData, uint32_t inMask)
{
	bool itDoes = true;
	for (uint32_t i = 0; (true == itDoes) && (i < std::min(inLength, 4U)); ++i)
	{
		const int32_t shift = 8*(3-static_cast<int32_t>(i));
		itDoes = inData[i] == (inData[i] & ((inMask >> shift) & 0x000000FF));
	}
	return itDoes;
}

template <typename T>
struct SAutoArray
{
public:
	SAutoArray() : mData(NULL) {}
	~SAutoArray() { Reset(NULL); }
	void Reset(T* inData) { if (NULL != mData) delete[] mData; mData = inData; }
	T* Get() { return mData; }
	
private:
	uint8_t* mData;
};

class AAX_IMIDIMessageInfoDelegate
{
public:
	virtual ~AAX_IMIDIMessageInfoDelegate() {}
	
	virtual uint32_t Mask() const = 0; // 1x status byte, 3x data bytes
	virtual uint32_t Length() const = 0;
	virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const = 0;
	
	// By default, just use Mask() and Length()
	virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const
	{
		return (inLength == Length()) && (PV_DataBufferMatchesMask(inLength, inData, Mask()));
	}
		
protected:
	bool Accepts_ExactStatus(uint32_t inLength, const uint8_t* inData) const
	{
		return ((inData[0]) == ((Mask() >> 24) & 0x000000FF)) && AAX_IMIDIMessageInfoDelegate::Accepts(inLength, inData);
	}
	
	static void ToString_AppendNumber(const char* inLabel, int32_t inData, AAX_CString& outString)
	{
		ToString_AppendLabel(inLabel, outString);
		outString.AppendNumber(inData);
	}
	
	static void ToString_AppendCStr(const char* inLabel, const char* inCStr, AAX_CString& outString)
	{
		ToString_AppendLabel(inLabel, outString);
		outString.Append(inCStr);
	}
	
	static void ToString_AppendByteRange(const char* inLabel, const uint8_t* inData, int32_t inNumBytes, AAX_CString& outString)
	{
		ToString_AppendLabel(inLabel, outString);
		outString.Append("[");
		for (int32_t i = 0; i < inNumBytes; ++i)
		{
			if (0 < i) outString.Append(", ");
			outString.AppendHex(inData[i], 2);
		}
		outString.Append("]");
	}
	
	static void ToString_AppendValid(bool inCheck, AAX_CString& outString)
	{
		if (false == inCheck)
		{
			ToString_AppendCStr("valid", "NO", outString);
		}
	}
	
private:
	static void ToString_AppendLabel(const char* inLabel, AAX_CString& outString)
	{
		if (false == outString.Empty()) outString.Append(", ");
		outString.Append(inLabel);
		outString.Append(": ");
	}
};

#if defined(_DEBUG)
typedef std::map<uint32_t, std::vector<AAX_IMIDIMessageInfoDelegate*> > TMIDIMessageInfoDelegateVectorMap;

static const TMIDIMessageInfoDelegateVectorMap& PV_MessageInfoDelegates()
{
	static TMIDIMessageInfoDelegateVectorMap sDelegates;
	if (sDelegates.empty())
	{
		//
		// Pro Tools Messages
		//
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0x9001FFFF; }
				virtual uint32_t Length() const { return 2; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", 0x00 == inData[1] ? "Accented click" : "Unaccented click", infoStr);
					ToString_AppendValid(2 == inLength, infoStr);
					return infoStr;
				}
				
				virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const { return Accepts_ExactStatus(inLength, inData); }
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		
		//
		// Channel Voice Messages
		//
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0x8F7F7FFF; }
				virtual uint32_t Length() const { return 3; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Note off", infoStr);
					ToString_AppendNumber("chan", PV_StatusBitsToChannel(inData[0]), infoStr);
					ToString_AppendNumber("note", inData[1], infoStr);
					ToString_AppendNumber("vel", inData[2], infoStr);
					return infoStr;
				}
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0x9F7F7FFF; }
				virtual uint32_t Length() const { return 3; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Note on", infoStr);
					ToString_AppendNumber("chan", PV_StatusBitsToChannel(inData[0]), infoStr);
					ToString_AppendNumber("note", inData[1], infoStr);
					ToString_AppendNumber("vel", inData[2], infoStr);
					return infoStr;
				}
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xAF7F7FFF; }
				virtual uint32_t Length() const { return 3; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Polyphonic key pressure", infoStr);
					ToString_AppendNumber("chan", PV_StatusBitsToChannel(inData[0]), infoStr);
					ToString_AppendNumber("note", inData[1], infoStr);
					ToString_AppendNumber("pres", inData[2], infoStr);
					return infoStr;
				}
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xBF7F7FFF; }
				virtual uint32_t Length() const { return 3; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Control change", infoStr);
					ToString_AppendNumber("chan", PV_StatusBitsToChannel(inData[0]), infoStr);
					ToString_AppendNumber("ctrl", inData[1], infoStr);
					ToString_AppendNumber("val", inData[2], infoStr);
					return infoStr;
				}
				
				virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const
				{
					// Only accept controller values (third bit) less than 120
					return AAX_IMIDIMessageInfoDelegate::Accepts(inLength, inData) && (120 > inData[1]);
				}
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xCF7FFFFF; }
				virtual uint32_t Length() const { return 2; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Program change", infoStr);
					ToString_AppendNumber("chan", PV_StatusBitsToChannel(inData[0]), infoStr);
					ToString_AppendNumber("prog", inData[1], infoStr);
					ToString_AppendValid(2 == inLength, infoStr);
					return infoStr;
				}
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xDF7FFFFF; }
				virtual uint32_t Length() const { return 2; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Channel pressure", infoStr);
					ToString_AppendNumber("chan", PV_StatusBitsToChannel(inData[0]), infoStr);
					ToString_AppendNumber("pres", inData[1], infoStr);
					ToString_AppendValid(2 == inLength, infoStr);
					return infoStr;
				}
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xEF7F7FFF; }
				virtual uint32_t Length() const { return 3; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Pitch bend change", infoStr);
					ToString_AppendNumber("chan", PV_StatusBitsToChannel(inData[0]), infoStr);
					const int16_t bender = ((int16_t)inData[1]) | ((int16_t)inData[2] << 7);
					ToString_AppendNumber("bend", bender, infoStr);
					ToString_AppendValid(3 == inLength, infoStr);
					return infoStr;
				}
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		
		//
		// Channel Mode Messages
		//
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xBF7F7FFF; }
				virtual uint32_t Length() const { return 3; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Channel mode message", infoStr);
					ToString_AppendNumber("chan", PV_StatusBitsToChannel(inData[0]), infoStr);
					ToString_AppendNumber("ctrl", inData[1], infoStr);
					ToString_AppendNumber("val", inData[2], infoStr);
					ToString_AppendValid(3 == inLength, infoStr);
					
					switch (inData[1])
					{
						case 120:
							ToString_AppendCStr("cmd", "all sound off", infoStr);
							ToString_AppendValid(0 == inData[2], infoStr);
							break;
						case 121:
							ToString_AppendCStr("cmd", "reset all controllers", infoStr);
							break;
						case 122:
							if (0 == inData[2]) ToString_AppendCStr("cmd", "local control off", infoStr);
							else if (127 == inData[2]) ToString_AppendCStr("cmd", "local control on", infoStr);
							else ToString_AppendCStr("cmd", "local control", infoStr);
							ToString_AppendValid(0 == inData[2] || 127 == inData[2], infoStr);
							break;
						case 123:
							ToString_AppendCStr("cmd", "all notes off", infoStr);
							ToString_AppendValid(0 == inData[2], infoStr);
							break;
						case 124:
							ToString_AppendCStr("cmd", "omni mode off", infoStr);
							ToString_AppendValid(0 == inData[2], infoStr);
							break;
						case 125:
							ToString_AppendCStr("cmd", "omni mode on", infoStr);
							ToString_AppendValid(0 == inData[2], infoStr);
							break;
						case 126:
							ToString_AppendCStr("cmd", "mono mode on", infoStr);
							break;
						case 127:
							ToString_AppendCStr("cmd", "poly mode on", infoStr);
							ToString_AppendValid(0 == inData[2], infoStr);
							break;
						default:
							break;
					}
					
					return infoStr;
				}
				
				virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const
				{
					// Only accept controller values (third bit) greater than or equal to 120
					return AAX_IMIDIMessageInfoDelegate::Accepts(inLength, inData) && (120 <= inData[1]);
				}
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		
		//
		// System Common Messages
		//
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xF07F7FFF; }
				virtual uint32_t Length() const { return 3; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "System exclusive", infoStr);
					ToString_AppendByteRange("data", &(inData[1]), static_cast<int32_t>(inLength-1), infoStr);
					ToString_AppendValid((3 <= inLength) && (0xF7 == inData[inLength-1]), infoStr);
					return infoStr;
				}
				
				virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const
				{
					// Only check the beginning and end bytes of SysEx messages
					return (3 <= inLength) && (0xF0 == inData[0]) && (0XF7 == inData[inLength-1]);
				}
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xF17FFFFF; }
				virtual uint32_t Length() const { return 2; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "MTC quarter frame", infoStr);
					ToString_AppendNumber("msg", (inData[1] & 0xF0) >> 4, infoStr);
					ToString_AppendNumber("val", (inData[1] & 0x0F), infoStr);
					ToString_AppendValid(2 == inLength, infoStr);
					return infoStr;
				}
				
				virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const { return Accepts_ExactStatus(inLength, inData); }
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xF27F7FFF; }
				virtual uint32_t Length() const { return 3; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Song position pointer", infoStr);
					const int16_t position = ((int16_t)inData[1]) | ((int16_t)inData[2] << 7);
					ToString_AppendNumber("pos", position, infoStr);
					ToString_AppendValid(3 == inLength, infoStr);
					return infoStr;
				}
				
				virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const { return Accepts_ExactStatus(inLength, inData); }
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xF37FFFFF; }
				virtual uint32_t Length() const { return 2; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Song select", infoStr);
					ToString_AppendNumber("sel", inData[1], infoStr);
					ToString_AppendValid(2 == inLength, infoStr);
					return infoStr;
				}
				
				virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const { return Accepts_ExactStatus(inLength, inData); }
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xF6FFFFFF; }
				virtual uint32_t Length() const { return 1; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Tune request", infoStr);
					ToString_AppendValid(1 == inLength, infoStr);
					return infoStr;
				}
				
				virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const { return Accepts_ExactStatus(inLength, inData); }
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		
		//
		// System Real-Time Messages
		//
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xF8FFFFFF; }
				virtual uint32_t Length() const { return 1; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Timing clock", infoStr);
					ToString_AppendValid(1 == inLength, infoStr);
					return infoStr;
				}
				
				virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const { return Accepts_ExactStatus(inLength, inData); }
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xFAFFFFFF; }
				virtual uint32_t Length() const { return 1; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Start", infoStr);
					ToString_AppendValid(1 == inLength, infoStr);
					return infoStr;
				}
				
				virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const { return Accepts_ExactStatus(inLength, inData); }
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xFBFFFFFF; }
				virtual uint32_t Length() const { return 1; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Continue", infoStr);
					ToString_AppendValid(1 == inLength, infoStr);
					return infoStr;
				}
				
				virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const { return Accepts_ExactStatus(inLength, inData); }
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xFCFFFFFF; }
				virtual uint32_t Length() const { return 1; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Stop", infoStr);
					ToString_AppendValid(1 == inLength, infoStr);
					return infoStr;
				}
				
				virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const { return Accepts_ExactStatus(inLength, inData); }
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xFEFFFFFF; }
				virtual uint32_t Length() const { return 1; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Active sensing", infoStr);
					ToString_AppendValid(1 == inLength, infoStr);
					return infoStr;
				}
				
				virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const { return Accepts_ExactStatus(inLength, inData); }
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		{
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xFFFFFFFF; }
				virtual uint32_t Length() const { return 1; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "Reset", infoStr);
					ToString_AppendValid(1 == inLength, infoStr);
					return infoStr;
				}
				
				virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const { return Accepts_ExactStatus(inLength, inData); }
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
		
		
		//
		// Unknown Messages
		//
		
		{
			// This delegate matches any message, so it must be added last
			class AAX_CMIDIMessageInfoDelegateOfType : public AAX_IMIDIMessageInfoDelegate
			{
			public:
				virtual uint32_t Mask() const { return 0xFFFFFFFF; }
				virtual uint32_t Length() const { return 0; }
				
				virtual AAX_CString ToString(uint32_t inLength, const uint8_t* inData) const
				{
					AAX_CString infoStr;
					ToString_AppendCStr("type", "UNKNOWN", infoStr);
					ToString_AppendByteRange("data", &(inData[1]), static_cast<int32_t>(inLength-1), infoStr);
					return infoStr;
				}
				
				virtual bool Accepts(uint32_t inLength, const uint8_t* inData) const { return true; }
			};
			static AAX_CMIDIMessageInfoDelegateOfType sDelegate;
			sDelegates[sDelegate.Mask()].push_back(&sDelegate);
		}
	}
	
	return sDelegates;
}

static const AAX_IMIDIMessageInfoDelegate* PV_GetMessageInfoDelegate(uint32_t inLength, const uint8_t* inData)
{
	const AAX_IMIDIMessageInfoDelegate* foundDelegate = NULL;
	
	const TMIDIMessageInfoDelegateVectorMap& messageInfoDelegates = PV_MessageInfoDelegates();
	for (TMIDIMessageInfoDelegateVectorMap::const_iterator delegateVecIter = messageInfoDelegates.begin();
		 (NULL == foundDelegate) && (delegateVecIter != messageInfoDelegates.end());
		 ++delegateVecIter)
	{
		if (true == PV_DataBufferMatchesMask(inLength, inData, delegateVecIter->first))
		{
			const std::vector<AAX_IMIDIMessageInfoDelegate*>& delegateVector = delegateVecIter->second;
			for (std::vector<AAX_IMIDIMessageInfoDelegate*>::const_iterator delegateIter = delegateVector.begin();
				 (NULL == foundDelegate) && (delegateIter != delegateVector.end());
				 ++delegateIter)
			{
				const AAX_IMIDIMessageInfoDelegate* delegatePtr = *delegateIter;
				if (delegatePtr && delegatePtr->Accepts(inLength, inData))
				{
					foundDelegate = delegatePtr;
					break;
				}
			}
		}
	}
	
	return foundDelegate;
}

void AAX::AsStringMIDIStream_Debug(const AAX_CMidiStream& inStream, char* outBuffer, int32_t inBufferSize)
{
	// Demand at least 256 samples of padding in the buffer
	static const int32_t kPaddingBits = 256;
	
	int32_t curBufferWriteOffset = 0;
	for (uint32_t packetIdx = 0; (inBufferSize > (curBufferWriteOffset+kPaddingBits)) && (packetIdx < inStream.mBufferSize); ++packetIdx)
	{
		const AAX_CMidiPacket& curPacket = inStream.mBuffer[packetIdx];
		const AAX_IMIDIMessageInfoDelegate* delegate = NULL;
		
		SAutoArray<uint8_t> autoBuffer;
		const uint8_t* dataPtr = NULL;
		uint32_t length = 0;
		
		// Handle SysEx messages, which may be spread across more than one packet
		if (0xF0 == curPacket.mData[0])
		{
			// Find the SysEx message end token
			uint32_t numBytesInSysEx = 0;
			bool foundEndOfSysEx = false;
			uint32_t searchPacketIdx = packetIdx;
			for (; (false == foundEndOfSysEx) && (searchPacketIdx < inStream.mBufferSize); ++searchPacketIdx)
			{
				if (0xF7 == inStream.mBuffer[searchPacketIdx].mData[0]) { numBytesInSysEx += 1; foundEndOfSysEx = true; }
				else if (0xF7 == inStream.mBuffer[searchPacketIdx].mData[1]) { numBytesInSysEx += 2; foundEndOfSysEx = true; }
				else if (0xF7 == inStream.mBuffer[searchPacketIdx].mData[2]) { numBytesInSysEx += 3; foundEndOfSysEx = true; }
				else if (0xF7 == inStream.mBuffer[searchPacketIdx].mData[3]) { numBytesInSysEx += 4; foundEndOfSysEx = true; }
				else { numBytesInSysEx += 4; }
			}
			
			if (true == foundEndOfSysEx)
			{
				packetIdx = searchPacketIdx;
				autoBuffer.Reset(new uint8_t[numBytesInSysEx]);
				dataPtr = autoBuffer.Get();
				length = numBytesInSysEx;
			}
		}
		else
		{
			dataPtr = curPacket.mData;
			length = curPacket.mLength;
		}
		
		if (NULL != dataPtr)
		{
			delegate = PV_GetMessageInfoDelegate(length, dataPtr);
		}
		
		// Write the results to the log buffer
		{
			if (0 == packetIdx) curBufferWriteOffset += sprintf(outBuffer+curBufferWriteOffset, "[");
			else curBufferWriteOffset += sprintf(outBuffer+curBufferWriteOffset, ", ");
			if (inBufferSize <= curBufferWriteOffset+kPaddingBits) break;
			
			curBufferWriteOffset += sprintf(outBuffer+curBufferWriteOffset, "(%s)", delegate ? delegate->ToString(length, dataPtr).CString() : "UNKNOWN PACKET");
			if (inBufferSize <= curBufferWriteOffset+kPaddingBits) break;
			
			if (inStream.mBufferSize-1 == packetIdx) curBufferWriteOffset += sprintf(outBuffer+curBufferWriteOffset, "]");
			if (inBufferSize <= curBufferWriteOffset+kPaddingBits) break;
		}
	}
	
	if (0 < inBufferSize)
	{
		outBuffer[inBufferSize-1] = '\0';
	}
}
#else
void AAX::AsStringMIDIStream_Debug(const AAX_CMidiStream& inStream, char* outBuffer, int32_t inBufferSize) { if (0 < inBufferSize) outBuffer[0] = '\0'; }
#endif
