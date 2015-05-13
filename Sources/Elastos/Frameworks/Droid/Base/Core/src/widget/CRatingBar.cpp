#include "CRatingBar.h"
#include "view/ViewGroup.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;
using Elastos::Droid::View::ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CRatingBar, RatingBar)

ECode CRatingBar::constructor(
    /* [in] */ IContext* ctx)
{
    return RatingBar::Init(ctx);
}

ECode CRatingBar::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return RatingBar::Init(ctx, attrs);
}

ECode CRatingBar::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return RatingBar::Init(ctx, attrs, defStyle);
}

PInterface CRatingBar::Probe(
        /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    } else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CRatingBar::Probe(riid);
}

Mutex* CRatingBar::GetSelfLock()
{
    return &mLock;
}

ECode CRatingBar::SetOnRatingBarChangeListener(
    /* [in] */ IOnRatingBarChangeListener* listener)
{
    return RatingBar::SetOnRatingBarChangeListener(listener);
}

ECode CRatingBar::GetOnRatingBarChangeListener(
    /* [out] */ IOnRatingBarChangeListener** listener)
{
    AutoPtr<IOnRatingBarChangeListener> lTemp = RatingBar::GetOnRatingBarChangeListener();
    *listener = lTemp;
    INTERFACE_ADDREF(*listener);
    return NOERROR;
}

ECode CRatingBar::SetIsIndicator(
    /* [in] */ Boolean isIndicator)
{
    return RatingBar::SetIsIndicator(isIndicator);
}

ECode CRatingBar::IsIndicator(
    /* [out] */ Boolean* isIndicator)
{
    *isIndicator = RatingBar::IsIndicator();
    return NOERROR;
}

ECode CRatingBar::SetNumStars(
    /* [in] */ Int32 numStars)
{
    return RatingBar::SetNumStars(numStars);
}

ECode CRatingBar::GetNumStars(
    /* [out] */ Int32* numStars)
{
    *numStars = RatingBar::GetNumStars();
    return NOERROR;
}

ECode CRatingBar::SetRating(
    /* [in] */ Float rating)
{
    return RatingBar::SetRating(rating);
}

ECode CRatingBar::GetRating(
    /* [out] */ Float* rating)
{
    *rating = RatingBar::GetRating();
    return NOERROR;
}

ECode CRatingBar::SetStepSize(
    /* [in] */ Float stepSize)
{
    return RatingBar::SetStepSize(stepSize);
}

ECode CRatingBar::GetStepSize(
    /* [out] */ Float* stepSize)
{
    *stepSize = RatingBar::GetStepSize();
    return NOERROR;
}

ECode CRatingBar::SetThumb(
    /* [in] */ IDrawable* thumb)
{
    return RatingBar::SetThumb(thumb);
}

ECode CRatingBar::GetThumb(
    /* [out] */ IDrawable** thumb)
{
    AutoPtr<IDrawable> temp = RatingBar::GetThumb();
    *thumb = temp;
    INTERFACE_ADDREF(*thumb);
    return NOERROR;
}

ECode CRatingBar::GetThumbOffset(
    /* [out] */ Int32* offset)
{
    *offset = RatingBar::GetThumbOffset();
    return NOERROR;
}

ECode CRatingBar::SetThumbOffset(
    /* [in] */ Int32 thumbOffset)
{
    return RatingBar::SetThumbOffset(thumbOffset);
}

ECode CRatingBar::SetKeyProgressIncrement(
    /* [in] */ Int32 increment)
{
    return RatingBar::SetKeyProgressIncrement(increment);
}

ECode CRatingBar::GetKeyProgressIncrement(
    /* [out] */ Int32* increment)
{
    *increment = RatingBar::GetKeyProgressIncrement();
    return NOERROR;
}

ECode CRatingBar::IsIndeterminate(
    /* [out] */ Boolean* indeterminate)
{
    *indeterminate = RatingBar::IsIndeterminate();
    return NOERROR;
}

ECode CRatingBar::SetIndeterminate(
    /* [in] */ Boolean indeterminate)
{
    return RatingBar::SetIndeterminate(indeterminate);
}

ECode CRatingBar::GetIndeterminateDrawable(
    /* [out] */ IDrawable** d)
{
    AutoPtr<IDrawable> temp = RatingBar::GetIndeterminateDrawable();
    *d = temp;
    INTERFACE_ADDREF(*d);
    return NOERROR;
}

ECode CRatingBar::SetIndeterminateDrawable(
    /* [in] */ IDrawable* d)
{
    return RatingBar::SetIndeterminateDrawable(d);
}

ECode CRatingBar::GetProgressDrawable(
    /* [out] */ IDrawable** d)
{
    AutoPtr<IDrawable> temp = RatingBar::GetProgressDrawable();
    *d = temp;
    INTERFACE_ADDREF(*d);
    return NOERROR;
}

ECode CRatingBar::SetProgressDrawable(
    /* [in] */ IDrawable* d)
{
    return RatingBar::SetProgressDrawable(d);
}

ECode CRatingBar::SetProgress(
    /* [in] */ Int32 progress)
{
    return RatingBar::SetProgress(progress);
}

ECode CRatingBar::SetSecondaryProgress(
    /* [in] */ Int32 secondaryProgress)
{
    return RatingBar::SetSecondaryProgress(secondaryProgress);
}

ECode CRatingBar::GetProgress(
    /* [out] */ Int32* progress)
{
    *progress = RatingBar::GetProgress();
    return NOERROR;
}

ECode CRatingBar::GetSecondaryProgress(
    /* [out] */ Int32* secondaryProgress)
{
    *secondaryProgress = RatingBar::GetSecondaryProgress();
    return NOERROR;
}

ECode CRatingBar::GetMax(
    /* [out] */ Int32* max)
{
    *max = RatingBar::GetMax();
    return NOERROR;
}

ECode CRatingBar::SetMax(
    /* [in] */ Int32 max)
{
    RatingBar::SetMax(max);
    return NOERROR;
}

ECode CRatingBar::IncrementProgressBy(
    /* [in] */ Int32 diff)
{
    return RatingBar::IncrementProgressBy(diff);
}

ECode CRatingBar::IncrementSecondaryProgressBy(
    /* [in] */ Int32 diff)
{
    return RatingBar::IncrementSecondaryProgressBy(diff);
}

ECode CRatingBar::SetInterpolator(
    /* [in] */ IContext* ctx,
    /* [in] */ Int32 resID)
{
    return RatingBar::SetInterpolator(ctx, resID);
}

ECode CRatingBar::SetInterpolatorEx(
    /* [in] */ IInterpolator* interpolator)
{
    return RatingBar::SetInterpolator(interpolator);
}

ECode CRatingBar::GetInterpolator(
    /* [out] */ IInterpolator** interpolator)
{
    AutoPtr<IInterpolator> temp = RatingBar::GetInterpolator();
    *interpolator = temp;
    INTERFACE_ADDREF(*interpolator);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
