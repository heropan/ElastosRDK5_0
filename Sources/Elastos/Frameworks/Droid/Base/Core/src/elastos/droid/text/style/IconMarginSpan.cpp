#include "elastos/droid/text/style/IconMarginSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

IconMarginSpan::IconMarginSpan()
{}

IconMarginSpan::IconMarginSpan(
    /* [in] */ IBitmap* b)
{
    Init(b);
}

IconMarginSpan::IconMarginSpan(
    /* [in] */ IBitmap* b,
    /* [in] */ Int32 pad)
{
    Init(b, pad);
}

void IconMarginSpan::Init(
    /* [in] */ IBitmap* b)
{
    mBitmap = b;
}

void IconMarginSpan::Init(
    /* [in] */ IBitmap* b,
    /* [in] */ Int32 pad)
{
    mBitmap = b;
    mPad = pad;
}

Int32 IconMarginSpan::GetLeadingMargin(
    /* [in] */ Boolean first)
{
    Int32 width;
    return (mBitmap->GetWidth(&width), width) + mPad;
}

void IconMarginSpan::DrawLeadingMargin(
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
    Int32 itop, lLineForOffset;
    layout->GetLineTop((layout->GetLineForOffset(st, &lLineForOffset), lLineForOffset), &itop);

    if (dir < 0){
        Int32 bWidth;
        x -= (mBitmap->GetWidth(&bWidth), bWidth);
    }

    c->DrawBitmap(mBitmap, x, itop, p);
}

void IconMarginSpan::ChooseHeight(
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
        mBitmap->GetHeight(&ht);

        Int32 descent, ascent;
        Int32 need = ht - (v + (fm->GetDescent(&descent), descent) - (fm->GetAscent(&ascent), ascent) - istartv);
        if (need > 0) {
            fm->SetDescent((fm->GetDescent(&descent), descent)+need);
        }

        Int32 bottom, top;
        need = ht - (v + (fm->GetBottom(&bottom), bottom) - (fm->GetTop(&top), top) - istartv);
        if (need > 0) {
            fm->SetBottom((fm->GetBottom(&bottom), bottom)+need);
        }
    }
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos