
#include "coredef.h"
#include "AbstractStringBuilder.h"
#include <elstring.h>
#include "Math.h"
#include "RealToString.h"
#include "IntegralToString.h"
#include "Character.h"
#include "ObjectUtils.h"
#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#include "CStringWrapper.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

#if defined(_DEBUG) || defined(_ELASTOS_DEBUG)
#include <stdio.h>
#endif

#define DEFAULT_STEP 16

namespace Elastos {
namespace Core {

AbstractStringBuilder::AbstractStringBuilder()
    :mByteCount(0)
{
    Init(NULL, DEFAULT_STEP);
}

AbstractStringBuilder::AbstractStringBuilder(
    /* [in] */ Int32 capacity)
    :mByteCount(0)
{
    Init(NULL, capacity);
}

AbstractStringBuilder::AbstractStringBuilder(
    /* [in] */ const String& str)
    :mByteCount(str.IsNull() ? 0 : str.GetByteLength())
{
    Init((const char *)str, DEFAULT_STEP);
}

AbstractStringBuilder::~AbstractStringBuilder()
{
    if (NULL != mString) {
        delete [] mString;
    }
}

void AbstractStringBuilder::Init(
    /* [in] */ const char *s,
    /* [in] */ Int32 step)
{
    mCharCount = 0;
    mIsCounted = FALSE;
    mShared = FALSE;
    mStep = step;
    mCapacity = mByteCount + mStep + 1;
    mString = new char[mCapacity];
    strcpy(mString, s ? s : "\0");
}

void AbstractStringBuilder::Break(
    /* [in] */ Int32 size)
{
    assert(size >= 0);

    Int32 newLength = mByteCount + size + 1;
    if (newLength > mCapacity) {
        EnlargeBuffer(newLength);
    }
}

ECode AbstractStringBuilder::EnlargeBuffer(
    /* [in] */ Int32 min)
{
    assert(min >= 0);

    Int32 newLength = ((mCapacity >> 1) + mCapacity) + 2;
    mCapacity = min > newLength ? min : newLength;
    char * newData = new char[mCapacity];
    strcpy(newData, mString);
    delete [] mString;
    mString = newData;
    mShared = FALSE;

    return NOERROR;
}

ECode AbstractStringBuilder::EnsureCapacityO(
    /* [in] */ Int32 min)
{
    if (min > mCapacity) {
        int ourMin = mCapacity * 2 + 2;
        EnlargeBuffer(Math::Max(ourMin, min));
    }

    return NOERROR;
}

ECode AbstractStringBuilder::TrimToSizeO()
{
    if (mByteCount < mCapacity && mCapacity > DEFAULT_STEP) {
        mCapacity = Math::Max(DEFAULT_STEP, mByteCount + 1);
        char * newData = new char[mCapacity];
        strcpy(newData, mString);
        delete [] mString;
        mString = newData;
        mShared = FALSE;
    }
    return NOERROR;
}

ECode AbstractStringBuilder::SetLengthO(
    /* [in] */ Int32 length)
{
    if (length < 0) {
        return E_STRING_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (length == 0) {
        memset(mString, 0, mByteCount);
        mByteCount = 0;
        mCharCount = 0;
        mIsCounted = TRUE;
        return NOERROR;
    }

    Int32 charCount = GetLengthO();
    if (length == charCount) {
        return NOERROR;
    }

    if (length > charCount) {
        Int32 diff = length - charCount;
        mByteCount += diff;
        EnsureCapacityO(mByteCount);
    }
    else {
        CString cstr(mString);
        Int32 byteIndex = 0, nextByteIndex = 0;
        Char32 ch;
        Character::GetOffsetByChars(cstr, 0, length - 1, &byteIndex);
        Character::GetCharAt(cstr, byteIndex, &ch, &nextByteIndex);

        Int32 cutByteCount = nextByteIndex - mByteCount;
        if (cutByteCount > 0) {
            memset(mString + mByteCount, 0, nextByteIndex - mByteCount);
            mByteCount -= cutByteCount;
            mIsCounted = FALSE;
        }
    }

    return NOERROR;
}

Int32 AbstractStringBuilder::GetLengthO()
{
    if (!mIsCounted) {
        mIsCounted = TRUE;
        ArrayOf<Char8> bytes(mString, mByteCount);
        Character::GetCharCount(bytes, 0, mByteCount, &mCharCount);
    }

    return mCharCount;
}

Int32 AbstractStringBuilder::GetByteCountO()
{
    return mByteCount;
}

Int32 AbstractStringBuilder::GetCapacityO()
{
    return mCapacity;
}

ECode AbstractStringBuilder::GetCharO(
    /* [in] */ Int32 index,
    /* [out] */ Char32* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 charCount = GetLengthO();
    if (index < 0 || index >= charCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    CString cstr(mString);
    Int32 byteIndex = 0;
    Character::GetOffsetByChars(cstr, 0, index, &byteIndex);
    return Character::GetCharAt(cstr, byteIndex, result);
}

ECode AbstractStringBuilder::GetCharsO(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstStart)
{
    VALIDATE_NOT_NULL(dst);

    CString cstr(mString);
    return Character::ToChar32s(cstr, start, end - start, dst, dstStart);
}

ECode AbstractStringBuilder::AppendONULL()
{
    return AppendOCStr("NULL");
}

ECode AbstractStringBuilder::AppendOCStr(
    /* [in] */ char const* str)
{
    if (NULL == str) {
        return AppendONULL();
    }

    String temp(str);
    return AppendOString(temp);
}

ECode AbstractStringBuilder::AppendOBoolean(
    /* [in] */ Boolean b)
{
    return AppendOCStr(b ? "TRUE" : "FALSE");
}

ECode AbstractStringBuilder::AppendOInt32(
    /* [in] */ Int32 i)
{
    String str = IntegralToString::Int32ToString(i);
    return AppendOString(str);
}

ECode AbstractStringBuilder::AppendOInt64(
    /* [in] */ Int64 l)
{
    String str = IntegralToString::Int64ToString(l);
    return AppendOString(str);
}

ECode AbstractStringBuilder::AppendOFloat(
    /* [in] */ Float f)
{
    String str = RealToString::GetInstance()->FloatToString(f);
    return AppendOString(str);
}

ECode AbstractStringBuilder::AppendODouble(
    /* [in] */ Double d)
{
    String str = RealToString::GetInstance()->DoubleToString(d);
    return AppendOString(str);
}

ECode AbstractStringBuilder::AppendOChars(
    /* [in] */ const ArrayOf<Char32>& chars)
{
    return AppendOCharsEx(chars, 0, chars.GetLength());
}

ECode AbstractStringBuilder::AppendOCharsEx(
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    if (offset < 0 || length < 0 || offset + length > chars.GetLength()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 appendSize = 0;
    for (Int32 i = 0; i < length; ++i) {
        appendSize += Character::GetByteCount(chars[offset + i]);
    }

    Break(appendSize);

    for (Int32 i = 0; i < length; ++i) {
        AppendOChar(chars[offset + i]);
    }

    return NOERROR;
}

ECode AbstractStringBuilder::AppendOChar(
    /* [in] */ Char32 ch)
{
    ArrayOf_<Char8, 5> buf;
    Int32 len;
    Character::ToChars(ch, buf, 0, &len);

    Break(len);

    strncat(mString, (const char*)buf.GetPayload(), len);
    mByteCount += len;
    if (mIsCounted) {
        mCharCount += 1;
    }

    return NOERROR;
}

ECode AbstractStringBuilder::AppendOString(
    /* [in] */ const String& str)
{
    if (str.IsNull()) {
        return AppendONULL();
    }

    Int32 length = str.GetByteLength();
    Break(length);

    strncat(mString, (const char*)str, length);
    mByteCount += length;
    mIsCounted = FALSE;

    return NOERROR;
}

ECode AbstractStringBuilder::AppendOCharSequence(
    /* [in] */ ICharSequence* csq)
{
    if (csq == NULL) {
        return AppendONULL();
    }

    Int32 length = 0;
    csq->GetLength(&length);

    AutoPtr< ArrayOf<Char32> > charArray = ArrayOf<Char32>::Alloc(length);
    for (Int32 i = 0; i < length; ++i) {
        csq->GetCharAt(i, &(*charArray)[i]);
    }

    return AppendOChars(*charArray);
}

ECode AbstractStringBuilder::AppendOCharSequenceEx(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (csq == NULL) {
        return AppendONULL();
    }

    if ((start | end) < 0 || start > end) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 length = 0;
    csq->GetLength(&length);
    if (start >= length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (end > length) {
        end = length;
    }

    length = end - start;
    if (length == 0) {
        return NOERROR;
    }

    AutoPtr< ArrayOf<Char32> > charArray = ArrayOf<Char32>::Alloc(length);
    for (Int32 i = 0; i < length; ++i) {
        csq->GetCharAt(start + i, &(*charArray)[i]);
    }

    return AppendOChars(*charArray);
}

ECode AbstractStringBuilder::AppendOObject(
    /* [in] */ IInterface* obj)
{
    if (obj) {
        ICharSequence * seq = ICharSequence::Probe(obj);
        if (seq) {
            return AppendOCharSequence(seq);
        }

        IObject * des = IObject::Probe(obj);
        if (des) {
            String str;
            des->ToString(&str);
            return AppendOString(str);
        }
    }

    return AppendONULL();
}

ECode AbstractStringBuilder::InsertOBoolean(
    /* [in] */ Int32 offset,
    /* [in] */ Boolean b)
{
    if (offset < 0 || offset > mByteCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    String str(b? "TRUE" : "FALSE");
    return InsertOString(offset, str);
}

ECode AbstractStringBuilder::InsertOInt32(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 i)
{
    if (offset < 0 || offset > mByteCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    char str[12];
    sprintf(str, "%d", i);
    return InsertOString(offset, String(str));
}

ECode AbstractStringBuilder::InsertOInt64(
    /* [in] */ Int32 offset,
    /* [in] */ Int64 l)
{
    if (offset < 0 || offset > mByteCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    char str[24];
    sprintf(str, "%lld", l);
    return InsertOString(offset, String(str));
}

ECode AbstractStringBuilder::InsertOFloat(
    /* [in] */ Int32 offset,
    /* [in] */ Float f)
{
    if (offset < 0 || offset > mByteCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    String str = RealToString::GetInstance()->FloatToString(f);
    return InsertOString(offset, str);
}

ECode AbstractStringBuilder::InsertODouble(
    /* [in] */ Int32 offset,
    /* [in] */ Double d)
{
    if (offset < 0 || offset > mByteCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    String str = RealToString::GetInstance()->DoubleToString(d);
    return InsertOString(offset, str);
}

ECode AbstractStringBuilder::InsertOChar(
    /* [in] */ Int32 offset,
    /* [in] */ Char32 c)
{
    if (offset < 0 || offset > mByteCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    AutoPtr< ArrayOf<Char8> > char8s;
    Character::ToChars(c, (ArrayOf<Char8>**)&char8s);

    char str[5];
    Int32 length = char8s->GetLength();
    for (Int32 i = 0; i < length; ++i) {
        str[i] = (*char8s)[i];
    }
    str[length] = '\0';
    return InsertOString(offset, String(str));
}

ECode AbstractStringBuilder::InsertOChars(
    /* [in] */ Int32 offset,
    /* [in] */ const ArrayOf<Char32>& chars)
{
    if (offset < 0 || offset > mByteCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    return InsertOCharsEx(offset, chars, 0, chars.GetLength());
}

void AbstractStringBuilder::GenerateNewBuffer(
    /* [in] */ Int32 capacity)
{
    assert(capacity > 0);

    mCapacity = capacity;
    char * newData = new char[mCapacity];
    memset(newData, 0, mCapacity);
    delete [] mString;
    mString = newData;
    mByteCount = 0;
    mCharCount = 0;
    mIsCounted = TRUE;
    mShared = FALSE;
}

ECode AbstractStringBuilder::InsertOCharsEx(
    /* [in] */ Int32 offset,
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [in] */ Int32 start,
    /* [in] */ Int32 length)
{
    if (offset < 0 || offset > mByteCount || start < 0 || length < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 charCount = GetLengthO();
    if (offset > charCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 charsLength = chars.GetLength();
    if (start >= charsLength) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (offset == charCount) {
        return AppendOCharsEx(chars, start, length);
    }
    else {
        Int32 end = start + length;
        if (end > charsLength) {
            end = charsLength ;
        }

        CString cstr(mString);
        AutoPtr< ArrayOf<Char32> > oldChars = ArrayOf<Char32>::Alloc(charCount);
        Character::ToChar32s(cstr, 0, charCount, oldChars, 0);

        Int32 insertSize = 0;
        for (Int32 i = start; i < end; ++i) {
            insertSize += Character::GetByteCount(chars[i]);
        }

        GenerateNewBuffer(mByteCount + insertSize + 1);

        for(Int32 i = 0; i < offset; ++i) {
            AppendOChar((*oldChars)[i]);
        }

        for (Int32 i = start; i < end; ++i) {
            AppendOChar(chars[i]);
        }

        for (Int32 i = offset; i < charCount; ++i) {
            AppendOChar((*oldChars)[i]);
        }
    }

    return NOERROR;
}

ECode AbstractStringBuilder::InsertOString(
    /* [in] */ Int32 offset,
    /* [in] */ const String& str)
{
    if (offset < 0 || offset > mByteCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 charCount = GetLengthO();
    if (offset > charCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (offset == charCount) {
        return AppendOString(str);
    }
    else if (offset == 0) {
        Int32 strLength = str.GetByteLength();
        Int32 newCapacity = mByteCount + strLength + 1;
        char * newData = new char[newCapacity];
        memcpy(newData, str, strLength);
        memcpy(newData + strLength, mString, mByteCount);
        newData[newCapacity - 1] = '\0';
        delete [] mString;
        mString = newData;
        mByteCount += strLength;
        mIsCounted = FALSE;
        mCapacity = newCapacity;

        return NOERROR;
    }

    CString cstr(mString);
    AutoPtr< ArrayOf<Char32> > chars = ArrayOf<Char32>::Alloc(charCount);
    Character::ToChar32s(cstr, 0, charCount, chars, 0);

    Int32 strLength = str.GetByteLength();
    GenerateNewBuffer(mByteCount + strLength + 1);

    for(Int32 i = 0; i < offset; ++i) {
        AppendOChar((*chars)[i]);
    }

    AppendOString(str);

    for (Int32 i = offset; i < charCount; ++i) {
        AppendOChar((*chars)[i]);
    }
    return NOERROR;
}

ECode AbstractStringBuilder::InsertOCharSequence(
    /* [in] */ Int32 offset,
    /* [in] */ ICharSequence* seq)
{
    if (seq == NULL) {
        return InsertOString(offset, String("NULL"));
    }

    Int32 seqLen;
    seq->GetLength(&seqLen);
    return InsertOCharSequenceEx(offset, seq, 0, seqLen);
}

ECode AbstractStringBuilder::InsertOCharSequenceEx(
    /* [in] */ Int32 offset,
    /* [in] */ ICharSequence* seq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (seq == NULL) {
        return InsertOString(offset, String("NULL"));
    }

    if (offset < 0 || offset > mByteCount || start < 0 || end < 0 || start > end) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (start == end) {
        return NOERROR;
    }

    Int32 seqLen;
    seq->GetLength(&seqLen);
    if (start >= seqLen) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (end > seqLen) {
        end = seqLen;
    }

    Char32 ch;
    Int32 charsLength = end - start;
    AutoPtr< ArrayOf<Char32> > chars = ArrayOf<Char32>::Alloc(charsLength);
    for (Int32 i = start; i < end; ++i) {
        seq->GetCharAt(i, &ch);
        (*chars)[i - start] = ch;
    }

    return InsertOChars(offset, *chars);
}

ECode AbstractStringBuilder::InsertOObject(
    /* [in] */ Int32 offset,
    /* [in] */ IInterface* obj)
{
    if (obj) {
        ICharSequence * seq = ICharSequence::Probe(obj);
        if (seq) {
            return AppendOCharSequence(seq);
        }

        IObject * des = IObject::Probe(obj);
        if (des) {
            String str;
            des->ToString(&str);
            return InsertOString(offset, str);
        }
    }

    return InsertOString(offset, String("NULL"));
}

ECode AbstractStringBuilder::ReplaceO(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const String& string)
{
    if (string.IsNull()) return E_NULL_POINTER_EXCEPTION;

    if (start < 0 || end < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 charCount = GetLengthO();
    if (end > charCount) {
        end = charCount;
    }

    if (end > start) {
        CString cstr(mString);
        AutoPtr< ArrayOf<Char32> > chars = ArrayOf<Char32>::Alloc(charCount);
        Character::ToChar32s(cstr, 0, charCount, chars, 0);

        Int32 replaceByteSize = 0;
        for (Int32 i = start; i < end; i++) {
            replaceByteSize += Character::GetByteCount((*chars)[i]);
        }

        mCapacity += (string.GetByteLength() - replaceByteSize);

        GenerateNewBuffer(mCapacity);

        for(Int32 i = 0; i < start; ++i) {
            AppendOChar((*chars)[i]);
        }

        AppendOString(string);

        for (Int32 i = end; i < charCount; ++i) {
            AppendOChar((*chars)[i]);
        }
        return NOERROR;;
    }

    if (end == start) {
        return InsertOString(start, string);
    }

    // end < start
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode AbstractStringBuilder::ReverseO()
{
    const char* prev = mString;
    mString = new char[mCapacity];
    memset(mString, 0, mCapacity);
    mShared = FALSE;
    mByteCount = 0;
    mCharCount = 0;
    mIsCounted = TRUE;
    CString prevStr(prev);

    Int32 charCount = GetLengthO();
    Int32 len = prevStr.GetLength();
    Character::GetCharCount(prevStr, 0, len, &charCount);
    AutoPtr< ArrayOf<Char32> > chars = ArrayOf<Char32>::Alloc(charCount);
    Character::ToChar32s(prevStr, 0, charCount, chars, 0);

    for (Int32 i = charCount - 1; i >= 0; --i) {
        AppendOChar((*chars)[i]);
    }

    delete [] prev;
    return NOERROR;
}

ECode AbstractStringBuilder::SetCharAtO(
    /* [in] */ Int32 index,
    /* [in] */ Char32 ch)
{
    if (index < 0 || index > mByteCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 charCount = GetLengthO();
    if (index >= charCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    CString cstr(mString);
    AutoPtr< ArrayOf<Char32> > chars = ArrayOf<Char32>::Alloc(charCount);
    Character::ToChar32s(cstr, 0, charCount, chars, 0);
    Char32 oldCh = (*chars)[index];
    (*chars)[index] = ch;

    Int32 oldChByteCount = Character::GetByteCount(oldCh);
    Int32 newChByteCount = Character::GetByteCount(ch);
    mCapacity += newChByteCount - oldChByteCount;

    GenerateNewBuffer(mCapacity);

    for(Int32 i = 0; i < charCount; i++) {
        AppendOChar((*chars)[i]);
    }
    return NOERROR;
}

ECode AbstractStringBuilder::DeleteO(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (start > end) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (start < 0 || end < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 charCount = GetLengthO();
    if (start >= charCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (end > charCount) {
        end = charCount;
    }

    CString cstr(mString);
    AutoPtr< ArrayOf<Char32> > chars = ArrayOf<Char32>::Alloc(charCount);
    Character::ToChar32s(cstr, 0, charCount, chars, 0);

    Int32 deleteSize = 0;
    for (Int32 i = start; i < end; i++) {
        deleteSize += Character::GetByteCount((*chars)[i]);
    }

    GenerateNewBuffer(mCapacity - deleteSize);

    for(Int32 i = 0; i < start; ++i) {
        AppendOChar((*chars)[i]);
    }
    for (Int32 i = end; i < charCount; ++i) {
        AppendOChar((*chars)[i]);
    }
    return NOERROR;
}

ECode AbstractStringBuilder::DeleteOChar(
    /* [in] */ Int32 index)
{
    return DeleteO(index, index + 1);
}

ECode AbstractStringBuilder::SubstringO(
    /* [in] */ Int32 start,
    /* [out] */ String* str)
{
    Int32 charCount = GetLengthO();
    return SubstringExO(start, charCount, str);
}

ECode AbstractStringBuilder::SubstringExO(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    Int32 charCount = GetLengthO();
    if (start < 0 || end < 0 || start > end || start > charCount ) {
        *str = String(NULL);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (end == 0 || start == end) {
        *str = "";
        return NOERROR;
    }

    if (end > charCount) {
        end = charCount;
    }

    Int32 count = 0;
    char *p = mString;
    char *pEnd = p + mByteCount + 1;
    char *p1 = p, *p2 = pEnd;
    char *p3 = p;

    if (start == mByteCount) {
        p1 += mByteCount;
    }
    else {
        Int32 charlen;
        while (p && *p && p < pEnd) {
            charlen = UTF8SequenceLength(*p);
            if (!charlen || p + charlen >= pEnd) break;
            else p += charlen;

            if (count == start) {
                p1 = p3;
            }
            if (count == end) {
                p2 = p3;
                break;
            }

            count++;
            p3 = p;
        }

        if (p2 - mString > mByteCount) {
            p2 = mString + mByteCount;
        }
    }

    Int32 len = p2 - p1;
    char *substr = (char *)malloc(len + 1);
    strncpy(substr, p1, len);
    *(substr + len) = '\0';
    *str = String(substr);
    free(substr);
    return NOERROR;
}

ECode AbstractStringBuilder::SubSequenceO(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** seq)
{
    VALIDATE_NOT_NULL(seq);
    *seq = NULL;

    String str;
    FAIL_RETURN(SubstringExO(start, end, &str));
    FAIL_RETURN(CStringWrapper::New(str, seq));
    return NOERROR;
}

ECode AbstractStringBuilder::IndexOfO(
    /* [in] */ const String& subString,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return IndexOfExO(subString, 0, index);
}

ECode AbstractStringBuilder::IndexOfExO(
    /* [in] */ const String& subString,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = -1;

    if (start < 0) {
        start = 0;
    }

    Int32 charLength = GetLengthO();
    if (start >= charLength) {
        return NOERROR;
    }

    Int32 byteStart = 0;
    Int32 subByteLength = subString.GetByteLength();

    if (subByteLength > 0) {
        Int32 charlen;
        Int32 charCount = 0;
        char *p = mString;
        char *pEnd = p + mByteCount + 1;

        while (p && *p && p < pEnd) {
            if (charCount == start) {
                byteStart = p - mString;
                break;
            }

            charlen = UTF8SequenceLength(*p);
            if (!charlen || p + charlen >= pEnd) {
                return NOERROR;
            }
            else {
                p += charlen;
                charCount++;
            }
        }

        if (subByteLength + byteStart > mByteCount) {
            return NOERROR;
        }

        Int32 i, j;
        Int32 charStart = start;

        Boolean found;
        while (TRUE) {
            i = byteStart;
            found = FALSE;
            for (; i < mByteCount;) {
                p = mString + i;
                charlen = UTF8SequenceLength(*p);
                if (!charlen || p + charlen >= pEnd) {
                    return NOERROR;
                }
                else {
                    found = TRUE;
                    for (j = 0; j < charlen; ++j) {
                        if (*(p + j) != subString[j]) {
                            found = FALSE;
                            break;
                        }
                    }
                    if (found) {
                        if (j == subByteLength) {
                            *index = charStart;
                            return NOERROR;
                        }
                        break;
                    }
                    else {
                        i += charlen;
                        charStart++;
                    }
                }
            }

            if (!found || subByteLength + i > mByteCount) {
                return NOERROR; // handles subByteLength > count || start >= count
            }

            Int32 o1 = i, o2 = 0;
            while (++o2 < subByteLength && mString[++o1] == subString[o2]) {
                // Intentionally empty
            }
            if (o2 == subByteLength) {
                *index = charStart;
                return NOERROR;
            }

            if (found) {
                i += charlen;
                charStart++;
            }
            byteStart = i;
        }
    }

    *index = (byteStart < mByteCount || start == 0) ? start : charLength;
    return NOERROR;
}

ECode AbstractStringBuilder::LastIndexOfO(
    /* [in] */ const String& subString,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    Int32 charCount = GetLengthO();
    return LastIndexOfExO(subString, charCount - 1, index);
}

ECode AbstractStringBuilder::LastIndexOfExO(
    /* [in] */ const String& subString,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = -1;

    if (start < 0) {
        return NOERROR;
    }

    Int32 charLength = GetLengthO();
    if (start >= charLength) {
        start = charLength - 1;
    }

    Int32 byteStart = 0;
    Int32 subByteLength = subString.GetByteLength();
    if (subByteLength <= mByteCount) {
        if (subByteLength > 0) {
            // Record the length of each unicode-character.
            AutoPtr<ArrayOf<Int32> > charLens = ArrayOf<Int32>::Alloc(start);

            Int32 charlen;
            Int32 charCount = 0;
            char *p = mString;
            char *pEnd = p + mByteCount + 1;

            while (p && *p && p < pEnd) {
                if (charCount == start) {
                    byteStart = p - mString;
                    break;
                }

                charlen = UTF8SequenceLength(*p);
                (*charLens)[charCount] = charlen;
                if (!charlen || p + charlen >= pEnd) {
                    return NOERROR;
                }
                else {
                    p += charlen;
                    charCount++;
                }
            }

            if (byteStart > mByteCount - subByteLength) {
                byteStart = mByteCount - subByteLength; // count and subCount are both
                Int32 total = 0;
                for (Int32 i = 0; i < start; ++i) {
                    total += (*charLens)[i];
                    if (total >= byteStart) {
                        start = i + 1;
                        byteStart = total;
                        break;
                    }
                }
            }

            // >= 1
            // TODO optimize charAt to direct array access

            Int32 i, j;
            Int32 charStart = start;
            Boolean found;
            charlen = 0;
            while (TRUE) {
                i = byteStart;
                found = FALSE;
                for (; i >= 0;) {
                    p = mString + i;
                    if (charlen == 0) {
                        charlen = UTF8SequenceLength(*p);
                    }
                    if (!charlen || p + charlen >= pEnd) {
                        return NOERROR;
                    }
                    else {
                        found = TRUE;
                        for (j = 0; j < charlen; ++j) {
                            if (*(p + j) != subString[j]) {
                                found = FALSE;
                                break;
                            }
                        }
                        if (found) {
                            if (j == subByteLength) {
                                *index = charStart;
                                return NOERROR;
                            }
                            break;
                        }
                        else {
                            if (--charStart < 0) {
                                return NOERROR;
                            }
                            i -= (*charLens)[charStart];
                        }
                    }
                }

                if (!found) {
                    return NOERROR;
                }

                Int32 o1 = i, o2 = 0;
                while (++o2 < subByteLength && mString[++o1] == subString[o2]) {
                    // Intentionally empty
                }
                if (o2 == subByteLength) {
                    *index = charStart;
                    return NOERROR;
                }

                if (found) {
                    if (--charStart < 0) {
                        return NOERROR;
                    }
                    i -= (*charLens)[charStart];
                }
                byteStart = i;
            }
        }

        *index = (byteStart < mByteCount) ? start : charLength;
    }

    return NOERROR;
}

ECode AbstractStringBuilder::ToASCIICaseString(
    /* [in] */ Boolean isToUpperCase)
{
    char *p = mString;
    char *pEnd = p + mByteCount + 1;
    Int32 charlen;
    while (p && *p && p < pEnd) {
        charlen = UTF8SequenceLength(*p);
        if (!charlen || p + charlen >= pEnd) {
            break;
        }

        if (IsASCII(*p)) {
            if (isToUpperCase) {
                if ('a' <= *p && *p <= 'z') {
                    *p = *p - ('a' - 'A');
                }
            }
            else {
                if ('A' <= *p && *p <= 'Z') {
                    *p = *p + ('a' - 'A');
                }
            }
        }

        p += charlen;
    }

    return NOERROR;
}

void AbstractStringBuilder::DbgPrint()
{
#if defined(_DEBUG) || defined(_ELASTOS_DEBUG)
    printf(" >> AbstractStringBuilder::DbgPrint()\n");
    printf("    mCapacity: %d\n", mCapacity);
    printf("    mByteCount: %d\n", mByteCount);
    printf("    mCharCount: %d\n", GetLengthO());
    printf("    mString: %s\n", mString);
#endif
}

} // namespace Core
} // namespace Elastos
