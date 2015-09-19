
#ifndef __ELASTOS_DROID_WIDGET_IMAGEVIEW_H__
#define __ELASTOS_DROID_WIDGET_IMAGEVIEW_H__

#include "ext/frameworkext.h"
#include "view/View.h"
#include "graphics/CRectF.h"

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IMatrix;
using Elastos::Droid::Graphics::CRectF;
using Elastos::Droid::Graphics::IColorFilter;
using Elastos::Droid::Graphics::PorterDuffMode;
using Elastos::Droid::Graphics::Drawable::IDrawable;

namespace Elastos {
namespace Droid {
namespace Widget {


class ImageView : public Elastos::Droid::View::View
{
public:
    ImageView();

    ImageView(
        /* [in] */ IContext* context);

    ImageView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    ImageView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    virtual CARAPI_(Boolean) GetAdjustViewBounds();

    virtual CARAPI SetAdjustViewBounds(
        /* [in] */ Boolean adjustViewBounds);

    virtual CARAPI_(Int32) GetMaxWidth();

    virtual CARAPI SetMaxWidth(
        /* [in] */ Int32 maxWidth);

    virtual CARAPI_(Int32) GetMaxHeight();

    virtual CARAPI SetMaxHeight(
        /* [in] */ Int32 maxHeight);

    virtual CARAPI_(AutoPtr<IDrawable>) GetDrawable();

    virtual CARAPI SetImageResource(
        /* [in] */ Int32 resId);

    virtual CARAPI SetImageURI(
        /* [in] */ IUri* uri);

    virtual CARAPI SetImageDrawable(
        /* [in] */ IDrawable* drawable);

    virtual CARAPI SetImageBitmap(
        /* [in] */ IBitmap* bm);

    virtual CARAPI SetImageState(
        /* [in] */ const ArrayOf<Int32>* state,
        /* [in] */ Boolean merge);

    virtual CARAPI SetImageLevel(
        /* [in] */ Int32 level);

    virtual CARAPI SetScaleType(
        /* [in] */ ImageViewScaleType scaleType);

    virtual CARAPI_(ImageViewScaleType) GetScaleType();

    virtual CARAPI_(AutoPtr<IMatrix>) GetImageMatrix();

    virtual CARAPI SetImageMatrix(
        /* [in] */ IMatrix* matrix);

    virtual CARAPI_(Boolean) GetCropToPadding();

    virtual CARAPI SetCropToPadding(
        /* [in] */ Boolean cropToPadding);

    virtual CARAPI SetBaseline(
        /* [in] */ Int32 baseline);

    virtual CARAPI SetBaselineAlignBottom(
        /* [in] */ Boolean aligned);

    virtual CARAPI_(Boolean) GetBaselineAlignBottom();

    virtual CARAPI SetColorFilter(
        /* [in] */ Int32 color);

    virtual CARAPI SetColorFilter(
        /* [in] */ Int32 color,
        /* [in] */ PorterDuffMode mode);

    virtual CARAPI_(AutoPtr<IColorFilter>) GetColorFilter();

    virtual CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    virtual CARAPI ClearColorFilter();

    virtual CARAPI_(Int32) GetImageAlpha();

    virtual CARAPI SetImageAlpha(
        /* [in] */ Int32 alpha);

    virtual CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    CARAPI GetBaseline(
        /* [out] */ Int32* baseLine);

    CARAPI InvalidateDrawable(
        /* [in] */ IDrawable* drawable);

    CARAPI SetSelected(
        /* [in] */ Boolean selected);

    virtual CARAPI JumpDrawablesToCurrentState();

    virtual CARAPI_(Boolean) HasOverlappingRendering();

    virtual CARAPI OnPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI SetVisibility(
        /* [in] */ Int32 visibility);

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

protected:
    CARAPI_(Boolean) VerifyDrawable(
        /* [in] */ IDrawable* dr);

    CARAPI OnCreateDrawableState(
        /* [in] */ Int32 extraSpace,
        /* [out] */ ArrayOf<Int32>** drawableState);

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI_(Boolean) SetFrame(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI DrawableStateChanged();

    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI OnAttachedToWindow();

    CARAPI OnDetachedFromWindow();

private:
    static CARAPI_(MatrixScaleToFit) ScaleTypeToScaleToFit(
        /* [in] */ ImageViewScaleType st);

    CARAPI_(void) InitImageView();

    CARAPI_(void) ResolveUri();

    CARAPI_(void) UpdateDrawable(
        /* [in] */ IDrawable* d);

    CARAPI_(void) ResizeFromDrawable();

    CARAPI_(Int32) ResolveAdjustedSize(
        /* [in] */ Int32 desiredSize,
        /* [in] */ Int32 maxSize,
        /* [in] */ Int32 measureSpec);

    CARAPI_(void) ConfigureBounds();

    CARAPI_(void) ApplyColorMod();

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:
    static const ImageViewScaleType sScaleTypeArray[];
    static const MatrixScaleToFit sS2FArray[4];

    // settable by the client
    AutoPtr<IUri> mUri;
    Int32 mResource;
    AutoPtr<IMatrix> mMatrix;
    ImageViewScaleType mScaleType;
    Boolean mHaveFrame;
    Boolean mAdjustViewBounds;
    Int32 mMaxWidth;
    Int32 mMaxHeight;


    // these are applied to the drawable
    AutoPtr<IColorFilter> mColorFilter;
    Int32 mAlpha;
    Int32 mViewAlphaScale;
    Boolean mColorMod;

    AutoPtr<IDrawable> mDrawable;
    AutoPtr<ArrayOf<Int32> > mState;
    Boolean mMergeState;
    Int32 mLevel;
    Int32 mDrawableWidth;
    Int32 mDrawableHeight;
    AutoPtr<IMatrix> mDrawMatrix;

    // Avoid allocations...
    AutoPtr<CRectF> mTempSrc;
    AutoPtr<CRectF> mTempDst;

    Boolean mCropToPadding;

    Int32 mBaseline;

    Boolean mBaselineAlignBottom;
};


} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_IMAGEVIEW_H__
