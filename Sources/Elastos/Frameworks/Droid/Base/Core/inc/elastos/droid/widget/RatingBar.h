#ifndef __ELASTOS_DROID_WIDGET_RATINGBAR_H__
#define __ELASTOS_DROID_WIDGET_RATINGBAR_H__

#include "elastos/droid/ext/frameworkext.h"
#include "widget/AbsSeekBar.h"

namespace Elastos {
namespace Droid {
namespace Widget {

class RatingBar : public AbsSeekBar
{
public:
    RatingBar(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::ratingBarStyle);

    virtual CARAPI SetOnRatingBarChangeListener(
        /* [in] */ IOnRatingBarChangeListener* listener);

    virtual CARAPI_(AutoPtr<IOnRatingBarChangeListener>) GetOnRatingBarChangeListener();

    virtual CARAPI SetIsIndicator(
        /* [in] */ Boolean isIndicator);

    virtual CARAPI_(Boolean) IsIndicator();

    virtual CARAPI SetNumStars(
        /* [in] */ Int32 numStars);

    virtual CARAPI_(Int32) GetNumStars();

    virtual CARAPI SetRating(
        /* [in] */ Float rating);

    virtual CARAPI_(Float) GetRating();

    virtual CARAPI SetStepSize(
        /* [in] */ Float stepSize);

    virtual CARAPI_(Float) GetStepSize();

    virtual CARAPI SetMax(
        /* [in] */ Int32 max);

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);


private:
    CARAPI_(Float) GetProgressPerStar();

    CARAPI UpdateSecondaryProgress(
        /* [in] */ Int32 progress);

protected:
    RatingBar();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::ratingBarStyle);

    CARAPI_(AutoPtr<IShape>) GetDrawableShape();

    CARAPI_(void) OnProgressRefresh(
        /* [in] */ Float scale,
        /* [in] */ Boolean fromUser);

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI_(void) OnStartTrackingTouch();

    CARAPI_(void) OnStopTrackingTouch();

    CARAPI_(void) OnKeyChange();


    CARAPI DispatchRatingChange(
        /* [in] */ Boolean fromUser);

private:

    static const String RATINGBAR_NAME;
    Int32 mNumStars;

    Int32 mProgressOnStartTracking;

    AutoPtr<IOnRatingBarChangeListener> mOnRatingBarChangeListener;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_RATINGBAR_H__
