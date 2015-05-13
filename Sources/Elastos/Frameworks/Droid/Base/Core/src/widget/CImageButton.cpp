#include "widget/CImageButton.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CImageButton, ImageButton)
IDRAWABLECALLBACK_METHODS_IMPL(CImageButton, ImageButton)
IKEYEVENTCALLBACK_METHODS_IMPL(CImageButton, ImageButton)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CImageButton, ImageButton)


PInterface CImageButton::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CImageButton::Probe(riid);
}

ECode CImageButton::constructor(
    /* [in] */ IContext* context)
{
    return ImageButton::Init(context);
}

ECode CImageButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ImageButton::Init(context, attrs);
}

ECode CImageButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ImageButton::Init(context, attrs, defStyle);
}

ECode CImageButton::GetAdjustViewBounds(
        /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ImageButton::GetAdjustViewBounds();
    return NOERROR;
}

ECode CImageButton::SetAdjustViewBounds(
        /* [in] */ Boolean adjustViewBounds)
{
    return ImageButton::SetAdjustViewBounds(adjustViewBounds);
}

ECode CImageButton::GetMaxWidth(
        /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = ImageButton::GetMaxWidth();
    return NOERROR;
}

ECode CImageButton::SetMaxWidth(
        /* [in] */ Int32 maxWidth)
{
    return ImageButton::SetMaxWidth(maxWidth);
}

ECode CImageButton::GetMaxHeight(
        /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = ImageButton::GetMaxHeight();
    return NOERROR;
}

ECode CImageButton::SetMaxHeight(
        /* [in] */ Int32 maxHeight)
{
    return ImageButton::SetMaxHeight(maxHeight);
}

ECode CImageButton::GetDrawable(
        /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    *drawable = ImageButton::GetDrawable();
    return NOERROR;
}

ECode CImageButton::SetImageResource(
        /* [in] */ Int32 resId)
{
    return ImageButton::SetImageResource(resId);
}

ECode CImageButton::SetImageURI(
        /* [in] */ IUri* uri)
{
    return ImageButton::SetImageURI(uri);
}

ECode CImageButton::SetImageDrawable(
        /* [in] */ IDrawable* drawable)
{
    return ImageButton::SetImageDrawable(drawable);
}

ECode CImageButton::SetImageBitmap(
        /* [in] */ IBitmap* bm)
{
    return ImageButton::SetImageBitmap(bm);
}

ECode CImageButton::SetImageState(
        /* [in] */ ArrayOf<Int32>* state,
        /* [in] */ Boolean mg)
{
    return ImageButton::SetImageState(state, mg);
}

ECode CImageButton::SetImageLevel(
        /* [in] */ Int32 level)
{
    return ImageButton::SetImageLevel(level);
}

 ECode CImageButton::SetScaleType(
        /* [in] */ ImageViewScaleType scaleType)
 {
    return ImageButton::SetScaleType(scaleType);
 }

ECode CImageButton::GetScaleType(
        /* [out] */ ImageViewScaleType* scaleType)
{
    VALIDATE_NOT_NULL(scaleType);
    *scaleType = ImageButton::GetScaleType();
    return NOERROR;
}

ECode CImageButton::GetImageMatrix(
        /* [out] */ IMatrix** matrix)
{
    VALIDATE_NOT_NULL(matrix);
    AutoPtr<IMatrix> temp = ImageButton::GetImageMatrix();
    *matrix = temp;
    INTERFACE_ADDREF(*matrix);
    return NOERROR;
}

ECode CImageButton::SetImageMatrix(
        /* [in] */ IMatrix* matrix)
{
    return ImageButton::SetImageMatrix(matrix);
}

ECode CImageButton::GetCropToPadding(
        /* [out] */ Boolean* padding)
{
    VALIDATE_NOT_NULL(padding);
    *padding = ImageButton::GetCropToPadding();
    return NOERROR;
}

ECode CImageButton::SetCropToPadding(
        /* [in] */ Boolean cropToPadding)
{
    return ImageButton::SetCropToPadding(cropToPadding);
}

ECode CImageButton::SetBaseline(
        /* [in] */ Int32 baseline)
{
    return ImageButton::SetBaseline(baseline);
}

ECode CImageButton::SetBaselineAlignBottom(
        /* [in] */ Boolean aligned)
{
    return ImageButton::SetBaselineAlignBottom(aligned);
}

ECode CImageButton::GetBaselineAlignBottom(
        /* [out] */ Boolean* aligned)
{
    VALIDATE_NOT_NULL(aligned);
    *aligned = ImageButton::GetBaselineAlignBottom();
    return NOERROR;
}

ECode CImageButton::SetColorFilter(
        /* [in] */ Int32 color)
{
    return ImageButton::SetColorFilter(color);
}

ECode CImageButton::SetColorFilterEx(
        /* [in] */ Int32 color,
        /* [in] */ PorterDuffMode mode)
{
    return ImageButton::SetColorFilter(color, mode);
}

ECode CImageButton::ClearColorFilter()
{
    return ImageButton::ClearColorFilter();
}

ECode CImageButton::GetColorFilter(
        /* [out] */ IColorFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    *filter = ImageButton::GetColorFilter();
    return NOERROR;
}

ECode CImageButton::SetColorFilterEx2(
        /* [in] */ IColorFilter* cf)
{
    return ImageButton::SetColorFilter(cf);
}

ECode CImageButton::GetImageAlpha(
        /* [out] */ Int32* alpha)
{
    VALIDATE_NOT_NULL(alpha);
    *alpha = ImageButton::GetImageAlpha();
    return NOERROR;
}

ECode CImageButton::SetImageAlpha(
        /* [in] */ Int32 alpha)
{
    return ImageButton::SetImageAlpha(alpha);
}

ECode CImageButton::SetAlphaEx(
        /* [in] */ Int32 alpha)
{
    return ImageButton::SetAlphaEx(alpha);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos

