
#include "text/CBoringLayout.h"

namespace Elastos {
namespace Droid {
namespace Text {

ILAYOUT_METHODS_IMPL(CBoringLayout, BoringLayout, BoringLayout);

PInterface CBoringLayout::Probe(
    /* [in] */ REIID riid)
{
    return _CBoringLayout::Probe(riid);
}

/**
 * Returns a BoringLayout for the specified text, potentially reusing
 * this one if it is already suitable.  The caller must make sure that
 * no one is still using this Layout.
 */
ECode CBoringLayout::ReplaceOrMake(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [out] */ IBoringLayout** layout)
{
    VALIDATE_NOT_NULL(layout);

    return BoringLayout::ReplaceOrMake(
        source, paint, outerwidth, align, spacingmult,
        spacingadd, metrics, includepad, layout);
}

/**
 * Returns a BoringLayout for the specified text, potentially reusing
 * this one if it is already suitable.  The caller must make sure that
 * no one is still using this Layout.
 */
ECode CBoringLayout::ReplaceOrMake(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth,
    /* [out] */ IBoringLayout** layout)
{
    VALIDATE_NOT_NULL(layout);

    return BoringLayout::ReplaceOrMake(
        source, paint, outerwidth, align, spacingmult, spacingadd,
        metrics, includepad, ellipsize, ellipsizedWidth, layout);
}

CARAPI CBoringLayout::Ellipsized(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return BoringLayout::Ellipsized(start, end);
}

CARAPI CBoringLayout::constructor(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad)
{
    return BoringLayout::_Init(source, paint, outerwidth, align,
        spacingmult, spacingadd, metrics, includepad);
}

CARAPI CBoringLayout::constructor(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
{
    return BoringLayout::_Init(source, paint, outerwidth, align, spacingmult,
        spacingadd, metrics, includepad, ellipsize, ellipsizedWidth);
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos