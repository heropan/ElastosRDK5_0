#ifndef __ReplacementSpan_H__
#define __ReplacementSpan_H__

#include "text/style/MetricAffectingSpan.h"
#include "Elastos.Droid.Core_server.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::IPaintFontMetricsInt;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

//public abstract
class ReplacementSpan : public MetricAffectingSpan
{
public:
    virtual CARAPI_(Int32) GetSize(
        /* [in] */ IPaint* paint,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IPaintFontMetricsInt* fm) = 0;

    virtual CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Int32 top,
        /* [in] */ Int32 y,
        /* [in] */ Int32 bottom,
        /* [in] */ IPaint* paint) = 0;

    /**
     * This method does nothing, since ReplacementSpans are measured
     * explicitly instead of affecting Paint properties.
     */
    CARAPI UpdateMeasureState(
        /* [in] */ ITextPaint* p);

    /**
     * This method does nothing, since ReplacementSpans are drawn
     * explicitly instead of affecting Paint properties.
     */
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* ds);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ReplacementSpan_H__
