
#include "cmdef.h"
#include "StringBuilder.h"
#include "StringBuffer.h"
#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#include "CStringWrapper.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

namespace Elastos {
namespace Core {

UInt32 StringBuilder::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 StringBuilder::Release()
{
    return ElRefBase::Release();
}

PInterface StringBuilder::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IStringBuilder*)this;
    }
    else if (riid == EIID_IStringBuffer) {
        return (IStringBuilder*)this;
    }
    else if (riid == EIID_ICharSequence) {
        return (ICharSequence*)this;
    }
    else if (riid == EIID_IAppendable) {
        return (IAppendable*)this;
    }

    return NULL;
}

ECode StringBuilder::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IStringBuilder *)this) {
        *pIID = EIID_IStringBuilder;
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
    return ToASCIICaseString(FALSE);
}

ECode StringBuilder::ToASCIIUpperCase()
{
    return ToASCIICaseString(TRUE);
}

ECode StringBuilder::Reset()
{
    return SetLengthO(0);
}

ECode StringBuilder::GetLength(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length);
    *length = GetLengthO();
    return NOERROR;
}

ECode StringBuilder::GetByteCount(
    /* [in] */ Int32* byteLength)
{
    VALIDATE_NOT_NULL(byteLength);
    *byteLength = GetByteCountO();
    return NOERROR;
}

ECode StringBuilder::GetCapacity(
    /* [out] */ Int32* capacity)
{
    VALIDATE_NOT_NULL(capacity);
    *capacity = GetCapacityO();
    return NOERROR;
}

ECode StringBuilder::EnsureCapacity(
    /* [in] */ Int32 min)
{
    return EnsureCapacityO(min);
}

ECode StringBuilder::TrimToSize()
{
    return TrimToSizeO();
}

ECode StringBuilder::SetChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 ch)
{
    return SetCharAtO(index, ch);
}

Char32 StringBuilder::GetChar(
    /* [in] */ Int32 index)
{
    Char32 ch;
    GetCharO(index, &ch);
    return ch;
}

ECode StringBuilder::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    return GetCharO(index, c);
}

ECode StringBuilder::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in, out] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstStart)
{
    return GetCharsO(start, end, dst, dstStart);
}

ECode StringBuilder::SetLength(
    /* [in] */ Int32 length)
{
    return SetLengthO(length);
}

ECode StringBuilder::Substring(
    /* [in] */ Int32 start,
    /* [out] */ String* str)
{
    return SubstringO(start, str);
}

ECode StringBuilder::Substring(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ String* str)
{
    return SubstringExO(start, end, str);
}

String StringBuilder::Substring(
    /* [in] */ Int32 start)
{
    String str;
    SubstringO(start, &str);
    return str;
}

String StringBuilder::Substring(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    String str;
    SubstringExO(start, end, &str);
    return str;
}

ECode StringBuilder::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** seq)
{
    return SubSequenceO(start, end, seq);
}

ECode StringBuilder::IndexOf(
    /* [in] */ const String& string,
    /* [out] */ Int32* index)
{
    return IndexOfO(string, index);
}

ECode StringBuilder::IndexOf(
    /* [in] */ const String& subString,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index)
{
    return IndexOfExO(subString, start, index);
}

ECode StringBuilder::LastIndexOf(
    /* [in] */ const String& string,
    /* [out] */ Int32* index)
{
    return LastIndexOfO(string, index);
}

ECode StringBuilder::LastIndexOf(
    /* [in] */ const String& subString,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index)
{
    return LastIndexOfExO(subString, start, index);
}

ECode StringBuilder::Append()
{
    return AppendONULL();
}

ECode StringBuilder::Append(
    /* [in] */ const char* str)
{
    return AppendOCStr(str);
}

ECode StringBuilder::Append(
    /* [in] */ Boolean b)
{
    return AppendOBoolean(b);
}

ECode StringBuilder::Append(
    /* [in] */ Char32 c)
{
    return AppendOChar(c);
}

ECode StringBuilder::AppendInt32(
    /* [in] */ Int32 i)
{
    return AppendOInt32(i);
}

ECode StringBuilder::AppendInt64(
    /* [in] */ Int64 l)
{
    return AppendOInt64(l);
}

ECode StringBuilder::Append(
    /* [in] */ Float f)
{
    return AppendOFloat(f);
}

ECode StringBuilder::Append(
    /* [in] */ Double d)
{
    return AppendODouble(d);
}

ECode StringBuilder::Append(
    /* [in] */ IInterface* obj)
{
    return AppendOObject(obj);
}

ECode StringBuilder::Append(
    /* [in] */ const String& str)
{
    return AppendOString(str);
}

ECode StringBuilder::Append(
    /* [in] */ IStringBuffer* sb)
{
    if (NULL == sb) {
        return AppendNULL();
    }

    String str;
    sb->ToString(&str);
    return AppendOString(str);
}

ECode StringBuilder::Append(
    /* [in] */ IStringBuilder* sb)
{
    if (NULL == sb) {
        return AppendNULL();
    }

    String str;
    sb->ToString(&str);
    return AppendOString(str);
}

ECode StringBuilder::Append(
    /* [in] */ const ArrayOf<Char32>& chars)
{
    return AppendOChars(chars);
}

ECode StringBuilder::Append(
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return AppendOCharsEx(chars, offset, length);
}

ECode StringBuilder::Append(
    /* [in] */ ICharSequence* csq)
{
    return AppendOCharSequence(csq);
}

ECode StringBuilder::Append(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return AppendOCharSequenceEx(csq, start, end);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Char32 c)
{
    return InsertOChar(offset, c);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Boolean b)
{
    return InsertOBoolean(offset, b);
}

ECode StringBuilder::InsertInt32(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 i)
{
    return InsertOInt32(offset, i);
}

ECode StringBuilder::InsertInt64(
    /* [in] */ Int32 offset,
    /* [in] */ Int64 l)
{
    return InsertOInt64(offset, l);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Float f)
{
    return InsertOFloat(offset, f);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ Double d)
{
    return InsertODouble(offset, d);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ IInterface* obj)
{
    return InsertOObject(offset, obj);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ const String& str)
{
    return InsertOString(offset, str);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ const ArrayOf<Char32>& chars)
{
    return InsertOChars(offset, chars);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ const ArrayOf<Char32>& str,
    /* [in] */ Int32 strOffset,
    /* [in] */ Int32 strLen)
{
    return InsertOCharsEx(offset, str, strOffset, strLen);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ ICharSequence* s)
{
    return InsertOCharSequence(offset, s);
}

ECode StringBuilder::Insert(
    /* [in] */ Int32 offset,
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return InsertOCharSequenceEx(offset, s, start, end);
}

ECode StringBuilder::Replace(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const String& string)
{
    return ReplaceO(start, end, string);
}

ECode StringBuilder::Reverse()
{
    return ReverseO();
}

ECode StringBuilder::DeleteChar(
    /* [in] */ Int32 location)
{
    return DeleteOChar(location);
}

ECode StringBuilder::Delete(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return DeleteO(start, end);
}

ECode StringBuilder::ToString(
    /* [out] */ String* str)
{
    return SubstringO(0, str);
}

AutoPtr<ICharSequence> StringBuilder::ToCharSequence()
{
    String str;
    SubstringO(0, &str);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(str, (ICharSequence**)&seq);
    return seq;
}

Int32 StringBuilder::GetLength()
{
    return GetLengthO();
}

Int32 StringBuilder::GetByteCount()
{
    return GetByteCountO();
}

StringBuilder& StringBuilder::operator+=(const Boolean right)
{
    AppendOBoolean(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const char right)
{
    AppendOChar(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const Char32 right)
{
    AppendOChar(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const Int32 right)
{
    AppendOInt32(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const Int64 right)
{
    AppendOInt64(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const Float right)
{
    AppendOFloat(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const Double right)
{
    AppendODouble(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const char* right)
{
    AppendOCStr(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(const String& right)
{
    AppendOString(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(IInterface* right)
{
    AppendOObject(right);
    return *this;
}

StringBuilder& StringBuilder::operator+=(StringBuffer& right)
{
    AppendOString(right.ToString());
    return *this;
}

StringBuilder& StringBuilder::operator+=(StringBuilder& right)
{
    AppendOString(right.ToString());
    return *this;
}

} // namespace Core
} // namespace Elastos
