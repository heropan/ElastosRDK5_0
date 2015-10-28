
#include "elastos/droid/service/wallpaper/WallpaperService.h"
#ifdef DROID_CORE
#include "elastos/droid/content/CIntentFilter.h"
#include "elastos/droid/content/res/CConfiguration.h"
#include "elastos/droid/view/CMotionEventHelper.h"
#include "elastos/droid/view/CWindowManagerLayoutParams.h"
#include "elastos/droid/view/CInputChannel.h"
#include "elastos/droid/view/CWindowManagerGlobal.h"
#include "elastos/droid/graphics/CRect.h"
#include "elastos/droid/service/wallpaper/CWallpaperServiceEngineWindow.h"
#include "elastos/droid/service/wallpaper/CWallpaperServiceWrapper.h"
#include "elastos/droid/os/CLooperHelper.h"
#endif
#include "elastos/droid/R.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::Res::CConfiguration;
using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::ILooperHelper;
using Elastos::Droid::Os::CLooperHelper;
using Elastos::Utility::Iterator;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Etl::Vector;

namespace Elastos {
namespace Droid {
namespace Service {
namespace Wallpaper {

const String WallpaperService::SERVICE_INTERFACE("android.service.wallpaper.WallpaperService");
const String WallpaperService::SERVICE_META_DATA("android.service.wallpaper");
const String WallpaperService::TAG("WallpaperService");
const Boolean WallpaperService::DEBUG = FALSE;

const Int32 WallpaperService::MSG_UPDATE_SURFACE;
const Int32 WallpaperService::MSG_VISIBILITY_CHANGED;
const Int32 WallpaperService::MSG_WALLPAPER_OFFSETS;
const Int32 WallpaperService::MSG_WALLPAPER_COMMAND;
const Int32 WallpaperService::MSG_WINDOW_RESIZED;
const Int32 WallpaperService::MSG_WINDOW_MOVED;
const Int32 WallpaperService::MSG_TOUCH_EVENT;

CAR_INTERFACE_IMPL(WallpaperService::WallpaperCommand, IInterface)

WallpaperService::Engine::MyBroadcastReceiver::MyBroadcastReceiver(
    /* [in] */ Engine* owner)
    : mOwner(owner)
{
    //empty
}

ECode WallpaperService::Engine::MyBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (action.Equals(IIntent::ACTION_SCREEN_ON)) {
        mOwner->mScreenOn = TRUE;
        mOwner->ReportVisibility();
    } else if (action.Equals(IIntent::ACTION_SCREEN_OFF)) {
        mOwner->mScreenOn = FALSE;
        mOwner->ReportVisibility();
    }

    return NOERROR;
}

WallpaperService::Engine::MySurfaceHolder::MySurfaceHolder(
    /* [in] */ Engine* owner)
    : mOwner(owner)
{
    mRequestedFormat = IPixelFormat::RGB_565;
}

Boolean WallpaperService::Engine::MySurfaceHolder::OnAllowLockCanvas()
{
    return mOwner->mDrawingAllowed;
}

void WallpaperService::Engine::MySurfaceHolder::OnRelayoutContainer()
{
    AutoPtr<IMessage> msg;
    mOwner->mCaller->ObtainMessage(MSG_UPDATE_SURFACE, (IMessage**)&msg);
    mOwner->mCaller->SendMessage(msg);
}

ECode WallpaperService::Engine::MySurfaceHolder::OnUpdateSurface()
{
    AutoPtr<IMessage> msg;
    mOwner->mCaller->ObtainMessage(MSG_UPDATE_SURFACE, (IMessage**)&msg);
    return mOwner->mCaller->SendMessage(msg);
}

ECode WallpaperService::Engine::MySurfaceHolder::IsCreating(
    /* [out] */ Boolean* isCreating)
{
    VALIDATE_NOT_NULL(isCreating);
    *isCreating = mOwner->mIsCreating;
    return NOERROR;
}

ECode WallpaperService::Engine::MySurfaceHolder::SetFixedSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (!mOwner->mFixedSizeAllowed) {
        // Regular apps can't do this.  It can only work for
        // certain designs of window animations, so you can't
        // rely on it.
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
//        throw new UnsupportedOperationException(
//                "Wallpapers currently only support sizing from layout");
    }
    return BaseSurfaceHolder::SetFixedSize(width, height);
}

ECode WallpaperService::Engine::MySurfaceHolder::SetKeepScreenOn(
    /* [in] */ Boolean screenOn)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException(
//                        "Wallpapers do not support keep screen on");
}

WallpaperService::Engine::WallpaperInputEventReceiver::WallpaperInputEventReceiver(
    /* [in] */ IInputChannel* inputChannel,
    /* [in] */ ILooper* looper,
    /* [in] */ Engine* owner)
    : InputEventReceiver(inputChannel, looper)
    , mOwner(owner)
{
}

ECode WallpaperService::Engine::WallpaperInputEventReceiver::OnInputEvent(
    /* [in] */ IInputEvent* event)
{
    Boolean handled = FALSE;
    //try {
    Int32 source;
    if (IMotionEvent::Probe(event) != NULL &&
        ((event->GetSource(&source), source) & IInputDevice::SOURCE_CLASS_POINTER) != 0)
    {
        AutoPtr<IMotionEventHelper> mh;
        CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&mh);
        AutoPtr<IMotionEvent> dup;
        mh->ObtainNoHistory(IMotionEvent::Probe(event), (IMotionEvent**)&dup);
        mOwner->DispatchPointer(dup);
        handled = TRUE;
    }
    //} finally {
        FinishInputEvent(event, handled);
    //}
    return NOERROR;
}

WallpaperService::Engine::Engine(
    /* [in] */ WallpaperService* owner)
    : mInitializing(TRUE)
    , mVisible(FALSE)
    , mScreenOn(TRUE)
    , mReportedVisible(FALSE)
    , mDestroyed(FALSE)
    , mCreated(FALSE)
    , mSurfaceCreated(FALSE)
    , mIsCreating(FALSE)
    , mDrawingAllowed(FALSE)
    , mOffsetsChanged(FALSE)
    , mFixedSizeAllowed(FALSE)
    , mWidth(0)
    , mHeight(0)
    , mFormat(0)
    , mType(0)
    , mCurWidth(0)
    , mCurHeight(0)
    , mWindowFlags(IWindowManagerLayoutParams::FLAG_NOT_TOUCHABLE)
    , mWindowPrivateFlags(IWindowManagerLayoutParams::PRIVATE_FLAG_WANTS_OFFSET_NOTIFICATIONS)
    , mCurWindowFlags(mWindowFlags)
    , mCurWindowPrivateFlags(mWindowPrivateFlags)
    , mOffsetMessageEnqueued(FALSE)
    , mPendingXOffset(0)
    , mPendingYOffset(0)
    , mPendingXOffsetStep(0)
    , mPendingYOffsetStep(0)
    , mPendingSync(FALSE)
    , mOwner(owner)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mVisibleInsets));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mWinFrame));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mContentInsets));
    ASSERT_SUCCEEDED(CConfiguration::New((IConfiguration**)&mConfiguration));
    ASSERT_SUCCEEDED(CWindowManagerLayoutParams::New((IWindowManagerLayoutParams**)&mLayout));
    mReceiver = new MyBroadcastReceiver(this);
    mSurfaceHolder = new MySurfaceHolder(this);
    ASSERT_SUCCEEDED(CWallpaperServiceEngineWindow::New((Handle32)this, (IBaseIWindow**)&mWindow));
}

WallpaperService::Engine::~Engine()
{
}

AutoPtr<ISurfaceHolder> WallpaperService::Engine::GetSurfaceHolder()
{
    return mSurfaceHolder;
}

Int32 WallpaperService::Engine::GetDesiredMinimumWidth()
{
    Int32 w;
    mIWallpaperEngine->GetReqWidth(&w);
    return w;
}

Int32 WallpaperService::Engine::GetDesiredMinimumHeight()
{
    Int32 h;
    mIWallpaperEngine->GetReqHeight(&h);
    return h;
}

Boolean WallpaperService::Engine::IsVisible()
{
    return mReportedVisible;
}

Boolean WallpaperService::Engine::IsPreview()
{
    Boolean isPreview;
    mIWallpaperEngine->GetIsPreview(&isPreview);
    return isPreview;
}

void WallpaperService::Engine::SetTouchEventsEnabled(
    /* [in] */ Boolean enabled)
{
    mWindowFlags = enabled
            ? (mWindowFlags&~IWindowManagerLayoutParams::FLAG_NOT_TOUCHABLE)
            : (mWindowFlags|IWindowManagerLayoutParams::FLAG_NOT_TOUCHABLE);
    if (mCreated) {
        UpdateSurface(FALSE, FALSE, FALSE);
    }
}

void WallpaperService::Engine::SetOffsetNotificationsEnabled(
    /* [in] */ Boolean enabled)
{
    mWindowPrivateFlags = enabled
        ? (mWindowPrivateFlags |
            IWindowManagerLayoutParams::PRIVATE_FLAG_WANTS_OFFSET_NOTIFICATIONS)
        : (mWindowPrivateFlags &
            ~IWindowManagerLayoutParams::PRIVATE_FLAG_WANTS_OFFSET_NOTIFICATIONS);
    if (mCreated) {
        UpdateSurface(FALSE, FALSE, FALSE);
    }
}

void WallpaperService::Engine::SetFixedSizeAllowed(
    /* [in] */ Boolean  allowed)
{
    mFixedSizeAllowed = allowed;
}


void WallpaperService::Engine::OnCreate(
    /* [in] */ ISurfaceHolder* surfaceHolder)
{
}

void WallpaperService::Engine::OnDestroy()
{
}

void WallpaperService::Engine::OnVisibilityChanged(
    /* [in] */ Boolean visible)
{
}

void WallpaperService::Engine::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
}

void WallpaperService::Engine::OnOffsetsChanged(
    /* [in] */ Float xOffset,
    /* [in] */ Float yOffset,
    /* [in] */ Float xOffsetStep,
    /* [in] */ Float yOffsetStep,
    /* [in] */ Int32 xPixelOffset,
    /* [in] */ Int32 yPixelOffset)
{
}

AutoPtr<IBundle> WallpaperService::Engine::OnCommand(
    /* [in] */ const String& action,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean resultRequested)
{
    return NULL;
}

void WallpaperService::Engine::OnDesiredSizeChanged(
    /* [in] */ Int32 desiredWidth,
    /* [in] */ Int32 desiredHeight)
{
}

void WallpaperService::Engine::OnSurfaceChanged(
    /* [in] */ ISurfaceHolder* holder,
    /* [in] */ Int32 format,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
}

void WallpaperService::Engine::OnSurfaceRedrawNeeded(
    /* [in] */ ISurfaceHolder* holder)
{
}

void WallpaperService::Engine::OnSurfaceCreated(
    /* [in] */ ISurfaceHolder* holder)
{
}

void WallpaperService::Engine::OnSurfaceDestroyed(
    /* [in] */ ISurfaceHolder* holder)
{
}

void WallpaperService::Engine::Dump(
        /* [in] */ const String& prefix,
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* out,
        /* [in] */ const ArrayOf<String>& args)
{
    assert(0);
    // out->Print(prefix); out->Print("mInitializing="); out->Print(mInitializing);
    // out->Print(" mDestroyed="); out->Println(mDestroyed);
    // out->Print(prefix); out->Print("mVisible="); out->Print(mVisible);
    // out->Print(" mScreenOn="); out->Print(mScreenOn);
    // out->Print(" mReportedVisible="); out->Println(mReportedVisible);
    // out->Print(prefix); out->Print("mCreated="); out->Print(mCreated);
    // out->Print(" mSurfaceCreated="); out->Print(mSurfaceCreated);
    // out->Print(" mIsCreating="); out->Print(mIsCreating);
    // out->Print(" mDrawingAllowed="); out->Println(mDrawingAllowed);
    // out->Print(prefix); out->Print("mWidth="); out->Print(mWidth);
    // out->Print(" mCurWidth="); out->Print(mCurWidth);
    // out->Print(" mHeight="); out->Print(mHeight);
    // out->Print(" mCurHeight="); out->Println(mCurHeight);
    // out->Print(prefix); out->Print("mType="); out->Print(mType);
    // out->Print(" mWindowFlags="); out->Print(mWindowFlags);
    // out->Print(" mCurWindowFlags="); out->Println(mCurWindowFlags);
    // out->Print(" mWindowPrivateFlags="); out->Print(mWindowPrivateFlags);
    // out->Print(" mCurWindowPrivateFlags="); out->Println(mCurWindowPrivateFlags);
    // out->Print(prefix); out->Print("mVisibleInsets=");
    // out->Print(mVisibleInsets.toShortString());
    // out->Print(" mWinFrame="); out->Print(mWinFrame->ToShortString());
    // out->Print(" mContentInsets="); out->Println(mContentInsets->ToShortString());
    // out->Print(prefix); out->Print("mConfiguration="); out->Println(mConfiguration);
    // out->Print(prefix); out->Print("mLayout="); out->Println(mLayout);
    // //synchronized (mLock)
    // {
    //     AutoLock lock(mLock);
    //     out->Print(prefix); out->Print("mPendingXOffset="); out->Print(mPendingXOffset);
    //             out->Print(" mPendingXOffset="); out->Println(mPendingXOffset);
    //     out->Print(prefix); out->Print("mPendingXOffsetStep=");
    //             out->Print(mPendingXOffsetStep);
    //             out->Print(" mPendingXOffsetStep="); out->Println(mPendingXOffsetStep);
    //     out->Print(prefix); out->Print("mOffsetMessageEnqueued=");
    //             out->Print(mOffsetMessageEnqueued);
    //             out->Print(" mPendingSync="); out->Println(mPendingSync);
    //     if (mPendingMove != NULL)
    //     {
    //         out->Print(prefix); out->Print("mPendingMove="); out->Println(mPendingMove);
    //     }
    // }
}

