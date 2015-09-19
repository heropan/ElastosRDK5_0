
#include "view/SurfaceView.h"
#include "view/ViewRootImpl.h"
#include "view/CSurfaceView.h"
#include "view/CSurfaceViewWindow.h"
#include "view/CWindowManagerLayoutParams.h"
#include "content/res/CConfiguration.h"
#include "graphics/PixelFormat.h"
#include "os/SystemClock.h"
#include "os/SystemProperties.h"
#include <elastos/utility/logging/Logger.h>
#include "provider/Settings.h"

using Elastos::Core::CString;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::Res::ICompatibilityInfo;
using Elastos::Droid::Content::Res::CConfiguration;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Graphics::PorterDuffMode;
using Elastos::Droid::Provider::ISettingsSystem;
using Elastos::Droid::Provider::Settings;

namespace Elastos {
namespace Droid {
namespace View {

Boolean SurfaceView::mGameloftNeedCompat = FALSE;
Boolean SurfaceView::mMotionEventMayNeedAdjust = FALSE;

const char* TAG = "SurfaceView";
const Boolean SurfaceView::DEBUG = FALSE;

const Int32 SurfaceView::KEEP_SCREEN_ON_MSG = 1;
const Int32 SurfaceView::GET_NEW_SURFACE_MSG = 2;
const Int32 SurfaceView::UPDATE_WINDOW_MSG = 3;
Int32 SurfaceView::mScreenWidth = 0;
Int32 SurfaceView::mScreenHeight = 0;
Int32 SurfaceView::mGameSurfaceWidth = 900;
Int32 SurfaceView::mGameSurfaceHeight = 600;
Int32 SurfaceView::mScreenOrientation = -1;

Boolean SurfaceView::mAdapterMode = FALSE;

ECode SurfaceView::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, arg1;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);

    switch(what) {
        case SurfaceView::KEEP_SCREEN_ON_MSG: {
            mHost->SetKeepScreenOn(arg1 != 0);
        } break;
        case SurfaceView::GET_NEW_SURFACE_MSG: {
            mHost->HandleGetNewSurface();
        } break;
        case SurfaceView::UPDATE_WINDOW_MSG: {
            mHost->UpdateWindow(FALSE, FALSE);
        } break;
    }

    return NOERROR;
}

//==============================================================================
//          SurfaceView::_SurfaceHolder
//==============================================================================
CAR_INTERFACE_IMPL(SurfaceView::_SurfaceHolder, ISurfaceHolder)

SurfaceView::_SurfaceHolder::_SurfaceHolder(
    /* [in] */ SurfaceView* host)
    : mHost(host)
{}

SurfaceView::_SurfaceHolder::~_SurfaceHolder()
{}

ECode SurfaceView::_SurfaceHolder::IsCreating(
    /* [in] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHost->mIsCreating;
    return NOERROR;
}

ECode SurfaceView::_SurfaceHolder::AddCallback(
    /* [in] */ ISurfaceHolderCallback* callback)
{
    AutoLock lock(mHost->mCallbacksLock);
    AutoPtr<ISurfaceHolderCallback> cb = callback;
    // This is a linear search, but in practice we'll
    // have only a couple callbacks, so it doesn't matter.
    List<AutoPtr<ISurfaceHolderCallback> >::Iterator iter
        = Find(mHost->mCallbacks.Begin(), mHost->mCallbacks.End(), cb);
    if (iter == mHost->mCallbacks.End()) {
        mHost->mCallbacks.PushBack(cb);
    }

    return NOERROR;
}

ECode SurfaceView::_SurfaceHolder::RemoveCallback(
    /* [in] */ ISurfaceHolderCallback* callback)
{
    AutoLock lock(mHost->mCallbacksLock);
    AutoPtr<ISurfaceHolderCallback> cb = callback;
    List<AutoPtr<ISurfaceHolderCallback> >::Iterator iter
        = Find(mHost->mCallbacks.Begin(), mHost->mCallbacks.End(), cb);
    if (iter != mHost->mCallbacks.End()) {
        mHost->mCallbacks.Erase(iter);
    }

    return NOERROR;
}

ECode SurfaceView::_SurfaceHolder::SetFixedSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (mHost->mRequestedWidth != width || mHost->mRequestedHeight != height) {
        mHost->mRequestedWidth = width;
        mHost->mRequestedHeight = height;
        mHost->RequestLayout();
    }

    return NOERROR;
}

ECode SurfaceView::_SurfaceHolder::SetSizeFromLayout()
{
    if (mHost->mRequestedWidth != -1 || mHost->mRequestedHeight != -1) {
        mHost->mRequestedWidth = mHost->mRequestedHeight = -1;
        mHost->RequestLayout();
    }

    return NOERROR;
}

ECode SurfaceView::_SurfaceHolder::SetFormat(
    /* [in] */ Int32 format)
{
    // for backward compatibility reason, OPAQUE always
    // means 565 for SurfaceView
    if (format == IPixelFormat::OPAQUE) {
        format = IPixelFormat::RGB_565;
    }

    mHost->mRequestedFormat = format;
    if (mHost->mWindow != NULL) {
        mHost->UpdateWindow(FALSE, FALSE);
    }

    return NOERROR;
}

ECode SurfaceView::_SurfaceHolder::SetType(
    /* [in] */ Int32 type)
{
    return NOERROR;
}

ECode SurfaceView::_SurfaceHolder::SetKeepScreenOn(
    /* [in] */ Boolean screenOn)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(SurfaceView::KEEP_SCREEN_ON_MSG,
        screenOn ? 1 : 0, 0, (IMessage**)&msg);
    Boolean result;
    return mHost->mHandler->SendMessage(msg, &result);
}

ECode SurfaceView::_SurfaceHolder::LockCanvas(
    /* [out] */ ICanvas** canvas)
{
    VALIDATE_NOT_NULL(canvas);
    AutoPtr<ICanvas> _canvas = InternalLockCanvas(NULL);
    *canvas = _canvas.Get();
    REFCOUNT_ADD(*canvas);
    return NOERROR;
}

ECode SurfaceView::_SurfaceHolder::LockCanvas(
    /* [in]*/ IRect* dirty,
    /* [out]*/ ICanvas** canvas)
{
    VALIDATE_NOT_NULL(canvas);
    AutoPtr<ICanvas> _canvas = InternalLockCanvas(dirty);
    *canvas = _canvas.Get();
    REFCOUNT_ADD(*canvas);
    return NOERROR;
}

ECode SurfaceView::_SurfaceHolder::UnlockCanvasAndPost(
    /* [in]*/ ICanvas* canvas)
{
    mHost->mSurface->UnlockCanvasAndPost(canvas);
    mHost->mSurfaceLock.Unlock();
    return NOERROR;
}

ECode SurfaceView::_SurfaceHolder::GetSurface(
    /* [out]*/ ISurface** surface)
{
    VALIDATE_NOT_NULL(surface);
    *surface = mHost->mSurface;
    REFCOUNT_ADD(*surface);
    return NOERROR;
}

ECode SurfaceView::_SurfaceHolder::GetSurfaceFrame(
    /* [out]*/ IRect** rect)
{
    VALIDATE_NOT_NULL(rect);
    *rect = mHost->mSurfaceFrame;
    REFCOUNT_ADD(*rect);
    return NOERROR;
}

AutoPtr<ICanvas> SurfaceView::_SurfaceHolder::InternalLockCanvas(
    /* [in]*/ IRect* _dirty)
{
    AutoLock lock(mHost->mSurfaceLock);

    if (DEBUG) {
        Logger::I("SurfaceView", "Locking canvas... stopped=%d, win=%p\n",
            mHost->mDrawingStopped, mHost->mWindow.Get());
    }

    AutoPtr<IRect> dirty = _dirty;
    AutoPtr<ICanvas> c;
    if (!mHost->mDrawingStopped && mHost->mWindow != NULL) {
        if (dirty == NULL) {
            if (mHost->mTmpDirty == NULL) {
                CRect::New((IRect**)&mHost->mTmpDirty);
            }

            mHost->mTmpDirty->Set(mHost->mSurfaceFrame);
            dirty = mHost->mTmpDirty;
        }

        ECode ec = mHost->mSurface->LockCanvas(dirty, (ICanvas**)&c);
        if (FAILED(ec)) {
            Logger::E("SurfaceView", "Exception locking surface %08x", ec);
        }
    }

    if (DEBUG) Logger::I("SurfaceView", "Returned canvas: %p", c.Get());
    if (c != NULL) {
        mHost->mLastLockTime = SystemClock::GetUptimeMillis();
        return c;
    }

    // If the Surface is not ready to be drawn, then return NULL,
    // but throttle calls to this function so it isn't called more
    // than every 100ms.
    Int64 now = SystemClock::GetUptimeMillis();
    Int64 nextTime = mHost->mLastLockTime + 100;
    if (nextTime > now) {
        usleep(nextTime-now);
        now = SystemClock::GetUptimeMillis();
    }

    mHost->mLastLockTime = now;

    return c;
}

//==============================================================================
//          SurfaceView::_OnPreDrawListener
//==============================================================================

CAR_INTERFACE_IMPL(SurfaceView::_OnPreDrawListener, IOnPreDrawListener)

SurfaceView::_OnPreDrawListener::_OnPreDrawListener(
    /* [in] */ SurfaceView* host)
    : mHost(host)
{
}

SurfaceView::_OnPreDrawListener::~_OnPreDrawListener()
{
}

ECode SurfaceView::_OnPreDrawListener::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // reposition ourselves where the surface is
    mHost->mHaveFrame = mHost->GetWidth() > 0 && mHost->GetHeight() > 0;
    mHost->UpdateWindow(FALSE, FALSE);
    *result = TRUE;
    return NOERROR;
}

//==============================================================================
//          SurfaceView::_OnScrollChangedListener
//==============================================================================

CAR_INTERFACE_IMPL(SurfaceView::_OnScrollChangedListener, IOnScrollChangedListener)

SurfaceView::_OnScrollChangedListener::_OnScrollChangedListener(
    /* [in] */ SurfaceView* host)
    : mHost(host)
{
}

SurfaceView::_OnScrollChangedListener::~_OnScrollChangedListener()
{
}

ECode SurfaceView::_OnScrollChangedListener::OnScrollChanged()
{
    mHost->UpdateWindow(FALSE, FALSE);
    return NOERROR;
}

//==============================================================================
//          SurfaceView
//==============================================================================

void SurfaceView::TestGameloftNeedAdjust()
{
    String pckname;
    String substr("gameloft");
    AutoPtr<IApplicationInfo> appInfo;
    Int32 index;
    AutoPtr<IContext> ctx = GetContext();
    ctx->GetPackageName(&pckname);

    //Log.d(TAG,"pckname = " + pckname);

    index  = pckname.IndexOf(substr);
    if(index <= 0)
    {
        return ;
    }
    AutoPtr<IPackageManager> pm ;
    ctx->GetPackageManager((IPackageManager**)&pm);

    AutoPtr<IContentResolver> cr;
    ctx->GetContentResolver((IContentResolver**)&cr);

    mAdapterMode = Settings::System::GetInt32(cr, ISettingsSystem::DISPLAY_ADAPTION_ENABLE, 0) == 1;
    // try
    // {
    mGameloftNeedCompat = FALSE;
    ECode ec = pm->GetApplicationInfo(pckname, 0, (IApplicationInfo**)&appInfo);
    if(FAILED(ec))
        return;

    Int32 flags;
    appInfo->GetFlags(&flags);
    if((flags & IApplicationInfo::FLAG_SUPPORTS_XLARGE_SCREENS) != 0)
    {
        mGameloftNeedCompat = FALSE;
    }
    else if(index >= 0 && (mAdapterMode ==TRUE))
    {
        AutoPtr<IInterface> wmTemp;
        ctx->GetSystemService(IContext::WINDOW_SERVICE, (IInterface**)&wmTemp);
        AutoPtr<IWindowManager> wm = IWindowManager::Probe(wmTemp);
        AutoPtr<IDisplay> mDisplay;
        wm->GetDefaultDisplay((IDisplay**)&mDisplay);
        if (SystemProperties::GetInt32(String("ro.sf.hwrotation"), 0) == 270)
        {
            Int32 orientation;
            mDisplay->GetOrientation(&orientation);
            mScreenOrientation = (orientation +3) % 4;
        }
        else
        {
             mDisplay->GetOrientation(&mScreenOrientation);
        }
        mDisplay->GetWidth(&mScreenWidth);
        mDisplay->GetHeight(&mScreenHeight);
        mGameSurfaceWidth  =800;
        mGameSurfaceHeight =480;
        mGameloftNeedCompat = TRUE;

    }
    else
    {
        mGameloftNeedCompat = FALSE;
    }
    // } catch (NameNotFoundException e) {

    //     mGameloftNeedCompat = false;
    // }

  // Log.i(TAG, "mGameloftNeedCompat=" + mGameloftNeedCompat);
}

void SurfaceView::AdjustWindowLayout()
{
    if(mGameloftNeedCompat)
    {
        mLayout->SetX((mScreenWidth - mGameSurfaceWidth) >> 1);
        mLayout->SetY((mScreenHeight-mGameSurfaceHeight) >> 1);
        mLayout->SetWidth(mGameSurfaceWidth);
        mLayout->SetHeight(mGameSurfaceHeight);
    }
}

ECode SurfaceView::AdjustSurfaceViewMotion(
    /* [in] */ IMotionEvent* evt)
{
    if(mMotionEventMayNeedAdjust == FALSE)
    {
        return NOERROR;
    }
    else
    {
        Int32 offx = (mGameSurfaceWidth - mScreenWidth) >> 1;
        Int32 offy = (mGameSurfaceHeight - mScreenHeight) >> 1;
        evt->OffsetLocation(offx, offy);
    }
    return NOERROR;
}

