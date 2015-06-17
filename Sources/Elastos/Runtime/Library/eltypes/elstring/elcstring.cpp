
#include <car.h>
#include <elcstring.h>
#include <elstring.h>
#include <elquintet.h>

_ELASTOS_NAMESPACE_BEGIN

UInt32 CString::GetLength() const
{
    if (IsNullOrEmpty()) return 0;

    UInt32 charCount = 0;
    Int32 byteLength;
    const char* p = mString;
    const char *pEnd = mString + GetByteLength() + 1;
    while (p && *p && p < pEnd) {
        byteLength = String::UTF8SequenceLength(*p);
        if (!byteLength || p + byteLength >= pEnd) break;
        p += byteLength;
        ++charCount;
    }

    return charCount;
}

Int32 CString::Compare(const char* other) const
{
    if (mString == other) return 0;
    if (!mString) return -2;
    if (!other) return 2;

    return strcmp(mString, other);
}

Int32 CString::CompareIgnoreCase(const char* other) const
{
    if (mString == other) return 0;
    if (!mString) return -2;
    if (!other) return 2;

    UInt32 strLength = strlen(mString);
    UInt32 subLength = strlen(other);

    const char* p1 = mString;
    const char* p1End = mString + strLength;
    const char* p2 = other;
    const char* p2End = other + subLength;
    UInt32 len1, len2;
    Char32 ch1, ch2;

    while (p1 < p1End && p2 < p2End) {
        if (String::IsASCII(*p1)) {
            if (*p1 != *p2 && String::ToUpperCase(*p1) != String::ToUpperCase(*p2))
                return *p1 - *p2;
            ++p1; ++p2;
        }
        else {
            ch1 = String::GetCharInternal(p1, &len1);
            ch2 = String::GetCharInternal(p2, &len2);
            if (ch1 != ch2 && String::ToUpperCase(ch1) != String::ToUpperCase(ch2))
                return ch1 - ch2;
            p1 += len1; p2 += len2;
        }
    }

    return *p1 - *p2;
}

Char32 CString::GetChar(UInt32 index) const
{
    if (IsNullOrEmpty()) return String::INVALID_CHAR;

    UInt32 byteLength, i = 0;
    const char* p = mString;
    const char *pEnd = mString + GetByteLength() + 1;
    while (p && *p && p < pEnd) {
        byteLength = String::UTF8SequenceLength(*p);
        if (!byteLength || p + byteLength >= pEnd) break;
        if ((i++) == index) {
            return String::GetCharInternal(p, &byteLength);
        }
        p += byteLength;
    }

    // index out of bounds
    return String::INVALID_CHAR;
}

Boolean CString::Contains(const char* subString) const
{
    if (!mString || !subString) return FALSE;
    if (subString[0] == '\0') return TRUE;
    return (NULL != strstr(mString, subString));
}

Boolean CString::Contains(const CString& subString) const
{
    return Contains(subString.mString);
}

Boolean CString::StartWith(const char* subString) const
{
    if (!mString || !subString) return FALSE;
    if (subString[0] == '\0') return TRUE;
    return !strncmp(mString, subString, strlen(subString));
}

Boolean CString::StartWith(const CString& subString) const
{
    return StartWith(subString.mString);
}

Boolean CString::EndWith(const char* subString) const
{
    if (!mString || !subString) return FALSE;
    if (subString[0] == '\0') return TRUE;

    const char *endString = mString + strlen(mString) - strlen(subString);
    if (endString < mString) return FALSE;
    return !strncmp(endString, subString, strlen(subString));
}

Boolean CString::EndWith(const CString& subString) const
{
    return EndWith(subString.mString);
}

Boolean CString::ContainsIgnoreCase(const char* subString) const
{
    return IndexOfIgnoreCase(subString, 0) != -1;
}

Boolean CString::ContainsIgnoreCase(const CString& subString) const
{
    return ContainsIgnoreCase(subString.mString);
}

Boolean CString::StartWithIgnoreCase(const char* subString) const
{
    return String::InsensitiveStartWith(mString, subString);
}
Boolean CString::StartWithIgnoreCase(const CString& subString) const
{
    return StartWithIgnoreCase(subString.mString);
}

Boolean CString::EndWithIgnoreCase(const char* subString) const
{
    if (!mString || !subString) return FALSE;
    if (mString[0] == '\0' && subString[0] == '\0') return TRUE;

    const char *endString = mString + strlen(mString) - strlen(subString);
    if (endString < mString) return FALSE;

    return String::InsensitiveStartWith(endString, subString);
}

Boolean CString::EndWithIgnoreCase(const CString& subString) const
{
    return EndWithIgnoreCase(subString.mString);
}

//---- IndexOf ----

Int32 CString::ByteIndexOf(Char32 target) const
{
    if (!String::IsValidChar(target)) return -1;
    if (IsNullOrEmpty()) return -1;

    UInt32 byteLength;
    const char* p = mString;
    const char *pEnd = mString + GetByteLength() + 1;

    Char32 ch;
    while (p && *p && p < pEnd) {
        ch = String::GetCharInternal(p, &byteLength);
        if (!byteLength || p + byteLength >= pEnd) break;
        if (ch == target) return p - mString;
        p += byteLength;
    }

    return -1;
}

Int32 CString::IndexOf(Char32 target, UInt32 start) const
{
    if (!String::IsValidChar(target)) return -1;

    UInt32 byteCount = GetByteLength();
    if (start >= byteCount) return -1;

    UInt32 byteLength, i = 0;
    const char* p = mString;
    const char *pEnd = mString + byteCount + 1;
    Char32 ch;

    while (p && *p && p < pEnd) {
        if (i < start) {
            byteLength = String::UTF8SequenceLength(*p);
            if (!byteLength || p + byteLength >= pEnd) break;
        }
        else {
            ch = String::GetCharInternal(p, &byteLength);
            if (!byteLength || p + byteLength >= pEnd) break;
            if (ch == target) return i;
        }

        p += byteLength;
        ++i;
    }

    return -1;
}

Int32 CString::IndexOfIgnoreCase(Char32 target, UInt32 start) const
{
    if (!String::IsValidChar(target)) return -1;

    UInt32 byteCount = GetByteLength();
    if (start >= byteCount) return -1;

    UInt32 byteLength, i = 0;
    const char* p = mString;
    const char *pEnd = mString + byteCount + 1;
    const Char32 upperTarget = String::ToUpperCase(target);
    Char32 ch;

    while (p && *p && p < pEnd) {
        if (i < start) {
            byteLength = String::UTF8SequenceLength(*p);
            if (!byteLength || p + byteLength >= pEnd) break;
        }
        else {
            if (String::IsASCII(*p)) {
                byteLength = 1;
                if (p + byteLength >= pEnd) break;
                if (*p == target || String::ToUpperCase(*p) == upperTarget)
                    return i;
            }
            else {
                ch = String::GetCharInternal(p, &byteLength);
                if (!byteLength || p + byteLength >= pEnd) break;
                if (ch == target || String::ToUpperCase(ch) == upperTarget)
                    return i;
            }
        }

        p += byteLength;
        ++i;
    }

    return -1;
}

Int32 CString::IndexOf(const char* subString, UInt32 start) const
{
    return String::SensitiveIndexOf(mString, subString, start);
}

Int32 CString::IndexOf(const CString& subString, UInt32 start) const
{
    return IndexOf(subString.mString, start);
}

Int32 CString::IndexOfIgnoreCase(const char* subString, UInt32 start) const
{
    return String::InsensitiveIndexOf(mString, subString, start);
}

Int32 CString::IndexOfIgnoreCase(const CString& subString, UInt32 start) const
{
    return IndexOfIgnoreCase(subString.mString, start);
}

_ELASTOS_NAMESPACE_END
