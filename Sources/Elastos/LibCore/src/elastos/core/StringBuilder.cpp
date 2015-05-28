
#include "StringBuilder.h"
#include "StringBuffer.h"
#ifdef ELASTOS_CORELIBRARY
#include "CStringWrapper.h"
#endif

namespace Elastos {
namespace Core {

CAR_INTERFACE_IMPL(StringBuilder, AbstractStringBuilder, IStringBuilder)

StringBuilder::StringBuilder()
    :AbstractStringBuilder()
{}

StringBuilder::StringBuilder(
    /* [in] */ Int32 capacity)
    :AbstractStringBuilder(capacity)
{
}

StringBuilder::StringBuilder(
    /* [in] */ const String& str)
    :AbstractStringBuilder(str)
{
}

StringBuilder::StringBuilder(
    /* [in] */ const char* str)
    :AbstractStringBuilder(String(str))
{
}

StringBuilder::~StringBuilder()
{}

ECode StringBuilder::ToASCIILowerCase()
{
    return AbstractStringBuilder::ToASCIICaseString(FALSE);
}

ECode StringBuilder::ToASCIIUpperCase()
{
    return AbstractStringBuilder::ToASCIICaseString(TRUE);
}

ECode StringBuilder::Reset()
{
    return SetLength(0);
}

ECode StringBuilder::GetLength(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length);
    *length = AbstractStringBuilder::GetLength();
    return NOERROR;
}

ECode StringBuilder::GetByteCount(
    /* [in] */ Int32* byteLength)
{
    VALIDATE_NOT_NULL(byteLength);
    *byteLength = AbstractStringBuilder::GetByteCount();
    return NOERROR;
}

ECode StringBuilder::GetCapacity(
    /* [out] */ Int32* capacity)
{
    VALIDATE_NOT_NULL(capacity);
    *capacity = AbstractStringBuilder::GetCapacity();
    return NOERROR;
}

ECode StringBuilder::EnsureCapacity(
    /* [in] */ Int32 min)
{
    return AbstractStringBuilder::EnsureCapacity(min);
}

ECode StringBuilder::TrimToSize()
{
    return AbstractStringBuilder::TrimToSize();
}

ECode StringBuilder::SetChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 ch)
{
    return AbstractStringBuilder::SetChar(index, ch);
}

Char32 StringBuilder::GetChar(
    /* [in] */ Int32 index)
{
    Char32 ch;
    AbstractStringBuilder::GetChar(index, &ch);
    return ch;
}

ECode StringBuilder::GetChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    return AbstractStringBuilder::GetChar(index, c);
}

ECode StringBuilder::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in, out] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstStart)
{
    return AbstractStringBuilder::GetCharsO(start, end, dst, dstStart);
}

ECode StringBuilder::SetLength(
    /* [in] */ Int32 length)
{
    return AbstractStringBuilder::SetLength(length);
}

ECode StringBuilder::Substring(
    /* [in] */ Int32 start,
    /* [out] */ String* str)
{
    return AbstractStringBuilder::SubstringO(start, str);
}

ECode StringBuilder::Substring(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ String* str)
{
    return AbstractStringBuilder::SubstringExO(start, end, str);
}

String StringBuilder::Substring(
    /* [in] */ Int32 start)
{
    String str;
    AbstractStringBuilder::SubstringO(start, &str);
    return str;
}

String StringBuilder::Substring(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    String str;
    AbstractStringBuilder::SubstringExO(start, end, &str);
    return str;
}

ECode StringBuilder::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** seq)
{
    return AbstractStringBuilder::SubSequenceO(start, end, seq);
}

ECode StringBuilder::IndexOf(
    /* [in] */ const String& string,
    /* [out] */ Int32* index)
{
    return AbstractStringBuilder::IndexOfO(string, index);
}

ECode StringBuilder::IndexOf(
    /* [in] */ const String& subString,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index)
{
    return AbstractStringBuilder::IndexOfExO(subString, start, index);
}

ECode StringBuilder::LastIndexOf(
    /* [in] */ const String& string,
    /* [out] */ Int32* index)
{
    return AbstractStringBuilder::LastIndexOfO(string, index);
}

ECode StringBuilder::LastIndexOf(
    /* [in] */ const String& subString,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index)
{
    return AbstractStringBuilder::LastIndexOfExO(subString, start, index);
}

ECode StringBuilder::AppendNULL()
{
    return AbstractStringBuilder::AppendNULL();
}

ECode StringBuilder::Append(
    /* [in] */ const char* str)
{
    return AbstractStringBuilder::AppendOCStr(str);
}

ECode StringBuilder::Append(
    /* [in] */ Boolean b)
{
    return AbstractStringBuilder::AppendOBoolean(b);
}

ECode StringBuilder::AppendChar(
    /* [in] */ Char32 c)
{
    return AbstractStringBuilder::AppendOChar(c);
}

ECode StringBuilder::Append(
    /* [in] */ Int32 i)
{
    return AbstractStringBuilder::AppendOInt32(i);
}

ECode StringBuilder::Append(
    /* [in] */ Int64 l)
{
    return AbstractStringBuilder::AppendOInt64(l);
}

