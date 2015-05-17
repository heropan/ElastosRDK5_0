
#include "StringBuffer.h"
#include <elastos/Character.h>
#include <stdio.h>
#include "StringBuilder.h"
#ifdef ELASTOS_CORE
#include "Elastos.Core_server.h"
#include "CStringWrapper.h"
#else
#include "Elastos.Core.h"
#endif

#define DEFAULT_STEP 16

namespace Elastos {
namespace Core {

UInt32 StringBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 StringBuffer::Release()
{
    return ElRefBase::Release();
}

PInterface StringBuffer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IStringBuffer*)this;
    }
    else if (riid == EIID_IStringBuffer) {
        return (IStringBuffer*)this;
    }
    else if (riid == EIID_ICharSequence) {
        return (ICharSequence*)this;
    }
    else if (riid == EIID_IAppendable) {
        return (IAppendable*)this;
    }

    return NULL;
}

ECode StringBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IStringBuffer *)this) {
        *pIID = EIID_IStringBuffer;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(ICharSequence *)this) {
        *pIID = EIID_ICharSequence;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IAppendable *)this) {
        *pIID = EIID_IAppendable;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

StringBuffer::StringBuffer()
    :AbstractStringBuilder()
{}

StringBuffer::StringBuffer(
    /* [in] */ Int32 capacity)
    :AbstractStringBuilder(capacity)
{
}

StringBuffer::StringBuffer(
    /* [in] */ const String& str)
    :AbstractStringBuilder(str)
{
}

StringBuffer::StringBuffer(
    /* [in] */ const char* str)
    :AbstractStringBuilder(String(str))
{
}

StringBuffer::~StringBuffer()
{}

ECode StringBuffer::ToASCIILowerCase()
{
    Mutex::Autolock lock(mLock);
    return ToASCIICaseString(FALSE);
}

ECode StringBuffer::ToASCIIUpperCase()
{
    Mutex::Autolock lock(mLock);
    return ToASCIICaseString(TRUE);
}

ECode StringBuffer::Reset()
{
    Mutex::Autolock lock(mLock);
    return SetLengthO(0);
}

ECode StringBuffer::GetLength(
    /* [out] */ Int32* length)
{
    Mutex::Autolock lock(mLock);
    VALIDATE_NOT_NULL(length);
    *length = GetLengthO();
    return NOERROR;
}

ECode StringBuffer::GetByteCount(
    /* [in] */ Int32* byteLength)
{
    Mutex::Autolock lock(mLock);
    VALIDATE_NOT_NULL(byteLength);
    *byteLength = GetByteCountO();
    return NOERROR;
}

ECode StringBuffer::GetCapacity(
    /* [out] */ Int32* capacity)
{
    Mutex::Autolock lock(mLock);
    VALIDATE_NOT_NULL(capacity);
    *capacity = GetCapacityO();
    return NOERROR;
}

ECode StringBuffer::EnsureCapacity(
    /* [in] */ Int32 min)
{
    Mutex::Autolock lock(mLock);
    return EnsureCapacityO(min);
}

ECode StringBuffer::TrimToSize()
{
    Mutex::Autolock lock(mLock);
    return TrimToSizeO();
}

ECode StringBuffer::SetChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 ch)
{
    Mutex::Autolock lock(mLock);
    return SetCharAtO(index, ch);
}

Char32 StringBuffer::GetChar(
    /* [in] */ Int32 index)
{
    Mutex::Autolock lock(mLock);
    Char32 ch;
    GetCharO(index, &ch);
    return ch;
}

ECode StringBuffer::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    Mutex::Autolock lock(mLock);
    return GetCharO(index, c);
}

ECode StringBuffer::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstStart)
{
    Mutex::Autolock lock(mLock);
    return GetCharsO(start, end, dst, dstStart);
}

ECode StringBuffer::Substring(
    /* [in] */ Int32 start,
    /* [out] */ String* str)
{
    Mutex::Autolock lock(mLock);
    return SubstringO(start, str);
}

ECode StringBuffer::SubstringEx(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ String* str)
{
    Mutex::Autolock lock(mLock);
    return SubstringExO(start, end, str);
}

String StringBuffer::Substring(
    /* [in] */ Int32 start)
{
    Mutex::Autolock lock(mLock);
    String str;
    SubstringO(start, &str);
    return str;
}

String StringBuffer::Substring(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Mutex::Autolock lock(mLock);
    String str;
    SubstringExO(start, end, &str);
    return str;
}

ECode StringBuffer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** seq)
{
    Mutex::Autolock lock(mLock);
    return SubSequenceO(start, end, seq);
}

ECode StringBuffer::IndexOf(
    /* [in] */ const String& string,
    /* [out] */ Int32* index)
{
    Mutex::Autolock lock(mLock);
    return IndexOfO(string, index);
}

ECode StringBuffer::IndexOfEx(
    /* [in] */ const String& subString,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index)
{
    Mutex::Autolock lock(mLock);
    return IndexOfExO(subString, start, index);
}

ECode StringBuffer::LastIndexOf(
    /* [in] */ const String& string,
    /* [out] */ Int32* index)
{
    Mutex::Autolock lock(mLock);
    return LastIndexOfO(string, index);
}

ECode StringBuffer::LastIndexOfEx(
    /* [in] */ const String& subString,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index)
{
    Mutex::Autolock lock(mLock);
    return LastIndexOfExO(subString, start, index);
}

ECode StringBuffer::AppendNULL()
{
    Mutex::Autolock lock(mLock);
    return AppendONULL();
}

ECode StringBuffer::AppendCStr(
    /* [in] */ const char* str)
{
    Mutex::Autolock lock(mLock);
    return AppendOCStr(str);
}

ECode StringBuffer::AppendBoolean(
    /* [in] */ Boolean b)
{
    Mutex::Autolock lock(mLock);
    return AppendOBoolean(b);
}

ECode StringBuffer::AppendChar(
    /* [in] */ Char32 c)
{
    Mutex::Autolock lock(mLock);
    return AppendOChar(c);
}

ECode StringBuffer::AppendInt32(
    /* [in] */ Int32 i)
{
    Mutex::Autolock lock(mLock);
    return AppendOInt32(i);
}

ECode StringBuffer::AppendInt64(
    /* [in] */ Int64 l)
{
    Mutex::Autolock lock(mLock);
    return AppendOInt64(l);
}

ECode StringBuffer::AppendFloat(
    /* [in] */ Float f)
{
    Mutex::Autolock lock(mLock);
    return AppendOFloat(f);
}

ECode StringBuffer::AppendDouble(
    /* [in] */ Double d)
{
    Mutex::Autolock lock(mLock);
    return AppendODouble(d);
}

ECode StringBuffer::AppendObject(
    /* [in] */ IInterface* obj)
{
    Mutex::Autolock lock(mLock);
    return AppendOObject(obj);
}

ECode StringBuffer::AppendString(
    /* [in] */ const String& str)
{
    Mutex::Autolock lock(mLock);
    return AppendOString(str);
}

ECode StringBuffer::AppendStringBuffer(
    /* [in] */ IStringBuffer* sb)
{
    Mutex::Autolock lock(mLock);
    if (NULL == sb) {
        return AppendNULL();
    }

    String str;
    sb->ToString(&str);
    return AppendOString(str);
}

ECode StringBuffer::AppendStringBuilder(
    /* [in] */ IStringBuilder* sb)
{
    Mutex::Autolock lock(mLock);
    if (NULL == sb) {
        return AppendNULL();
    }

    String str;
    sb->ToString(&str);
    return AppendOString(str);
}

ECode StringBuffer::AppendChars(
    /* [in] */ const ArrayOf<Char32>& chars)
{
    Mutex::Autolock lock(mLock);
    return AppendOChars(chars);
}

ECode StringBuffer::AppendCharsEx(
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    Mutex::Autolock lock(mLock);
    return AppendOCharsEx(chars, offset, length);
}

ECode StringBuffer::AppendCharSequence(
    /* [in] */ ICharSequence* csq)
{
    Mutex::Autolock lock(mLock);
    return AppendOCharSequence(csq);
}

ECode StringBuffer::AppendCharSequenceEx(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Mutex::Autolock lock(mLock);
    return AppendOCharSequenceEx(csq, start, end);
}

ECode StringBuffer::InsertChar(
    /* [in] */ Int32 offset,
    /* [in] */ Char32 c)
{
    Mutex::Autolock lock(mLock);
    return InsertOChar(offset, c);
}

ECode StringBuffer::InsertBoolean(
    /* [in] */ Int32 offset,
    /* [in] */ Boolean b)
{
    Mutex::Autolock lock(mLock);
    return InsertOBoolean(offset, b);
}

ECode StringBuffer::InsertInt32(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 i)
{
    Mutex::Autolock lock(mLock);
    return InsertOInt32(offset, i);
}

ECode StringBuffer::InsertInt64(
    /* [in] */ Int32 offset,
    /* [in] */ Int64 l)
{
    Mutex::Autolock lock(mLock);
    return InsertOInt64(offset, l);
}

