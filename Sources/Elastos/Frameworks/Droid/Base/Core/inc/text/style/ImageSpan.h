#ifndef __ELASTOS_DROID_TEXT_STYLE_ImageSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_ImageSpan_H__

#include "text/style/DynamicDrawableSpan.h"
#include "Elastos.Droid.Core_server.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IPaintFontMetricsInt;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

class ImageSpan : public DynamicDrawableSpan
{
public:
    /**
     * @deprecated Use {@link #ImageSpan(Context, Bitmap)} instead.
     */
    //@Deprecated
    ImageSpan(
        /* [in] */ IBitmap* b);

    /**
     * @deprecated Use {@link #ImageSpan(Context, Bitmap, int) instead.
     */
    //@Deprecated
    ImageSpan(
        /* [in] */ IBitmap* b,
        /* [in] */ Int32 verticalAlignment);

    ImageSpan(
        /* [in] */ IContext* context,
        /* [in] */ IBitmap* b);

    /**
     * @param verticalAlignment one of {@link DynamicDrawableSpan#ALIGN_BOTTOM} or
     * {@link DynamicDrawableSpan#ALIGN_BASELINE}.
     */
    ImageSpan(
        /* [in] */ IContext* context,
        /* [in] */ IBitmap* b,
        /* [in] */ Int32 verticalAlignment);

    ImageSpan(
        /* [in] */ IDrawable* d);

    /**
     * @param verticalAlignment one of {@link DynamicDrawableSpan#ALIGN_BOTTOM} or
     * {@link DynamicDrawableSpan#ALIGN_BASELINE}.
     */
    ImageSpan(
        /* [in] */ IDrawable* d,
        /* [in] */ Int32 verticalAlignment);

    ImageSpan(
        /* [in] */ IDrawable* d,
        /* [in] */ const String& source);

    /**
     * @param verticalAlignment one of {@link DynamicDrawableSpan#ALIGN_BOTTOM} or
     * {@link DynamicDrawableSpan#ALIGN_BASELINE}.
     */
    ImageSpan(
        /* [in] */ IDrawable* d,
        /* [in] */ const String& source,
        /* [in] */ Int32 verticalAlignment);

    ImageSpan(
        /* [in] */ IContext* context,
        /* [in] */ IUri* uri);

    /**
     * @param verticalAlignment one of {@link DynamicDrawableSpan#ALIGN_BOTTOM} or
     * {@link DynamicDrawableSpan#ALIGN_BASELINE}.
     */
    ImageSpan(
        /* [in] */ IContext* context,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 verticalAlignment);

    ImageSpan(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceId);

    /**
     * @param verticalAlignment one of {@link DynamicDrawableSpan#ALIGN_BOTTOM} or
     * {@link DynamicDrawableSpan#ALIGN_BASELINE}.
     */
    ImageSpan(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceId,
        /* [in] */ Int32 verticalAlignment);

    CARAPI_(void) Init(
        /* [in] */ IBitmap* b);

    CARAPI_(void) Init(
        /* [in] */ IBitmap* b,
        /* [in] */ Int32 verticalAlignment);

    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ IBitmap* b);

    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ IBitmap* b,
        /* [in] */ Int32 verticalAlignment);

    CARAPI_(void) Init(
        /* [in] */ IDrawable* d);

    CARAPI_(void) Init(
        /* [in] */ IDrawable* d,
        /* [in] */ Int32 verticalAlignment);

    CARAPI_(void) Init(
        /* [in] */ IDrawable* d,
        /* [in] */ const String& source);

    CARAPI_(void) Init(
        /* [in] */ IDrawable* d,
        /* [in] */ const String& source,
        /* [in] */ Int32 verticalAlignment);

    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ IUri* uri);

    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 verticalAlignment);

    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceId);

    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceId,
        /* [in] */ Int32 verticalAlignment);

    //@Override
    CARAPI_(AutoPtr<IDrawable>) GetDrawable();

    /**
     * Returns the source string that was saved during construction.
     */
    CARAPI_(String) GetSource();

protected:
    ImageSpan();

private:
    AutoPtr<IDrawable> mDrawable;
    AutoPtr<IUri> mContentUri;
    Int32 mResourceId;
    AutoPtr<IContext> mContext;
    String mSource;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_ImageSpan_H__
