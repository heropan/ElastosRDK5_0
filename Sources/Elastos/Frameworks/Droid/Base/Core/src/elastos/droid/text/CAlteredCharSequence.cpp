
#include "elastos/droid/text/CAlteredCharSequence.h"
#include "elastos/droid/text/TextUtils.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringBuilder.h>
#include "ext/frameworkext.h"

using Elastos::Core::StringBuilder;
using Elastos::Core::EIID_ICharSequence;

namespace Elastos {
namespace Droid {
namespace Text {

/******************************CAlteredCharSequence::AlteredSpanned*************************/
PInterface CAlteredCharSequence::AlteredSpanned::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IAlteredCharSequence*)this;
    }
    else if (riid == EIID_IAlteredCharSequence) {
        return (IAlteredCharSequence*)this;
    }
    else if (riid == EIID_ISpanned) {
        return (ISpanned*)this;
    }
    else if (riid == EIID_IGetChars) {
        return (IGetChars*)this;
    }
    return NULL;
}

UInt32 CAlteredCharSequence::AlteredSpanned::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAlteredCharSequence::AlteredSpanned::Release()
{
    return ElRefBase::Release();
}

ECode CAlteredCharSequence::AlteredSpanned::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IAlteredCharSequence*)this) {
        *iID = EIID_IAlteredCharSequence;
    }
    else if (Object == (IInterface*)(ISpanned*)this) {
        *iID = EIID_ISpanned;
    }
    else if (Object == (IInterface*)(IGetChars*)this) {
        *iID = EIID_IGetChars;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CAlteredCharSequence::AlteredSpanned::AlteredSpanned(
    /* [in] */ ICharSequence* source,
    /* [in] */ ArrayOf<Char32>* sub,
    /* [in] */ Int32 substart,
    /* [in] */ Int32 subend)
{
    mSource = source;
    mChars = sub;
    mStart = substart;
    mEnd = subend;

    mSpanned = ISpanned::Probe(source);
}

ECode CAlteredCharSequence::AlteredSpanned::GetSpans(
    /*[in]*/ Int32 start,
    /*[in]*/ Int32 end,
    /*[in]*/ const InterfaceID& type,
    /*[out, callee]*/ ArrayOf<IInterface*>** objs)
{
    return mSpanned->GetSpans(start, end, type, objs);
}

ECode CAlteredCharSequence::AlteredSpanned::GetSpanStart(
    /*[in]*/ IInterface* tag,
    /*[out]*/ Int32* start)
{
    return mSpanned->GetSpanStart(tag, start);

}

ECode CAlteredCharSequence::AlteredSpanned::GetSpanEnd(
    /*[in]*/ IInterface* tag,
    /*[out]*/ Int32* end)
{
    return mSpanned->GetSpanEnd(tag, end);
}

ECode CAlteredCharSequence::AlteredSpanned::GetSpanFlags(
    /*[in]*/ IInterface* tag,
    /*[out]*/ Int32* flags)
{
    return mSpanned->GetSpanFlags(tag, flags);
}

ECode CAlteredCharSequence::AlteredSpanned::NextSpanTransition(
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [in] */ const InterfaceID& type,
    /* [out] */ Int32* offset)
{
    return mSpanned->NextSpanTransition(start, limit, type, offset);
}

AutoPtr<IAlteredCharSequence> CAlteredCharSequence::AlteredSpanned::Make(
    /* [in] */ ICharSequence* source,
    /* [in] */ ArrayOf<Char32>* sub,
    /* [in] */ Int32 substart,
    /* [in] */ Int32 subend)
{
    AutoPtr<IAlteredCharSequence> ret = (IAlteredCharSequence*)(new AlteredSpanned(source, sub, substart, subend));
    return ret;
}

void CAlteredCharSequence::AlteredSpanned::Update(
    /* [in] */ ArrayOf<Char32>* sub,
    /* [in] */ Int32 substart,
    /* [in] */ Int32 subend)
{
    mChars = sub;
    mStart = substart;
    mEnd = subend;
}

ECode CAlteredCharSequence::AlteredSpanned::GetCharAt(
    /*[in]*/ Int32 off,
    /*[out]*/ Char32* c)
{
    if (off >= mStart && off < mEnd) {
        *c = (*mChars)[off - mStart];
        return NOERROR;
    }
    else {
        return mSource->GetCharAt(off, c);
    }
}

ECode CAlteredCharSequence::AlteredSpanned::GetLength(
    /*[out]*/ Int32* number)
{
    return mSource -> GetLength(number);
}

ECode CAlteredCharSequence::AlteredSpanned::SubSequence(
    /*[in]*/ Int32 start,
    /*[in]*/ Int32 end,
    /*[out]*/ ICharSequence** csq)
{
    AutoPtr<ICharSequence> csq1;
    mSource->SubSequence(start, end, (ICharSequence**)&csq1);

    AutoPtr<IAlteredCharSequence> ret = CAlteredCharSequence::AlteredSpanned::Make(csq1, mChars, mStart - start, mEnd - start);
    *csq = ICharSequence::Probe(ret);
    REFCOUNT_ADD(*csq);
    return NOERROR;
}

ECode CAlteredCharSequence::AlteredSpanned::GetChars(
    /*[in]*/ Int32 start,
    /*[in]*/ Int32 end,
    /*[in]*/ ArrayOf<Char32>* dest,
    /*[in]*/ Int32 off)
{
    TextUtils::GetChars(mSource, start, end, dest, off);

    Int32 start1, end1;
    start1 = Elastos::Core::Math::Max(mStart, start);
    end1 = Elastos::Core::Math::Min(mEnd, end);

    if (start1 > end1) {
        dest->Copy(off, mChars, start - mStart, end - start);
    }
    return NOERROR;
}

ECode CAlteredCharSequence::AlteredSpanned::ToString(
    /*[out]*/ String* str)
{
    Int32 len;
    GetLength(&len);

    AutoPtr< ArrayOf<Char32> > ret = ArrayOf<Char32>::Alloc(len);
    GetChars(0, len, ret, 0);

    StringBuilder sb;
    for (Int32 i = 0; i < ret->GetLength(); i++)
    {
        sb.AppendChar((char)((*ret)[i]));
    }

    String strRet;
    sb.ToString(&strRet);
    *str = strRet;
    return NOERROR;
}

/******************************CAlteredCharSequence*************************/
AutoPtr<IAlteredCharSequence> CAlteredCharSequence::Make(
    /* [in] */ ICharSequence* source,
    /* [in] */ ArrayOf<Char32>* sub,
    /* [in] */ Int32 substart,
    /* [in] */ Int32 subend)
{
    ISpanned* s = ISpanned::Probe(source);
    if(s != NULL) {
        AutoPtr<AlteredSpanned> retC = new AlteredSpanned(source, sub, substart, subend);
        AutoPtr<IAlteredCharSequence> ret = (IAlteredCharSequence*)(retC.Get());
        return ret;
    }
    else {
        AutoPtr<CAlteredCharSequence> retC;
        CAlteredCharSequence::NewByFriend(source, sub, substart, subend, (CAlteredCharSequence**)&retC);
        AutoPtr<IAlteredCharSequence> ret = (IAlteredCharSequence*)(retC.Get());
        return ret;
    }
}

ECode CAlteredCharSequence::constructor(
    /* [in] */ ICharSequence* source,
    /* [in] */ ArrayOf<Char32>* sub,
    /* [in] */ Int32 substart,
    /* [in] */ Int32 subend)
{
    mSource = source;
    mChars = sub;
    mStart = substart;
    mEnd = subend;
    return NOERROR;
}

void CAlteredCharSequence::Update(
    /* [in] */ ArrayOf<Char32>* sub,
    /* [in] */ Int32 substart,
    /* [in] */ Int32 subend)
{
    mChars = sub;
    mStart = substart;
    mEnd = subend;
}

ECode CAlteredCharSequence::GetCharAt(
    /*[in]*/ Int32 off,
    /*[out]*/ Char32* c)
{
    if (off >= mStart && off < mEnd) {
        *c = (*mChars)[off - mStart];
        return NOERROR;
    }
    else {
        return mSource->GetCharAt(off, c);
    }
}

ECode CAlteredCharSequence::GetLength(
    /*[out]*/ Int32* number)
{
    return mSource -> GetLength(number);
}

ECode CAlteredCharSequence::SubSequence(
    /*[in]*/ Int32 start,
    /*[in]*/ Int32 end,
    /*[out]*/ ICharSequence** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ICharSequence> csq;
    mSource->SubSequence(start, end, (ICharSequence**)&csq);

    AutoPtr<IAlteredCharSequence> acs = CAlteredCharSequence::Make(csq, mChars, mStart - start, mEnd - start);
    *ret = ICharSequence::Probe(acs);
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CAlteredCharSequence::GetChars(
    /*[in]*/ Int32 start,
    /*[in]*/ Int32 end,
    /*[in]*/ ArrayOf<Char32>* dest,
    /*[in]*/ Int32 off)
{
    TextUtils::GetChars(mSource, start, end, dest, off);

    Int32 start1, end1;
    start1 = Elastos::Core::Math::Max(mStart, start);
    end1 = Elastos::Core::Math::Min(mEnd, end);

    if (start1 > end1) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    return NOERROR;
}

ECode CAlteredCharSequence::ToString(
            /*[out]*/ String* str)
{
    Int32 len;
    GetLength(&len);

    AutoPtr< ArrayOf<Char32> > ret = ArrayOf<Char32>::Alloc(len);
    GetChars(0, len, ret, 0);

    StringBuilder sb;
    for (Int32 i = 0; i < ret->GetLength(); i++)
    {
        sb.AppendChar((char)((*ret)[i]));
    }

    String strRet;
    sb.ToString(&strRet);
    *str = strRet;

    return NOERROR;
}

ECode CAlteredCharSequence::constructor()
{
    return NOERROR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
