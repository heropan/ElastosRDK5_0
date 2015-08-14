
#ifndef __ELASTOS_DROID_TEXT_STYLE_CICONMARGINSPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CICONMARGINSPAN_H__

#include "_CIconMarginSpan.h"
#include "text/style/IconMarginSpan.h"


#include "Elastos.Droid.Core_server.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::IPaintFontMetricsInt;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CIconMarginSpan)
    , public IconMarginSpan
{
public:
    ILEADINGMARGINSPAN_METHODS_DECL()
    ILineHeightSpan_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IBitmap* b);

    CARAPI constructor(
        /* [in] */ IBitmap* b,
        /* [in] */ Int32 pad);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CICONMARGINSPAN_H__
