
#include "ext/frameworkext.h"
#include "widget/CSeekBar.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CSeekBar, SeekBar);
IDRAWABLECALLBACK_METHODS_IMPL(CSeekBar, SeekBar);
IKEYEVENTCALLBACK_METHODS_IMPL(CSeekBar, SeekBar);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CSeekBar, SeekBar);

ECode CSeekBar::constructor(
    /* [in] */ IContext* ctx)
{
    return SeekBar::Init(ctx);
}

ECode CSeekBar::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return SeekBar::Init(ctx, attrs);
}

ECode CSeekBar::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return SeekBar::Init(ctx, attrs, defStyle);
}

PInterface CSeekBar::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CSeekBar::Probe(riid);
}

ECode CSeekBar::IsIndeterminate(
    /* [out] */ Boolean* isIndeterminate)
{
    VALIDATE_NOT_NULL(isIndeterminate);
    *isIndeterminate = SeekBar::IsIndeterminate();

    return NOERROR;
}

ECode CSeekBar::SetIndeterminate(
    /* [in] */ Boolean indeterminate)
{
    return SeekBar::SetIndeterminate(indeterminate);
}

ECode CSeekBar::GetIndeterminateDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> _drawable = SeekBar::GetIndeterminateDrawable();
    *drawable = _drawable;
    INTERFACE_ADDREF(*drawable);

    return NOERROR;
}

ECode CSeekBar::SetIndeterminateDrawable(
    /* [in] */ IDrawable* drawable)
{
    return SeekBar::SetIndeterminateDrawable(drawable);
}

ECode CSeekBar::GetProgressDrawable(
    /* [out] */ IDrawable ** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> _drawable = SeekBar::GetProgressDrawable();
    *drawable = _drawable;
    INTERFACE_ADDREF(*drawable);

    return NOERROR;
}

ECode CSeekBar::SetProgressDrawable(
    /* [in] */ IDrawable * drawable)
{
    return SeekBar::SetProgressDrawable(drawable);
}

ECode CSeekBar::SetProgress(
    /* [in] */ Int32 progress)
{
    return SeekBar::SetProgress(progress);
}

ECode CSeekBar::SetSecondaryProgress(
    /* [in] */ Int32 secondaryProgress)
{
    return SeekBar::SetSecondaryProgress(secondaryProgress);
}

ECode CSeekBar::GetProgress(
    /* [out] */ Int32* progress)
{
    VALIDATE_NOT_NULL(progress);
    *progress = SeekBar::GetProgress();

    return NOERROR;
}

ECode CSeekBar::GetSecondaryProgress(
    /* [out] */ Int32* secProgress)
{
    VALIDATE_NOT_NULL(secProgress);
    *secProgress = SeekBar::GetSecondaryProgress();

    return NOERROR;
}

ECode CSeekBar::GetMax(
    /* [out] */ Int32* max)
{
    VALIDATE_NOT_NULL(max);
    *max = SeekBar::GetMax();

    return NOERROR;
}

ECode CSeekBar::SetMax(
    /* [in] */ Int32 max)
{
    return SeekBar::SetMax(max);
}

ECode CSeekBar::IncrementProgressBy(
    /* [in] */ Int32 diff)
{
    return SeekBar::IncrementProgressBy(diff);
}

ECode CSeekBar::IncrementSecondaryProgressBy(
    /* [in] */ Int32 diff)
{
    return SeekBar::IncrementSecondaryProgressBy(diff);
}

ECode CSeekBar::SetInterpolator(
    /* [in] */ IContext* ctx,
    /* [in] */ Int32 resID)
{
    return SeekBar::SetInterpolator(ctx, resID);
}

ECode CSeekBar::SetInterpolator(
    /* [in] */ IInterpolator* interpolator)
{
    return SeekBar::SetInterpolator(interpolator);
}

ECode CSeekBar::GetInterpolator(
    /* [out] */ IInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    AutoPtr<IInterpolator> _interpolator = SeekBar::GetInterpolator();
    *interpolator = _interpolator;
    INTERFACE_ADDREF(*interpolator);

    return NOERROR;
}

ECode CSeekBar::SetThumb(
    /* [in] */ IDrawable* thumb)
{
    return SeekBar::SetThumb(thumb);
}

ECode CSeekBar::GetThumb(
    /* [out] */ IDrawable** thumb)
{
    VALIDATE_NOT_NULL(thumb);
    AutoPtr<IDrawable> drawable = SeekBar::GetThumb();
    *thumb = drawable;
    INTERFACE_ADDREF(*thumb);
    return NOERROR;
}

ECode CSeekBar::GetThumbOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = SeekBar::GetThumbOffset();

    return NOERROR;
}

ECode CSeekBar::SetThumbOffset(
    /* [in] */ Int32 thumbOffset)
{
    return SeekBar::SetThumbOffset(thumbOffset);
}

ECode CSeekBar::SetKeyProgressIncrement(
    /* [in] */ Int32 increment)
{
    return SeekBar::SetKeyProgressIncrement(increment);
}

ECode CSeekBar::GetKeyProgressIncrement(
    /* [out] */ Int32* increment)
{
    VALIDATE_NOT_NULL(increment);
    *increment = SeekBar::GetKeyProgressIncrement();

    return NOERROR;
}

ECode CSeekBar::SetOnSeekBarChangeListener(
    /* [in] */ ISeekBarOnSeekBarChangeListener* l)
{
    return SeekBar::SetOnSeekBarChangeListener(l);
}

Mutex* CSeekBar::GetSelfLock()
{
    return &_m_syncLock;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
