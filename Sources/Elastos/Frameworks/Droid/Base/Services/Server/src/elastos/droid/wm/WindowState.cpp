
#include "wm/WindowState.h"
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::View::CWindowManagerLayoutParams;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::CRectF;
using Elastos::Droid::Graphics::CMatrix;
using Elastos::Droid::Graphics::CRegion;
using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::CGravity;
using Elastos::Droid::View::IInternalInsetsInfo;
using Elastos::Droid::View::EIID_IWindowState;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

const String WindowState::TAG("WindowState");
const Boolean WindowState::DEBUG_VISIBILITY = CWindowManagerService::DEBUG_VISIBILITY;
const Boolean WindowState::SHOW_TRANSACTIONS = CWindowManagerService::SHOW_TRANSACTIONS;
const Boolean WindowState::SHOW_LIGHT_TRANSACTIONS = CWindowManagerService::SHOW_LIGHT_TRANSACTIONS;
const Boolean WindowState::SHOW_SURFACE_ALLOC = CWindowManagerService::SHOW_SURFACE_ALLOC;

CAR_INTERFACE_IMPL(WindowState::DeathRecipient, IProxyDeathRecipient);

WindowState::DeathRecipient::DeathRecipient(
    /* [in] */ WindowState* owner)
{
    owner->GetWeakReference((IWeakReference**)&mWeakOwner);
}

ECode WindowState::DeathRecipient::ProxyDied()
{
    AutoPtr<IWindowState> iowner;
    mWeakOwner->Resolve(EIID_IWindowState, (IInterface**)&iowner);
    if (iowner == NULL) {
        return NOERROR;
    }

    AutoPtr<WindowState> owner = (WindowState*)iowner.Get();
    Slogger::I(TAG, "WindowState:: %p, DeathRecipient::ProxyDied(): ", owner.Get());

    Autolock lock(&owner->mService->mWindowMapLock);

    AutoPtr<IBinder> binder = IBinder::Probe(owner->mClient);
    AutoPtr<WindowState> win;
    owner->mService->WindowForClientLocked(
        owner->mSession, owner->mClient, FALSE, (WindowState**)&win);
    Slogger::I(TAG, "WIN DEATH: %p", win.Get());
    if (win != NULL) {
        owner->mService->RemoveWindowLocked(owner->mSession, win);
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL(WindowState, IWindowState)

WindowState::~WindowState()
{
}

WindowState::WindowState(
    /* [in] */ CWindowManagerService* service,
    /* [in] */ CSession* s,
    /* [in] */ IIWindow* c,
    /* [in] */ WindowToken* token,
    /* [in] */ WindowState* attachedWindow,
    /* [in] */ Int32 seq,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [in] */ Int32 viewVisibility,
    /* [in] */ DisplayContent* displayContent)
    : mService(service)
    , mPolicy(service->mPolicy)
    , mContext(service->mContext)
    , mSession(s)
    , mClient(c)
    , mToken(token)
    , mAppToken(NULL)
    , mSeq(seq)
    , mViewVisibility(viewVisibility)
    , mSystemUiVisibility(0)
    , mPolicyVisibility(TRUE)
    , mPolicyVisibilityAfterAnim(TRUE)
    , mAppFreezing(FALSE)
    , mAttachedHidden(FALSE)
    , mWallpaperVisible(FALSE)
    , mRequestedWidth(0)
    , mRequestedHeight(0)
    , mLastRequestedWidth(0)
    , mLastRequestedHeight(0)
    , mLayer(0)
    , mHaveFrame(FALSE)
    , mObscured(FALSE)
    , mTurnOnScreen(FALSE)
    , mLayoutSeq(-1)
    , mConfigHasChanged(FALSE)
    , mVisibleInsetsChanged(FALSE)
    , mContentInsetsChanged(FALSE)
    , mGivenInsetsPending(FALSE)
    , mTouchableInsets(IInternalInsetsInfo::TOUCHABLE_INSETS_FRAME)
    , mGlobalScale(1)
    , mInvGlobalScale(1)
    , mHScale(1)
    , mVScale(1)
    , mLastHScale(1)
    , mLastVScale(1)
    , mContentChanged(FALSE)
    , mWallpaperX(-1)
    , mWallpaperY(-1)
    , mWallpaperXStep(-1)
    , mWallpaperYStep(-1)
    , mXOffset(0)
    , mYOffset(0)
    , mRelayoutCalled(FALSE)
    , mLayoutNeeded(FALSE)
    , mExiting(FALSE)
    , mDestroying(FALSE)
    , mRemoveOnExit(FALSE)
    , mOrientationChanging(FALSE)
    , mRemoved(FALSE)
    , mRebuilding(FALSE)
    , mWasExiting(FALSE)
    , mHasSurface(FALSE)
    , mDisplayContent(displayContent)
    , mOwnerUid(s->mUid)
    , mShowToOwnerOnly(FALSE)
{
    CWindowManagerLayoutParams::New((IWindowManagerLayoutParams**)&mAttrs);
    Int32 result;
    mAttrs->CopyFrom(attrs, &result);

    CRectF::New((IRectF**)&mShownFrame);
    CRect::New((IRect**)&mVisibleInsets);
    CRect::New((IRect**)&mLastVisibleInsets);
    CRect::New((IRect**)&mContentInsets);
    CRect::New((IRect**)&mLastContentInsets);
    CRect::New((IRect**)&mGivenContentInsets);
    CRect::New((IRect**)&mGivenVisibleInsets);
    CRegion::New((IRegion**)&mGivenTouchableRegion);
    CRect::New((IRect**)&mSystemDecorRect);
    CRect::New((IRect**)&mLastSystemDecorRect);

    CMatrix::New((IMatrix**)&mTmpMatrix);

    CRect::New((IRect**)&mFrame);
    CRect::New((IRect**)&mLastFrame);
    CRect::New((IRect**)&mCompatFrame);
    CRect::New((IRect**)&mContainingFrame);
    CRect::New((IRect**)&mDisplayFrame);
    CRect::New((IRect**)&mContentFrame);
    CRect::New((IRect**)&mParentFrame);
    CRect::New((IRect**)&mVisibleFrame);

    AutoPtr<DeathRecipient> deathRecipient = new DeathRecipient(this);

    Int32 flags;
    mAttrs->GetFlags(&flags);
    mEnforceSizeCompat = (flags & IWindowManagerLayoutParams::FLAG_COMPATIBLE_WINDOW) != 0;
    // if (WindowManagerService.localLOGV) Slog.v(
    //     TAG, "Window " + this + " client=" + c.asBinder()
    //     + " token=" + token + " (" + mAttrs.token + ")" + " params=" + a);
    // try {
    AutoPtr<IProxy> proxy = (IProxy*)c->Probe(EIID_IProxy);
    if (proxy != NULL && FAILED(proxy->LinkToDeath(deathRecipient.Get(), 0))) {
        mDeathRecipient = NULL;
        mAttachedWindow = NULL;
        mLayoutAttached = FALSE;
        mIsImWindow = FALSE;
        mIsWallpaper = FALSE;
        mIsFloatingLayer = FALSE;
        mBaseLayer = 0;
        mSubLayer = 0;
        mInputWindowHandle = NULL;
        mWinAnimator = NULL;
        return;
    }
    else {
        mDeathRecipient = deathRecipient;

        Int32 type;
        mAttrs->GetType(&type);
        if ((type >= IWindowManagerLayoutParams::FIRST_SUB_WINDOW
            && type <= IWindowManagerLayoutParams::LAST_SUB_WINDOW)) {
            // The multiplier here is to reserve space for multiple
            // windows in the same type layer.
            Int32 layer, attachedWinType;
            attachedWindow->mAttrs->GetType(&attachedWinType);
            mPolicy->WindowTypeToLayerLw(attachedWinType, &layer);
            mBaseLayer = layer * CWindowManagerService::TYPE_LAYER_MULTIPLIER +
                    CWindowManagerService::TYPE_LAYER_OFFSET;
            mPolicy->SubWindowTypeToLayerLw(type, &mSubLayer);
            mAttachedWindow = attachedWindow;
            // if (WindowManagerService.DEBUG_ADD_REMOVE)
            //     Slog.v(TAG, "Adding " + this + " to " + mAttachedWindow);
            mAttachedWindow->mChildWindows.PushBack(this);
            mLayoutAttached = type !=
                IWindowManagerLayoutParams::TYPE_APPLICATION_ATTACHED_DIALOG;
            mIsImWindow = attachedWinType == IWindowManagerLayoutParams::TYPE_INPUT_METHOD
                || attachedWinType == IWindowManagerLayoutParams::TYPE_INPUT_METHOD_DIALOG;
            mIsWallpaper = attachedWinType == IWindowManagerLayoutParams::TYPE_WALLPAPER;
            mIsFloatingLayer = mIsImWindow || mIsWallpaper;
        }
        else {
            // The multiplier here is to reserve space for multiple
            // windows in the same type layer.
            Int32 layer;
            mPolicy->WindowTypeToLayerLw(type, &layer);
            mBaseLayer = layer * CWindowManagerService::TYPE_LAYER_MULTIPLIER
                    + CWindowManagerService::TYPE_LAYER_OFFSET;
            mSubLayer = 0;
            mAttachedWindow = NULL;
            mLayoutAttached = FALSE;
            mIsImWindow = type == IWindowManagerLayoutParams::TYPE_INPUT_METHOD
                || type == IWindowManagerLayoutParams::TYPE_INPUT_METHOD_DIALOG;
            mIsWallpaper = type == IWindowManagerLayoutParams::TYPE_WALLPAPER;
            mIsFloatingLayer = mIsImWindow || mIsWallpaper;
        }

        WindowState* appWin = this;
        while (appWin->mAttachedWindow != NULL) {
            appWin = appWin->mAttachedWindow;
        }
        AutoPtr<WindowToken> appToken = appWin->mToken;
        while (appToken->mAppWindowToken == NULL) {
            AutoPtr<WindowToken> parent;
            HashMap<AutoPtr<IBinder>, AutoPtr<WindowToken> >::Iterator it = mService->mTokenMap.End();
            if (appToken->mToken != NULL)
                it = mService->mTokenMap.Find(appToken->mToken);
            if (it != mService->mTokenMap.End()) {
                parent = it->mSecond;
            }
            if ((parent == NULL) || (appToken == parent)) {
                break;
            }
            appToken = parent;
        }
        mRootToken = appToken;
        mAppToken = appToken->mAppWindowToken;

        mWinAnimator = new WindowStateAnimator(this);
        attrs->GetAlpha(&mWinAnimator->mAlpha);

        mInputWindowHandle = new InputWindowHandle(
                mAppToken != NULL ? mAppToken->mInputApplicationHandle : NULL, this,
                displayContent->GetDisplayId());
    }
}

void WindowState::Attach()
{
    // if (WindowManagerService.localLOGV) Slog.v(
    //         TAG, "Attaching " + this + " token=" + mToken
    //         + ", list=" + mToken.windows);
    mSession->WindowAddedLocked();
}

ECode WindowState::ComputeFrameLw(
    /* [in] */ IRect* pf,
    /* [in] */ IRect* df,
    /* [in] */ IRect* cf,
    /* [in] */ IRect* vf)
{
    mHaveFrame = TRUE;

    AutoPtr<IRect> container = mContainingFrame;
    container->Set(pf);

    AutoPtr<IRect> display = mDisplayFrame;
    display->Set(df);

    Int32 right, left, bottom, top;
    container->GetRight(&right);
    container->GetLeft(&left);
    container->GetBottom(&bottom);
    container->GetTop(&top);
    Int32 pw = right - left;
    Int32 ph = bottom - top;

    Int32 w, h;
    Int32 flags;
    mAttrs->GetFlags(&flags);
    if ((flags & IWindowManagerLayoutParams::FLAG_SCALED) != 0) {
        Int32 attrsW;
        mAttrs->GetWidth(&attrsW);
        if (attrsW < 0) {
            w = pw;
        }
        else if (mEnforceSizeCompat) {
            w = (Int32)(attrsW * mGlobalScale + 0.5);
        }
        else {
            w = attrsW;
        }
        Int32 attrsH;
        mAttrs->GetHeight(&attrsH);
        if (attrsH < 0) {
            h = ph;
        }
        else if (mEnforceSizeCompat) {
            h = (Int32)(attrsH * mGlobalScale + 0.5);
        } else {
            h = attrsH;
        }
    }
    else {
        Int32 attrsW;
        mAttrs->GetWidth(&attrsW);
        if (attrsW == IWindowManagerLayoutParams::MATCH_PARENT) {
            w = pw;
        }
        else if (mEnforceSizeCompat) {
            w = (Int32)(mRequestedWidth * mGlobalScale + 0.5);
        }
        else {
            w = mRequestedWidth;
        }
        Int32 attrsH;
        mAttrs->GetHeight(&attrsH);
        if (attrsH == IWindowManagerLayoutParams::MATCH_PARENT) {
            h = ph;
        }
        else if (mEnforceSizeCompat) {
            h = (Int32)(mRequestedHeight * mGlobalScale + 0.5);
        }
        else {
            h = mRequestedHeight;
        }
    }

    Boolean equals = FALSE;
    mParentFrame->Equals(pf, &equals);
    if (!equals) {
        // Int32 pl, pt, pr, pb, l, t, r, b;
        // mParentFrame->Get(&pl, &pt, &pr, &pb);
        // pf->Get(&l, &t, &r, &b);
        // Slogger::I(TAG, "Window %p content frame from (%d, %d, %d, %d) to (%d, %d, %d, %d)",
        //         this, pl, pt, pr, pb, l, t, r, b);

        mParentFrame->Set(pf);
        mContentChanged = TRUE;
    }
    if (mRequestedWidth != mLastRequestedWidth || mRequestedHeight != mLastRequestedHeight) {
        mLastRequestedWidth = mRequestedWidth;
        mLastRequestedHeight = mRequestedHeight;
        mContentChanged = TRUE;
    }

    AutoPtr<IRect> content = mContentFrame;
    content->Set(cf);

    AutoPtr<IRect> visible = mVisibleFrame;
    visible->Set(vf);

    AutoPtr<IRect> frame = mFrame;
    Int32 fw, fh;
    frame->GetWidth(&fw);
    frame->GetHeight(&fh);

    // System.out.println("In: w=" + w + " h=" + h + " container=" +
    //                    container + " x=" + mAttrs.x + " y=" + mAttrs.y);

    Float x, y;
    Int32 attrsX, attrsY;
    mAttrs->GetX(&attrsX);
    mAttrs->GetY(&attrsY);
    if (mEnforceSizeCompat) {
        x = attrsX * mGlobalScale;
        y = attrsY * mGlobalScale;
    }
    else {
        x = attrsX;
        y = attrsY;
    }

    Int32 attrsGravity;
    Float attrsHMargin, attrsVMargin;
    mAttrs->GetGravity(&attrsGravity);
    mAttrs->GetHorizontalMargin(&attrsHMargin);
    mAttrs->GetVerticalMargin(&attrsVMargin);
    AutoPtr<IGravity> gravity;
    CGravity::AcquireSingleton((IGravity**)&gravity);
    gravity->Apply(attrsGravity, w, h, container,
            (Int32)(x + attrsHMargin * pw),
            (Int32)(y + attrsVMargin * ph), frame);

    // System.out.println("Out: " + mFrame);

    // Now make sure the window fits in the overall display.
    gravity->ApplyDisplay(attrsGravity, df, frame);

    // Make sure the content and visible frames are inside of the
    // final window frame.
    Int32 contentLeft, contentTop, contentRight, contentBottom;
    content->GetLeft(&contentLeft);
    content->GetTop(&contentTop);
    content->GetRight(&contentRight);
    content->GetBottom(&contentBottom);

    Int32 visibleLeft, visibleTop, visibleRight, visibleBottom;
    visible->GetLeft(&visibleLeft);
    visible->GetTop(&visibleTop);
    visible->GetRight(&visibleRight);
    visible->GetBottom(&visibleBottom);

    Int32 frameLeft, frameTop, frameRight, frameBottom;
    frame->GetLeft(&frameLeft);
    frame->GetTop(&frameTop);
    frame->GetRight(&frameRight);
    frame->GetBottom(&frameBottom);

    if (contentLeft < frameLeft) {
        contentLeft = frameLeft;
        content->SetLeft(contentLeft);
    }
    if (contentTop < frameTop) {
        contentTop = frameTop;
        content->SetTop(contentTop);
    }
    if (contentRight > frameRight) {
        contentRight = frameRight;
        content->SetRight(contentRight);
    }
    if (contentBottom > frameBottom) {
        contentBottom = frameBottom;
        content->SetBottom(contentBottom);
    }
    if (visibleLeft < frameLeft) {
        visibleLeft = frameLeft;
        visible->SetLeft(visibleLeft);
    }
    if (visibleTop < frameTop) {
        visibleTop = frameTop;
        visible->SetTop(visibleTop);
    }
    if (visibleRight > frameRight) {
        visibleRight = frameRight;
        visible->SetRight(visibleRight);
    }
    if (visibleBottom > frameBottom) {
        visibleBottom = frameBottom;
        visible->SetBottom(visibleBottom);
    }

    mContentInsets->SetLeft(contentLeft - frameLeft);
    mContentInsets->SetTop(contentTop - frameTop);
    mContentInsets->SetRight(frameRight - contentRight);
    mContentInsets->SetBottom(frameBottom - contentBottom);

    mVisibleInsets->SetLeft(visibleLeft - frameLeft);
    mVisibleInsets->SetTop(visibleTop - frameTop);
    mVisibleInsets->SetRight(frameRight - visibleRight);
    mVisibleInsets->SetBottom(frameBottom - visibleBottom);

    mCompatFrame->Set(frame);
    if (mEnforceSizeCompat) {
        // If there is a size compatibility scale being applied to the
        // window, we need to apply this to its insets so that they are
        // reported to the app in its coordinate space.
        mContentInsets->Scale(mInvGlobalScale);
        mVisibleInsets->Scale(mInvGlobalScale);

        // Also the scaled frame that we report to the app needs to be
        // adjusted to be in its coordinate space.
        mCompatFrame->Scale(mInvGlobalScale);
    }

    Int32 fwTemp, fhTemp;
    frame->GetWidth(&fwTemp);
    frame->GetHeight(&fhTemp);
    if (mIsWallpaper && (fw != fwTemp || fh != fhTemp)) {
        AutoPtr<IDisplayInfo> displayInfo = mDisplayContent->GetDisplayInfo();
        Int32 w, h;
        displayInfo->GetAppWidth(&w);
        displayInfo->GetAppHeight(&h);
        mService->UpdateWallpaperOffsetLocked(this, w, h, FALSE);
    }

    // if (WindowManagerService.localLOGV) {
    //     //if ("com.google.android.youtube".equals(mAttrs.packageName)
    //     //        && mAttrs.type == WindowManager.LayoutParams.TYPE_APPLICATION_PANEL) {
    //         Slog.v(TAG, "Resolving (mRequestedWidth="
    //                 + mRequestedWidth + ", mRequestedheight="
    //                 + mRequestedHeight + ") to" + " (pw=" + pw + ", ph=" + ph
    //                 + "): frame=" + mFrame.toShortString()
    //                 + " ci=" + contentInsets.toShortString()
    //                 + " vi=" + visibleInsets.toShortString());
    //     //}
    // }
    return NOERROR;
}

AutoPtr<MagnificationSpec> WindowState::GetWindowMagnificationSpecLocked()
{
    AutoPtr<MagnificationSpec> spec = mDisplayContent->mMagnificationSpec;
    if (spec != NULL && !spec->IsNop()) {
        if (mAttachedWindow != NULL) {
            Boolean result;
            mPolicy->CanMagnifyWindowLw(mAttachedWindow->mAttrs, &result);
            if (!result) {
                return NULL;
            }
        }
        Boolean result;
        mPolicy->CanMagnifyWindowLw(mAttrs, &result);
        if (!result) {
            return NULL;
        }
    }
    return spec;
}

ECode WindowState::GetFrameLw(
    /* [out] */ IRect** frame)
{
    VALIDATE_NOT_NULL(frame);
    *frame = mFrame;
    REFCOUNT_ADD(*frame);
    return NOERROR;
}

ECode WindowState::GetShownFrameLw(
    /* [out] */ IRectF** shownFrame)
{
    VALIDATE_NOT_NULL(shownFrame);
    *shownFrame = mShownFrame;
    REFCOUNT_ADD(*shownFrame);
    return NOERROR;
}

ECode WindowState::GetDisplayFrameLw(
    /* [out] */ IRect** displayFrame)
{
    VALIDATE_NOT_NULL(displayFrame);
    *displayFrame = mDisplayFrame;
    REFCOUNT_ADD(*displayFrame);
    return NOERROR;
}

ECode WindowState::GetContentFrameLw(
    /* [out] */ IRect** contentFrame)
{
    VALIDATE_NOT_NULL(contentFrame);
    *contentFrame = mContentFrame;
    REFCOUNT_ADD(*contentFrame);
    return NOERROR;
}

ECode WindowState::GetVisibleFrameLw(
    /* [out] */ IRect** visibleFrame)
{
    VALIDATE_NOT_NULL(visibleFrame);
    *visibleFrame = mVisibleFrame;
    REFCOUNT_ADD(*visibleFrame);
    return NOERROR;
}

ECode WindowState::GetGivenInsetsPendingLw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mGivenInsetsPending;
    return NOERROR;
}

ECode WindowState::GetGivenContentInsetsLw(
    /* [out] */ IRect** insetsRect)
{
    VALIDATE_NOT_NULL(insetsRect);
    *insetsRect = mGivenContentInsets;
    REFCOUNT_ADD(*insetsRect);
    return NOERROR;
}

ECode WindowState::GetGivenVisibleInsetsLw(
    /* [out] */ IRect** visibleArea)
{
    VALIDATE_NOT_NULL(visibleArea);
    *visibleArea = mGivenVisibleInsets;
    REFCOUNT_ADD(*visibleArea);
    return NOERROR;
}

ECode WindowState::GetAttrs(
    /* [out] */ IWindowManagerLayoutParams** attrs)
{
    VALIDATE_NOT_NULL(attrs);
    *attrs = mAttrs;
    REFCOUNT_ADD(*attrs);
    return NOERROR;
}

ECode WindowState::GetNeedsMenuLw(
    /* [in] */ IWindowState* bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<WindowState> ws = this;
    List<AutoPtr<WindowState> >& windows = GetWindowList();
    List<AutoPtr<WindowState> >::ReverseIterator indexRIt = windows.REnd();
    while (TRUE) {
        Int32 flags;
        ws->mAttrs->GetPrivateFlags(&flags);
        if ((flags
                & IWindowManagerLayoutParams::PRIVATE_FLAG_SET_NEEDS_MENU_KEY) != 0) {
            ws->mAttrs->GetFlags(&flags);
            return (flags & IWindowManagerLayoutParams::FLAG_NEEDS_MENU_KEY) != 0;
        }
        // If we reached the bottom of the range of windows we are considering,
        // assume no menu is needed.
        if (ws.Get() == bottom) {
            *result = FALSE;
            return NOERROR;
        }
        // The current window hasn't specified whether menu key is needed;
        // look behind it.
        // First, we may need to determine the starting position.
        List<AutoPtr<WindowState> >::ReverseIterator rit;
        if (indexRIt == windows.REnd()) {
            for (rit = windows.RBegin(); rit != windows.REnd(); ++rit) {
                if (*rit == ws) {
                    indexRIt = rit;
                    break;
                }
            }
        }
        indexRIt++;
        if (indexRIt == windows.REnd()) {
            *result = FALSE;
            return NOERROR;
        }
        ws = *indexRIt;
    }
}

ECode WindowState::GetSystemUiVisibility(
    /* [out] */ Int32* flag)
{
    VALIDATE_NOT_NULL(flag);
    *flag = mSystemUiVisibility;
    return NOERROR;
}

ECode WindowState::GetSurfaceLayer(
    /* [out] */ Int32* surfaceLayer)
{
    VALIDATE_NOT_NULL(surfaceLayer);
    *surfaceLayer = mLayer;
    return NOERROR;
}

ECode WindowState::GetAppToken(
    /* [out] */ IApplicationToken** token)
{
    VALIDATE_NOT_NULL(token);
    *token = mAppToken != NULL ? mAppToken->mAppToken : NULL;
    REFCOUNT_ADD(*token);
    return NOERROR;
}

Int32 WindowState::GetDisplayId()
{
    return mDisplayContent->GetDisplayId();
}

Int64 WindowState::GetInputDispatchingTimeoutNanos()
{
    return mAppToken != NULL
            ? mAppToken->mInputDispatchingTimeoutNanos
            : CWindowManagerService::DEFAULT_INPUT_DISPATCHING_TIMEOUT_NANOS;
}

ECode WindowState::HasAppShownWindows(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mAppToken != NULL && (mAppToken->mFirstWindowDrawn || mAppToken->mStartingDisplayed);
    return NOERROR;
}

Boolean WindowState::IsIdentityMatrix(
    /* [in] */ Float dsdx,
    /* [in] */ Float dtdx,
    /* [in] */ Float dsdy,
    /* [in] */ Float dtdy)
{
    if (dsdx < 0.99999f || dsdx > 1.00001f) return FALSE;
    if (dtdy < 0.99999f || dtdy > 1.00001f) return FALSE;
    if (dtdx < -0.000001f || dtdx > 0.000001f) return FALSE;
    if (dsdy < -0.000001f || dsdy > 0.000001f) return FALSE;
    return TRUE;
}

void WindowState::Prelayout()
{
    if (mEnforceSizeCompat) {
        mGlobalScale = mService->mCompatibleScreenScale;
        mInvGlobalScale = 1 / mGlobalScale;
    }
    else {
        mGlobalScale = mInvGlobalScale = 1;
    }
}

ECode WindowState::IsVisibleLw(
    /* [out] */ Boolean* isVisible)
{
    VALIDATE_NOT_NULL(isVisible);
    AutoPtr<AppWindowToken> atoken = mAppToken;
    *isVisible = mHasSurface && mPolicyVisibility && !mAttachedHidden
            && (atoken == NULL || !atoken->mHiddenRequested)
            && !mExiting && !mDestroying;
    return NOERROR;
}

ECode WindowState::IsVisibleOrBehindKeyguardLw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (mRootToken->mWaitingToShow &&
            mService->mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET) {
        return FALSE;
    }
    AutoPtr<AppWindowToken> atoken = mAppToken;
    Boolean animating = atoken != NULL
            ? (atoken->mAppAnimator->mAnimation != NULL) : FALSE;
    *result = mHasSurface && !mDestroying && !mExiting
            && (atoken == NULL ? mPolicyVisibility : !atoken->mHiddenRequested)
            && ((!mAttachedHidden && mViewVisibility == IView::VISIBLE
                            && !mRootToken->mHidden)
                    || mWinAnimator->mAnimation != NULL || animating);
    return NOERROR;
}

Boolean WindowState::IsWinVisibleLw()
{
    AutoPtr<AppWindowToken> atoken = mAppToken;
    return mHasSurface && mPolicyVisibility && !mAttachedHidden
            && (atoken == NULL || !atoken->mHiddenRequested || atoken->mAppAnimator->mAnimating)
            && !mExiting && !mDestroying;
}

Boolean WindowState::IsVisibleNow()
{
    return mHasSurface && mPolicyVisibility && !mAttachedHidden
            && !mRootToken->mHidden && !mExiting && !mDestroying;
}

Boolean WindowState::IsPotentialDragTarget()
{
    return IsVisibleNow() && !mRemoved
            && mInputChannel != NULL && mInputWindowHandle != NULL;
}

Boolean WindowState::IsVisibleOrAdding()
{
    AutoPtr<AppWindowToken> atoken = mAppToken;
    return (mHasSurface || (!mRelayoutCalled && mViewVisibility == IView::VISIBLE))
            && mPolicyVisibility && !mAttachedHidden
            && (atoken == NULL || !atoken->mHiddenRequested)
            && !mExiting && !mDestroying;
}

Boolean WindowState::IsOnScreen()
{
    if (!mHasSurface || !mPolicyVisibility || mDestroying) {
        return FALSE;
    }
    AutoPtr<AppWindowToken> atoken = mAppToken;
    if (atoken != NULL) {
        return ((!mAttachedHidden && !atoken->mHiddenRequested)
                || mWinAnimator->mAnimation != NULL || atoken->mAppAnimator->mAnimation != NULL);
    }
    return !mAttachedHidden || mWinAnimator->mAnimation != NULL;
}

Boolean WindowState::IsReadyForDisplay()
{
    if (mRootToken->mWaitingToShow &&
            mService->mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET) {
        return FALSE;
    }
    return mHasSurface && mPolicyVisibility && !mDestroying
                && ((!mAttachedHidden && mViewVisibility == IView::VISIBLE
                                && !mRootToken->mHidden)
                        || mWinAnimator->mAnimation != NULL
                        || ((mAppToken != NULL) && (mAppToken->mAppAnimator->mAnimation != NULL)));
}

Boolean WindowState::IsReadyForDisplayIgnoringKeyguard()
{
    if (mRootToken->mWaitingToShow &&
                mService->mNextAppTransition != IWindowManagerPolicy::TRANSIT_UNSET) {
        return FALSE;
    }
    AutoPtr<AppWindowToken> atoken = mAppToken;
    if (atoken == NULL && !mPolicyVisibility) {
        // If this is not an app window, and the policy has asked to force
        // hide, then we really do want to hide.
        return FALSE;
    }
    return mHasSurface && !mDestroying
            && ((!mAttachedHidden && mViewVisibility == IView::VISIBLE
                            && !mRootToken->mHidden)
                    || mWinAnimator->mAnimation != NULL
                    || ((atoken != NULL) && (atoken->mAppAnimator->mAnimation != NULL)
                            && !mWinAnimator->IsDummyAnimation()));
}

ECode WindowState::IsDisplayedLw(
    /* [out] */ Boolean* isDisplayed)
{
    VALIDATE_NOT_NULL(isDisplayed);
    AutoPtr<AppWindowToken> atoken = mAppToken;
    *isDisplayed = IsDrawnLw() && mPolicyVisibility
        && ((!mAttachedHidden &&
                (atoken == NULL || !atoken->mHiddenRequested))
                    || mWinAnimator->mAnimating
                    || (atoken != NULL && atoken->mAppAnimator->mAnimation != NULL));
    return NOERROR;
}

ECode WindowState::IsAnimatingLw(
    /* [out] */ Boolean* isAnimating)
{
    VALIDATE_NOT_NULL(isAnimating);
    *isAnimating = mWinAnimator->mAnimation != NULL;
    return NOERROR;
}

ECode WindowState::IsGoneForLayoutLw(
    /* [out] */ Boolean* isGone)
{
    VALIDATE_NOT_NULL(isGone);
    AutoPtr<AppWindowToken> atoken = mAppToken;
    *isGone = mViewVisibility == IView::GONE
            || !mRelayoutCalled
            || (atoken == NULL && mRootToken->mHidden)
            || (atoken != NULL && (atoken->mHiddenRequested || atoken->mHidden))
            || mAttachedHidden
            || mExiting || mDestroying;
    return NOERROR;
}

Boolean WindowState::IsDrawnLw()
{
    return mHasSurface && !mDestroying &&
            (mWinAnimator->mDrawState == WindowStateAnimator::READY_TO_SHOW
            || mWinAnimator->mDrawState == WindowStateAnimator::HAS_DRAWN);
}

Boolean WindowState::IsOpaqueDrawn()
{
    Int32 format, type;
    mAttrs->GetFormat(&format);
    mAttrs->GetType(&type);
    return (format == IPixelFormat::OPAQUE
            || type == IWindowManagerLayoutParams::TYPE_WALLPAPER)
            && IsDrawnLw() && mWinAnimator->mAnimation == NULL
            && (mAppToken == NULL || mAppToken->mAppAnimator->mAnimation == NULL);
}

Boolean WindowState::ShouldAnimateMove()
{
    Int32 top, lastTop, left, lastLeft;
    mFrame->GetTop(&top);
    mFrame->GetLeft(&left);
    mLastFrame->GetTop(&lastTop);
    mLastFrame->GetLeft(&lastLeft);
    return mContentChanged && !mExiting && !mWinAnimator->mLastHidden && mService->OkToDisplay()
            && (top != lastTop || left != lastLeft)
            && (mAttachedWindow == NULL || !mAttachedWindow->ShouldAnimateMove());
}

Boolean WindowState::IsFullscreen(
    /* [in] */ Int32 screenWidth,
    /* [in] */ Int32 screenHeight)
{
    Int32 left, top, right, bottom;
    mFrame->GetLeft(&left);
    mFrame->GetTop(&top);
    mFrame->GetRight(&right);
    mFrame->GetBottom(&bottom);
    return left <= 0 && top <= 0 && right >= screenWidth && bottom >= screenHeight;
}

Boolean WindowState::IsConfigChanged()
{
    Int32 result;
    Boolean configChanged = mConfiguration != mService->mCurConfiguration
            && (mConfiguration == NULL || (mConfiguration->Diff(mService->mCurConfiguration, &result), result != 0));

    Int32 type;
    mAttrs->GetType(&type);
    if (type == IWindowManagerLayoutParams::TYPE_KEYGUARD) {
        // Retain configuration changed status until resetConfiguration called.
        mConfigHasChanged |= configChanged;
        configChanged = mConfigHasChanged;
    }

    return configChanged;
}

Boolean WindowState::IsConfigDiff(
    /* [in] */ Int32 mask)
{
    Int32 result;
    return mConfiguration != mService->mCurConfiguration
            && mConfiguration != NULL && ((mConfiguration->Diff(mService->mCurConfiguration, &result), result) & mask) != 0;
}

void WindowState::RemoveLocked()
{
    DisposeInputChannel();

    if (mAttachedWindow != NULL) {
        // if (WindowManagerService.DEBUG_ADD_REMOVE) Slog.v(TAG, "Removing " + this + " from " + mAttachedWindow);
        mAttachedWindow->mChildWindows.Remove(this);
    }
    mWinAnimator->DestroyDeferredSurfaceLocked(FALSE);
    mWinAnimator->DestroySurfaceLocked(FALSE);
    mSession->WindowRemovedLocked();
    AutoPtr<IProxy> proxy = (IProxy*)mClient->Probe(EIID_IProxy);
    if (proxy != NULL) {
        Boolean res;
        proxy->UnlinkToDeath(mDeathRecipient.Get(), 0, &res);
    }
}

ECode WindowState::SetConfiguration(
    /* [in] */ IConfiguration* newConfig)
{
    mConfiguration = newConfig;
    mConfigHasChanged = FALSE;
    return NOERROR;
}

void WindowState::SetInputChannel(
    /* [in] */ IInputChannel* inputChannel)
{
    // if (mInputChannel != null) {
    //     throw new IllegalStateException("Window already has an input channel.");
    // }
    assert(mInputChannel == NULL);

    mInputChannel = inputChannel;
    mInputWindowHandle->mInputChannel = inputChannel;
}

void WindowState::DisposeInputChannel()
{
    if (mInputChannel != NULL) {
        mService->mInputManager->UnregisterInputChannel(mInputChannel);

        mInputChannel->Dispose();
        mInputChannel = NULL;
    }

    mInputWindowHandle->mInputChannel = NULL;
}

Boolean WindowState::CanReceiveKeys()
{
    Int32 flags;
    mAttrs->GetFlags(&flags);
    return IsVisibleOrAdding()
            && (mViewVisibility == IView::VISIBLE)
            && ((flags & IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE) == 0);
}

ECode WindowState::HasDrawnLw(
    /* [out] */ Boolean* drawn)
{
    VALIDATE_NOT_NULL(drawn);
    *drawn = mWinAnimator->mDrawState == WindowStateAnimator::HAS_DRAWN;
    return NOERROR;
}

ECode WindowState::ShowLw(
    /* [in] */ Boolean doAnimation,
    /* [out] */ Boolean* canShown)
{
    VALIDATE_NOT_NULL(canShown);
    *canShown = ShowLw(doAnimation, TRUE);
    return NOERROR;
}

Boolean WindowState::ShowLw(
    /* [in] */ Boolean doAnimation,
    /* [in] */ Boolean requestAnim)
{
    if (IsHiddenFromUserLocked()) {
        Int32 type;
        mAttrs->GetType(&type);
        Slogger::W(TAG, "current user violation %d trying to display %p, type %d, belonging to %d"
                , mService->mCurrentUserId, this, type, mOwnerUid);
        return FALSE;
    }
    if (mPolicyVisibility && mPolicyVisibilityAfterAnim) {
        // Already showing.
        return FALSE;
    }
    // if (DEBUG_VISIBILITY) Slog.v(TAG, "Policy visibility true: " + this);
    if (doAnimation) {
        // if (DEBUG_VISIBILITY) Slog.v(TAG, "doAnimation: mPolicyVisibility="
        //         + mPolicyVisibility + " mAnimation=" + mWinAnimator.mAnimation);
        if (!mService->OkToDisplay()) {
            doAnimation = FALSE;
        }
        else if (mPolicyVisibility && mWinAnimator->mAnimation == NULL) {
            // Check for the case where we are currently visible and
            // not animating; we do not want to do animation at such a
            // point to become visible when we already are.
            doAnimation = FALSE;
        }
    }
    mPolicyVisibility = TRUE;
    mPolicyVisibilityAfterAnim = TRUE;
    if (doAnimation) {
        mWinAnimator->ApplyAnimationLocked(IWindowManagerPolicy::TRANSIT_ENTER, TRUE);
    }
    if (requestAnim) {
        mService->UpdateLayoutToAnimationLocked();
    }
    return TRUE;
}

ECode WindowState::HideLw(
    /* [in] */ Boolean doAnimation,
    /* [out] */ Boolean* canHidden)
{
    VALIDATE_NOT_NULL(canHidden);
    *canHidden = HideLw(doAnimation, TRUE);
    return NOERROR;
}

Boolean WindowState::HideLw(
    /* [in] */ Boolean doAnimation,
    /* [in] */ Boolean requestAnim)
{
    if (doAnimation) {
        if (!mService->OkToDisplay()) {
            doAnimation = FALSE;
        }
    }
    Boolean current = doAnimation ? mPolicyVisibilityAfterAnim
            : mPolicyVisibility;
    if (!current) {
        // Already hiding.
        return FALSE;
    }
    if (doAnimation) {
        mWinAnimator->ApplyAnimationLocked(IWindowManagerPolicy::TRANSIT_EXIT, FALSE);
        if (mWinAnimator->mAnimation == NULL) {
            doAnimation = FALSE;
        }
    }
    if (doAnimation) {
        mPolicyVisibilityAfterAnim = FALSE;
    }
    else {
        // if (DEBUG_VISIBILITY) Slog.v(TAG, "Policy visibility false: " + this);
        mPolicyVisibilityAfterAnim = FALSE;
        mPolicyVisibility = FALSE;
        // Window is no longer visible -- make sure if we were waiting
        // for it to be displayed before enabling the display, that
        // we allow the display to be enabled now.
        mService->EnableScreenIfNeededLocked();
        if (mService->mCurrentFocus.Get() == this) {
            mService->mFocusMayChange = TRUE;
        }
    }
    if (requestAnim) {
        mService->UpdateLayoutToAnimationLocked();
    }
    return TRUE;
}

ECode WindowState::IsAlive(
    /* [out] */ Boolean* isAlive)
{
    // VALIDATE_NOT_NULL(isAlive);
    // *isAlive = mClient.asBinder().isBinderAlive();
    return E_NOT_IMPLEMENTED;
}

Boolean WindowState::IsClosing()
{
    List<AutoPtr<AppWindowToken> >::Iterator it = mService->mClosingApps.Begin();
    for (; it != mService->mClosingApps.End(); ++it) {
        if ((*it).Get() == mAppToken) break;
    }
    return mExiting || (it != mService->mClosingApps.End());
}

ECode WindowState::IsDefaultDisplay(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mDisplayContent->mIsDefaultDisplay;
    return NOERROR;
}

void WindowState::SetShowToOwnerOnlyLocked(
    /* [in] */ Boolean showToOwnerOnly)
{
    mShowToOwnerOnly = showToOwnerOnly;
}

Boolean WindowState::IsHiddenFromUserLocked()
{
    // Attached windows are evaluated based on the window that they are attached to.
    AutoPtr<WindowState> win = this;
    while (win->mAttachedWindow != NULL) {
        win = win->mAttachedWindow;
    }
    Int32 type;
    win->mAttrs->GetType(&type);
    if (type < IWindowManagerLayoutParams::FIRST_SYSTEM_WINDOW
            && win->mAppToken != NULL && win->mAppToken->mShowWhenLocked) {
        // Save some cycles by not calling getDisplayInfo unless it is an application
        // window intended for all users.
        AutoPtr<IDisplayInfo> displayInfo = win->mDisplayContent->GetDisplayInfo();
        Int32 left, top, right, bottom, appWidth, appHeight;
        win->mFrame->GetLeft(&left);
        win->mFrame->GetTop(&top);
        win->mFrame->GetRight(&right);
        win->mFrame->GetBottom(&bottom);
        displayInfo->GetAppWidth(&appWidth);
        displayInfo->GetAppHeight(&appHeight);
        if (left <= 0 && top <= 0 && right >= appWidth
                && bottom >= appHeight) {
            // Is a fullscreen window, like the clock alarm. Show to everyone.
            return FALSE;
        }
    }

    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    Int32 id;
    helper->GetMyUserId(&id);
    return win->mShowToOwnerOnly && id != mService->mCurrentUserId;
}

void WindowState::ApplyInsets(
    /* [in] */ IRegion* outRegion,
    /* [in] */ IRect* frame,
    /* [in] */ IRect* inset)
{
    Int32 left, top, right, bottom, insetLeft, insetTop, insetRight, insetBottom;
    frame->GetLeft(&left);
    frame->GetTop(&top);
    frame->GetRight(&right);
    frame->GetBottom(&bottom);
    inset->GetLeft(&insetLeft);
    inset->GetTop(&insetTop);
    inset->GetRight(&insetRight);
    inset->GetBottom(&insetBottom);
    Boolean result;
    outRegion->Set(left + insetLeft, top + insetTop,
            right - insetRight, bottom - insetBottom, &result);
}

void WindowState::GetTouchableRegion(
    /* [in] */ IRegion* outRegion)
{
    AutoPtr<IRect> frame = mFrame;
    switch (mTouchableInsets) {
        default:
        case IInternalInsetsInfo::TOUCHABLE_INSETS_FRAME:
            Boolean result;
            outRegion->Set(frame, &result);
            break;
        case IInternalInsetsInfo::TOUCHABLE_INSETS_CONTENT:
            ApplyInsets(outRegion, frame, mGivenContentInsets);
            break;
        case IInternalInsetsInfo::TOUCHABLE_INSETS_VISIBLE:
            ApplyInsets(outRegion, frame, mGivenVisibleInsets);
            break;
        case IInternalInsetsInfo::TOUCHABLE_INSETS_REGION: {
            AutoPtr<IRegion> givenTouchableRegion = mGivenTouchableRegion;
            Boolean result;
            outRegion->Set(givenTouchableRegion, &result);
            Int32 left, top;
            frame->GetLeft(&left);
            frame->GetTop(&top);
            outRegion->Translate(left, top);
            break;
        }
    }
}

List<AutoPtr<WindowState> >& WindowState::GetWindowList()
{
    return mDisplayContent->GetWindowList();
}

String WindowState::MakeInputChannelName()
{
    String s = StringUtils::ToString((Int32)this);
    s += " ";
    AutoPtr<ICharSequence> cs;
    mAttrs->GetTitle((ICharSequence**)&cs);
    String aStr;
    cs->ToString(&aStr);
    s += aStr;
    return s;
}

ECode WindowState::ToString(
    /* [out]*/ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = ToString();
    return NOERROR;
}

String WindowState::ToString()
{
    AutoPtr<ICharSequence> title;
    mAttrs->GetTitle((ICharSequence**)&title);
    if (mStringNameCache.IsNull() || mLastTitle.Get() != title.Get()
            || mWasExiting != mExiting) {
        mLastTitle = title;
        mWasExiting = mExiting;
        StringBuilder sb("Window{");
        sb += (Int32)this;
        sb += " u";
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        Int32 uid;
        helper->GetUserId(mSession->mUid, &uid);
        sb += uid;
        sb += " ";
        sb.AppendCharSequence(mLastTitle.Get());
        sb += mExiting ? " EXITING}" : "}";
        mStringNameCache = sb.ToString();
    }
    return mStringNameCache;
}

} // Wm
} // Server
} // Droid
} // Elastos
