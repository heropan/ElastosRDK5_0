#include "widget/RatingBar.h"
#include <elastos/Math.h>
#include "Elastos.Core.h"
#include "graphics/drawable/shapes/CRectShape.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::Graphics::Drawable::Shapes::CRectShape;
using Elastos::Droid::Graphics::Drawable::Shapes::IRectShape;

namespace Elastos {
namespace Droid {
namespace Widget {


const String RatingBar::RATINGBAR_NAME("CRatingBar");

RatingBar::RatingBar(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : AbsSeekBar(context, attrs, defStyle)
    , mNumStars(5)
    , mProgressOnStartTracking(0)
{
    Init(context, attrs, defStyle);
}

RatingBar::RatingBar()
    : mNumStars(5)
    , mProgressOnStartTracking(0)
{
}

ECode RatingBar::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(AbsSeekBar::Init(context, attrs, defStyle));
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::RatingBar),
        ARRAY_SIZE(R::styleable::RatingBar));

    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributesEx3(attrs, attrIds, defStyle, 0, (ITypedArray**)&a));
    Int32 numStars = 0;
    a->GetInt32(R::styleable::RatingBar_numStars, mNumStars, &numStars);
    Boolean res = FALSE;
    a->GetBoolean(R::styleable::RatingBar_isIndicator, !mIsUserSeekable, &res);
    SetIsIndicator(res);
    Float rating = 0, stepSize = 0;
    a->GetFloat(R::styleable::RatingBar_rating, -1, &rating);
    a->GetFloat(R::styleable::RatingBar_stepSize, -1, &stepSize);
    a->Recycle();

    if(numStars > 0 && numStars != mNumStars) {
        SetNumStars(numStars);
    }

    if(stepSize > 0) {
        SetStepSize(stepSize);
    } else {
        SetStepSize(0.5f);
    }

    if(rating > 0) {
        SetRating(rating);
    }

    // A touch inside a star fill up to that fractional area (slightly more
    // than 1 so boundaries round up).
    mTouchProgressOffset = 1.1f;

    return NOERROR;
}

ECode RatingBar::SetOnRatingBarChangeListener(
    /* [in] */ IOnRatingBarChangeListener* listener)
{
    mOnRatingBarChangeListener = listener;
    return NOERROR;
}

AutoPtr<IOnRatingBarChangeListener> RatingBar::GetOnRatingBarChangeListener()
{
    return mOnRatingBarChangeListener;
}

ECode RatingBar::SetIsIndicator(
    /* [in] */ Boolean isIndicator)
{
    mIsUserSeekable = !isIndicator;
    SetFocusable(!isIndicator);
    return NOERROR;
}

Boolean RatingBar::IsIndicator()
{
    return !mIsUserSeekable;
}

ECode RatingBar::SetNumStars(
    /* [in] */ Int32 numStars)
{
    if (numStars < 0) {
        return NOERROR;
    }
    mNumStars = numStars;
    RequestLayout();

    return NOERROR;
}

Int32 RatingBar::GetNumStars()
{
    return mNumStars;
}

ECode RatingBar::SetRating(
    /* [in] */ Float rating)
{
    SetProgress(Elastos::Core::Math::Round(rating * GetProgressPerStar()));
    return NOERROR;
}

Float RatingBar::GetRating()
{
    return GetProgress() / GetProgressPerStar();
}

ECode RatingBar::SetStepSize(
    /* [in] */ Float stepSize)
{
    if (stepSize <= 0) {
        return NOERROR;
    }
    Float newMax = mNumStars / stepSize;
    Int32 newProgress = (Int32)(newMax / GetMax() * GetProgress());
    SetMax((Int32)newMax);
    SetProgress(newProgress);
    return NOERROR;
}

Float RatingBar::GetStepSize()
{
    return (Float)GetNumStars() / GetMax();
}

ECode RatingBar::SetMax(
    /* [in] */ Int32 max)
{
    if (max <= 0) {
        return NOERROR;
    }
    AbsSeekBar::SetMax(max);
    return NOERROR;
}

ECode RatingBar::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    AbsSeekBar::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(RATINGBAR_NAME, (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

ECode RatingBar::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    AbsSeekBar::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(RATINGBAR_NAME, (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}

Float RatingBar::GetProgressPerStar()
{
    if (mNumStars > 0) {
        return 1.f * GetMax() / mNumStars;
    } else {
        return 1.f;
    }
}

ECode RatingBar::UpdateSecondaryProgress(
    /* [in] */ Int32 progress)
{
    Float ratio = GetProgressPerStar();
    if (ratio > 0) {
        Float progressInStars = progress / ratio;
        Int32 secondaryProgress = (Int32)(Elastos::Core::Math::Ceil(progressInStars) * ratio);
        SetSecondaryProgress(secondaryProgress);
    }
    return NOERROR;
}

AutoPtr<IShape> RatingBar::GetDrawableShape()
{
    AutoPtr<IRectShape> shape;
    CRectShape::New((IRectShape**)&shape);
    return shape;
}

void RatingBar::OnProgressRefresh(
    /* [in] */ Float scale,
    /* [in] */ Boolean fromUser)
{
    AbsSeekBar::OnProgressRefresh(scale, fromUser);
    UpdateSecondaryProgress(GetProgress());
    if (!fromUser) {
        DispatchRatingChange(FALSE);
    }
}

void RatingBar::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    AbsSeekBar::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    if(mSampleTile != NULL) {
        Int32 width = 0;
        mSampleTile->GetWidth(&width);
        width *= mNumStars;
        SetMeasuredDimension(ResolveSizeAndState(width, widthMeasureSpec, 0), GetMeasuredHeight());
    }
}

void RatingBar::OnStartTrackingTouch()
{
    mProgressOnStartTracking = GetProgress();
    AbsSeekBar::OnStartTrackingTouch();
}

void RatingBar::OnStopTrackingTouch()
{
    AbsSeekBar::OnStopTrackingTouch();
    if (GetProgress() != mProgressOnStartTracking) {
        DispatchRatingChange(TRUE);
    }
}

void RatingBar::OnKeyChange()
{
    AbsSeekBar::OnKeyChange();
    DispatchRatingChange(TRUE);
}


ECode RatingBar::DispatchRatingChange(
    /* [in] */ Boolean fromUser)
{
    if (mOnRatingBarChangeListener != NULL) {
        mOnRatingBarChangeListener->OnRatingChanged((IRatingBar*)this->Probe(EIID_IRatingBar), GetRating(), fromUser);
    }

    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
