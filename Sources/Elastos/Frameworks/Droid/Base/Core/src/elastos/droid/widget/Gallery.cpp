
#include "widget/Gallery.h"
#include <elastos/core/Math.h>
#include "view/CViewConfiguration.h"
#include "view/SoundEffectConstants.h"
#include "view/CGestureDetector.h"
#include "widget/GalleryLayoutParams.h"
#include "widget/CGalleryLayoutParams.h"
#include "widget/CScroller.h"


using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::View::SoundEffectConstants;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IHapticFeedbackConstants;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::View::CViewConfiguration;
using Elastos::Droid::View::CGestureDetector;
using Elastos::Droid::View::IOnGestureListener;
using Elastos::Droid::View::EIID_IOnGestureListener;
using Elastos::Droid::Widget::CGalleryLayoutParams;
using Elastos::Droid::Widget::CScroller;

namespace Elastos{
namespace Droid{
namespace Widget{


//==============================================================================
//              Gallery::FlingRunnable
//==============================================================================

Gallery::FlingRunnable::FlingRunnable(
    /* [in] */ Gallery* host)
    : mHost(host)
    , mLastFlingX(0)
{
    CScroller::New(mHost->GetContext(), (IScroller**)&mScroller);
}

void Gallery::FlingRunnable::StartCommon()
{
    mHost->RemoveCallbacks((IRunnable*)this);
}

void Gallery::FlingRunnable::StartUsingVelocity(
    /* [in] */ Int32 initialVelocity)
{
    if (initialVelocity == 0) return;

    StartCommon();

    Int32 initialX = initialVelocity < 0 ? Elastos::Core::Math::INT32_MAX_VALUE : 0;
    mLastFlingX = initialX;
    mScroller->Fling(initialX, 0, initialVelocity, 0,
        0, Elastos::Core::Math::INT32_MAX_VALUE, 0, Elastos::Core::Math::INT32_MAX_VALUE);
    AutoPtr<IRunnable> r = (IRunnable*)this;
    mHost->Post(r);
}

void Gallery::FlingRunnable::StartUsingDistance(
    /* [in] */ Int32 distance)
{
    if (distance == 0) return;

    StartCommon();

    mLastFlingX = 0;
    mScroller->StartScroll(0, 0, -distance, 0, mHost->mAnimationDuration);
    AutoPtr<IRunnable> r = (IRunnable*)this;
    mHost->Post(r);
}

void Gallery::FlingRunnable::Stop(
    /* [in] */ Boolean scrollIntoSlots)
{
    mHost->RemoveCallbacks((IRunnable*)this);
    EndFling(scrollIntoSlots);
}

void Gallery::FlingRunnable::EndFling(
    /* [in] */ Boolean scrollIntoSlots)
{
    mScroller->ForceFinished(TRUE);

    if (scrollIntoSlots) mHost->ScrollIntoSlots();
}

ECode Gallery::FlingRunnable::Run()
{
    if (mHost->mItemCount == 0) {
        EndFling(TRUE);
        return NOERROR;
    }

    mHost->mShouldStopFling = FALSE;

    AutoPtr<IScroller> scroller = mScroller;
    Boolean more = FALSE;
    scroller->ComputeScrollOffset(&more);
    Int32 x = 0;
    scroller->GetCurrX(&x);

    Int32 delta = mLastFlingX - x;

    if (delta > 0) {
        mHost->mDownTouchPosition = mHost->mIsRtl ? (mHost->mFirstPosition + mHost->GetChildCount() - 1) :
            mHost->mFirstPosition;

    delta = Elastos::Core::Math::Min(mHost->GetWidth() - mHost->mPaddingLeft - mHost->mPaddingRight - 1, delta);
    } else {
        Int32 offsetToLast = mHost->GetChildCount() - 1;
        mHost->mDownTouchPosition = mHost->mIsRtl ? mHost->mFirstPosition :
            (mHost->mFirstPosition + mHost->GetChildCount() - 1);

        delta = Elastos::Core::Math::Max(-(mHost->GetWidth() - mHost->mPaddingRight - mHost->mPaddingLeft - 1), delta);
    }

    mHost->TrackMotionScroll(delta);

    if (more && !mHost->mShouldStopFling) {
        mLastFlingX = x;
        AutoPtr<IRunnable> r = (IRunnable*)this;
        mHost->Post(r);
    }
    else {
       EndFling(TRUE);
    }
    return NOERROR;
}

//==============================================================================
//              Gallery::DisableSuppressSelectionChangedRunnable
//==============================================================================

Gallery::DisableSuppressSelectionChangedRunnable::DisableSuppressSelectionChangedRunnable(
    /* [in] */ Gallery* host)
    : mHost(host)
{}

ECode Gallery::DisableSuppressSelectionChangedRunnable::Run()
{
    mHost->mSuppressSelectionChanged = FALSE;
    mHost->SelectionChanged();

    return NOERROR;
}

//==============================================================================
//              Gallery::KeyUpRunnable
//==============================================================================

Gallery::KeyUpRunnable::KeyUpRunnable(
    /* [in] */ Gallery* host)
    : mHost(host)
{}


ECode Gallery::KeyUpRunnable::Run()
{
    mHost->DispatchUnpress();

    return NOERROR;
}

//==============================================================================
//              Gallery
//==============================================================================
const Boolean Gallery::localLOGV;
const Int32 Gallery::SCROLL_TO_FLING_UNCERTAINTY_TIMEOUT;
const String Gallery::GALLERY_NAME = String("Gallery");

Gallery::Gallery()
    : mSpacing(0)
    , mAnimationDuration(400)
    , mUnselectedAlpha(0.0)
    , mLeftMost(0)
    , mRightMost(0)
    , mGravity(0)
    , mDownTouchPosition(0)
    , mShouldStopFling(FALSE)
    , mShouldCallbackDuringFling(TRUE)
    , mShouldCallbackOnUnselectedItemClick(TRUE)
    , mSuppressSelectionChanged(FALSE)
    , mReceivedInvokeKeyDown(FALSE)
    , mIsFirstScroll(FALSE)
    , mIsRtl(TRUE)
    , mSelectedCenterOffset(0)
{
}

Gallery::~Gallery()
{
}

ECode Gallery::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(AbsSpinner::Init(context, attrs, defStyle));