void SurfaceView::InitMem()
{
    mSurfaceHolder = new _SurfaceHolder(this);
    mLocation = ArrayOf<Int32>::Alloc(2);// = new Int32[2];

    //ReentrantLock mSurfaceLock = new ReentrantLock();
    CSurface::New((ISurface**)&mSurface);
    CSurface::New((ISurface**)&mNewSurface);
    mDrawingStopped = TRUE;

    CWindowManagerLayoutParams::New((IWindowManagerLayoutParams**)&mLayout);

    CRect::New((IRect**)&mVisibleInsets);
    CRect::New((IRect**)&mWinFrame);
    CRect::New((IRect**)&mContentInsets);

    CConfiguration::New((IConfiguration**)&mConfiguration);

    mWindowType = IWindowManagerLayoutParams::TYPE_APPLICATION_MEDIA;

    mIsCreating = FALSE;

    mHandler = new MyHandler(this);
    mScrollChangedListener = new _OnScrollChangedListener(this);

    mRequestedVisible = FALSE;
    mWindowVisibility = FALSE;
    mViewVisibility = FALSE;
    mRequestedWidth = -1;
    mRequestedHeight = -1;

    /* Set SurfaceView's format to 565 by default to maintain backward
     * compatibility with applications assuming this format.
     */
    mRequestedFormat = IPixelFormat::RGB_565;

    mHaveFrame = FALSE;
    mLastLockTime = 0;

    mVisible = FALSE;
    mLeft = -1;
    mTop = -1;
    mWidth = -1;
    mHeight = -1;
    mFormat = -1;
    CRect::New((IRect**)&mSurfaceFrame);
    mLastSurfaceWidth = -1;
    mLastSurfaceHeight = -1;
    mUpdateWindowNeeded = FALSE;
    mReportDrawNeeded = FALSE;

    mDrawListener = new _OnPreDrawListener(this);

    mGlobalListenersAdded = FALSE;
}

SurfaceView::SurfaceView()
{
}

SurfaceView::SurfaceView(
    /* [in] */ IContext* context)
    : View(context)
{
}

SurfaceView::SurfaceView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : View(context, attrs)
{
}

SurfaceView::SurfaceView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : View(context, attrs, defStyle)
{
}

ECode SurfaceView::Init(
    /* [in] */ IContext* context)
{
    FAIL_RETURN(View::Init(context));
    Init();
    return NOERROR;
}

ECode SurfaceView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(View::Init(context, attrs));
    Init();
    return NOERROR;
}

ECode SurfaceView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(View::Init(context, attrs, defStyle));
    Init();
    return NOERROR;
}

SurfaceView::~SurfaceView()
{
}

void SurfaceView::Init()
{
    InitMem();

    SetWillNotDraw(TRUE);
    mMotionEventMayNeedAdjust =FALSE;
    // "TODO"
    // if(this instanceof GLSurfaceView)
    // {
    //     testGameloftNeedAdjust();
    // }
}

AutoPtr<ISurfaceHolder> SurfaceView::GetHolder()
{
    return mSurfaceHolder;
}

ECode SurfaceView::OnAttachedToWindow()
{
    View::OnAttachedToWindow();
    mParent->RequestTransparentRegion((IView*)this->Probe(EIID_IView));
    mSession = GetWindowSession();
    assert(mSession != NULL);
    mLayout->SetToken(GetWindowToken());

    AutoPtr<ICharSequence> text;
    CString::New(String("SurfaceView"), (ICharSequence**)&text);
    mLayout->SetTitle(text);
    mViewVisibility = GetVisibility() == IView::VISIBLE;

    if (!mGlobalListenersAdded) {
        AutoPtr<IViewTreeObserver> observer = GetViewTreeObserver();
        observer->AddOnScrollChangedListener(mScrollChangedListener);
        observer->AddOnPreDrawListener(mDrawListener);
        mGlobalListenersAdded = TRUE;
    }
    return NOERROR;
}

void SurfaceView::OnWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    View::OnWindowVisibilityChanged(visibility);
    mWindowVisibility = visibility == IView::VISIBLE;
    mRequestedVisible = mWindowVisibility && mViewVisibility;
    UpdateWindow(FALSE, FALSE);
}

ECode SurfaceView::SetVisibility(
    /* [in] */ Int32 visibility)
{
    View::SetVisibility(visibility);
    mViewVisibility = visibility == IView::VISIBLE;

    Boolean newRequestedVisible = mWindowVisibility && mViewVisibility;
    if (newRequestedVisible != mRequestedVisible) {
        // our base class (View) invalidates the layout only when
        // we go from/to the GONE state. However, SurfaceView needs
        // to request a re-layout when the visibility changes at all.
        // This is needed because the transparent region is computed
        // as part of the layout phase, and it changes (obviously) when
        // the visibility changes.
        RequestLayout();
    }
    mRequestedVisible = newRequestedVisible;

    UpdateWindow(FALSE, FALSE);

    return NOERROR;
}

ECode SurfaceView::OnDetachedFromWindow()
{
    if (mGlobalListenersAdded) {
        AutoPtr<IViewTreeObserver> observer = GetViewTreeObserver();
        observer->RemoveOnScrollChangedListener(mScrollChangedListener);
        observer->RemoveOnPreDrawListener(mDrawListener);
        mGlobalListenersAdded = FALSE;
    }

    mRequestedVisible = FALSE;
    UpdateWindow(FALSE, FALSE);
    mHaveFrame = FALSE;
    if (mWindow != NULL) {
        mSession->Remove(mWindow);
        mWindow = NULL;
    }

    mSession = NULL;
    mLayout->SetToken(NULL);

    return View::OnDetachedFromWindow();
}

void SurfaceView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 width = mRequestedWidth >= 0
            ? ResolveSizeAndState(mRequestedWidth, widthMeasureSpec, 0)
            : GetDefaultSize(0, widthMeasureSpec);
    Int32 height = mRequestedHeight >= 0
            ? ResolveSizeAndState(mRequestedHeight, heightMeasureSpec, 0)
            : GetDefaultSize(0, heightMeasureSpec);

    SetMeasuredDimension(width, height);
}

/** @hide */
Boolean SurfaceView::SetFrame(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    Boolean result = View::SetFrame(left, top, right, bottom);
    UpdateWindow(FALSE, FALSE);
    return result;
}

Boolean SurfaceView::GatherTransparentRegion(
    /* [in] */ IRegion* region)
{
    if (mWindowType == IWindowManagerLayoutParams::TYPE_APPLICATION_PANEL) {
        return View::GatherTransparentRegion(region);
    }

    Boolean opaque = TRUE;
    if ((mPrivateFlags & PFLAG_SKIP_DRAW) == 0) {
        // this view draws, remove it from the transparent region
        opaque = View::GatherTransparentRegion(region);
    } else if (region != NULL) {
        Int32 w = GetWidth();
        Int32 h = GetHeight();
        if (w>0 && h>0) {
            GetLocationInWindow(&(*mLocation)[0], &(*mLocation)[1]);
            // otherwise, punch a hole in the whole hierarchy
            Int32 l = (*mLocation)[0];
            Int32 t = (*mLocation)[1];

            Boolean res;
            region->Op(l, t, l+w, t+h, RegionOp_UNION, &res);
        }
    }
    if (PixelFormat::FormatHasAlpha(mRequestedFormat)) {
        opaque = FALSE;
    }
    return opaque;
}

ECode SurfaceView::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (mWindowType != IWindowManagerLayoutParams::TYPE_APPLICATION_PANEL) {
        // draw() is not called when PFLAG_SKIP_DRAW is set
        if ((mPrivateFlags & PFLAG_SKIP_DRAW) == 0) {
            // punch a whole in the view-hierarchy below us
            canvas->DrawColor(0, PorterDuffMode_CLEAR);
        }
    }
    return View::Draw(canvas);
}

void SurfaceView::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    if (mWindowType != IWindowManagerLayoutParams::TYPE_APPLICATION_PANEL) {
        // if PFLAG_SKIP_DRAW is cleared, draw() has already punched a hole
        if ((mPrivateFlags & PFLAG_SKIP_DRAW) == PFLAG_SKIP_DRAW) {
            // punch a whole in the view-hierarchy below us
            canvas->DrawColor(0, PorterDuffMode_CLEAR);
        }
    }

    View::DispatchDraw(canvas);
}

ECode SurfaceView::SetZOrderMediaOverlay(
    /* [in] */ Boolean isMediaOverlay)
{
    mWindowType = isMediaOverlay
            ? IWindowManagerLayoutParams::TYPE_APPLICATION_MEDIA_OVERLAY
            : IWindowManagerLayoutParams::TYPE_APPLICATION_MEDIA;

    return NOERROR;
}

ECode SurfaceView::SetZOrderOnTop(
    /* [in] */ Boolean onTop)
{
    Int32 flags;
    mLayout->GetFlags(&flags);
    if (onTop) {
        mWindowType = IWindowManagerLayoutParams::TYPE_APPLICATION_PANEL;
        // ensures the surface is placed below the IME
        flags |= IWindowManagerLayoutParams::FLAG_ALT_FOCUSABLE_IM;
    } else {
        mWindowType = IWindowManagerLayoutParams::TYPE_APPLICATION_MEDIA;
        flags &= ~IWindowManagerLayoutParams::FLAG_ALT_FOCUSABLE_IM;
    }
    mLayout->SetFlags(flags);
    return NOERROR;
}

ECode SurfaceView::SetSecure(
    /* [in] */ Boolean isSecure)
{
    Int32 flags;
    mLayout->GetFlags(&flags);
    if (isSecure) {
        flags |= IWindowManagerLayoutParams::FLAG_SECURE;
    } else {
        flags &= ~IWindowManagerLayoutParams::FLAG_SECURE;
    }
    mLayout->SetFlags(flags);
    return NOERROR;
}

ECode SurfaceView::SetWindowType(
    /* [in] */ Int32 type)
{
    mWindowType = type;
    return NOERROR;
}

ECode SurfaceView::UpdateWindow(
    /* [in] */ Boolean force,
    /* [in] */ Boolean redrawNeeded)
{
    if (!mHaveFrame) {
        return NOERROR;
    }

    ViewRootImpl* viewRoot = GetViewRootImpl();
    if (viewRoot != NULL) {
        mTranslator = viewRoot->mTranslator;
    }

    if (mTranslator != NULL) {
        mSurface->SetCompatibilityTranslator(mTranslator);
    }

    TestGameloftNeedAdjust();
    Int32 myWidth = mRequestedWidth;
    if (myWidth <= 0) myWidth = GetWidth();
    Int32 myHeight = mRequestedHeight;
    if (myHeight <= 0) myHeight = GetHeight();

    GetLocationInWindow(&(*mLocation)[0], &(*mLocation)[1]);
    Boolean creating = mWindow == NULL;
    Boolean formatChanged = mFormat != mRequestedFormat;
    Boolean sizeChanged = mWidth != myWidth || mHeight != myHeight;
    Boolean visibleChanged = mVisible != mRequestedVisible;

    if (force || creating || formatChanged || sizeChanged || visibleChanged
        || mLeft != (*mLocation)[0] || mTop != (*mLocation)[1]
        || mUpdateWindowNeeded || mReportDrawNeeded || redrawNeeded) {

        if (DEBUG)
            Logger::I(TAG, "Changes: creating=%d format=%d size=%d visible=%d left=%d top=%d w=%d h=%d",
                creating, formatChanged, sizeChanged, visibleChanged,
                (mLeft != (*mLocation)[0]), (mTop != (*mLocation)[1]),
                myWidth, myHeight);

        //try {
            Boolean visible = mVisible = mRequestedVisible;
            mLeft = (*mLocation)[0];
            mTop = (*mLocation)[1];
            mWidth = myWidth;
            mHeight = myHeight;
            mFormat = mRequestedFormat;

            // Scaling/Translate window's layout here because mLayout is not used elsewhere.
            // Places the window relative
            mLayout->SetX(mLeft);
            mLayout->SetY(mTop);
            mLayout->SetWidth(GetWidth());
            mLayout->SetHeight(GetHeight());
            AdjustWindowLayout();

            if (mTranslator != NULL) {
                mTranslator->TranslateLayoutParamsInAppWindowToScreen(mLayout);
            }

            mLayout->SetFormat(mRequestedFormat);
            Int32 flags;
            mLayout->GetFlags(&flags);
            flags |= IWindowManagerLayoutParams::FLAG_NOT_TOUCHABLE
                    | IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE
                    | IWindowManagerLayoutParams::FLAG_LAYOUT_NO_LIMITS
                    | IWindowManagerLayoutParams::FLAG_SCALED
                    | IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE
                    | IWindowManagerLayoutParams::FLAG_NOT_TOUCHABLE;
            mLayout->SetFlags(flags);

            AutoPtr<IContext> context = GetContext();
            AutoPtr<IResources> resources;
            context->GetResources((IResources**)&resources);
            AutoPtr<ICompatibilityInfo> compat;
            resources->GetCompatibilityInfo((ICompatibilityInfo**)&compat);
            Boolean supportsScreen;
            compat->SupportsScreen(&supportsScreen);

            if (!supportsScreen) {
                mLayout->GetFlags(&flags);
                mLayout->SetFlags(flags | IWindowManagerLayoutParams::FLAG_COMPATIBLE_WINDOW);
            }

            if (mWindow == NULL) {
                AutoPtr<IDisplay> display = GetDisplay();
                CSurfaceViewWindow::New((Handle32)this, (IIWindow**)&mWindow);
                mLayout->SetType(mWindowType);
                mLayout->SetGravity(IGravity::START | IGravity::TOP);

                assert(mSession != NULL && display != NULL && mWindow != NULL);

                Int32 displayId = 0;
                display->GetDisplayId(&displayId);
                AutoPtr<IRect> outContentInsets;
                Int32 result = 0;
                mSession->AddToDisplayWithoutInputChannel(
                        mWindow, ((CSurfaceViewWindow*)mWindow.Get())->mSeq, mLayout,
                        mVisible ? IView::VISIBLE : IView::GONE,
                        displayId, mContentInsets, (IRect**)&outContentInsets, &result);
                mContentInsets->Set(outContentInsets);
            }

            Boolean realSizeChanged;
            Boolean reportDrawNeeded;
            Int32 relayoutResult = 0;

            {
                AutoLock lock(mSurfaceLock);
                mUpdateWindowNeeded = FALSE;
                reportDrawNeeded = mReportDrawNeeded;
                mReportDrawNeeded = FALSE;
                mDrawingStopped = !visible;

                //if (DEBUG) Log.i(TAG, "Cur surface: " + mSurface);
                Int32 wParams = mWidth, hParams = mHeight;

                if(mGameloftNeedCompat)
                {
                    wParams = 800;
                    hParams = 480;
                }
                mSurfaceLock.Unlock();
                AutoPtr<IRect> outFrame, outContentInsets, outVisibleInsets;
                AutoPtr<IConfiguration> outConfig;
                AutoPtr<ISurface> outSurface;
                mSession->Relayout(
                        mWindow, ((CSurfaceViewWindow*)mWindow.Get())->mSeq, mLayout,
                        wParams, hParams,
                        visible ? IView::VISIBLE : IView::GONE,
                        IWindowManagerGlobal::RELAYOUT_DEFER_SURFACE_DESTROY,
                        mWinFrame, mContentInsets, mVisibleInsets,
                        mConfiguration, mNewSurface,
                        (IRect**)&outFrame, (IRect**)&outContentInsets,
                        (IRect**)&outVisibleInsets,
                        (IConfiguration**)&outConfig, &relayoutResult,
                        (ISurface**)&outSurface);
                mSurfaceLock.Lock();
                mWinFrame->Set(outFrame);
                mContentInsets->Set(outContentInsets);
                mVisibleInsets->Set(outVisibleInsets);
                mConfiguration->SetTo(outConfig);
                Handle32 nativeSurface;
                outSurface->GetSurface(&nativeSurface);
                mNewSurface->SetSurface(nativeSurface);

                if ((relayoutResult & IWindowManagerGlobal::RELAYOUT_RES_FIRST_TIME) != 0) {
                    mReportDrawNeeded = TRUE;
                }

                /*if (localLOGV) Log.i(TAG, "New surface: " + mSurface
                        + ", vis=" + visible + ", frame=" + mWinFrame);*/

                Int32 surfaceWidth, surfaceHeight;
                mWinFrame->GetWidth(&surfaceWidth);
                mWinFrame->GetHeight(&surfaceHeight);
                if (mTranslator != NULL) {
                    Float appInvertedScale = 0.0;
                    mTranslator->GetApplicationInvertedScale(&appInvertedScale);
                    surfaceWidth = (Int32)(surfaceWidth * appInvertedScale + 0.5f);
                    surfaceHeight = (Int32)(surfaceHeight * appInvertedScale + 0.5f);
                }

                mSurfaceFrame->Set(0, 0, surfaceWidth, surfaceHeight);

                realSizeChanged = mLastSurfaceWidth != surfaceWidth
                        || mLastSurfaceHeight != surfaceHeight;
                mLastSurfaceWidth = surfaceWidth;
                mLastSurfaceHeight = surfaceHeight;
            }

            //try {
            redrawNeeded |= creating | reportDrawNeeded;

            AutoPtr<ArrayOf<ISurfaceHolderCallback*> > callbacks;
            Int32 size = 0;
            Boolean surfaceChanged = (relayoutResult
                    & IWindowManagerGlobal::RELAYOUT_RES_SURFACE_CHANGED) != 0;
            if (mSurfaceCreated && (surfaceChanged || (!visible && visibleChanged))) {
                mSurfaceCreated = FALSE;
                Boolean valid = FALSE;
                mSurface->IsValid(&valid);
                if (valid) {
                    // if (DEBUG) Log.i(TAG, "visibleChanged -- surfaceDestroyed");
                    callbacks = GetSurfaceCallbacks();
                    size = callbacks->GetLength();
                    for (Int32 i = 0; i < size; ++i) {
                        AutoPtr<ISurfaceHolderCallback> c = (*callbacks)[i];
                        assert(c != NULL);
                        c->SurfaceDestroyed(mSurfaceHolder.Get());
                    }
                }
            }

            mSurface->TransferFrom(mNewSurface);

            Boolean valid = FALSE;
            mSurface->IsValid(&valid);
            if (visible && valid) {
                if (!mSurfaceCreated && (surfaceChanged || visibleChanged)) {
                    mSurfaceCreated = TRUE;
                    mIsCreating = TRUE;
                    // if (DEBUG) Log.i(TAG, "visibleChanged -- surfaceCreated");
                    if (callbacks == NULL) {
                        callbacks = GetSurfaceCallbacks();
                    }
                    size = callbacks->GetLength();
                    for (Int32 i = 0; i < size; ++i) {
                        AutoPtr<ISurfaceHolderCallback> c = (*callbacks)[i];
                        assert(c != NULL);
                        c->SurfaceCreated(mSurfaceHolder.Get());
                    }
                }
                if (creating || formatChanged || sizeChanged
                        || visibleChanged || realSizeChanged) {
                    // if (DEBUG) Log.i(TAG, "surfaceChanged -- format=" + mFormat
                    //         + " w=" + myWidth + " h=" + myHeight);
                    if (callbacks == NULL) {
                        callbacks = GetSurfaceCallbacks();
                    }
                    size = callbacks->GetLength();
                    for (Int32 i = 0; i < size; ++i) {
                        AutoPtr<ISurfaceHolderCallback> c = (*callbacks)[i];
                        assert(c != NULL);
                        c->SurfaceChanged(mSurfaceHolder.Get(), mFormat, myWidth, myHeight);
                    }
                }
                if (redrawNeeded) {
                    // if (DEBUG) Log.i(TAG, "surfaceRedrawNeeded");
                    if (callbacks == NULL) {
                        callbacks = GetSurfaceCallbacks();
                    }
                    size = callbacks->GetLength();
                    for (Int32 i = 0; i < size; ++i) {
                        AutoPtr<ISurfaceHolderCallback> c = (*callbacks)[i];
                        assert(c != NULL);
                        if (ISurfaceHolderCallback2::Probe(c) != NULL) {
                            ISurfaceHolderCallback2::Probe(c)->SurfaceRedrawNeeded(
                                    mSurfaceHolder);
                        }
                    }
                }
            }

            mIsCreating = FALSE;
            if (redrawNeeded) {
                // if (DEBUG) Log.i(TAG, "finishedDrawing");
                mSession->FinishDrawing(mWindow);
            }

            mSession->PerformDeferredDestroy(mWindow);
        //} catch (RemoteException ex) {
        //}
        /*if (localLOGV) Log.v(
            TAG, "Layout: x=" + mLayout.x + " y=" + mLayout.y +
            " w=" + mLayout.width + " h=" + mLayout.height +
            ", frame=" + mSurfaceFrame);*/
    }
    return NOERROR;
}

AutoPtr<ArrayOf<ISurfaceHolderCallback*> > SurfaceView::GetSurfaceCallbacks()
{
    AutoLock lock(mCallbacksLock);

    AutoPtr<ArrayOf<ISurfaceHolderCallback*> > callbacks;
    Int32 size = mCallbacks.GetSize();
    if (size > 0) {
        callbacks = ArrayOf<ISurfaceHolderCallback*>::Alloc(size);
        List<AutoPtr<ISurfaceHolderCallback> >::Iterator iter = mCallbacks.Begin();
        for (Int32 i = 0; iter != mCallbacks.End(); ++iter, ++i) {
            callbacks->Set(i, *iter);
        }
    }
    return callbacks;
}

ECode SurfaceView::HandleGetNewSurface()
{
    return UpdateWindow(FALSE, FALSE);
}

Boolean SurfaceView::IsFixedSize()
{
    return (mRequestedWidth != -1 || mRequestedHeight != -1);
}

AutoPtr<IInputConnection> SurfaceView::OnCreateInputConnection(
    /* [in] */ IEditorInfo* outAttrs)
{
    AutoPtr<IInputConnection> ic;
    if (mDelegate != NULL) {
        mDelegate->OnCreateInputConnection(outAttrs, (IInputConnection**)&ic);
    }
    return ic;
}