ECode StringBuilder::Append(
    /* [in] */ Float f)
{
    return AbstractStringBuilder::AppendOFloat(f);
}

ECode StringBuilder::Append(
    /* [in] */ Double d)
{
    return AbstractStringBuilder::AppendODouble(d);
}

ECode StringBuilder::Append(
    /* [in] */ IInterface* obj)
{
    return AbstractStringBuilder::AppendOObject(obj);
}

ECode StringBuilder::Append(
    /* [in] */ const String& str)
{
    return AbstractStringBuilder::AppendOString(str);
}

ECode StringBuilder::Append(
    /* [in] */ IStringBuffer* sb)
{
    if (NULL == sb) {
        return AbstractStringBuilder::AppendNULL();
    }

    String str;
    ((StringBuilder*)sb)->ToString(&str);
    return AbstractStringBuilder::AppendOString(str);
}

ECode StringBuilder::Append(
    /* [in] */ IStringBuilder* sb)
{
    if (NULL == sb) {
        return AbstractStringBuilder::AppendNULL();
    }

    String str;
    ((StringBuilder*)sb)->ToString(&str);
    return AbstractStringBuilder::AppendOString(str);
}

ECode StringBuilder::Append(
    /* [in] */ const ArrayOf<Char32>& chars)
{
    return AbstractStringBuilder::AppendOChars(chars);
}

ECode StringBuilder::Append(
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return AbstractStringBuilder::AppendOCharsEx(chars, offset, length);
}

ECode StringBuilder::Append(
    /* [in] */ ICharSequence* csq)
{
    return AbstractStringBuilder::AppendOCharSequence(csq);
}

ECode StringBuilder::Append(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return AbstractStringBuilder::AppendOCharSequenceEx(csq, start, end);
}

ECode StringBuilder::InsertChar(
    /* [in] */ Int32 offset,
    /* [in] */ Char32 c)
{
    return AbstractStringBuilder::InsertOChar(offset, c);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Boolean b)
{
    return AbstractStringBuilder::InsertOBoolean(offset, b);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 i)
{
    return AbstractStringBuilder::InsertOInt32(offset, i);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Int64 l)
{
    return AbstractStringBuilder::InsertOInt64(offset, l);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Float f)
{
    return AbstractStringBuilder::InsertOFloat(offset, f);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Double d)
{
    return AbstractStringBuilder::InsertODouble(offset, d);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ IInterface* obj)
{
    return AbstractStringBuilder::InsertOObject(offset, obj);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ const String& str)
{
    return AbstractStringBuilder::InsertOString(offset, str);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ const ArrayOf<Char32>& chars)
{
    return AbstractStringBuilder::InsertOChars(offset, chars);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ const ArrayOf<Char32>& str,
    /* [in] */ Int32 strOffset,
    /* [in] */ Int32 strLen)
{
    return AbstractStringBuilder::InsertOCharsEx(offset, str, strOffset, strLen);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ ICharSequence* s)
{
    return AbstractStringBuilder::InsertOCharSequence(offset, s);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return AbstractStringBuilder::InsertOCharSequenceEx(offset, s, start, end);
}

ECode StringBuilder::Replace(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const String& string)
{
    return AbstractStringBuilder::ReplaceO(start, end, string);
}

ECode StringBuilder::Reverse()
{
    return AbstractStringBuilder::ReverseO();
}

ECode StringBuilder::Delete(
    /* [in] */ Int32 location)
{
    return AbstractStringBuilder::DeleteOChar(location);
}

ECode StringBuilder::Delete(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return AbstractStringBuilder::DeleteO(start, end);
}

ECode StringBuilder::ToString(
    /* [out] */ String* str)
{
    return AbstractStringBuilder::SubstringO(0, str);
}

String StringBuilder::ToString()
{
   String str;
   ToString(&str);
   return str;
}

AutoPtr<ICharSequence> StringBuilder::ToCharSequence()
{
    String str;
    AbstractStringBuilder::SubstringO(0, &str);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(str, (ICharSequence**)&seq);
    return seq;
}

StringBuilder& StringBuilder::operator+=(const Boolean right)
{
    AbstractStringBuilder::AppendOBoolean(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const char right)
{
    AbstractStringBuilder::AppendOChar(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const Char32 right)
{
    AbstractStringBuilder::AppendOChar(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const Int32 right)
{
    AbstractStringBuilder::AppendOInt32(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const Int64 right)
{
    AbstractStringBuilder::AppendOInt64(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const Float right)
{
    AbstractStringBuilder::AppendOFloat(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const Double right)
{
    AbstractStringBuilder::AppendODouble(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const char* right)
{
    AbstractStringBuilder::AppendOCStr(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const String& right)
{
    AbstractStringBuilder::AppendOString(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(IInterface* right)
{
    AbstractStringBuilder::AppendOObject(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(StringBuffer& right)
{
    String str;
    right.ToString(&str);
    AbstractStringBuilder::AppendOString(str);
    return *this;
}

StringBuilder& StringBuilder::operator+=(StringBuilder& right)
{
    String str;
    right.ToString(&str);
    AbstractStringBuilder::AppendOString(str);
    return *this;
}

} // namespace Core
} // namespace Elastos
