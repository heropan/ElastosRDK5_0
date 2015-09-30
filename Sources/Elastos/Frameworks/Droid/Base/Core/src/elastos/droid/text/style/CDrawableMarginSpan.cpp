
#include "elastos/droid/text/style/CDrawableMarginSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

//ILEADINGMARGINSPAN_METHODS_IMPL(CDrawableMarginSpan, DrawableMarginSpan, DrawableMarginSpan)
//ILineHeightSpan_METHODS_IMPL(CDrawableMarginSpan, DrawableMarginSpan, DrawableMarginSpan)

ECode CDrawableMarginSpan::GetLeadingMargin(
    /* [in] */ Boolean first,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset=DrawableMarginSpan::GetLeadingMargin(first);
    return NOERROR;
}

ECode CDrawableMarginSpan::DrawLeadingMargin(
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
    DrawableMarginSpan::DrawLeadingMargin(c, p, x, dir, top, baseline, bottom, text, start, end, first, layout);
    return NOERROR;
}

ECode CDrawableMarginSpan::ChooseHeight(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 spanstartv,
    /* [in] */ Int32 v,
    /* [in] */ IPaintFontMetricsInt* fm)
{
    DrawableMarginSpan::ChooseHeight(text, start, end, spanstartv, v, fm);
    return NOERROR;
}

ECode CDrawableMarginSpan::constructor(
    /* [in] */ IDrawable* b)
{
    Init(b);
    return NOERROR;
}

ECode CDrawableMarginSpan::constructor(
    /* [in] */ IDrawable* b,
    /* [in] */ Int32 pad)
{
    Init(b, pad);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

