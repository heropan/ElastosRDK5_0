#include "text/method/ReplacementTransformationMethod.h"
#include "text/TextUtils.h"
#include "text/CSpannedString.h"
#include "ext/frameworkext.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::CString;
using Elastos::Core::EIID_ICharSequence;
using Elastos::Core::StringBuilder;
using Elastos::Droid::Text::ISpannedString;
using Elastos::Droid::Text::CSpannedString;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {
/*****************************ReplacementTransformationMethod::ReplacementCharSequence*****************************/
PInterface ReplacementTransformationMethod::ReplacementCharSequence::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IGetChars*)this;
    }
    else if (riid == EIID_IGetChars) {
        return (IGetChars*)this;
    }
    else if (riid == EIID_ICharSequence) {
        return (ICharSequence*)this;
    }
    return NULL;
}

UInt32 ReplacementTransformationMethod::ReplacementCharSequence::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReplacementTransformationMethod::ReplacementCharSequence::Release()
{
    return ElRefBase::Release();
}

ECode ReplacementTransformationMethod::ReplacementCharSequence::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (object == (IInterface*)(IGetChars*)this) {
        *iID = EIID_IGetChars;
    }
    else if (object == (IInterface*)(ICharSequence*)this) {
        *iID = EIID_ICharSequence;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ReplacementTransformationMethod::ReplacementCharSequence::ReplacementCharSequence(
    /* [in] */ ICharSequence* source,
    /* [in] */ ArrayOf<Char32>* original,
    /* [in] */ ArrayOf<Char32>* replacement)
    : mOriginal(original)
    , mReplacement(replacement)
    , mSource(source)
{
}

ECode ReplacementTransformationMethod::ReplacementCharSequence::GetLength(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    mSource->GetLength(number);
    return NOERROR;
}

ECode ReplacementTransformationMethod::ReplacementCharSequence::GetCharAt(
    /* [in] */ Int32 i,
    /* [out] */ Char32* cr)
{
    VALIDATE_NOT_NULL(cr);
    Char32 c ;
    mSource->GetCharAt(i, &c);

    Int32 n = mOriginal->GetLength();
    for (Int32 j = 0; j < n; j++) {
        if (c == (*mOriginal)[j]) {
            c = (*mReplacement)[j];
        }
    }
    *cr = c;
    return NOERROR;
}

ECode ReplacementTransformationMethod::ReplacementCharSequence::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);
    AutoPtr< ArrayOf<Char32> > c = ArrayOf<Char32>::Alloc(end - start);
    GetChars(start, end, c, 0);

    StringBuilder sb;
    sb.AppendChars(*c);
    AutoPtr<ICharSequence> cs = sb.ToCharSequence();
    *csq = cs;
    REFCOUNT_ADD(*csq);
    return NOERROR;
}

ECode ReplacementTransformationMethod::ReplacementCharSequence::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    Int32 length;
    GetLength(&length);
    AutoPtr< ArrayOf<Char32> > c = ArrayOf<Char32>::Alloc(length);
    GetChars(0, length, c, 0);

    *str = String(*c);
    return NOERROR;
}

ECode ReplacementTransformationMethod::ReplacementCharSequence::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Char32>* dest,
    /* [in] */ Int32 off)
{
    TextUtils::GetChars(mSource, start, end, dest, off);

    Int32 offend = end - start + off;
    Int32 n = mOriginal->GetLength();
    Char32 c;
    for (Int32 i = off; i < offend; i++) {
        c = (*dest)[i];
        for (Int32 j = 0; j < n; j++) {
            if (c == (*mOriginal)[j]) {
                (*dest)[i] = (*mReplacement)[j];
            }
        }
    }
    return NOERROR;
}

/*****************************ReplacementTransformationMethod::SpannedReplacementCharSequence*****************************/
PInterface ReplacementTransformationMethod::SpannedReplacementCharSequence::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IGetChars*)this;
    }
    else if (riid == EIID_IGetChars) {
        return (IGetChars*)this;
    }
    else if (riid == EIID_ISpanned) {
        return (ISpanned*)this;
    }
    else if (riid == EIID_ICharSequence) {
        return (ICharSequence*)(ReplacementCharSequence*)this;
    }
    return NULL;
}

UInt32 ReplacementTransformationMethod::SpannedReplacementCharSequence::AddRef()
{
    return ReplacementCharSequence::AddRef();
}

UInt32 ReplacementTransformationMethod::SpannedReplacementCharSequence::Release()
{
    return ReplacementCharSequence::Release();
}