    FAIL_RETURN(CGestureDetector::New(context, (IOnGestureListener*)this->Probe(EIID_IOnGestureListener),
        (IGestureDetector**)&mGestureDetector));
    mGestureDetector->SetIsLongpressEnabled(TRUE);

    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
    mFlingRunnable = new FlingRunnable(this);
    mDisableSuppressSelectionChangedRunnable =
            new DisableSuppressSelectionChangedRunnable(this);

    // We draw the selected item last (because otherwise the item to the
    // right overlaps it)
    mGroupFlags |= FLAG_USE_CHILD_DRAWING_ORDER;
    mGroupFlags |= FLAG_SUPPORT_STATIC_TRANSFORMATIONS;
    return NOERROR;
}

ECode Gallery::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::Gallery),
            ARRAY_SIZE(R::styleable::Gallery));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributes(
        attrs, attrIds, defStyle, 0, (ITypedArray**)&a));

    Int32 index = 0;
    a->GetInt32(R::styleable::Gallery_gravity, -1, &index);
    if (index >= 0) {
        SetGravity(index);
    }

    Int32 animationDuration = 0;
    a->GetInt32(R::styleable::Gallery_animationDuration, -1, &animationDuration);
    if (animationDuration > 0) {
        SetAnimationDuration(animationDuration);
    }

    Int32 spacing = 0;
    a->GetDimensionPixelOffset(R::styleable::Gallery_spacing, 0, &spacing);
    SetSpacing(spacing);

    Float unselectedAlpha = 0;
    a->GetFloat(
           R::styleable::Gallery_unselectedAlpha, 0.5f, &unselectedAlpha);
    SetUnselectedAlpha(unselectedAlpha);

    a->Recycle();
    return NOERROR;
}

ECode Gallery::SetCallbackDuringFling(
    /* [in] */ Boolean shouldCallback)
{
    mShouldCallbackDuringFling = shouldCallback;

    return NOERROR;
}

ECode Gallery::SetCallbackOnUnselectedItemClick(
    /* [in] */ Boolean shouldCallback)
{
    mShouldCallbackOnUnselectedItemClick = shouldCallback;

    return NOERROR;
}

ECode Gallery::SetAnimationDuration(
    /* [in] */ Int32 animationDurationMillis)
{
    mAnimationDuration = animationDurationMillis;

    return NOERROR;
}

ECode Gallery::SetSpacing(
    /* [in] */ Int32 spacing)
{
    mSpacing = spacing;

    return NOERROR;
}

ECode Gallery::SetUnselectedAlpha(
    /* [in] */ Float unselectedAlpha)
{
    mUnselectedAlpha = unselectedAlpha;

    return NOERROR;
}

Boolean Gallery::GetChildStaticTransformation(
    /* [in] */ IView* child,
    /* [in] */ ITransformation* t)
{
    t->Clear();
    t->SetAlpha(child == mSelectedChild ? 1.0f : mUnselectedAlpha);

    return TRUE;
}

Int32 Gallery::ComputeHorizontalScrollExtent()
{
    return 1;
}

Int32 Gallery::ComputeHorizontalScrollOffset()
{
    return mSelectedPosition;
}

Int32 Gallery::ComputeHorizontalScrollRange()
{
    return mItemCount;
}

Boolean Gallery::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return IGalleryLayoutParams::Probe(p) != NULL;
}

AutoPtr<IViewGroupLayoutParams> Gallery::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    AutoPtr<IGalleryLayoutParams> lp;
    CGalleryLayoutParams::New(p, (IGalleryLayoutParams**)&lp);
    return lp;
}