Boolean SurfaceView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (mKeyEventCallbackDelegate != NULL) {
        Boolean result;
        mKeyEventCallbackDelegate->OnKeyDown(keyCode, event, &result);
        return result;
    }
    return View::OnKeyDown(keyCode, event);
}

Boolean SurfaceView::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (mKeyEventCallbackDelegate != NULL) {
        Boolean result;
        mKeyEventCallbackDelegate->OnKeyLongPress(keyCode, event, &result);
        return result;
    }
    return View::OnKeyLongPress(keyCode, event);
}

Boolean SurfaceView::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (mKeyEventCallbackDelegate != NULL) {
        Boolean result;
        mKeyEventCallbackDelegate->OnKeyUp(keyCode, event, &result);
        return result;
    }
    return View::OnKeyUp(keyCode, event);
}

Boolean SurfaceView::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 repeatCount,
    /* [in] */ IKeyEvent* event)
{
    if (mKeyEventCallbackDelegate != NULL) {
        Boolean result;
        mKeyEventCallbackDelegate->OnKeyMultiple(keyCode, repeatCount, event, &result);
        return NOERROR;
    }
    return View::OnKeyMultiple(keyCode, repeatCount, event);
}

ECode SurfaceView::SetCreateInputConnectionDelegate(
    /* [in] */ IView* view)
{
    mDelegate = view;
    return NOERROR;
}

ECode SurfaceView::SetKeyEventCallbackDelegate(
    /* [in] */IKeyEventCallback* cb)
{
    mKeyEventCallbackDelegate = cb;
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
