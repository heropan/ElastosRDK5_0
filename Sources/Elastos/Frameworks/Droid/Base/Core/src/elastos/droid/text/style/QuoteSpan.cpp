#include "elastos/droid/text/style/QuoteSpan.h"

using Elastos::Droid::Graphics::PaintStyle;
using Elastos::Droid::Graphics::PaintStyle_FILL;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

const Int32 QuoteSpan::STRIPE_WIDTH = 2;
const Int32 QuoteSpan::GAP_WIDTH = 2;

QuoteSpan::QuoteSpan()
{
    Init();
}

QuoteSpan::QuoteSpan(
    /* [in] */ Int32 color)
{
    Init(color);
}

QuoteSpan::QuoteSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void QuoteSpan::Init()
{
    mColor = 0xff0000ff;
}

void QuoteSpan::Init(
    /* [in] */ Int32 color)
{
    mColor = color;
}

void QuoteSpan::Init(
    /* [in] */ IParcel* src)
{
    ReadFromParcel(src);
}

Int32 QuoteSpan::GetSpanTypeId()
{
    return ITextUtils::QUOTE_SPAN;
}

Int32 QuoteSpan::DescribeContents()
{
    return 0;
}

ECode QuoteSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return source->ReadInt32(&mColor);
}

ECode QuoteSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return dest->WriteInt32(mColor);
}

Int32 QuoteSpan::GetColor()
{
    return mColor;
}

Int32 QuoteSpan::GetLeadingMargin(
    /* [in] */ Boolean first)
{
    return STRIPE_WIDTH + GAP_WIDTH;
}

void QuoteSpan::DrawLeadingMargin(
    /* [in] */ ICanvas* c,
    /* [in] */ IPaint* p,
    /* [in] */ Int32 x,
    /* [in] */ Int32 dir,
    /* [in] */ Int32 top,
    /* [in] */ Int32 baseline,
    /* [in] */ Int32 bottom,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Boolean first,
    /* [in] */ ILayout* layout)
{
    PaintStyle style;
    p->GetStyle(&style);
    Int32 color;
    p->GetColor(&color);

    p->SetStyle(PaintStyle_FILL);
    p->SetColor(mColor);

    c->DrawRect(x, top, x + dir * STRIPE_WIDTH, bottom, p);

    p->SetStyle(style);
    p->SetColor(color);
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos