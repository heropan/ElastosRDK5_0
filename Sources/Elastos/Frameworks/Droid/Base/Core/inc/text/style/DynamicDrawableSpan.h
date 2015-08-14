#ifndef __ELASTOS_DROID_TEXT_STYLE_DynamicDrawableSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_DynamicDrawableSpan_H__

#include "text/style/ReplacementSpan.h"

using Elastos::Droid::Graphics::Drawable::IDrawable;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

//public abstract
class DynamicDrawableSpan
    : public ReplacementSpan
{
public:
    DynamicDrawableSpan();

    CARAPI_(void) Init();

    /**
     * Returns the vertical alignment of this span, one of {@link #ALIGN_BOTTOM} or
     * {@link #ALIGN_BASELINE}.
     */
    CARAPI_(Int32) GetVerticalAlignment();

    /**
     * Your subclass must implement this method to provide the bitmap
     * to be drawn.  The dimensions of the bitmap must be the same
     * from each call to the next.
     */
    virtual CARAPI_(AutoPtr<IDrawable>) GetDrawable() = 0;

    //@Override
    CARAPI_(Int32) GetSize(
        /* [in] */ IPaint* paint,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IPaintFontMetricsInt* fm);

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Int32 top,
        /* [in] */ Int32 y,
        /* [in] */ Int32 bottom,
        /* [in] */ IPaint* paint);

protected:
    /**
     * @param verticalAlignment one of {@link #ALIGN_BOTTOM} or {@link #ALIGN_BASELINE}.
     */
    DynamicDrawableSpan(
        /* [in] */ Int32 verticalAlignment);

    CARAPI_(void) Init(
        /* [in] */ Int32 verticalAlignment);

private:
    CARAPI_(AutoPtr<IDrawable>) GetCachedDrawable();

protected:
    /*const*/ Int32 mVerticalAlignment;

private:
    static const CString TAG;// = "DynamicDrawableSpan";
    AutoPtr<IDrawable> mDrawableRef;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_DynamicDrawableSpan_H__
