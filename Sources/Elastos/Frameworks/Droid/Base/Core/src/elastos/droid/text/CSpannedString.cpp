#include "elastos/droid/text/CSpannedString.h"

using Elastos::Core::EIID_ICharSequence;

namespace Elastos {
namespace Droid {
namespace Text {

PInterface CSpannedString::Probe(
    /* [in] */ REIID riid)
{
    return _CSpannedString::Probe(riid);
}

ECode CSpannedString::constructor(
    /* [in] */ ICharSequence* source)
{
    return SpannedString::Init(source);
}

ECode CSpannedString::constructor(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return SpannedString::Init(source, start, end);
}

CARAPI CSpannedString::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<ICharSequence> seq = SpannedString::SubSequence(start, end);
    *res = seq;
    REFCOUNT_ADD(*res);
    return NOERROR;
}

CARAPI CSpannedString::GetLength(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = SpannedString::GetLength();
    return NOERROR;
}

CARAPI CSpannedString::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    VALIDATE_NOT_NULL(c);
    *c = SpannedString::GetCharAt(index);
    return NOERROR;
}

CARAPI CSpannedString::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    return SpannedString::ToString(str);
}

CARAPI CSpannedString::GetSpans(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const InterfaceID& type,
    /* [out, callee] */ ArrayOf<IInterface*>** objs)
{
    VALIDATE_NOT_NULL(objs);
    return SpannedString::GetSpans(start, end, type, objs);
}

CARAPI CSpannedString::GetSpanStart(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* start)
{
    VALIDATE_NOT_NULL(start);
    *start = SpannedString::GetSpanStart(tag);
    return NOERROR;
}

CARAPI CSpannedString::GetSpanEnd(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* end)
{
    VALIDATE_NOT_NULL(end);
    *end = SpannedString::GetSpanEnd(tag);
    return NOERROR;
}

CARAPI CSpannedString::GetSpanFlags(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = SpannedString::GetSpanFlags(tag);
    return NOERROR;
}

CARAPI CSpannedString::NextSpanTransition(
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [in] */ const InterfaceID& type,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = SpannedString::NextSpanTransition(start, limit, type);
    return NOERROR;
}

CARAPI CSpannedString::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Char32>* dest,
    /* [in] */ Int32 destoff)
{
    return SpannedString::GetChars(start, end, dest, destoff);
}

CARAPI CSpannedString::SetSpan(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 flags)
{
    return SpannedString::SetSpan(what, start, end, flags);
}

CARAPI CSpannedString::Remove(
    /* [in] */ IInterface* what)
{
    return SpannedString::RemoveSpan(what);
}

AutoPtr<ISpannedString> CSpannedString::ValueOf(
        /* [in] */ ICharSequence* source)
{
    AutoPtr<ISpannedString> ss = ISpannedString::Probe(source);
    if (ss != NULL) {
        return ss;
    }

    AutoPtr<CSpannedString> css;
    CSpannedString::NewByFriend(source, (CSpannedString**)&css);
    return css;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
