
#include "elastos/droid/view/ViewGroup.h"
#include "elastos/droid/view/CViewGroupLayoutParams.h"
#include "elastos/droid/view/CMotionEvent.h"
#include "elastos/droid/view/ViewRootImpl.h"
#include "elastos/droid/view/FocusFinder.h"
#include "elastos/droid/view/animation/Animation.h"
#include "elastos/droid/view/animation/AnimationUtils.h"
#include "elastos/droid/view/animation/CTransformation.h"
#include "elastos/droid/view/animation/CLayoutAnimationController.h"
#include "elastos/droid/view/animation/CTransformation.h"
#include "elastos/droid/view/animation/LayoutAnimationController.h"
#include "elastos/droid/animation/CLayoutTransition.h"
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/graphics/CPointF.h"
#include "elastos/droid/os/Build.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/os/SystemProperties.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/Math.h>
#include "elastos/droid/R.h"

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
using Elastos::Utility::ILocale;
using Elastos::Utility::Logging::Logger;

using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace View {

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

const Int32 ViewGroup::FLAG_LAYOUT_MODE_WAS_EXPLICITLY_SET;
const Int32 ViewGroup::FLAG_IS_TRANSITION_GROUP;
const Int32 ViewGroup::FLAG_IS_TRANSITION_GROUP_SET;
const Int32 ViewGroup::FLAG_TOUCHSCREEN_BLOCKS_FOCUS;
const Int32 ViewGroup::LAYOUT_MODE_UNDEFINED;

Int32 ViewGroup::LAYOUT_MODE_DEFAULT = LAYOUT_MODE_CLIP_BOUNDS;

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

Boolean ViewGroup::DEBUG_DRAW = FALSE;

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
    if (mHost->mLayoutCalledWhileSuppressed && !(transition->IsChangingLayout(&changing), changing)) {
        mHost->RequestLayout();
        mHost->mLayoutCalledWhileSuppressed = FALSE;
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
{
}

AutoPtr<ViewGroup::ChildListForAccessibility> ViewGroup::ChildListForAccessibility::Obtain(
    /* [in] */ IViewGroup* parent,
    /* [in] */ Boolean sort)
{
    AutoPtr<ChildListForAccessibility> list = sPool->Acquire();;
    if (sPool == NULL) {
        list = new ChildListForAccessibility();
    }

    list->Init(parent, sort);
    return list;
}

void ViewGroup::ChildListForAccessibility::Recycle()
{
    Clear();

    sPool->Release(this);
}

Int32 ViewGroup::ChildListForAccessibility::GetChildCount()
{
    Int32 result;
    mChildren->GetSize(&result);
    return result;
}

AutoPtr<IView> ViewGroup::ChildListForAccessibility::GetChildAt(
    /* [in] */ Int32 index)
{
    AutoPtr<IInterface> temp;
    mChildren->Get(index, (IInterface**)&temp);
    AutoPtr<IView> result = IView::Probe(temp);
    return result;
}

Int32 ViewGroup::ChildListForAccessibility::GetChildIndex(
    /* [in] */ IView* child)
{
    Int32 result;
    mChildren->IndexOf(child, &result);
    return result;
}

void ViewGroup::ChildListForAccessibility::Init(
    /* [in] */ IViewGroup* parent,
    /* [in] */ Boolean sort)
{
    Int32 childCount = parent->GetChildCount();
    for (Int32 i = 0; i < childCount; i++) {
        AutoPtr<IView> child = parent->GetChildAt(i);
        mChildren->Add((IInterface*)child->Probe(EIID_IInterface));
    }
    if (sort) {
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IInterface> temp;
            mChildren->Get(index, (IInterface**)&temp);
            AutoPtr<IView> child = IView::Probe(temp);
            AutoPtr<ViewLocationHolder> holder = ViewLocationHolder::Obtain(parent, child);
            mHolders->Add((IInterface*)holder->Probe(EIID_IInterface));
        }
        Sort(mHolders);
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IInterface> temp, view;
            mHolders->Get(index, (IInterface**)&temp);
            AutoPtr<ViewLocationHolder> holder = ViewLocationHolder::Probe(temp);
            view = (IInterface*)holder->mView->Probe(EIID_IInterface);
            mChildren->Set(i, view);
            holder->Recycle();
        }
        mHolders->Clear();
    }
}

ECode ViewGroup::ChildListForAccessibility::Sort(
    /* [in] */ IList* holders)
{
    // This is gross but the least risky solution. The current comparison
    // strategy breaks transitivity but produces very good results. Coming
    // up with a new strategy requires time which we do not have, so ...
    //try {
        ECode ec = ViewLocationHolder::SetComparisonStrategy(
                ViewLocationHolder::COMPARISON_STRATEGY_STRIPE);
        ECode other = Collections::Sort(holders);
        if (ec == E_ILLEGAL_ARGUMENT_EXCEPTION || other == E_ILLEGAL_ARGUMENT_EXCEPTION) {
            ViewLocationHolder::SetComparisonStrategy(
                ViewLocationHolder::COMPARISON_STRATEGY_LOCATION);
            Collections::Sort(holders);
        }
    //} catch (IllegalArgumentException iae) {
        // Note that in practice this occurs extremely rarely in a couple
        // of pathological cases.
    //}
}

void ViewGroup::ChildListForAccessibility::Clear()
{
    mChildren.Clear();
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
    AutoPtr<ViewLocationHolder> holder = sPool->Acquire();
    if (sPool == NULL) {
        holder = new ViewLocationHolder();
    }
    holder->Init(root, view);
    return holder;
}

void ViewGroup::ViewLocationHolder::SetComparisonStrategy(
    /* [in] */ Int32 strategy)
{
    sComparisonStrategy = strategy;
}

void ViewGroup::ViewLocationHolder::Recycle()
{
    Clear();
    sPool->Release(this);
}

Int32 ViewGroup::ViewLocationHolder::CompareTo(
    /* [in] */ ViewLocationHolder* another)
{
    // This instance is greater than an invalid argument.
    if (another == NULL) {
        return 1;
    }

    if (sComparisonStrategy == COMPARISON_STRATEGY_STRIPE) {
        // First is above second.
        if (mLocation->mBottom - another->mLocation->mTop <= 0) {
            return -1;
        }
        // First is below second.
        if (mLocation->mTop - another->mLocation->mBottom >= 0) {
            return 1;
        }
    }

    // We are ordering left-to-right, top-to-bottom.
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
    mView = View::Probe(view);
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
    , mLayoutMode(LAYOUT_MODE_UNDEFINED)
    , mChildrenCount(0)
    , mSuppressLayout(FALSE)
    , mLayoutCalledWhileSuppressed(FALSE)
    , mChildCountWithTransientState(0)
{
    CTransformation::New((ITransformation**)&mChildTransformation);
    CPointF::New((IPointF**)&mLocalPoint);

    mLayoutTransitionListener = new LayoutTransitionListener(this);
}

ECode ViewGroup::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode ViewGroup::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, 0);
}

ECode ViewGroup::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode ViewGroup::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    View::constructor(context, attrs, defStyleAttr, defStyleRes);
    InitViewGroup();
    InitFromAttributes(context, attrs, defStyleAttr, defStyleRes);
    return NOERROR;
}

Boolean ViewGroup::DebugDraw()
{
    return DEBUG_DRAW || mAttachInfo != NULL && mAttachInfo->mDebugLayout;
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
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
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
            case R.styleable.ViewGroup_layoutMode: {
                Int32 temp;
                a->GetInt32(attr, LAYOUT_MODE_UNDEFINED, &temp);
                SetLayoutMode(temp);
                break;
            }
            case R.styleable.ViewGroup_transitionGroup: {
                Boolean value = FALSE;
                a->GetBoolean(attr, FALSE, &value);
                SetTransitionGroup(value);
                break;
            }
            case R.styleable.ViewGroup_touchscreenBlocksFocus:
            {
                Boolean value = FALSE;
                a->GetBoolean(attr, FALSE, &value);
                SetTouchscreenBlocksFocus(value);
                break;
            }
            default:
                break;
        }
    }

    a->Recycle();
}

ECode ViewGroup::GetDescendantFocusability(
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(res)
    *res = mGroupFlags & FLAG_MASK_FOCUSABILITY;
    return NOERROR;
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
        (View::Probe(mFocused))->UnFocus(THIS_PROBE(IView));
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
    View::UnFocus(focused);

    // We had a previous notion of who had focus. Clear it.
    if (mFocused.Get() != child) {
        if (mFocused != NULL) {
            (View::Probe(mFocused))->UnFocus(focused);
        }

        mFocused = child;
    }

    if (mParent != NULL) {
        mParent->RequestChildFocus(IView::Probe(this), focused);
    }
    return NOERROR;
}

ECode ViewGroup::FocusableViewAvailable(
    /* [in] */ IView* v)
{
    Int32 descendantFocusability;
    GetDescendantFocusability(&descendantFocusability);
    Boolean isFocused, isFocusableInTouchMode;
    IsFocused(&isFocused);
    IsFocusableInTouchMode(&isFocusableInTouchMode);

    if (mParent != NULL
        // shortcut: don't report a new focusable view if we block our descendants from
        // getting focus
        && (descendantFocusability != FOCUS_BLOCK_DESCENDANTS)
        && (isFocusableInTouchMode || !ShouldBlockFocusForTouchscreen())
        // shortcut: don't report a new focusable view if we already are focused
        // (and we don't prefer our descendants)
        //
        // note: knowing that mFocused is non-NULL is not a good enough reason
        // to break the traversal since in that case we'd actually have to find
        // the focused view and make sure it wasn't FOCUS_AFTER_DESCENDANTS and
        // an ancestor of v; this will get checked for at ViewAncestor
        && !(isFocused && descendantFocusability != FOCUS_AFTER_DESCENDANTS)) {
        mParent->FocusableViewAvailable(v);
    }
    return NOERROR;
}

ECode ViewGroup::ShowContextMenuForChild(
    /* [in] */ IView* originalView,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if (mParent != NULL) {
        mParent->ShowContextMenuForChild(originalView, res);
    }

    return NOERROR;
}

ECode ViewGroup::StartActionModeForChild(
    /* [in] */ IView* originalView,
    /* [in] */ IActionModeCallback* callback,
    /* [out] */ IActionMode** res)
{
    VALIDATE_NOT_NULL(res)
    if (mParent != NULL) {
        mParent->StartActionModeForChild(originalView, callback, res);
        return NOERROR;
    }

    *res = NULL;
    return NOERROR;
}

ECode ViewGroup::FocusSearch(
    /* [in] */ IView* focused,
    /* [in] */ Int32 direction,
    /* [out] */ IView** res)
{
    VALIDATE_NOT_NULL(res)
    if (IsRootNamespace()) {
        // root namespace means we should consider ourselves the top of the
        // tree for focus searching; otherwise we could be focus searching
        // into other tabs.  see LocalActivityManager and TabHost for more info
        //
        FocusFinder::GetInstance()->FindNextFocus(
            THIS_PROBE(IViewGroup), focused, direction, res);
    }
    else if (mParent != NULL) {
        mParent->FocusSearch(focused, direction, res);
    }

    return NOERROR;
}

ECode ViewGroup::RequestChildRectangleOnScreen(
    /* [in] */ IView* child,
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = FALSE;
    return NOERROR;
}

ECode ViewGroup::RequestSendAccessibilityEvent(
    /* [in] */ IView* child,
    /* [in] */ IAccessibilityEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    AutoPtr<IViewParent> parent = mParent;
    if (parent == NULL) {
        *res = FALSE;
        return NOERROR;
    }
    Boolean propagate = OnRequestSendAccessibilityEvent(child, event);
    if (!propagate) {
        *res = FALSE;
        return NOERROR;
    }
    parent->RequestSendAccessibilityEvent(IView::Probe(this), event, res);
    return NOERROR;
}

ECode ViewGroup::OnRequestSendAccessibilityEvent(
    /* [in] */ IView* child,
    /* [in] */ IAccessibilityEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if (mAccessibilityDelegate != NULL) {
        mAccessibilityDelegate->OnRequestSendAccessibilityEvent(
                THIS_PROBE(IViewGroup), child, event, res);
        return NOERROR;
    }
    else {
        *res = OnRequestSendAccessibilityEventInternal(child, event);
        return NOERROR;
    }
}

Boolean ViewGroup::OnRequestSendAccessibilityEventInternal(
    /* [in] */ IView* child,
    /* [in] */ IAccessibilityEvent* event)
{
    return TRUE;
}

/**
 * Translates the given bounds and intersections from child coordinates to
 * local coordinates. In case any interactive sibling of the calling child
 * covers the latter, a new intersections is added to the intersection list.
 * This method is for the exclusive use by the accessibility layer to compute
 * a point where a sequence of down and up events would click on a view.
 *
 * @param child The child making the call.
 * @param bounds The bounds to translate in child coordinates.
 * @param intersections The intersections of interactive views covering the child.
 * @return True if the bounds and intersections were computed, false otherwise.
 */
ECode ViewGroup::TranslateBoundsAndIntersectionsInWindowCoordinates(
    /* [in] */ IView* child,
    /* [in] */ IRectF* bounds,
    /* [in] */ IList* intersections,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    // Not attached, done.
    if (mAttachInfo == NULL) {
        *res = FALSE;
        return NOERROR;
    }

    Float alpha, transitionAlpha;
    Int32 visibility;
    if ((GetAlpha(&alpha), alpha) <= 0 || (GetTransitionAlpha(&transitionAlpha), transitionAlpha) <= 0
        || (GetVisibility(&visibility), visibility) != IView::VISIBLE) {
        // Cannot click on a view with an invisible predecessor.
        *res = FALSE;
        return NOERROR;
    }

    // Compensate for the child transformation.
    Boolean hasIdentityMatrix;
    (View::Probe(child))->HasIdentityMatrix(&hasIdentityMatrix);
    if (!hasIdentityMatrix) {
        AutoPtr<IMatrix> matrix;
        child->GetMatrix((IMatrix**)&matrix);
        matrix->MapRect(bounds, &hasIdentityMatrix);

        Int32 intersectionCount;
        intersections->GetSize(&intersectionCount);

        for (Int32 i = 0; i < intersectionCount; i++)
        {
            AutoPtr<IInterface> temp;
            intersections->Get(i, (IInterface**)&temp);
            AutoPtr<IRectF> intersection = IRectF::Probe(temp);
            matrix->MapRect(intersection, &hasIdentityMatrix);
        }
    }

    // Translate the bounds from child to parent coordinates.
    View* temp = View::Probe(child);
    Int32 dx = temp->mLeft - mScrollX;
    Int32 dy = temp->mTop - mScrollY;
    bounds->Offset(dx, dy);
    OffsetRects(intersections, dx, dy);

    // If the bounds do not intersect our bounds, done.
    Int32 width, height;
    GetWidth(&width);
    GetHeight(&height);
    Boolean result;
    bounds->Intersects(0, 0, width, height, &result);
    if (!result) {
        *res = FALSE;
        return NOERROR;
    }

    // Check whether any clickable siblings cover the child
    // view and if so keep track of the intersections. Also
    // respect Z ordering when iterating over children.
    AutoPtr<IList> orderedList = BuildOrderedChildList();
    Boolean useCustomOrder = orderedList == NULL
            && IsChildrenDrawingOrderEnabled();

    Int32 childCount = mChildrenCount;
    for (Int32 i = childCount - 1; i >= 0; i--) {
        Int32 childIndex = useCustomOrder
                ? GetChildDrawingOrder(childCount, i) : i;
        AutoPtr<IView> sibling = (orderedList == NULL)
        if (orderedList == NULL) {
            sibling = (*mChildren)[childIndex];
        } else {
            AutoPtr<IInterface> temp;
            orderedList->Get(childIndex, (IInterface**)&temp);
            sibling = IView::Probe(temp);
        }

        // We care only about siblings over the child.
        if (child == sibling) {
            break;
        }

        // Ignore invisible views as they are not interactive.
        Int32 siblingVisibility;
        sibling->GetVisibility(&siblingVisibility);
        if (siblingVisibility != IView::VISIBLE) {
            continue;
        }

        // If sibling is not interactive we do not care.
        Boolean isClickable, isLongClickable;
        sibling->IsClickable(&isClickable);
        sibling->IsLongClickable(&isLongClickable);
        if (!isClickable && !isLongClickable) {
            continue;
        }

        // Compute the sibling bounds in its coordinates.
        AutoPtr<IRectF> siblingBounds = mAttachInfo->mTmpTransformRect1;
        Int32 siblingW, siblingH;
        sibling->GetWidth(&siblingW);
        sibling->GetHeight(&siblingH);
        siblingBounds->Set(0, 0, siblingW, siblingH);

        // Take into account the sibling transformation matrix.
        Boolean siblingHasIdentityMatrix;
        sibling->HasIdentityMatrix(&siblingHasIdentityMatrix);
        if (!siblingHasIdentityMatrix) {
            AutoPtr<IMatrix> matrix;
            sibling->GetMatrix((IMatrix**)&matrix);
            Boolean result;
            matrix->MapRect(siblingBounds, &result);
        }

        // Offset the sibling to our coordinates.
        View* siblingTemp = View::Probe(sibling);
        Int32 siblingDx = siblingTemp->mLeft - mScrollX;
        Int32 siblingDy = siblingTemp->mTop - mScrollY;
        siblingBounds->Offset(siblingDx, siblingDy);

        // Compute the intersection between the child and the sibling.
        Boolean siblingBoundsIntersect;
        siblingBounds->Intersect(bounds, &siblingBoundsIntersect);
        if (siblingBoundsIntersect) {
            // If an interactive sibling completely covers the child, done.
            siblingBounds->Equals(bounds, &siblingBoundsIntersect)
            if (siblingBoundsIntersect) {
                *res = FALSE;
                return NOERROR;
            }
            // Keep track of the intersection rectangle.
            AutoPtr<IRectF> intersection;
            CRectF::New(siblingBounds, (IRectF**)&intersection);
            intersections->Add((IInterface*)intersection->Probe(EIID_IInterface));
        }
    }

    if (IViewGroup::Probe(mParent)) {
        AutoPtr<ViewGroup> parentGroup = (ViewGroup*)IViewGroup::Probe(mParent);
        *res = parentGroup->TranslateBoundsAndIntersectionsInWindowCoordinates(
                THIS_PROBE(IView), bounds, intersections);
        return NOERROR;
    }
    *res = TRUE;
    return NOERROR;
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

ECode ViewGroup::HasTransientState(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = mChildCountWithTransientState > 0 || View::HasTransientState();
    return NOERROR;
}

ECode ViewGroup::DispatchUnhandledMove(
     /* [in] */ IView* focused,
     /* [in] */ Int32 direction,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if (mFocused != NULL) {
        mFocused->DispatchUnhandledMove(focused, direction, res);
    }

    return NOERROR;
}

ECode ViewGroup::ClearChildFocus(
    /* [in] */ IView* child)
{
    if (DBG) {
        Logger::D(VG_TAG, /*this + */" clearChildFocus()");
    }

    mFocused = NULL;
    if (mParent != NULL) {
        mParent->ClearChildFocus(IView::Probe(this));
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

ECode ViewGroup::UnFocus(
    /* [in] */ IView* focused)
{
    if (mFocused == NULL) {
        View::UnFocus(focused);
    }
    else {
        (View::Probe(mFocused))->UnFocus(focused);
        mFocused = NULL;
    }

    return NOERROR;
}

ECode ViewGroup::GetFocusedChild(
    /* [out] */ IView** res)
{
    VALIDATE_NOT_NULL(res)
    *res = mFocused;
    REFCOUNT_ADD(*res)
    return NOERROR;
}

ECode ViewGroup::GetDeepestFocusedChild(
    /* [out] */ IView** focused)
{
    VALIDATE_NOT_NULL(focused)
    AutoPtr<IView> v = THIS_PROBE(IView);
    while (v != NULL) {
        Boolean res;
        v->IsFocused(&res);
        if (res) {
            *focused = v;
            REFCOUNT_ADD(*focused);
            return NOERROR;
        }
        if (IViewGroup::Probe(v)) {
            (IViewGroup::Probe(v))->GetFocusedChild((IView**)&v);
        } else {
            v = NULL;
        }
    }
    *focused = NULL;
    return NOERROR;
}

ECode ViewGroup::HasFocus(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = (mPrivateFlags & PFLAG_FOCUSED) != 0 || mFocused != NULL;
    return NOERROR;
}

ECode ViewGroup::FindFocus(
    /* [out] */ IView** focused)
{
    VALIDATE_NOT_NULL(res)

    if (IsFocused()) {
        *res = IView::Probe(this);
        REFCOUNT_ADD(*res)
        return NOERROR;
    }

    if (mFocused != NULL) {
        *res = (View::Probe(mFocused))->FindFocus();
        REFCOUNT_ADD(*res)
        return NOERROR;
    }
    *res = NULL;
    return NOERROR;
}

ECode ViewGroup::HasFocusable(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if ((mViewFlags & VISIBILITY_MASK) != IView::VISIBLE) {
        *res = FALSE;
        return NOERROR;
    }

    if (IsFocusable()) {
        *res = TRUE;
        return NOERROR;
    }

    if (GetDescendantFocusability() != FOCUS_BLOCK_DESCENDANTS) {
        for (Int32 i = 0; i < mChildrenCount; i++) {
            Boolean hasFocusable = FALSE;
            (*mChildren)[i]->HasFocusable(&hasFocusable);
            if (hasFocusable) {
                *res = TRUE;
                return NOERROR;
            }
        }
    }

    *res = FALSE;
    return NOERROR;
}

ECode ViewGroup::AddFocusables(
    /* [in] */ IList* views,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 focusableMode)
{
    assert(views);

    Int32 focusableCount;
    views->GetObjectCount(&focusableCount);

    Int32 descendantFocusability = GetDescendantFocusability();
    if (descendantFocusability != FOCUS_BLOCK_DESCENDANTS) {
        if (ShouldBlockFocusForTouchscreen()) {
            focusableMode |= FOCUSABLES_TOUCH_MODE;
        }
        for (Int32 i = 0; i < mChildrenCount; i++) {
            View* temp = View::Probe((*mChildren)[i]);
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
    views->GetSize(&size);
    Boolean isFocusableInTouchMode;
    IsFocusableInTouchMode(&isFocusableInTouchMode);
    if ((descendantFocusability != FOCUS_AFTER_DESCENDANTS
            // No focusable descendants
            || (focusableCount == size)) &&
            (isFocusableInTouchMode || !ShouldBlockFocusForTouchscreen())) {
            View::AddFocusables(views, direction, focusableMode);
    }

    return NOERROR;
}

/**
 * Set whether this ViewGroup should ignore focus requests for itself and its children.
 * If this option is enabled and the ViewGroup or a descendant currently has focus, focus
 * will proceed forward.
 *
 * @param touchscreenBlocksFocus true to enable blocking focus in the presence of a touchscreen
 */
ECode ViewGroup::SetTouchscreenBlocksFocus(
    /* [in] */ Boolean touchscreenBlocksFocus)
{
    if (touchscreenBlocksFocus) {
        mGroupFlags |= FLAG_TOUCHSCREEN_BLOCKS_FOCUS;
        if (HasFocus()) {
            AutoPtr<IView> focusedChild;
            GetDeepestFocusedChild((IView**)&focusedChild);
            Boolean isFocusableInTouchMode;
            focusedChild->IsFocusableInTouchMode(&isFocusableInTouchMode);
            if (!isFocusableInTouchMode) {
                AutoPtr<IView> newFocus = FocusSearch(FOCUS_FORWARD);
                if (newFocus != NULL) {
                    newFocus->RequestFocus();
                }
            }
        }
    } else {
        mGroupFlags &= ~FLAG_TOUCHSCREEN_BLOCKS_FOCUS;
    }
}

/**
 * Check whether this ViewGroup should ignore focus requests for itself and its children.
 */
ECode ViewGroup::getTouchscreenBlocksFocus(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = (mGroupFlags & FLAG_TOUCHSCREEN_BLOCKS_FOCUS) != 0;
    return NOERROR;
}

Boolean ViewGroup::ShouldBlockFocusForTouchscreen()
{
    Boolean touchscreenBlocksFocus, hasSystemFeature;
    GetTouchscreenBlocksFocus(&touchscreenBlocksFocus);
    AutoPtr<IPackageManager> pmg;
    mContext0->GetPackageManager((IPackageManager**)&pmg);
    pmg->HasSystemFeature(IPackageManager::FEATURE_TOUCHSCREEN, &hasSystemFeature);
    return touchscreenBlocksFocus && hasSystemFeature;
}

ECode ViewGroup::FindViewsWithText(
    /* [in, out] */ IObjectContainer* outViews,
    /* [in] */ ICharSequence* searched,
    /* [in] */ Int32 flags)
{
    View::FindViewsWithText(outViews, searched, flags);
    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* child = View::Probe((*mChildren)[i]);
        if ((child->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE
                && (child->mPrivateFlags & PFLAG_IS_ROOT_NAMESPACE) == 0) {
            child->FindViewsWithText(outViews, searched, flags);
        }
    }

    return NOERROR;
}

ECode ViewGroup::FindViewByAccessibilityIdTraversal(
    /* [in] */ Int32 accessibilityId,
    /* [out] */ IView** focused)
{
    VALIDATE_NOT_NULL(res)
    AutoPtr<IView> foundView = View::FindViewByAccessibilityIdTraversal(accessibilityId);
    if (foundView != NULL) {
        *res = foundView;
        REFCOUNT_ADD(*res)
        return NOERROR;
    }

    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* child = View::Probe((*mChildren)[i]);
        foundView = child->FindViewByAccessibilityIdTraversal(accessibilityId);
        if (foundView != NULL) {
           *res = foundView;
           REFCOUNT_ADD(*res)
           return NOERROR;
        }
    }
    *res = NULL;
    return NOERROR;
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
        View* temp = View::Probe((*mChildren)[i]);
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

ECode ViewGroup::DispatchVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    View::DispatchVisibilityChanged(changedView, visibility);

    for (Int32 i = 0; i < mChildrenCount; i++) {
        View::Probe((*mChildren)[i])->DispatchVisibilityChanged(
            changedView, visibility);
    }
    return NOERROR;
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
            return mParent->RecomputeViewAttributes(IView::Probe(this));
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
            View* child = View::Probe((*mChildren)[i]);
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
        View::Probe(child)->mParent =
            THIS_PROBE(IViewParent);
        RequestLayout();
        Invalidate();
    }
    return NOERROR;
}

AutoPtr<IPointF> ViewGroup::GetLocalPoint()
{
    if (mLocalPoint == NULL) {
        CPointF::New((IPointF**)&mLocalPoint);
    }
    return mLocalPoint;
}

ECode ViewGroup::DispatchDragEvent(
    /* [in] */ IDragEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    Boolean retval = FALSE;
    Float tx = 0.0f;
    event->GetX(&tx);
    Float ty = 0.0f;
    event->GetY(&ty);

    ViewRootImpl* root = GetViewRootImpl();

    // Dispatch down the view hierarchy
    AutoPtr<IPointF> localPoint = GetLocalPoint();

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
            View* child = View::Probe((*mChildren)[i]);
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
                View* child = View::Probe(iter->Get());
                assert(child != NULL);
                // If a child was notified about an ongoing drag, it's told that it's over
                child->DispatchDragEvent(event);
                child->mPrivateFlags2 &= ~DRAG_MASK;
                child->RefreshDrawableState();
            }

            mDragNotifiedChildren.Clear();
            if (mCurrentDrag != NULL) {
                mCurrentDrag->Recycle();
                mCurrentDrag = NULL;
            }
        }

        // We consider drag-ended to have been handled if one of our children
        // had offered to handle the drag.
        if (mChildAcceptsDrag) {
            retval = TRUE;
        }
    } break;

    case IDragEvent::ACTION_DRAG_LOCATION: {
        // Find the [possibly new] drag target
        View* target = View::Probe(FindFrontmostDroppableChildAt(tx, ty, localPoint));

        // If we've changed apparent drag target, tell the view root which view
        // we're over now [for purposes of the eventual drag-recipient-changed
        // notifications to the framework] and tell the new target that the drag
        // has entered its bounds.  The root will see setDragFocus() calls all
        // the way down to the final leaf view that is handling the LOCATION event
        // before reporting the new potential recipient to the framework.
        if (mCurrentDragView != target) {
            root->SetDragFocus(IView::Probe(target));

            // If we've dragged off of a child view, send it the EXITED message
            if (mCurrentDragView != NULL) {
                View* view = View::Probe(mCurrentDragView);
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
            localPoint->GetX(&tmp);
            event->SetX(tmp);
            localPoint->GetY(&tmp);
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
        View* target = View::Probe(FindFrontmostDroppableChildAt(tx, ty, localPoint));
        if (target != NULL) {
            Float tmp = 0.0f;
            localPoint->GetX(&tmp);
            event->SetX(tmp);
            localPoint->GetY(&tmp);
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

    *res = retval;
    return NOERROR;
}

AutoPtr<IView> ViewGroup::FindFrontmostDroppableChildAt(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPointF* outLocalPoint)
{
    for (Int32 i = mChildrenCount - 1; i >= 0; i--) {
        AutoPtr<IView> child = (*mChildren)[i];
        if (!View::Probe(child)->CanAcceptDrag()) {
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
        View* _child = View::Probe(child);
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
        View* child = View::Probe((*mChildren)[i]);
        changed |= child->UpdateLocalSystemUiVisibility(localValue, localChanges);
    }

    return changed;
}

ECode ViewGroup::DispatchKeyEventPreIme(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if ((mPrivateFlags & (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS)) == (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS)) {
        View::DispatchKeyEventPreIme(event, res);
        return NOERROR;
    }
    else if (mFocused != NULL) {
        View* focused = View::Probe(mFocused);
        assert(focused);
        if ((focused->mPrivateFlags & PFLAG_HAS_BOUNDS) == PFLAG_HAS_BOUNDS) {
            focused->DispatchKeyEventPreIme(event, res);
            return NOERROR;
        }
    }

    *res = FALSE;
    return NOERROR;
}

ECode ViewGroup::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnKeyEvent(event, 1);
    }

    if ((mPrivateFlags & (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS))
            == (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS)) {
        if (View::DispatchKeyEvent(event)) {
            *res = TRUE;
            return NOERROR;
        }
    }
    else if (mFocused != NULL && (View::Probe(mFocused)->mPrivateFlags & PFLAG_HAS_BOUNDS)
            == PFLAG_HAS_BOUNDS) {
        if (View::Probe(mFocused)->DispatchKeyEvent(event)) {
            *res = TRUE;
            return NOERROR;
        }
    }

    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnUnhandledEvent(event, 1);
    }

    *res = FALSE;
    return NOERROR;
}

ECode ViewGroup::DispatchKeyShortcutEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if ((mPrivateFlags & (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS)) == (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS)) {
        View::DispatchKeyShortcutEvent(event, res);
        return NOERROR;
    }
    else if (mFocused != NULL) {
        View* focused = View::Probe(mFocused);
        assert(focused);
        if ((focused->mPrivateFlags & PFLAG_HAS_BOUNDS) == PFLAG_HAS_BOUNDS) {
            focused->DispatchKeyShortcutEvent(event, res);
            return NOERROR;
        }
    }

    *res = FALSE;
    return NOERROR;
}

ECode ViewGroup::DispatchTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnTrackballEvent(event, 1);
    }

    if ((mPrivateFlags & (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS))
            == (PFLAG_FOCUSED | PFLAG_HAS_BOUNDS)) {
        if (View::DispatchTrackballEvent(event)) {
            *res = TRUE;
            return NOERROR;
        }
    }
    else if (mFocused != NULL && (View::Probe(mFocused)->mPrivateFlags & PFLAG_HAS_BOUNDS)
            == PFLAG_HAS_BOUNDS) {
        if (View::Probe(mFocused)->DispatchTrackballEvent(event)) {
            *res = TRUE;
            return NOERROR;
        }
    }

    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnUnhandledEvent(event, 1);
    }

    *res = FALSE;
    return NOERROR;
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
            AutoPtr<IList> preorderedList = BuildOrderedChildList();
            Boolean customOrder = preorderedList == NULL && IsChildrenDrawingOrderEnabled();

            AutoPtr<HoverTarget> lastHoverTarget;
            for (Int32 i = mChildrenCount - 1; i >= 0; i--) {
                Int32 childIndex = customOrder ? GetChildDrawingOrder(mChildrenCount, i) : i;
                AutoPtr<IView> child;
                if (preorderedList == NULL) {
                    child = (*mChildren)[childIndex];
                } else {
                    AutoPtr<IInterface> temp;
                    preorderedList->Get(childIndex, (IInterface**)&temp);
                    child = IView::Probe(temp);
                }

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
                    mFirstHoverTarget = hoverTarget;
                }
                lastHoverTarget = hoverTarget;

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
            if (preorderedList != NULL) preorderedList->Clear();
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
            (View::Probe(view))->DispatchHoverEvent(event.Get());
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
        View* child = View::Probe(children->GetChildAt(i));
        if ((child->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE) {
            if (child->IncludeForAccessibility()) {
                childrenForAccessibility->Add(IView::Probe(child));
            }
            else {
                child->AddChildrenForAccessibility(childrenForAccessibility);
            }
        }
    }
    children->Recycle();

    return NOERROR;
}

ECode ViewGroup::OnInterceptHoverEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = FALSE;
    return NOERROR;
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

        AutoPtr<IList> preorderedList = BuildOrderedChildList();
        const Boolean customOrder = preorderedList == NULL && IsChildrenDrawingOrderEnabled();

        for (Int32 i = mChildrenCount - 1; i >= 0; i--) {
            Int32 childIndex = customOrder ? GetChildDrawingOrder(mChildrenCount, i) : i;
            AutoPtr<IView> child;
            if (preorderedList == NULL) {
                child = (*mChildren)[childIndex];
            } else {
                AutoPtr<IInterface> temp;
                preorderedList->Get(childIndex, (IInterface*)&temp);
                child = IView::Probe(temp);
            }
            if (!CanViewReceivePointerEvents(child)
                    || !IsTransformedTouchPointInView(x, y, child, NULL)) {
                continue;
            }

            if (DispatchTransformedGenericPointerEvent(event, child)) {
                if (preorderedList != NULL) preorderedList->Clear();
                return TRUE;
            }
        }
        if (preorderedList != NULL) preorderedList->Clear();
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
    else if (mFocused != NULL && ((View::Probe(mFocused))->mPrivateFlags & PFLAG_HAS_BOUNDS)
            == PFLAG_HAS_BOUNDS) {
        return (View::Probe(mFocused))->DispatchGenericMotionEvent(event);
    }
    return FALSE;
}

Boolean ViewGroup::DispatchTransformedGenericPointerEvent(
    /* [in] */ IMotionEvent* event,
    /* [in] */ IView* _child)
{
    View* child = View::Probe(_child);
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

ECode ViewGroup::DispatchTouchEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
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
                if (newTouchTarget == NULL && childrenCount != 0) {
                    // Find a child that can receive the event.
                    // Scan children from front to back.
                    Float x = 0.0f;
                    ev->GetX(actionIndex, &x);
                    Float y = 0.0f;
                    ev->GetY(actionIndex, &y);

                    AutoPtr<IList> preorderedList = BuildOrderedChildList();
                    Boolean customOrder = preorderedList == NULL && IsChildrenDrawingOrderEnabled();

                    for (Int32 i = childrenCount - 1; i >= 0; i--) {
                        Int32 childIndex = customOrder ?
                                GetChildDrawingOrder(childrenCount, i) : i;
                        AutoPtr<IView> child;
                        if (preorderedList == NULL) {
                            child = (*mChildren)[childIndex];
                        } else {
                            AutoPtr<IInterface> temp;
                            preorderedList->Get(childIndex, (IInterface**)&temp);
                            child = IView::Probe(temp);
                        }
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

                        ResetCancelNextUpFlag(View::Probe(child));
                        if (DispatchTransformedTouchEvent(ev, FALSE, child, idBitsToAssign)) {
                            // Child wants to receive touch within its bounds.
                            ev->GetDownTime(&mLastTouchDownTime);

                            if (preorderedList != NULL) {
                                // childIndex points into presorted list, find original index
                                for (Int32 j = 0; j < childrenCount; j++) {
                                    if ((*mChildren)[childIndex] == (*mChildren)[j]) {
                                        mLastTouchDownIndex = j;
                                        break;
                                    }
                                }
                            } else {
                                mLastTouchDownIndex = childIndex;
                            }
                            ev->GetX(&mLastTouchDownX);
                            ev->GetY(&mLastTouchDownY);
                            newTouchTarget = AddTouchTarget(child, idBitsToAssign);
                            alreadyDispatchedToNewTouchTarget = TRUE;
                            break;
                        }
                    }
                    if (preorderedList != NULL) preorderedList->Clear();
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
                    Boolean cancelChild = ResetCancelNextUpFlag(View::Probe(target->mChild))
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

    *res = handled;
    return NOERROR;
}

void ViewGroup::ResetTouchState()
{
    ClearTouchTargets();
    ResetCancelNextUpFlag(this);
    mGroupFlags &= ~FLAG_DISALLOW_INTERCEPT;
    mNestedScrollAxes = IView::SCROLL_AXIS_NONE;
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
            ResetCancelNextUpFlag(View::Probe(target->mChild));
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
    View* _child = View::Probe(child);
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
    View* _child = View::Probe(child);
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
        _child = View::Probe(child);
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

ECode ViewGroup::IsMotionEventSplittingEnabled(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = (mGroupFlags & FLAG_SPLIT_MOTION_EVENTS) == FLAG_SPLIT_MOTION_EVENTS;
    return NOERROR;
}

/**
 * Returns true if this ViewGroup should be considered as a single entity for removal
 * when executing an Activity transition. If this is false, child elements will move
 * individually during the transition.
 * @return True if the ViewGroup should be acted on together during an Activity transition.
 * The default value is false when the background is null and true when the background
 * is not null or if {@link #getTransitionName()} is not null.
 */
ECode ViewGroup::IsTransitionGroup(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if ((mGroupFlags & FLAG_IS_TRANSITION_GROUP_SET) != 0) {
        *res = ((mGroupFlags & FLAG_IS_TRANSITION_GROUP) != 0);
        return NOERROR;
    } else {
        AutoPtr<IDrawable> background;
        GetBackground((IDrawable**)&background);
        String str;
        getTransitionName(&str);

        *res = (background != NULL || str.IsNullOrEmpty());
        return NOERROR;
    }
}

/**
 * Changes whether or not this ViewGroup should be treated as a single entity during
 * Activity Transitions.
 * @param isTransitionGroup Whether or not the ViewGroup should be treated as a unit
 *                          in Activity transitions. If false, the ViewGroup won't transition,
 *                          only its children. If true, the entire ViewGroup will transition
 *                          together.
 * @see android.app.ActivityOptions#makeSceneTransitionAnimation(android.app.Activity,
 * android.util.Pair[])
 */
ECode ViewGroup::SetTransitionGroup(
    /* [in] */ Boolean isTransitionGroup)
{
    mGroupFlags |= FLAG_IS_TRANSITION_GROUP_SET;
    if (isTransitionGroup) {
        mGroupFlags |= FLAG_IS_TRANSITION_GROUP;
    } else {
        mGroupFlags &= ~FLAG_IS_TRANSITION_GROUP;
    }
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
ECode ViewGroup::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = FALSE;
    return NOERROR;
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
ECode ViewGroup::RequestFocus(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if (DBG) {
        Logger::D(VG_TAG,
            "0x%08x ViewGroup.requestFocus direction = %d", this, direction);
    }

    Int32 descendantFocusability = GetDescendantFocusability();

    switch (descendantFocusability) {
    case FOCUS_BLOCK_DESCENDANTS:
        View::RequestFocus(direction, previouslyFocusedRect, res);
        return NOERROR;
    case FOCUS_BEFORE_DESCENDANTS:
        {
            Boolean took = View::RequestFocus(direction, previouslyFocusedRect);
            if (took) {
                *res = took;
            } else {
                OnRequestFocusInDescendants(direction, previouslyFocusedRect, res);
            }
            return NOERROR;
        }
    case FOCUS_AFTER_DESCENDANTS:
        {
            Boolean took = OnRequestFocusInDescendants(
                direction, previouslyFocusedRect);
            if (took) {
                *res = took
            } else {
                View::RequestFocus(direction, previouslyFocusedRect, res);
            }
            return NOERROR;
        }
    default:
        {
            Logger::E(VG_TAG, "descendant focusability must be "
                "one of FOCUS_BEFORE_DESCENDANTS, FOCUS_AFTER_DESCENDANTS"
                ", FOCUS_BLOCK_DESCENDANTS but is %d", descendantFocusability);

        }
    }
    *res = FALSE;
    return NOERROR;
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
        View* child = View::Probe((*mChildren)[i]);
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

ECode ViewGroup::DispatchAttachedToWindow(
    /* [in] */ AttachInfo* info,
    /* [in] */ Int32 visibility)
{
    mGroupFlags |= FLAG_PREVENT_DISPATCH_ATTACHED_TO_WINDOW;
    View::DispatchAttachedToWindow(info, visibility);
    mGroupFlags &= ~FLAG_PREVENT_DISPATCH_ATTACHED_TO_WINDOW;

    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* child = View::Probe((*mChildren)[i]);
        child->DispatchAttachedToWindow(info,
                visibility | (child->mViewFlags&VISIBILITY_MASK));
    }
    return NOERROR;
}

//@Override
void ViewGroup::DispatchScreenStateChanged(
    /* [in] */ Int32 screenState)
{
    View::DispatchScreenStateChanged(screenState);

    for (Int32 i = 0; i < mChildrenCount; i++) {
        (View::Probe((*mChildren)[i]))->DispatchScreenStateChanged(screenState);
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
        View* child = View::Probe(children->GetChildAt(i));
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
            info->AddChildUnchecked(child);
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

//@Override
ECode ViewGroup::NotifySubtreeAccessibilityStateChanged(
    /* [in] */ IView* child,
    /* [in] */ IView* source,
    /* [in] */ Int32 changeType)
{
    // If this is a live region, we should send a subtree change event
    // from this view. Otherwise, we can let it propagate up.
    Int32 accessibilityLiveRegion = 0;
    GetAccessibilityLiveRegion(&accessibilityLiveRegion);
    if (accessibilityLiveRegion != IView::ACCESSIBILITY_LIVE_REGION_NONE) {
        NotifyViewAccessibilityStateChangedIfNeeded(changeType);
    } else if (mParent != NULL) {
        //try {
        ECode ec = mParent->NotifySubtreeAccessibilityStateChanged(THIS_PROBE(IView), source, changeType);
        //} catch (AbstractMethodError e) {
        //    Log.e(VIEW_LOG_TAG, mParent.getClass().getSimpleName() +
        //            " does not fully implement ViewParent", e);
        //}
        return ec;
    }
    return NOERROR;
}

//@Override
ECode ViewGroup::ResetSubtreeAccessibilityStateChanged()
{
    View::ResetSubtreeAccessibilityStateChanged();
    for (Int32 i = 0; i < mChildrenCount; i++) {
        AutoPtr<IView> child = (*mChildren)[i];
        child->ResetSubtreeAccessibilityStateChanged();
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
    mLayoutCalledWhileSuppressed = FALSE;

    // Tear down our drag tracking
    mDragNotifiedChildren.Clear();
    if (mCurrentDrag != NULL) {
        mCurrentDrag->Recycle();
        mCurrentDrag = NULL;
    }

    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* tmp = View::Probe((*mChildren)[i]);
        FAIL_RETURN(tmp->DispatchDetachedFromWindow());
    }
    ClearDisappearingChildren();
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
        View* c = View::Probe((*mChildren)[i]);
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
        View* c = View::Probe((*mChildren)[i]);
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
            View* child = View::Probe((*mChildren)[i]);
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

/** Return true if this ViewGroup is laying out using optical bounds. */
Boolean ViewGroup::IsLayoutModeOptical()
{
    return mLayoutMode == LAYOUT_MODE_OPTICAL_BOUNDS;
}

AutoPtr<IInsets> ViewGroup::ComputeOpticalInsets()
{
    if (IsLayoutModeOptical()) {
        Int32 left = 0, top = 0, right = 0, bottom = 0;
        for (Int32 i = 0; i < mChildrenCount; i++) {
            AutoPtr<IView> child = GetChildAt(i);
            Int32 visibility;
            child->GetVisibility(&visibility);
            if (visibility == IView::VISIBLE) {
                AutoPtr<IInsets> insets;
                child->GetOpticalInsets((IInsets**)&insets);
                Insets* temp = Insets::Probe(insets);
                left = Elastos::Core::Math::Max(left, temp->mLeft);
                top = Elastos::Core::Math::Max(top,    temp->mTop);
                right = Elastos::Core::Math::Max(right,  temp->mRight);
                bottom = Elastos::Core::Math::Max(bottom, temp->mBottom);
            }
        }
        return Insets::Of(left, top, right, bottom);
    } else {
        return Insets::NONE;
    }
}

void ViewGroup::FillRect(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint,
    /* [in] */ Int32 x1,
    /* [in] */ Int32 y1,
    /* [in] */ Int32 x2,
    /* [in] */ Int32 y2)
{
    if (x1 != x2 && y1 != y2) {
        if (x1 > x2) {
            Int32 tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        if (y1 > y2) {
            Int32 tmp = y1;
            y1 = y2;
            y2 = tmp;
        }
        canvas->DrawRect(x1, y1, x2, y2, paint);
    }
}

Int32 ViewGroup::Sign(
    /* [in] */ Int32 x)
{
    return (x >= 0) ? 1 : -1;
}

void ViewGroup::DrawCorner(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint,
    /* [in] */ Int32 x1,
    /* [in] */ Int32 y1,
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy,
    /* [in] */ Int32 lw)
{
    FillRect(canvas, paint, x1, y1, x1 + dx, y1 + lw * Sign(dy));
    FillRect(canvas, paint, x1, y1, x1 + lw * Sign(dx), y1 + dy);
}

Int32 ViewGroup::DipsToPixels(
    /* [in] */ Int32 dips)
{
    AutoPtr<IResources> res;
    GetContext()->GetResources((IResources**)&res);
    AutoPtr<IDisplayMetrics> metrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    Float scale;
    metrics->GetDensity(&scale);
    return (Int32) (dips * scale + 0.5f);
}

void ViewGroup::DrawRectCorners(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Int32 x1,
    /* [in] */ Int32 y1,
    /* [in] */ Int32 x2,
    /* [in] */ Int32 y2,
    /* [in] */ IPaint* paint,
    /* [in] */ Int32 lineLength,
    /* [in] */ Int32 lineWidth)
{
    DrawCorner(canvas, paint, x1, y1, lineLength, lineLength, lineWidth);
    DrawCorner(canvas, paint, x1, y2, lineLength, -lineLength, lineWidth);
    DrawCorner(canvas, paint, x2, y1, -lineLength, lineLength, lineWidth);
    DrawCorner(canvas, paint, x2, y2, -lineLength, -lineLength, lineWidth);
}

void ViewGroup::FillDifference(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Int32 x2,
    /* [in] */ Int32 y2,
    /* [in] */ Int32 x3,
    /* [in] */ Int32 y3,
    /* [in] */ Int32 dx1,
    /* [in] */ Int32 dy1,
    /* [in] */ Int32 dx2,
    /* [in] */ Int32 dy2,
    /* [in] */ IPaint* paint)
{
    Int32 x1 = x2 - dx1;
    Int32 y1 = y2 - dy1;

    Int32 x4 = x3 + dx2;
    Int32 y4 = y3 + dy2;

    FillRect(canvas, paint, x1, y1, x4, y2);
    FillRect(canvas, paint, x1, y2, x2, y3);
    FillRect(canvas, paint, x3, y2, x4, y3);
    FillRect(canvas, paint, x1, y3, x4, y4);
}

void ViewGroup::DrawRect(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint,
    /* [in] */ Int32 x1,
    /* [in] */ Int32 y1,
    /* [in] */ Int32 x2,
    /* [in] */ Int32 y2)
{
    if (sDebugLines== null) {
        // TODO: This won't work with multiple UI threads in a single process
        sDebugLines = ArrayOf<Float>::Alloc(16);
    }

    (*sDebugLines)[0] = x1;
    (*sDebugLines)[1] = y1;
    (*sDebugLines)[2] = x2;
    (*sDebugLines)[3] = y1;

    (*sDebugLines)[4] = x2;
    (*sDebugLines)[5] = y1;
    (*sDebugLines)[6] = x2;
    (*sDebugLines)[7] = y2;

    (*sDebugLines)[8] = x2;
    (*sDebugLines)[9] = y2;
    (*sDebugLines)[10] = x1;
    (*sDebugLines)[11] = y2;

    (*sDebugLines)[12] = x1;
    (*sDebugLines)[13] = y2;
    (*sDebugLines)[14] = x1;
    (*sDebugLines)[15] = y1;

    canvas->DrawLines(sDebugLines, paint);
}

void ViewGroup::OnDebugDrawMargins(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    for (Int32 i = 0; i < GetChildCount(); i++) {
        AutoPtr<IView> c = GetChildAt(i);
        assert(c != NULL);
        AutoPtr<IViewGroupLayoutParams> params;
        c->GetLayoutParams((IViewGroupLayoutParams**)&params);
        params->OnDebugDraw(c, canvas, paint);
    }
}

void ViewGroup::OnDebugDraw(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IPaint> paint = GetDebugPaint();

    // Draw optical bounds
    {
        paint->SetColor(IColor::RED);
        paint->SetStyle(PaintStyle_STROKE);

        for (Int32 i = 0; i < GetChildCount(); i++) {
            View* c = View::Probe(GetChildAt(i));
            AutoPtr<IInsets> insets = c->GetOpticalInsets();
            Int32 l, r, t, b;
            insets->GetLeft(&l);
            insets->GetRight(&r);
            insets->GetTop(&t);
            insets->GetBottom(&b);
            DrawRect(canvas, paint,
                c->GetLeft() + l, c->GetTop() + t,
                c->GetRight() - r - 1, c->GetBottom() - b - 1);
        }
    }

    // Draw margins
    {
        Int32 marginsColor;
        CColor::Argb(63, 255, 0, 255, &marginsColor);
        paint->SetColor(marginsColor);
        paint->SetStyle(PaintStyle_FILL);

        OnDebugDrawMargins(canvas, paint);
    }

    // Draw clip bounds
    {
        Int32 clipColor;
        CColor::Rgb(63, 127, 255, &clipColor);
        paint->SetColor(clipColor);
        paint->SetStyle(PaintStyle_FILL);

        Int32 lineLength = DipsToPixels(8);
        Int32 lineWidth = DipsToPixels(1);
        for (Int32 i = 0; i < GetChildCount(); i++) {
            View* c = View::Probe(GetChildAt(i));
            DrawRectCorners(canvas, c.getLeft(), c.getTop(), c.getRight(), c.getBottom(),
                    paint, lineLength, lineWidth);
        }
    }
}

void ViewGroup::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    Boolean usingRenderNodeProperties;
    canvas->IsRecordingFor((IInterface*)mRenderNode->Probe(EIID_IInterface), &usingRenderNodeProperties);
    Int32 childrenCount = mChildrenCount;
    AutoPtr<ArrayOf<IView*> > children = mChildren;
    Int32 flags = mGroupFlags;

    if ((flags & FLAG_RUN_ANIMATION) != 0 && CanAnimate()) {
        const Boolean cache = (mGroupFlags & FLAG_ANIMATION_CACHE) == FLAG_ANIMATION_CACHE;
        Boolean buildCache = !IsHardwareAccelerated();

        for (Int32 i = 0; i < childrenCount; i++) {
            View* child = View::Probe((*children)[i]);
            if ((child->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE) {
                const AutoPtr<IViewGroupLayoutParams> params = child->GetLayoutParams();
                AttachLayoutAnimationParameters(IView::Probe(child), params, i, childrenCount);
                BindLayoutAnimation(IView::Probe(child));
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

    Int32 clipSaveCount = 0;
    const Boolean clipToPadding = (flags & CLIP_TO_PADDING_MASK) == CLIP_TO_PADDING_MASK;
    if (clipToPadding) {
        canvas->Save(&clipSaveCount);
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

    if (usingRenderNodeProperties) canvas->InsertReorderBarrier();
    // Only use the preordered list if not HW accelerated, since the HW pipeline will do the
    // draw reordering internally
    AutoPtr<IList> preorderedList = usingRenderNodeProperties
            ? NULL : BuildOrderedChildList();
    Boolean customOrder = preorderedList == NULL && IsChildrenDrawingOrderEnabled();
    for (Int32 i = 0; i < childrenCount; i++) {
        Int32 childIndex = customOrder ? GetChildDrawingOrder(childrenCount, i) : i;
        AutoPtr<IView> child;
        if (preorderedList == null) {
            child = (*children)[childIndex];
        } else {
            AutoPtr<IInterface> temp;
            preorderedList->Get(childIndex, (IInterface**)&temp);
            child = IView::Probe(temp);
        }
        if ((View::Probe(child)->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE || child->GetAnimation() != NULL) {
            more |= DrawChild(canvas, child, drawingTime);
        }
    }
    if (preorderedList != NULL) preorderedList->Clear();

    // Draw any disappearing views that have animations
    List<AutoPtr<IView> >::ReverseIterator rit = mDisappearingChildren.RBegin();
    // Go backwards -- we may delete as animations finish
    for (; rit != mDisappearingChildren.REnd(); ++rit) {
        more |= DrawChild(canvas, *rit, drawingTime);
    }

    if (usingRenderNodeProperties) canvas->InsertInorderBarrier();

    if (DebugDraw()) {
        OnDebugDraw(canvas);
    }

    if (clipToPadding) {
        canvas->RestoreToCount(clipSaveCount);
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
 * Returns the ViewGroupOverlay for this view group, creating it if it does
 * not yet exist. In addition to {@link ViewOverlay}'s support for drawables,
 * {@link ViewGroupOverlay} allows views to be added to the overlay. These
 * views, like overlay drawables, are visual-only; they do not receive input
 * events and should not be used as anything other than a temporary
 * representation of a view in a parent container, such as might be used
 * by an animation effect.
 *
 * <p>Note: Overlays do not currently work correctly with {@link
 * SurfaceView} or {@link TextureView}; contents in overlays for these
 * types of views may not display correctly.</p>
 *
 * @return The ViewGroupOverlay object for this view.
 * @see ViewGroupOverlay
 */
//@Override
ECode ViewGroup::GetOverlay(
    /* [out] */ IViewGroupOverlay** overlay)
{
    VALIDATE_NOT_NULL(overlay)
    if (mOverlay == NULL) {
        AutoPtr<IViewGroupOverlay> temp;
        ViewGroupOverlay::New((IViewGroupOverlay**)&temp);
        temp->constructor(mContext, THIS_PROBE(IView));
        mOverlay = *overlay = temp;
        REFCOUNT_ADD(*overlay)
    }
    return NOERROR;
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

Boolean ViewGroup::HasChildWithZ()
{
    for (Int32 i = 0; i < mChildrenCount; i++) {
        Float z;
        (*mChildren)[i]->GetZ(&z);
        if (z != 0) return TRUE;
    }
    return FALSE;
}

/**
 * Populates (and returns) mPreSortedChildren with a pre-ordered list of the View's children,
 * sorted first by Z, then by child drawing order (if applicable).
 *
 * Uses a stable, insertion sort which is commonly O(n) for ViewGroups with very few elevated
 * children.
 */
AutoPtr<IList> ViewGroup::BuildOrderedChildList()
{
    Int32 count = mChildrenCount;
    if (count <= 1 || !HasChildWithZ()) return NULL;

    if (mPreSortedChildren == NULL) {
        CArrayList::New(count, (IArrayList**)&mPreSortedChildren);
    } else {
        mPreSortedChildren->EnsureCapacity(count);
    }

    Boolean useCustomOrder = IsChildrenDrawingOrderEnabled();
    for (Int32 i = 0; i < mChildrenCount; i++) {
        // add next child (in child order) to end of list
        Int32 childIndex = useCustomOrder ? GetChildDrawingOrder(mChildrenCount, i) : i;
        AutoPtr<IView> nextChild = (*mChildren)[childIndex];
        float currentZ;
        nextChild->GetZ(&currentZ);

        // insert ahead of any Views with greater Z
        Int32 insertIndex = i;
        Float z;
        AutoPtr<IInterface> temp;
        mPreSortedChildren->Get(insertIndex - 1, (IInterface**)&temp);
        (View::Probe(temp))->GetZ(&z);
        while (insertIndex > 0 && z > currentZ) {
            insertIndex--;
            mPreSortedChildren->Get(insertIndex - 1, (IInterface**)&temp);
            (View::Probe(temp))->GetZ(&z);
        }
        mPreSortedChildren->Add(insertIndex, (IInterface*)nextChild->Probe(EIID_IInterface));
    }
    return mPreSortedChildren;
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
        View* child = View::Probe((*mChildren)[i]);
        if (((child->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE || child->GetAnimation() != NULL) &&
                child->HasStaticLayer()) {
            recreateChildDisplayList(child);
        }
    }
    if (mOverlay != NULL) {
        AutoPtr<IView> overlayView = mOverlay->GetOverlayView();
        RecreateChildDisplayList(overlayView);
    }
    if (mDisappearingChildren != NULL) {
        AutoPtr<IList> disappearingChildren = mDisappearingChildren;
        Int32 disappearingCount;
        disappearingChildren->GetSize(&disappearingCount);
        for (Int32 i = 0; i < disappearingCount; ++i) {
            AutoPtr<IInterface> temp;
            disappearingChildren->Get(i, (IInterface**)&temp);
            AutoPtr<IView> child = IView::Probe(temp);
            RecreateChildDisplayList(child);
        }
    }
}

void ViewGroup::RecreateChildDisplayList(
    /* [in] */ IView* child)
{
    View* temp = View::Probe(child);
    temp->mRecreateDisplayList = (temp->mPrivateFlags & PFLAG_INVALIDATED) == PFLAG_INVALIDATED;
    temp->mPrivateFlags &= ~PFLAG_INVALIDATED;
    AutoPtr<IRenderNode> node;
    temp->GetDisplayList((IRenderNode**)&node);
    temp->mRecreateDisplayList = FALSE;
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
    return (View::Probe(child))->Draw(canvas, THIS_PROBE(IViewGroup), drawingTime);
}

/**
 * Returns whether this group's children are clipped to their bounds before drawing.
 * The default value is true.
 * @see #setClipChildren(boolean)
 *
 * @return True if the group's children will be clipped to their bounds,
 * false otherwise.
 */
//@ViewDebug.ExportedProperty(category = "drawing")
ECode ViewGroup::GetClipChildren(
        /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = ((mGroupFlags & FLAG_CLIP_CHILDREN) != 0);
    return NOERROR;
}

/**
  * By default, children are clipped to their bounds before drawing. This
  * allows view groups to override this behavior for animations, etc.
  *
  * @param clipChildren TRUE to clip children to their bounds,
  *        FALSE otherwise
  * @attr ref android.R.styleable#ViewGroup_clipChildren
  */
ECode ViewGroup::SetClipChildren(
    /* [in] */ Boolean clipChildren)
{
    Boolean previousValue = (mGroupFlags & FLAG_CLIP_CHILDREN) == FLAG_CLIP_CHILDREN;
    if (clipChildren != previousValue) {
        SetBooleanFlag(FLAG_CLIP_CHILDREN, clipChildren);
        for (Int32 i = 0; i < mChildrenCount; ++i) {
            View* child = View::Probe(GetChildAt(i));
            if (child->mRenderNode != NULL) {
                child->mRenderNode->SetClipToBounds(clipChildren);
            }
        }
        Invalidate(TRUE);
    }
    return NOERROR;
}

/**
  * By default, children are clipped to the padding of the ViewGroup. This
  * allows view groups to override this behavior
  *
  * @param clipToPadding TRUE to clip children to the padding of the
  *        group, FALSE otherwise
  * @attr ref android.R.styleable#ViewGroup_clipToPadding
  */
ECode ViewGroup::SetClipToPadding(
    /* [in] */ Boolean clipToPadding)
{
    if (HasBooleanFlag(FLAG_CLIP_TO_PADDING) != clipToPadding) {
        SetBooleanFlag(FLAG_CLIP_TO_PADDING, clipToPadding);
        Invalidate(TRUE);
    }
    return NOERROR;
}

/**
 * Check if this ViewGroup is configured to clip child views to its padding.
 *
 * @return true if this ViewGroup clips children to its padding, false otherwise
 *
 * @attr ref android.R.styleable#ViewGroup_clipToPadding
 */
//@ViewDebug.ExportedProperty(category = "drawing")
ECode ViewGroup::GetClipToPadding(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = HasBooleanFlag(FLAG_CLIP_TO_PADDING);
    return NOERROR;
}

/**
  * {@inheritDoc}
  */
//@Override
ECode ViewGroup::DispatchSetSelected(
    /* [in] */ Boolean selected)
{
    for (Int32 i = 0; i < mChildrenCount; i++) {
        (*mChildren)[i]->SetSelected(selected);
    }
    return NOERROR;
}

ECode ViewGroup::DispatchSetActivated(
    /* [in] */ Boolean activated)
{
    for (Int32 i = 0; i < mChildrenCount; i++) {
        (*mChildren)[i]->SetActivated(activated);
    }
    return NOERROR;
}

//@Override
void ViewGroup::DispatchSetPressed(
    /* [in] */ Boolean pressed)
{
    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* child = View::Probe((*mChildren)[i]);
        // Children that are clickable on their own should not
        // show a pressed state when their parent view does.
        // Clearing a pressed state always propagates.
        if (!pressed || (!child->IsClickable() && !child->IsLongClickable())) {
            child->SetPressed(pressed);
        }
    }
}

//@Override
void ViewGroup::DispatchCancelPendingInputEvents()
{
    View::DispatchCancelPendingInputEvents();
    for (Int32 i = 0; i < mChildrenCount; i++) {
        (*mChildren)[i]->DispatchCancelPendingInputEvents();
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

AutoPtr<ITransformation> ViewGroup::GetChildTransformation()
{
    if (mChildTransformation == NULL) {
        CTransformation::New((ITransformation**)&mChildTransformation);
    }
    return mChildTransformation;
}

AutoPtr<IView> ViewGroup::FindViewTraversal(
    /* [in] */ Int32 id)
{
    if (id == mID) {
        return AutoPtr<IView>(IView::Probe(this));
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
       return IView::Probe(this);
   }

   for (Int32 i = 0; i < mChildrenCount; i++) {
       View* v = View::Probe((*mChildren)[i]);

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
        View* v = View::Probe((*mChildren)[i]);

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

void ViewGroup::ClearCachedLayoutMode()
{
    if (!HasBooleanFlag(FLAG_LAYOUT_MODE_WAS_EXPLICITLY_SET)) {
       mLayoutMode = LAYOUT_MODE_UNDEFINED;
    }
}

//@Override
void ViewGroup::OnAttachedToWindow()
{
    View::OnAttachedToWindow();
    ClearCachedLayoutMode();
}

//@Override
void ViewGroup::OnDetachedFromWindow()
{
    View::OnDetachedFromWindow();
    ClearCachedLayoutMode();
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
    View* v = View::Probe(child);
    v->mParent = NULL;
    AddViewInner(child, index, params, preventRequestLayout);
    v->mPrivateFlags = (v->mPrivateFlags & ~PFLAG_DIRTY_MASK) | PFLAG_DRAWN;
    return TRUE;
}


void ViewGroup::CleanupLayoutState(
    /* [in] */ IView* child)
{
    View* v = View::Probe(child);
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
    View* v = View::Probe(child);

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

    Int32 lastVisibility;
    child->GetVisibility(&lastVisibility);
    if (lastVisibility != IView::GONE) {
        NotifySubtreeAccessibilityStateChangedIfNeeded();
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
        (View::Probe(v))->mParent = NULL;
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
            (View::Probe((*mChildren)[i]))->mParent = NULL;
            mChildren->Set(i, NULL);
        }
    }
    else {
        for (Int32 i = start; i < end; i++) {
            (View::Probe((*mChildren)[i]))->mParent = NULL;
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
        animationParams = new LayoutAnimationController::AnimationParameters();
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

    View* v = View::Probe(view);
    Boolean clearChildFocus = FALSE;
    if (view == mFocused) {
        v->UnFocus(NULL);
        clearChildFocus = TRUE;
    }

    if (v->IsAccessibilityFocused()) {
        view->ClearAccessibilityFocus();
    }

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

    NeedGlobalAttributesUpdate(FALSE);

    RemoveFromArray(index);

    if (clearChildFocus) {
        ClearChildFocus(view);
        if (!RootViewRequestFocus()) {
            NotifyGlobalFocusCleared(THIS_PROBE(IView));
        }
    }

    OnViewRemoved(view);

    Int32 lastVisibility;
    view->GetVisibility(&lastVisibility);
    if ( != IView::GONE) {
        NotifySubtreeAccessibilityStateChangedIfNeeded();
    }
}

ECode ViewGroup::SetLayoutTransition(
    /* [in] */ ILayoutTransition* transition)
{
    if (mTransition != NULL) {
        AutoPtr<ILayoutTransition> previousTransition = mTransition;
        previousTransition->Cancel();
        previousTransition->RemoveTransitionListener(mLayoutTransitionListener);
    }

    mTransition = transition;
    if (mTransition != NULL) {
        mTransition->AddTransitionListener(mLayoutTransitionListener);
    }

    return NOERROR;
}

ECode ViewGroup::GetLayoutTransition(
    /* [out] */ ILayoutTransition** res)
{
    VALIDATE_NOT_NULL(res)
    *res = mTransition;
    REFCOUNT_ADD(*res)
    return NOERROR;
}

void ViewGroup::RemoveViewsInternal(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    AutoPtr<IView> focused = mFocused;
    Boolean detach = mAttachInfo != NULL;
    Boolean clearChildFocus = FALSE;

    const Int32 end = start + count;

    for (Int32 i = start; i < end; i++) {
        AutoPtr<IView> _view = (*mChildren)[i];
        View* view = View::Probe(_view);

        if (mTransition != NULL) {
            mTransition->RemoveChild(THIS_PROBE(IViewGroup), _view);
        }

        if (_view == focused) {
            view->UnFocus(NULL);
            clearChildFocus = TRUE;
        }

        if (view->IsAccessibilityFocused()) {
            view->ClearAccessibilityFocus();
        }

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

    if (clearChildFocus) {
        ClearChildFocus(focused);
        if (!RootViewRequestFocus()) {
            NotifyGlobalFocusCleared(focused);
        }
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
    Boolean clearChildFocus = FALSE;

    NeedGlobalAttributesUpdate(FALSE);

    for (Int32 i = count - 1; i >= 0; i--) {
        AutoPtr<IView> view = (*mChildren)[i];

        if (mTransition != NULL) {
            mTransition->RemoveChild(THIS_PROBE(IViewGroup), view);
        }

        if (view == focused) {
            (View::Probe(view))->UnFocus(NULL);
            clearChildFocus = TRUE;
        }

        if (view->IsAccessibilityFocused()) {
            view->ClearAccessibilityFocus();
        }

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
        View* v = View::Probe(view);
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
        ClearChildFocus(focused);
        if (!RootViewRequestFocus()) {
            NotifyGlobalFocusCleared(focused);
        }
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

    View* _child = View::Probe(child);
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
    View* v = View::Probe(child);

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
        View::Probe((*mChildren)[i])->mParent = NULL;
        mChildren->Set(i, NULL);
    }
}

ECode ViewGroup::InvalidateChild(
    /* [in] */ IView* child,
    /* [in] */ IRect* dirty)
{
    assert(child != NULL);
    View* v = View::Probe(child);

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
                view = View::Probe(parent);
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

            if ((mGroupFlags & FLAG_CLIP_CHILDREN) == 0) {
                dirty->Union(0, 0, mRight - mLeft, mBottom - mTop);
            }

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
            }

            return mParent;
        }
    }

    return NULL;
}

/**
 * Native-calculated damage path
 * Returns false if this path was unable to complete successfully. This means
 * it hit a ViewParent it doesn't recognize and needs to fall back to calculating
 * damage area
 * @hide
 */
ECode ViewGroup::DamageChildDeferred(
    /* [in] */ IView* child,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    AutoPtr<IViewParent> parent;
    GetParent((IViewParent**)&parent);
    while (parent != NULL) {
        if (IViewGroup::Probe(parent)) {
           parent->GetParent((IViewParent**)&parent);
        } else if (ViewRootImpl::Probe(parent)) {
            (ViewRootImpl::Probe(parent))->Invalidate();
            *res = TRUE;
            return NOERROR;
        } else {
            parent = NULL;
        }
    }
    *res = FALSE;
    return NOERROR;
}

ECode ViewGroup::DamageChild(
    /* [in] */ IView* child,
    /* [in] */ IRect* dirty)
{
    assert(child != NULL);
    if (DamageChildDeferred(child)) {
        return NOERROR;
    }
    AutoPtr<IViewParent> parent = THIS_PROBE(IViewParent);

    AttachInfo* attachInfo = mAttachInfo;
    View* v = View::Probe(child);
    if (attachInfo != NULL) {

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
                    parent = parentVG->DamageChildInParent(left, top, dirty);
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

AutoPtr<IViewParent> ViewGroup::DamageChildInParent(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ IRect* dirty)
{
    if ((mPrivateFlags & PFLAG_DRAWN) != 0 ||
            (mPrivateFlags & PFLAG_DRAWING_CACHE_VALID) != 0) {
        dirty->Offset(left - mScrollX, top - mScrollY);

        if ((mGroupFlags & FLAG_CLIP_CHILDREN) == 0) {
            dirty->Union(0, 0, mRight - mLeft, mBottom - mTop);
        }

        Boolean isIntersect = FALSE;
        if ((mGroupFlags & FLAG_CLIP_CHILDREN) == 0 ||
            (dirty->Intersect(0, 0, mRight - mLeft, mBottom - mTop, &isIntersect), isIntersect)) {

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
    if (descendant == IView::Probe(this)) {
        return NOERROR;
    }

    View* v = View::Probe(descendant);
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
                View* p = View::Probe(theParent);
                rect->Intersect(0, 0, p->mRight - p->mLeft, p->mBottom - p->mTop, &result);
            }
        }
        else {
            if (clipToBounds) {
                View* p = View::Probe(theParent);
                rect->Intersect(0, 0, p->mRight - p->mLeft, p->mBottom - p->mTop, &result);
            }
            rect->Offset(v->mScrollX - v->mLeft, v->mScrollY - v->mTop);
        }

        v = View::Probe(theParent);
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
    Boolean invalidate = FALSE;
    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* v = View::Probe((*mChildren)[i]);
        v->mTop += offset;
        v->mBottom += offset;
        if (v->mRenderNode != NULL) {
            invalidate = TRUE;
            v->mDisplayList->OffsetTopBottom(offset);
        }
    }

    if (invalidate) {
        InvalidateViewProperty(FALSE, FALSE);
    }
    NotifySubtreeAccessibilityStateChangedIfNeeded();

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
    View* _child = View::Probe(child);
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

        return mParent->GetChildVisibleRect(IView::Probe(this), r, offset, result);
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
    if (!mSuppressLayout && (mTransition == NULL || !(mTransition->IsChangingLayout(&changing), changing)) {
        if (mTransition != NULL) {
            mTransition->LayoutChange(THIS_PROBE(IViewGroup));
        }

        View::Layout(l, t, r, b);
    }
    else {
        // record the fact that we noop'd it; request layout when transition finishes
        mLayoutCalledWhileSuppressed = TRUE;
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

ECode ViewGroup::GetLayoutAnimation(
    /* [out] */ ILayoutAnimationController** res)
{
    VALIDATE_NOT_NULL(res)
    *res = mLayoutAnimationController;
    REFCOUNT_ADD(*res)
    return NOERROR;
}

ECode ViewGroup::IsAnimationCacheEnabled(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = (mGroupFlags & FLAG_ANIMATION_CACHE) == FLAG_ANIMATION_CACHE;
    return NOERROR;
}

ECode ViewGroup::SetAnimationCacheEnabled(
    /* [in] */ Boolean enabled)
{
    SetBooleanFlag(FLAG_ANIMATION_CACHE, enabled);
    return NOERROR;
}

ECode ViewGroup::IsAlwaysDrawnWithCacheEnabled(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = (mGroupFlags & FLAG_ALWAYS_DRAWN_WITH_CACHE) == FLAG_ALWAYS_DRAWN_WITH_CACHE;
    return NOERROR;
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

Boolean ViewGroup::HasBooleanFlag(
    /* [in] */ Int32 flag)
{
    return (mGroupFlags & flag) == flag;
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

ECode ViewGroup::GetPersistentDrawingCache(
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(res)
    *res = mPersistentDrawingCache;
    return NOERROR;
}

ECode ViewGroup::SetPersistentDrawingCache(
    /* [in] */ Int32 drawingCacheToKeep)
{
    mPersistentDrawingCache = drawingCacheToKeep & PERSISTENT_ALL_CACHES;
    return NOERROR;
}

void ViewGroup::SetLayoutMode(
    /* [in] */ Int32 layoutMode,
    /* [in] */ Boolean explicitly)
{
    mLayoutMode = layoutMode;
    SetBooleanFlag(FLAG_LAYOUT_MODE_WAS_EXPLICITLY_SET, explicitly);
}

/**
 * Recursively traverse the view hierarchy, resetting the layoutMode of any
 * descendants that had inherited a different layoutMode from a previous parent.
 * Recursion terminates when a descendant's mode is:
 * <ul>
 *     <li>Undefined</li>
 *     <li>The same as the root node's</li>
 *     <li>A mode that had been explicitly set</li>
 * <ul/>
 * The first two clauses are optimizations.
 * @param layoutModeOfRoot
 */
//@Override
void ViewGroup::InvalidateInheritedLayoutMode(
    /* [in] */ Int32 layoutModeOfRoot)
{
    if (mLayoutMode == LAYOUT_MODE_UNDEFINED ||
        mLayoutMode == layoutModeOfRoot ||
        HasBooleanFlag(FLAG_LAYOUT_MODE_WAS_EXPLICITLY_SET)) {
        return;
    }
    SetLayoutMode(LAYOUT_MODE_UNDEFINED, FALSE);

    // apply recursively
    for (Int32 i = 0, N = 0, GetChildCount(&N); i < N; i++) {
        GetChildAt(i)->InvalidateInheritedLayoutMode(layoutModeOfRoot);
    }
}

/**
 * Returns the basis of alignment during layout operations on this ViewGroup:
 * either {@link #LAYOUT_MODE_CLIP_BOUNDS} or {@link #LAYOUT_MODE_OPTICAL_BOUNDS}.
 * <p>
 * If no layoutMode was explicitly set, either programmatically or in an XML resource,
 * the method returns the layoutMode of the view's parent ViewGroup if such a parent exists,
 * otherwise the method returns a default value of {@link #LAYOUT_MODE_CLIP_BOUNDS}.
 *
 * @return the layout mode to use during layout operations
 *
 * @see #setLayoutMode(int)
 */
ECode ViewGroup::GetLayoutMode(
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(res)
    if (mLayoutMode == LAYOUT_MODE_UNDEFINED) {
        Int32 inheritedLayoutMode = LAYOUT_MODE_DEFAULT;
        if (IViewGroup::Probe(mParent)) {
            (IViewGroup::Probe(mParent))->GetLayoutMode(&inheritedLayoutMode);
        }
        SetLayoutMode(inheritedLayoutMode, FALSE);
    }
    *res = mLayoutMode;
    return NOERROR;
}

ECode ViewGroup::SetLayoutMode(
    /* [in] */ Int32 layoutMode)
{
    if (mLayoutMode != layoutMode) {
        InvalidateInheritedLayoutMode(layoutMode);
        SetLayoutMode(layoutMode, layoutMode != LAYOUT_MODE_UNDEFINED);
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
ECode ViewGroup::IndexOfChild(
    /* [in] */ IView* child,
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(res)
    for (Int32 i = 0; i < mChildrenCount; i++) {
        if ((*mChildren)[i] == child) {
            *res = i;
            return NOERROR;
        }
    }

    *res = -1;
    return NOERROR;
}

/**
 * Returns the number of children in the group.
 *
 * @return a positive integer representing the number of children in
 *         the group
 */
ECode ViewGroup::GetChildCount(
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(res)
    *res = mChildrenCount;
    return NOERROR;
}

/**
 * Returns the view at the specified position in the group.
 *
 * @param index the position at which to get the view from
 * @return the view at the specified position or NULL if the position
 *         does not exist within the group
 */
ECode ViewGroup::GetChildAt(
    /* [in] */ Int32 index,
    /* [out] */ IView** res)
{
    VALIDATE_NOT_NULL(res)
    if (index < 0 || index >= mChildrenCount) {
        *res = NULL;
        return NOERROR;
    }
    *res = (*mChildren)[index];
    REFCOUNT_ADD(*res)
    return NOERROR;
}

void ViewGroup::MeasureChildren(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* v = View::Probe((*mChildren)[i]);
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
    if (mDisappearingChildren != NULL) {
        Int32 count;
        mDisappearingChildren->GetSize(&count);
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IInterface> temp;
            mDisappearingChildren->Get(i, (IInterface**)&temp);
            View* view = View::Probe(temp);
            if (view->mAttachInfo != NULL) {
                view->DispatchDetachedFromWindow();
            }
            view->ClearAnimation();
        }
        mDisappearingChildren->Clear();
        Invalidate();
    }
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
    View* v = View::Probe(view);

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
    View* v = View::Probe(view);
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
            View* v = View::Probe(view);
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

/**
 * Tells this ViewGroup to suppress all layout() calls until layout
 * suppression is disabled with a later call to suppressLayout(false).
 * When layout suppression is disabled, a requestLayout() call is sent
 * if layout() was attempted while layout was being suppressed.
 *
 * @hide
 */
ECode ViewGroup::SuppressLayout(
    /* [in] */ Boolean suppress)
{
    mSuppressLayout = suppress;
    if (!suppress) {
        if (mLayoutCalledWhileSuppressed) {
            RequestLayout();
            mLayoutCalledWhileSuppressed = FALSE;
        }
    }
}

/**
 * Returns whether layout calls on this container are currently being
 * suppressed, due to an earlier call to {@link #suppressLayout(boolean)}.
 *
 * @return true if layout calls are currently suppressed, false otherwise.
 *
 * @hide
 */
ECode ViewGroup::IsLayoutSuppressed(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = mSuppressLayout;
    return NOERROR;
}

ECode ViewGroup::GatherTransparentRegion(
    /* [in, out] */ IRegion* region,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    // If no transparent regions requested, we are always opaque.
    const Boolean meOpaque = (mPrivateFlags & View::PFLAG_REQUEST_TRANSPARENT_REGIONS) == 0;
    if (meOpaque && region == NULL) {
        // The caller doesn't care about the region, so stop now.
        *res = TRUE;
        return NOERROR;
    }
    View::GatherTransparentRegion(region);
    Boolean noneOfTheChildrenAreTransparent = TRUE;
    for (Int32 i = 0; i < mChildrenCount; i++) {
        View* v = View::Probe((*mChildren)[i]);
        if ((v->mViewFlags & VISIBILITY_MASK) == IView::VISIBLE || v->GetAnimation() != NULL) {
            if (!v->GatherTransparentRegion(region)) {
                noneOfTheChildrenAreTransparent = FALSE;
            }
        }
    }
    *res = meOpaque || noneOfTheChildrenAreTransparent;
    return NOERROR;
}

ECode ViewGroup::RequestTransparentRegion(
    /* [in] */ IView* child)
{
    if (child != NULL) {
        View* v = View::Probe(child);
        v->mPrivateFlags |= View::PFLAG_REQUEST_TRANSPARENT_REGIONS;
        if (mParent != NULL) {
            mParent->RequestTransparentRegion(IView::Probe(this));
        }
    }
    return NOERROR;
}

ECode ViewGroup::DispatchApplyWindowInsets(
    /* [in] */ IWindowInsets* insets,
    /* [out] */ IWindowInsets** res)
{
    VALIDATE_NOT_NULL(res)
    View::DispatchApplyWindowInsets(insets, (IWindowInsets**)&insets);
    Boolean isConsumed;
    if (!(insets->IsConsumed(&isConsumed), isConsumed)) {
        Int32 count = getChildCount();
        for (Int32 i = 0; i < count; i++) {
            GetChildAt(i)->DispatchApplyWindowInsets(insets, (IWindowInsets**)&insets);
            if ((insets->IsConsumed(&isConsumed), isConsumed)) {
                break;
            }
        }
    }
    *res = insets;
    REFCOUNT_ADD(*res)
    return NOERROR;
}

ECode ViewGroup::GetLayoutAnimationListener(
    /* [out] */ IAnimationListener** res)
{
    VALIDATE_NOT_NULL(res)
    *res = mAnimationListener;
    REFCOUNT_ADD(*res)
    return NOERROR;
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
            View* v = View::Probe((*mChildren)[i]);
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

//@Override
ECode ViewGroup::DrawableHotspotChanged(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    View::DrawableHotspotChanged(x, y);

    if ((mGroupFlags & FLAG_NOTIFY_CHILDREN_ON_DRAWABLE_STATE_CHANGE) != 0) {
        if ((mGroupFlags & FLAG_ADD_STATES_FROM_CHILDREN) != 0) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        for (Int32 i = 0; i < mChildrenCount; i++) {
            View* child = View::Probe((*mChildren)[i]);
            if ((child->mViewFlags & DUPLICATE_PARENT_STATE) != 0) {
                child->DrawableHotspotChanged(x, y);
            }
        }
    }
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
        View* v = View::Probe(GetChildAt(i));
        AutoPtr<ArrayOf<Int32> > childState = v->GetDrawableState();

        if (childState != NULL) {
            need += childState->GetLength();
        }
    }

    AutoPtr<ArrayOf<Int32> > state;
    ASSERT_SUCCEEDED(View::OnCreateDrawableState(extraSpace + need, (ArrayOf<Int32>**)&state));

    for (Int32 i = 0; i < n; i++) {
        View* v = View::Probe(GetChildAt(i));
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

ECode ViewGroup::AddStatesFromChildren(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = (mGroupFlags & FLAG_ADD_STATES_FROM_CHILDREN) != 0;
    return NOERROR;
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

ECode ViewGroup::ResolveRtlPropertiesIfNeeded(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    View::ResolveRtlPropertiesIfNeeded(res);
    if (*res) {
        Int32 count = GetChildCount();
        for (Int32 i = 0; i < count; i++) {
            View* child = View::Probe(GetChildAt(i));
            if (child->IsLayoutDirectionInherited()) {
                child->ResolveRtlPropertiesIfNeeded();
            }
        }
    }
    return NOERROR;
}

ECode ViewGroup::ResolveLayoutDirection(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    Boolean result = View::ResolveLayoutDirection();
    if (result) {
        Int32 count = GetChildCount();
        for (Int32 i = 0; i < count; i++) {
            View* child = View::Probe(GetChildAt(i));
            if (child->IsLayoutDirectionInherited()) {
                child->ResolveLayoutDirection();
            }
        }
    }

    *res = result;
    return NOERROR;
}

ECode ViewGroup::ResolveTextDirection(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    Boolean result = View::ResolveTextDirection();
    if (result) {
        Int32 count = GetChildCount();
        for (Int32 i = 0; i < count; i++) {
            View* child = View::Probe(GetChildAt(i));
            if (child->IsTextDirectionInherited()) {
                child->ResolveTextDirection();
            }
        }
    }
    *res = result;
    return NOERROR;
}

ECode ViewGroup::ResolveTextAlignment(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    Boolean result = View::ResolveTextAlignment();
    if (result) {
        Int32 count = GetChildCount();
        for (Int32 i = 0; i < count; i++) {
            View* child = View::Probe(GetChildAt(i));
            if (child->IsTextAlignmentInherited()) {
                child->ResolveTextAlignment();
            }
        }
    }
    *res = result;
    return NOERROR;
}

ECode ViewGroup::ResolvePadding()
{
    View::ResolvePadding();
    Int32 count = GetChildCount();
    for (Int32 i = 0; i < count; i++) {
        View* child = View::Probe(GetChildAt(i));
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
        View* child = View::Probe(GetChildAt(i));
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
        View* child = View::Probe(GetChildAt(i));
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
        View* child = View::Probe(GetChildAt(i));
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
        View* child = View::Probe(GetChildAt(i));
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
        View* child = View::Probe(GetChildAt(i));
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
        View* child = View::Probe(GetChildAt(i));
        if (child->IsLayoutDirectionInherited()) {
            child->ResetResolvedDrawables();
        }
    }
}

ECode ViewGroup::ShouldDelayChildPressedState(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = TRUE;
    return NOERROR;
}

/**
 * @inheritDoc
 */
//@Override
ECode ViewGroup::OnStartNestedScroll(
    /* [in] */ IView* child,
    /* [in] */ IView* target,
    /* [in] */ Int32 nestedScrollAxes,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = FALSE;
    return NOERROR;
}

/**
 * @inheritDoc
 */
//@Override
ECode ViewGroup::OnNestedScrollAccepted(
    /* [in] */ IView* child,
    /* [in] */ IView* target,
    /* [in] */ Int32 axes)
{
    mNestedScrollAxes = axes;
}

/**
 * @inheritDoc
 *
 * <p>The default implementation of onStopNestedScroll calls
 * {@link #stopNestedScroll()} to halt any recursive nested scrolling in progress.</p>
 */
//@Override
ECode ViewGroup::OnStopNestedScroll(
    /* [in] */ IView* child)
{
    // Stop any recursive nested scrolling.
    StopNestedScroll();
}

/**
 * @inheritDoc
 */
//@Override
ECode ViewGroup::OnNestedScroll(
    /* [in] */ IView* target,
    /* [in] */ Int32 dxConsumed,
    /* [in] */ Int32 dyConsumed,
    /* [in] */ Int32 dxUnconsumed,
    /* [in] */ Int32 dyUnconsumed)
{
    // Do nothing
}

/**
 * @inheritDoc
 */
//@Override
ECode ViewGroup::OnNestedPreScroll(
    /* [in] */ IView* target,
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy,
    /* [in] */ ArrayOf<Int32>* consumed)
{
    // Do nothing
}

/**
 * @inheritDoc
 */
//@Override
ECode ViewGroup::OnNestedFling(
    /* [in] */ IView* target,
    /* [in] */ Float velocityX,
    /* [in] */ Float velocityY,
    /* [in] */ Boolean consumed,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = FALSE;
    return NOERROR;
}

/**
 * @inheritDoc
 */
//@Override
ECode ViewGroup::OnNestedPreFling(
    /* [in] */ IView* target,
    /* [in] */ Float velocityX,
    /* [in] */ Float velocityY,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = FALSE;
    return NOERROR;
}

/**
 * Return the current axes of nested scrolling for this ViewGroup.
 *
 * <p>A ViewGroup returning something other than {@link #SCROLL_AXIS_NONE} is currently
 * acting as a nested scrolling parent for one or more descendant views in the hierarchy.</p>
 *
 * @return Flags indicating the current axes of nested scrolling
 * @see #SCROLL_AXIS_HORIZONTAL
 * @see #SCROLL_AXIS_VERTICAL
 * @see #SCROLL_AXIS_NONE
 */
ECode ViewGroup::GetNestedScrollAxes(
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(res)
    *res = mNestedScrollAxes;
    return NOERROR;
}

void ViewGroup::OnSetLayoutParams(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* layoutParams)
{
}

/** @hide */
//@Override
ECode ViewGroup::CaptureTransitioningViews(
    /* [in] */ IList* transitioningViews)
{
    Int32 visibility;
    GetVisibility(&visibility);
    if (visibility != IView::VISIBLE) {
        return NOERROR;
    }
    Boolean isTransitionGroup;
    if (IsTransitionGroup(&isTransitionGroup), isTransitionGroup) {
        transitioningViews->Add((IInterface*)this->Probe(EIID_IInterface));
    } else {
        Int32 count = GetChildCount();
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> child = GetChildAt(i);
            child->CaptureTransitioningViews(transitioningViews);
        }
    }
}

/** @hide */
//@Override
ECode ViewGroup::FindNamedViews(
    /* [in] */ IMap* namedElements)
{
    Int32 visibility;
    GetVisibility(&visibility);
    if (visibility != IView::VISIBLE && mGhostView == NULL) {
        return NOERROR;
    }
    View::FindNamedViews(namedElements);
    Int32 count = getChildCount();
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        child->FindNamedViews(namedElements);
    }
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