void WallpaperService::Engine::DispatchPointer(
    /* [in] */ IMotionEvent* event)
{
    Boolean isTouchEvent;
    event->IsTouchEvent(&isTouchEvent);
    if (isTouchEvent) {
        //synchronized (mLock)
        {
            AutoLock lock(mLock);
            Int32 action;
            event->GetAction(&action);
            if (action == IMotionEvent::ACTION_MOVE) {
                mPendingMove = event;
            } else {
                mPendingMove = NULL;
            }
        }

        AutoPtr<IMessage> msg;
        mCaller->ObtainMessage(WallpaperService::MSG_TOUCH_EVENT, event, (IMessage**)&msg);
        mCaller->SendMessage(msg);
    }
    else {
        event->Recycle();
    }
}

void WallpaperService::Engine::UpdateSurface(
    /* [in] */ Boolean forceRelayout,
    /* [in] */ Boolean forceReport,
    /* [in] */ Boolean redrawNeeded)
{
    if (mDestroyed) {
        Logger::W(TAG, "Ignoring updateSurface: destroyed");
        return;
    }

    Int32 myWidth = mSurfaceHolder->GetRequestedWidth();
    if (myWidth <= 0)
        myWidth = IViewGroupLayoutParams::MATCH_PARENT;

    Int32 myHeight = mSurfaceHolder->GetRequestedHeight();
    if (myHeight <= 0)
        myHeight = IViewGroupLayoutParams::MATCH_PARENT;

    Boolean creating = !mCreated;
    Boolean surfaceCreating = !mSurfaceCreated;
    Boolean formatChanged = mFormat != mSurfaceHolder->GetRequestedFormat();
    Boolean sizeChanged = mWidth != myWidth || mHeight != myHeight;
    Boolean typeChanged = mType != mSurfaceHolder->GetRequestedType();
    Boolean flagsChanged = mCurWindowFlags != mWindowFlags ||
            mCurWindowPrivateFlags != mWindowPrivateFlags;

    Boolean showReported;
    if (forceRelayout || creating || surfaceCreating || formatChanged || sizeChanged
        || typeChanged || flagsChanged || redrawNeeded
        || !(mIWallpaperEngine->GetShownReported(&showReported), showReported)) {

        //if (DEBUG) Log.v(TAG, "Changes: creating=" + creating
        //        + " format=" + formatChanged + " size=" + sizeChanged);

        //try
        {
            mWidth = myWidth;
            mHeight = myHeight;
            mFormat = mSurfaceHolder->GetRequestedFormat();
            mType = mSurfaceHolder->GetRequestedType();

            mLayout->SetX(0);
            mLayout->SetY(0);
            mLayout->SetWidth(myWidth);
            mLayout->SetHeight(myHeight);

            mLayout->SetFormat(mFormat);

            mCurWindowFlags = mWindowFlags;
            mLayout->SetFlags(
                mWindowFlags| IWindowManagerLayoutParams::FLAG_LAYOUT_NO_LIMITS
                    | IWindowManagerLayoutParams::FLAG_LAYOUT_IN_SCREEN
                    | IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE);

            mCurWindowPrivateFlags = mWindowPrivateFlags;
            mLayout->SetPrivateFlags(mWindowPrivateFlags);

            mLayout->SetMemoryType(mType);
            mLayout->SetToken(mWindowToken);

            Int32 seq;
            mWindow->GetSeq(&seq);
            if (!mCreated) {
                Int32 type;
                mIWallpaperEngine->GetWindowType(&type);
                mLayout->SetType(type);
                mLayout->SetGravity(IGravity::START|IGravity::TOP);
                AutoPtr<ICharSequence> cs;
                CString::New(String("WallpaperService"), (ICharSequence**)&cs);
                mLayout->SetTitle(cs);
                mLayout->SetWindowAnimations(R::style::Animation_Wallpaper);
                CInputChannel::New((IInputChannel**)&mInputChannel);
                AutoPtr<IRect> outContentInsets;
                AutoPtr<IInputChannel> outInputChannel;
                Int32 res;
                mSession->AddToDisplay(mWindow.Get(), seq, mLayout, IView::VISIBLE,
                    IDisplay::DEFAULT_DISPLAY, mContentInsets, mInputChannel,
                    (IRect**)&outContentInsets, (IInputChannel**)&outInputChannel, &res);
                if (res < 0) {
                    Logger::W(TAG, "Failed to add window while updating wallpaper surface.");
                    return;
                }
                mContentInsets->Set(outContentInsets);
                outInputChannel->TransferTo(mInputChannel);
                mCreated = TRUE;

                AutoPtr<ILooperHelper> looperHelper;
                CLooperHelper::AcquireSingleton((ILooperHelper**)&looperHelper);
                AutoPtr<ILooper> looper;
                looperHelper->MyLooper((ILooper**)&looper);
                mInputEventReceiver = new WallpaperInputEventReceiver(
                        mInputChannel, looper, this);
            }

            mSurfaceHolder->mSurfaceLock.Lock();
            mDrawingAllowed = TRUE;

            AutoPtr<IRect> winFrame, contentInsets, visibleInsets;
            AutoPtr<IConfiguration> configuration;
            AutoPtr<ISurface> surface;
            AutoPtr<ISurface> olderSurface = mSurfaceHolder->mSurface;
            Int32 relayoutResult = 0;
            mSurfaceHolder->mSurfaceLock.Unlock();
            mSession->Relayout(
                mWindow, seq, mLayout, mWidth, mHeight, IView::VISIBLE,
                0, mWinFrame, mContentInsets, mVisibleInsets, mConfiguration,
                olderSurface, (IRect**)&winFrame, (IRect**)&contentInsets,
                (IRect**)&visibleInsets, (IConfiguration**)&configuration,
                &relayoutResult, (ISurface**)&surface);
            mSurfaceHolder->mSurfaceLock.Lock();
            if (winFrame) mWinFrame->Set(winFrame);
            if (contentInsets) mContentInsets->Set(contentInsets);
            if (visibleInsets) mVisibleInsets->Set(visibleInsets);
            if (configuration) mConfiguration->SetTo(configuration);
            if (surface) {
                Handle32 nativeSurface;
                surface->GetSurface(&nativeSurface);
                mSurfaceHolder->mSurface->SetSurface(nativeSurface);
            }
            //if (DEBUG) Log.v(TAG, "New surface: " + mSurfaceHolder.mSurface
            //        + ", frame=" + mWinFrame);

            Int32 w;
            mWinFrame->GetWidth(&w);
            if (mCurWidth != w) {
                sizeChanged = TRUE;
                mCurWidth = w;
            }
            Int32 h;
            mWinFrame->GetHeight(&h);
            if (mCurHeight != h) {
                sizeChanged = TRUE;
                mCurHeight = h;
            }

            mSurfaceHolder->SetSurfaceFrameSize(w, h);
            mSurfaceHolder->mSurfaceLock.Unlock();

            Boolean isValid;
            mSurfaceHolder->mSurface->IsValid(&isValid);
            if (!isValid) {
                ReportSurfaceDestroyed();
                if (DEBUG) Logger::V(TAG, "Layout: Surface destroyed");
                return;
            }

            Boolean didSurface = FALSE;

            //try
            {
                mSurfaceHolder->UngetCallbacks();

                if (surfaceCreating) {
                    mIsCreating = TRUE;
                    didSurface = TRUE;
                    //if (DEBUG) Log.v(TAG, "onSurfaceCreated("
                    //        + mSurfaceHolder + "): " + this);
                    OnSurfaceCreated(mSurfaceHolder);
                    Vector<AutoPtr<ISurfaceHolderCallback> >& callbacks = mSurfaceHolder->GetCallbacks();
                    Vector<AutoPtr<ISurfaceHolderCallback> >::Iterator iter;
                    for (iter = callbacks.Begin(); iter != callbacks.End(); ++iter) {
                        (*iter).Get()->SurfaceCreated(mSurfaceHolder);
                    }
                }

                redrawNeeded |= creating || (relayoutResult
                        & IWindowManagerGlobal::RELAYOUT_RES_FIRST_TIME) != 0;

                if (forceReport || creating || surfaceCreating
                        || formatChanged || sizeChanged) {
//                    if (DEBUG)
//                    {
//                        RuntimeException e = new RuntimeException();
//                        e.fillInStackTrace();
//                        Log.w(TAG, "forceReport=" + forceReport + " creating=" + creating
//                                + " formatChanged=" + formatChanged
//                                + " sizeChanged=" + sizeChanged, e);
//                    }
//                    if (DEBUG) Log.v(TAG, "onSurfaceChanged("
//                            + mSurfaceHolder + ", " + mFormat
//                            + ", " + mCurWidth + ", " + mCurHeight
//                            + "): " + this);
                    didSurface = TRUE;
                    OnSurfaceChanged(mSurfaceHolder, mFormat,
                            mCurWidth, mCurHeight);
                    Vector<AutoPtr<ISurfaceHolderCallback> >& callbacks = mSurfaceHolder->GetCallbacks();
                    Vector<AutoPtr<ISurfaceHolderCallback> >::Iterator iter;
                    for (iter = callbacks.Begin(); iter != callbacks.End(); ++iter) {
                        (*iter).Get()->SurfaceChanged(mSurfaceHolder, mFormat,
                                    mCurWidth, mCurHeight);
                    }
                }

                if (redrawNeeded) {
                    OnSurfaceRedrawNeeded(mSurfaceHolder);
                    Vector<AutoPtr<ISurfaceHolderCallback> >& callbacks = mSurfaceHolder->GetCallbacks();
                    Vector<AutoPtr<ISurfaceHolderCallback> >::Iterator iter;
                    for (iter = callbacks.Begin(); iter != callbacks.End(); ++iter) {
                        AutoPtr<ISurfaceHolderCallback> c = *iter;
                        if (ISurfaceHolderCallback2::Probe(c) != NULL) {
                            ISurfaceHolderCallback2::Probe(c)->SurfaceRedrawNeeded(
                                mSurfaceHolder);
                        }
                    }
                }

                if (didSurface && !mReportedVisible) {
                    // This wallpaper is currently invisible, but its
                    // surface has changed.  At this point let's tell it
                    // again that it is invisible in case the report about
                    // the surface caused it to start running.  We really
                    // don't want wallpapers running when not visible.
                    if (mIsCreating) {
                        // Some wallpapers will ignore this call if they
                        // had previously been told they were invisble,
                        // so if we are creating a new surface then toggle
                        // the state to get them to notice.
                        //if (DEBUG) Log.v(TAG, "onVisibilityChanged(true) at surface: "
                        //        + this);
                        OnVisibilityChanged(TRUE);
                    }
                    //if (DEBUG) Log.v(TAG, "onVisibilityChanged(false) at surface: "
                    //            + this);
                    OnVisibilityChanged(FALSE);
                }

            }
            //finally
            //{
                mIsCreating = false;
                mSurfaceCreated = true;
                if (redrawNeeded) {
                    mSession->FinishDrawing(mWindow);
                }
                mIWallpaperEngine->ReportShown();
            //}
        }
        //catch (RemoteException ex) {
        //}

        //if (DEBUG) Log.v(
        //    TAG, "Layout: x=" + mLayout.x + " y=" + mLayout.y +
        //    " w=" + mLayout.width + " h=" + mLayout.height);
    }
}

void WallpaperService::Engine::Attach(
    /* [in] */ IWallpaperEngineWrapper* wrapper)
{
    //if (DEBUG) Log.v(TAG, "attach: " + this + " wrapper=" + wrapper);
    if (mDestroyed) {
        return;
    }

    mIWallpaperEngine = wrapper;
    mCaller = NULL;
    mConnection = NULL;
    mWindowToken = NULL;
    wrapper->GetHandlerCaller((IHandlerCaller**)&mCaller);
    wrapper->GetConnection((IWallpaperConnection**)&mConnection);
    wrapper->GetWindowToken((IBinder**)&mWindowToken);
    mSurfaceHolder->SetSizeFromLayout();
    mInitializing = TRUE;

    AutoPtr<ILooper> looper;
    mOwner->GetMainLooper((ILooper**)&looper);
    AutoPtr<IWindowManagerGlobal> wg;
    CWindowManagerGlobal::AcquireSingleton((IWindowManagerGlobal**)&wg);
    mSession = NULL;
    wg->GetWindowSession(looper, (IWindowSession**)&mSession);

    mWindow->SetSession(mSession);

    mScreenOn = TRUE;//((IPowerManager*)context->GetSystemService(IContext::POWER_SERVICE))->IsScreenOn();

    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(IIntent::ACTION_SCREEN_ON);
    filter->AddAction(IIntent::ACTION_SCREEN_OFF);
    AutoPtr<IIntent> intent;
    mOwner->RegisterReceiver(mReceiver, filter, (IIntent**)&intent);

    //if (DEBUG) Log.v(TAG, "onCreate(): " + this);
    OnCreate(mSurfaceHolder);

    mInitializing = FALSE;
    mReportedVisible = FALSE;
    UpdateSurface(FALSE, FALSE, FALSE);
}

void WallpaperService::Engine::DoDesiredSizeChanged(
    /* [in] */ Int32 desiredWidth,
    /* [in] */ Int32 desiredHeight)
{
    if (!mDestroyed) {
        //if (DEBUG) Log.v(TAG, "onDesiredSizeChanged("
        //        + desiredWidth + "," + desiredHeight + "): " + this);
        mIWallpaperEngine->SetReqWidth(desiredWidth);
        mIWallpaperEngine->SetReqHeight(desiredHeight);
        OnDesiredSizeChanged(desiredWidth, desiredHeight);
        DoOffsetsChanged(TRUE);
    }
}

void WallpaperService::Engine::DoVisibilityChanged(
    /* [in] */ Boolean visible)
{
    if (!mDestroyed) {
        mVisible = visible;
        ReportVisibility();
    }
}

void WallpaperService::Engine::ReportVisibility()
{
    if (!mDestroyed) {
        Boolean visible = mVisible && mScreenOn;
        if (mReportedVisible != visible) {
            mReportedVisible = visible;
            //if (DEBUG) Log.v(TAG, "onVisibilityChanged(" + visible
            //        + "): " + this);
            if (visible) {
                // If becoming visible, in preview mode the surface
                // may have been destroyed so now we need to make
                // sure it is re-created.
                DoOffsetsChanged(FALSE);
                UpdateSurface(FALSE, FALSE, FALSE);
            }
            OnVisibilityChanged(visible);
        }
    }
}

