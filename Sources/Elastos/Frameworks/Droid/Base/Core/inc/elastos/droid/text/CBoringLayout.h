
#ifndef __ELASTOS_DROID_TEXT_CBORINGLAYOUT_H__
#define __ELASTOS_DROID_TEXT_CBORINGLAYOUT_H__

#include "_Elastos_Droid_Text_CBoringLayout.h"
#include "elastos/droid/text/BoringLayout.h"


namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CBoringLayout), public BoringLayout
{
public:
    ILAYOUT_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Returns a BoringLayout for the specified text, potentially reusing
     * this one if it is already suitable.  The caller must make sure that
     * no one is still using this Layout.
     */
    CARAPI ReplaceOrMake(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad,
        /* [out] */ IBoringLayout** layout);

    /**
     * Returns a BoringLayout for the specified text, potentially reusing
     * this one if it is already suitable.  The caller must make sure that
     * no one is still using this Layout.
     */
    CARAPI ReplaceOrMake(
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
        /* [out] */ IBoringLayout** layout);

    CARAPI Ellipsized(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI constructor(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad);

    CARAPI constructor(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize,
        /* [in] */ Int32 ellipsizedWidth);
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_TEXT_CBORINGLAYOUT_H__