ECode Gallery::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<IGalleryLayoutParams> lp;
    FAIL_RETURN(CGalleryLayoutParams::New(GetContext(), attrs, (IGalleryLayoutParams**)&lp));
    *params = IViewGroupLayoutParams::Probe(lp);
    REFCOUNT_ADD(*params);
    return NOERROR;
}

AutoPtr<IViewGroupLayoutParams> Gallery::GenerateDefaultLayoutParams()
{
    AutoPtr<IGalleryLayoutParams> lp;
    CGalleryLayoutParams::New(IViewGroupLayoutParams::WRAP_CONTENT,
            IViewGroupLayoutParams::WRAP_CONTENT, (IGalleryLayoutParams**)&lp);
    return lp;
}

void Gallery::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    AbsSpinner::OnLayout(changed, l, t, r, b);

    mInLayout = TRUE;
    Layout(0, FALSE);
    mInLayout = FALSE;
}


Int32 Gallery::GetChildHeight(
    /* [in] */ IView* child)
{
    Int32 height = 0;
    child->GetMeasuredHeight(&height);

    return height;
}

void Gallery::TrackMotionScroll(
    /* [in] */ Int32 deltaX)
{
    if (GetChildCount() == 0) {
        return;
    }

    Boolean toLeft = deltaX < 0;

    Int32 limitedDeltaX = GetLimitedMotionScrollAmount(toLeft, deltaX);
    if (limitedDeltaX != deltaX) {
        mFlingRunnable->EndFling(FALSE);
        OnFinishedMovement();
    }

    OffsetChildrenLeftAndRight(limitedDeltaX);

    DetachOffScreenChildren(toLeft);

    if (toLeft) {
        FillToGalleryRight();
    }
    else {
        FillToGalleryLeft();
    }

    mRecycler->Clear();

    SetSelectionToCenterChild();

    AutoPtr<IView> selChild = mSelectedChild;
    if (selChild != NULL) {
        Int32 childLeft = 0;
        selChild->GetLeft(&childLeft);
        Int32 childCenter = 0;
        selChild->GetWidth(&childCenter);
        childCenter = childCenter / 2;
        Int32 galleryCenter = 0;
        selChild->GetWidth(&galleryCenter);
        galleryCenter = galleryCenter / 2;
        mSelectedCenterOffset = childLeft + childCenter - galleryCenter;
    }

    OnScrollChanged(0, 0, 0, 0);

    Invalidate();
}

Int32 Gallery::GetLimitedMotionScrollAmount(
    /* [in] */ Boolean motionToLeft,
    /* [in] */ Int32 deltaX)
{
    Int32 extremeItemPosition = motionToLeft != mIsRtl ? mItemCount - 1 : 0;
    AutoPtr<IView> extremeChild = GetChildAt(extremeItemPosition - mFirstPosition);

    if (extremeChild == NULL) {
        return deltaX;
    }

    Int32 extremeChildCenter = GetCenterOfView(extremeChild);
    Int32 galleryCenter = GetCenterOfGallery();

    if (motionToLeft) {
        if (extremeChildCenter <= galleryCenter) {
            return 0;
        }
    }
    else {
        if (extremeChildCenter >= galleryCenter) {
            return 0;
        }
    }

    Int32 centerDifference = galleryCenter - extremeChildCenter;

    return motionToLeft
        ? Elastos::Core::Math::Max(centerDifference, deltaX)
        : Elastos::Core::Math::Min(centerDifference, deltaX);
}

void Gallery::OffsetChildrenLeftAndRight(
    /* [in] */ Int32 offset)
{
    for (Int32 i = GetChildCount() - 1; i >= 0; i--) {
        GetChildAt(i)->OffsetLeftAndRight(offset);
    }
}

Int32 Gallery::GetCenterOfGallery()
{
    return (GetWidth() - mPaddingLeft - mPaddingRight) / 2 + mPaddingLeft;
}

Int32 Gallery::GetCenterOfView(
    /* [in] */ IView* view)
{
    Int32 left, width;
    view->GetLeft(&left);
    view->GetWidth(&width);

    return left + width / 2;
}

void Gallery::DetachOffScreenChildren(
    /* [in] */ Boolean toLeft)
{
    Int32 numChildren = GetChildCount();
    Int32 firstPosition = mFirstPosition;
    Int32 start = 0;
    Int32 count = 0;

    if (toLeft) {
        Int32 galleryLeft = mPaddingLeft;
        for (Int32 i = 0; i < numChildren; i++) {
            int n = mIsRtl ? (numChildren - 1 - i) : i;

            AutoPtr<IView> child = GetChildAt(n);
            Int32 right;
            child->GetRight(&right);
            if (right >= galleryLeft) {
                break;
            }
            else {
                start = n;
                count++;
                mRecycler->Put(firstPosition + n, child);
            }
        }
        if (!mIsRtl) {
            start = 0;
        }
    }
    else {
        Int32 galleryRight = GetWidth() - mPaddingRight;
        for (Int32 i = numChildren - 1; i >= 0; i--) {
            int n = mIsRtl ? numChildren - 1 - i : i;
            AutoPtr<IView> child = GetChildAt(n);
            Int32 left;
            child->GetLeft(&left);
            if (left <= galleryRight) {
                break;
            }
            else {
                start = n;
                count++;
                mRecycler->Put(firstPosition + n, child);
            }
        }
        if (mIsRtl) {
            start = 0;
        }
    }

    DetachViewsFromParent(start, count);

    if (toLeft != mIsRtl) {
        mFirstPosition += count;
    }
}

void Gallery::ScrollIntoSlots()
{
    if (GetChildCount() == 0 || mSelectedChild == NULL) return;

    Int32 selectedCenter = GetCenterOfView(mSelectedChild);
    Int32 targetCenter = GetCenterOfGallery();

    Int32 scrollAmount = targetCenter - selectedCenter;
    if (scrollAmount != 0) {
        mFlingRunnable->StartUsingDistance(scrollAmount);
    }
    else {
        OnFinishedMovement();
    }
}

void Gallery::OnFinishedMovement()
{
    if (mSuppressSelectionChanged) {
        mSuppressSelectionChanged = FALSE;
        AbsSpinner::SelectionChanged();
    }
    mSelectedCenterOffset = 0;

    Invalidate();
}


void Gallery::SelectionChanged()
{
    if (!mSuppressSelectionChanged) {
        AbsSpinner::SelectionChanged();
    }
}

void Gallery::SetSelectionToCenterChild()
{
    AutoPtr<IView> selView = mSelectedChild;
    if (mSelectedChild == NULL) return;

    Int32 galleryCenter = GetCenterOfGallery();

    Int32 left, right;
    selView->GetLeft(&left);
    selView->GetRight(&right);
    if (left <= galleryCenter && right >= galleryCenter) {
        return;
    }

    Int32 closestEdgeDistance = Elastos::Core::Math::INT32_MAX_VALUE;
    Int32 newSelectedChildIndex = 0;
    for (Int32 i = GetChildCount() - 1; i >= 0; i--) {

        AutoPtr<IView> child = GetChildAt(i);

        child->GetLeft(&left);
        child->GetRight(&right);
        if (left <= galleryCenter && right >= galleryCenter) {
            newSelectedChildIndex = i;
            break;
        }

        Int32 childClosestEdgeDistance = Elastos::Core::Math::Min(Elastos::Core::Math::Abs(left - galleryCenter),
            Elastos::Core::Math::Abs(right - galleryCenter));
        if (childClosestEdgeDistance < closestEdgeDistance) {
            closestEdgeDistance = childClosestEdgeDistance;
            newSelectedChildIndex = i;
        }
    }

    Int32 newPos = mFirstPosition + newSelectedChildIndex;

    if (newPos != mSelectedPosition) {
        SetSelectedPositionInt(newPos);
        SetNextSelectedPositionInt(newPos);
        CheckSelectionChanged();
    }
}

void Gallery::Layout(
    /* [in] */ Int32 delta,
    /* [in] */ Boolean animate)
{
    mIsRtl = IsLayoutRtl();

    Int32 childrenLeft = mSpinnerPadding->mLeft;
    Int32 childrenWidth = mRight - mLeft - mSpinnerPadding->mLeft - mSpinnerPadding->mRight;

    if (mDataChanged) {
        HandleDataChanged();
    }

    if (mItemCount == 0) {
        ResetList();
        return;
    }

    if (mNextSelectedPosition >= 0) {
        SetSelectedPositionInt(mNextSelectedPosition);
    }

    RecycleAllViews();

    DetachAllViewsFromParent();

    mRightMost = 0;
    mLeftMost = 0;


    mFirstPosition = mSelectedPosition;
    AutoPtr<IView> sel = MakeAndAddView(mSelectedPosition, 0, 0, TRUE);

    Int32 width = 0;
    sel->GetWidth(&width);
    Int32 selectedOffset = childrenLeft + (childrenWidth / 2) - (width / 2) +
        mSelectedCenterOffset;
    sel->OffsetLeftAndRight(selectedOffset);

    FillToGalleryRight();
    FillToGalleryLeft();

    mRecycler->Clear();

    Invalidate();
    CheckSelectionChanged();

    mDataChanged = FALSE;
    mNeedSync = FALSE;
    SetNextSelectedPositionInt(mSelectedPosition);

    UpdateSelectedItemMetadata();
}

void Gallery::FillToGalleryLeft()
{
    if (mIsRtl) {
        FillToGalleryLeftRtl();
    } else {
        FillToGalleryLeftLtr();
    }
}

void Gallery::FillToGalleryLeftRtl()
{
    Int32 itemSpacing = mSpacing;
    Int32 galleryLeft = mPaddingLeft;
    Int32 numChildren = GetChildCount();

    AutoPtr<IView> prevIterationView = GetChildAt(numChildren - 1);
    Int32 curPosition = 0;
    Int32 curRightEdge = 0;


    Int32 preLeft = 0;
    if (prevIterationView != NULL) {
        curPosition = mFirstPosition + numChildren;
        prevIterationView->GetLeft(&preLeft);
        curRightEdge =  preLeft - itemSpacing;
    } else {
        mFirstPosition = curPosition = mItemCount - 1;
        curRightEdge = mRight - mLeft - mPaddingRight;
        mShouldStopFling = TRUE;
    }

    while (curRightEdge > galleryLeft && curPosition < mItemCount) {
        prevIterationView = MakeAndAddView(curPosition, curPosition - mSelectedPosition,
            curRightEdge, FALSE);

        prevIterationView->GetLeft(&preLeft);
        curRightEdge = preLeft - itemSpacing;
        curPosition++;
    }
}

void Gallery::FillToGalleryLeftLtr()
{
    Int32 itemSpacing = mSpacing;
    Int32 galleryLeft = mPaddingLeft;

    AutoPtr<IView> prevIterationView = GetChildAt(0);
    Int32 curPosition = 0;
    Int32 curRightEdge = 0;

    Int32 left = 0;
    if (prevIterationView != NULL) {
        curPosition = mFirstPosition - 1;
        prevIterationView->GetLeft(&left);
        curRightEdge = left - itemSpacing;
    }
    else {
        curPosition = 0;
        curRightEdge = mRight - mLeft - mPaddingRight;
        mShouldStopFling = TRUE;
    }

    while (curRightEdge > galleryLeft && curPosition >= 0) {
        prevIterationView = MakeAndAddView(curPosition,
                curPosition - mSelectedPosition,
                curRightEdge, FALSE);

        mFirstPosition = curPosition;

        prevIterationView->GetLeft(&left);
        curRightEdge = left - itemSpacing;
        curPosition--;
    }
}

void Gallery::FillToGalleryRight()
{
    if (mIsRtl) {
        FillToGalleryRightRtl();
    } else {
        FillToGalleryRightLtr();
    }
}

void Gallery::FillToGalleryRightRtl()
{
    Int32 itemSpacing = mSpacing;
    Int32 galleryRight = mRight - mLeft - mPaddingRight;

    AutoPtr<IView> prevIterationView = GetChildAt(0);
    Int32 curPosition = 0;
    Int32 curLeftEdge = 0;

    Int32 preRight = 0;
    if (prevIterationView != NULL) {
        curPosition = mFirstPosition -1;
        prevIterationView->GetRight(&preRight);
        curLeftEdge = preRight + itemSpacing;
    } else {
        curPosition = 0;
        curLeftEdge = mPaddingLeft;
        mShouldStopFling = TRUE;
    }

    while (curLeftEdge < galleryRight && curPosition >= 0) {
        prevIterationView = MakeAndAddView(curPosition, curPosition - mSelectedPosition,
            curLeftEdge, TRUE);

        mFirstPosition = curPosition;

        prevIterationView->GetRight(&preRight);
        curLeftEdge = preRight + itemSpacing;
        curPosition--;
    }
}

void Gallery::FillToGalleryRightLtr()
{
    Int32 itemSpacing = mSpacing;
    Int32 galleryRight = mRight - mLeft - mPaddingRight;
    Int32 numChildren = GetChildCount();
    Int32 numItems = mItemCount;

    AutoPtr<IView> prevIterationView = GetChildAt(numChildren - 1);
    Int32 curPosition = 0;
    Int32 curLeftEdge = 0;

    Int32 right = 0;
    if (prevIterationView.Get() != NULL) {
        curPosition = mFirstPosition + numChildren;
        prevIterationView->GetRight(&right);
        curLeftEdge = right + itemSpacing;
    }
    else {
        mFirstPosition = curPosition = mItemCount - 1;
        curLeftEdge = mPaddingLeft;
        mShouldStopFling = TRUE;
    }

    while (curLeftEdge < galleryRight && curPosition < numItems) {
        prevIterationView = MakeAndAddView(curPosition,
                curPosition - mSelectedPosition,
                curLeftEdge, TRUE);

        prevIterationView->GetRight(&right);
        curLeftEdge = right + itemSpacing;
        curPosition++;
    }
}
AutoPtr<IView> Gallery::MakeAndAddView(
    /* [in] */ Int32 position,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 x,
    /* [in] */ Boolean fromLeft)
{
    AutoPtr<IView> child;

    if (!mDataChanged) {
        child = mRecycler->Get(position);
        if (child != NULL) {
            Int32 childLeft = 0;
            child->GetLeft(&childLeft);

            Int32 width = 0;
            child->GetMeasuredWidth(&width);
            mRightMost = Elastos::Core::Math::Max(mRightMost, childLeft + width);
            mLeftMost = Elastos::Core::Math::Min(mLeftMost, childLeft);

            SetUpChild(child, offset, x, fromLeft);

            return child;
        }
    }
    child = NULL;
    mAdapter->GetView(position, NULL, (IViewGroup*)this->Probe(EIID_IViewGroup), (IView**)&child);

    SetUpChild(child, offset, x, fromLeft);

    return child;
}