void WallpaperService::Engine::DoOffsetsChanged(
    /* [in] */ Boolean always)
{
    if (mDestroyed) {
        return;
    }

    if (!always && !mOffsetsChanged) {
        return;
    }

    Float xOffset;
    Float yOffset;
    Float xOffsetStep;
    Float yOffsetStep;
    Boolean sync;
    //synchronized (mLock)
    {
        AutoLock lock(mLock);
        xOffset = mPendingXOffset;
        yOffset = mPendingYOffset;
        xOffsetStep = mPendingXOffsetStep;
        yOffsetStep = mPendingYOffsetStep;
        sync = mPendingSync;
        mPendingSync = FALSE;
        mOffsetMessageEnqueued = FALSE;
    }

    if (mSurfaceCreated) {
        if (mReportedVisible) {
            //if (DEBUG) Log.v(TAG, "Offsets change in " + this
            //        + ": " + xOffset + "," + yOffset);
            Int32 availw;
            mIWallpaperEngine->GetReqWidth(&availw);
            availw -= mCurWidth;
            Int32 xPixels = availw > 0 ? -(Int32)(availw*xOffset+.5f) : 0;
            Int32 availh;
            mIWallpaperEngine->GetReqHeight(&availh);
            availh -= mCurHeight;
            Int32 yPixels = availh > 0 ? -(Int32)(availh*yOffset+.5f) : 0;
            OnOffsetsChanged(xOffset, yOffset, xOffsetStep, yOffsetStep, xPixels, yPixels);
        }
        else {
            mOffsetsChanged = TRUE;
        }
    }

    if (sync)
    {
        //try {
            //if (DEBUG) Log.v(TAG, "Reporting offsets change complete");
            mSession->WallpaperOffsetsComplete(IBinder::Probe(mWindow));
        //} catch (RemoteException e) {
        //}
    }
}

void WallpaperService::Engine::DoCommand(
    /* [in] */ WallpaperCommand* cmd)
{
    AutoPtr<IBundle> result;
    if (!mDestroyed) {
        result = OnCommand(cmd->mAction, cmd->mX, cmd->mY, cmd->mZ,
                cmd->mExtras, cmd->mSync);
    }
    else {
        result = NULL;
    }

    if (cmd->mSync) {
        //try {
            //if (DEBUG) Log.v(TAG, "Reporting command complete");
            mSession->WallpaperCommandComplete(IBinder::Probe(mWindow), result);
       // }
        //catch (RemoteException e) {
        //}
    }
}

