
#include "StringBuffer.h"
#include "Character.h"//<elastos/Character.h>
#include <stdio.h>
#include "StringBuilder.h"
#ifdef ELASTOS_CORELIBRARY
#include "CStringWrapper.h"
#endif

#define DEFAULT_STEP 16

namespace Elastos {
namespace Core {

CAR_INTERFACE_IMPL(StringBuilder, AbstractStringBuilder, IStringBuffer)

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
    return AbstractStringBuilder::ToASCIICaseString(FALSE);
}

ECode StringBuffer::ToASCIIUpperCase()
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::ToASCIICaseString(TRUE);
}

ECode StringBuffer::Reset()
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::SetLength(0);
}

ECode StringBuffer::GetLength(
    /* [out] */ Int32* length)
{
    Mutex::Autolock lock(mLock);
    VALIDATE_NOT_NULL(length);
    *length = GetLength();
    return NOERROR;
}

ECode StringBuffer::GetByteCount(
    /* [in] */ Int32* byteLength)
{
    Mutex::Autolock lock(mLock);
    VALIDATE_NOT_NULL(byteLength);
    *byteLength = GetByteCount();
    return NOERROR;
}

ECode StringBuffer::GetCapacity(
    /* [out] */ Int32* capacity)
{
    Mutex::Autolock lock(mLock);
    VALIDATE_NOT_NULL(capacity);
    *capacity = GetCapacity();
    return NOERROR;
}

ECode StringBuffer::EnsureCapacity(
    /* [in] */ Int32 min)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::EnsureCapacity(min);
}

ECode StringBuffer::TrimToSize()
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::TrimToSize();
}

ECode StringBuffer::SetChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 ch)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::SetChar(index, ch);
}

Char32 StringBuffer::GetChar(
    /* [in] */ Int32 index)
{
    Mutex::Autolock lock(mLock);
    Char32 ch;
    AbstractStringBuilder::GetChar(index, &ch);
    return ch;
}

ECode StringBuffer::GetChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::GetChar(index, c);
}

ECode StringBuffer::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstStart)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::GetCharsO(start, end, dst, dstStart);
}

ECode StringBuffer::Substring(
    /* [in] */ Int32 start,
    /* [out] */ String* str)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::SubstringO(start, str);
}

ECode StringBuffer::Substring(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ String* str)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::SubstringExO(start, end, str);
}

String StringBuffer::Substring(
    /* [in] */ Int32 start)
{
    Mutex::Autolock lock(mLock);
    String str;
    AbstractStringBuilder::SubstringO(start, &str);
    return str;
}

String StringBuffer::Substring(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Mutex::Autolock lock(mLock);
    String str;
    AbstractStringBuilder::SubstringExO(start, end, &str);
    return str;
}

ECode StringBuffer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** seq)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::SubSequenceO(start, end, seq);
}

ECode StringBuffer::IndexOf(
    /* [in] */ const String& string,
    /* [out] */ Int32* index)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::IndexOfO(string, index);
}

ECode StringBuffer::IndexOf(
    /* [in] */ const String& subString,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::IndexOfExO(subString, start, index);
}

ECode StringBuffer::LastIndexOf(
    /* [in] */ const String& string,
    /* [out] */ Int32* index)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::LastIndexOfO(string, index);
}

ECode StringBuffer::LastIndexOf(
    /* [in] */ const String& subString,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::LastIndexOfExO(subString, start, index);
}

ECode StringBuffer::AppendNULL()
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::AppendNULL();
}

ECode StringBuffer::Append(
    /* [in] */ const char* str)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::AppendOCStr(str);
}

ECode StringBuffer::Append(
    /* [in] */ Boolean b)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::AppendOBoolean(b);
}

ECode StringBuffer::AppendChar(
    /* [in] */ Char32 c)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::AppendOChar(c);
}

ECode StringBuffer::Append(
    /* [in] */ Int32 i)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::AppendOInt32(i);
}

ECode StringBuffer::Append(
    /* [in] */ Int64 l)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::AppendOInt64(l);
}

ECode StringBuffer::Append(
    /* [in] */ Float f)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::AppendOFloat(f);
}

ECode StringBuffer::Append(
    /* [in] */ Double d)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::AppendODouble(d);
}

ECode StringBuffer::Append(
    /* [in] */ IInterface* obj)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::AppendOObject(obj);
}

ECode StringBuffer::Append(
    /* [in] */ const String& str)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::AppendOString(str);
}

ECode StringBuffer::Append(
    /* [in] */ IStringBuffer* sb)
{
    Mutex::Autolock lock(mLock);
    if (NULL == sb) {
        return AbstractStringBuilder::AppendNULL();
    }

    String str;
    ((StringBuffer*)sb)->ToString(&str);
    return AbstractStringBuilder::AppendOString(str);
}

ECode StringBuffer::Append(
    /* [in] */ IStringBuilder* sb)
{
    Mutex::Autolock lock(mLock);
    if (NULL == sb) {
        return AbstractStringBuilder::AppendNULL();
    }

    String str;
    ((StringBuffer*)sb)->ToString(&str);
    return AbstractStringBuilder::AppendOString(str);
}

ECode StringBuffer::Append(
    /* [in] */ const ArrayOf<Char32>& chars)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::AppendOChars(chars);
}

ECode StringBuffer::Append(
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::AppendOCharsEx(chars, offset, length);
}

ECode StringBuffer::Append(
    /* [in] */ ICharSequence* csq)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::AppendOCharSequence(csq);
}

ECode StringBuffer::Append(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::AppendOCharSequenceEx(csq, start, end);
}

ECode StringBuffer::InsertChar(
    /* [in] */ Int32 offset,
    /* [in] */ Char32 c)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::InsertOChar(offset, c);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Boolean b)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::InsertOBoolean(offset, b);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 i)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::InsertOInt32(offset, i);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Int64 l)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::InsertOInt64(offset, l);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Float f)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::InsertOFloat(offset, f);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Double d)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::InsertODouble(offset, d);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ IInterface* obj)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::InsertOObject(offset, obj);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ const String& str)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::InsertOString(offset, str);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ const ArrayOf<Char32>& chars)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::InsertOChars(offset, chars);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ const ArrayOf<Char32>& str,
    /* [in] */ Int32 strOffset,
    /* [in] */ Int32 strLen)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::InsertOCharsEx(offset, str, strOffset, strLen);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ ICharSequence* s)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::InsertOCharSequence(offset, s);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::InsertOCharSequenceEx(offset, s, start, end);
}

ECode StringBuffer::Replace(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const String& string)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::ReplaceO(start, end, string);
}

ECode StringBuffer::Reverse()
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::ReverseO();
}

ECode StringBuffer::Delete(
    /* [in] */ Int32 location)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::DeleteOChar(location);
}

ECode StringBuffer::Delete(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::DeleteO(start, end);
}

ECode StringBuffer::ToString(
    /* [out] */ String* str)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::SubstringO(0, str);
}

String StringBuffer::ToString()
{
   String str;
   ToString(&str);
   return str;
}

AutoPtr<ICharSequence> StringBuffer::ToCharSequence()
{
    Mutex::Autolock lock(mLock);
    String str;
    AbstractStringBuilder::SubstringO(0, &str);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(str, (ICharSequence**)&seq);
    return seq;
}

Int32 StringBuffer::GetLength()
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::GetLength();
}

Int32 StringBuffer::GetByteCount()
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::GetByteCount();
}

StringBuffer& StringBuffer::operator+=(const Boolean right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::AppendOBoolean(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const char right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::AppendOChar(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Char32 right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::AppendOChar(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Int32 right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::AppendOInt32(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Int64 right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::AppendOInt64(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Float right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::AppendOFloat(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Double right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::AppendODouble(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const char* right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::AppendOCStr(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const String& right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::AppendOString(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(IInterface* right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::AppendOObject(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(StringBuffer& right)
{
    Mutex::Autolock lock(mLock);
    String str;
    right.ToString(&str);
    AbstractStringBuilder::AppendOString(str);
    return *this;
}

StringBuffer& StringBuffer::operator+=(StringBuilder& right)
{
    Mutex::Autolock lock(mLock);
    String str;
    right.ToString(&str);
    AbstractStringBuilder::AppendOString(str);
    return *this;
}

} // namespace Core
} // namespace Elastos
