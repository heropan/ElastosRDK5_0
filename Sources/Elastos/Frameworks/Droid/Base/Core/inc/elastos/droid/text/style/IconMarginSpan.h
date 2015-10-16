#ifndef __ELASTOS_DROID_TEXT_STYLE_IconMarginSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_IconMarginSpan_H__

#include "Elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Core::ICharSequence;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::IPaintFontMetricsInt;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

class IconMarginSpan
{
public:
    IconMarginSpan(
        /* [in] */ IBitmap* b);

    IconMarginSpan(
        /* [in] */ IBitmap* b,
        /* [in] */ Int32 pad);

    CARAPI_(void) Init(
        /* [in] */ IBitmap* b);

    CARAPI_(void) Init(
        /* [in] */ IBitmap* b,
        /* [in] */ Int32 pad);

    CARAPI_(Int32) GetLeadingMargin(
        /* [in] */ Boolean first);

    CARAPI_(void) DrawLeadingMargin(
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

    CARAPI_(void) ChooseHeight(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 istartv,
        /* [in] */ Int32 v,
        /* [in] */ IPaintFontMetricsInt* fm);

protected:
    IconMarginSpan();

private:
    AutoPtr<IBitmap> mBitmap;
    Int32 mPad;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_IconMarginSpan_H__
