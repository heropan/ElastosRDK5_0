#include "elastos/droid/text/style/TypefaceSpan.h"
#include "elastos/droid/graphics/CTypeface.h"
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Graphics::ITypeface;
using Elastos::Droid::Graphics::CTypeface;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

TypefaceSpan::TypefaceSpan()
{}

TypefaceSpan::TypefaceSpan(
    /* [in] */ const String& family)
{
    mFamily = family;
}

TypefaceSpan::TypefaceSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void TypefaceSpan::Init(
    /* [in] */ const String& family)
{
    mFamily = family;
}

void TypefaceSpan::Init(
    /* [in] */ IParcel* src)
{
    ReadFromParcel(src);
}

Int32 TypefaceSpan::GetSpanTypeId(
            /* [in] */ Int32* id);
{
    return ITextUtils::TYPEFACE_SPAN;
}

Int32 TypefaceSpan::DescribeContents()
{
    return 0;
}

ECode TypefaceSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return source->ReadString(&mFamily);
}

ECode TypefaceSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return dest->WriteString(mFamily);
}

String TypefaceSpan::GetFamily()
{
    return mFamily;
}

ECode TypefaceSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    Apply(ds, mFamily);
    return NOERROR;
}

ECode TypefaceSpan::UpdateMeasureState(
    /* [in] */ ITextPaint* paint)
{
    Apply(paint, mFamily);
    return NOERROR;
}

void TypefaceSpan::Apply(
    /* [in] */ IPaint* paint,
    /* [in] */ const String& family)
{
    assert(paint != NULL);
    Int32 oldStyle;

    AutoPtr<ITypeface> old;
    paint->GetTypeface( (ITypeface**)&old);
    if (old == NULL) {
        oldStyle = 0;
    } else {
        old->GetStyle(&oldStyle);
    }

    AutoPtr<ITypeface> tf;
    CTypeface::Create(family, oldStyle, (ITypeface**)&tf);
    Int32 tfStyle;
    Int32 fake = oldStyle & ~(tf->GetStyle(&tfStyle), tfStyle);

    if ((fake & ITypeface::BOLD) != 0) {
        paint->SetFakeBoldText(TRUE);
    }

    if ((fake & ITypeface::ITALIC) != 0) {
        paint->SetTextSkewX(-0.25f);
    }

    paint->SetTypeface(tf);
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos