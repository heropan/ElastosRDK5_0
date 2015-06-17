
//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __ELSTRINGAPI_H__
#define __ELSTRINGAPI_H__

#include <elquintettype.h>

_ELASTOS_NAMESPACE_BEGIN

/** @addtogroup BaseTypesRef
  *   @{
  */
typedef enum _StringCase
{
    StringCase_Sensitive         = 0x0000,
    StringCase_Insensitive       = 0x0001,
} StringCase;

/** @} */

_ELASTOS_NAMESPACE_END

#ifdef __cplusplus
extern "C" {
#endif
    _ELASTOS Int32 __cdecl _String_Compare(const char *string1,
        const char *string2, _ELASTOS StringCase stringCase);

    _ELASTOS Int32 __cdecl _String_GetHashCode(const char *string);

    char* __cdecl _String_Duplicate(const char *strSource);
    void __cdecl _String_Free(char * str);

    _ELASTOS Char32 __cdecl _String_Char32_ToLowerCase(_ELASTOS Char32 ch);
    _ELASTOS Char32 __cdecl _String_Char32_ToUpperCase(_ELASTOS Char32 ch);
#ifdef __cplusplus
}
#endif

#endif //__ELSTRINGAPI_H__
