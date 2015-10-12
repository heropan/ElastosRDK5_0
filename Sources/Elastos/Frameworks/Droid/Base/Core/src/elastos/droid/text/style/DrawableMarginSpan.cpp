#include "elastos/droid/text/style/DrawableMarginSpan.h"
#include "elastos/droid/graphics/CPaintFontMetricsInt.h"

using Elastos::Droid::Graphics::CPaintFontMetricsInt;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

DrawableMarginSpan::DrawableMarginSpan()
{}

DrawableMarginSpan::DrawableMarginSpan(
    /* [in] */ IDrawable* b)
{
    Init(b);
}

DrawableMarginSpan::DrawableMarginSpan(
    /* [in] */ IDrawable* b,
    /* [in] */ Int32 pad)
{
    Init(b, pad);
}

void DrawableMarginSpan::Init(
    /* [in] */ IDrawable* b)
{
    mDrawable = b;
}

void DrawableMarginSpan::Init(
    /* [in] */ IDrawable* b,
    /* [in] */ Int32 pad)
{
    mDrawable = b;
    mPad = pad;
}

Int32 DrawableMarginSpan::GetLeadingMargin(
    /* [in] */ Boolean first)
{
    Int32 intrinsicWidth;
    return (mDrawable->GetIntrinsicWidth(&intrinsicWidth), intrinsicWidth) + mPad;
}

void DrawableMarginSpan::DrawLeadingMargin(
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
    Int32 st;
    ((ISpanned*) text)->GetSpanStart((IInterface*)this, &st);
    Int32 ix = (Int32)x;
    Int32 lineForOffset;
    Int32 itop;
    layout->GetLineTop((layout->GetLineForOffset(st, &lineForOffset), lineForOffset), &itop);

    Int32 dw;
    mDrawable->GetIntrinsicWidth(&dw);
    Int32 dh;
    mDrawable->GetIntrinsicHeight(&dh);

    // XXX What to do about Paint?
    mDrawable->SetBounds(ix, itop, ix+dw, itop+dh);
    mDrawable->Draw(c);
}

void DrawableMarginSpan::ChooseHeight(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 istartv,
    /* [in] */ Int32 v,
    /* [in] */ IPaintFontMetricsInt* fm)
{
    Int32 se;
    if (end == (((ISpanned*) text)->GetSpanEnd((IInterface*)this, &se), se)) {
        Int32 ht;
        mDrawable->GetIntrinsicHeight(&ht);

        CPaintFontMetricsInt* pfm = (CPaintFontMetricsInt*)fm;
        Int32 descent, ascent;
        Int32 need = ht - (v + (pfm->GetDescent(&descent), descent) - (pfm->GetAscent(&ascent), ascent) - istartv);
        if (need > 0) {
            pfm->SetDescent((pfm->GetDescent(&descent), descent) + need);
        }

        Int32 bottom, top;
        need = ht - (v + (pfm->GetBottom(&bottom), bottom) - (pfm->GetTop(&top), top) - istartv);
        if (need > 0) {
            pfm->SetBottom((pfm->GetBottom(&bottom), bottom) + need);
        }
    }
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos