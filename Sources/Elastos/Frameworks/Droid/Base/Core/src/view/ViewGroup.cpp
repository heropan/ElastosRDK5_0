
#include "view/ViewGroup.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/CMotionEvent.h"
#include "view/ViewRootImpl.h"
#include "view/FocusFinder.h"
#include "view/animation/Animation.h"
#include "view/animation/AnimationUtils.h"
#include "view/animation/CTransformation.h"
#include "view/animation/CLayoutAnimationController.h"
#include "view/animation/CTransformation.h"
#include "view/animation/LayoutAnimationController.h"
#include "animation/CLayoutTransition.h"
#include "graphics/CPaint.h"
#include "graphics/CPointF.h"
#include "os/Build.h"
#include "os/SystemClock.h"
#include "os/SystemProperties.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/Math.h>
#include "R.h"

using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Droid::Animation::CLayoutTransition;
using Elastos::Droid::Animation::EIID_ITransitionListener;
using Elastos::Droid::View::Animation::CTransformation;
using Elastos::Droid::View::Animation::LayoutAnimationController;
using Elastos::Droid::View::Animation::CLayoutAnimationController;
using Elastos::Droid::View::Animation::Animation;
using Elastos::Droid::View::Animation::AnimationUtils;
using Elastos::Droid::View::Animation::CTransformation;
using Libcore::ICU::ILocale;
using Elastos::Utility::Logging::Logger;
using Elastos::Core::CObjectContainer;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace View {

extern "C" const InterfaceID EIID_ViewGroup =
        { 0x9480e8f3, 0xf994, 0x4994, { 0xae, 0x66, 0x3a, 0xaf, 0x7f, 0x21, 0x7e, 0x62 } };

const Int32 ViewGroup::FOCUS_BEFORE_DESCENDANTS;
const Int32 ViewGroup::FOCUS_AFTER_DESCENDANTS;
const Int32 ViewGroup::FOCUS_BLOCK_DESCENDANTS;
const Int32 ViewGroup::PERSISTENT_NO_CACHE;
const Int32 ViewGroup::PERSISTENT_ANIMATION_CACHE;
const Int32 ViewGroup::PERSISTENT_SCROLLING_CACHE;
const Int32 ViewGroup::PERSISTENT_ALL_CACHES;
const Boolean ViewGroup::DBG = FALSE;
const char* ViewGroup::VG_TAG = "ViewGroup";
const Int32 ViewGroup::FLAG_CLIP_CHILDREN;
const Int32 ViewGroup::FLAG_CLIP_TO_PADDING;
const Int32 ViewGroup::FLAG_INVALIDATE_REQUIRED;
const Int32 ViewGroup::FLAG_RUN_ANIMATION;
const Int32 ViewGroup::FLAG_ANIMATION_DONE;
const Int32 ViewGroup::FLAG_PADDING_NOT_NULL;
const Int32 ViewGroup::FLAG_ANIMATION_CACHE;
const Int32 ViewGroup::FLAG_OPTIMIZE_INVALIDATE;
const Int32 ViewGroup::FLAG_CLEAR_TRANSFORMATION;
const Int32 ViewGroup::FLAG_NOTIFY_ANIMATION_LISTENER;
const Int32 ViewGroup::FLAG_ALPHA_LOWER_THAN_ONE;
const Int32 ViewGroup::FLAG_ADD_STATES_FROM_CHILDREN;
const Int32 ViewGroup::FLAG_ALWAYS_DRAWN_WITH_CACHE;
const Int32 ViewGroup::FLAG_CHILDREN_DRAWN_WITH_CACHE;
const Int32 ViewGroup::FLAG_NOTIFY_CHILDREN_ON_DRAWABLE_STATE_CHANGE;
const Int32 ViewGroup::FLAG_MASK_FOCUSABILITY;
const Int32 ViewGroup::DESCENDANT_FOCUSABILITY_FLAGS[] = {
    FOCUS_BEFORE_DESCENDANTS, FOCUS_AFTER_DESCENDANTS,
    FOCUS_BLOCK_DESCENDANTS
};
const Int32 ViewGroup::ARRAY_INITIAL_CAPACITY;
const Int32 ViewGroup::ARRAY_CAPACITY_INCREMENT;
const Int32 ViewGroup::FLAG_USE_CHILD_DRAWING_ORDER;
const Int32 ViewGroup::FLAG_SUPPORT_STATIC_TRANSFORMATIONS;
const Int32 ViewGroup::FLAG_DISALLOW_INTERCEPT;
const Int32 ViewGroup::CLIP_TO_PADDING_MASK;
const Int32 ViewGroup::CHILD_LEFT_INDEX;
const Int32 ViewGroup::CHILD_TOP_INDEX;

AutoPtr<IPaint> ViewGroup::sDebugPaint;
AutoPtr<ArrayOf<Float> > ViewGroup::sDebugLines;

const Int32 ViewGroup::TouchTarget::ALL_POINTER_IDS = -1; // all ones
const Int32 ViewGroup::TouchTarget::MAX_RECYCLED = 32;
Mutex ViewGroup::TouchTarget::sRecycleLock;
AutoPtr<ViewGroup::TouchTarget> ViewGroup::TouchTarget::sRecycleBin;
Int32 ViewGroup::TouchTarget::sRecycledCount = 0;

const Int32 ViewGroup::HoverTarget::MAX_RECYCLED = 32;
Mutex ViewGroup::HoverTarget::sRecycleLock;
AutoPtr<ViewGroup::HoverTarget> ViewGroup::HoverTarget::sRecycleBin;
Int32 ViewGroup::HoverTarget::sRecycledCount = 0;

const Int32 ViewGroup::ChildListForAccessibility::MAX_POOL_SIZE = 32;
Mutex ViewGroup::ChildListForAccessibility::sPoolLock;
AutoPtr<ViewGroup::ChildListForAccessibility> ViewGroup::ChildListForAccessibility::sPool;
Int32 ViewGroup::ChildListForAccessibility::sPoolSize = 0;

const Int32 ViewGroup::ViewLocationHolder::MAX_POOL_SIZE = 32;
Mutex ViewGroup::ViewLocationHolder::sPoolLock;
AutoPtr<ViewGroup::ViewLocationHolder> ViewGroup::ViewLocationHolder::sPool;
Int32 ViewGroup::ViewLocationHolder::sPoolSize = 0;


ViewGroup::NotifyAnimationListenerRunnable::NotifyAnimationListenerRunnable(
    /* [in] */ ViewGroup* host)
    : mHost(host)
{
}

ECode ViewGroup::NotifyAnimationListenerRunnable::Run()
{
    AutoPtr<IAnimation> animation;
    mHost->mLayoutAnimationController->GetAnimation((IAnimation**)&animation);
    mHost->mAnimationListener->OnAnimationEnd(animation);
    return NOERROR;
}

ViewGroup::DispatchDrawRunnable::DispatchDrawRunnable(
    /* [in] */ ViewGroup* host)
    : mHost(host)
{
}

ECode ViewGroup::DispatchDrawRunnable::Run()
{
    mHost->NotifyAnimationListener();
    return NOERROR;
}

CAR_INTERFACE_IMPL(ViewGroup::LayoutTransitionListener, ITransitionListener);

ViewGroup::LayoutTransitionListener::LayoutTransitionListener(
    /* [in] */ ViewGroup* host)
    : mHost(host)
{
}

ECode ViewGroup::LayoutTransitionListener::StartTransition(
    /* [in] */ ILayoutTransition* transition,
    /* [in] */ IViewGroup* container,
    /* [in] */ IView* view,
    /* [in] */ Int32 transitionType)
{
    // We only care about disappearing items, since we need special logic to keep
    // those items visible after they've been 'removed'
    if (transitionType == ILayoutTransition::DISAPPEARING) {
        mHost->StartViewTransition(view);
    }

    return NOERROR;
}

ECode ViewGroup::LayoutTransitionListener::EndTransition(
    /* [in] */ ILayoutTransition* transition,
    /* [in] */ IViewGroup* container,
    /* [in] */ IView* view,
    /* [in] */ Int32 transitionType)
{
    Boolean changing = FALSE;
    if (mHost->mLayoutSuppressed && !(transition->IsChangingLayout(&changing), changing)) {
        mHost->RequestLayout();
        mHost->mLayoutSuppressed = FALSE;
    }
    if (transitionType == ILayoutTransition::DISAPPEARING && !mHost->mTransitioningViews.IsEmpty()) {
        mHost->EndViewTransition(view);
    }

    return NOERROR;
}

ViewGroup::TouchTarget::TouchTarget()
    : mPointerIdBits(0)
{
}

AutoPtr<ViewGroup::TouchTarget> ViewGroup::TouchTarget::Obtain(
    /* [in] */ IView* child,
    /* [in] */ Int32 pointerIdBits)
{
    AutoPtr<TouchTarget> target;
    AutoLock lock(sRecycleLock);
    {
        if (sRecycleBin == NULL) {
            target = new TouchTarget();
        }
        else {
            target = sRecycleBin;
            sRecycleBin = target->mNext;
            sRecycledCount--;
            target->mNext = NULL;
        }
    }

    target->mChild = child;
    target->mPointerIdBits = pointerIdBits;
    return target;
}

void ViewGroup::TouchTarget::Recycle()
{
    AutoLock lock(sRecycleLock);
    {
        if (sRecycledCount < MAX_RECYCLED) {
            mNext = sRecycleBin;
            sRecycleBin = this;
            sRecycledCount += 1;
        }
        else {
            mNext = NULL;
        }
        mChild = NULL;
    }
}


ViewGroup::HoverTarget::HoverTarget()
{
}

AutoPtr<ViewGroup::HoverTarget> ViewGroup::HoverTarget::Obtain(
    /* [in] */ IView* child)
{
    AutoPtr<HoverTarget> target;
    AutoLock lock(sRecycleLock);
    {
        if (sRecycleBin == NULL) {
            target = new HoverTarget();
        }
        else {
            target = sRecycleBin;
            sRecycleBin = target->mNext;
             sRecycledCount--;
            target->mNext = NULL;
        }
    }
    target->mChild = child;
    return target;
}

void ViewGroup::HoverTarget::Recycle()
{
    AutoLock lock(sRecycleLock);
    {
        if (sRecycledCount < MAX_RECYCLED) {
            mNext = sRecycleBin;
            sRecycleBin = this;
            sRecycledCount += 1;
        }
        else {
            mNext = NULL;
        }
        mChild = NULL;
    }
}

ViewGroup::ChildListForAccessibility::ChildListForAccessibility()
    : mIsPooled(FALSE)
{
}

AutoPtr<ViewGroup::ChildListForAccessibility> ViewGroup::ChildListForAccessibility::Obtain(
    /* [in] */ IViewGroup* parent,
    /* [in] */ Boolean sort)
{
    AutoPtr<ChildListForAccessibility> list;
    AutoLock lock(sPoolLock);
    {
        if (sPool != NULL) {
            list = sPool;
            sPool = list->mNext;
            list->mNext = NULL;
            list->mIsPooled = FALSE;
            sPoolSize--;
        }
        else {
            list = new ChildListForAccessibility();
        }

        list->Init(parent, sort);
        return list;
    }
}

void ViewGroup::ChildListForAccessibility::Recycle()
{
    if (mIsPooled) {
        assert(0);
        //throw new IllegalStateException("Instance already recycled.");
    }

    Clear();
    AutoLock lock(sPoolLock);
    {
        if (sPoolSize < MAX_POOL_SIZE) {
            mNext = sPool;
            mIsPooled = TRUE;
            sPool = this;
            sPoolSize++;
        }
    }
}

Int32 ViewGroup::ChildListForAccessibility::GetChildCount()
{
    return mChildren.GetSize();
}

AutoPtr<IView> ViewGroup::ChildListForAccessibility::GetChildAt(
    /* [in] */ Int32 index)
{
    return mChildren[index];
}

Int32 ViewGroup::ChildListForAccessibility::GetChildIndex(
    /* [in] */ IView* child)
{
    List<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
    for (Int32 i = 0; iter != mChildren.End(); ++iter, ++i) {
        if (iter->Get() == child)
            return i;
    }
    return -1;
}

void ViewGroup::ChildListForAccessibility::Init(
    /* [in] */ IViewGroup* parent,
    /* [in] */ Boolean sort)
{
    Int32 childCount = 0;
    parent->GetChildCount(&childCount);
    for (Int32 i = 0; i < childCount; i++) {
        AutoPtr<IView> child;
        parent->GetChildAt(i, (IView**)&child);
        mChildren.PushBack(child);
    }

    if (sort) {
        List<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
        for (; iter != mChildren.End(); ++iter) {
            AutoPtr<ViewLocationHolder> holder = ViewLocationHolder::Obtain(parent, iter->Get());
            mHolders.PushBack(holder);
        }

        SortList(mHolders);

        iter = mChildren.Begin();
        List<AutoPtr<ViewLocationHolder> >::Iterator iter2 = mHolders.Begin();
        for (; iter != mChildren.End(); ++iter2, ++iter) {
            *iter = IVIEW_PROBE((*iter2)->mView);
            (*iter2)->Recycle();
        }

        mHolders.Clear();
    }
}

void ViewGroup::ChildListForAccessibility::Clear()
{
    mChildren.Clear();
}

void ViewGroup::ChildListForAccessibility::Merge(
    /* [in] */ List<AutoPtr<ViewLocationHolder> >& list1,
    /* [in] */ List<AutoPtr<ViewLocationHolder> >& list2)
{
    List<AutoPtr<ViewLocationHolder> >::Iterator first1 = list1.Begin();
    List<AutoPtr<ViewLocationHolder> >::Iterator last1 = list1.End();
    List<AutoPtr<ViewLocationHolder> >::Iterator first2 = list2.Begin();
    List<AutoPtr<ViewLocationHolder> >::Iterator last2 = list2.End();
    while (first1 != last1 && first2 != last2) {
        if ((*first1)->CompareTo(first2->Get()) < 0) {
            List<AutoPtr<ViewLocationHolder> >::Iterator next = first2;
            ++next;
            list1.Splice(first1, list2, first2);
            first2 = next;
        }
        else {
            ++first1;
        }
    }
    if (first2 != last2)
        list1.Splice(last1, list2, first2, last2);
}

void ViewGroup::ChildListForAccessibility::SortList(
    /* [in] */ List<AutoPtr<ViewLocationHolder> >& list)
{
    // Do nothing if the list has length 0 or 1.
    if (list.GetSize() > 1) {
        List<AutoPtr<ViewLocationHolder> > carry;
        List<AutoPtr<ViewLocationHolder> > counter[64];
        Int32 fill = 0;
        while (!list.IsEmpty()) {
            carry.Splice(carry.Begin(), list, list.Begin());
            Int32 i = 0;
            while(i < fill && !counter[i].IsEmpty()) {
                Merge(counter[i], carry);
                carry.Swap(counter[i++]);
            }
            carry.Swap(counter[i]);
            if (i == fill) ++fill;
        }

        for (Int32 i = 1; i < fill; ++i) {
            Merge(counter[i], counter[i-1]);
        }
        list.Swap(counter[fill-1]);
    }
}

ViewGroup::ViewLocationHolder::ViewLocationHolder()
    : mIsPooled(FALSE)
    , mLayoutDirection(0)
{
    CRect::NewByFriend((CRect**)&mLocation);
}

AutoPtr<ViewGroup::ViewLocationHolder> ViewGroup::ViewLocationHolder::Obtain(
    /* [in] */ IViewGroup* root,
    /* [in] */ IView* view)
{
    AutoPtr<ViewLocationHolder> holder;
    AutoLock lock(sPoolLock);
    {
        if (sPool != NULL) {
            holder = sPool;
            sPool = holder->mNext;
            holder->mNext = NULL;
            holder->mIsPooled = FALSE;
            sPoolSize--;
        }
        else {
            holder = new ViewLocationHolder();
        }
        holder->Init(root, view);
        return holder;
    }
}

void ViewGroup::ViewLocationHolder::Recycle()
{
    if (mIsPooled) {
        assert(0);
        //throw new IllegalStateException("Instance already recycled.");
    }
    Clear();

    AutoLock lock(sPoolLock);
    {
        if (sPoolSize < MAX_POOL_SIZE) {
            mNext = sPool;
            mIsPooled = TRUE;
            sPool = this;
            sPoolSize++;
        }
    }
}

Int32 ViewGroup::ViewLocationHolder::CompareTo(
    /* [in] */ ViewLocationHolder* another)
{
    // This instance is greater than an invalid argument.
    if (another == NULL) {
        return 1;
    }

    // if (getClass() != another.getClass()) {
    //     return 1;
    // }
    // First is above second.
    if (mLocation->mBottom - another->mLocation->mTop <= 0) {
        return -1;
    }
    // First is below second.
    if (mLocation->mTop - another->mLocation->mBottom >= 0) {
        return 1;
    }
    // LTR
    if (mLayoutDirection == IView::LAYOUT_DIRECTION_LTR) {
        Int32 leftDifference = mLocation->mLeft - another->mLocation->mLeft;
        // First more to the left than second.
        if (leftDifference != 0) {
            return leftDifference;
        }
    }
    else { // RTL
        Int32 rightDifference = mLocation->mRight - another->mLocation->mRight;
        // First more to the right than second.
        if (rightDifference != 0) {
            return -rightDifference;
        }
    }
    // Break tie by top.
    Int32 topDiference = mLocation->mTop - another->mLocation->mTop;
    if (topDiference != 0) {
        return topDiference;
    }
    // Break tie by height.
    Int32 h1, h2;
    mLocation->GetHeight(&h1);
    another->mLocation->GetHeight(&h2);
    Int32 heightDiference = h1 - h2;
    if (heightDiference != 0) {
        return -heightDiference;
    }
    // Break tie by width.
    Int32 w1, w2;
    mLocation->GetHeight(&w1);
    another->mLocation->GetHeight(&w2);
    Int32 widthDiference = w1 - w2;
    if (widthDiference != 0) {
        return -widthDiference;
    }

    // Just break the tie somehow. The accessibliity ids are unique
    // and stable, hence this is deterministic tie breaking.
    return mView->GetAccessibilityViewId() - another->mView->GetAccessibilityViewId();;
}

void ViewGroup::ViewLocationHolder::Init(
    /* [in] */ IViewGroup* root,
    /* [in] */ IView* view)
{
    view->GetDrawingRect(mLocation.Get());
    root->OffsetDescendantRectToMyCoords(view, mLocation.Get());
    assert(view != NULL);
    mView = VIEW_PROBE(view);
    root->GetLayoutDirection(&mLayoutDirection);
}

void ViewGroup::ViewLocationHolder::Clear()
{
    mView = NULL;
    mLocation->Set(0, 0, 0, 0);
}

ViewGroup::ViewGroup()
    : mGroupFlags(0)
    , mPersistentDrawingCache(0)
    , mChildAcceptsDrag(FALSE)
    , mLastTouchDownTime(0)
    , mLastTouchDownIndex(-1)
    , mLastTouchDownX(0.0f)
    , mLastTouchDownY(0.0f)
    , mHoveredSelf(FALSE)
    , mLayoutMode(CLIP_BOUNDS)
    , mLayoutSuppressed(FALSE)
    , mChildrenCount(0)
    , mChildCountWithTransientState(0)
{
    CTransformation::New((ITransformation**)&mChildTransformation);
    CPointF::New((IPointF**)&mLocalPoint);

    mLayoutTransitionListener = new LayoutTransitionListener(this);
}

ViewGroup::ViewGroup(
    /* [in] */ IContext* context)
    : View(context)
    , mGroupFlags(0)
    , mPersistentDrawingCache(0)
    , mChildAcceptsDrag(FALSE)
    , mLastTouchDownTime(0)
    , mLastTouchDownIndex(-1)
    , mLastTouchDownX(0.0f)
    , mLastTouchDownY(0.0f)
    , mHoveredSelf(FALSE)
    , mLayoutMode(CLIP_BOUNDS)
    , mLayoutSuppressed(FALSE)
    , mChildrenCount(0)
    , mChildCountWithTransientState(0)
{
    CTransformation::New((ITransformation**)&mChildTransformation);
    CPointF::New((IPointF**)&mLocalPoint);

    mLayoutTransitionListener = new LayoutTransitionListener(this);

    InitViewGroup();
}

ViewGroup::ViewGroup(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : View(context, attrs)
    , mGroupFlags(0)
    , mPersistentDrawingCache(0)
    , mChildAcceptsDrag(FALSE)
    , mLastTouchDownTime(0)
    , mLastTouchDownIndex(-1)
    , mLastTouchDownX(0.0f)
    , mLastTouchDownY(0.0f)
    , mHoveredSelf(FALSE)
    , mLayoutMode(CLIP_BOUNDS)
    , mLayoutSuppressed(FALSE)
    , mChildrenCount(0)
    , mChildCountWithTransientState(0)
{
    CTransformation::New((ITransformation**)&mChildTransformation);
    CPointF::New((IPointF**)&mLocalPoint);

    mLayoutTransitionListener = new LayoutTransitionListener(this);

    InitViewGroup();
    InitFromAttributes(context, attrs);
}

ViewGroup::ViewGroup(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : View(context, attrs, defStyle)
    , mGroupFlags(0)
    , mPersistentDrawingCache(0)
    , mChildAcceptsDrag(FALSE)
    , mLastTouchDownTime(0)
    , mLastTouchDownIndex(-1)
    , mLastTouchDownX(0.0f)
    , mLastTouchDownY(0.0f)
    , mHoveredSelf(FALSE)
    , mLayoutMode(CLIP_BOUNDS)
    , mLayoutSuppressed(FALSE)
    , mChildrenCount(0)
    , mChildCountWithTransientState(0)
{
    CTransformation::New((ITransformation**)&mChildTransformation);
    CPointF::New((IPointF**)&mLocalPoint);

    mLayoutTransitionListener = new LayoutTransitionListener(this);

    InitViewGroup();
    InitFromAttributes(context, attrs);
}

ViewGroup::~ViewGroup()
{
    if (mChildren != NULL) {
        for (Int32 i = 0; i < mChildren->GetLength(); ++i) {
            AutoPtr<IView> child = (*mChildren)[i];
            if (child != NULL) {
                View* v = reinterpret_cast<View*>(child->Probe(EIID_View));
                v->AssignParent(NULL);
                mChildren->Set(i, NULL);
            }
        }
    }

}

Boolean ViewGroup::DebugDraw()
{
    return mAttachInfo != NULL && mAttachInfo->mDebugLayout;
}

void ViewGroup::InitViewGroup()
{
    // ViewGroup doesn't draw by default
    SetFlags(WILL_NOT_DRAW, DRAW_MASK);
    mGroupFlags |= FLAG_CLIP_CHILDREN;
    mGroupFlags |= FLAG_CLIP_TO_PADDING;
    mGroupFlags |= FLAG_ANIMATION_DONE;
    mGroupFlags |= FLAG_ANIMATION_CACHE;
    mGroupFlags |= FLAG_ALWAYS_DRAWN_WITH_CACHE;

    AutoPtr<IApplicationInfo> appInfo;
    mContext->GetApplicationInfo((IApplicationInfo**)&appInfo);
    assert(appInfo != NULL);
    Int32 targetSdkVersion = 0;
    appInfo->GetTargetSdkVersion(&targetSdkVersion);
    if (targetSdkVersion >= Build::VERSION_CODES::HONEYCOMB) {
        mGroupFlags |= FLAG_SPLIT_MOTION_EVENTS;
    }

    SetDescendantFocusability(FOCUS_BEFORE_DESCENDANTS);
    mChildren = ArrayOf<IView*>::Alloc(ARRAY_INITIAL_CAPACITY);
    mChildrenCount = 0;

    mPersistentDrawingCache = PERSISTENT_SCROLLING_CACHE;
}

void ViewGroup::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::ViewGroup),
        ARRAY_SIZE(R::styleable::ViewGroup));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributes(
        attrs, attrIds, (ITypedArray**)&a));

    Int32 N;
    a->GetIndexCount(&N);
    for (Int32 i = 0; i < N; i++) {
        Int32 attr;
        a->GetIndex(i, &attr);
        switch (attr) {
            case R::styleable::ViewGroup_clipChildren:
            {
                Boolean value;
                a->GetBoolean(attr, TRUE, &value);
                SetClipChildren(value);
                break;
            }
            case R::styleable::ViewGroup_clipToPadding:
            {
                Boolean value;
                a->GetBoolean(attr, TRUE, &value);
                SetClipToPadding(value);
                break;
            }
            case R::styleable::ViewGroup_animationCache:
            {
                Boolean value;
                a->GetBoolean(attr, TRUE, &value);
                SetAnimationCacheEnabled(value);
                break;
            }
            case R::styleable::ViewGroup_persistentDrawingCache:
            {
                Int32 value;
                a->GetInt32(attr, PERSISTENT_SCROLLING_CACHE, &value);
                SetPersistentDrawingCache(value);
                break;
            }
            case R::styleable::ViewGroup_addStatesFromChildren:
            {
                Boolean value;
                a->GetBoolean(attr, FALSE, &value);
                SetAddStatesFromChildren(value);
                break;
            }
            case R::styleable::ViewGroup_alwaysDrawnWithCache:
            {
                Boolean value;
                a->GetBoolean(attr, TRUE, &value);
                SetAlwaysDrawnWithCacheEnabled(value);
                break;
            }
            case R::styleable::ViewGroup_layoutAnimation:
            {
                Int32 id;
                a->GetResourceId(attr, -1, &id);
                if (id > 0) {
                    AutoPtr<ILayoutAnimationController> controller;
                     AnimationUtils::LoadLayoutAnimation(
                         mContext, id, (ILayoutAnimationController**)&controller);
                    SetLayoutAnimation(controller);
                }
                break;
            }
            case R::styleable::ViewGroup_descendantFocusability:
            {
                Int32 value;
                a->GetInt32(attr, 0, &value);
                SetDescendantFocusability(DESCENDANT_FOCUSABILITY_FLAGS[value]);
                break;
            }
            case R::styleable::ViewGroup_splitMotionEvents: {
                Boolean value = FALSE;
                a->GetBoolean(attr, FALSE, &value);
                SetMotionEventSplittingEnabled(value);
                break;
            }
            case R::styleable::ViewGroup_animateLayoutChanges: {
                Boolean animateLayoutChanges = FALSE;
                a->GetBoolean(attr, FALSE, &animateLayoutChanges);
                if (animateLayoutChanges) {
                    AutoPtr<ILayoutTransition> transition;
                    CLayoutTransition::New((ILayoutTransition**)&transition);
                    SetLayoutTransition(transition);
                }
                break;
            }
            default:
                break;
        }
    }

    a->Recycle();
}

Int32 ViewGroup::GetDescendantFocusability()
{
    return mGroupFlags & FLAG_MASK_FOCUSABILITY;
}

ECode ViewGroup::SetDescendantFocusability(
    /* [in] */ Int32 focusability)
{
    switch (focusability) {
        case FOCUS_BEFORE_DESCENDANTS:
        case FOCUS_AFTER_DESCENDANTS:
        case FOCUS_BLOCK_DESCENDANTS:
            break;
        default:
//            throw new IllegalArgumentException("must be one of FOCUS_BEFORE_DESCENDANTS, "
//                    + "FOCUS_AFTER_DESCENDANTS, FOCUS_BLOCK_DESCENDANTS");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mGroupFlags &= ~FLAG_MASK_FOCUSABILITY;
    mGroupFlags |= (focusability & FLAG_MASK_FOCUSABILITY);
    return NOERROR;
}

ECode ViewGroup::HandleFocusGainInternal(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    if (mFocused != NULL) {
        VIEW_PROBE(mFocused)->UnFocus();
        mFocused = NULL;
    }

    return View::HandleFocusGainInternal(direction, previouslyFocusedRect);
}

ECode ViewGroup::RequestChildFocus(
    /* [in] */ IView* child,
    /* [in] */ IView* focused)
{
    if (DBG) {
        Logger::D(VG_TAG, /*this + */" requestChildFocus()");
    }

    if (GetDescendantFocusability() == FOCUS_BLOCK_DESCENDANTS) {
        return NOERROR;
    }

    // Unfocus us, if necessary
    View::UnFocus();

    // We had a previous notion of who had focus. Clear it.
    if (mFocused.Get() != child) {
        if (mFocused != NULL) {
            VIEW_PROBE(mFocused)->UnFocus();
        }

        mFocused = child;
    }

    if (mParent != NULL) {
        mParent->RequestChildFocus(IVIEW_PROBE(this), focused);
    }
    return NOERROR;
}

ECode ViewGroup::FocusableViewAvailable(
    /* [in] */ IView* v)
{
    if (mParent != NULL
        // shortcut: don't report a new focusable view if we block our descendants from
        // getting focus
        && (GetDescendantFocusability() != FOCUS_BLOCK_DESCENDANTS)
        // shortcut: don't report a new focusable view if we already are focused
        // (and we don't prefer our descendants)
        //
        // note: knowing that mFocused is non-NULL is not a good enough reason
        // to break the traversal since in that case we'd actually have to find
        // the focused view and make sure it wasn't FOCUS_AFTER_DESCENDANTS and
        // an ancestor of v; this will get checked for at ViewAncestor
        && !(IsFocused() && GetDescendantFocusability() != FOCUS_AFTER_DESCENDANTS)) {
        mParent->FocusableViewAvailable(v);
    }
    return NOERROR;
}

Boolean ViewGroup::ShowContextMenuForChild(
    /* [in] */ IView* originalView)
{
    Boolean result = FALSE;
    if (mParent != NULL) {
        mParent->ShowContextMenuForChild(originalView, &result);
    }

    return result;
}

AutoPtr<IActionMode> ViewGroup::StartActionModeForChild(
    /* [in] */ IView* originalView,
    /* [in] */ IActionModeCallback* callback)
{
    if (mParent != NULL) {
        AutoPtr<IActionMode> mode;
        mParent->StartActionModeForChild(originalView, callback, (IActionMode**)&mode);
        return mode;
    }

    return NULL;
}

AutoPtr<IView> ViewGroup::FocusSearch(
     /* [in] */ IView* focused,
      /* [in] */ Int32 direction)
{
    AutoPtr<IView> result;
    if (IsRootNamespace()) {
        // root namespace means we should consider ourselves the top of the
        // tree for focus searching; otherwise we could be focus searching
        // into other tabs.  see LocalActivityManager and TabHost for more info
        //
        FocusFinder::GetInstance()->FindNextFocus(
            THIS_PROBE(IViewGroup), focused, direction, (IView**)&result);
    }
    else if (mParent != NULL) {
        mParent->FocusSearch(focused, direction, (IView**)&result);
    }

    return result;
}

Boolean ViewGroup::RequestChildRectangleOnScreen(
    /* [in] */ IView* child,
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate)
{
    return FALSE;
}

Boolean ViewGroup::RequestSendAccessibilityEvent(
    /* [in] */ IView* child,
    /* [in] */ IAccessibilityEvent* event)
{
    AutoPtr<IViewParent> parent = mParent;
    if (parent == NULL) {
        return FALSE;
    }
    Boolean propagate = OnRequestSendAccessibilityEvent(child, event);
    if (!propagate) {
        return FALSE;
    }
    Boolean res;
    parent->RequestSendAccessibilityEvent(IVIEW_PROBE(this), event, &res);
    return res;
}

Boolean ViewGroup::OnRequestSendAccessibilityEvent(
    /* [in] */ IView* child,
    /* [in] */ IAccessibilityEvent* event)
{
    if (mAccessibilityDelegate != NULL) {
        Boolean res;
        mAccessibilityDelegate->OnRequestSendAccessibilityEvent(
                THIS_PROBE(IViewGroup), child, event, &res);
        return res;
    }
    else {
        return OnRequestSendAccessibilityEventInternal(child, event);
    }
}

Boolean ViewGroup::OnRequestSendAccessibilityEventInternal(
    /* [in] */ IView* child,
    /* [in] */ IAccessibilityEvent* event)
{
    return TRUE;
}

ECode ViewGroup::ChildHasTransientStateChanged(
    /* [in] */ IView* child,
    /* [in] */ Boolean childHasTransientState)
{
    Boolean oldHasTransientState = HasTransientState();
    if (childHasTransientState) {
        mChildCountWithTransientState++;
    }
    else {
        mChildCountWithTransientState--;
    }

    Boolean newHasTransientState = HasTransientState();
    if (mParent != NULL && oldHasTransientState != newHasTransientState) {
        return mParent->ChildHasTransientStateChanged(THIS_PROBE(IViewGroup), newHasTransientState);
    }

    return NOERROR;
}

Boolean ViewGroup::HasTransientState()
{
    return mChildCountWithTransientState > 0 || View::HasTransientState();
}

Boolean ViewGroup::DispatchUnhandledMove(
     /* [in] */ IView* focused,
     /* [in] */ Int32 direction)
{
    Boolean result = FALSE;
    if (mFocused != NULL) {
        mFocused->DispatchUnhandledMove(focused, direction, &result);
    }

    return result;
}

ECode ViewGroup::ClearChildFocus(
    /* [in] */ IView* child)
{
    if (DBG) {
        Logger::D(VG_TAG, /*this + */" clearChildFocus()");
    }

    mFocused = NULL;
    if (mParent != NULL) {
        mParent->ClearChildFocus(IVIEW_PROBE(this));
    }

    return NOERROR;
}

ECode ViewGroup::ClearFocus()
{
    if (mFocused == NULL) {
        View::ClearFocus();
    }
    else {
        AutoPtr<IView> focused = mFocused;
        mFocused = NULL;
        focused->ClearFocus();
    }

    return NOERROR;
}

ECode ViewGroup::UnFocus()
{
    if (mFocused == NULL) {
        View::UnFocus();
    }
    else {
        VIEW_PROBE(mFocused)->UnFocus();
        mFocused = NULL;
    }

    return NOERROR;
}

AutoPtr<IView> ViewGroup::GetFocusedChild()
{
    return mFocused;
}

Boolean ViewGroup::HasFocus()
{
    return (mPrivateFlags & PFLAG_FOCUSED) != 0 || mFocused != NULL;
}

AutoPtr<IView> ViewGroup::FindFocus()
{
    if (DBG) {
        //System.out.println("Find focus in " + this + ": flags="
        //        + isFocused() + ", child=" + mFocused);
    }

    if (IsFocused()) {
        return AutoPtr<IView>(IVIEW_PROBE(this));
    }

    if (mFocused != NULL) {
        return VIEW_PROBE(mFocused)->FindFocus();
    }
    return AutoPtr<IView>(NULL);
}

Boolean ViewGroup::HasFocusable()
{
    if ((mViewFlags & VISIBILITY_MASK) != IView::VISIBLE) {
        return FALSE;
    }

    if (IsFocusable()) {
        return TRUE;
    }

    if (GetDescendantFocusability() != FOCUS_BLOCK_DESCENDANTS) {
        for (Int32 i = 0; i < mChildrenCount; i++) {
            Boolean hasFocusable = FALSE;
            (*mChildren)[i]->HasFocusable(&hasFocusable);
            if (hasFocusable) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

ECode ViewGroup::AddFocusables(
    /* [in] */ IObjectContainer* views,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 focusableMode)
{
    assert(views);

    Int32 focusableCount;
    views->GetObjectCount(&focusableCount);

    Int32 descendantFocusability = GetDescendantFocusability();
    if (descendantFocusability != FOCUS_BLOCK_DESCENDANTS) {
        for (Int32 i = 0; i < mChildrenCount; i++) {
            View* temp = VIEW_PROBE((*mChildren)[i]);
            if ((temp->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE) {
                temp->AddFocusables(views, direction, focusableMode);
            }
        }
    }

    // we add ourselves (if focusable) in all cases except for when we are
    // FOCUS_AFTER_DESCENDANTS and there are some descendants focusable.  this is
    // to avoid the focus search finding layouts when a more precise search
    // among the focusable children would be more interesting.
    //
    Int32 size;
    views->GetObjectCount(&size);
    if (descendantFocusability != FOCUS_AFTER_DESCENDANTS ||
        // No focusable descendants
        (focusableCount == size)) {
            View::AddFocusables(views, direction, focusableMode);
    }

    return NOERROR;
}

ECode ViewGroup::FindViewsWithText(
    /* [in, out] */ IObjectContainer* outViews,
    /* [in] */ ICharSequence* searched,
    /* [in] */ Int32 flags)
{
    View::FindViewsWithText(outViews, searched, flags);
    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* child = VIEW_PROBE((*mChildren)[i]);
        if ((child->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE
                && (child->mPrivateFlags & PFLAG_IS_ROOT_NAMESPACE) == 0) {
            child->FindViewsWithText(outViews, searched, flags);
        }
    }

    return NOERROR;
}

AutoPtr<IView> ViewGroup::FindViewByAccessibilityIdTraversal(
    /* [in] */ Int32 accessibilityId)
{
    AutoPtr<IView> foundView = View::FindViewByAccessibilityIdTraversal(accessibilityId);
    if (foundView != NULL) {
        return foundView;
    }

    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* child = VIEW_PROBE((*mChildren)[i]);
        foundView = child->FindViewByAccessibilityIdTraversal(accessibilityId);
        if (foundView != NULL) {
            return foundView;
        }
    }
    return NULL;
}

ECode ViewGroup::DispatchWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    View::DispatchWindowFocusChanged(hasFocus);

    for (Int32 i = 0; i < mChildrenCount; i++) {
        (*mChildren)[i]->DispatchWindowFocusChanged(hasFocus);
    }

    return NOERROR;
}

ECode ViewGroup::AddTouchables(
    /* [in] */ IObjectContainer* views)
{
    View::AddTouchables(views);

    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* temp = VIEW_PROBE((*mChildren)[i]);
        if ((temp->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE) {
            temp->AddTouchables(views);
        }
    }

    return NOERROR;
}

ECode ViewGroup::MakeOptionalFitsSystemWindows()
{
    View::MakeOptionalFitsSystemWindows();
    for (Int32 i = 0; i < mChildrenCount; i++) {
        (*mChildren)[i]->MakeOptionalFitsSystemWindows();
    }

    return NOERROR;
}

ECode ViewGroup::DispatchDisplayHint(
    /* [in] */ Int32 hint)
{
    View::DispatchDisplayHint(hint);

    for (Int32 i = 0; i < mChildrenCount; i++) {
        (*mChildren)[i]->DispatchDisplayHint(hint);
    }

    return NOERROR;
}

void ViewGroup::OnChildVisibilityChanged(
    /* [in] */ IView* child,
    /* [in] */ Int32 oldVisibility,
    /* [in] */ Int32 newVisibility)
{
    if (mTransition != NULL) {
        if (newVisibility == IView::VISIBLE) {
            mTransition->ShowChild(THIS_PROBE(IViewGroup), child, oldVisibility);
        }
        else {
            mTransition->HideChild(THIS_PROBE(IViewGroup), child, newVisibility);
            if (!mTransitioningViews.IsEmpty()) {
                AutoPtr<IView> temp = child;
                List<AutoPtr<IView> >::Iterator iter =
                    Find(mTransitioningViews.Begin(), mTransitioningViews.End(), temp);
                if (iter != mTransitioningViews.End()) {
                    // Only track this on disappearing views - appearing views are already visible
                    // and don't need special handling during drawChild()
                    mVisibilityChangingChildren.PushBack(child);
                    AddDisappearingView(child);
                }
            }
        }
    }

    // in all cases, for drags
    if (mCurrentDrag != NULL) {
        if (newVisibility == IView::VISIBLE) {
            NotifyChildOfDrag(child);
        }
    }
}

void ViewGroup::DispatchVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    View::DispatchVisibilityChanged(changedView, visibility);

    for (Int32 i = 0; i < mChildrenCount; i++) {
        VIEW_PROBE((*mChildren)[i])->DispatchVisibilityChanged(
            changedView, visibility);
    }
}

ECode ViewGroup::DispatchWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    View::DispatchWindowVisibilityChanged(visibility);

    for (Int32 i = 0; i < mChildrenCount; i++) {
        (*mChildren)[i]->DispatchWindowVisibilityChanged(visibility);
    }

    return NOERROR;
}

ECode ViewGroup::DispatchConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    View::DispatchConfigurationChanged(newConfig);

    for (Int32 i = 0; i < mChildrenCount; i++) {
        (*mChildren)[i]->DispatchConfigurationChanged(newConfig);
    }

    return NOERROR;
}

ECode ViewGroup::RecomputeViewAttributes(
    /* [in] */ IView* child)
{
    if (mAttachInfo != NULL && !mAttachInfo->mRecomputeGlobalAttributes) {
        if (mParent != NULL) {
            return mParent->RecomputeViewAttributes(IVIEW_PROBE(this));
        }
    }

    return NOERROR;
}

ECode ViewGroup::DispatchCollectViewAttributes(
    /* [in] */ AttachInfo* attachInfo,
    /* [in] */ Int32 visibility)
{
    if ((visibility & VISIBILITY_MASK) == IView::VISIBLE) {
        View::DispatchCollectViewAttributes(attachInfo, visibility);
        for (Int32 i = 0; i < mChildrenCount; i++) {
            View* child = VIEW_PROBE((*mChildren)[i]);
            child->DispatchCollectViewAttributes(attachInfo,
                    visibility | (child->mViewFlags & VISIBILITY_MASK));
        }
    }

    return NOERROR;
}

ECode ViewGroup::BringChildToFront(
    /* [in] */ IView* child)
{
    Int32 index = IndexOfChild(child);
    if (index >= 0) {
        RemoveFromArray(index);
        AddInArray(child, mChildrenCount);
        VIEW_PROBE(child)->mParent =
            THIS_PROBE(IViewParent);
    }
    return NOERROR;
}


Boolean ViewGroup::DispatchDragEvent(
    /* [in] */ IDragEvent* event)
{
    Boolean retval = FALSE;
    Float tx = 0.0f;
    event->GetX(&tx);
    Float ty = 0.0f;
    event->GetY(&ty);

    ViewRootImpl* root = GetViewRootImpl();

    Int32 action = 0;
    event->GetAction(&action);
    // Dispatch down the view hierarchy
    switch (action) {
    case IDragEvent::ACTION_DRAG_STARTED: {
        // clear state to recalculate which views we drag over
        mCurrentDragView = NULL;

        // Set up our tracking of drag-started notifications
        //mCurrentDrag = CDragEvent::Obtain(event);
        mDragNotifiedChildren.Clear();

        // Now dispatch down to our children, caching the responses
        mChildAcceptsDrag = FALSE;
        for (Int32 i = 0; i < mChildrenCount; i++) {
            View* child = VIEW_PROBE((*mChildren)[i]);
            child->mPrivateFlags2 &= ~DRAG_MASK;
            if (child->GetVisibility() == IView::VISIBLE) {
                Boolean handled = NotifyChildOfDrag((*mChildren)[i]);
                if (handled) {
                    mChildAcceptsDrag = TRUE;
                }
            }
        }

        // Return HANDLED if one of our children can accept the drag
        if (mChildAcceptsDrag) {
            retval = TRUE;
        }
    } break;

    case IDragEvent::ACTION_DRAG_ENDED: {
        // Release the bookkeeping now that the drag lifecycle has ended
        if (!mDragNotifiedChildren.IsEmpty()) {
            List<AutoPtr<IView> >::Iterator iter = mDragNotifiedChildren.Begin();
            for (; iter != mDragNotifiedChildren.End(); ++iter) {
                View* child = VIEW_PROBE(iter->Get());
                assert(child != NULL);
                // If a child was notified about an ongoing drag, it's told that it's over
                child->DispatchDragEvent(event);
                child->mPrivateFlags2 &= ~DRAG_MASK;
                child->RefreshDrawableState();
            }

            mDragNotifiedChildren.Clear();
            mCurrentDrag->Recycle();
            mCurrentDrag = NULL;
        }

        // We consider drag-ended to have been handled if one of our children
        // had offered to handle the drag.
        if (mChildAcceptsDrag) {
            retval = TRUE;
        }
    } break;

    case IDragEvent::ACTION_DRAG_LOCATION: {
        // Find the [possibly new] drag target
        View* target = VIEW_PROBE(FindFrontmostDroppableChildAt(tx, ty, mLocalPoint));

        // If we've changed apparent drag target, tell the view root which view
        // we're over now [for purposes of the eventual drag-recipient-changed
        // notifications to the framework] and tell the new target that the drag
        // has entered its bounds.  The root will see setDragFocus() calls all
        // the way down to the final leaf view that is handling the LOCATION event
        // before reporting the new potential recipient to the framework.
        if (mCurrentDragView != target) {
            root->SetDragFocus(IVIEW_PROBE(target));

            // If we've dragged off of a child view, send it the EXITED message
            if (mCurrentDragView != NULL) {
                View* view = VIEW_PROBE(mCurrentDragView);
                event->SetAction(IDragEvent::ACTION_DRAG_EXITED);
                view->DispatchDragEvent(event);
                view->mPrivateFlags2 &= ~PFLAG2_DRAG_HOVERED;
                view->RefreshDrawableState();
            }
            mCurrentDragView = target;

            // If we've dragged over a new child view, send it the ENTERED message
            if (target != NULL) {
                event->SetAction(IDragEvent::ACTION_DRAG_ENTERED);
                target->DispatchDragEvent(event);
                target->mPrivateFlags2 |= PFLAG2_DRAG_HOVERED;
                target->RefreshDrawableState();
            }
            event->SetAction(action);  // restore the event's original state
        }

        // Dispatch the actual drag location notice, localized into its coordinates
        if (target != NULL) {
            Float tmp = 0.0f;
            mLocalPoint->GetX(&tmp);
            event->SetX(tmp);
            mLocalPoint->GetY(&tmp);
            event->SetY(tmp);

            retval = target->DispatchDragEvent(event);

            event->SetX(tx);
            event->SetY(ty);
        }
    } break;

    /* Entered / exited dispatch
     *
     * DRAG_ENTERED is not dispatched downwards from ViewGroup.  The reason for this is
     * that we're about to get the corresponding LOCATION event, which we will use to
     * determine which of our children is the new target; at that point we will
     * push a DRAG_ENTERED down to the new target child [which may itself be a ViewGroup].
     *
     * DRAG_EXITED *is* dispatched all the way down immediately: once we know the
     * drag has left this ViewGroup, we know by definition that every contained subview
     * is also no longer under the drag point.
     */

    case IDragEvent::ACTION_DRAG_EXITED: {
        if (mCurrentDragView != NULL) {
            mCurrentDragView->DispatchDragEvent(event);
            mCurrentDragView->mPrivateFlags2 &= ~PFLAG2_DRAG_HOVERED;
            mCurrentDragView->RefreshDrawableState();

            mCurrentDragView = NULL;
        }
    } break;

    case IDragEvent::ACTION_DROP: {
        // if (ViewDebug.DEBUG_DRAG) Log.d(View::VIEW_LOG_TAG, "Drop event: " + event);
        View* target = VIEW_PROBE(FindFrontmostDroppableChildAt(tx, ty, mLocalPoint));
        if (target != NULL) {
            Float tmp = 0.0f;
            mLocalPoint->GetX(&tmp);
            event->SetX(tmp);
            mLocalPoint->GetY(&tmp);
            event->SetY(tmp);

            retval = target->DispatchDragEvent(event);

            event->SetX(tx);
            event->SetY(ty);

        }
        else {
            // if (ViewDebug.DEBUG_DRAG) {
            //     Log.d(View::VIEW_LOG_TAG, "   not dropped on an accepting view");
            // }
        }
    } break;
    }

    // If none of our children could handle the event, try here
    if (!retval) {
        // Call up to the View implementation that dispatches to installed listeners
        retval = View::DispatchDragEvent(event);
    }

    return retval;
}

AutoPtr<IView> ViewGroup::FindFrontmostDroppableChildAt(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPointF* outLocalPoint)
{
    for (Int32 i = mChildrenCount - 1; i >= 0; i--) {
        AutoPtr<IView> child = (*mChildren)[i];
        if (!VIEW_PROBE(child)->CanAcceptDrag()) {
            continue;
        }

        if (IsTransformedTouchPointInView(x, y, child, outLocalPoint)) {
            return child;
        }
    }
    return NULL;
}

Boolean ViewGroup::NotifyChildOfDrag(
    /* [in] */ IView* child)
{
    // if (ViewDebug.DEBUG_DRAG) {
    //     Log.d(View::VIEW_LOG_TAG, "Sending drag-started to view: " + child);
    // }

    assert(child != NULL);
    Boolean canAccept = FALSE;
    AutoPtr<IView> temp = child;
    List<AutoPtr<IView> >::Iterator iter =
        Find(mDragNotifiedChildren.Begin(), mDragNotifiedChildren.End(), temp);

    if (iter == mDragNotifiedChildren.End()) {
        mDragNotifiedChildren.PushBack(child);
        View* _child = VIEW_PROBE(child);
        canAccept = _child->DispatchDragEvent(mCurrentDrag);
        if (canAccept && !_child->CanAcceptDrag()) {
            _child->mPrivateFlags2 |= PFLAG2_DRAG_CAN_ACCEPT;
            _child->RefreshDrawableState();
        }
    }

    return canAccept;
}

ECode ViewGroup::DispatchWindowSystemUiVisiblityChanged(
    /* [in] */ Int32 visible)
{
    View::DispatchWindowSystemUiVisiblityChanged(visible);

    for (Int32 i = 0; i < mChildrenCount; i++) {
        (*mChildren)[i]->DispatchWindowSystemUiVisiblityChanged(visible);
    }

    return NOERROR;
}

ECode ViewGroup::DispatchSystemUiVisibilityChanged(
    /* [in] */ Int32 visible)
{
    View::DispatchSystemUiVisibilityChanged(visible);

    for (Int32 i=0; i <mChildrenCount; i++) {
        (*mChildren)[i]->DispatchSystemUiVisibilityChanged(visible);
    }

    return NOERROR;
}

Boolean ViewGroup::UpdateLocalSystemUiVisibility(
    /* [in] */ Int32 localValue,
    /* [in] */ Int32 localChanges)
{
    Boolean changed = View::UpdateLocalSystemUiVisibility(localValue, localChanges);

    for (Int32 i=0; i <mChildrenCount; i++) {
        View* child = VIEW_PROBE((*mChildren)[i]);
        changed |= child->UpdateLocalSystemUiVisibility(localValue, localChanges);
    }

    return changed;
}

Boolean ViewGroup::DispatchKeyEventPreIme(
    /* [in] */ IKeyEvent* event)
{
    if ((mPrivateFlags & (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS)) == (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS)) {
        return View::DispatchKeyEventPreIme(event);
    }
    else if (mFocused != NULL) {
        View* focused = VIEW_PROBE(mFocused);
        assert(focused);
        if ((focused->mPrivateFlags & PFLAG_HAS_BOUNDS) == PFLAG_HAS_BOUNDS) {
            return focused->DispatchKeyEventPreIme(event);
        }
    }

    return FALSE;
}

Boolean ViewGroup::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnKeyEvent(event, 1);
    }

    if ((mPrivateFlags & (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS))
            == (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS)) {
        if (View::DispatchKeyEvent(event)) {
            return TRUE;
        }
    }
    else if (mFocused != NULL && (VIEW_PROBE(mFocused)->mPrivateFlags & PFLAG_HAS_BOUNDS)
            == PFLAG_HAS_BOUNDS) {
        if (VIEW_PROBE(mFocused)->DispatchKeyEvent(event)) {
            return TRUE;
        }
    }

    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnUnhandledEvent(event, 1);
    }

    return FALSE;
}

Boolean ViewGroup::DispatchKeyShortcutEvent(
    /* [in] */ IKeyEvent* event)
{
    if ((mPrivateFlags & (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS)) == (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS)) {
        return View::DispatchKeyShortcutEvent(event);
    }
    else if (mFocused != NULL) {
        View* focused = VIEW_PROBE(mFocused);
        assert(focused);
        if ((focused->mPrivateFlags & PFLAG_HAS_BOUNDS) == PFLAG_HAS_BOUNDS) {
            return focused->DispatchKeyShortcutEvent(event);
        }
    }

    return FALSE;
}

Boolean ViewGroup::DispatchTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnTrackballEvent(event, 1);
    }

    if ((mPrivateFlags & (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS))
            == (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS)) {
        if (View::DispatchTrackballEvent(event)) {
            return TRUE;
        }
    }
    else if (mFocused != NULL && (VIEW_PROBE(mFocused)->mPrivateFlags & PFLAG_HAS_BOUNDS)
            == PFLAG_HAS_BOUNDS) {
        if (VIEW_PROBE(mFocused)->DispatchTrackballEvent(event)) {
            return TRUE;
        }
    }

    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnUnhandledEvent(event, 1);
    }

    return FALSE;
}

Boolean ViewGroup::DispatchHoverEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 action = 0;
    event->GetAction(&action);

    // First check whether the view group wants to intercept the hover event.
    Boolean interceptHover = OnInterceptHoverEvent(event);
    event->SetAction(action); // restore action in case it was changed

    AutoPtr<IMotionEvent> eventNoHistory = event;
    Boolean handled = FALSE;

    // Send events to the hovered children and build a new list of hover targets until
    // one is found that handles the event.
    AutoPtr<HoverTarget> firstOldHoverTarget = mFirstHoverTarget;
    mFirstHoverTarget = NULL;
    if (!interceptHover && action != IMotionEvent::ACTION_HOVER_EXIT) {
        Float x = 0.0f;
        event->GetX(&x);
        Float y = 0.0f;
        event->GetY(&y);

        if (mChildrenCount != 0) {
            AutoPtr<HoverTarget> lastHoverTarget;
            for (Int32 i = mChildrenCount - 1; i >= 0; i--) {
                AutoPtr<IView> child = (*mChildren)[i];
                if (!CanViewReceivePointerEvents(child)
                        || !IsTransformedTouchPointInView(x, y, child, NULL)) {
                    continue;
                }

                // Obtain a hover target for this child.  Dequeue it from the
                // old hover target list if the child was previously hovered.
                AutoPtr<HoverTarget> hoverTarget = firstOldHoverTarget;
                Boolean wasHovered = FALSE;
                for (AutoPtr<HoverTarget> predecessor; ;) {
                    if (hoverTarget == NULL) {
                        hoverTarget = HoverTarget::Obtain(child);
                        wasHovered = FALSE;
                        break;
                    }

                    if (hoverTarget->mChild == child) {
                        if (predecessor != NULL) {
                            predecessor->mNext = hoverTarget->mNext;
                        }
                        else {
                            firstOldHoverTarget = hoverTarget->mNext;
                        }
                        hoverTarget->mNext = NULL;
                        wasHovered = TRUE;
                        break;
                    }

                    predecessor = hoverTarget;
                    hoverTarget = hoverTarget->mNext;
                }

                // Enqueue the hover target onto the new hover target list.
                if (lastHoverTarget != NULL) {
                    lastHoverTarget->mNext = hoverTarget;
                }
                else {
                    lastHoverTarget = hoverTarget;
                    mFirstHoverTarget = hoverTarget;
                }

                // Dispatch the event to the child.
                if (action == IMotionEvent::ACTION_HOVER_ENTER) {
                    if (!wasHovered) {
                        // Send the enter as is.
                        handled |= DispatchTransformedGenericPointerEvent(
                                event, child); // enter
                    }
                }
                else if (action == IMotionEvent::ACTION_HOVER_MOVE) {
                    if (!wasHovered) {
                        // Synthesize an enter from a move.
                        eventNoHistory = ObtainMotionEventNoHistoryOrSelf(eventNoHistory);
                        eventNoHistory->SetAction(IMotionEvent::ACTION_HOVER_ENTER);
                        handled |= DispatchTransformedGenericPointerEvent(
                                eventNoHistory, child); // enter
                        eventNoHistory->SetAction(action);

                        handled |= DispatchTransformedGenericPointerEvent(
                                eventNoHistory, child); // move
                    }
                    else {
                        // Send the move as is.
                        handled |= DispatchTransformedGenericPointerEvent(event, child);
                    }
                }
                if (handled) {
                    break;
                }
            }
        }
    }

    // Send exit events to all previously hovered children that are no longer hovered.
    while (firstOldHoverTarget != NULL) {
        AutoPtr<IView> child = firstOldHoverTarget->mChild;

        // Exit the old hovered child.
        if (action == IMotionEvent::ACTION_HOVER_EXIT) {
            // Send the exit as is.
            handled |= DispatchTransformedGenericPointerEvent(
                    event, child); // exit
        }
        else {
            // Synthesize an exit from a move or enter.
            // Ignore the result because hover focus has moved to a different view.
            if (action == IMotionEvent::ACTION_HOVER_MOVE) {
                DispatchTransformedGenericPointerEvent(
                        event, child); // move
            }
            eventNoHistory = ObtainMotionEventNoHistoryOrSelf(eventNoHistory);
            eventNoHistory->SetAction(IMotionEvent::ACTION_HOVER_EXIT);
            DispatchTransformedGenericPointerEvent(
                    eventNoHistory, child); // exit
            eventNoHistory->SetAction(action);
        }

        AutoPtr<HoverTarget> nextOldHoverTarget = firstOldHoverTarget->mNext;
        firstOldHoverTarget->Recycle();
        firstOldHoverTarget = nextOldHoverTarget;
    }

    // Send events to the view group itself if no children have handled it.
    Boolean newHoveredSelf = !handled;
    if (newHoveredSelf == mHoveredSelf) {
        if (newHoveredSelf) {
            // Send event to the view group as before.
            handled |= View::DispatchHoverEvent(event);
        }
    }
    else {
        if (mHoveredSelf) {
            // Exit the view group.
            if (action == IMotionEvent::ACTION_HOVER_EXIT) {
                // Send the exit as is.
                handled |= View::DispatchHoverEvent(event); // exit
            }
            else {
                // Synthesize an exit from a move or enter.
                // Ignore the result because hover focus is moving to a different view.
                if (action == IMotionEvent::ACTION_HOVER_MOVE) {
                    View::DispatchHoverEvent(event); // move
                }
                eventNoHistory = ObtainMotionEventNoHistoryOrSelf(eventNoHistory);
                eventNoHistory->SetAction(IMotionEvent::ACTION_HOVER_EXIT);
                View::DispatchHoverEvent(eventNoHistory); // exit
                eventNoHistory->SetAction(action);
            }
            mHoveredSelf = FALSE;
        }

        if (newHoveredSelf) {
            // Enter the view group.
            if (action == IMotionEvent::ACTION_HOVER_ENTER) {
                // Send the enter as is.
                handled |= View::DispatchHoverEvent(event); // enter
                mHoveredSelf = TRUE;
            }
            else if (action == IMotionEvent::ACTION_HOVER_MOVE) {
                // Synthesize an enter from a move.
                eventNoHistory = ObtainMotionEventNoHistoryOrSelf(eventNoHistory);
                eventNoHistory->SetAction(IMotionEvent::ACTION_HOVER_ENTER);
                handled |= View::DispatchHoverEvent(eventNoHistory); // enter
                eventNoHistory->SetAction(action);

                handled |= View::DispatchHoverEvent(eventNoHistory); // move
                mHoveredSelf = TRUE;
            }
        }
    }

    // Recycle the copy of the event that we made.
    if (eventNoHistory.Get() != event) {
        eventNoHistory->Recycle();
    }

    // Done.
    return handled;
}

void ViewGroup::ExitHoverTargets()
{
    if (mHoveredSelf || mFirstHoverTarget != NULL) {
        Int64 now = SystemClock::GetUptimeMillis();
        AutoPtr<CMotionEvent> event;
        CMotionEvent::Obtain(
            now, now, IMotionEvent::ACTION_HOVER_EXIT,
            0.0f, 0.0f, 0, (CMotionEvent**)&event);
        event->SetSource(IInputDevice::SOURCE_TOUCHSCREEN);
        DispatchHoverEvent(event.Get());
        event->Recycle();
    }
}

void ViewGroup::CancelHoverTarget(
    /* [in] */ IView* view)
{
    AutoPtr<HoverTarget> predecessor;
    AutoPtr<HoverTarget> target = mFirstHoverTarget;
    while (target != NULL) {
        AutoPtr<HoverTarget> next = target->mNext;
        if (target->mChild.Get() == view) {
            if (predecessor == NULL) {
                mFirstHoverTarget = next;
            }
            else {
                predecessor->mNext = next;
            }

            target->Recycle();

            Int64 now = SystemClock::GetUptimeMillis();

            AutoPtr<CMotionEvent> event;
            CMotionEvent::Obtain(
                now, now, IMotionEvent::ACTION_HOVER_EXIT,
                0.0f, 0.0f, 0, (CMotionEvent**)&event);
            event->SetSource(IInputDevice::SOURCE_TOUCHSCREEN);
            VIEW_PROBE(view)->DispatchHoverEvent(event.Get());
            event->Recycle();
            return;
        }

        predecessor = target;
        target = next;
    }
}

Boolean ViewGroup::HasHoveredChild()
{
    return mFirstHoverTarget != NULL;
}

ECode ViewGroup::AddChildrenForAccessibility(
    /* [in] */ IObjectContainer* childrenForAccessibility)
{
    AutoPtr<ChildListForAccessibility> children =
        ChildListForAccessibility::Obtain(THIS_PROBE(IViewGroup), TRUE);
    const Int32 childrenCount = children->GetChildCount();
    for (Int32 i = 0; i < childrenCount; i++) {
        View* child = VIEW_PROBE(children->GetChildAt(i));
        if ((child->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE) {
            if (child->IncludeForAccessibility()) {
                childrenForAccessibility->Add(IVIEW_PROBE(child));
            }
            else {
                child->AddChildrenForAccessibility(childrenForAccessibility);
            }
        }
    }
    children->Recycle();

    return NOERROR;
}

ECode ViewGroup::ChildAccessibilityStateChanged(
    /* [in] */ IView* child)
{
    if (mParent != NULL) {
        return mParent->ChildAccessibilityStateChanged(child);
    }

    return NOERROR;
}

Boolean ViewGroup::OnInterceptHoverEvent(
    /* [in] */ IMotionEvent* event)
{
    return FALSE;
}

AutoPtr<IMotionEvent> ViewGroup::ObtainMotionEventNoHistoryOrSelf(
    /* [in] */ IMotionEvent* event)
{
    Int32 size = 0;
    event->GetHistorySize(&size);
    if (size == 0) {
        return event;
    }

    AutoPtr<CMotionEvent> motionEvent;
    CMotionEvent::ObtainNoHistory((CMotionEvent*)event, (CMotionEvent**)&motionEvent);

    return motionEvent;
}

Boolean ViewGroup::DispatchGenericPointerEvent(
    /* [in] */ IMotionEvent* event)
{
    // Send the event to the child under the pointer.
    if (mChildrenCount != 0) {
        Float x = 0.0f;
        event->GetX(&x);
        Float y = 0.0f;
        event->GetY(&y);

        const Boolean customOrder = IsChildrenDrawingOrderEnabled();
        for (Int32 i = mChildrenCount - 1; i >= 0; i--) {
            Int32 childIndex = customOrder ? GetChildDrawingOrder(mChildrenCount, i) : i;
            AutoPtr<IView> child = (*mChildren)[childIndex];
            if (!CanViewReceivePointerEvents(child)
                    || !IsTransformedTouchPointInView(x, y, child, NULL)) {
                continue;
            }

            if (DispatchTransformedGenericPointerEvent(event, child)) {
                return TRUE;
            }
        }
    }

    // No child handled the event.  Send it to this view group.
    return View::DispatchGenericPointerEvent(event);
}

Boolean ViewGroup::DispatchGenericFocusedEvent(
    /* [in] */ IMotionEvent* event)
{
    // Send the event to the focused child or to this view group if it has focus.
    if ((mPrivateFlags & (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS))
            == (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS)) {
        return View::DispatchGenericFocusedEvent(event);
    }
    else if (mFocused != NULL && (VIEW_PROBE(mFocused)->mPrivateFlags & PFLAG_HAS_BOUNDS)
            == PFLAG_HAS_BOUNDS) {
        return VIEW_PROBE(mFocused)->DispatchGenericMotionEvent(event);
    }
    return FALSE;
}

Boolean ViewGroup::DispatchTransformedGenericPointerEvent(
    /* [in] */ IMotionEvent* event,
    /* [in] */ IView* _child)
{
    View* child = VIEW_PROBE(_child);
    Float offsetX = mScrollX - child->mLeft;
    Float offsetY = mScrollY - child->mTop;

    Boolean handled = FALSE;
    if (!child->HasIdentityMatrix()) {
        AutoPtr<CMotionEvent> transformedEvent;
        CMotionEvent::Obtain((CMotionEvent*)event, (CMotionEvent**)&transformedEvent);
        transformedEvent->OffsetLocation(offsetX, offsetY);

        AutoPtr<IMatrix> matrix = child->GetInverseMatrix();
        transformedEvent->Transform(matrix);
        handled = child->DispatchGenericMotionEvent(transformedEvent);
        transformedEvent->Recycle();
    }
    else {
        event->OffsetLocation(offsetX, offsetY);
        handled = child->DispatchGenericMotionEvent(event);
        event->OffsetLocation(-offsetX, -offsetY);
    }

    return handled;
}

Boolean ViewGroup::DispatchTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnTouchEvent(ev, 1);
    }

    Boolean handled = FALSE;
    if (OnFilterTouchEventForSecurity(ev)) {
        Int32 action = 0;
        ev->GetAction(&action);
        Int32 actionMasked = action & IMotionEvent::ACTION_MASK;

        // Handle an initial down.
        if (actionMasked == IMotionEvent::ACTION_DOWN) {
            // Throw away all previous state when starting a new touch gesture.
            // The framework may have dropped the up or cancel event for the previous gesture
            // due to an app switch, ANR, or some other state change.
            CancelAndClearTouchTargets(ev);
            ResetTouchState();
        }

        // Check for interception.
        Boolean intercepted = FALSE;
        if (actionMasked == IMotionEvent::ACTION_DOWN
                || mFirstTouchTarget != NULL) {
            Boolean disallowIntercept = (mGroupFlags & FLAG_DISALLOW_INTERCEPT) != 0;
            if (!disallowIntercept) {
                intercepted = OnInterceptTouchEvent(ev);
                ev->SetAction(action); // restore action in case it was changed
            }
            else {
                intercepted = FALSE;
            }
        }
        else {
            // There are no touch targets and this action is not an initial down
            // so this view group continues to intercept touches.
            intercepted = TRUE;
        }

        // Check for cancelation.
        Boolean canceled = ResetCancelNextUpFlag(this)
                || actionMasked == IMotionEvent::ACTION_CANCEL;

        // Update list of touch targets for pointer down, if needed.
        Boolean split = (mGroupFlags & FLAG_SPLIT_MOTION_EVENTS) != 0;
        AutoPtr<TouchTarget> newTouchTarget;
        Boolean alreadyDispatchedToNewTouchTarget = FALSE;
        if (!canceled && !intercepted) {
            if (actionMasked == IMotionEvent::ACTION_DOWN
                    || (split && actionMasked == IMotionEvent::ACTION_POINTER_DOWN)
                    || actionMasked == IMotionEvent::ACTION_HOVER_MOVE) {
                Int32 actionIndex = 0;
                ev->GetActionIndex(&actionIndex); // always 0 for down

                Int32 idBitsToAssign = TouchTarget::ALL_POINTER_IDS;

                if (split) {
                    Int32 tmp = 0;
                    ev->GetPointerId(actionIndex, &tmp);
                    idBitsToAssign = 1 << tmp;
                }

                // Clean up earlier touch targets for this pointer id in case they
                // have become out of sync.
                RemovePointersFromTouchTargets(idBitsToAssign);

                const Int32 childrenCount = mChildrenCount;
                if (childrenCount != 0) {
                    // Find a child that can receive the event.
                    // Scan children from front to back.
                    Float x = 0.0f;
                    ev->GetX(actionIndex, &x);
                    Float y = 0.0f;
                    ev->GetY(actionIndex, &y);

                    Boolean customOrder = IsChildrenDrawingOrderEnabled();
                    for (Int32 i = childrenCount - 1; i >= 0; i--) {
                        Int32 childIndex = customOrder ?
                                GetChildDrawingOrder(childrenCount, i) : i;
                        AutoPtr<IView> child = (*mChildren)[childIndex];
                        if (!CanViewReceivePointerEvents(child)
                                || !IsTransformedTouchPointInView(x, y, child, NULL)) {
                            continue;
                        }

                        newTouchTarget = GetTouchTarget(child);
                        if (newTouchTarget != NULL) {
                            // Child is already receiving touch within its bounds.
                            // Give it the new pointer in addition to the ones it is handling.
                            newTouchTarget->mPointerIdBits |= idBitsToAssign;
                            break;
                        }

                        ResetCancelNextUpFlag(VIEW_PROBE(child));
                        if (DispatchTransformedTouchEvent(ev, FALSE, child, idBitsToAssign)) {
                            // Child wants to receive touch within its bounds.
                            ev->GetDownTime(&mLastTouchDownTime);
                            mLastTouchDownIndex = childIndex;
                            ev->GetX(&mLastTouchDownX);
                            ev->GetY(&mLastTouchDownY);
                            newTouchTarget = AddTouchTarget(child, idBitsToAssign);
                            alreadyDispatchedToNewTouchTarget = TRUE;
                            break;
                        }
                    }
                }

                if (newTouchTarget == NULL && mFirstTouchTarget != NULL) {
                    // Did not find a child to receive the event.
                    // Assign the pointer to the least recently added target.
                    newTouchTarget = mFirstTouchTarget;
                    while (newTouchTarget->mNext != NULL) {
                        newTouchTarget = newTouchTarget->mNext;
                    }
                    newTouchTarget->mPointerIdBits |= idBitsToAssign;
                }
            }
        }

        // Dispatch to touch targets.
        if (mFirstTouchTarget == NULL) {
            // No touch targets so treat this as an ordinary view.
            handled = DispatchTransformedTouchEvent(ev, canceled, NULL,
                    TouchTarget::ALL_POINTER_IDS);
        }
        else {
            // Dispatch to touch targets, excluding the new touch target if we already
            // dispatched to it.  Cancel touch targets if necessary.
            AutoPtr<TouchTarget> predecessor;
            AutoPtr<TouchTarget> target = mFirstTouchTarget;
            while (target != NULL) {
                AutoPtr<TouchTarget> next = target->mNext;
                if (alreadyDispatchedToNewTouchTarget && target == newTouchTarget) {
                    handled = TRUE;
                }
                else {
                    Boolean cancelChild = ResetCancelNextUpFlag(VIEW_PROBE(target->mChild))
                        || intercepted;
                    if (DispatchTransformedTouchEvent(ev, cancelChild,
                            target->mChild, target->mPointerIdBits)) {
                        handled = TRUE;
                    }
                    if (cancelChild) {
                        if (predecessor == NULL) {
                            mFirstTouchTarget = next;
                        }
                        else {
                            predecessor->mNext = next;
                        }
                        target->Recycle();
                        target = next;
                        continue;
                    }
                }
                predecessor = target;
                target = next;
            }
        }

        // Update list of touch targets for pointer up or cancel, if needed.
        if (canceled
                || actionMasked == IMotionEvent::ACTION_UP
                || actionMasked == IMotionEvent::ACTION_HOVER_MOVE) {
            ResetTouchState();
        }
        else if (split && actionMasked == IMotionEvent::ACTION_POINTER_UP) {
            Int32 actionIndex = 0;
            ev->GetActionIndex(&actionIndex);


            Int32 idBitsToRemove = 0, tmp = 0;
            ev->GetPointerId(actionIndex, &tmp);
            idBitsToRemove = 1 << tmp;
            RemovePointersFromTouchTargets(idBitsToRemove);
        }
    }

    if (!handled && mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnUnhandledEvent(ev, 1);
    }

    return handled;
}

void ViewGroup::ResetTouchState()
{
    ClearTouchTargets();
    ResetCancelNextUpFlag(this);
    mGroupFlags &= ~FLAG_DISALLOW_INTERCEPT;
}

Boolean ViewGroup::ResetCancelNextUpFlag(
    /* [in] */ View* view)
{
    if ((view->mPrivateFlags & PFLAG_CANCEL_NEXT_UP_EVENT) != 0) {
        view->mPrivateFlags &= ~PFLAG_CANCEL_NEXT_UP_EVENT;
        return TRUE;
    }
    return FALSE;
}

void ViewGroup::ClearTouchTargets()
{
    AutoPtr<TouchTarget> target = mFirstTouchTarget;
    if (target != NULL) {
        do {
            AutoPtr<TouchTarget> next = target->mNext;
            target->Recycle();
            target = next;
        } while (target != NULL);
        mFirstTouchTarget = NULL;
    }
}

void ViewGroup::CancelAndClearTouchTargets(
    /* [in] */IMotionEvent* event)
{
    if (mFirstTouchTarget != NULL) {
        Boolean syntheticEvent = FALSE;
        AutoPtr<IMotionEvent> temp = event;
        if (temp == NULL) {
            Int64 now = SystemClock::GetUptimeMillis();
            CMotionEvent::Obtain(
                now, now, IMotionEvent::ACTION_CANCEL,
                0.0f, 0.0f, 0, (CMotionEvent**)&temp);
            temp->SetSource(IInputDevice::SOURCE_TOUCHSCREEN);
            syntheticEvent = TRUE;
        }

        AutoPtr<TouchTarget> target = mFirstTouchTarget;
        for (; target != NULL; target = target->mNext) {
            ResetCancelNextUpFlag(VIEW_PROBE(target->mChild));
            DispatchTransformedTouchEvent(temp, TRUE, target->mChild, target->mPointerIdBits);
        }
        ClearTouchTargets();

        if (syntheticEvent) {
            if (temp) temp->Recycle();
        }
    }
}

AutoPtr<ViewGroup::TouchTarget> ViewGroup::GetTouchTarget(
    /* [in] */ IView* child)
{
    for (AutoPtr<TouchTarget> target = mFirstTouchTarget; target != NULL; target = target->mNext) {
        if (target->mChild.Get() == child) {
            return target;
        }
    }
    return NULL;
}

AutoPtr<ViewGroup::TouchTarget> ViewGroup::AddTouchTarget(
    /* [in] */ IView* child,
    /* [in] */ Int32 pointerIdBits)
{
    AutoPtr<TouchTarget> target = TouchTarget::Obtain(child, pointerIdBits);
    target->mNext = mFirstTouchTarget;
    mFirstTouchTarget = target;
    return target;
}

void ViewGroup::RemovePointersFromTouchTargets(
    /* [in] */ Int32 pointerIdBits)
{
    AutoPtr<TouchTarget> predecessor;
    AutoPtr<TouchTarget> target = mFirstTouchTarget;
    while (target != NULL) {
        AutoPtr<TouchTarget> next = target->mNext;
        if ((target->mPointerIdBits & pointerIdBits) != 0) {
            target->mPointerIdBits &= ~pointerIdBits;
            if (target->mPointerIdBits == 0) {
                if (predecessor == NULL) {
                    mFirstTouchTarget = next;
                }
                else {
                    predecessor->mNext = next;
                }
                target->Recycle();
                target = next;
                continue;
            }
        }
        predecessor = target;
        target = next;
    }
}

void ViewGroup::CancelTouchTarget(
    /* [in] */ IView* view)
{
    AutoPtr<TouchTarget> predecessor;
    AutoPtr<TouchTarget> target = mFirstTouchTarget;
    while (target != NULL) {
        AutoPtr<TouchTarget> next = target->mNext;
        if (target->mChild.Get() == view) {
            if (predecessor == NULL) {
                mFirstTouchTarget = next;
            }
            else {
                predecessor->mNext = next;
            }
            target->Recycle();

            Int64 now = SystemClock::GetUptimeMillis();
            AutoPtr<CMotionEvent> event;
            CMotionEvent::Obtain(
                now, now, IMotionEvent::ACTION_CANCEL,
                0.0f, 0.0f, 0, (CMotionEvent**)&event);
            event->SetSource(IInputDevice::SOURCE_TOUCHSCREEN);
            Boolean res;
            view->DispatchTouchEvent(event, &res);
            event->Recycle();
            return;
        }
        predecessor = target;
        target = next;
    }
}

Boolean ViewGroup::CanViewReceivePointerEvents(
    /* [in] */ IView* child)
{
    View* _child = VIEW_PROBE(child);
    assert(_child != NULL);
    return (_child->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE
            || _child->GetAnimation() != NULL;
}

Boolean ViewGroup::IsTransformedTouchPointInView(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IView* child,
    /* [in] */ IPointF* outLocalPoint)
{
    View* _child = VIEW_PROBE(child);
    Float localX = x + mScrollX - _child->mLeft;
    Float localY = y + mScrollY - _child->mTop;
    if (! _child->HasIdentityMatrix() && mAttachInfo != NULL) {
        AutoPtr<ArrayOf<Float> > locations = ArrayOf<Float>::Alloc(2);
        (*locations)[0] = localX;
        (*locations)[1] = localY;
        _child->GetInverseMatrix()->MapPoints(locations);
        localX = (*locations)[0];
        localY = (*locations)[1];
        mAttachInfo->mTmpTransformLocation[0] = localX;
        mAttachInfo->mTmpTransformLocation[1] = localY;
    }
    Boolean isInView = _child->PointInView(localX, localY);
    if (isInView && outLocalPoint != NULL) {
        outLocalPoint->Set(localX, localY);
    }
    return isInView;
}

Boolean ViewGroup::DispatchTransformedTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Boolean cancel,
    /* [in] */ IView* child,
    /* [in] */ Int32 desiredPointerIdBits)
{
    Boolean handled = FALSE;
    View* _child = NULL;
    if (child != NULL) {
        _child = VIEW_PROBE(child);
    }

    // Canceling motions is a special case.  We don't need to perform any transformations
    // or filtering.  The important part is the action, not the contents.
    Int32 oldAction = 0;
    event->GetAction(&oldAction);
    if (cancel || oldAction == IMotionEvent::ACTION_CANCEL) {
        event->SetAction(IMotionEvent::ACTION_CANCEL);
        if (_child == NULL) {
            handled = View::DispatchTouchEvent(event);
        }
        else {
            handled = _child->DispatchTouchEvent(event);
        }
        event->SetAction(oldAction);
        return handled;
    }

    // Calculate the number of pointers to deliver.
    Int32 oldPointerIdBits = 0;
    event->GetPointerIdBits(&oldPointerIdBits);
    Int32 newPointerIdBits = oldPointerIdBits & desiredPointerIdBits;

    // If for some reason we ended up in an inconsistent state where it looks like we
    // might produce a motion event with no pointers in it, then drop the event.
    if (newPointerIdBits == 0) {
        return FALSE;
    }

    // If the number of pointers is the same and we don't need to perform any fancy
    // irreversible transformations, then we can reuse the motion event for this
    // dispatch as long as we are careful to revert any changes we make.
    // Otherwise we need to make a copy.
    AutoPtr<IMotionEvent> transformedEvent;
    if (newPointerIdBits == oldPointerIdBits) {
        if (_child == NULL || _child->HasIdentityMatrix()) {
            if (_child == NULL) {
                handled = View::DispatchTouchEvent(event);
            }
            else {
                Float offsetX = mScrollX - _child->mLeft;
                Float offsetY = mScrollY - _child->mTop;
                event->OffsetLocation(offsetX, offsetY);

                handled = _child->DispatchTouchEvent(event);

                event->OffsetLocation(-offsetX, -offsetY);
            }
            return handled;
        }
        AutoPtr<CMotionEvent> temp;
        CMotionEvent::Obtain(
            (CMotionEvent*)event, (CMotionEvent**)&temp);
        transformedEvent = temp;
    }
    else {
        event->Split(newPointerIdBits, (IMotionEvent**)&transformedEvent);
    }

    // Perform any necessary transformations and dispatch.
    if (_child == NULL) {
        handled = View::DispatchTouchEvent(transformedEvent);
    }
    else {
        Float offsetX = mScrollX - _child->mLeft;
        Float offsetY = mScrollY - _child->mTop;
        transformedEvent->OffsetLocation(offsetX, offsetY);
        if (!_child->HasIdentityMatrix()) {
            transformedEvent->Transform(_child->GetInverseMatrix());
        }

        handled = _child->DispatchTouchEvent(transformedEvent);
    }

    // Done.
    transformedEvent->Recycle();
    return handled;
}

ECode ViewGroup::SetMotionEventSplittingEnabled(
    /* [in] */ Boolean split)
{
    // TODO Applications really shouldn't change this setting mid-touch event,
    // but perhaps this should handle that case and send ACTION_CANCELs to any child views
    // with gestures in progress when this is changed.
    if (split) {
        mGroupFlags |= FLAG_SPLIT_MOTION_EVENTS;
    }
    else {
        mGroupFlags &= ~FLAG_SPLIT_MOTION_EVENTS;
    }

    return NOERROR;
}

Boolean ViewGroup::IsMotionEventSplittingEnabled()
{
    return (mGroupFlags & FLAG_SPLIT_MOTION_EVENTS) == FLAG_SPLIT_MOTION_EVENTS;
}

ECode ViewGroup::RequestDisallowInterceptTouchEvent(
    /* [in] */ Boolean disallowIntercept)
{
    if (disallowIntercept == ((mGroupFlags & FLAG_DISALLOW_INTERCEPT) != 0)) {
        // We're already in this state, assume our ancestors are too
        return NOERROR;
    }

    if (disallowIntercept) {
        mGroupFlags |= FLAG_DISALLOW_INTERCEPT;
    }
    else {
        mGroupFlags &= ~FLAG_DISALLOW_INTERCEPT;
    }

    // Pass it up to our parent
    if (mParent != NULL) {
        return mParent->RequestDisallowInterceptTouchEvent(disallowIntercept);
    }

    return NOERROR;
}

/**
 * Implement this method to intercept all touch screen motion events.  This
 * allows you to watch events as they are dispatched to your children, and
 * take ownership of the current gesture at any point.
 *
 * <p>Using this function takes some care, as it has a fairly complicated
 * interaction with {@link View#onTouchEvent(MotionEvent)
 * View.onTouchEvent(MotionEvent)}, and using it requires implementing
 * that method as well as this one in the correct way.  Events will be
 * received in the following order:
 *
 * <ol>
 * <li> You will receive the down event here.
 * <li> The down event will be handled either by a child of this view
 * group, or given to your own onTouchEvent() method to handle; this means
 * you should implement onTouchEvent() to return TRUE, so you will
 * continue to see the rest of the gesture (instead of looking for
 * a parent view to handle it).  Also, by returning TRUE from
 * onTouchEvent(), you will not receive any following
 * events in onInterceptTouchEvent() and all touch processing must
 * happen in onTouchEvent() like normal.
 * <li> For as long as you return FALSE from this function, each following
 * event (up to and including the final up) will be delivered first here
 * and then to the target's onTouchEvent().
 * <li> If you return TRUE from here, you will not receive any
 * following events: the target view will receive the same event but
 * with the action {@link MotionEvent#ACTION_CANCEL}, and all further
 * events will be delivered to your onTouchEvent() method and no longer
 * appear here.
 * </ol>
 *
 * @param ev The motion event being dispatched down the hierarchy.
 * @return Return TRUE to steal motion events from the children and have
 * them dispatched to this ViewGroup through onTouchEvent().
 * The current target will receive an ACTION_CANCEL event, and no further
 * messages will be delivered here.
 */
Boolean ViewGroup::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    return FALSE;
}

/**
 *
 * Looks for a view to give focus to respecting the setting specified by
 * {@link #getDescendantFocusability()}.
 *
 * Uses {@link #onRequestFocusInDescendants(Int32, android.graphics.Rect)} to
 * find focus within the children of this group when appropriate.
 *
 * @see #FOCUS_BEFORE_DESCENDANTS
 * @see #FOCUS_AFTER_DESCENDANTS
 * @see #FOCUS_BLOCK_DESCENDANTS
 * @see #onRequestFocusInDescendants
 */
Boolean ViewGroup::RequestFocus(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    if (DBG) {
        Logger::D(VG_TAG,
            "0x%08x ViewGroup.requestFocus direction = %d", this, direction);
    }

    Int32 descendantFocusability = GetDescendantFocusability();

    switch (descendantFocusability) {
    case FOCUS_BLOCK_DESCENDANTS:
        return View::RequestFocus(direction, previouslyFocusedRect);
    case FOCUS_BEFORE_DESCENDANTS:
        {
            Boolean took = View::RequestFocus(direction, previouslyFocusedRect);
            return took ? took : OnRequestFocusInDescendants(
                direction, previouslyFocusedRect);
        }
    case FOCUS_AFTER_DESCENDANTS:
        {
            Boolean took = OnRequestFocusInDescendants(
                direction, previouslyFocusedRect);
            return took ? took : View::RequestFocus(
                direction, previouslyFocusedRect);
        }
    default:
        {
            Logger::E(VG_TAG, "descendant focusability must be "
                "one of FOCUS_BEFORE_DESCENDANTS, FOCUS_AFTER_DESCENDANTS"
                ", FOCUS_BLOCK_DESCENDANTS but is %d", descendantFocusability);

        }
    }
    return FALSE;
}

/**
  * Look for a descendant to call {@link View#requestFocus} on.
  * Called by {@link ViewGroup#requestFocus(Int32, android.graphics.Rect)}
  * when it wants to request focus within its children.  Override this to
  * customize how your {@link ViewGroup} requests focus within its children.
  * @param direction One of FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, and FOCUS_RIGHT
  * @param previouslyFocusedRect The rectangle (in this View's coordinate system)
  *        to give a finer grained hint about where focus is coming from.  May be NULL
  *        if there is no hint.
  * @return Whether focus was taken.
  */
Boolean ViewGroup::OnRequestFocusInDescendants(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    Int32 index;
    Int32 increment;
    Int32 end;
    Int32 count = mChildrenCount;
    if (count == 0) {
        return FALSE;
    }

    if ((direction & IView::FOCUS_FORWARD) != 0) {
        index = 0;
        increment = 1;
        end = count;
    }
    else {
        index = count - 1;
        increment = -1;
        end = -1;
    }

    for (Int32 i = index; i != end; i += increment) {
        View* child = VIEW_PROBE((*mChildren)[i]);
        if ((child->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE) {
            if (child->RequestFocus(direction, previouslyFocusedRect)) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

ECode ViewGroup::DispatchStartTemporaryDetach()
{
    View::DispatchStartTemporaryDetach();

    for (Int32 i = 0; i < mChildrenCount; i++) {
        (*mChildren)[i]->DispatchStartTemporaryDetach();
    }

    return NOERROR;
}

ECode ViewGroup::DispatchFinishTemporaryDetach()
{
    View::DispatchFinishTemporaryDetach();

    for (Int32 i = 0; i < mChildrenCount; i++) {
        (*mChildren)[i]->DispatchFinishTemporaryDetach();
    }

    return NOERROR;
}

void ViewGroup::DispatchAttachedToWindow(
    /* [in] */ AttachInfo* info,
    /* [in] */ Int32 visibility)
{
    mGroupFlags |= FLAG_PREVENT_DISPATCH_ATTACHED_TO_WINDOW;
    View::DispatchAttachedToWindow(info, visibility);
    mGroupFlags &= ~FLAG_PREVENT_DISPATCH_ATTACHED_TO_WINDOW;

    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* child = VIEW_PROBE((*mChildren)[i]);
        child->DispatchAttachedToWindow(info,
                visibility | (child->mViewFlags&VISIBILITY_MASK));
    }
}

//@Override
void ViewGroup::DispatchScreenStateChanged(
    /* [in] */ Int32 screenState)
{
    View::DispatchScreenStateChanged(screenState);

    for (Int32 i = 0; i < mChildrenCount; i++) {
        VIEW_PROBE((*mChildren)[i])->DispatchScreenStateChanged(screenState);
    }
}

//@Override
Boolean ViewGroup::DispatchPopulateAccessibilityEventInternal(
    /* [in] */ IAccessibilityEvent* event)
{
    Boolean handled = FALSE;
    if (IncludeForAccessibility()) {
        handled = View::DispatchPopulateAccessibilityEventInternal(event);
        if (handled) {
            return handled;
        }
    }
    // Let our children have a shot in populating the event.
    AutoPtr<ChildListForAccessibility> children = ChildListForAccessibility::Obtain(
        THIS_PROBE(IViewGroup), TRUE);
    Int32 childCount = children->GetChildCount();
    for (Int32 i = 0; i < childCount; i++) {
        View* child = VIEW_PROBE(children->GetChildAt(i));
        if ((child->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE) {
            handled = child->DispatchPopulateAccessibilityEvent(event);
            if (handled) {
                return handled;
            }
        }
    }

    children->Recycle();

    return FALSE;
}

//@Override
void ViewGroup::OnInitializeAccessibilityNodeInfoInternal(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    View::OnInitializeAccessibilityNodeInfoInternal(info);
    if (mAttachInfo != NULL) {
        mAttachInfo->mTempArrayList.Clear();

        AutoPtr<IObjectContainer> childrenForAccessibility;
        CObjectContainer::New((IObjectContainer**)&childrenForAccessibility);

        AddChildrenForAccessibility(childrenForAccessibility);

        AutoPtr<IObjectEnumerator> children;
        childrenForAccessibility->GetObjectEnumerator((IObjectEnumerator**)&children);
        Boolean hasNext = FALSE;
        while(children->MoveNext(&hasNext), hasNext) {
            AutoPtr<IView> child;
            children->Current((IInterface**)&child);
            info->AddChild(child);
        }

        childrenForAccessibility->Dispose();
    }
}

//@Override
void ViewGroup::OnInitializeAccessibilityEventInternal(
    /* [in] */ IAccessibilityEvent* event)
{
    View::OnInitializeAccessibilityEventInternal(event);
    AutoPtr<ICharSequence> seq;
    CString::New(String("ViewGroup"), (ICharSequence**)&seq);
    event->SetClassName(seq);
}

/**
 * @hide
 */
//@Override
ECode ViewGroup::ResetAccessibilityStateChanged()
{
    View::ResetAccessibilityStateChanged();
    for (Int32 i = 0; i < mChildrenCount; i++) {
        AutoPtr<IView> child = (*mChildren)[i];
        child->ResetAccessibilityStateChanged();
    }

    return NOERROR;
}

ECode ViewGroup::DispatchDetachedFromWindow()
{
    // If we still have a touch target, we are still in the process of
    // dispatching motion events to a child; we need to get rid of that
    // child to avoid dispatching events to it after the window is torn
    // down. To make sure we keep the child in a consistent state, we
    // first send it an ACTION_CANCEL motion event.
    CancelAndClearTouchTargets(NULL);

    // Similarly, set ACTION_EXIT to all hover targets and clear them.
    ExitHoverTargets();

    // In case view is detached while transition is running
    mLayoutSuppressed = FALSE;

    // Tear down our drag tracking
    mDragNotifiedChildren.Clear();
    if (mCurrentDrag != NULL) {
        mCurrentDrag->Recycle();
        mCurrentDrag = NULL;
    }

    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* tmp = VIEW_PROBE((*mChildren)[i]);
        FAIL_RETURN(tmp->DispatchDetachedFromWindow());
    }

    return View::DispatchDetachedFromWindow();
}

void ViewGroup::InternalSetPadding(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    View::InternalSetPadding(left, top, right, bottom);

    if ((mPaddingLeft | mPaddingTop | mPaddingRight | mPaddingBottom) != 0) {
        mGroupFlags |= FLAG_PADDING_NOT_NULL;
    }
    else {
        mGroupFlags &= ~FLAG_PADDING_NOT_NULL;
    }
}

ECode ViewGroup::DispatchSaveInstanceState(
    /* [in] */ IObjectInt32Map* container)
{
    FAIL_RETURN(View::DispatchSaveInstanceState(container));

    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* c = VIEW_PROBE((*mChildren)[i]);
        if ((c->mViewFlags & PARENT_SAVE_DISABLED_MASK) != PARENT_SAVE_DISABLED) {
            c->DispatchSaveInstanceState(container);
        }
    }

    return NOERROR;
}

ECode ViewGroup::DispatchFreezeSelfOnly(
    /* [in] */ IObjectInt32Map* container)
{
    return View::DispatchSaveInstanceState(container);
}

ECode ViewGroup::DispatchRestoreInstanceState(
    /* [in] */ IObjectInt32Map* container)
{
    FAIL_RETURN(View::DispatchRestoreInstanceState(container));

    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* c = VIEW_PROBE((*mChildren)[i]);
        if ((c->mViewFlags & PARENT_SAVE_DISABLED_MASK) != PARENT_SAVE_DISABLED) {
            c->DispatchRestoreInstanceState(container);
        }
    }

    return NOERROR;
}

ECode ViewGroup::DispatchThawSelfOnly(
    /* [in] */ IObjectInt32Map* container)
{
    return View::DispatchRestoreInstanceState(container);
}

/**
  * Enables or disables the drawing cache for each child of this view group.
  *
  * @param enabled TRUE to enable the cache, FALSE to dispose of it
  */
void ViewGroup::SetChildrenDrawingCacheEnabled(
    /* [in] */ Boolean enabled)
{
    if (enabled || (mPersistentDrawingCache & PERSISTENT_ALL_CACHES) != PERSISTENT_ALL_CACHES) {
        for (Int32 i = 0; i < mChildrenCount; i++) {
            (*mChildren)[i]->SetDrawingCacheEnabled(enabled);
        }
    }
}

void ViewGroup::OnAnimationStart()
{
    View::OnAnimationStart();

    // When this ViewGroup's animation starts, build the cache for the children
    if ((mGroupFlags & FLAG_ANIMATION_CACHE) == FLAG_ANIMATION_CACHE) {
        Boolean buildCache = !IsHardwareAccelerated();
        for (Int32 i = 0; i < mChildrenCount; i++) {
            View* child = VIEW_PROBE((*mChildren)[i]);
            if ((child->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE) {
                child->SetDrawingCacheEnabled(TRUE);
                if (buildCache) {
                    child->BuildDrawingCache(TRUE);
                }
            }
        }

        mGroupFlags |= FLAG_CHILDREN_DRAWN_WITH_CACHE;
    }
}

void ViewGroup::OnAnimationEnd()
{
    View::OnAnimationEnd();

    // When this ViewGroup's animation ends, destroy the cache of the children
    if ((mGroupFlags & FLAG_ANIMATION_CACHE) == FLAG_ANIMATION_CACHE) {
        mGroupFlags &= ~FLAG_CHILDREN_DRAWN_WITH_CACHE;

        if ((mPersistentDrawingCache & PERSISTENT_ANIMATION_CACHE) == 0) {
            SetChildrenDrawingCacheEnabled(FALSE);
        }
    }
}

ECode ViewGroup::CreateSnapshot(
    /* [in] */ BitmapConfig quality,
    /* [in] */ Int32 backgroundColor,
    /* [in] */ Boolean skipChildren,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    Int32 count = mChildrenCount;
    Int32* visibilities = NULL;

    if (skipChildren) {
        visibilities = new Int32[count];
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> child = GetChildAt(i);
            child->GetVisibility(&visibilities[i]);
            if (visibilities[i] == IView::VISIBLE) {
                child->SetVisibility(IView::INVISIBLE);
            }
        }
    }

    AutoPtr<IBitmap> b;
    FAIL_RETURN(View::CreateSnapshot(quality, backgroundColor,
            skipChildren, (IBitmap**)&b));

    if (skipChildren) {
        for (Int32 i = 0; i < count; i++) {
            GetChildAt(i)->SetVisibility(visibilities[i]);
        }
    }

    if (visibilities != NULL) {
        delete[] visibilities;
    }
    *bitmap = b;
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

void ViewGroup::DrawRect(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Int32 x1,
    /* [in] */ Int32 y1,
    /* [in] */ Int32 x2,
    /* [in] */ Int32 y2,
    /* [in] */ Int32 color)
{
    AutoPtr<IPaint> paint = GetDebugPaint();
    paint->SetColor(color);

    canvas->DrawLines(*GetDebugLines(x1, y1, x2, y2), paint);
}

void ViewGroup::OnDebugDrawMargins(
    /* [in] */ ICanvas* canvas)
{
    for (Int32 i = 0; i < GetChildCount(); i++) {
        AutoPtr<IView> c = GetChildAt(i);
        assert(c != NULL);
        AutoPtr<IViewGroupLayoutParams> params;
        c->GetLayoutParams((IViewGroupLayoutParams**)&params);
        params->OnDebugDraw(c, canvas);
    }
}

void ViewGroup::OnDebugDraw(
    /* [in] */ ICanvas* canvas)
{
    // Draw optical bounds
    if (GetLayoutMode() == OPTICAL_BOUNDS) {
        for (Int32 i = 0; i < GetChildCount(); i++) {
            View* c = VIEW_PROBE(GetChildAt(i));
            AutoPtr<IInsets> insets = c->GetOpticalInsets();
            Int32 l, r, t, b;
            insets->GetLeft(&l);
            insets->GetRight(&r);
            insets->GetTop(&t);
            insets->GetBottom(&b);
            DrawRect(
                canvas,c->GetLeft() + l, c->GetTop() + t,
                c->GetRight() - r, c->GetBottom() - b, IColor::RED);
        }
    }

    // Draw margins
    OnDebugDrawMargins(canvas);

    // Draw bounds
    for (Int32 i = 0; i < GetChildCount(); i++) {
        View* c = VIEW_PROBE(GetChildAt(i));
        DrawRect(canvas, c->GetLeft(), c->GetTop(), c->GetRight(), c->GetBottom(), IColor::BLUE);
    }
}

void ViewGroup::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    Int32 count = mChildrenCount;
    ArrayOf<IView*>* children = mChildren;
    Int32 flags = mGroupFlags;

    if ((flags & FLAG_RUN_ANIMATION) != 0 && CanAnimate()) {
        const Boolean cache = (mGroupFlags & FLAG_ANIMATION_CACHE) == FLAG_ANIMATION_CACHE;
        Boolean buildCache = !IsHardwareAccelerated();

        for (Int32 i = 0; i < count; i++) {
            View* child = VIEW_PROBE((*children)[i]);
            if ((child->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE) {
                const AutoPtr<IViewGroupLayoutParams> params = child->GetLayoutParams();
                AttachLayoutAnimationParameters(IVIEW_PROBE(child), params, i, count);
                BindLayoutAnimation(IVIEW_PROBE(child));
                if (cache) {
                    child->SetDrawingCacheEnabled(TRUE);

                    if (buildCache) {
                        child->BuildDrawingCache(TRUE);
                    }
                }
            }
        }

        Boolean willOverlap;
        mLayoutAnimationController->WillOverlap(&willOverlap);
        if (willOverlap) {
            mGroupFlags |= FLAG_OPTIMIZE_INVALIDATE;
        }

        mLayoutAnimationController->Start();

        mGroupFlags &= ~FLAG_RUN_ANIMATION;
        mGroupFlags &= ~FLAG_ANIMATION_DONE;

        if (cache) {
            mGroupFlags |= FLAG_CHILDREN_DRAWN_WITH_CACHE;
        }

        if (mAnimationListener != NULL) {
            AutoPtr<IAnimation> animation;
            mLayoutAnimationController->GetAnimation((IAnimation**)&animation);
            mAnimationListener->OnAnimationStart(animation);
        }
    }

    Int32 saveCount = 0;
    const Boolean clipToPadding = (flags & CLIP_TO_PADDING_MASK) == CLIP_TO_PADDING_MASK;
    if (clipToPadding) {
        canvas->Save(&saveCount);
        Boolean isNonEmpty;
        canvas->ClipRect(mScrollX + mPaddingLeft, mScrollY + mPaddingTop,
                mScrollX + mRight - mLeft - mPaddingRight,
                mScrollY + mBottom - mTop - mPaddingBottom, &isNonEmpty);

    }

    // We will draw our child's animation, let's reset the flag
    mPrivateFlags &= ~PFLAG_DRAW_ANIMATION;
    mGroupFlags &= ~FLAG_INVALIDATE_REQUIRED;

    Boolean more = FALSE;
    const Int64 drawingTime = GetDrawingTime();

    if ((flags & FLAG_USE_CHILD_DRAWING_ORDER) == 0) {
        for (Int32 i = 0; i < count; i++) {
            View* child = VIEW_PROBE((*children)[i]);
            if ((child->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE || child->GetAnimation() != NULL) {
                more |= DrawChild(canvas, IVIEW_PROBE(child), drawingTime);
            }
        }
    }
    else {
        for (Int32 i = 0; i < count; i++) {
            View* child = VIEW_PROBE((*children)[GetChildDrawingOrder(count, i)]);
            if ((child->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE || child->GetAnimation() != NULL) {
                more |= DrawChild(canvas, IVIEW_PROBE(child), drawingTime);
            }
        }
    }

    // Draw any disappearing views that have animations
    List<AutoPtr<IView> >::ReverseIterator rit = mDisappearingChildren.RBegin();
    // Go backwards -- we may delete as animations finish
    for (; rit != mDisappearingChildren.REnd(); ++rit) {
        more |= DrawChild(canvas, *rit, drawingTime);
    }

    if (DebugDraw()) {
        OnDebugDraw(canvas);
    }

    if (clipToPadding) {
        canvas->RestoreToCount(saveCount);
    }

    // mGroupFlags might have been updated by drawChild()
    flags = mGroupFlags;

    if ((flags & FLAG_INVALIDATE_REQUIRED) == FLAG_INVALIDATE_REQUIRED) {
        Invalidate(TRUE);
    }

     Boolean isDone = FALSE;
     if(mLayoutAnimationController)
            mLayoutAnimationController->IsDone(&isDone);
     if ((flags & FLAG_ANIMATION_DONE) == 0 &&
         (flags & FLAG_NOTIFY_ANIMATION_LISTENER) == 0 && isDone && !more) {
          // We want to erase the drawing cache and notify the listener after the
          // next frame is drawn because one extra invalidate() is caused by
          // drawChild() after the animation is over
          mGroupFlags |= FLAG_NOTIFY_ANIMATION_LISTENER;
          AutoPtr<DispatchDrawRunnable> end = new DispatchDrawRunnable(this);
          Post(end);
      }
}

/**
  * Returns the index of the child to draw for this iteration. Override this
  * if you want to change the drawing order of children. By default, it
  * returns i.
  * <p>
  * NOTE: In order for this method to be called, you must enable child ordering
  * first by calling {@link #setChildrenDrawingOrderEnabled(Boolean)}.
  *
  * @param i The current iteration.
  * @return The index of the child to draw this iteration.
  *
  * @see #setChildrenDrawingOrderEnabled(Boolean)
  * @see #isChildrenDrawingOrderEnabled()
  */
Int32 ViewGroup::GetChildDrawingOrder(
    /* [in] */ Int32 childCount,
    /* [in] */ Int32 i)
{
    return i;
}

void ViewGroup::NotifyAnimationListener()
{
    mGroupFlags &= ~FLAG_NOTIFY_ANIMATION_LISTENER;
    mGroupFlags |= FLAG_ANIMATION_DONE;

    if (mAnimationListener != NULL) {
        AutoPtr<IRunnable> end = new NotifyAnimationListenerRunnable(this);
        Post(end);
    }

    if ((mGroupFlags & FLAG_ANIMATION_CACHE) == FLAG_ANIMATION_CACHE) {
        mGroupFlags &= ~FLAG_CHILDREN_DRAWN_WITH_CACHE;
        if ((mPersistentDrawingCache & PERSISTENT_ANIMATION_CACHE) == 0) {
            SetChildrenDrawingCacheEnabled(FALSE);
        }
    }

    Invalidate(TRUE);
}

void ViewGroup::DispatchGetDisplayList()
{
    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* child = VIEW_PROBE((*mChildren)[i]);
        if (((child->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE || child->GetAnimation() != NULL) &&
                child->HasStaticLayer()) {
            child->mRecreateDisplayList = (child->mPrivateFlags & PFLAG_INVALIDATED)
                    == PFLAG_INVALIDATED;
            child->mPrivateFlags &= ~PFLAG_INVALIDATED;
            child->GetDisplayList();
            child->mRecreateDisplayList = FALSE;
        }
    }
}

/**
  * Draw one child of this View Group. This method is responsible for getting
  * the canvas in the right state. This includes clipping, translating so
  * that the child's scrolled origin is at 0, 0, and applying any animation
  * transformations.
  *
  * @param canvas The canvas on which to draw the child
  * @param child Who to draw
  * @param drawingTime The time at which draw is occuring
  * @return True if an invalidate() was issued
  */
Boolean ViewGroup::DrawChild(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IView* child,
    /* [in] */ Int64 drawingTime)
{
    return VIEW_PROBE(child)->Draw(canvas, THIS_PROBE(IViewGroup), drawingTime);
}

/**
  * By default, children are clipped to their bounds before drawing. This
  * allows view groups to override this behavior for animations, etc.
  *
  * @param clipChildren TRUE to clip children to their bounds,
  *        FALSE otherwise
  * @attr ref android.R.styleable#ViewGroup_clipChildren
  */
void ViewGroup::SetClipChildren(
    /* [in] */ Boolean clipChildren)
{
    Boolean previousValue = (mGroupFlags & FLAG_CLIP_CHILDREN) == FLAG_CLIP_CHILDREN;
    if (clipChildren != previousValue) {
        SetBooleanFlag(FLAG_CLIP_CHILDREN, clipChildren);
        for (Int32 i = 0; i < mChildrenCount; ++i) {
            View* child = VIEW_PROBE(GetChildAt(i));
            if (child->mDisplayList != NULL) {
                child->mDisplayList->SetClipChildren(clipChildren);
            }
        }
    }
}

/**
  * By default, children are clipped to the padding of the ViewGroup. This
  * allows view groups to override this behavior
  *
  * @param clipToPadding TRUE to clip children to the padding of the
  *        group, FALSE otherwise
  * @attr ref android.R.styleable#ViewGroup_clipToPadding
  */
void ViewGroup::SetClipToPadding(
    /* [in] */ Boolean clipToPadding)
{
    SetBooleanFlag(FLAG_CLIP_TO_PADDING, clipToPadding);
}

/**
  * {@inheritDoc}
  */
//@Override
void ViewGroup::DispatchSetSelected(
    /* [in] */ Boolean selected)
{
    for (Int32 i = 0; i < mChildrenCount; i++) {
        (*mChildren)[i]->SetSelected(selected);
    }
}

void ViewGroup::DispatchSetActivated(
    /* [in] */ Boolean activated)
{
    for (Int32 i = 0; i < mChildrenCount; i++) {
        (*mChildren)[i]->SetActivated(activated);
    }
}

//@Override
void ViewGroup::DispatchSetPressed(
    /* [in] */ Boolean pressed)
{
    for (Int32 i = 0; i < mChildrenCount; i++) {
        (*mChildren)[i]->SetPressed(pressed);

        for (Int32 i = 0; i < mChildrenCount; i++) {
            View* child = VIEW_PROBE((*mChildren)[i]);
            // Children that are clickable on their own should not
            // show a pressed state when their parent view does.
            // Clearing a pressed state always propagates.
            if (!pressed || (!child->IsClickable() && !child->IsLongClickable())) {
                child->SetPressed(pressed);
            }
        }
    }
}

/**
  * When this property is set to TRUE, this ViewGroup supports static transformations on
  * children; this causes
  * {@link #getChildStaticTransformation(View, android.view.animation.Transformation)} to be
  * invoked when a child is drawn.
  *
  * Any subclass overriding
  * {@link #getChildStaticTransformation(View, android.view.animation.Transformation)} should
  * set this property to TRUE.
  *
  * @param enabled True to enable static transformations on children, FALSE otherwise.
  *
  * @see #FLAG_SUPPORT_STATIC_TRANSFORMATIONS
  */
void ViewGroup::SetStaticTransformationsEnabled(
    /* [in] */ Boolean enabled)
{
    SetBooleanFlag(FLAG_SUPPORT_STATIC_TRANSFORMATIONS, enabled);
}

Boolean ViewGroup::GetChildStaticTransformation(
    /* [in] */ IView* child,
    /* [in] */ ITransformation* t)
{
    return FALSE;
}

AutoPtr<IView> ViewGroup::FindViewTraversal(
    /* [in] */ Int32 id)
{
    if (id == mID) {
        return AutoPtr<IView>(IVIEW_PROBE(this));
    }

    for (Int32 i = 0; i < mChildrenCount; i++) {
        Boolean isRootNS;
        (*mChildren)[i]->IsRootNamespace(&isRootNS);
        if (!isRootNS) {
            AutoPtr<IView> vv;
            (*mChildren)[i]->FindViewById(id, (IView**)&vv);
            if (vv != NULL) {
                return vv;
            }
        }
    }

    return AutoPtr<IView>(NULL);
}

/**
   * {@hide}
   */
//@Override
AutoPtr<IView> ViewGroup::FindViewWithTagTraversal(
    /* [in] */ IInterface* tag)
{
   if (tag != NULL && tag == mTag) {
       return IVIEW_PROBE(this);
   }

   for (Int32 i = 0; i < mChildrenCount; i++) {
       View* v = VIEW_PROBE((*mChildren)[i]);

       if ((v->mPrivateFlags & PFLAG_IS_ROOT_NAMESPACE) == 0) {
           AutoPtr<IView> view = v->FindViewWithTag(tag);
           if (view != NULL) {
               return view;
           }
       }
   }

   return NULL;
}

AutoPtr<IView> ViewGroup::FindViewByPredicateTraversal(
    /* [in] */ IPredicate* predicate,
    /* [in] */ IView* childToSkip)
{
    Boolean apply = FALSE;
    if ((predicate->Apply(THIS_PROBE(IViewGroup), &apply), apply)) {
        return THIS_PROBE(IViewGroup);
    }

    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* v = VIEW_PROBE((*mChildren)[i]);

        if ((*mChildren)[i] != childToSkip &&
            (v->mPrivateFlags & PFLAG_IS_ROOT_NAMESPACE) == 0) {
            AutoPtr<IView> view = v->FindViewByPredicate(predicate);
            if (view != NULL) {
                return view;
            }
        }
    }

    return NULL;
}

/**
 * Adds a child view. If no layout parameters are already set on the child, the
 * default parameters for this ViewGroup are set on the child.
 *
 * @param child the child view to add
 *
 * @see #generateDefaultLayoutParams()
 */
ECode ViewGroup::AddView(
    /* [in] */ IView* child)
{
    return AddView(child, -1);
}

/**
 * Adds a child view. If no layout parameters are already set on the child, the
 * default parameters for this ViewGroup are set on the child.
 *
 * @param child the child view to add
 * @param index the position at which to add the child
 *
 * @see #generateDefaultLayoutParams()
 */
ECode ViewGroup::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index)
{
    AutoPtr<IViewGroupLayoutParams> params;
    child->GetLayoutParams((IViewGroupLayoutParams**)&params);
    if (params == NULL) {
        GenerateDefaultLayoutParams((IViewGroupLayoutParams**)&params);
        if (params == NULL) {
//            throw new IllegalArgumentException("generateDefaultLayoutParams() cannot return NULL");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
    return AddView(child, index, params);
}

/**
 * Adds a child view with this ViewGroup's default layout parameters and the
 * specified width and height.
 *
 * @param child the child view to add
 */
ECode ViewGroup::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    AutoPtr<IViewGroupLayoutParams> params;
    GenerateDefaultLayoutParams((IViewGroupLayoutParams**)&params);

    params->SetWidth(width);
    params->SetHeight(height);
    return AddView(child, -1, params.Get());
}

/**
 * Adds a child view with the specified layout parameters.
 *
 * @param child the child view to add
 * @param params the layout parameters to set on the child
 */
ECode ViewGroup::AddView(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return AddView(child, -1, params);
}

/**
 * Adds a child view with the specified layout parameters.
 *
 * @param child the child view to add
 * @param index the position at which to add the child
 * @param params the layout parameters to set on the child
 */
ECode ViewGroup::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    //if (DBG) {
    //    System.out.println(this + " addView");
    //}

    // addViewInner() will call child.requestLayout() when setting the new LayoutParams
    // therefore, we call requestLayout() on ourselves before, so that the child's request
    // will be blocked at our level
    RequestLayout();
    Invalidate(TRUE);
    return AddViewInner(child, index, params, FALSE);
}

/**
  * {@inheritDoc}
  */
ECode ViewGroup::UpdateViewLayout(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (!CheckLayoutParams(params)) {
//        throw new IllegalArgumentException("Invalid LayoutParams supplied to " + this);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IViewParent> parent;
    view->GetParent((IViewParent**)&parent);
    if (parent.Get() != THIS_PROBE(IViewParent)) {
//        throw new IllegalArgumentException("Given view not a child of " + this);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return view->SetLayoutParams(params);
}

Boolean ViewGroup::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return  p != NULL;
}

ECode ViewGroup::SetOnHierarchyChangeListener(
    /* [in] */ IViewGroupOnHierarchyChangeListener* listener)
{
    mOnHierarchyChangeListener = listener;
    return NOERROR;
}

void ViewGroup::OnViewAdded(
    /* [in] */ IView* child)
{
    if (mOnHierarchyChangeListener != NULL) {
        mOnHierarchyChangeListener->OnChildViewAdded(THIS_PROBE(IView), child);
    }
}

void ViewGroup::OnViewRemoved(
    /* [in] */ IView* child)
{
    if (mOnHierarchyChangeListener != NULL) {
        mOnHierarchyChangeListener->OnChildViewRemoved(THIS_PROBE(IView), child);
    }
}

Boolean ViewGroup::AddViewInLayout(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return AddViewInLayout(child, index, params, FALSE);
}

Boolean ViewGroup::AddViewInLayout(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params,
    /* [in] */ Boolean preventRequestLayout)
{
    View* v = VIEW_PROBE(child);
    v->mParent = NULL;
    AddViewInner(child, index, params, preventRequestLayout);
    v->mPrivateFlags = (v->mPrivateFlags & ~PFLAG_DIRTY_MASK) | PFLAG_DRAWN;
    return TRUE;
}


void ViewGroup::CleanupLayoutState(
    /* [in] */ IView* child)
{
    View* v = VIEW_PROBE(child);
    v->mPrivateFlags &= ~View::PFLAG_FORCE_LAYOUT;
}

ECode ViewGroup::AddViewInner(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* _params,
    /* [in] */ Boolean preventRequestLayout)
{
    assert(child != NULL && "Child view cannot be NULL.");

    if (mTransition != NULL) {
        // Don't prevent other add transitions from completing, but cancel remove
        // transitions to let them complete the process before we add to the container
        mTransition->Cancel(ILayoutTransition::DISAPPEARING);
    }

    AutoPtr<IViewGroupLayoutParams> params = _params;
    View* v = VIEW_PROBE(child);

    if (v->GetParent() != NULL) {
//        throw new IllegalStateException("The specified child already has a parent. " +
//                "You must call removeView() on the child's parent first.");
        Logger::E("ViewGroup", "The specified child already has a parent. " \
            "You must call removeView() on the child's parent first.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (mTransition != NULL) {
        mTransition->AddChild(THIS_PROBE(IViewGroup), child);
    }

    if (!CheckLayoutParams(params)) {
        params = GenerateLayoutParams(params);
    }

    if (preventRequestLayout) {
        v->mLayoutParams = params;
    }
    else {
        v->SetLayoutParams(params);
    }

    if (index < 0) {
        index = mChildrenCount;
    }

    AddInArray(child, index);

    // tell our children
    if (preventRequestLayout) {
        v->AssignParent(THIS_PROBE(IViewParent));
    }
    else {
        v->mParent = THIS_PROBE(IViewParent);
    }

    if (v->HasFocus()) {
        RequestChildFocus(child, v->FindFocus());
    }

    AttachInfo* ai = mAttachInfo;
    if (ai != NULL && (mGroupFlags & FLAG_PREVENT_DISPATCH_ATTACHED_TO_WINDOW) == 0) {
        Boolean lastKeepOn = ai->mKeepScreenOn;
        ai->mKeepScreenOn = FALSE;
        v->DispatchAttachedToWindow(mAttachInfo, (mViewFlags & VISIBILITY_MASK));
        if (ai->mKeepScreenOn) {
            NeedGlobalAttributesUpdate(TRUE);
        }
        ai->mKeepScreenOn = lastKeepOn;
    }

    if (v->IsLayoutDirectionInherited()) {
        child->ResetRtlProperties();
    }

    OnViewAdded(child);

    if ((v->mViewFlags & DUPLICATE_PARENT_STATE) == DUPLICATE_PARENT_STATE) {
        mGroupFlags |= FLAG_NOTIFY_CHILDREN_ON_DRAWABLE_STATE_CHANGE;
    }

    if (v->HasTransientState()) {
        ChildHasTransientStateChanged(child, TRUE);
    }

    return NOERROR;
}

ECode ViewGroup::AddInArray(
    /* [in] */ IView* child,
    /* [in] */ Int32 index)
{
    AutoPtr<ArrayOf<IView*> > children = mChildren;
    const Int32 count = mChildrenCount;
    const Int32 size = children->GetLength();
    if (index == count) {
        if (size == count) {
            mChildren = ArrayOf<IView*>::Alloc(size + ARRAY_CAPACITY_INCREMENT);
            for (Int32 i = 0; i < size; i++) {
                mChildren->Set(i, (*children)[i]);
            }
            children = mChildren;
        }
        children->Set(mChildrenCount++, child);
    }
    else if (index < count) {
        if (size == count) {
            mChildren = ArrayOf<IView*>::Alloc(size + ARRAY_CAPACITY_INCREMENT);
            for (Int32 i = 0; i < index; i++) {
                mChildren->Set(i, (*children)[i]);
            }

            for (Int32 i = index; i < count - index; i++) {
                mChildren->Set(i + 1, (*children)[i]);
            }
            children = mChildren;
        }
        else {
            for (Int32 i = count - 1; i >= index; --i) {
                children->Set(i + 1, (*children)[i]);
            }
        }
        children->Set(index, child);
        mChildrenCount++;

        if (mLastTouchDownIndex >= index) {
            mLastTouchDownIndex++;
        }
    }
    else {
        Logger::E(VG_TAG, "index = %d, count = %d", index, count);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    return NOERROR;
}

// This method also sets the child's mParent to NULL
ECode ViewGroup::RemoveFromArray(
    /* [in] */ Int32 index)
{
    AutoPtr<IView> v = (*mChildren)[index];
    if (Find(mTransitioningViews.Begin(), mTransitioningViews.End(), v)
            == mTransitioningViews.End()) {
        VIEW_PROBE(v)->mParent = NULL;
    }

    const Int32 count = mChildrenCount;
    if (index == count - 1) {
        mChildren->Set(--mChildrenCount, NULL);
    }
    else if (index >= 0 && index < count) {
        for (Int32 i = index; i < count - 1; i++) {
            mChildren->Set(i, (*mChildren)[i + 1]);
        }
        mChildren->Set(--mChildrenCount, NULL);
    }
    else {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (mLastTouchDownIndex == index) {
        mLastTouchDownTime = 0;
        mLastTouchDownIndex = -1;
    }
    else if (mLastTouchDownIndex > index) {
        mLastTouchDownIndex--;
    }

    return NOERROR;
}

// This method also sets the children's mParent to NULL
ECode ViewGroup::RemoveFromArray(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    AutoPtr<ArrayOf<IView*> > children = mChildren;
    const Int32 childrenCount = mChildrenCount;

    start = Elastos::Core::Math::Max(0, start);
    const Int32 end = Elastos::Core::Math::Min(childrenCount, start + count);

    if (start == end) {
        return NOERROR;
    }

    if (end == childrenCount) {
        for (Int32 i = start; i < end; i++) {
            VIEW_PROBE((*mChildren)[i])->mParent = NULL;
            mChildren->Set(i, NULL);
        }
    }
    else {
        for (Int32 i = start; i < end; i++) {
            VIEW_PROBE((*mChildren)[i])->mParent = NULL;
        }

        // Since we're looping above, we might as well do the copy, but is arraycopy()
        // faster than the extra 2 bounds checks we would do in the loop?
        //TODO
        //System.arraycopy(children, end, children, start, childrenCount - end);
        for (Int32 i = end; i < childrenCount; i++) {
            mChildren->Set(start + i - end, (*mChildren)[i]);
        }

        for (Int32 i = childrenCount - (end - start); i < childrenCount; i++) {
            mChildren->Set(i, NULL);
        }
    }

    mChildrenCount -= (end - start);
    return NOERROR;
}

void ViewGroup::BindLayoutAnimation(
    /* [in] */ IView* child)
{
    AutoPtr<IAnimation> a;
    mLayoutAnimationController->GetAnimationForView(child, (IAnimation**)&a);
    child->SetAnimation(a);
}


ECode ViewGroup::AttachLayoutAnimationParameters(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* params,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count)
{
    AutoPtr<IAnimationParameters> animationParams;
    params->GetLayoutAnimationParameters((IAnimationParameters**)&animationParams);
    if (animationParams == NULL) {
        animationParams = new LayoutAnimationController::LayoutAnimationParameters();
        params->SetLayoutAnimationParameters(animationParams);
    }

    animationParams->SetCount(count);
    animationParams->SetIndex(index);

    return NOERROR;
}

ECode ViewGroup::RemoveView(
    /* [in] */ IView* view)
{
    RemoveViewInternal(view);
    RequestLayout();
    Invalidate(TRUE);
    return NOERROR;
}

ECode ViewGroup::RemoveViewInLayout(
    /* [in] */ IView* view)
{
    RemoveViewInternal(view);

    return NOERROR;
}

ECode ViewGroup::RemoveViewsInLayout(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    RemoveViewsInternal(start, count);

    return NOERROR;
}


ECode ViewGroup::RemoveViewAt(
    /* [in] */ Int32 index)
{
    RemoveViewInternal(index, GetChildAt(index));
    RequestLayout();
    Invalidate(TRUE);
    return NOERROR;
}

ECode ViewGroup::RemoveViews(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    RemoveViewsInternal(start, count);
    RequestLayout();
    Invalidate(TRUE);

    return NOERROR;
}

void ViewGroup::RemoveViewInternal(
    /* [in] */ IView* view)
{
    const Int32 index = IndexOfChild(view);
    if (index >= 0) {
        RemoveViewInternal(index, view);
    }
}

void ViewGroup::RemoveViewInternal(
    /* [in] */ Int32 index,
    /* [in] */ IView* view)
{
    if (mTransition != NULL) {
        mTransition->RemoveChild(THIS_PROBE(IViewGroup), view);
    }

    View* v = VIEW_PROBE(view);
    Boolean clearChildFocus = FALSE;
    if (view == mFocused) {
        v->UnFocus();
        clearChildFocus = TRUE;
    }

    view->ClearAccessibilityFocus();

    CancelTouchTarget(view);
    CancelHoverTarget(view);

    Boolean contains = FALSE;
    List<AutoPtr<IView> >::Iterator ator = mTransitioningViews.Begin();
    for (; ator != mTransitioningViews.End(); ++ator) {
        if (ator->Get() == view) {
            contains = TRUE;
            break;
        }
    }

    if (v->GetAnimation() != NULL || contains) {
        AddDisappearingView(view);
    }
    else if (v->mAttachInfo != NULL) {
        v->DispatchDetachedFromWindow();
    }

    if (v->HasTransientState()) {
        ChildHasTransientStateChanged(view, FALSE);
    }

    OnViewRemoved(view);

    NeedGlobalAttributesUpdate(FALSE);

    RemoveFromArray(index);

    if (clearChildFocus) {
        ClearChildFocus(view);
        EnsureInputFocusOnFirstFocusable();
    }

    if (v->IsAccessibilityFocused()) {
        v->ClearAccessibilityFocus();
    }
}

ECode ViewGroup::SetLayoutTransition(
    /* [in] */ ILayoutTransition* transition)
{
    if (mTransition != NULL) {
        mTransition->RemoveTransitionListener(mLayoutTransitionListener);
    }

    mTransition = transition;
    if (mTransition != NULL) {
        mTransition->AddTransitionListener(mLayoutTransitionListener);
    }

    return NOERROR;
}

AutoPtr<ILayoutTransition> ViewGroup::GetLayoutTransition()
{
    return mTransition;
}

void ViewGroup::RemoveViewsInternal(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    AutoPtr<IView> focused = mFocused;
    Boolean detach = mAttachInfo != NULL;
    AutoPtr<IView> clearChildFocus;

    const Int32 end = start + count;

    for (Int32 i = start; i < end; i++) {
        AutoPtr<IView> _view = (*mChildren)[i];
        View* view = VIEW_PROBE(_view);

        if (mTransition != NULL) {
            mTransition->RemoveChild(THIS_PROBE(IViewGroup), _view);
        }

        if (_view == focused) {
            view->UnFocus();
            clearChildFocus = _view;
        }

        view->ClearAccessibilityFocus();

        CancelTouchTarget(_view);
        CancelHoverTarget(_view);

        Boolean contains = FALSE;
        List<AutoPtr<IView> >::Iterator ator = mTransitioningViews.Begin();
        for (; ator != mTransitioningViews.End(); ++ator) {
            if (ator->Get() == _view) {
                contains = TRUE;
            }
        }

        if (view->GetAnimation() != NULL || contains) {
            AddDisappearingView(_view);
        }
        else if (detach) {
           view->DispatchDetachedFromWindow();
        }

        if (view->HasTransientState()) {
            ChildHasTransientStateChanged(_view, FALSE);
        }

        NeedGlobalAttributesUpdate(FALSE);

        OnViewRemoved(_view);
    }

    RemoveFromArray(start, count);

    if (clearChildFocus != NULL) {
        ClearChildFocus(clearChildFocus);
        EnsureInputFocusOnFirstFocusable();
    }
}

ECode ViewGroup::RemoveAllViews()
{
    RemoveAllViewsInLayout();
    RequestLayout();
    Invalidate(TRUE);
    return NOERROR;
}


ECode ViewGroup::RemoveAllViewsInLayout()
{
    const Int32 count = mChildrenCount;
    if (count <= 0) {
        return NOERROR;
    }

    mChildrenCount = 0;

    AutoPtr<IView> focused = mFocused;
    Boolean detach = mAttachInfo != NULL;
    AutoPtr<IView> clearChildFocus = NULL;

    NeedGlobalAttributesUpdate(FALSE);

    for (Int32 i = count - 1; i >= 0; i--) {
        AutoPtr<IView> view = (*mChildren)[i];

        if (mTransition != NULL) {
            mTransition->RemoveChild(THIS_PROBE(IViewGroup), view);
        }

        if (view == focused) {
            VIEW_PROBE(view)->UnFocus();
            clearChildFocus = view;
        }

        view->ClearAccessibilityFocus();

        CancelTouchTarget(view);
        CancelHoverTarget(view);


        Boolean contains = FALSE;
        List<AutoPtr<IView> >::Iterator ator = mTransitioningViews.Begin();
        for (; ator != mTransitioningViews.End(); ++ator) {
            if (ator->Get() == view) {
                contains = TRUE;
                break;
            }
        }

        assert(view->Probe(EIID_View) != NULL);
        View* v = VIEW_PROBE(view);
        if (v->GetAnimation() != NULL || contains) {
            AddDisappearingView(view);
        }
        else if (detach) {
           v->DispatchDetachedFromWindow();
        }

        if (v->HasTransientState()) {
            ChildHasTransientStateChanged(view, FALSE);
        }

        OnViewRemoved(view);

        v->mParent = NULL;
        mChildren->Set(i, NULL);
    }

    if (clearChildFocus != NULL) {
        ClearChildFocus(clearChildFocus);
        EnsureInputFocusOnFirstFocusable();
    }

    return NOERROR;
}

void ViewGroup::RemoveDetachedView(
    /* [in] */ IView* child,
    /* [in] */ Boolean animate)
{
    if (mTransition != NULL) {
        mTransition->RemoveChild(THIS_PROBE(IViewGroup), child);
    }

    if (child == mFocused) {
        child->ClearFocus();
    }

    child->ClearAccessibilityFocus();

    CancelTouchTarget(child);
    CancelHoverTarget(child);

    View* _child = VIEW_PROBE(child);
    assert(_child != NULL);

    Boolean contains = FALSE;
    List<AutoPtr<IView> >::Iterator ator = mTransitioningViews.Begin();
    for (; ator != mTransitioningViews.End(); ++ator) {
        if (ator->Get() == child) {
            contains = TRUE;
        }
    }
    if ((animate && _child->GetAnimation() != NULL) || contains) {
        AddDisappearingView(child);
    }
    else if (_child->mAttachInfo != NULL) {
        _child->DispatchDetachedFromWindow();
    }

    if (_child->HasTransientState()) {
        ChildHasTransientStateChanged(child, FALSE);
    }

    OnViewRemoved(child);
}

void ViewGroup::AttachViewToParent(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    assert(child != NULL);
    View* v = VIEW_PROBE(child);

    v->mLayoutParams = params;

    if (index < 0) {
        index = mChildrenCount;
    }

    AddInArray(child, index);

    v->mParent = THIS_PROBE(IViewParent);
    v->mPrivateFlags = (v->mPrivateFlags & ~PFLAG_DIRTY_MASK
                    & ~PFLAG_DRAWING_CACHE_VALID)
            | PFLAG_DRAWN | PFLAG_INVALIDATED;
    mPrivateFlags |= PFLAG_INVALIDATED;

    if (v->HasFocus()) {
        RequestChildFocus(child, v->FindFocus());
    }
}

void ViewGroup::DetachViewFromParent(
    /* [in] */ IView* child)
{
    RemoveFromArray(IndexOfChild(child));
}

void ViewGroup::DetachViewFromParent(
    /* [in] */ Int32 index)
{
    RemoveFromArray(index);
}

void ViewGroup::DetachViewsFromParent(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    RemoveFromArray(start, count);
}

void ViewGroup::DetachAllViewsFromParent()
{
    Int32 count = mChildrenCount;
    if (count <= 0) {
        return;
    }

    mChildrenCount = 0;

    for (int i = count - 1; i >= 0; i--) {
        VIEW_PROBE((*mChildren)[i])->mParent = NULL;
        mChildren->Set(i, NULL);
    }
}

ECode ViewGroup::InvalidateChild(
    /* [in] */ IView* child,
    /* [in] */ IRect* dirty)
{
    assert(child != NULL);
    View* v = VIEW_PROBE(child);

    AutoPtr<IViewParent> parent = THIS_PROBE(IViewParent);

    AttachInfo* attachInfo = mAttachInfo;
    if (attachInfo != NULL) {
        // If the child is drawing an animation, we want to copy this flag onto
        // ourselves and the parent to make sure the invalidate request goes
        // through
        Boolean drawAnimation = (v->mPrivateFlags & PFLAG_DRAW_ANIMATION)
                == PFLAG_DRAW_ANIMATION;

        // Check whether the child that requests the invalidate is fully opaque
        // Views being animated or transformed are not considered opaque because we may
        // be invalidating their old position and need the parent to paint behind them.
        AutoPtr<IMatrix> childMatrix = v->GetMatrix();
        Boolean isIdentity = FALSE;
        childMatrix->IsIdentity(&isIdentity);
        Boolean isOpaque = v->IsOpaque() && !drawAnimation &&
                v->GetAnimation() == NULL && isIdentity;
        // Mark the child as dirty, using the appropriate flag
        // Make sure we do not set both flags at the same time
        Int32 opaqueFlag = isOpaque ? PFLAG_DIRTY_OPAQUE : PFLAG_DIRTY;

        if (v->mLayerType != IView::LAYER_TYPE_NONE) {
            mPrivateFlags |= PFLAG_INVALIDATED;
            mPrivateFlags &= ~PFLAG_DRAWING_CACHE_VALID;
            v->mLocalDirtyRect->Union(dirty);
        }

        AutoPtr<ArrayOf<Int32> > location = ArrayOf<Int32>::Alloc(2);
        (*location)[CHILD_LEFT_INDEX] = v->mLeft;
        (*location)[CHILD_TOP_INDEX] = v->mTop;
        if (!isIdentity || (mGroupFlags & FLAG_SUPPORT_STATIC_TRANSFORMATIONS) != 0) {
            AutoPtr<IRectF> boundingRect = attachInfo->mTmpTransformRect;
            boundingRect->Set(dirty);
            AutoPtr<IMatrix> transformMatrix;
            if ((mGroupFlags & FLAG_SUPPORT_STATIC_TRANSFORMATIONS) != 0) {
                AutoPtr<ITransformation> t = attachInfo->mTmpTransformation;
                Boolean transformed = GetChildStaticTransformation(child, t);
                if (transformed) {
                    transformMatrix = attachInfo->mTmpMatrix;

                    AutoPtr<IMatrix> tmpMatrix;
                    t->GetMatrix((IMatrix**)&tmpMatrix);

                    transformMatrix->Set(tmpMatrix);
                    if (!isIdentity) {
                        Boolean res;
                        transformMatrix->PreConcat(childMatrix, &res);
                    }
                }
                else {
                    transformMatrix = childMatrix;
                }
            }
            else {
                transformMatrix = childMatrix;
            }

            Boolean res;
            transformMatrix->MapRect(boundingRect, &res);
            Float l, r, t, b;
            boundingRect->GetLeft(&l);
            boundingRect->GetRight(&r);
            boundingRect->GetTop(&t);
            boundingRect->GetBottom(&b);
            dirty->Set(
                (Int32)(l - 0.5f), (Int32)(t - 0.5f),
                (Int32)(r + 0.5f), (Int32)(b + 0.5f));
        }

        do {
            View* view = NULL;
            if (IView::Probe(parent) != NULL) {
                view = VIEW_PROBE(parent);
            }

            if (drawAnimation) {
                if (view != NULL) {
                    view->mPrivateFlags |= PFLAG_DRAW_ANIMATION;
                }
                else if (parent != NULL) {
                    ViewRootImpl* viewRootImpl = reinterpret_cast<ViewRootImpl*>(parent->Probe(EIID_ViewRootImpl));

                    if (viewRootImpl != NULL) {
                        viewRootImpl->mIsAnimating = TRUE;
                    }
                }
            }

            // If the parent is dirty opaque or not dirty, mark it dirty with the opaque
            // flag coming from the child that initiated the invalidate
            if (view != NULL) {
                if ((view->mViewFlags & FADING_EDGE_MASK) != 0 &&
                        view->GetSolidColor() == 0) {
                    opaqueFlag = PFLAG_DIRTY;
                }
                if ((view->mPrivateFlags & PFLAG_DIRTY_MASK) != PFLAG_DIRTY) {
                    view->mPrivateFlags = (view->mPrivateFlags & ~PFLAG_DIRTY_MASK) | opaqueFlag;
                }
            }

            AutoPtr<IViewParent> tempParent;
            parent->InvalidateChildInParent(location, dirty, (IViewParent**)&tempParent);
            parent = tempParent;
            if (view != NULL) {
                // Account for transform on current parent
                AutoPtr<IMatrix> m = view->GetMatrix();
                m->IsIdentity(&isIdentity);
                if (!isIdentity) {
                    AutoPtr<IRectF> boundingRect = attachInfo->mTmpTransformRect;
                    boundingRect->Set(dirty);
                    Boolean res;
                    m->MapRect(boundingRect, &res);

                    Float l, r, t, b;
                    boundingRect->GetLeft(&l);
                    boundingRect->GetRight(&r);
                    boundingRect->GetTop(&t);
                    boundingRect->GetBottom(&b);
                    dirty->Set(
                        (Int32)(l - 0.5f), (Int32)(t - 0.5f),
                        (Int32)(r + 0.5f), (Int32)(b + 0.5f));
                }
            }
        } while (parent != NULL);

        attachInfo->mInvalidateChildLocation[0] = (*location)[0];
        attachInfo->mInvalidateChildLocation[1] = (*location)[1];
    }

    return NOERROR;
}

AutoPtr<IViewParent> ViewGroup::InvalidateChildInParent(
        /* [in] */ ArrayOf<Int32>* location,
        /* [in] */ IRect* dirty)
{
    if ((mPrivateFlags & PFLAG_DRAWN) == PFLAG_DRAWN ||
            (mPrivateFlags & PFLAG_DRAWING_CACHE_VALID) == PFLAG_DRAWING_CACHE_VALID) {
        if ((mGroupFlags & (FLAG_OPTIMIZE_INVALIDATE | FLAG_ANIMATION_DONE)) !=
                    FLAG_OPTIMIZE_INVALIDATE) {
            dirty->Offset((*location)[CHILD_LEFT_INDEX] - mScrollX,
                    (*location)[CHILD_TOP_INDEX] - mScrollY);

            const Int32 left = mLeft;
            const Int32 top = mTop;

            if ((mGroupFlags & FLAG_CLIP_CHILDREN) == FLAG_CLIP_CHILDREN) {
                Boolean tmp = FALSE;
                if (!(dirty->Intersect(0, 0, mRight - left, mBottom - top, &tmp), tmp)) {
                    dirty->SetEmpty();
                }
            }

            mPrivateFlags &= ~PFLAG_DRAWING_CACHE_VALID;

            (*location)[CHILD_LEFT_INDEX] = left;
            (*location)[CHILD_TOP_INDEX] = top;

            if (mLayerType != IView::LAYER_TYPE_NONE) {
                mPrivateFlags |= PFLAG_INVALIDATED;
                mLocalDirtyRect->Union(dirty);
            }

            return mParent;

        }
        else {
            mPrivateFlags &= ~PFLAG_DRAWN & ~PFLAG_DRAWING_CACHE_VALID;

            (*location)[CHILD_LEFT_INDEX] = mLeft;
            (*location)[CHILD_TOP_INDEX] = mTop;
            if ((mGroupFlags & FLAG_CLIP_CHILDREN) == FLAG_CLIP_CHILDREN) {
                dirty->Set(0, 0, mRight - mLeft, mBottom - mTop);
            }
            else {
                // in case the dirty rect extends outside the bounds of this container
                dirty->Union(0, 0, mRight - mLeft, mBottom - mTop);
            }

            if (mLayerType != IView::LAYER_TYPE_NONE) {
                mPrivateFlags |= PFLAG_INVALIDATED;
                mLocalDirtyRect->Union(dirty);
            }

            return mParent;
        }
    }

    return NULL;
}

ECode ViewGroup::InvalidateChildFast(
    /* [in] */ IView* child,
    /* [in] */ IRect* dirty)
{
    assert(child != NULL);
    AutoPtr<IViewParent> parent = THIS_PROBE(IViewParent);

    AttachInfo* attachInfo = mAttachInfo;
    View* v = VIEW_PROBE(child);
    if (attachInfo != NULL) {
        if (v->mLayerType != IView::LAYER_TYPE_NONE) {
            v->mLocalDirtyRect->Union(dirty);
        }

        Int32 left = v->mLeft;
        Int32 top = v->mTop;
        AutoPtr<IMatrix> tmpMatrix = v->GetMatrix();
        Boolean identity = FALSE;
        tmpMatrix->IsIdentity(&identity);
        if (!identity) {
            v->TransformRect(dirty);
        }

        do {
            if (IViewGroup::Probe(parent) != NULL) {
                ViewGroup* parentVG = VIEWGROUP_PROBE(parent);
                if (parentVG->mLayerType != IView::LAYER_TYPE_NONE) {
                    // Layered parents should be recreated, not just re-issued
                    parentVG->Invalidate();
                    parent = NULL;
                }
                else {
                    parent = parentVG->InvalidateChildInParentFast(left, top, dirty);
                    left = parentVG->mLeft;
                    top = parentVG->mTop;
                }
            }
            else {
                // Reached the top; this calls into the usual invalidate method in
                // ViewRootImpl, which schedules a traversal
                AutoPtr<ArrayOf<Int32> > location = ArrayOf<Int32>::Alloc(2);

                (*location)[0] = left;
                (*location)[1] = top;
                AutoPtr<IViewParent> tempParent;
                parent->InvalidateChildInParent(location, dirty, (IViewParent**)&tempParent);
                parent = tempParent;
                attachInfo->mInvalidateChildLocation[0] = (*location)[0];
                attachInfo->mInvalidateChildLocation[1] = (*location)[1];
            }
        } while (parent != NULL);
    }

    return NOERROR;
}

AutoPtr<IViewParent> ViewGroup::InvalidateChildInParentFast(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ IRect* dirty)
{
    if ((mPrivateFlags & PFLAG_DRAWN) == PFLAG_DRAWN ||
            (mPrivateFlags & PFLAG_DRAWING_CACHE_VALID) == PFLAG_DRAWING_CACHE_VALID) {
        dirty->Offset(left - mScrollX, top - mScrollY);

        Boolean isIntersect = FALSE;
        if ((mGroupFlags & FLAG_CLIP_CHILDREN) == 0 ||
            (dirty->Intersect(0, 0, mRight - mLeft, mBottom - mTop, &isIntersect), isIntersect)) {

            if (mLayerType != IView::LAYER_TYPE_NONE) {
                mLocalDirtyRect->Union(dirty);
            }

            AutoPtr<IMatrix> tmpMatrix = GetMatrix();
            assert(tmpMatrix != NULL);
            Boolean identity = FALSE;
            tmpMatrix->IsIdentity(&identity);
            if (!identity) {
                TransformRect(dirty);
            }

            return mParent;
        }
    }

    return NULL;
}

ECode ViewGroup::OffsetDescendantRectToMyCoords(
    /* [in] */ IView* descendant,
    /* [in] */ IRect* rect)
{
    return OffsetRectBetweenParentAndChild(descendant, rect, TRUE, FALSE);
}

ECode ViewGroup::OffsetRectIntoDescendantCoords(
    /* [in] */ IView* descendant,
    /* [in] */ IRect* rect)
{
    return OffsetRectBetweenParentAndChild(descendant, rect, FALSE, FALSE);
}

ECode ViewGroup::OffsetRectBetweenParentAndChild(
    /* [in] */ IView* descendant,
    /* [in] */ IRect* rect,
    /* [in] */ Boolean offsetFromChildToParent,
    /* [in] */ Boolean clipToBounds)
{
    assert(descendant != NULL);

    // already in the same coord system :)
    if (descendant == IVIEW_PROBE(this)) {
        return NOERROR;
    }

    View* v = VIEW_PROBE(descendant);
    AutoPtr<IViewParent> theParent = v->mParent;
    IViewParent* thisParent = THIS_PROBE(IViewParent);
    Boolean result;

    // search and offset up to the parent
    while ((theParent != NULL)
            && (theParent->Probe(EIID_View) != NULL)
            && (theParent.Get() != thisParent)) {

        if (offsetFromChildToParent) {
            rect->Offset(v->mLeft - v->mScrollX, v->mTop - v->mScrollY);
            if (clipToBounds) {
                View* p = VIEW_PROBE(theParent);
                rect->Intersect(0, 0, p->mRight - p->mLeft, p->mBottom - p->mTop, &result);
            }
        }
        else {
            if (clipToBounds) {
                View* p = VIEW_PROBE(theParent);
                rect->Intersect(0, 0, p->mRight - p->mLeft, p->mBottom - p->mTop, &result);
            }
            rect->Offset(v->mScrollX - v->mLeft, v->mScrollY - v->mTop);
        }

        v = VIEW_PROBE(theParent);
        theParent = v->mParent;
    }

    // now that we are up to this view, need to offset one more time
    // to get into our coordinate space
    if (theParent.Get() == thisParent) {
        if (offsetFromChildToParent) {
            rect->Offset(v->mLeft - v->mScrollX, v->mTop - v->mScrollY);
        }
        else {
            rect->Offset(v->mScrollX - v->mLeft, v->mScrollY - v->mTop);
        }
    }
    else {
        Logger::E(VG_TAG, "parameter must be a descendant of this view");
//        throw new IllegalArgumentException("parameter must be a descendant of this view");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode ViewGroup::OffsetChildrenTopAndBottom(
    /* [in] */ Int32 offset)
{
    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* v = VIEW_PROBE((*mChildren)[i]);
        v->mTop += offset;
        v->mBottom += offset;
        if (v->mDisplayList != NULL) {
            v->mDisplayList->OffsetTopBottom(offset);
            InvalidateViewProperty(FALSE, FALSE);
        }
    }

    return NOERROR;
}

ECode ViewGroup::GetChildVisibleRect(
    /* [in] */ IView* child,
    /* [in] */ IRect* r,
    /* [in] */ IPoint* offset,
    /* [out] */ Boolean* result)
{
    // It doesn't make a whole lot of sense to call this on a view that isn't attached,
    // but for some simple tests it can be useful. If we don't have attach info this
    // will allocate memory.
    AutoPtr<IRectF> rect;
    if (mAttachInfo != NULL) {
        rect = mAttachInfo->mTmpTransformRect;
    }
    else {
        CRectF::New((IRectF**)&rect);
    }

    rect->Set(r);

    assert(child != NULL && result != NULL);
    View* _child = VIEW_PROBE(child);
    if (!_child->HasIdentityMatrix()) {
        Boolean res;
        _child->GetMatrix()->MapRect(rect, &res);
    }

    Int32 dx = _child->mLeft - mScrollX;
    Int32 dy = _child->mTop - mScrollY;

    rect->Offset(dx, dy);

    if (offset != NULL) {
        if (!_child->HasIdentityMatrix()) {
            AutoPtr<ArrayOf<Float> > position = ArrayOf<Float>::Alloc(2);
            Int32 x, y;
            offset->GetX(&x);
            offset->GetY(&y);
            (*position)[0] = x;
            (*position)[1] = y;

            _child->GetMatrix()->MapPoints(position);
            offset->Set((Int32)((*position)[0] + 0.5f), (Int32)((*position)[1] + 0.5f));

            if (mAttachInfo != NULL) {
                mAttachInfo->mTmpTransformLocation[0] = (*position)[0];
                mAttachInfo->mTmpTransformLocation[1] = (*position)[1];
            }
        }
        offset->Offset(dx, dy);
    }

    Boolean tmp = FALSE;
    if ((rect->Intersect(0, 0, mRight - mLeft, mBottom - mTop, &tmp), tmp)) {
        if (mParent == NULL) {
            *result = FALSE;
            return NOERROR;
        }

        Float l, rr, t, b;
        rect->GetLeft(&l);
        rect->GetRight(&rr);
        rect->GetTop(&t);
        rect->GetBottom(&b);

        r->Set((Int32)(l + 0.5f), (Int32)(t + 0.5f),
            (Int32)(rr + 0.5f), (Int32)(b + 0.5f));

        return mParent->GetChildVisibleRect(IVIEW_PROBE(this), r, offset, result);
    }

    *result = FALSE;
    return NOERROR;
}

ECode ViewGroup::Layout(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    Boolean changing = FALSE;
    if (mTransition == NULL || !(mTransition->IsChangingLayout(&changing), changing)) {
        if (mTransition != NULL) {
            mTransition->LayoutChange(THIS_PROBE(IViewGroup));
        }

        View::Layout(l, t, r, b);
    }
    else {
        // record the fact that we noop'd it; request layout when transition finishes
        mLayoutSuppressed = TRUE;
    }

    return NOERROR;
}

Boolean ViewGroup::CanAnimate()
{
    return mLayoutAnimationController != NULL;
}

ECode ViewGroup::StartLayoutAnimation()
{
    if (mLayoutAnimationController != NULL) {
        mGroupFlags |= FLAG_RUN_ANIMATION;
        RequestLayout();
    }
    return NOERROR;
}

ECode ViewGroup::ScheduleLayoutAnimation()
{
    mGroupFlags |= FLAG_RUN_ANIMATION;
    return NOERROR;
}

ECode ViewGroup::SetLayoutAnimation(
    /* [in] */ ILayoutAnimationController* controller)
{
    mLayoutAnimationController = controller;
    if (mLayoutAnimationController != NULL) {
        mGroupFlags |= FLAG_RUN_ANIMATION;
    }
    return NOERROR;
}

AutoPtr<ILayoutAnimationController> ViewGroup::GetLayoutAnimation()
{
    return mLayoutAnimationController;
}

Boolean ViewGroup::IsAnimationCacheEnabled()
{
    return (mGroupFlags & FLAG_ANIMATION_CACHE) == FLAG_ANIMATION_CACHE;
}

ECode ViewGroup::SetAnimationCacheEnabled(
    /* [in] */ Boolean enabled)
{
    SetBooleanFlag(FLAG_ANIMATION_CACHE, enabled);
    return NOERROR;
}

Boolean ViewGroup::IsAlwaysDrawnWithCacheEnabled()
{
    return (mGroupFlags & FLAG_ALWAYS_DRAWN_WITH_CACHE) == FLAG_ALWAYS_DRAWN_WITH_CACHE;
}

ECode ViewGroup::SetAlwaysDrawnWithCacheEnabled(
    /* [in] */ Boolean always)
{
    SetBooleanFlag(FLAG_ALWAYS_DRAWN_WITH_CACHE, always);
    return NOERROR;
}

Boolean ViewGroup::IsChildrenDrawnWithCacheEnabled()
{
    return (mGroupFlags & FLAG_CHILDREN_DRAWN_WITH_CACHE) == FLAG_CHILDREN_DRAWN_WITH_CACHE;
}

void ViewGroup::SetChildrenDrawnWithCacheEnabled(
    /* [in] */ Boolean enabled)
{
    SetBooleanFlag(FLAG_CHILDREN_DRAWN_WITH_CACHE, enabled);
}

Boolean ViewGroup::IsChildrenDrawingOrderEnabled()
{
    return (mGroupFlags & FLAG_USE_CHILD_DRAWING_ORDER) == FLAG_USE_CHILD_DRAWING_ORDER;
}

void ViewGroup::SetChildrenDrawingOrderEnabled(
    /* [in] */ Boolean enabled)
{
    SetBooleanFlag(FLAG_USE_CHILD_DRAWING_ORDER, enabled);
}

void ViewGroup::SetBooleanFlag(
    /* [in] */ Int32 flag,
    /* [in] */ Boolean value)
{
    if (value) {
        mGroupFlags |= flag;
    }
    else {
        mGroupFlags &= ~flag;
    }
}

Int32 ViewGroup::GetPersistentDrawingCache()
{
    return mPersistentDrawingCache;
}

ECode ViewGroup::SetPersistentDrawingCache(
    /* [in] */ Int32 drawingCacheToKeep)
{
    mPersistentDrawingCache = drawingCacheToKeep & PERSISTENT_ALL_CACHES;
    return NOERROR;
}

Int32 ViewGroup::GetLayoutMode()
{
    return mLayoutMode;
}

ECode ViewGroup::SetLayoutMode(
    /* [in] */ Int32 layoutMode)
{
    if (mLayoutMode != layoutMode) {
        mLayoutMode = layoutMode;
        RequestLayout();
    }

    return NOERROR;
}

ECode ViewGroup::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<IViewGroupLayoutParams> temp;
    AutoPtr<IContext> ctx = GetContext();
    CViewGroupLayoutParams::New(ctx, attrs, (IViewGroupLayoutParams**)&temp);
    *params = temp;
    REFCOUNT_ADD(*params);
    return NOERROR;
}

AutoPtr<IViewGroupLayoutParams> ViewGroup::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return AutoPtr<IViewGroupLayoutParams>(p);
}

/**
 * Returns a set of default layout parameters. These parameters are requested
 * when the View passed to {@link #addView(View)} has no layout parameters
 * already set. If NULL is returned, an exception is thrown from addView.
 *
 * @return a set of default layout parameters or NULL
 */
ECode ViewGroup::GenerateDefaultLayoutParams(
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<IViewGroupLayoutParams> temp;
    CViewGroupLayoutParams::New(
        IViewGroupLayoutParams::WRAP_CONTENT,
        IViewGroupLayoutParams::WRAP_CONTENT, (IViewGroupLayoutParams**)&temp);
    *params = temp;
    REFCOUNT_ADD(*params);
    return NOERROR;
}

///**
//    * {@inheritDoc}
//    */
//@Override
//protected void debug(Int32 depth) {
//    super.debug(depth);
//    String output;
//
//    if (mFocused != NULL) {
//        output = debugIndent(depth);
//        output += "mFocused";
//        Log.d(VIEW_LOG_TAG, output);
//    }
//    if (mChildrenCount != 0) {
//        output = debugIndent(depth);
//        output += "{";
//        Log.d(VIEW_LOG_TAG, output);
//    }
//    Int32 count = mChildrenCount;
//    for (Int32 i = 0; i < count; i++) {
//        View child = mChildren[i];
//        child.debug(depth + 1);
//    }
//
//    if (mChildrenCount != 0) {
//        output = debugIndent(depth);
//        output += "}";
//        Log.d(VIEW_LOG_TAG, output);
//    }
//}

/**
  * Returns the position in the group of the specified child view.
  *
  * @param child the view for which to get the position
  * @return a positive integer representing the position of the view in the
  *         group, or -1 if the view does not exist in the group
  */
Int32 ViewGroup::IndexOfChild(
    /* [in] */ IView* child)
{
    for (Int32 i = 0; i < mChildrenCount; i++) {
        if ((*mChildren)[i] == child) {
            return i;
        }
    }

    return -1;
}

/**
 * Returns the number of children in the group.
 *
 * @return a positive integer representing the number of children in
 *         the group
 */
Int32 ViewGroup::GetChildCount()
{
    return mChildrenCount;
}

/**
 * Returns the view at the specified position in the group.
 *
 * @param index the position at which to get the view from
 * @return the view at the specified position or NULL if the position
 *         does not exist within the group
 */
AutoPtr<IView> ViewGroup::GetChildAt(
    /* [in] */ Int32 index)
{
    if (index < 0 || index >= mChildrenCount) {
        return NULL;
    }
    return (*mChildren)[index];
}

void ViewGroup::MeasureChildren(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* v = VIEW_PROBE((*mChildren)[i]);
        if ((v->mViewFlags & VISIBILITY_MASK) != IView::GONE) {
            MeasureChild((*mChildren)[i], widthMeasureSpec, heightMeasureSpec);
        }
    }
}

void ViewGroup::MeasureChild(
    /* [in] */ IView* child,
    /* [in] */ Int32 parentWidthMeasureSpec,
    /* [in] */ Int32 parentHeightMeasureSpec)
{
    assert(child != NULL);

    AutoPtr<IViewGroupLayoutParams> lp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

    Int32 width, height;
    lp->GetWidth(&width);
    lp->GetHeight(&height);

    const Int32 childWidthMeasureSpec = GetChildMeasureSpec(parentWidthMeasureSpec,
            mPaddingLeft + mPaddingRight, width);
    const Int32 childHeightMeasureSpec = GetChildMeasureSpec(parentHeightMeasureSpec,
            mPaddingTop + mPaddingBottom, height);

    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

/**
 * Ask one of the children of this view to measure itself, taking into
 * account both the MeasureSpec requirements for this view and its padding
 * and margins. The child must have MarginLayoutParams The heavy lifting is
 * done in getChildMeasureSpec.
 *
 * @param child The child to measure
 * @param parentWidthMeasureSpec The width requirements for this view
 * @param widthUsed Extra space that has been used up by the parent
 *        horizontally (possibly by other children of the parent)
 * @param parentHeightMeasureSpec The height requirements for this view
 * @param heightUsed Extra space that has been used up by the parent
 *        vertically (possibly by other children of the parent)
 */
void ViewGroup::MeasureChildWithMargins(
    /* [in] */ IView* child,
    /* [in] */ Int32 parentWidthMeasureSpec,
    /* [in] */ Int32 widthUsed,
    /* [in] */ Int32 parentHeightMeasureSpec,
    /* [in] */ Int32 heightUsed)
{
    AutoPtr<IViewGroupLayoutParams> lv;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lv);

    IViewGroupMarginLayoutParams* lp = (IViewGroupMarginLayoutParams*)lv.Get();

    Int32 width, height, left, top, right, bottom;
    lp->GetWidth(&width);
    lp->GetHeight(&height);
    lp->GetMargins(&left, &top, &right, &bottom);

    Int32 childWidthMeasureSpec = GetChildMeasureSpec(parentWidthMeasureSpec,
            mPaddingLeft + mPaddingRight + left + right + widthUsed, width);
    Int32 childHeightMeasureSpec = GetChildMeasureSpec(parentHeightMeasureSpec,
            mPaddingTop + mPaddingBottom + top + bottom + heightUsed, height);

    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

/**
 * Does the hard part of measureChildren: figuring out the MeasureSpec to
 * pass to a particular child. This method figures out the right MeasureSpec
 * for one dimension (height or width) of one child view.
 *
 * The goal is to combine information from our MeasureSpec with the
 * LayoutParams of the child to get the best possible results. For example,
 * if the this view knows its size (because its MeasureSpec has a mode of
 * EXACTLY), and the child has indicated in its LayoutParams that it wants
 * to be the same size as the parent, the parent should ask the child to
 * layout given an exact size.
 *
 * @param spec The requirements for this view
 * @param padding The padding of this view for the current dimension and
 *        margins, if applicable
 * @param childDimension How big the child wants to be in the current
 *        dimension
 * @return a MeasureSpec integer for the child
 */
Int32 ViewGroup::GetChildMeasureSpec(
    /* [in] */ Int32 spec,
    /* [in] */ Int32 padding,
    /* [in] */ Int32 childDimension)
{
    Int32 specMode = MeasureSpec::GetMode(spec);
    Int32 specSize = MeasureSpec::GetSize(spec);

    Int32 size = Elastos::Core::Math::Max(0, specSize - padding);

    Int32 resultSize = 0;
    Int32 resultMode = 0;

    switch (specMode) {
    // Parent has imposed an exact size on us
    case MeasureSpec::EXACTLY:
        if (childDimension >= 0) {
            resultSize = childDimension;
            resultMode = MeasureSpec::EXACTLY;
        }
        else if (childDimension == IViewGroupLayoutParams::MATCH_PARENT) {
            // Child wants to be our size. So be it.
            resultSize = size;
            resultMode = MeasureSpec::EXACTLY;
        }
        else if (childDimension == IViewGroupLayoutParams::WRAP_CONTENT) {
            // Child wants to determine its own size. It can't be
            // bigger than us.
            resultSize = size;
            resultMode = MeasureSpec::AT_MOST;
        }
        break;

    // Parent has imposed a maximum size on us
    case MeasureSpec::AT_MOST:
        if (childDimension >= 0) {
            // Child wants a specific size... so be it
            resultSize = childDimension;
            resultMode = MeasureSpec::EXACTLY;
        }
        else if (childDimension == IViewGroupLayoutParams::MATCH_PARENT) {
            // Child wants to be our size, but our size is not fixed.
            // Constrain child to not be bigger than us.
            resultSize = size;
            resultMode = MeasureSpec::AT_MOST;
        }
        else if (childDimension == IViewGroupLayoutParams::WRAP_CONTENT) {
            // Child wants to determine its own size. It can't be
            // bigger than us.
            resultSize = size;
            resultMode = MeasureSpec::AT_MOST;
        }
        break;

    // Parent asked to see how big we want to be
    case MeasureSpec::UNSPECIFIED:
        if (childDimension >= 0) {
            // Child wants a specific size... let him have it
            resultSize = childDimension;
            resultMode = MeasureSpec::EXACTLY;
        }
        else if (childDimension == IViewGroupLayoutParams::MATCH_PARENT) {
            // Child wants to be our size... find out how big it should
            // be
            resultSize = 0;
            resultMode = MeasureSpec::UNSPECIFIED;
        }
        else if (childDimension == IViewGroupLayoutParams::WRAP_CONTENT) {
            // Child wants to determine its own size.... find out how
            // big it should be
            resultSize = 0;
            resultMode = MeasureSpec::UNSPECIFIED;
        }
        break;
    }
    return MeasureSpec::MakeMeasureSpec(resultSize, resultMode);
}

ECode ViewGroup::ClearDisappearingChildren()
{
    mDisappearingChildren.Clear();
    Invalidate();
    return NOERROR;
}

void ViewGroup::AddDisappearingView(
    /* [in] */ IView* v)
{
    mDisappearingChildren.PushBack(AutoPtr<IView>(v));
}

void ViewGroup::FinishAnimatingView(
    /* [in] */ IView* view,
    /* [in] */ IAnimation* animation)
{
    assert(view != NULL);
    View* v = VIEW_PROBE(view);

    if (!mDisappearingChildren.IsEmpty()) {
        List<AutoPtr<IView> >::Iterator it = Find(
            mDisappearingChildren.Begin(), mDisappearingChildren.End(), AutoPtr<IView>(view));
        if (it != mDisappearingChildren.End()) {
            if (v->mAttachInfo != NULL) {
                v->DispatchDetachedFromWindow();
            }

            v->ClearAnimation();
            mGroupFlags |= FLAG_INVALIDATE_REQUIRED;

            mDisappearingChildren.Erase(it);
        }
    }


    if (animation != NULL) {
        Boolean result;
        animation->GetFillAfter(&result);
        if (!result) {
            v->ClearAnimation();
        }
    }

    if ((v->mPrivateFlags & PFLAG_ANIMATION_STARTED) == PFLAG_ANIMATION_STARTED) {
        v->OnAnimationEnd();
        // Should be performed by onAnimationEnd() but this avoid an infinite loop,
        // so we'd rather be safe than sorry
        v->mPrivateFlags &= ~PFLAG_ANIMATION_STARTED;
        // Draw one more frame after the animation is done
        mGroupFlags |= FLAG_INVALIDATE_REQUIRED;
    }
}

Boolean ViewGroup::IsViewTransitioning(
    /* [in] */ IView* view)
{
    Boolean contains = FALSE;
    List<AutoPtr<IView> >::Iterator ator = mTransitioningViews.Begin();
    for (; ator != mTransitioningViews.End(); ++ator) {
        if (ator->Get() == view) {
            contains = TRUE;
        }
    }

    return contains;
}

ECode ViewGroup::StartViewTransition(
    /* [in] */ IView* view)
{
    View* v = VIEW_PROBE(view);
    if (v->mParent == THIS_PROBE(IViewParent)) {
        mTransitioningViews.PushBack(view);
    }

    return NOERROR;
}

ECode ViewGroup::EndViewTransition(
    /* [in] */ IView* view)
{
    mTransitioningViews.Remove(view);
    List<AutoPtr<IView> >::Iterator ator = mDisappearingChildren.Begin();
    Boolean contains = FALSE;
    for (; ator != mDisappearingChildren.End(); ++ator) {
        if (ator->Get() == view) {
            contains = TRUE;
            break;
        }
    }

    if (contains) {
        mDisappearingChildren.Remove(view);

        contains = FALSE;
        ator = mVisibilityChangingChildren.Begin();
        for (; ator != mVisibilityChangingChildren.End(); ++ator) {
            if (ator->Get() == view) {
                contains = TRUE;
                break;
            }
        }

        if (contains) {
            mVisibilityChangingChildren.Remove(view);
        }
        else {
            View* v = VIEW_PROBE(view);
            if (v->mAttachInfo != NULL) {
                v->DispatchDetachedFromWindow();
            }
            if (v->mParent != NULL) {
                v->mParent = NULL;
            }
        }

        Invalidate();
    }

    return NOERROR;
}

Boolean ViewGroup::GatherTransparentRegion(
    /* [in, out] */ IRegion* region)
{
    // If no transparent regions requested, we are always opaque.
    const Boolean meOpaque = (mPrivateFlags & View::PFLAG_REQUEST_TRANSPARENT_REGIONS) == 0;
    if (meOpaque && region == NULL) {
        // The caller doesn't care about the region, so stop now.
        return TRUE;
    }
    View::GatherTransparentRegion(region);
    Boolean noneOfTheChildrenAreTransparent = TRUE;
    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* v = VIEW_PROBE((*mChildren)[i]);
        if ((v->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE || v->GetAnimation() != NULL) {
            if (!v->GatherTransparentRegion(region)) {
                noneOfTheChildrenAreTransparent = FALSE;
            }
        }
    }
    return meOpaque || noneOfTheChildrenAreTransparent;
}

ECode ViewGroup::RequestTransparentRegion(
    /* [in] */ IView* child)
{
    if (child != NULL) {
        View* v = VIEW_PROBE(child);
        v->mPrivateFlags |= View::PFLAG_REQUEST_TRANSPARENT_REGIONS;
        if (mParent != NULL) {
            mParent->RequestTransparentRegion(IVIEW_PROBE(this));
        }
    }
    return NOERROR;
}

Boolean ViewGroup::FitSystemWindows(
    /* [in] */ IRect* insets)
{
    Boolean done = View::FitSystemWindows(insets);
    if (!done) {
        for (Int32 i = 0; i < mChildrenCount; i++) {
            View* v = VIEW_PROBE((*mChildren)[i]);
            done = v->FitSystemWindows(insets);
            if (done) {
                break;
            }
        }
    }
    return done;
}

AutoPtr<IAnimationListener> ViewGroup::GetLayoutAnimationListener()
{
    return mAnimationListener;
}

ECode ViewGroup::DrawableStateChanged()
{
    View::DrawableStateChanged();

    if ((mGroupFlags & FLAG_NOTIFY_CHILDREN_ON_DRAWABLE_STATE_CHANGE) != 0) {
        if ((mGroupFlags & FLAG_ADD_STATES_FROM_CHILDREN) != 0) {
//            throw new IllegalStateException("addStateFromChildren cannot be enabled if a"
//                    + " child has duplicateParentState set to TRUE");
            return E_ILLEGAL_STATE_EXCEPTION;
        }

        for (Int32 i = 0; i < mChildrenCount; i++) {
            View* v = VIEW_PROBE((*mChildren)[i]);
            if ((v->mViewFlags & DUPLICATE_PARENT_STATE) != 0) {
                v->RefreshDrawableState();
            }
        }
    }
    return NOERROR;
}

ECode ViewGroup::JumpDrawablesToCurrentState()
{
    View::JumpDrawablesToCurrentState();
    for (Int32 i = 0; i < mChildrenCount; i++) {
        (*mChildren)[i]->JumpDrawablesToCurrentState();
    }

    return NOERROR;
}

ECode ViewGroup::OnCreateDrawableState(
    /* [in] */ Int32 extraSpace,
    /* [out] */ ArrayOf<Int32>** drawableState)
{
    VALIDATE_NOT_NULL(drawableState);
    *drawableState = NULL;

    if ((mGroupFlags & FLAG_ADD_STATES_FROM_CHILDREN) == 0) {
        return View::OnCreateDrawableState(extraSpace, drawableState);
    }

    Int32 need = 0;
    Int32 n = GetChildCount();
    for (Int32 i = 0; i < n; i++) {
        View* v = VIEW_PROBE(GetChildAt(i));
        AutoPtr<ArrayOf<Int32> > childState = v->GetDrawableState();

        if (childState != NULL) {
            need += childState->GetLength();
        }
    }

    AutoPtr<ArrayOf<Int32> > state;
    ASSERT_SUCCEEDED(View::OnCreateDrawableState(extraSpace + need, (ArrayOf<Int32>**)&state));

    for (Int32 i = 0; i < n; i++) {
        View* v = VIEW_PROBE(GetChildAt(i));
        AutoPtr<ArrayOf<Int32> > childState = v->GetDrawableState();

        if (childState != NULL) {
            MergeDrawableStates(state, childState);
        }
    }

    *drawableState = state;
    REFCOUNT_ADD(*drawableState);
    return NOERROR;
}

ECode ViewGroup::SetAddStatesFromChildren(
    /* [in] */ Boolean addsStates)
{
    if (addsStates) {
        mGroupFlags |= FLAG_ADD_STATES_FROM_CHILDREN;
    }
    else {
        mGroupFlags &= ~FLAG_ADD_STATES_FROM_CHILDREN;
    }

    return RefreshDrawableState();
}

Boolean ViewGroup::AddStatesFromChildren()
{
    return (mGroupFlags & FLAG_ADD_STATES_FROM_CHILDREN) != 0;
}

ECode ViewGroup::ChildDrawableStateChanged(
    /* [in] */ IView* child)
{
    if ((mGroupFlags & FLAG_ADD_STATES_FROM_CHILDREN) != 0) {
        return RefreshDrawableState();
    }
    return NOERROR;
}

ECode ViewGroup::SetLayoutAnimationListener(
    /* [in] */ IAnimationListener* animationListener)
{
    mAnimationListener = animationListener;
    return NOERROR;
}

ECode ViewGroup::RequestTransitionStart(
    /* [in] */ ILayoutTransition* transition)
{
    ViewRootImpl* viewAncestor = GetViewRootImpl();
    if (viewAncestor != NULL) {
        viewAncestor->RequestTransitionStart(transition);
    }

    return NOERROR;
}

ECode ViewGroup::ResolveRtlPropertiesIfNeeded()
{
    View::ResolveRtlPropertiesIfNeeded();
    Int32 count = GetChildCount();
    for (Int32 i = 0; i < count; i++) {
        View* child = VIEW_PROBE(GetChildAt(i));
        if (child->IsLayoutDirectionInherited()) {
            child->ResolveRtlPropertiesIfNeeded();
        }
    }

    return NOERROR;
}

Boolean ViewGroup::ResolveLayoutDirection()
{
    Boolean result = View::ResolveLayoutDirection();
    if (result) {
        Int32 count = GetChildCount();
        for (Int32 i = 0; i < count; i++) {
            View* child = VIEW_PROBE(GetChildAt(i));
            if (child->IsLayoutDirectionInherited()) {
                child->ResolveLayoutDirection();
            }
        }
    }

    return result;
}

Boolean ViewGroup::ResolveTextDirection()
{
    Boolean result = View::ResolveTextDirection();
    if (result) {
        Int32 count = GetChildCount();
        for (Int32 i = 0; i < count; i++) {
            View* child = VIEW_PROBE(GetChildAt(i));
            if (child->IsTextDirectionInherited()) {
                child->ResolveTextDirection();
            }
        }
    }
    return result;
}

Boolean ViewGroup::ResolveTextAlignment()
{
    Boolean result = View::ResolveTextAlignment();
    if (result) {
        Int32 count = GetChildCount();
        for (Int32 i = 0; i < count; i++) {
            View* child = VIEW_PROBE(GetChildAt(i));
            if (child->IsTextAlignmentInherited()) {
                child->ResolveTextAlignment();
            }
        }
    }
    return result;
}

ECode ViewGroup::ResolvePadding()
{
    View::ResolvePadding();
    Int32 count = GetChildCount();
    for (Int32 i = 0; i < count; i++) {
        View* child = VIEW_PROBE(GetChildAt(i));
        if (child->IsLayoutDirectionInherited()) {
            child->ResolvePadding();
        }
    }

    return NOERROR;
}

void ViewGroup::ResolveDrawables()
{
    View::ResolveDrawables();
    Int32 count = GetChildCount();
    for (Int32 i = 0; i < count; i++) {
        View* child = VIEW_PROBE(GetChildAt(i));
        if (child->IsLayoutDirectionInherited()) {
            child->ResolveDrawables();
        }
    }
}

ECode ViewGroup::ResolveLayoutParams()
{
    View::ResolveLayoutParams();
    Int32 count = GetChildCount();
    for (Int32 i = 0; i < count; i++) {
        GetChildAt(i)->ResolveLayoutParams();
    }

    return NOERROR;
}

ECode ViewGroup::ResetResolvedLayoutDirection()
{
    View::ResetResolvedLayoutDirection();

    Int32 count = GetChildCount();
    for (Int32 i = 0; i < count; i++) {
        View* child = VIEW_PROBE(GetChildAt(i));
        if (child->IsLayoutDirectionInherited()) {
            child->ResetResolvedLayoutDirection();
        }
    }

    return NOERROR;
}

ECode ViewGroup::ResetResolvedTextDirection()
{
    View::ResetResolvedTextDirection();

    Int32 count = GetChildCount();
    for (Int32 i = 0; i < count; i++) {
        View* child = VIEW_PROBE(GetChildAt(i));
        if (child->IsTextDirectionInherited()) {
            child->ResetResolvedTextDirection();
        }
    }

    return NOERROR;
}

ECode ViewGroup::ResetResolvedTextAlignment()
{
    View::ResetResolvedTextAlignment();

    Int32 count = GetChildCount();
    for (Int32 i = 0; i < count; i++) {
        View* child = VIEW_PROBE(GetChildAt(i));
        if (child->IsTextAlignmentInherited()) {
            child->ResetResolvedTextAlignment();
        }
    }

    return NOERROR;
}

ECode ViewGroup::ResetResolvedPadding()
{
    View::ResetResolvedPadding();

    Int32 count = GetChildCount();
    for (Int32 i = 0; i < count; i++) {
        View* child = VIEW_PROBE(GetChildAt(i));
        if (child->IsLayoutDirectionInherited()) {
            child->ResetResolvedPadding();
        }
    }

    return NOERROR;
}

void ViewGroup::ResetResolvedDrawables()
{
    View::ResetResolvedDrawables();

    Int32 count = GetChildCount();
    for (Int32 i = 0; i < count; i++) {
        View* child = VIEW_PROBE(GetChildAt(i));
        if (child->IsLayoutDirectionInherited()) {
            child->ResetResolvedDrawables();
        }
    }
}

Boolean ViewGroup::ShouldDelayChildPressedState()
{
    return TRUE;
}

void ViewGroup::OnSetLayoutParams(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* layoutParams)
{
}

ECode ViewGroup::Init(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(View::Init(context));
    InitViewGroup();
    return NOERROR;
}

ECode ViewGroup::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(View::Init(context, attrs));
    InitViewGroup();
    InitFromAttributes(context, attrs);
    return NOERROR;
}

ECode ViewGroup::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(View::Init(context, attrs, defStyle));
    InitViewGroup();
    InitFromAttributes(context, attrs);
    return NOERROR;
}

AutoPtr<IPaint> ViewGroup::GetDebugPaint()
{
    if (sDebugPaint == NULL) {
        CPaint::New((IPaint**)&sDebugPaint);
        sDebugPaint->SetAntiAlias(FALSE);
    }

    return sDebugPaint;
}

AutoPtr<ArrayOf<Float> > ViewGroup::GetDebugLines(
    /* [in] */ Int32 x1,
    /* [in] */ Int32 y1,
    /* [in] */ Int32 x2,
    /* [in] */ Int32 y2)
{
    if (sDebugLines== NULL) {
        sDebugLines = ArrayOf<Float>::Alloc(16);
    }

    x2--;
    y2--;

    (*sDebugLines)[0] = x1;
    (*sDebugLines)[1] = y1;
    (*sDebugLines)[2] = x2;
    (*sDebugLines)[3] = y1;

    (*sDebugLines)[4] = x2;
    (*sDebugLines)[5] = y1;
    (*sDebugLines)[6] = x2;
    (*sDebugLines)[7] = y2 + 1;

    (*sDebugLines)[8] = x2 + 1;
    (*sDebugLines)[9] = y2;
    (*sDebugLines)[10] = x1;
    (*sDebugLines)[11] = y2;

    (*sDebugLines)[12]  = x1;
    (*sDebugLines)[13]  = y2;
    (*sDebugLines)[14] = x1;
    (*sDebugLines)[15] = y1;

    return sDebugLines;
}

}   //namespace View
}   //namespace Droid
}   //namespace Elastos
