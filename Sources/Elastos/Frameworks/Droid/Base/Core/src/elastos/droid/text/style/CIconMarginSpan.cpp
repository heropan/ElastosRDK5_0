
#include "elastos/droid/text/style/CIconMarginSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ILEADINGMARGINSPAN_METHODS_IMPL(CIconMarginSpan, IconMarginSpan, IconMarginSpan)
ILineHeightSpan_METHODS_IMPL(CIconMarginSpan, IconMarginSpan, IconMarginSpan)

ECode CIconMarginSpan::constructor(
    /* [in] */ IBitmap* b)
{
    Init(b);
    return NOERROR;
}

ECode CIconMarginSpan::constructor(
    /* [in] */ IBitmap* b,
    /* [in] */ Int32 pad)
{
    Init(b, pad);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

