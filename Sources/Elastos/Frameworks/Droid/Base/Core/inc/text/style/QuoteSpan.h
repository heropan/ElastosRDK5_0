#ifndef __QuoteSpan_H__
#define __QuoteSpan_H__

#include "Elastos.Droid.Core_server.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IPaint;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

class QuoteSpan{
public:
    QuoteSpan();

    QuoteSpan(
        /* [in] */ Int32 color);

    QuoteSpan(
        /* [in] */ IParcel* src);

    CARAPI_(void) Init();

    CARAPI_(void) Init(
        /* [in] */ Int32 color);

    CARAPI_(void) Init(
        /* [in] */ IParcel* src);

    CARAPI_(Int32) GetSpanTypeId();

    CARAPI_(Int32) DescribeContents();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI_(Int32) GetColor();

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

private:
    static const Int32 STRIPE_WIDTH;// = 2;
    static const Int32 GAP_WIDTH;// = 2;
    /*const*/ Int32 mColor;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __QuoteSpan_H__