void Gallery::SetUpChild(
    /* [in] */ IView* child,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 x,
    /* [in] */ Boolean fromLeft)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    if (lp == NULL) {
        lp = GenerateDefaultLayoutParams();
    }

    AddViewInLayout(child, fromLeft != mIsRtl ? -1 : 0, lp);

    child->SetSelected(offset == 0);

    Int32 h;
    lp->GetHeight(&h);
    Int32 childHeightSpec = ViewGroup::GetChildMeasureSpec(mHeightMeasureSpec,
            mSpinnerPadding->mTop + mSpinnerPadding->mBottom, h);
    Int32 w;
    lp->GetWidth(&w);
    Int32 childWidthSpec = ViewGroup::GetChildMeasureSpec(mWidthMeasureSpec,
            mSpinnerPadding->mLeft + mSpinnerPadding->mRight, w);

    child->Measure(childWidthSpec, childHeightSpec);

    Int32 childLeft = 0;
    Int32 childRight = 0;

    Int32 childTop = CalculateTop(child, TRUE);

    Int32 height = 0;
    child->GetMeasuredHeight(&height);
    Int32 childBottom = childTop + height;

    Int32 width;
    child->GetMeasuredWidth(&width);
    if (fromLeft) {
        childLeft = x;
        childRight = childLeft + width;
    }
    else {
        childLeft = x - width;
        childRight = x;
    }

    child->Layout(childLeft, childTop, childRight, childBottom);
}

Int32 Gallery::CalculateTop(
    /* [in] */ IView* child,
    /* [in] */ Boolean duringLayout)
{
    Int32 myHeight = duringLayout ? GetMeasuredHeight() : GetHeight();

    Int32 measuredHeight = 0;
    Int32 height = 0;
    child->GetMeasuredHeight(&measuredHeight);
    child->GetHeight(&height);
    Int32 childHeight = duringLayout ? measuredHeight : height;

    Int32 childTop = 0;

    switch (mGravity) {
    case IGravity::TOP:
        childTop = mSpinnerPadding->mTop;
        break;
    case IGravity::CENTER_VERTICAL: {
        Int32 availableSpace = myHeight - mSpinnerPadding->mBottom
                - mSpinnerPadding->mTop - childHeight;
        childTop = mSpinnerPadding->mTop + (availableSpace / 2);
        break;
    }
    case IGravity::BOTTOM:
        childTop = myHeight - mSpinnerPadding->mBottom - childHeight;
        break;
    }
    return childTop;
}

Boolean Gallery::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Boolean retValue = FALSE;
    mGestureDetector->OnTouchEvent(event, &retValue);

    Int32 action = 0;
    event->GetAction(&action);
    if (action == IMotionEvent::ACTION_UP) {
        OnUp();
    }
    else if (action == IMotionEvent::ACTION_CANCEL) {
        OnCancel();
    }

    return retValue;
}

Boolean Gallery::OnSingleTapUp(
    /* [in] */ IMotionEvent* e)
{
    if (mDownTouchPosition >= 0) {

        ScrollToChild(mDownTouchPosition - mFirstPosition);
        if (mShouldCallbackOnUnselectedItemClick || mDownTouchPosition == mSelectedPosition) {
            Int64 id = 0;
            mAdapter->GetItemId(mDownTouchPosition, &id);
            PerformItemClick(mDownTouchView, mDownTouchPosition, id);
        }

        return TRUE;
    }
    return FALSE;
}

Boolean Gallery::OnFling(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float velocityX,
    /* [in] */ Float velocityY)
{
    if (!mShouldCallbackDuringFling) {

        RemoveCallbacks(mDisableSuppressSelectionChangedRunnable);

        if (!mSuppressSelectionChanged) mSuppressSelectionChanged = TRUE;
    }

    mFlingRunnable->StartUsingVelocity((Int32)-velocityX);

    return TRUE;
}

Boolean Gallery::OnScroll(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float distanceX,
    /* [in] */ Float distanceY)
{
    mParent->RequestDisallowInterceptTouchEvent(TRUE);

    if (!mShouldCallbackDuringFling) {
        if (mIsFirstScroll) {
            if (!mSuppressSelectionChanged) mSuppressSelectionChanged = TRUE;
            PostDelayed(mDisableSuppressSelectionChangedRunnable, SCROLL_TO_FLING_UNCERTAINTY_TIMEOUT);
        }
    }
    else {
        if (mSuppressSelectionChanged) mSuppressSelectionChanged = FALSE;
    }

    TrackMotionScroll(-1 * (Int32) distanceX);

    mIsFirstScroll = FALSE;
    return TRUE;
}

