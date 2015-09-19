#ifndef __ELASTOS_DROID_TEXT_STYLE_AbsoluteSizeSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_AbsoluteSizeSpan_H__

#include "text/style/MetricAffectingSpan.h"
#include "Elastos.Droid.Core_server.h"

using Elastos::Droid::Text::ITextPaint;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

class AbsoluteSizeSpan : public MetricAffectingSpan
{
public:
    /**
     * Set the text size to <code>size</code> physical pixels.
     */
    AbsoluteSizeSpan(
        /* [in] */ Int32 size);

    /**
     * Set the text size to <code>size</code> physical pixels,
     * or to <code>size</code> device-independent pixels if
     * <code>dip</code> is true.
     */
    AbsoluteSizeSpan(
        /* [in] */ Int32 size,
        /* [in] */ Boolean dip);

    AbsoluteSizeSpan(
        /* [in] */ IParcel* src);

    CARAPI_(void) Init(
        /* [in] */ Int32 size);

    CARAPI_(void) Init(
        /* [in] */ Int32 size,
        /* [in] */ Boolean dip);

    CARAPI_(void) Init(
        /* [in] */ IParcel* src);

    CARAPI_(Int32) GetSpanTypeId();

    CARAPI_(Int32) DescribeContents();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI_(Int32) GetSize();

    CARAPI_(Boolean) GetDip();

    //@Override
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* ds);

    //@Override
    CARAPI UpdateMeasureState(
        /* [in] */ ITextPaint* ds);

protected:
    AbsoluteSizeSpan();

private:
    /*const*/ Int32 mSize;
    Boolean mDip;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_AbsoluteSizeSpan_H__
