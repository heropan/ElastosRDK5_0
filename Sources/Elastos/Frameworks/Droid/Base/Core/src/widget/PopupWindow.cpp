#include "widget/PopupWindow.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>
#include <R.h>
#include "os/Build.h"
#include "view/CWindowManagerLayoutParams.h"
#include "widget/CFrameLayoutLayoutParams.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::R;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::IStateListDrawable;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::EIID_ViewGroup;
using Elastos::Droid::View::EIID_IViewParent;
using Elastos::Droid::View::EIID_IViewManager;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::View::EIID_IOnScrollChangedListener;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::CWindowManagerLayoutParams;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::Accessibility::EIID_IAccessibilityEventSource;

namespace Elastos {
namespace Droid {
namespace Widget {

//==============================================================================
//          PopupWindow::_PopupViewContainer
//==============================================================================

ECode PopupWindow::_PopupViewContainer::OnCreateDrawableState(
    /* [in] */ Int32 extraSpace,
    /* [out] */ ArrayOf<Int32>** drawableState)
{
    VALIDATE_NOT_NULL(drawableState);
    *drawableState = NULL;

    AutoPtr<ArrayOf<Int32> > ds;
    if (mOwner->mAboveAnchor) {
        // 1 more needed for the above anchor state
        FAIL_RETURN(FrameLayout::OnCreateDrawableState(extraSpace + 1, (ArrayOf<Int32>**)&ds));
        View::MergeDrawableStates(ds, ABOVE_ANCHOR_STATE_SET);
        *drawableState = ds;
        REFCOUNT_ADD(*drawableState);
        return NOERROR;
    }
    else {
        FAIL_RETURN(FrameLayout::OnCreateDrawableState(extraSpace, (ArrayOf<Int32>**)&ds));
        *drawableState = ds;
        REFCOUNT_ADD(*drawableState);
        return NOERROR;
    }
}

Boolean PopupWindow::_PopupViewContainer::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    if (keyCode == IKeyEvent::KEYCODE_BACK) {
        AutoPtr<IDispatcherState> dispatcherState;
        GetKeyDispatcherState((IDispatcherState**)&dispatcherState);
        if (!dispatcherState) {
            return FrameLayout::DispatchKeyEvent(event);
        }

        Int32 action, count;
        event->GetAction(&action);
        event->GetRepeatCount(&count);
        if (action == IKeyEvent::ACTION_DOWN && count == 0) {
            AutoPtr<IDispatcherState> state;
            GetKeyDispatcherState((IDispatcherState**)&state);
            if (state != NULL) {
                state->StartTracking(event, this->Probe(EIID_IInterface));
            }
            return TRUE;
        }
        else if (action == IKeyEvent::ACTION_UP) {
                AutoPtr<IDispatcherState> state;
                GetKeyDispatcherState((IDispatcherState**)&state);
                if (state != NULL) {
                    Boolean tracking, canceled;
                    state->IsTracking(event, &tracking);
                    event->IsCanceled(&canceled);
                    if (tracking && !canceled) {
                        mOwner->Dismiss();
                        return TRUE;
                    }
                }
            }
        return FrameLayout::DispatchKeyEvent(event);
    }
    else {
        return FrameLayout::DispatchKeyEvent(event);
    }
}

Boolean PopupWindow::_PopupViewContainer::DispatchTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    if (mOwner->mTouchInterceptor != NULL) {
        Boolean result;
        mOwner->mTouchInterceptor->OnTouch(THIS_PROBE(IView), event, &result);
        if (result)
            return TRUE;
    }
    return FrameLayout::DispatchTouchEvent(event);
}

Boolean PopupWindow::_PopupViewContainer::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    assert(event != NULL);

    Float _x, _y;
    event->GetX(&_x);
    event->GetY(&_y);
    Int32 x = (Int32)_x;
    Int32 y = (Int32)_y;

    Int32 action;
    event->GetAction(&action);
    if ((action == IMotionEvent::ACTION_DOWN)
            && ((x < 0) || (x >= mOwner->GetWidth()) || (y < 0) || (y >= mOwner->GetHeight()))) {
        mOwner->Dismiss();
        return TRUE;
    }
    else if (action == IMotionEvent::ACTION_OUTSIDE) {
        mOwner->Dismiss();
        return TRUE;
    }
    else {
        return FrameLayout::OnTouchEvent(event);
    }
}

ECode PopupWindow::_PopupViewContainer::SendAccessibilityEvent(
    /* [in] */ Int32 eventType)
{
    // clinets are interested in the content not the container, make it event source
    if (mOwner->mContentView != NULL) {
        return IAccessibilityEventSource::Probe(mOwner->mContentView)->SendAccessibilityEvent(eventType);
    }
    else {
        return FrameLayout::SendAccessibilityEvent(eventType);
    }
    return NOERROR;
}

//==============================================================================
//          PopupWindow::PopupViewContainer
//==============================================================================

IVIEW_METHODS_IMPL(PopupWindow::PopupViewContainer, PopupWindow::_PopupViewContainer)
IVIEWGROUP_METHODS_IMPL(PopupWindow::PopupViewContainer, PopupWindow::_PopupViewContainer)
IVIEWPARENT_METHODS_IMPL(PopupWindow::PopupViewContainer, PopupWindow::_PopupViewContainer)
IVIEWMANAGER_METHODS_IMPL(PopupWindow::PopupViewContainer, PopupWindow::_PopupViewContainer)
IDRAWABLECALLBACK_METHODS_IMPL(PopupWindow::PopupViewContainer, PopupWindow::_PopupViewContainer)
IKEYEVENTCALLBACK_METHODS_IMPL(PopupWindow::PopupViewContainer, PopupWindow::_PopupViewContainer)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(PopupWindow::PopupViewContainer, PopupWindow::_PopupViewContainer)

PInterface PopupWindow::PopupViewContainer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    } else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    else if (EIID_IInterface == riid) {
        return (IInterface *)(IFrameLayout *)this;
    }
    else if (EIID_IView == riid) {
        return (IView *)(IFrameLayout *)this;
    }
    else if (EIID_IViewGroup == riid) {
        return (IViewGroup *)(IFrameLayout *)this;
    }
    else if (EIID_IFrameLayout == riid) {
        return (IFrameLayout *)this;
    }
    else if (EIID_IViewParent == riid) {
        return (IViewParent *)this;
    }
    else if (EIID_IViewManager == riid) {
        return (IViewManager *)this;
    }
    else if (EIID_IDrawableCallback == riid) {
        return (IDrawableCallback *)this;
    }
    else if (EIID_IKeyEventCallback == riid) {
        return (IKeyEventCallback *)this;
    }
    else if (EIID_IAccessibilityEventSource == riid) {
        return (IAccessibilityEventSource *)this;
    }
    else if (EIID_IWeakReferenceSource == riid) {
        return (IWeakReferenceSource *)this;
    }

    return NULL;
}

UInt32 PopupWindow::PopupViewContainer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 PopupWindow::PopupViewContainer::Release()
{
    return ElRefBase::Release();
}

ECode PopupWindow::PopupViewContainer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode PopupWindow::PopupViewContainer::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

ECode PopupWindow::PopupViewContainer::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    return FrameLayout::SetForegroundGravity(foregroundGravity);
}

ECode PopupWindow::PopupViewContainer::GetForegroundGravity(
    /* [out] */ Int32* foregroundGravity)
{
    VALIDATE_NOT_NULL(foregroundGravity);
    *foregroundGravity = FrameLayout::GetForegroundGravity();
    return NOERROR;
}

ECode PopupWindow::PopupViewContainer::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    return FrameLayout::SetForeground(drawable);
}

ECode PopupWindow::PopupViewContainer::GetForeground(
    /* [out] */ IDrawable** foreground)
{
    VALIDATE_NOT_NULL(foreground);
    AutoPtr<IDrawable> d = FrameLayout::GetForeground();
    *foreground = d.Get();
    REFCOUNT_ADD(*foreground);

    return NOERROR;
}

ECode PopupWindow::PopupViewContainer::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    return FrameLayout::SetMeasureAllChildren(measureAll);
}

ECode PopupWindow::PopupViewContainer::GetMeasureAllChildren(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll = FrameLayout::GetMeasureAllChildren();
    return NOERROR;
}

ECode PopupWindow::PopupViewContainer::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll = FrameLayout::GetConsiderGoneChildrenWhenMeasuring();
    return NOERROR;
}

//==============================================================================
//          PopupWindow::PopupWindowScrollChangedListener
//==============================================================================

CAR_INTERFACE_IMPL(PopupWindow::PopupWindowScrollChangedListener, IOnScrollChangedListener);

ECode PopupWindow::PopupWindowScrollChangedListener::OnScrollChanged()
{
    AutoPtr<IView> anchor;
    if (mOwner->mAnchor != NULL) {
        mOwner->mAnchor->Resolve(EIID_IView, (IInterface**)&anchor);
    }

    if (anchor != NULL && mOwner->mPopupView != NULL) {
        AutoPtr<IViewGroupLayoutParams> p;
        mOwner->mPopupView->GetLayoutParams((IViewGroupLayoutParams**)&p);
        IWindowManagerLayoutParams* lp = IWindowManagerLayoutParams::Probe(p);
        assert(lp != NULL && "IWindowManagerLayoutParams cannot be null.");
        Int32 x, y;
        lp->GetX(&x);
        lp->GetY(&y);
        mOwner->UpdateAboveAnchor(mOwner->FindDropDownPosition(
                anchor, lp, mOwner->mAnchorXoff, mOwner->mAnchorYoff));
        mOwner->Update(x, y, -1, -1, TRUE);
    }

    return NOERROR;
}


//==============================================================================
//          PopupWindow
//==============================================================================
static AutoPtr<ArrayOf<Int32> > InitABOVE_ANCHOR_STATE_SET()
{
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(1);
    (*array)[0] = R::attr::state_above_anchor;
    return array;
}

AutoPtr<ArrayOf<Int32> > PopupWindow::ABOVE_ANCHOR_STATE_SET = InitABOVE_ANCHOR_STATE_SET();

PopupWindow::PopupWindow()
    : mIsShowing(FALSE)
    , mIsDropdown(FALSE)
    , mFocusable(FALSE)
    , mInputMethodMode(0/*INPUT_METHOD_FROM_FOCUSABLE*/)
    , mSoftInputMode(0)
    , mTouchable(TRUE)
    , mOutsideTouchable(FALSE)
    , mClippingEnabled(TRUE)
    , mSplitTouchEnabled(-1)
    , mLayoutInScreen(FALSE)
    , mClipToScreen(FALSE)
    , mAllowScrollingAnchorParent(TRUE)
    , mLayoutInsetDecor(FALSE)
    , mNotTouchModal(FALSE)
    , mWidthMode(0)
    , mWidth(0)
    , mLastWidth(0)
    , mHeightMode(0)
    , mHeight(0)
    , mLastHeight(0)
    , mPopupWidth(0)
    , mPopupHeight(0)
    , mAboveAnchor(FALSE)
    , mWindowLayoutType(IWindowManagerLayoutParams::TYPE_APPLICATION_PANEL)
    , mIgnoreCheekPress(FALSE)
    , mAnimationStyle(-1)
    , mAnchor(NULL)
    , mAnchorXoff(0)
    , mAnchorYoff(0)
    , mPopupViewInitialLayoutDirectionInherited(FALSE)
{
    mDrawingLocation = ArrayOf<Int32>::Alloc(2);
    mScreenLocation = ArrayOf<Int32>::Alloc(2);
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));

    ASSERT_SUCCEEDED(Init());
}

PopupWindow::PopupWindow(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : mIsShowing(FALSE)
    , mIsDropdown(FALSE)
    , mFocusable(FALSE)
    , mInputMethodMode(0/*INPUT_METHOD_FROM_FOCUSABLE*/)
    , mSoftInputMode(0)
    , mTouchable(TRUE)
    , mOutsideTouchable(FALSE)
    , mClippingEnabled(TRUE)
    , mSplitTouchEnabled(-1)
    , mLayoutInScreen(FALSE)
    , mClipToScreen(FALSE)
    , mAllowScrollingAnchorParent(TRUE)
    , mLayoutInsetDecor(FALSE)
    , mNotTouchModal(FALSE)
    , mWidthMode(0)
    , mWidth(0)
    , mLastWidth(0)
    , mHeightMode(0)
    , mHeight(0)
    , mLastHeight(0)
    , mPopupWidth(0)
    , mPopupHeight(0)
    , mAboveAnchor(FALSE)
    , mWindowLayoutType(IWindowManagerLayoutParams::TYPE_APPLICATION_PANEL)
    , mIgnoreCheekPress(FALSE)
    , mAnimationStyle(-1)
    , mAnchorXoff(0)
    , mAnchorYoff(0)
    , mPopupViewInitialLayoutDirectionInherited(FALSE)
{
    mDrawingLocation = ArrayOf<Int32>::Alloc(2);
    mScreenLocation = ArrayOf<Int32>::Alloc(2);
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));

    ASSERT_SUCCEEDED(Init(context, attrs, defStyle, 0));
}


/**
 * <p>Create a new, empty, non focusable popup window of dimension (0,0).</p>
 *
 * <p>The popup does not provide a background.</p>
 */
PopupWindow::PopupWindow(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
    : mIsShowing(FALSE)
    , mIsDropdown(FALSE)
    , mFocusable(FALSE)
    , mInputMethodMode(0/*INPUT_METHOD_FROM_FOCUSABLE*/)
    , mSoftInputMode(0)
    , mTouchable(TRUE)
    , mOutsideTouchable(FALSE)
    , mClippingEnabled(TRUE)
    , mSplitTouchEnabled(-1)
    , mLayoutInScreen(FALSE)
    , mClipToScreen(FALSE)
    , mAllowScrollingAnchorParent(TRUE)
    , mLayoutInsetDecor(FALSE)
    , mNotTouchModal(FALSE)
    , mWidthMode(0)
    , mWidth(0)
    , mLastWidth(0)
    , mHeightMode(0)
    , mHeight(0)
    , mLastHeight(0)
    , mPopupWidth(0)
    , mPopupHeight(0)
    , mAboveAnchor(FALSE)
    , mWindowLayoutType(IWindowManagerLayoutParams::TYPE_APPLICATION_PANEL)
    , mIgnoreCheekPress(FALSE)
    , mAnimationStyle(-1)
    , mAnchorXoff(0)
    , mAnchorYoff(0)
    , mPopupViewInitialLayoutDirectionInherited(FALSE)
{
    mDrawingLocation = ArrayOf<Int32>::Alloc(2);
    mScreenLocation = ArrayOf<Int32>::Alloc(2);
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));

    ASSERT_SUCCEEDED(Init(context, attrs, defStyleAttr, defStyleRes));
}

PopupWindow::PopupWindow(
    /* [in] */ IView* contentView,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean focusable)
    : mIsShowing(FALSE)
    , mIsDropdown(FALSE)
    , mFocusable(FALSE)
    , mInputMethodMode(0/*INPUT_METHOD_FROM_FOCUSABLE*/)
    , mSoftInputMode(0)
    , mTouchable(TRUE)
    , mOutsideTouchable(FALSE)
    , mClippingEnabled(TRUE)
    , mSplitTouchEnabled(-1)
    , mLayoutInScreen(FALSE)
    , mClipToScreen(FALSE)
    , mAllowScrollingAnchorParent(TRUE)
    , mLayoutInsetDecor(FALSE)
    , mNotTouchModal(FALSE)
    , mWidthMode(0)
    , mWidth(0)
    , mLastWidth(0)
    , mHeightMode(0)
    , mHeight(0)
    , mLastHeight(0)
    , mPopupWidth(0)
    , mPopupHeight(0)
    , mAboveAnchor(FALSE)
    , mWindowLayoutType(IWindowManagerLayoutParams::TYPE_APPLICATION_PANEL)
    , mIgnoreCheekPress(FALSE)
    , mAnimationStyle(-1)
    , mAnchorXoff(0)
    , mAnchorYoff(0)
    , mPopupViewInitialLayoutDirectionInherited(FALSE)
{
    mDrawingLocation = ArrayOf<Int32>::Alloc(2);
    mScreenLocation = ArrayOf<Int32>::Alloc(2);
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));

    ASSERT_SUCCEEDED(Init(contentView, width, height, focusable));
}

ECode PopupWindow::Init(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    assert(ctx != NULL);

    mContext = ctx;
    ctx->GetSystemService(IContext::WINDOW_SERVICE, (IInterface**)&mWindowManager);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::PopupWindow),
            ARRAY_SIZE(R::styleable::PopupWindow));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(ctx->ObtainStyledAttributes(
            attrs, attrIds, defStyleAttr, defStyleRes, (ITypedArray**)&a));

    a->GetDrawable(R::styleable::PopupWindow_popupBackground, (IDrawable**)&mBackground);

    Int32 animStyle;
    a->GetResourceId(R::styleable::PopupWindow_popupAnimationStyle, -1, &animStyle);
    mAnimationStyle = animStyle == R::style::Animation_PopupWindow ? -1 :
            animStyle;

    // If this is a StateListDrawable, try to find and store the drawable to be
    // used when the drop-down is placed above its anchor view, and the one to be
    // used when the drop-down is placed below its anchor view. We extract
    // the drawables ourselves to work around a problem with using refreshDrawableState
    // that it will take into account the padding of all drawables specified in a
    // StateListDrawable, thus adding superfluous padding to drop-down views.
    //
    // We assume a StateListDrawable will have a drawable for ABOVE_ANCHOR_STATE_SET and
    // at least one other drawable, intended for the 'below-anchor state'.
    if (mBackground && IStateListDrawable::Probe(mBackground) != NULL) {
        IStateListDrawable* background = IStateListDrawable::Probe(mBackground);

        // Find the above-anchor view - this one's easy, it should be labeled as such.
        Int32 aboveAnchorStateIndex;
        background->GetStateDrawableIndex(*ABOVE_ANCHOR_STATE_SET, &aboveAnchorStateIndex);

        // Now, for the below-anchor view, look for any other drawable specified in the
        // StateListDrawable which is not for the above-anchor state and use that.
        Int32 count;
        background->GetStateCount(&count);
        Int32 belowAnchorStateIndex = -1;
        for (Int32 i = 0; i < count; i++) {
            if (i != aboveAnchorStateIndex) {
                belowAnchorStateIndex = i;
                break;
            }
        }

        // Store the drawables we found, if we found them. Otherwise, set them both
        // to NULL so that we'll just use refreshDrawableState.
        if (aboveAnchorStateIndex != -1 && belowAnchorStateIndex != -1) {
            background->GetStateDrawable(aboveAnchorStateIndex,
                    (IDrawable**)&mAboveAnchorBackgroundDrawable);
            background->GetStateDrawable(belowAnchorStateIndex,
                    (IDrawable**)&mBelowAnchorBackgroundDrawable);
        }
        else {
            mBelowAnchorBackgroundDrawable = NULL;
            mAboveAnchorBackgroundDrawable = NULL;
        }
    }

    a->Recycle();

    return NOERROR;
}

ECode PopupWindow::Init(
    /* [in] */ IView* contentView,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean focusable)
{
    if (contentView != NULL) {
        contentView->GetContext((IContext**)&mContext);
        mContext->GetSystemService(IContext::WINDOW_SERVICE, (IInterface**)&mWindowManager);
    }
    SetContentView(contentView);
    SetWidth(width);
    SetHeight(height);
    SetFocusable(focusable);

    return NOERROR;
}

AutoPtr<IDrawable> PopupWindow::GetBackground()
{
    return mBackground;
}

ECode PopupWindow::SetBackgroundDrawable(
    /* [in] */ IDrawable* background)
{
    mBackground = background;

    return NOERROR;
}

Int32 PopupWindow::GetAnimationStyle()
{
    return mAnimationStyle;
}

ECode PopupWindow::SetIgnoreCheekPress()
{
    mIgnoreCheekPress = TRUE;

    return NOERROR;
}

ECode PopupWindow::SetAnimationStyle(
    /* [in] */ Int32 animationStyle)
{
    mAnimationStyle = animationStyle;

    return NOERROR;
}

AutoPtr<IView> PopupWindow::GetContentView()
{
    return mContentView;
}

ECode PopupWindow::SetContentView(
    /* [in] */ IView* contentView)
{
    if (IsShowing()) {
        return NOERROR;
    }

    mContentView = contentView;
    if (mContext == NULL && mContentView != NULL) {
        mContentView->GetContext((IContext**)&mContext);
    }

    if (mWindowManager == NULL && mContentView != NULL) {
        mContext->GetSystemService(IContext::WINDOW_SERVICE, (IInterface**)&mWindowManager);
    }

    return NOERROR;
}

ECode PopupWindow::SetTouchInterceptor(
    /* [in] */ IViewOnTouchListener* l)
{
    mTouchInterceptor = l;

    return NOERROR;
}

Boolean PopupWindow::IsFocusable()
{
    return mFocusable;
}

ECode PopupWindow::SetFocusable(
     /* [in] */ Boolean focusable)
{
    mFocusable = focusable;

    return NOERROR;
}

Int32 PopupWindow::GetInputMethodMode()
{
    return mInputMethodMode;
}

ECode PopupWindow::SetInputMethodMode(
    /* [in] */ Int32 mode)
{
    mInputMethodMode = mode;

    return NOERROR;
}

ECode PopupWindow::SetSoftInputMode(
    /* [in] */ Int32 mode)
{
    mSoftInputMode = mode;

    return NOERROR;
}

Int32 PopupWindow::GetSoftInputMode()
{
    return mSoftInputMode;
}

Boolean PopupWindow::IsTouchable()
{
    return mTouchable;
}

ECode PopupWindow::SetTouchable(
    /* [in] */ Boolean touchable)
{
    mTouchable = touchable;

    return NOERROR;
}

Boolean PopupWindow::IsOutsideTouchable()
{
    return mOutsideTouchable;
}

ECode PopupWindow::SetOutsideTouchable(
    /* [in] */ Boolean touchable)
{
    mOutsideTouchable = touchable;

    return NOERROR;
}

Boolean PopupWindow::IsClippingEnabled()
{
    return mClippingEnabled;
}

ECode PopupWindow::SetClippingEnabled(
    /* [in] */ Boolean enabled)
{
    mClippingEnabled = enabled;

    return NOERROR;
}

ECode PopupWindow::SetClipToScreenEnabled(
    /* [in] */ Boolean enabled)
{
    mClipToScreen = enabled;
    SetClippingEnabled(!enabled);
    return NOERROR;
}

ECode PopupWindow::SetAllowScrollingAnchorParent(
    /* [in] */ Boolean enabled)
{
    mAllowScrollingAnchorParent = enabled;
    return NOERROR;
}

Boolean PopupWindow::IsSplitTouchEnabled()
{
    if (mSplitTouchEnabled < 0 && mContext != NULL) {
        AutoPtr<IApplicationInfo> info;
        mContext->GetApplicationInfo((IApplicationInfo**)&info);
        Int32 targetSdkVersion;
        info->GetTargetSdkVersion(&targetSdkVersion);
        return targetSdkVersion >= Build::VERSION_CODES::HONEYCOMB;
    }
    return mSplitTouchEnabled == 1;
}

ECode PopupWindow::SetSplitTouchEnabled(
    /* [in] */ Boolean enabled)
{
    mSplitTouchEnabled = enabled ? 1 : 0;

    return NOERROR;
}

Boolean PopupWindow::IsLayoutInScreenEnabled()
{
    return mLayoutInScreen;
}

ECode PopupWindow::SetLayoutInScreenEnabled(
    /* [in] */ Boolean enabled)
{
    mLayoutInScreen = enabled;

    return NOERROR;
}

ECode PopupWindow::SetLayoutInsetDecor(
    /* [in] */ Boolean enabled)
{
    mLayoutInsetDecor = enabled;
    return NOERROR;
}

ECode PopupWindow::SetWindowLayoutType(
    /* [in] */ Int32 layoutType)
{
    mWindowLayoutType = layoutType;

    return NOERROR;
}

Int32 PopupWindow::GetWindowLayoutType()
{
    return mWindowLayoutType;
}

ECode PopupWindow::SetTouchModal(
    /* [in] */ Boolean touchModal)
{
    mNotTouchModal = !touchModal;
    return NOERROR;
}

ECode PopupWindow::SetWindowLayoutMode(
    /* [in] */ Int32 widthSpec,
    /* [in] */ Int32 heightSpec)
{
    mWidthMode = widthSpec;
    mHeightMode = heightSpec;

    return NOERROR;
}

Int32 PopupWindow::GetHeight()
{
    return mHeight;
}

ECode PopupWindow::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;

    return NOERROR;
}

Int32 PopupWindow::GetWidth()
{
    return mWidth;
}

ECode PopupWindow::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;

    return NOERROR;
}

Boolean PopupWindow::IsShowing()
{
    return mIsShowing;
}

ECode PopupWindow::ShowAtLocation(
    /* [in] */ IView* parent,
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    AutoPtr<IBinder> token;
    parent->GetWindowToken((IBinder**)&token);
    return ShowAtLocation(token, gravity, x, y);
}

ECode PopupWindow::ShowAtLocation(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (IsShowing() || mContentView == NULL) {
        return NOERROR;
    }

    UnregisterForScrollChanged();

    mIsShowing = TRUE;
    mIsDropdown = FALSE;

    AutoPtr<IWindowManagerLayoutParams> p = CreatePopupLayout(token);
    ((CWindowManagerLayoutParams*)p.Get())->mWindowAnimations = ComputeAnimationResource();

    FAIL_RETURN(PreparePopup(p));
    if (gravity == IGravity::NO_GRAVITY) {
        gravity = IGravity::TOP | IGravity::START;
    }
    ((CWindowManagerLayoutParams*)p.Get())->mGravity = gravity;
    ((CWindowManagerLayoutParams*)p.Get())->mX = x;
    ((CWindowManagerLayoutParams*)p.Get())->mY = y;
    if (mHeightMode < 0) ((CWindowManagerLayoutParams*)p.Get())->mHeight = mLastHeight = mHeightMode;
    if (mWidthMode < 0) ((CWindowManagerLayoutParams*)p.Get())->mWidth = mLastWidth = mWidthMode;
    InvokePopup(p);

    return NOERROR;
}

ECode PopupWindow::ShowAsDropDown(
    /* [in] */ IView* anchor)
{
    return ShowAsDropDown(anchor, 0, 0);
}

ECode PopupWindow::ShowAsDropDown(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 xoff,
    /* [in] */ Int32 yoff)
{
    if (IsShowing() || mContentView == NULL) {
        return NOERROR;
    }

    RegisterForScrollChanged(anchor, xoff, yoff);

    mIsShowing = TRUE;
    mIsDropdown = TRUE;

    AutoPtr<IBinder> token;
    anchor->GetWindowToken((IBinder**)&token);
    AutoPtr<IWindowManagerLayoutParams> p = CreatePopupLayout(token);
    FAIL_RETURN(PreparePopup(p));

    UpdateAboveAnchor(FindDropDownPosition(anchor, p, xoff, yoff));

    if (mHeightMode < 0) {
        ((CWindowManagerLayoutParams*)p.Get())->mHeight = mLastHeight = mHeightMode;
    }
    if (mWidthMode < 0) {
        ((CWindowManagerLayoutParams*)p.Get())->mWidth = mLastWidth = mWidthMode;
    }

    ((CWindowManagerLayoutParams*)p.Get())->mWindowAnimations = ComputeAnimationResource();

    InvokePopup(p);

    return NOERROR;
}

void PopupWindow::UpdateAboveAnchor(
    /* [in] */ Boolean aboveAnchor)
{
    if (aboveAnchor != mAboveAnchor) {
        mAboveAnchor = aboveAnchor;

        if (mBackground != NULL) {
            // If the background drawable provided was a StateListDrawable with above-anchor
            // and below-anchor states, use those. Otherwise rely on refreshDrawableState to
            // do the job.
            if (mAboveAnchorBackgroundDrawable != NULL) {
                if (mAboveAnchor) {
                    mPopupView->SetBackgroundDrawable(mAboveAnchorBackgroundDrawable);
                }
                else {
                    mPopupView->SetBackgroundDrawable(mBelowAnchorBackgroundDrawable);
                }
            }
            else {
                mPopupView->RefreshDrawableState();
            }
        }
    }
}

Boolean PopupWindow::IsAboveAnchor()
{
    return mAboveAnchor;
}

ECode PopupWindow::PreparePopup(
    /* [in] */ IWindowManagerLayoutParams* p)
{
    if (mContentView == NULL || mContext == NULL || mWindowManager == NULL) {
//        throw new IllegalStateException("You must specify a valid content view by "
//                + "calling setContentView() before attempting to show the popup.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (mBackground != NULL) {
        AutoPtr<IViewGroupLayoutParams> layoutParams;
        mContentView->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
        Int32 height = IViewGroupLayoutParams::MATCH_PARENT;
        Int32 h;
        if (layoutParams != NULL && (layoutParams->GetHeight(&h),
            h) == IViewGroupLayoutParams::WRAP_CONTENT) {
            height = IViewGroupLayoutParams::WRAP_CONTENT;
        }

        // when a background is available, we embed the content view
        // within another view that owns the background drawable
        AutoPtr<PopupViewContainer> popupViewContainer = new PopupViewContainer(mContext, this);
        AutoPtr<IFrameLayoutLayoutParams> listParams;
        CFrameLayoutLayoutParams::New(IViewGroupLayoutParams::MATCH_PARENT, height,
                (IFrameLayoutLayoutParams**)&listParams);
        popupViewContainer->SetBackgroundDrawable(mBackground);
        popupViewContainer->AddView(mContentView, (IViewGroupLayoutParams*)listParams);

        mPopupView = (IView*)(popupViewContainer->Probe(EIID_IView));
    }
    else {
        mPopupView = mContentView;
    }

    Int32 dir;
    mPopupView->GetRawLayoutDirection(&dir);
    mPopupViewInitialLayoutDirectionInherited = (dir == IView::LAYOUT_DIRECTION_INHERIT);
    mPopupWidth = ((CWindowManagerLayoutParams*)p)->mWidth;
    mPopupHeight = ((CWindowManagerLayoutParams*)p)->mHeight;

    return NOERROR;
}

void PopupWindow::InvokePopup(
    /* [in] */ IWindowManagerLayoutParams* p)
{
    if (mContext != NULL) {
        assert(p != NULL);
        String pn;
        mContext->GetPackageName(&pn);
        p->SetPackageName(pn);
    }
    mPopupView->SetFitsSystemWindows(mLayoutInsetDecor);
    SetLayoutDirectionFromAnchor();

    mWindowManager->AddView(mPopupView, p);
}

void PopupWindow::SetLayoutDirectionFromAnchor()
{
    if (mAnchor != NULL) {
        AutoPtr<IView> anchor;
        mAnchor->Resolve(EIID_IView, (IInterface**)&anchor);
        if (anchor != NULL && mPopupViewInitialLayoutDirectionInherited) {
            Int32 dir;
            anchor->GetLayoutDirection(&dir);
            mPopupView->SetLayoutDirection(dir);
        }
    }
}

AutoPtr<IWindowManagerLayoutParams> PopupWindow::CreatePopupLayout(
   /* [in] */ IBinder* token)
{
    // generates the layout parameters for the drop down
    // we want a fixed size view located at the bottom left of the anchor
    AutoPtr<CWindowManagerLayoutParams> p;
    CWindowManagerLayoutParams::NewByFriend((CWindowManagerLayoutParams**)&p);
    // these gravity settings put the view at the top left corner of the
    // screen. The view is then positioned to the appropriate location
    // by setting the x and y offsets to match the anchor's bottom
    // left corner
    p->mGravity = IGravity::START | IGravity::TOP;
    p->mWidth = mLastWidth = mWidth;
    p->mHeight = mLastHeight = mHeight;
    if (mBackground != NULL) {
        mBackground->GetOpacity(&(p->mFormat));
    }
    else {
        p->mFormat = Elastos::Droid::Graphics::IPixelFormat::TRANSLUCENT;
    }
    p->mFlags = ComputeFlags(p->mFlags);
    p->mType = mWindowLayoutType;
    p->mToken = token;
    p->mSoftInputMode = mSoftInputMode;
    AutoPtr<ICharSequence> title;
    CStringWrapper::New(String("PopupWindow:")/* + Integer.toHexString(hashCode())*/, (ICharSequence**)&title);
    p->SetTitle(title);

    return p;
}

Int32 PopupWindow::ComputeFlags(
    /* [in] */ Int32 curFlags)
{
    curFlags &= ~(
            IWindowManagerLayoutParams::FLAG_IGNORE_CHEEK_PRESSES |
            IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE |
            IWindowManagerLayoutParams::FLAG_NOT_TOUCHABLE |
            IWindowManagerLayoutParams::FLAG_WATCH_OUTSIDE_TOUCH |
            IWindowManagerLayoutParams::FLAG_LAYOUT_NO_LIMITS |
            IWindowManagerLayoutParams::FLAG_ALT_FOCUSABLE_IM |
            IWindowManagerLayoutParams::FLAG_SPLIT_TOUCH);
    if(mIgnoreCheekPress) {
        curFlags |= IWindowManagerLayoutParams::FLAG_IGNORE_CHEEK_PRESSES;
    }
    if (!mFocusable) {
        curFlags |= IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE;
        if (mInputMethodMode == IPopupWindow::INPUT_METHOD_NEEDED) {
            curFlags |= IWindowManagerLayoutParams::FLAG_ALT_FOCUSABLE_IM;
        }
    }
    else if (mInputMethodMode == IPopupWindow::INPUT_METHOD_NOT_NEEDED) {
        curFlags |= IWindowManagerLayoutParams::FLAG_ALT_FOCUSABLE_IM;
    }
    if (!mTouchable) {
        curFlags |= IWindowManagerLayoutParams::FLAG_NOT_TOUCHABLE;
    }
    if (mOutsideTouchable) {
        curFlags |= IWindowManagerLayoutParams::FLAG_WATCH_OUTSIDE_TOUCH;
    }
    if (!mClippingEnabled) {
        curFlags |= IWindowManagerLayoutParams::FLAG_LAYOUT_NO_LIMITS;
    }
    if (IsSplitTouchEnabled()) {
        curFlags |= IWindowManagerLayoutParams::FLAG_SPLIT_TOUCH;
    }
    if (mLayoutInScreen) {
        curFlags |= IWindowManagerLayoutParams::FLAG_LAYOUT_IN_SCREEN;
    }

    if (mLayoutInsetDecor) {
        curFlags |= IWindowManagerLayoutParams::FLAG_LAYOUT_INSET_DECOR;
    }
    if (mNotTouchModal) {
        curFlags |= IWindowManagerLayoutParams::FLAG_NOT_TOUCH_MODAL;
    }

    return curFlags;
}

Int32 PopupWindow::ComputeAnimationResource()
{
    if (mAnimationStyle == -1) {
        if (mIsDropdown) {
            return mAboveAnchor
                    ? R::style::Animation_DropDownUp
                    : R::style::Animation_DropDownDown;
        }
        return 0;
    }
    return mAnimationStyle;
}

Boolean PopupWindow::FindDropDownPosition(
    /* [in] */ IView* anchor,
    /* [in] */ IWindowManagerLayoutParams* _p,
    /* [in] */ Int32 xoff,
    /* [in] */ Int32 yoff)
{
    assert(anchor != NULL);
    Int32 anchorHeight;
    anchor->GetHeight(&anchorHeight);

    CWindowManagerLayoutParams* p = (CWindowManagerLayoutParams*)_p;

    Int32 x, y;
    anchor->GetLocationInWindow(&x, &y);
    (*mDrawingLocation)[0] = x;
    (*mDrawingLocation)[1] = y;

    ((CWindowManagerLayoutParams*)p)->mX = (*mDrawingLocation)[0] + xoff;
    ((CWindowManagerLayoutParams*)p)->mY = (*mDrawingLocation)[1] + anchorHeight + yoff;

    Boolean onTop = FALSE;

    p->mGravity = IGravity::START | IGravity::TOP;

    anchor->GetLocationOnScreen(&x, &y);
    (*mScreenLocation)[0] = x;
    (*mScreenLocation)[1] = y;

    AutoPtr<CRect> displayFrame;
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&displayFrame));
    anchor->GetWindowVisibleDisplayFrame((IRect*)displayFrame.Get());

    Int32 screenY = (*mScreenLocation)[1] + anchorHeight + yoff;

    AutoPtr<IView> root;
    anchor->GetRootView((IView**)&root);
    Int32 width;
    root->GetWidth(&width);
    if (screenY + mPopupHeight > displayFrame->mBottom
                || p->mX + mPopupWidth - width > 0) {
        // if the drop down disappears at the bottom of the screen. we try to
        // scroll a parent scrollview or move the drop down back up on top of
        // the edit box
        if (mAllowScrollingAnchorParent) {
            Int32 scrollX, scrollY;
            anchor->GetScrollX(&scrollX);
            anchor->GetScrollY(&scrollY);

            AutoPtr<IRect> r;
            ASSERT_SUCCEEDED(CRect::New(scrollX, scrollY,  scrollX + mPopupWidth + xoff,
                    scrollY + mPopupHeight + anchorHeight + yoff, (IRect**)&r));
            Boolean result;
            anchor->RequestRectangleOnScreen(r, TRUE, &result);
        }

        // now we re-evaluate the space available, and decide from that
        // whether the pop-up will go above or below the anchor.
        anchor->GetHeight(&anchorHeight);
        anchor->GetLocationInWindow(&x, &y);
        (*mDrawingLocation)[0] = x;
        (*mDrawingLocation)[1] = y;
        p->mX = (*mDrawingLocation)[0] + xoff;
        p->mY = (*mDrawingLocation)[1] + anchorHeight + yoff;

        // determine whether there is more space above or below the anchor
        anchor->GetHeight(&anchorHeight);
        anchor->GetLocationOnScreen(&x, &y);
        (*mScreenLocation)[0] = x;
        (*mScreenLocation)[1] = y;
        onTop = (displayFrame->mBottom - (*mScreenLocation)[1] - anchorHeight - yoff) <
                ((*mScreenLocation)[1] - yoff - displayFrame->mTop);
        if (onTop) {
            p->mGravity = IGravity::START | IGravity::BOTTOM;
            Int32 rootH;
            root->GetHeight(&rootH);
            p->mY = rootH - (*mDrawingLocation)[1] + yoff;
        }
        else {
            p->mY = (*mDrawingLocation)[1] + anchorHeight + yoff;
        }
    }

    if (mClipToScreen) {
        Int32 displayFrameWidth = displayFrame->mRight - displayFrame->mLeft;

        int right = p->mX + p->mWidth;
        if (right > displayFrameWidth) {
            p->mX -= right - displayFrameWidth;
        }
        if (p->mX < displayFrame->mLeft) {
            p->mX = displayFrame->mLeft;
            p->mWidth = Elastos::Core::Math::Min(p->mWidth, displayFrameWidth);
        }

        if (onTop) {
            Int32 popupTop = (*mScreenLocation)[1] + yoff - mPopupHeight;
            if (popupTop < 0) {
                p->mY += popupTop;
            }
        } else {
            p->mY = Elastos::Core::Math::Max(p->mY, displayFrame->mTop);
        }
    }

    p->mGravity |= IGravity::DISPLAY_CLIP_VERTICAL;

    return onTop;
}

Int32 PopupWindow::GetMaxAvailableHeight(
    /* [in] */ IView* anchor)
{
    return GetMaxAvailableHeight(anchor, 0);
}

Int32 PopupWindow::GetMaxAvailableHeight(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 yOffset)
{
    return GetMaxAvailableHeight(anchor, yOffset, FALSE);
}

Int32 PopupWindow::GetMaxAvailableHeight(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 yOffset,
    /* [in] */ Boolean ignoreBottomDecorations)
{
    assert(anchor != NULL);

    AutoPtr<CRect> displayFrame;
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&displayFrame));
    anchor->GetWindowVisibleDisplayFrame((IRect*)displayFrame.Get());

    Int32 x, y;
    anchor->GetLocationOnScreen(&x, &y);
    (*mDrawingLocation)[0] = x;
    (*mDrawingLocation)[1] = y;
    AutoPtr<ArrayOf<Int32> > anchorPos = mDrawingLocation;

    Int32 bottomEdge = displayFrame->mBottom;
    if (ignoreBottomDecorations) {
        AutoPtr<IContext> context;
        anchor->GetContext((IContext**)&context);
        AutoPtr<IResources> resources;
        context->GetResources((IResources**)&resources);
        AutoPtr<IDisplayMetrics> metrics;
        resources->GetDisplayMetrics((IDisplayMetrics**)&metrics);
        metrics->GetHeightPixels(&bottomEdge);
    }
    Int32 height;
    anchor->GetHeight(&height);
    Int32 distanceToBottom = bottomEdge - ((*anchorPos)[1] + height) - yOffset;
    Int32 distanceToTop = (*anchorPos)[1] - displayFrame->mTop + yOffset;

    // anchorPos[1] is distance from anchor to top of screen
    Int32 returnedHeight = Elastos::Core::Math::Max(distanceToBottom, distanceToTop);
    if (mBackground != NULL) {
        Boolean result;
        mBackground->GetPadding(mTempRect, &result);
        Int32 top, bottom;
        mTempRect->GetTop(&top);
        mTempRect->GetBottom(&bottom);
        returnedHeight -= top + bottom;
    }

    return returnedHeight;
}

ECode PopupWindow::Dismiss()
{
    if (IsShowing() && mPopupView != NULL) {
        mIsShowing = FALSE;
        UnregisterForScrollChanged();

        mWindowManager->RemoveViewImmediate(mPopupView);

        if (mPopupView != mContentView &&
                IViewGroup::Probe(mPopupView) != NULL) {
            IViewGroup::Probe(mPopupView)->RemoveViewInLayout(mContentView);
        }
        mPopupView = NULL;

        if (mOnDismissListener != NULL) {
            mOnDismissListener->OnDismiss();
        }
    }

    return NOERROR;
}

ECode PopupWindow::SetOnDismissListener(
    /* [in] */ IPopupWindowOnDismissListener* onDismissListener)
{
    mOnDismissListener = onDismissListener;

    return NOERROR;
}

ECode PopupWindow::Update()
{
    if (!IsShowing() || mContentView == NULL) {
        return NOERROR;
    }

    AutoPtr<IViewGroupLayoutParams> p;
    mPopupView->GetLayoutParams((IViewGroupLayoutParams**)&p);
    CWindowManagerLayoutParams* lp =
            (CWindowManagerLayoutParams*)IWindowManagerLayoutParams::Probe(p);

    Boolean update = FALSE;

    Int32 newAnim = ComputeAnimationResource();
    if (newAnim != lp->mWindowAnimations) {
        lp->mWindowAnimations = newAnim;
        update = TRUE;
    }

    Int32 newFlags = ComputeFlags(lp->mFlags);
    if (newFlags != lp->mFlags) {
        lp->mFlags = newFlags;
        update = TRUE;
    }

    if (update) {
        SetLayoutDirectionFromAnchor();
        mWindowManager->UpdateViewLayout(mPopupView, (IWindowManagerLayoutParams*)lp);
    }

    return NOERROR;
}

ECode PopupWindow::Update(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    AutoPtr<IViewGroupLayoutParams> p;
    mPopupView->GetLayoutParams((IViewGroupLayoutParams**)&p);
    CWindowManagerLayoutParams* lp =
            (CWindowManagerLayoutParams*)IWindowManagerLayoutParams::Probe(p);

    return Update(lp->mX, lp->mY, width, height, FALSE);
}

ECode PopupWindow::Update(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return Update(x, y, width, height, FALSE);
}

ECode PopupWindow::Update(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean force)
{
    if (width != -1) {
        mLastWidth = width;
        SetWidth(width);
    }

    if (height != -1) {
        mLastHeight = height;
        SetHeight(height);
    }

    if (!IsShowing() || mContentView == NULL) {
        return NOERROR;
    }

    AutoPtr<IViewGroupLayoutParams> p;
    mPopupView->GetLayoutParams((IViewGroupLayoutParams**)&p);
    CWindowManagerLayoutParams* lp =
            (CWindowManagerLayoutParams*)IWindowManagerLayoutParams::Probe(p);

    Boolean update = force;

    Int32 finalWidth = mWidthMode < 0 ? mWidthMode : mLastWidth;
    if (width != -1 && lp->mWidth != finalWidth) {
        lp->mWidth = mLastWidth = finalWidth;
        update = TRUE;
    }

    Int32 finalHeight = mHeightMode < 0 ? mHeightMode : mLastHeight;
    if (height != -1 && lp->mHeight != finalHeight) {
        lp->mHeight = mLastHeight = finalHeight;
        update = TRUE;
    }

    if (lp->mX != x) {
        lp->mX = x;
        update = TRUE;
    }

    if (lp->mY != y) {
        lp->mY = y;
        update = TRUE;
    }

    Int32 newAnim = ComputeAnimationResource();
    if (newAnim != lp->mWindowAnimations) {
        lp->mWindowAnimations = newAnim;
        update = TRUE;
    }

    Int32 newFlags = ComputeFlags(lp->mFlags);
    if (newFlags != lp->mFlags) {
        lp->mFlags = newFlags;
        update = TRUE;
    }

    if (update) {
        SetLayoutDirectionFromAnchor();
        mWindowManager->UpdateViewLayout(mPopupView, (IWindowManagerLayoutParams*)lp);
    }

    return NOERROR;
}

ECode PopupWindow::Update(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return Update(anchor, FALSE, 0, 0, TRUE, width, height);
}

ECode PopupWindow::Update(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 xoff,
    /* [in] */ Int32 yoff,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return Update(anchor, TRUE, xoff, yoff, TRUE, width, height);
}

ECode PopupWindow::Update(
    /* [in] */ IView* anchor,
    /* [in] */ Boolean updateLocation,
    /* [in] */ Int32 xoff,
    /* [in] */ Int32 yoff,
    /* [in] */ Boolean updateDimension,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (!IsShowing() || mContentView == NULL) {
        return NOERROR;
    }

    AutoPtr<IWeakReference> oldAnchor = mAnchor;
    AutoPtr<IView> oldView;
    if (oldAnchor != NULL) {
        oldAnchor->Resolve(EIID_IView, (IInterface**)&oldView);
    }

    Boolean needsUpdate = updateLocation && (mAnchorXoff != xoff || mAnchorYoff != yoff);
    if (oldAnchor == NULL || oldView.Get() != anchor || (needsUpdate && !mIsDropdown)) {
        RegisterForScrollChanged(anchor, xoff, yoff);
    }
    else if (needsUpdate) {
        // No need to register again if this is a DropDown, showAsDropDown already did.
        mAnchorXoff = xoff;
        mAnchorYoff = yoff;
    }

    AutoPtr<IViewGroupLayoutParams> p;
    mPopupView->GetLayoutParams((IViewGroupLayoutParams**)&p);
    CWindowManagerLayoutParams* lp =
            (CWindowManagerLayoutParams*)IWindowManagerLayoutParams::Probe(p);

    if (updateDimension) {
        if (width == -1) {
            width = mPopupWidth;
        }
        else {
            mPopupWidth = width;
        }
        if (height == -1) {
            height = mPopupHeight;
        }
        else {
            mPopupHeight = height;
        }
    }

    Int32 x = lp->mX;
    Int32 y = lp->mY;

    if (updateLocation) {
        UpdateAboveAnchor(FindDropDownPosition(anchor,
                (IWindowManagerLayoutParams*)lp, xoff, yoff));
    }
    else {
        UpdateAboveAnchor(FindDropDownPosition(anchor,
                (IWindowManagerLayoutParams*)lp, mAnchorXoff, mAnchorYoff));
    }

    return Update(lp->mX, lp->mY, width, height, x != lp->mX || y != lp->mY);
}

void PopupWindow::UnregisterForScrollChanged()
{
    AutoPtr<IView> anchor;
    if (mAnchor != NULL) {
        mAnchor->Resolve(EIID_IView, (IInterface**)&anchor);
    }
    if (anchor != NULL) {
        AutoPtr<IViewTreeObserver> vto;
        anchor->GetViewTreeObserver((IViewTreeObserver**)&vto);
        if (vto != NULL)
            vto->RemoveOnScrollChangedListener(mOnScrollChangedListener);
    }
    mAnchor = NULL;
}

void PopupWindow::RegisterForScrollChanged(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 xoff,
    /* [in] */ Int32 yoff)
{
    assert(anchor != NULL);

    UnregisterForScrollChanged();

    AutoPtr<IWeakReferenceSource> wrs = (IWeakReferenceSource*)anchor->Probe(EIID_IWeakReferenceSource);
    mAnchor = NULL;
    wrs->GetWeakReference((IWeakReference**)&mAnchor);

    AutoPtr<IViewTreeObserver> vto;
    anchor->GetViewTreeObserver((IViewTreeObserver**)&vto);
    if (vto != NULL) {
        vto->AddOnScrollChangedListener(mOnScrollChangedListener);
    }

    mAnchorXoff = xoff;
    mAnchorYoff = yoff;
}


}// namespace Widget
}// namespace Droid
}// namespace Elastos