Boolean Gallery::OnDown(
    /* [in] */ IMotionEvent* e)
{
    mFlingRunnable->Stop(FALSE);

    Float X = 0;
    Float Y = 0;
    e->GetX(&X);
    e->GetY(&Y);
    mDownTouchPosition = PointToPosition((Int32)X, (Int32)Y);

    if (mDownTouchPosition >= 0) {
        mDownTouchView = GetChildAt(mDownTouchPosition - mFirstPosition);
        mDownTouchView->SetPressed(TRUE);
    }
    mIsFirstScroll = TRUE;

    return TRUE;
}

ECode Gallery::OnUp()
{
    Boolean res = FALSE;
    mFlingRunnable->mScroller->IsFinished(&res);
    if (res) {
        ScrollIntoSlots();
    }

    DispatchUnpress();
    return NOERROR;
}

ECode Gallery::OnCancel()
{
    return OnUp();
}

ECode Gallery::OnLongPress(
    /* [in] */ IMotionEvent* e)
{
    if (mDownTouchPosition < 0) {
        return NOERROR;
    }

    PerformHapticFeedback(IHapticFeedbackConstants::LONG_PRESS);
    Int64 id = GetItemIdAtPosition(mDownTouchPosition);
    DispatchLongPress(mDownTouchView, mDownTouchPosition, id);
    return NOERROR;
}

ECode Gallery::OnShowPress(
    /* [in] */ IMotionEvent* e)
{
    return NOERROR;
}

void Gallery::DispatchPress(
    /* [in] */ IView* child)
{
    if (child != NULL) {
        child->SetPressed(TRUE);
    }

    SetPressed(TRUE);
}

void Gallery::DispatchUnpress()
{
    for (Int32 i = GetChildCount() - 1; i >= 0; i--) {
        GetChildAt(i)->SetPressed(FALSE);
    }

    SetPressed(FALSE);
}

void Gallery::DispatchSetSelected(
    /* [in] */ Boolean selected)
{
}

void Gallery::DispatchSetPressed(
    /* [in] */ Boolean pressed)
{
    if (mSelectedChild != NULL) {
        mSelectedChild->SetPressed(pressed);
    }
}

AutoPtr<IContextMenuInfo> Gallery::GetContextMenuInfo()
{
    return (IContextMenuInfo*)mContextMenuInfo.Get();
}

Boolean Gallery::ShowContextMenuForChild(
    /* [in] */ IView* originalView)
{
    Int32 longPressPosition = GetPositionForView(originalView);
    if (longPressPosition < 0) {
        return FALSE;
    }

    Int64 longPressId = 0;
    mAdapter->GetItemId(longPressPosition, &longPressId);
    return DispatchLongPress(originalView, longPressPosition, longPressId);
}

Boolean Gallery::ShowContextMenu()
{
    if (IsPressed() && mSelectedPosition >= 0) {
        Int32 index = mSelectedPosition - mFirstPosition;
        AutoPtr<IView> v = GetChildAt(index);
        return DispatchLongPress(v, mSelectedPosition, mSelectedRowId);
    }

    return FALSE;
}

Boolean Gallery::DispatchLongPress(
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    Boolean handled = FALSE;

    if (mOnItemLongClickListener != NULL) {
        mOnItemLongClickListener->OnItemLongClick(THIS_PROBE(IAdapterView),
                mDownTouchView, mDownTouchPosition, id, &handled);
    }

    if (!handled) {
        mContextMenuInfo = new AdapterContextMenuInfo(view, position, id);
        handled = AbsSpinner::ShowContextMenuForChild(THIS_PROBE(IView));
    }

    if (handled) {
        PerformHapticFeedback(IHapticFeedbackConstants::LONG_PRESS);
    }

    return handled;
}

Boolean Gallery::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    Boolean result = FALSE;
    event->Dispatch(THIS_PROBE(IKeyEventCallback), NULL, NULL, &result);

    return result;
}

Boolean Gallery::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    switch (keyCode) {

    case IKeyEvent::KEYCODE_DPAD_LEFT:
        if (MovePrevious()) {
            PlaySoundEffect(SoundEffectConstants::NAVIGATION_LEFT);
            return TRUE;
        }
        break;

    case IKeyEvent::KEYCODE_DPAD_RIGHT:
        if (MoveNext()) {
            PlaySoundEffect(SoundEffectConstants::NAVIGATION_RIGHT);
            return TRUE;
        }
        break;

    case IKeyEvent::KEYCODE_DPAD_CENTER:
    case IKeyEvent::KEYCODE_ENTER:
        mReceivedInvokeKeyDown = TRUE;
    }

    return AbsSpinner::OnKeyDown(keyCode, event);
}

