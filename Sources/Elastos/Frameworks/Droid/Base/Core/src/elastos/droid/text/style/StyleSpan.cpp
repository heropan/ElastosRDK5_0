#include "elastos/droid/text/style/StyleSpan.h"
#include "graphics/CTypeface.h"

using Elastos::Droid::Graphics::ITypeface;
using Elastos::Droid::Graphics::CTypeface;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

StyleSpan::StyleSpan()
{}

StyleSpan::StyleSpan(
    /* [in] */ Int32 style)
{
    Init(style);
}

StyleSpan::StyleSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void StyleSpan::Init(
    /* [in] */ Int32 style)
{
    mStyle = style;
}

void StyleSpan::Init(
    /* [in] */ IParcel* src)
{
    ReadFromParcel(src);
}

Int32 StyleSpan::GetSpanTypeId()
{
    return ITextUtils::STYLE_SPAN;
}

Int32 StyleSpan::DescribeContents()
{
    return 0;
}

ECode StyleSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return source->ReadInt32(&mStyle);
}

ECode StyleSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return dest->WriteInt32(mStyle);
}

Int32 StyleSpan::GetStyle()
{
    return mStyle;
}

ECode StyleSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    Apply(ds, mStyle);
    return NOERROR;
}

ECode StyleSpan::UpdateMeasureState(
    /* [in] */ ITextPaint* paint)
{
    Apply(paint, mStyle);
    return NOERROR;
}

void StyleSpan::Apply(
    /* [in] */ IPaint* paint,
    /* [in] */ Int32 style)
{
    assert(paint != NULL);
    Int32 oldStyle;

    AutoPtr<ITypeface> old;
    paint->GetTypeface( (ITypeface**)&old );
    if (old == NULL) {
        oldStyle = 0;
    } else {
        old->GetStyle(&oldStyle);
    }

    Int32 want = oldStyle | style;

    AutoPtr<ITypeface> tf;
    if (old == NULL) {
        CTypeface::DefaultFromStyle(want, (ITypeface**)&tf);
    } else {
        CTypeface::Create(old, want, (ITypeface**)&tf);
    }

    Int32 s;
    tf->GetStyle(&s);

    Int32 fake = want & ~s;

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