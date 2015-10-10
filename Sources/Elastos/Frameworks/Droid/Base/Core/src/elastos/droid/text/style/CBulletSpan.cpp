
#include "elastos/droid/text/style/CBulletSpan.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

IPARCELABLESPAN_METHODS_IMPL(CBulletSpan, BulletSpan, BulletSpan)
//ILEADINGMARGINSPAN_METHODS_IMPL(CBulletSpan, BulletSpan, BulletSpan)

ECode CBulletSpan::GetLeadingMargin(
    /* [in] */ Boolean first,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset=BulletSpan::GetLeadingMargin(first);
    return NOERROR;
}

ECode CBulletSpan::DrawLeadingMargin(
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
    BulletSpan::DrawLeadingMargin(c, p, x, dir, top, baseline, bottom, text, start, end, first, layout);
    return NOERROR;
}

ECode CBulletSpan::constructor()
{
    Init();
    return NOERROR;
}

ECode CBulletSpan::constructor(
    /* [in] */ Int32 gapWidth)
{
    Init(gapWidth);
    return NOERROR;
}

ECode CBulletSpan::constructor(
    /* [in] */ Int32 gapWidth,
    /* [in] */ Int32 color)
{
    Init(gapWidth, color);
    return NOERROR;
}

ECode CBulletSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

