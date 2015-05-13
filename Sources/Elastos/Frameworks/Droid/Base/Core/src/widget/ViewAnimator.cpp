
#include "widget/ViewAnimator.h"
#ifdef DROID_CORE
#include "view/animation/CAnimationUtils.h"
#endif
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::View::Animation::CAnimationUtils;
using Elastos::Droid::View::Animation::IAnimationUtils;
using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Widget {

ViewAnimator::ViewAnimator()
    : mWhichChild(0)
    , mFirstTime(TRUE)
    , mAnimateFirstTime(TRUE)
{}

ViewAnimator::ViewAnimator(
    /* [in] */ IContext* context)
    : FrameLayout(context)
    , mWhichChild(0)
    , mFirstTime(TRUE)
    , mAnimateFirstTime(TRUE)
{
    InitViewAnimator(context, NULL);
}

ViewAnimator::ViewAnimator(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : FrameLayout(context, attrs)
    , mWhichChild(0)
    , mFirstTime(TRUE)
    , mAnimateFirstTime(TRUE)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::ViewAnimator),
            ARRAY_SIZE(R::styleable::ViewAnimator));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributesEx2(
            attrs, attrIds, (ITypedArray**)&a));
    Int32 resource;
    a->GetResourceId(R::styleable::ViewAnimator_inAnimation, 0, &resource);

    if (resource > 0) {
        SetInAnimation(context, resource);
    }

    a->GetResourceId(R::styleable::ViewAnimator_outAnimation, 0, &resource);
    if (resource > 0) {
        SetOutAnimation(context, resource);
    }

    Boolean flag;
    a->GetBoolean(R::styleable::ViewAnimator_animateFirstView, TRUE, &flag);
    SetAnimateFirstView(flag);

    a->Recycle();
    InitViewAnimator(context, NULL);
}

void ViewAnimator::InitViewAnimator(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    if (attrs == NULL) {
        // For compatibility, always measure children when undefined.
        SetMeasureAllChildren(TRUE);
        return;
    }

    // For compatibility, default to measure children, but allow XML
    // attribute to override.
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::FrameLayout),
            ARRAY_SIZE(R::styleable::FrameLayout));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributesEx2(
            attrs, attrIds, (ITypedArray**)&a));

    Boolean measureAllChildren;
    a->GetBoolean(R::styleable::FrameLayout_measureAllChildren, TRUE, &measureAllChildren);
    SetMeasureAllChildren(measureAllChildren);
    a->Recycle();
}

ECode ViewAnimator::SetDisplayedChild(
    /* [in] */ Int32 whichChild)
{
    mWhichChild = whichChild;
    if (whichChild >= GetChildCount()) {
        mWhichChild = 0;
    } else if (whichChild < 0) {
        mWhichChild = GetChildCount() - 1;
    }
    Boolean hasFocus = GetFocusedChild() != NULL;
    // This will clear old focus if we had it
    ShowOnly(mWhichChild);
    if (hasFocus) {
        // Try to retake focus if we had it
        RequestFocus(IView::FOCUS_FORWARD);
    }
    return NOERROR;
}

ECode ViewAnimator::GetDisplayedChild(
    /* [in] */ Int32* child)
{
    *child = mWhichChild;
    return NOERROR;
}

ECode ViewAnimator::ShowNext()
{
    return SetDisplayedChild(mWhichChild + 1);
}

ECode ViewAnimator::ShowPrevious()
{
    return SetDisplayedChild(mWhichChild - 1);
}

ECode ViewAnimator::ShowOnly(
    /* [in] */ Int32 childIndex,
    /* [in] */ Boolean animate)
{
    Int32 count = GetChildCount();
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        if (i == childIndex) {
            if (animate && mInAnimation != NULL) {
                child->StartAnimation(mInAnimation);
            }
            child->SetVisibility(IView::VISIBLE);
            mFirstTime = FALSE;
        } else {
            Int32 visible;
            child->GetVisibility(&visible);
            AutoPtr<IAnimation> animTemp;
            child->GetAnimation((IAnimation**)&animTemp);
            if (animate && mOutAnimation != NULL && visible == IView::VISIBLE) {
                child->StartAnimation(mOutAnimation);
            } else if (animTemp == mInAnimation) {
                child->ClearAnimation();
            }
            child->SetVisibility(IView::GONE);
        }
    }
    return NOERROR;
}

ECode ViewAnimator::ShowOnly(
    /* [in] */ Int32 childIndex) {
    Boolean animate = (!mFirstTime || mAnimateFirstTime);
    return ShowOnly(childIndex, animate);
}

ECode ViewAnimator::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    FrameLayout::AddView(child, index, params);
    if (GetChildCount() == 1) {
        child->SetVisibility(IView::VISIBLE);
    } else {
        child->SetVisibility(IView::GONE);
    }
    if (index >= 0 && mWhichChild >= index) {
        // Added item above current one, increment the index of the displayed child
        SetDisplayedChild(mWhichChild + 1);
    }
    return NOERROR;
}

ECode ViewAnimator::RemoveAllViews()
{
    FrameLayout::RemoveAllViews();
    mWhichChild = 0;
    mFirstTime = TRUE;
    return NOERROR;
}

ECode ViewAnimator::RemoveView(
    /* [IN] */ IView* view)
{
    Int32 index = IndexOfChild(view);
    if (index >= 0) {
        RemoveViewAt(index);
    }
    return NOERROR;
}

ECode ViewAnimator::RemoveViewAt(
    /* [in] */ Int32 index)
{
    FrameLayout::RemoveViewAt(index);
    Int32 childCount = GetChildCount();
    if (childCount == 0) {
        mWhichChild = 0;
        mFirstTime = TRUE;
    } else if (mWhichChild >= childCount) {
        // Displayed is above child count, so float down to top of stack
        SetDisplayedChild(childCount - 1);
    } else if (mWhichChild == index) {
        // Displayed was removed, so show the new child living in its place
        SetDisplayedChild(mWhichChild);
    }
    return NOERROR;
}

ECode ViewAnimator::RemoveViewInLayout(
    /* [in] */ IView* view)
{
    return RemoveView(view);
}

ECode ViewAnimator::RemoveViews(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    FrameLayout::RemoveViews(start, count);
    if (GetChildCount() == 0) {
        mWhichChild = 0;
        mFirstTime = TRUE;
    } else if (mWhichChild >= start && mWhichChild < start + count) {
        // Try showing new displayed child, wrapping if needed
        SetDisplayedChild(mWhichChild);
    }
    return NOERROR;
}

ECode ViewAnimator::RemoveViewsInLayout(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    return RemoveViews(start, count);
}

ECode ViewAnimator::GetCurrentView(
    /* [out] */ IView** curView)
{
    AutoPtr<IView> temp = GetChildAt(mWhichChild);
    *curView = temp;
    INTERFACE_ADDREF(*curView);
    return NOERROR;
}

ECode ViewAnimator::GetInAnimation(
    /* [out] */ IAnimation** anim)
{
    *anim = mInAnimation;
    INTERFACE_ADDREF(*anim);
    return NOERROR;
}

ECode ViewAnimator::SetInAnimation(
    /* [in] */ IAnimation* inAnimation)
{
    mInAnimation = inAnimation;
    return NOERROR;
}

ECode ViewAnimator::GetOutAnimation(
    /* [out] */ IAnimation** anim)
{
    *anim = mOutAnimation;
    INTERFACE_ADDREF(*anim);
    return NOERROR;
}

ECode ViewAnimator::SetOutAnimation(
    /* [in] */ IAnimation* outAnimation)
{
    mOutAnimation = outAnimation;
    return NOERROR;
}

ECode ViewAnimator::SetInAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
    AutoPtr<IAnimation> anim;
    AutoPtr<IAnimationUtils> utils;
    CAnimationUtils::AcquireSingleton((IAnimationUtils**)&utils);
    utils->LoadAnimation(context, resourceID, (IAnimation**)&anim);
    return SetInAnimation(anim);
}

ECode ViewAnimator::SetOutAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
    AutoPtr<IAnimation> anim;
    AutoPtr<IAnimationUtils> utils;
    CAnimationUtils::AcquireSingleton((IAnimationUtils**)&utils);
    utils->LoadAnimation(context, resourceID, (IAnimation**)&anim);
    return SetOutAnimation(anim);
}

ECode ViewAnimator::GetAnimateFirstView(
    /* [out] */ Boolean* animate)
{
    *animate = mAnimateFirstTime;
    return NOERROR;
}

ECode ViewAnimator::SetAnimateFirstView(
    /* [in] */ Boolean animate)
{
    mAnimateFirstTime = animate;
    return NOERROR;
}

ECode ViewAnimator::GetBaseline(
    /* [out] */ Int32* baseline)
{
    AutoPtr<IView> view;
    GetCurrentView((IView**)&view);
    Int32 viewBaseline;
    view->GetBaseline(&viewBaseline);
    Int32 superBaseline;
    FrameLayout::GetBaseline(&superBaseline);
    *baseline = (view != NULL) ? viewBaseline : superBaseline;
    return NOERROR;
}

ECode ViewAnimator::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    FrameLayout::OnInitializeAccessibilityEvent(event);
    String className("CViewAnimator");
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(className, (ICharSequence**)&cs);
    event->SetClassName(cs);
    return NOERROR;
}

ECode ViewAnimator::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    FrameLayout::OnInitializeAccessibilityNodeInfo(info);
    String className("CViewAnimator");
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(className, (ICharSequence**)&cs);
    info->SetClassName(cs);
    return NOERROR;
}

ECode ViewAnimator::Init(
    /* [in] */ IContext* context)
{
    FrameLayout::Init(context);
    InitViewAnimator(context, NULL);
    return NOERROR;
}

ECode ViewAnimator::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FrameLayout::Init(context, attrs);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::ViewAnimator),
            ARRAY_SIZE(R::styleable::ViewAnimator));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributesEx2(
            attrs, attrIds, (ITypedArray**)&a));
    Int32 resource;
    a->GetResourceId(R::styleable::ViewAnimator_inAnimation, 0, &resource);

    if (resource > 0) {
        SetInAnimation(context, resource);
    }

    a->GetResourceId(R::styleable::ViewAnimator_outAnimation, 0, &resource);
    if (resource > 0) {
        SetOutAnimation(context, resource);
    }

    Boolean flag;
    a->GetBoolean(R::styleable::ViewAnimator_animateFirstView, TRUE, &flag);
    SetAnimateFirstView(flag);

    a->Recycle();
    InitViewAnimator(context, NULL);
    return NOERROR;
}

}// namespace Elastos
}// namespace Droid
}// namespace Widget
