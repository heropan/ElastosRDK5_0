
#ifndef __ELASTOS_DROID_TEXT_STYLE_CBULLETSPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CBULLETSPAN_H__

#include "_CBulletSpan.h"
#include "text/style/BulletSpan.h"



namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CBulletSpan), public BulletSpan
{
public:
    IPARCELABLESPAN_METHODS_DECL()
    //ILEADINGMARGINSPAN_METHODS_DECL()

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

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 gapWidth);

    CARAPI constructor(
        /* [in] */ Int32 gapWidth,
        /* [in] */ Int32 color);

    CARAPI constructor(
        /* [in] */ IParcel* src);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CBULLETSPAN_H__