ECode StringBuffer::InsertFloat(
    /* [in] */ Int32 offset,
    /* [in] */ Float f)
{
    Mutex::Autolock lock(mLock);
    return InsertOFloat(offset, f);
}

ECode StringBuffer::InsertDouble(
    /* [in] */ Int32 offset,
    /* [in] */ Double d)
{
    Mutex::Autolock lock(mLock);
    return InsertODouble(offset, d);
}

ECode StringBuffer::InsertObject(
    /* [in] */ Int32 offset,
    /* [in] */ IInterface* obj)
{
    Mutex::Autolock lock(mLock);
    return InsertOObject(offset, obj);
}

ECode StringBuffer::InsertString(
    /* [in] */ Int32 offset,
    /* [in] */ const String& str)
{
    Mutex::Autolock lock(mLock);
    return InsertOString(offset, str);
}

ECode StringBuffer::InsertChars(
    /* [in] */ Int32 offset,
    /* [in] */ const ArrayOf<Char32>& chars)
{
    Mutex::Autolock lock(mLock);
    return InsertOChars(offset, chars);
}

ECode StringBuffer::InsertCharsEx(
    /* [in] */ Int32 offset,
    /* [in] */ const ArrayOf<Char32>& str,
    /* [in] */ Int32 strOffset,
    /* [in] */ Int32 strLen)
{
    Mutex::Autolock lock(mLock);
    return InsertOCharsEx(offset, str, strOffset, strLen);
}

ECode StringBuffer::InsertCharSequence(
    /* [in] */ Int32 offset,
    /* [in] */ ICharSequence* s)
{
    Mutex::Autolock lock(mLock);
    return InsertOCharSequence(offset, s);
}

ECode StringBuffer::InsertCharSequenceEx(
    /* [in] */ Int32 offset,
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Mutex::Autolock lock(mLock);
    return InsertOCharSequenceEx(offset, s, start, end);
}

ECode StringBuffer::Replace(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const String& string)
{
    Mutex::Autolock lock(mLock);
    return ReplaceO(start, end, string);
}

ECode StringBuffer::Reverse()
{
    Mutex::Autolock lock(mLock);
    return ReverseO();
}

ECode StringBuffer::DeleteChar(
    /* [in] */ Int32 location)
{
    Mutex::Autolock lock(mLock);
    return DeleteOChar(location);
}

ECode StringBuffer::Delete(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Mutex::Autolock lock(mLock);
    return DeleteO(start, end);
}

ECode StringBuffer::ToString(
    /* [out] */ String* str)
{
    Mutex::Autolock lock(mLock);
    return SubstringO(0, str);
}

String StringBuffer::ToString()
{
    Mutex::Autolock lock(mLock);
    String str;
    SubstringO(0, &str);
    return str;
}

AutoPtr<ICharSequence> StringBuffer::ToCharSequence()
{
    Mutex::Autolock lock(mLock);
    String str;
    SubstringO(0, &str);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(str, (ICharSequence**)&seq);
    return seq;
}

Int32 StringBuffer::GetLength()
{
    Mutex::Autolock lock(mLock);
    return GetLengthO();
}

Int32 StringBuffer::GetByteCount()
{
    Mutex::Autolock lock(mLock);
    return GetByteCountO();
}

StringBuffer& StringBuffer::operator+=(const Boolean right)
{
    Mutex::Autolock lock(mLock);
    AppendOBoolean(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const char right)
{
    Mutex::Autolock lock(mLock);
    AppendOChar(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Char32 right)
{
    Mutex::Autolock lock(mLock);
    AppendOChar(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Int32 right)
{
    Mutex::Autolock lock(mLock);
    AppendOInt32(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Int64 right)
{
    Mutex::Autolock lock(mLock);
    AppendOInt64(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Float right)
{
    Mutex::Autolock lock(mLock);
    AppendOFloat(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Double right)
{
    Mutex::Autolock lock(mLock);
    AppendODouble(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const char* right)
{
    Mutex::Autolock lock(mLock);
    AppendOCStr(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const String& right)
{
    Mutex::Autolock lock(mLock);
    AppendOString(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(IInterface* right)
{
    Mutex::Autolock lock(mLock);
    AppendOObject(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(StringBuffer& right)
{
    Mutex::Autolock lock(mLock);
    AppendOString(right.ToString());
    return *this;
}

StringBuffer& StringBuffer::operator+=(StringBuilder& right)
{
    Mutex::Autolock lock(mLock);
    AppendOString(right.ToString());
    return *this;
}

} // namespace Core
} // namespace Elastos
