
#include "StringBuffer.h"
#include "StringBuilder.h"
#include "Character.h"
#include "CStringWrapper.h"
#include <stdio.h>
#include "Autolock.h"

#define DEFAULT_STEP 16

namespace Elastos {
namespace Core {

CAR_INTERFACE_IMPL(StringBuffer, AbstractStringBuilder, IStringBuffer)

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
    Autolock lock(this);
    return AbstractStringBuilder::ToASCIICaseString(FALSE);
}

ECode StringBuffer::ToASCIIUpperCase()
{
    Autolock lock(this);
    return AbstractStringBuilder::ToASCIICaseString(TRUE);
}

ECode StringBuffer::Reset()
{
    Autolock lock(this);
    return AbstractStringBuilder::SetLength(0);
}

ECode StringBuffer::GetLength(
    /* [out] */ Int32* length)
{
    Autolock lock(this);
    VALIDATE_NOT_NULL(length);
    *length = GetLength();
    return NOERROR;
}

ECode StringBuffer::GetByteCount(
    /* [in] */ Int32* byteLength)
{
    Autolock lock(this);
    VALIDATE_NOT_NULL(byteLength);
    *byteLength = GetByteCount();
    return NOERROR;
}

ECode StringBuffer::GetCapacity(
    /* [out] */ Int32* capacity)
{
    Autolock lock(this);
    VALIDATE_NOT_NULL(capacity);
    *capacity = GetCapacity();
    return NOERROR;
}

ECode StringBuffer::EnsureCapacity(
    /* [in] */ Int32 min)
{
    Autolock lock(this);
    return AbstractStringBuilder::EnsureCapacity(min);
}

ECode StringBuffer::TrimToSize()
{
    Autolock lock(this);
    return AbstractStringBuilder::TrimToSize();
}

ECode StringBuffer::SetCharAt(
    /* [in] */ Int32 index,
    /* [in] */ Char32 ch)
{
    Autolock lock(this);
    return AbstractStringBuilder::SetCharAt(index, ch);
}

Char32 StringBuffer::GetCharAt(
    /* [in] */ Int32 index)
{
    Autolock lock(this);
    Char32 ch;
    AbstractStringBuilder::GetCharAt(index, &ch);
    return ch;
}

ECode StringBuffer::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    Autolock lock(this);
    return AbstractStringBuilder::GetCharAt(index, c);
}

ECode StringBuffer::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstStart)
{
    Autolock lock(this);
    return AbstractStringBuilder::GetChars(start, end, dst, dstStart);
}

ECode StringBuffer::Substring(
    /* [in] */ Int32 start,
    /* [out] */ String* str)
{
    Autolock lock(this);
    return AbstractStringBuilder::Substring(start, str);
}

ECode StringBuffer::Substring(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ String* str)
{
    Autolock lock(this);
    return AbstractStringBuilder::Substring(start, end, str);
}

String StringBuffer::Substring(
    /* [in] */ Int32 start)
{
    Autolock lock(this);
    String str;
    AbstractStringBuilder::Substring(start, &str);
    return str;
}

String StringBuffer::Substring(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Autolock lock(this);
    String str;
    AbstractStringBuilder::Substring(start, end, &str);
    return str;
}

ECode StringBuffer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** seq)
{
    Autolock lock(this);
    return AbstractStringBuilder::SubSequence(start, end, seq);
}

ECode StringBuffer::IndexOf(
    /* [in] */ const String& string,
    /* [out] */ Int32* index)
{
    Autolock lock(this);
    return AbstractStringBuilder::IndexOf(string, index);
}

ECode StringBuffer::IndexOf(
    /* [in] */ const String& subString,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index)
{
    Autolock lock(this);
    return AbstractStringBuilder::IndexOf(subString, start, index);
}

ECode StringBuffer::LastIndexOf(
    /* [in] */ const String& string,
    /* [out] */ Int32* index)
{
    Autolock lock(this);
    return AbstractStringBuilder::LastIndexOf(string, index);
}

ECode StringBuffer::LastIndexOf(
    /* [in] */ const String& subString,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index)
{
    Autolock lock(this);
    return AbstractStringBuilder::LastIndexOf(subString, start, index);
}

ECode StringBuffer::AppendNULL()
{
    Autolock lock(this);
    return AbstractStringBuilder::AppendNULL();
}

ECode StringBuffer::Append(
    /* [in] */ const char* str)
{
    Autolock lock(this);
    return AbstractStringBuilder::Append(str);
}

ECode StringBuffer::Append(
    /* [in] */ Boolean b)
{
    Autolock lock(this);
    return AbstractStringBuilder::Append(b);
}

ECode StringBuffer::AppendChar(
    /* [in] */ Char32 c)
{
    Autolock lock(this);
    return AbstractStringBuilder::AppendChar(c);
}

ECode StringBuffer::Append(
    /* [in] */ Int32 i)
{
    Autolock lock(this);
    return AbstractStringBuilder::Append(i);
}

ECode StringBuffer::Append(
    /* [in] */ Int64 l)
{
    Autolock lock(this);
    return AbstractStringBuilder::Append(l);
}

