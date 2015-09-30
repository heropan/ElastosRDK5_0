
#ifndef __ELASTOS_DROID_TEXT_CDYNAMICLAYOUT_H__
#define __ELASTOS_DROID_TEXT_CDYNAMICLAYOUT_H__

#include "_Elastos_Droid_Text_CDynamicLayout.h"
#include "elastos/droid/text/DynamicLayout.h"


namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CDynamicLayout), public DynamicLayout
{
public:
    ILAYOUT_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Make a layout for the specified text that will be updated as
     * the text is changed.
     */
    CARAPI constructor(
        /* [in] */ ICharSequence* base,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad);

    /**
     * Make a layout for the transformed text (password transformation
     * being the primary example of a transformation)
     * that will be updated as the base text is changed.
     */
    CARAPI constructor(
        /* [in] */ ICharSequence* base,
        /* [in] */ ICharSequence* display,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad);

    /**
     * Make a layout for the transformed text (password transformation
     * being the primary example of a transformation)
     * that will be updated as the base text is changed.
     * If ellipsize is non-null, the Layout will ellipsize the text
     * down to ellipsizedWidth.
     */
    CARAPI constructor(
        /* [in] */ ICharSequence* base,
        /* [in] */ ICharSequence* display,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize,
        /* [in] */ Int32 ellipsizedWidth);

    /**
     * Make a layout for the transformed text (password transformation
     * being the primary example of a transformation)
     * that will be updated as the base text is changed.
     * If ellipsize is non-null, the Layout will ellipsize the text
     * down to ellipsizedWidth.
     * *
     * *@hide
     */
    CARAPI constructor(
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
        /* [in] */ Int32 ellipsizedWidth);

    /**
     * @hide
     */
    CARAPI GetBlockEndLines(
        /* [out, callee] */ ArrayOf<Int32>** ret);

    /**
     * @hide
     */
    CARAPI GetBlockIndices(
        /* [out, callee] */ ArrayOf<Int32>** ret);

    /**
     * @hide
     */
    CARAPI GetNumberOfBlocks(
        /* [out] */ Int32* ret);
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_CDYNAMICLAYOUT_H__
