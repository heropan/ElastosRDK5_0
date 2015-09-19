
#ifndef __ELASTOS_DROID_TEXT_STYLE_CDRAWABLEMARGINSPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CDRAWABLEMARGINSPAN_H__

#include "_Elastos_Droid_Text_Style_CDrawableMarginSpan.h"
#include "text/style/DrawableMarginSpan.h"



namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CDrawableMarginSpan)
    , public DrawableMarginSpan
{
public:
    //ILEADINGMARGINSPAN_METHODS_DECL()
    //ILineHeightSpan_METHODS_DECL()

    CARAPI GetLeadingMargin(
        /* [in] */ Boolean first,
        /* [out] */ Int32* offset);

    CARAPI DrawLeadingMargin(
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
        /* [in] */ ILayout* layout);

    CARAPI ChooseHeight(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 spanstartv,
        /* [in] */ Int32 v,
        /* [in] */ IPaintFontMetricsInt* fm);

    CARAPI constructor(
        /* [in] */ IDrawable* b);

    CARAPI constructor(
        /* [in] */ IDrawable* b,
        /* [in] */ Int32 pad);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CDRAWABLEMARGINSPAN_H__