ECode StringBuffer::Append(
    /* [in] */ Float f)
{
    Autolock lock(this);
    return AbstractStringBuilder::Append(f);
}

ECode StringBuffer::Append(
    /* [in] */ Double d)
{
    Autolock lock(this);
    return AbstractStringBuilder::Append(d);
}

ECode StringBuffer::Append(
    /* [in] */ IInterface* obj)
{
    Autolock lock(this);
    return AbstractStringBuilder::Append(obj);
}

ECode StringBuffer::Append(
    /* [in] */ const String& str)
{
    Autolock lock(this);
    return AbstractStringBuilder::Append(str);
}

ECode StringBuffer::Append(
    /* [in] */ IStringBuffer* sb)
{
    Autolock lock(this);
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
    Autolock lock(this);
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
    Autolock lock(this);
    return AbstractStringBuilder::Append(chars);
}

ECode StringBuffer::Append(
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    Autolock lock(this);
    return AbstractStringBuilder::Append(chars, offset, length);
}

ECode StringBuffer::Append(
    /* [in] */ ICharSequence* csq)
{
    Autolock lock(this);
    return AbstractStringBuilder::Append(csq);
}

ECode StringBuffer::Append(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Autolock lock(this);
    return AbstractStringBuilder::Append(csq, start, end);
}

ECode StringBuffer::InsertChar(
    /* [in] */ Int32 offset,
    /* [in] */ Char32 c)
{
    Autolock lock(this);
    return AbstractStringBuilder::InsertChar(offset, c);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Boolean b)
{
    Autolock lock(this);
    return AbstractStringBuilder::Insert(offset, b);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 i)
{
    Autolock lock(this);
    return AbstractStringBuilder::Insert(offset, i);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Int64 l)
{
    Autolock lock(this);
    return AbstractStringBuilder::Insert(offset, l);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Float f)
{
    Autolock lock(this);
    return AbstractStringBuilder::Insert(offset, f);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Double d)
{
    Autolock lock(this);
    return AbstractStringBuilder::Insert(offset, d);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ IInterface* obj)
{
    Autolock lock(this);
    return AbstractStringBuilder::Insert(offset, obj);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ const String& str)
{
    Autolock lock(this);
    return AbstractStringBuilder::Insert(offset, str);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ const ArrayOf<Char32>& chars)
{
    Autolock lock(this);
    return AbstractStringBuilder::Insert(offset, chars);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ const ArrayOf<Char32>& str,
    /* [in] */ Int32 strOffset,
    /* [in] */ Int32 strLen)
{
    Autolock lock(this);
    return AbstractStringBuilder::Insert(offset, str, strOffset, strLen);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ ICharSequence* s)
{
    Autolock lock(this);
    return AbstractStringBuilder::Insert(offset, s);
}

ECode StringBuffer::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Autolock lock(this);
    return AbstractStringBuilder::Insert(offset, s, start, end);
}

ECode StringBuffer::Replace(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const String& string)
{
    Autolock lock(this);
    return AbstractStringBuilder::Replace(start, end, string);
}

ECode StringBuffer::Reverse()
{
    Autolock lock(this);
    return AbstractStringBuilder::Reverse();
}

ECode StringBuffer::DeleteCharAt(
    /* [in] */ Int32 location)
{
    Autolock lock(this);
    return AbstractStringBuilder::DeleteCharAt(location);
}

ECode StringBuffer::Delete(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Autolock lock(this);
    return AbstractStringBuilder::Delete(start, end);
}

ECode StringBuffer::ToString(
    /* [out] */ String* str)
{
    Autolock lock(this);
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
    Autolock lock(this);
    String str;
    AbstractStringBuilder::Substring(0, &str);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(str, (ICharSequence**)&seq);
    return seq;
}

Int32 StringBuffer::GetLength()
{
    Autolock lock(this);
    return AbstractStringBuilder::GetLength();
}

Int32 StringBuffer::GetByteCount()
{
    Autolock lock(this);
    return AbstractStringBuilder::GetByteCount();
}

StringBuffer& StringBuffer::operator+=(const Boolean right)
{
    Autolock lock(this);
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const char right)
{
    Autolock lock(this);
    AbstractStringBuilder::AppendChar(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Char32 right)
{
    Autolock lock(this);
    AbstractStringBuilder::AppendChar(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Int32 right)
{
    Autolock lock(this);
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Int64 right)
{
    Autolock lock(this);
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Float right)
{
    Autolock lock(this);
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const Double right)
{
    Autolock lock(this);
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const char* right)
{
    Autolock lock(this);
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(const String& right)
{
    Autolock lock(this);
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(IInterface* right)
{
    Autolock lock(this);
    AbstractStringBuilder::Append(right);
    return *this;
}

StringBuffer& StringBuffer::operator+=(StringBuffer& right)
{
    Autolock lock(this);
    String str;
    right.ToString(&str);
    AbstractStringBuilder::Append(str);
    return *this;
}

StringBuffer& StringBuffer::operator+=(StringBuilder& right)
{
    Autolock lock(this);
    String str;
    right.ToString(&str);
    AbstractStringBuilder::Append(str);
    return *this;
}

} // namespace Core
} // namespace Elastos