Boolean Gallery::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    switch (keyCode) {
        case IKeyEvent::KEYCODE_DPAD_CENTER:
        case IKeyEvent::KEYCODE_ENTER: {

            if (mReceivedInvokeKeyDown) {
                if (mItemCount > 0) {
                    if (mKeyUpRunnable = NULL) {
                        mKeyUpRunnable = new KeyUpRunnable(this);
                    }
                    DispatchPress(mSelectedChild);
                    PostDelayed(mKeyUpRunnable, CViewConfiguration::GetPressedStateDuration());

                    Int32 selectedIndex = mSelectedPosition - mFirstPosition;

                    Int64 id = 0;
                    mAdapter->GetItemId(mSelectedPosition, &id);
                    PerformItemClick(GetChildAt(selectedIndex), mSelectedPosition, id);
                }
            }

            mReceivedInvokeKeyDown = FALSE;

            return TRUE;
        }
    }

    return AbsSpinner::OnKeyUp(keyCode, event);
}

Boolean Gallery::MovePrevious()
{
    if (mItemCount > 0 && mSelectedPosition > 0) {
        ScrollToChild(mSelectedPosition - mFirstPosition - 1);
        return TRUE;
    }
    else {
        return FALSE;
    }
}

Boolean Gallery::MoveNext()
{
    if (mItemCount > 0 && mSelectedPosition < mItemCount - 1) {
        ScrollToChild(mSelectedPosition - mFirstPosition + 1);
        return TRUE;
    }
    else {
        return FALSE;
    }
}

Boolean Gallery::ScrollToChild(
    /* [in] */ Int32 childPosition)
{
    AutoPtr<IView> child = GetChildAt(childPosition);

    if (child != NULL) {
        Int32 distance = GetCenterOfGallery() - GetCenterOfView(child);
        mFlingRunnable->StartUsingDistance(distance);
        return TRUE;
    }

    return FALSE;
}

void Gallery::SetSelectedPositionInt(
    /* [in] */ Int32 position)
{
    AbsSpinner::SetSelectedPositionInt(position);

    UpdateSelectedItemMetadata();
}

void Gallery::UpdateSelectedItemMetadata()
{
    AutoPtr<IView> oldSelectedChild = mSelectedChild;

    AutoPtr<IView> child = mSelectedChild = GetChildAt(mSelectedPosition - mFirstPosition);
    if (child == NULL) {
        return;
    }

    child->SetSelected(TRUE);
    child->SetFocusable(TRUE);

    if (HasFocus()) {
        Boolean result = FALSE;
        child->RequestFocus(&result);
    }

    if (oldSelectedChild != NULL && oldSelectedChild != child) {
        oldSelectedChild->SetSelected(FALSE);
        oldSelectedChild->SetFocusable(FALSE);
    }
}

ECode Gallery::SetGravity(
    /* [in] */ Int32 gravity)
{
    if (mGravity != gravity) {
        mGravity = gravity;
        RequestLayout();
    }

    return NOERROR;
}

Int32 Gallery::GetChildDrawingOrder(
    /* [in] */ Int32 childCount,
    /* [in] */ Int32 i)
{
    Int32 selectedIndex = mSelectedPosition - mFirstPosition;

    if (selectedIndex < 0) return i;

    if (i == childCount - 1) {
        return selectedIndex;
    }
    else if (i >= selectedIndex) {
        return i + 1;
    }
    else {
        return i;
    }
}

void Gallery::OnFocusChanged(
    /* [in] */ Boolean gainFocus,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    AbsSpinner::OnFocusChanged(gainFocus, direction, previouslyFocusedRect);

    if (gainFocus && mSelectedChild != NULL) {
        Boolean isFocus = FALSE;
        mSelectedChild->RequestFocus(direction, &isFocus);
        mSelectedChild->SetSelected(TRUE);
    }
}

ECode Gallery::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    AbsSpinner::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(GALLERY_NAME, (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

ECode Gallery::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    AbsSpinner::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(GALLERY_NAME, (ICharSequence**)&seq));
    info->SetClassName(seq);
    info->SetScrollable(mItemCount > 1);
    if(IsEnabled()) {
        if(mItemCount > 0 && mSelectedPosition < mItemCount - 1){
            info->AddAction(IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD);
        }
        if(IsEnabled() && mItemCount > 0 && mSelectedPosition > 0){
            info->AddAction(IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD);
        }
    }
    return NOERROR;
}

Boolean Gallery::PerformAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    if (AbsSpinner::PerformAccessibilityAction(action, arguments)) {
        return TRUE;
    }
    switch (action) {
        case IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD: {
            if (IsEnabled() && mItemCount > 0 && mSelectedPosition < mItemCount - 1) {
                Int32 currentChildIndex = mSelectedPosition - mFirstPosition;
                return ScrollToChild(currentChildIndex + 1);
            }
        } return FALSE;
        case IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD: {
            if (IsEnabled() && mItemCount > 0 && mSelectedPosition > 0) {
                Int32 currentChildIndex = mSelectedPosition - mFirstPosition;
                return ScrollToChild(currentChildIndex - 1);
            }
        } return FALSE;
    }
    return FALSE;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
