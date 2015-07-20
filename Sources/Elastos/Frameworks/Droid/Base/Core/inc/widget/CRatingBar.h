#ifndef __CRATINGBAR_H__
#define __CRATINGBAR_H__

#include "_CRatingBar.h"

#include "RatingBar.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CRatingBar) , public RatingBar
{
public:

    IVIEW_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

    CARAPI SetOnRatingBarChangeListener(
        /* [in] */ IOnRatingBarChangeListener* listener);

    CARAPI GetOnRatingBarChangeListener(
        /* [out] */ IOnRatingBarChangeListener** listener);

    CARAPI SetIsIndicator(
        /* [in] */ Boolean isIndicator);

    CARAPI IsIndicator(
        /* [out] */ Boolean* isIndicator);

    CARAPI SetNumStars(
        /* [in] */ Int32 numStars);

    CARAPI GetNumStars(
        /* [out] */ Int32* numStars);

    CARAPI SetRating(
        /* [in] */ Float rating);

    CARAPI GetRating(
        /* [out] */ Float* rating);

    CARAPI SetStepSize(
        /* [in] */ Float stepSize);

    CARAPI GetStepSize(
        /* [out] */ Float* stepSize);

    CARAPI SetThumb(
        /* [in] */ IDrawable* thumb);

    CARAPI GetThumb(
        /* [out] */ IDrawable** thumb);

    CARAPI GetThumbOffset(
        /* [out] */ Int32* offset);

    CARAPI SetThumbOffset(
        /* [in] */ Int32 thumbOffset);

    CARAPI SetKeyProgressIncrement(
        /* [in] */ Int32 increment);

    CARAPI GetKeyProgressIncrement(
        /* [out] */ Int32* increment);

    CARAPI IsIndeterminate(
        /* [out] */ Boolean* indeterminate);

    CARAPI SetIndeterminate(
        /* [in] */ Boolean indeterminate);

    CARAPI GetIndeterminateDrawable(
        /* [out] */ IDrawable** d);

    CARAPI SetIndeterminateDrawable(
        /* [in] */ IDrawable* d);

    CARAPI GetProgressDrawable(
        /* [out] */ IDrawable** d);

    CARAPI SetProgressDrawable(
        /* [in] */ IDrawable* d);

    CARAPI SetProgress(
        /* [in] */ Int32 progress);

    CARAPI SetSecondaryProgress(
        /* [in] */ Int32 secondaryProgress);

    CARAPI GetProgress(
        /* [out] */ Int32* progress);

    CARAPI GetSecondaryProgress(
        /* [out] */ Int32* secondaryProgress);

    CARAPI GetMax(
        /* [out] */ Int32* max);

    CARAPI SetMax(
        /* [in] */ Int32 max);

    CARAPI IncrementProgressBy(
        /* [in] */ Int32 diff);

    CARAPI IncrementSecondaryProgressBy(
        /* [in] */ Int32 diff);

    CARAPI SetInterpolator(
        /* [in] */ IContext* ctx,
        /* [in] */ Int32 resID);

    CARAPI SetInterpolator(
        /* [in] */ IInterpolator* interpolator);

    CARAPI GetInterpolator(
        /* [out] */ IInterpolator** interpolator);
protected:
    CARAPI_(Object*) GetSelfLock();

private:
    Object mLock;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
