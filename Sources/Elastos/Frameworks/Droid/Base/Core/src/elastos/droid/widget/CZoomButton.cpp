#include "widget/CZoomButton.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CZoomButton, ZoomButton)
IDRAWABLECALLBACK_METHODS_IMPL(CZoomButton, ZoomButton)
IKEYEVENTCALLBACK_METHODS_IMPL(CZoomButton, ZoomButton)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CZoomButton, ZoomButton)

PInterface CZoomButton::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CZoomButton::Probe(riid);
}

ECode CZoomButton::constructor(
    /* [in] */ IContext* context)
{
    return ZoomButton::Init(context);
}

ECode CZoomButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ZoomButton::Init(context, attrs);
}

ECode CZoomButton::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ZoomButton::Init(context, attrs, defStyle);
}

ECode CZoomButton::SetZoomSpeed(
    /* [in] */ Int64 speed)
{
    return ZoomButton::SetZoomSpeed(speed);
}

ECode CZoomButton::OnLongClick(
    /* [in] */ IView* v,
    /* [out] */ Boolean * res)
{
    VALIDATE_NOT_NULL(res);
    *res = ZoomButton::OnLongClick(v);
    return NOERROR;
}

ECode CZoomButton::GetAdjustViewBounds(
        /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ZoomButton::GetAdjustViewBounds();
    return NOERROR;
}

ECode CZoomButton::SetAdjustViewBounds(
        /* [in] */ Boolean adjustViewBounds)
{
    return ZoomButton::SetAdjustViewBounds(adjustViewBounds);
}

ECode CZoomButton::GetMaxWidth(
        /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = ZoomButton::GetMaxWidth();
    return NOERROR;
}

ECode CZoomButton::SetMaxWidth(
        /* [in] */ Int32 maxWidth)
{
    return ZoomButton::SetMaxWidth(maxWidth);
}

ECode CZoomButton::GetMaxHeight(
        /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = ZoomButton::GetMaxHeight();
    return NOERROR;
}

ECode CZoomButton::SetMaxHeight(
        /* [in] */ Int32 maxHeight)
{
    return ZoomButton::SetMaxHeight(maxHeight);
}

ECode CZoomButton::GetDrawable(
        /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    *drawable = ZoomButton::GetDrawable();
    return NOERROR;
}

ECode CZoomButton::SetImageResource(
        /* [in] */ Int32 resId)
{
    return ZoomButton::SetImageResource(resId);
}

ECode CZoomButton::SetImageURI(
        /* [in] */ IUri* uri)
{
    return ZoomButton::SetImageURI(uri);
}

ECode CZoomButton::SetImageDrawable(
        /* [in] */ IDrawable* drawable)
{
    return ZoomButton::SetImageDrawable(drawable);
}

ECode CZoomButton::SetImageBitmap(
        /* [in] */ IBitmap* bm)
{
    return ZoomButton::SetImageBitmap(bm);
}

ECode CZoomButton::SetImageState(
        /* [in] */ ArrayOf<Int32>* state,
        /* [in] */ Boolean mg)
{
    return ZoomButton::SetImageState(state, mg);
}

ECode CZoomButton::SetImageLevel(
        /* [in] */ Int32 level)
{
    return ZoomButton::SetImageLevel(level);
}

 ECode CZoomButton::SetScaleType(
        /* [in] */ ImageViewScaleType scaleType)
 {
    return ZoomButton::SetScaleType(scaleType);
 }

ECode CZoomButton::GetScaleType(
        /* [out] */ ImageViewScaleType* scaleType)
{
    VALIDATE_NOT_NULL(scaleType);
    *scaleType = ZoomButton::GetScaleType();
    return NOERROR;
}

ECode CZoomButton::GetImageMatrix(
        /* [out] */ IMatrix** martix)
{
    VALIDATE_NOT_NULL(martix);
    *martix = ZoomButton::GetImageMatrix();
    return NOERROR;
}

ECode CZoomButton::SetImageMatrix(
        /* [in] */ IMatrix* matrix)
{
    return ZoomButton::SetImageMatrix(matrix);
}

ECode CZoomButton::GetCropToPadding(
        /* [out] */ Boolean* padding)
{
    VALIDATE_NOT_NULL(padding);
    *padding = ZoomButton::GetCropToPadding();
    return NOERROR;
}

ECode CZoomButton::SetCropToPadding(
        /* [in] */ Boolean cropToPadding)
{
    return ZoomButton::SetCropToPadding(cropToPadding);
}

ECode CZoomButton::SetBaseline(
        /* [in] */ Int32 baseline)
{
    return ZoomButton::SetBaseline(baseline);
}

ECode CZoomButton::SetBaselineAlignBottom(
        /* [in] */ Boolean aligned)
{
    return ZoomButton::SetBaselineAlignBottom(aligned);
}

ECode CZoomButton::GetBaselineAlignBottom(
        /* [out] */ Boolean* aligned)
{
    VALIDATE_NOT_NULL(aligned);
    *aligned = ZoomButton::GetBaselineAlignBottom();
    return NOERROR;
}

ECode CZoomButton::SetColorFilter(
        /* [in] */ Int32 color)
{
    return ZoomButton::SetColorFilter(color);
}

ECode CZoomButton::SetColorFilter(
        /* [in] */ Int32 color,
        /* [in] */ PorterDuffMode mode)
{
    return ZoomButton::SetColorFilter(color, mode);
}

ECode CZoomButton::ClearColorFilter()
{
    return ZoomButton::ClearColorFilter();
}

ECode CZoomButton::GetColorFilter(
        /* [out] */ IColorFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    *filter = ZoomButton::GetColorFilter();
    return NOERROR;
}

ECode CZoomButton::SetColorFilter(
        /* [in] */ IColorFilter* cf)
{
    return ZoomButton::SetColorFilter(cf);
}

ECode CZoomButton::GetImageAlpha(
        /* [out] */ Int32* alpha)
{
    VALIDATE_NOT_NULL(alpha);
    *alpha = ZoomButton::GetImageAlpha();
    return NOERROR;
}

ECode CZoomButton::SetImageAlpha(
        /* [in] */ Int32 alpha)
{
    return ZoomButton::SetImageAlpha(alpha);
}

ECode CZoomButton::SetAlpha(
        /* [in] */ Int32 alpha)
{
    return ZoomButton::SetAlpha(alpha);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos