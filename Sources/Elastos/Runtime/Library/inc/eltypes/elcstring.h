//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __EZCSTRING_H__
#define __EZCSTRING_H__

#include <elstringapi.h>
#include <assert.h>

#ifdef __cplusplus

_ELASTOS_NAMESPACE_BEGIN

/** @addtogroup CARTypesRef
  *   @{
  */
class CString
{
public:
    // e.g., CString as;
    CString() : m_string(NULL) {}

//  This class accords with bitwise copy semantics, and needn't declare copy
//  contructor.
//  //e.g., CString as(str);
//  CString(const CString& str) : m_string(str.m_string) {};

    // e.g., CString as("Hello");
    CString(const char* pch) : m_string(pch) {}

    Int32 GetLength(Int32 maxLen = -1) const { // e.g., as.Length(64);
        return _String_GetLength(m_string, maxLen);
    }

    Int32 Compare(const CString& str, StringCase stringCase = StringCase_Sensitive) const { //e.g., as.Compare(str);
        assert(0);
        return -1;
    }

    Int32 Equals(const CString& str) const {
        return !Compare(str, StringCase_Sensitive);
    }

    Int32 EqualsIgnoreCase(const CString& str) const {
        return !Compare(str, StringCase_Insensitive);
    }

    Boolean IsNull() const {  // e.g., if (str.IsNull()) {...} or Boolean b = str.IsNull();
        return m_string == NULL;
    }

    Boolean IsEmpty() const {
        assert(m_string);
        return m_string[0] == '\0';
    }

    Boolean IsNullOrEmpty() const {
        return (m_string == NULL || m_string[0] == '\0');
    }

    Int32 GetCharCount() const {
        assert(0);
        return -1;
    }

    Char32 GetChar(Int32 index) const {
        assert(0);
        return 0;
    }

    Int32 GetCharByOffset(UInt32 offset, UInt32 *nextOffset = NULL) const {
        assert(0);
        return -1;
    }

    //---- Contains ----
    Boolean Contains(const CString&  substr, StringCase stringCase = StringCase_Sensitive) const {
        assert(0);
        return FALSE;
    }

    Boolean StartWith(const CString&  substr,
            StringCase stringCase = StringCase_Sensitive) const {
        assert(0);
        return FALSE;
    }

    Boolean EndWith(const CString& substr,
            StringCase stringCase = StringCase_Sensitive) const {
        assert(0);
        return FALSE;
    }

    //---- IndexOf ----
    Int32 IndexOf(Char8 ch, Int32 start = 0,
        StringCase stringCase = StringCase_Sensitive) const {
        assert(0);
        return -1;
    }

    Int32 IndexOfAny(const CString& strCharSet, Int32 start = 0,
            StringCase stringCase = StringCase_Sensitive) const {
        assert(0);
        return -1;
    }

    Int32 IndexOfChar(Char32 ch, Int32 start = 0, StringCase stringCase  = StringCase_Sensitive) const {
        assert(0);
        return -1;
    }

    Int32 IndexOfAnyChar(Char32 *strCharSet, Int32 start = 0,
            StringCase stringCase = StringCase_Sensitive) const {
        assert(0);
        return -1;
    }

    Int32 IndexOf(const CString& str, Int32 start = 0, StringCase stringCase  = StringCase_Sensitive) const {
        assert(0);
        return -1;
    }

    //---- LastIndexOf ----
    Int32 LastIndexOf(Char8 ch, Int32 start = 0,
            StringCase stringCase = StringCase_Sensitive) const {
        assert(0);
        return -1;
    }

    Int32 LastIndexOfAny(const CString& strCharSet, Int32 start = 0,
            StringCase stringCase = StringCase_Sensitive) const {
        assert(0);
        return -1;
    }

    Int32 LastIndexOfChar(Char32 ch, Int32 start = 0,
        StringCase stringCase  = StringCase_Sensitive) const {
        assert(0);
        return -1;
    }

    Int32 LastIndexOfAnyChar(Char32 *strCharSet, Int32 start = 0,
            StringCase stringCase = StringCase_Sensitive) const {
        assert(0);
        return -1;
    }

    Int32 LastIndexOf(const CString& str, Int32 start = 0,
        StringCase stringCase  = StringCase_Sensitive) const {
        assert(0);
        return -1;
    }

    operator const char*() const
    {  //  for 3rd party API such as foo(char* pch);
        return (char *)m_string;
    }

    void operator=(const CString& str) { // e.g., str1 = str2;
        m_string = str.m_string;
    }

    void operator=(const char* pch) {  // e.g., str = "Hello";
        m_string = pch;
    }

    const Char8 operator[](Int32 idx) const {
        assert(idx >= 0);
        return m_string[idx];
    }

    Int32 GetHashCode() {
        return _String_GetHashCode(m_string);
    }

    static CString Duplicate(const CString& strSource) {
        return CString(_String_Duplicate(strSource));
    }

    static void Free(CString& str) {
        _String_Free((char *)str.m_string);
    }

protected:
    void operator==(const char *) {}
    void operator!=(const char *) {}
    void operator!() {}
    void operator*() {}

    void operator+=(const char *) {}
    void operator+(const char *) {}
    void operator+=(const int) {}
    void operator-=(const int) {}
    void operator+(const int) {}
    void operator-(const int) {}

    const char* m_string;
};
/** @} */

_ELASTOS_NAMESPACE_END

#else //!__cplusplus

typedef const char* CString;

#endif //__cplusplus

#endif //__EZCSTRING_H__
