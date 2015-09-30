#ifndef __ELASTOS_DROID_TEXT_CSTATICLAYOUT_H__
#define __ELASTOS_DROID_TEXT_CSTATICLAYOUT_H__

#include "_Elastos_Droid_Text_CStaticLayout.h"
#include "elastos/droid/text/StaticLayout.h"


namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CStaticLayout), public StaticLayout
{
public:
    ILAYOUT_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad);

    /**
     * @hide
     */
    CARAPI constructor(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ ITextDirectionHeuristic* textDir,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad);

    CARAPI constructor(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 bufstart,
        /* [in] */ Int32 bufend,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad);

    /**
     * @hide
     */
    CARAPI constructor(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 bufstart,
        /* [in] */ Int32 bufend,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ ITextDirectionHeuristic* textDir,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad);

    CARAPI constructor(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 bufstart,
        /* [in] */ Int32 bufend,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize,
        /* [in] */ Int32 ellipsizedWidth);

    /**
     * @hide
     */
    CARAPI constructor(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 bufstart,
        /* [in] */ Int32 bufend,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ ITextDirectionHeuristic* textDir,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize,
        /* [in] */ Int32 ellipsizedWidth,
        /* [in] */ Int32 maxLines);

    CARAPI constructor(
        /* [in] */ ICharSequence* text);

    using StaticLayout::GetLineCount;
    using StaticLayout::GetTopPadding;
    using StaticLayout::GetBottomPadding;
    using StaticLayout::GetLineStart;
    using StaticLayout::GetParagraphDirection;
    using StaticLayout::GetLineContainsTab;
    using StaticLayout::GetLineTop;
    using StaticLayout::GetLineDescent;
    using StaticLayout::GetLineDirections;
    using StaticLayout::GetEllipsisStart;
    using StaticLayout::GetEllipsisCount;
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_CSTATICLAYOUT_H__
