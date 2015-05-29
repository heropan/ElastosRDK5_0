
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
    return AbstractStringBuilder::GetChars(start, end, dst, dstStart);
}

ECode StringBuffer::Substring(
    /* [in] */ Int32 start,
    /* [out] */ String* str)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Substring(start, str);
}

ECode StringBuffer::Substring(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ String* str)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Substring(start, end, str);
}

String StringBuffer::Substring(
    /* [in] */ Int32 start)
{
    Mutex::Autolock lock(mLock);
    String str;
    AbstractStringBuilder::Substring(start, &str);
    return str;
}

String StringBuffer::Substring(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Mutex::Autolock lock(mLock);
    String str;
    AbstractStringBuilder::Substring(start, end, &str);
    return str;
}

ECode StringBuffer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** seq)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::SubSequence(start, end, seq);
}

ECode StringBuffer::IndexOf(
    /* [in] */ const String& string,
    /* [out] */ Int32* index)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::IndexOf(string, index);
}

ECode StringBuffer::IndexOf(
    /* [in] */ const String& subString,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::IndexOf(subString, start, index);
}

ECode StringBuffer::LastIndexOf(
    /* [in] */ const String& string,
    /* [out] */ Int32* index)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::LastIndexOf(string, index);
}

ECode StringBuffer::LastIndexOf(
    /* [in] */ const String& subString,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::LastIndexOf(subString, start, index);
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
    return AbstractStringBuilder::Append(str);
}

ECode StringBuffer::Append(
    /* [in] */ Boolean b)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Append(b);
}

ECode StringBuffer::AppendChar(
    /* [in] */ Char32 c)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::AppendChar(c);
}

ECode StringBuffer::Append(
    /* [in] */ Int32 i)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Append(i);
}

ECode StringBuffer::Append(
    /* [in] */ Int64 l)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Append(l);
}

ECode StringBuffer::Append(
    /* [in] */ Float f)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Append(f);
}

ECode StringBuffer::Append(
    /* [in] */ Double d)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Append(d);
}

ECode StringBuffer::Append(
    /* [in] */ IInterface* obj)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Append(obj);
}

ECode StringBuffer::Append(
    /* [in] */ const String& str)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Append(str);
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
    return AbstractStringBuilder::Append(str);
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
    return AbstractStringBuilder::Append(str);
}

ECode StringBuffer::Append(
    /* [in] */ const ArrayOf<Char32>& chars)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Append(chars);
}

ECode StringBuffer::Append(
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Append(chars, offset, length);
}

ECode StringBuffer::Append(
    /* [in] */ ICharSequence* csq)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Append(csq);
}

ECode StringBuffer::Append(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Append(csq, start, end);
}

ECode StringBuffer::InsertChar(
    /* [in] */ Int32 offset,
    /* [in] */ Char32 c)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::InsertChar(offset, c);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Boolean b)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Insert(offset, b);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 i)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Insert(offset, i);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Int64 l)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Insert(offset, l);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Float f)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Insert(offset, f);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Double d)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Insert(offset, d);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ IInterface* obj)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Insert(offset, obj);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ const String& str)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Insert(offset, str);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ const ArrayOf<Char32>& chars)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Insert(offset, chars);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ const ArrayOf<Char32>& str,
    /* [in] */ Int32 strOffset,
    /* [in] */ Int32 strLen)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Insert(offset, str, strOffset, strLen);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ ICharSequence* s)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Insert(offset, s);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Insert(offset, s, start, end);
}

ECode StringBuffer::Replace(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const String& string)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Replace(start, end, string);
}

ECode StringBuffer::Reverse()
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Reverse();
}

ECode StringBuffer::Delete(
    /* [in] */ Int32 location)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Delete(location);
}

ECode StringBuffer::Delete(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Delete(start, end);
}

ECode StringBuffer::ToString(
    /* [out] */ String* str)
{
    Mutex::Autolock lock(mLock);
    return AbstractStringBuilder::Substring(0, str);
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
    AbstractStringBuilder::Substring(0, &str);
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
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const char right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::AppendChar(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Char32 right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::AppendChar(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Int32 right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Int64 right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Float right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Double right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const char* right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const String& right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(IInterface* right)
{
    Mutex::Autolock lock(mLock);
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(StringBuffer& right)
{
    Mutex::Autolock lock(mLock);
    String str;
    right.ToString(&str);
    AbstractStringBuilder::Append(str);
    return *this;
}

StringBuffer& StringBuffer::operator+=(StringBuilder& right)
{
    Mutex::Autolock lock(mLock);
    String str;
    right.ToString(&str);
    AbstractStringBuilder::Append(str);
    return *this;
}

} // namespace Core
} // namespace Elastos