void WallpaperService::Engine::ReportSurfaceDestroyed()
{
    if (mSurfaceCreated) {
        mSurfaceCreated = FALSE;
        mSurfaceHolder->UngetCallbacks();
        Vector<AutoPtr<ISurfaceHolderCallback> >& callbacks = mSurfaceHolder->GetCallbacks();
        Vector<AutoPtr<ISurfaceHolderCallback> >::Iterator iter;
        for (iter = callbacks.Begin(); iter != callbacks.End(); ++iter) {
            (*iter).Get()->SurfaceDestroyed(mSurfaceHolder);
        }
        //if (DEBUG) Log.v(TAG, "onSurfaceDestroyed("
        //        + mSurfaceHolder + "): " + this);
        OnSurfaceDestroyed(mSurfaceHolder);
    }
}

void WallpaperService::Engine::Detach()
{
    if (mDestroyed) {
        return;
    }

    mDestroyed = TRUE;

    if (mVisible) {
        mVisible = FALSE;
        //if (DEBUG) Log.v(TAG, "onVisibilityChanged(false): " + this);
        OnVisibilityChanged(FALSE);
    }

    ReportSurfaceDestroyed();

    if (DEBUG)
        Logger::V(TAG, "onDestroy(): %p", this);
    OnDestroy();

    mOwner->UnregisterReceiver(mReceiver);

    if (mCreated) {
        //try {
            //if (DEBUG) Log.v(TAG, "Removing window and destroying surface "
            //        + mSurfaceHolder.getSurface() + " of: " + this);

            if (mInputEventReceiver != NULL) {
                mInputEventReceiver->Dispose();
                mInputEventReceiver = NULL;
            }

            mSession->Remove(mWindow);
        //}
        //catch (RemoteException e) {
        //}
        mSurfaceHolder->mSurface->ReleaseSurface();
        mCreated = FALSE;

        // Dispose the input channel after removing the window so the Window Manager
        // doesn't interpret the input channel being closed as an abnormal termination.
        if (mInputChannel != NULL)
        {
            mInputChannel->Dispose();
            mInputChannel = NULL;
        }
    }
}

ECode WallpaperService::OnCreate()
{
    return Service::OnCreate();
}

ECode WallpaperService::OnDestroy()
{
    Service::OnDestroy();
    List<AutoPtr<Engine> >::Iterator iter = mActiveEngines.Begin();
    for (; iter != mActiveEngines.End(); ++iter) {
        (*iter)->Detach();
    }
    mActiveEngines.Clear();

    return NOERROR;
}

/**
 * Implement to return the implementation of the internal accessibility
 * service interface.  Subclasses should not override.
 */
ECode WallpaperService::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    AutoPtr<IIWallpaperService> service;
    CWallpaperServiceWrapper::New((Handle32)this, (IIWallpaperService**)&service);
    *binder = IBinder::Probe(service);
    assert(*binder != NULL);
    REFCOUNT_ADD(*binder);

    return NOERROR;
}

/**
 * This allows subclasses to change the thread that most callbacks
 * occur on.  Currently hidden because it is mostly needed for the
 * image wallpaper (which runs in the system process and doesn't want
 * to get stuck running on that seriously in use main thread).  Not
 * exposed right now because the semantics of this are not totally
 * well defined and some callbacks can still happen on the main thread).
 * @hide
 */
ECode WallpaperService::SetCallbackLooper(
    /* [in] */ ILooper* looper)
{
    mCallbackLooper = looper;//not using now

    return NOERROR;
}

ECode WallpaperService::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* out,
    /* [in] */ const ArrayOf<String>& args)
{
    // out->Print("State of wallpaper ");
    // out->Print(this);
    // out->Println(":");
    // for (Int32 i=0; i<mActiveEngines.Size(); i++)
    // {
    //     AutoPtr<WallpaperServiceEngine> engine = mActiveEngines.Get(i);
    //     out->Print("  Engine ");
    //     out->Print(engine);
    //     out->Println(":");
    //     engine->Dump("    ", fd, out, args);
    // }
    return E_NOT_IMPLEMENTED;
}

} // namespace Wallpaper
} // namespace Service
} // namepsace Droid
} // namespace Elastos
