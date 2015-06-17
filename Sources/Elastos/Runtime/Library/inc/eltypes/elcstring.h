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

_ELASTOS_NAMESPACE_BEGIN

/** @addtogroup CARTypesRef
  *   @{
  */
class CString
{
public:
    // e.g., CString as;
    CString() : mString(NULL) {}

//  This class accords with bitwise copy semantics, and needn't declare copy
//  contructor.
//  //e.g., CString as(str);
//  CString(const CString& str) : mString(str.mString) {};

    // e.g., CString as("Hello");
    CString(const char* pch) : mString(pch) {}

    /**
     * @return the number of characters in this string.
     */
    UInt32 GetLength() const;

    /**
     * @return the number of bytes in this string.
     */
    inline UInt32 GetByteLength() const
    {
        if (IsNullOrEmpty()) return 0;
        return strlen(mString);
    }

    Int32 Compare(const char* other) const;
    Int32 CompareIgnoreCase(const char* other) const;

    inline Int32 Compare(const CString& other) const
    {
        return Compare(other.mString);
    }

    inline Int32 CompareIgnoreCase(const CString& other) const
    {
        return CompareIgnoreCase(other.mString);
    }

    inline Boolean Equals(const CString& other) const
    {
        return Compare(other) == 0;
    }

    inline Boolean Equals(const char* other) const
    {
        return Compare(other) == 0;
    }

    inline Boolean EqualsIgnoreCase(const CString& other) const
    {
        return CompareIgnoreCase(other) == 0;
    }

    inline Boolean EqualsIgnoreCase(const char* other) const
    {
        return CompareIgnoreCase(other) == 0;
    }

    Boolean IsNull() const {  // e.g., if (str.IsNull()) {...} or Boolean b = str.IsNull();
        return mString == NULL;
    }

    Boolean IsEmpty() const {
        assert(mString);
        return mString[0] == '\0';
    }

    Boolean IsNullOrEmpty() const {
        return (mString == NULL || mString[0] == '\0');
    }

    /*
     * @returns the character at the specified offset in this string.
     */
    Char32 GetChar(UInt32 charIndex) const;

    //---- Contains ----
    Boolean Contains(const char* str) const;
    Boolean Contains(const CString& str) const;
    Boolean StartWith(const char* str) const;
    Boolean StartWith(const CString& str) const;
    Boolean EndWith(const char* str) const;
    Boolean EndWith(const CString& str) const;

    Boolean ContainsIgnoreCase(const char* str) const;
    Boolean ContainsIgnoreCase(const CString& str) const;
    Boolean StartWithIgnoreCase(const char* str) const;
    Boolean StartWithIgnoreCase(const CString& str) const;
    Boolean EndWithIgnoreCase(const char* str) const;
    Boolean EndWithIgnoreCase(const CString& str) const;

    //---- IndexOf ----
    /*
     * @returns the byte offset of a specified character in this string.
     */
    Int32 ByteIndexOf(Char32 ch) const;

    Int32 IndexOf(const char* str, UInt32 startChar = 0) const;
    Int32 IndexOf(Char32 ch, UInt32 startChar = 0) const;
    Int32 IndexOf(const CString& str, UInt32 startChar = 0) const;

    Int32 IndexOfIgnoreCase(const char* str, UInt32 startChar = 0) const;
    Int32 IndexOfIgnoreCase(Char32 ch, UInt32 startChar = 0) const;
    Int32 IndexOfIgnoreCase(const CString& str, UInt32 startChar = 0) const;

    operator const char*() const
    {  //  for 3rd party API such as foo(char* pch);
        return (char *)mString;
    }

    void operator=(const CString& str) { // e.g., str1 = str2;
        mString = str.mString;
    }

    void operator=(const char* pch) {  // e.g., str = "Hello";
        mString = pch;
    }

    Int32 GetHashCode() {
        return _String_GetHashCode(mString);
    }

    static CString Duplicate(const CString& strSource) {
        return CString(_String_Duplicate(strSource));
    }

    static void Free(CString& str) {
        _String_Free((char *)str.mString);
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

    const char* mString;
};
/** @} */

_ELASTOS_NAMESPACE_END

#endif //__EZCSTRING_H__
