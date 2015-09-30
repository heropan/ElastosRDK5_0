
#include "ext/frameworkext.h"
#include "elastos/droid/text/CDynamicLayout.h"
#include "elastos/droid/text/TextDirectionHeuristics.h"

namespace Elastos {
namespace Droid {
namespace Text {

ILAYOUT_METHODS_IMPL(CDynamicLayout, DynamicLayout, DynamicLayout);

PInterface CDynamicLayout::Probe(
    /* [in] */ REIID riid)
{
    return _CDynamicLayout::Probe(riid);
}

/**
 * Make a layout for the specified text that will be updated as
 * the text is changed.
 */
ECode CDynamicLayout::constructor(
    /* [in] */ ICharSequence* base,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
{
    return DynamicLayout::Init(base, base, paint, width, align, spacingmult, spacingadd,
             includepad);
}

/**
 * Make a layout for the transformed text (password transformation
 * being the primary example of a transformation)
 * that will be updated as the base text is changed.
 */
ECode CDynamicLayout::constructor(
    /* [in] */ ICharSequence* base,
    /* [in] */ ICharSequence* display,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
{
    return DynamicLayout::Init(base, display, paint, width, align, spacingmult, spacingadd,
             includepad, -1, 0);
}

/**
 * Make a layout for the transformed text (password transformation
 * being the primary example of a transformation)
 * that will be updated as the base text is changed.
 * If ellipsize is non-null, the Layout will ellipsize the text
 * down to ellipsizedWidth.
 */
ECode CDynamicLayout::constructor(
    /* [in] */ ICharSequence* base,
    /* [in] */ ICharSequence* display,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
{
    return DynamicLayout::Init(base, display, paint, width, align, TextDirectionHeuristics::FIRSTSTRONG_LTR,
                spacingmult, spacingadd, includepad, ellipsize, ellipsizedWidth);
}

/**
 * Make a layout for the transformed text (password transformation
 * being the primary example of a transformation)
 * that will be updated as the base text is changed.
 * If ellipsize is non-null, the Layout will ellipsize the text
 * down to ellipsizedWidth.
 * *
 * *@hide
 */
ECode CDynamicLayout::constructor(
    /* [in] */ ICharSequence* base,
    /* [in] */ ICharSequence* display,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ ITextDirectionHeuristic* textDir,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
{
    return DynamicLayout::Init(base, display, paint, width, align,
         textDir, spacingmult, spacingadd,
         includepad, ellipsize, ellipsizedWidth);
}

ECode CDynamicLayout::GetBlockEndLines(
    /* [out, callee] */ ArrayOf<Int32>** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr< ArrayOf<Int32> > bel = DynamicLayout::GetBlockEndLines();
    *ret = bel;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CDynamicLayout::GetBlockIndices(
    /* [out, callee] */ ArrayOf<Int32>** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr< ArrayOf<Int32> > bi = DynamicLayout::GetBlockIndices();
    *ret = bi;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CDynamicLayout::GetNumberOfBlocks(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = DynamicLayout::GetNumberOfBlocks();
    return NOERROR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
