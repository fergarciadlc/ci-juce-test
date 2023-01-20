/*===================================================================================================
AAX_JuceAdditionalDefines.h

Copyright 2015, 2017 by Avid Technology, Inc. 
All rights reserved.
===================================================================================================*/

#if defined _MSC_VER
#if _MSC_VER <= 1900
#define fseeko fseek
#define ftello ftell
#endif
#endif