ECode ReplacementTransformationMethod::SpannedReplacementCharSequence::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (object == (IInterface*)(IGetChars*)this) {
        *iID = EIID_IGetChars;
    }
    else if (object == (IInterface*)(ISpanned*)this) {
        *iID = EIID_ISpanned;
    }
    else if (object == (IInterface*)(ICharSequence*)(ReplacementCharSequence*)this) {
        *iID = EIID_ICharSequence;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode ReplacementTransformationMethod::SpannedReplacementCharSequence::GetLength(
    /* [out] */ Int32* number)
{
    return ReplacementCharSequence::GetLength(number);
}

ECode ReplacementTransformationMethod::SpannedReplacementCharSequence::GetCharAt(
    /* [in] */ Int32 i,
    /* [out] */ Char32* cr)
{
    return ReplacementCharSequence::GetCharAt(i, cr);
}

ECode ReplacementTransformationMethod::SpannedReplacementCharSequence::ToString(
    /* [out] */ String* str)
{
    return ReplacementCharSequence::ToString(str);
}

ECode ReplacementTransformationMethod::SpannedReplacementCharSequence::GetSpans(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const InterfaceID& type,
    /* [out, callee] */ ArrayOf<IInterface*>** objs)
{
    return mSpanned->GetSpans(start, end, type, objs);
}

ECode ReplacementTransformationMethod::SpannedReplacementCharSequence::GetSpanStart(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* start)
{
    return mSpanned->GetSpanStart(tag, start);
}

ECode ReplacementTransformationMethod::SpannedReplacementCharSequence::GetSpanEnd(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* end)
{
    return mSpanned->GetSpanEnd(tag, end);
}

ECode ReplacementTransformationMethod::SpannedReplacementCharSequence::GetSpanFlags(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* flags)
{
    return mSpanned->GetSpanFlags(tag, flags);

}

ECode ReplacementTransformationMethod::SpannedReplacementCharSequence::NextSpanTransition(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const InterfaceID& type,
    /* [out] */ Int32* offset)
{
    return mSpanned->NextSpanTransition(start, end, type, offset);
}

ECode ReplacementTransformationMethod::SpannedReplacementCharSequence::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);
    AutoPtr<ISpannedString> sS;
    CSpannedString::New((ICharSequence*)this->Probe(EIID_ICharSequence), (ISpannedString**)&sS);
    AutoPtr<CSpannedString> cS = (CSpannedString*)(sS.Get());
    return cS->SubSequence(start, end, csq);
}

ReplacementTransformationMethod::SpannedReplacementCharSequence::SpannedReplacementCharSequence(
    /* [in] */ ISpanned* source,
    /* [in] */ ArrayOf<Char32>* original,
    /* [in] */ ArrayOf<Char32>* replacement)
    : ReplacementCharSequence(source, original, replacement)
    , mSpanned(source)
{
}

/*****************************ReplacementTransformationMethod*****************************/
AutoPtr<ICharSequence> ReplacementTransformationMethod::GetTransformation(
    /* [in] */ ICharSequence* source,
    /* [in] */ IView* v)
{
    AutoPtr< ArrayOf<Char32> > original = GetOriginal();
    AutoPtr< ArrayOf<Char32> > replacement = GetReplacement();

    /*
     * Short circuit for faster display if the text will never change.
     */
    AutoPtr<IEditable> editable = IEditable::Probe(source);
    if (editable == NULL) {
        /*
         * Check whether the text does not contain any of the
         * source characters so can be used unchanged.
         */
        Boolean doNothing = TRUE;
        Int32 n = original->GetLength();
        for (Int32 i = 0; i < n; i++) {
            if (TextUtils::IndexOf(source, (*original)[i]) >= 0) {
                doNothing = FALSE;
                break;
            }
        }
        if (doNothing) {
            return source;
        }

        AutoPtr<ISpannable> spannable = ISpannable::Probe(source);
        if (spannable == NULL) {
            /*
             * The text contains some of the source characters,
             * but they can be flattened out now instead of
             * at display time.
             */
             AutoPtr<ISpanned> spanned = ISpanned::Probe(source);
            if (spanned != NULL) {
                AutoPtr<ICharSequence> srcs;
                srcs = (ReplacementCharSequence*)(new SpannedReplacementCharSequence( spanned, original, replacement));
                AutoPtr<ISpannedString> sS;
                CSpannedString::New(srcs, (ISpannedString**)&sS);
                AutoPtr<ICharSequence> seq = ICharSequence::Probe(sS);
                return seq;
            }
            else {
                AutoPtr<IGetChars> gc = new ReplacementCharSequence(source, original, replacement);
                return gc;
            }
        }
    }


    AutoPtr<ISpanned> spanned = ISpanned::Probe(source);

    if (spanned != NULL) {
        AutoPtr<ICharSequence> srcs = (ReplacementCharSequence*)(new SpannedReplacementCharSequence(spanned, original, replacement));
        AutoPtr<ISpannedString> sS;
        CSpannedString::New(srcs.Get(), (ISpannedString**)&sS);
        AutoPtr<ICharSequence> seq = ICharSequence::Probe(sS);
        return seq;
    }
    else {
        AutoPtr<ICharSequence> gc = new ReplacementCharSequence(source, original, replacement);
        return gc;
    }

}

void ReplacementTransformationMethod::OnFocusChanged(
    /* [in] */ IView* view,
    /* [in] */ ICharSequence* sourceText,
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    // This callback isn't used.
}

//AutoPtr< ArrayOf<Char32> > ReplacementTransformationMethod::GetOriginal(){}

//AutoPtr< ArrayOf<Char32> > ReplacementTransformationMethod::GetReplacement(){}



} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
