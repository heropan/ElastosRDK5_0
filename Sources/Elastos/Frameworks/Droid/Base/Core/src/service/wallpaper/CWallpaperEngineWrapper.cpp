
#include "service/wallpaper/CWallpaperEngineWrapper.h"
#include <elastos/utility/logging/Logger.h>
#include "os/HandlerCaller.h"

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::HandlerCaller;
using Elastos::Droid::Content::EIID_IContext;

namespace Elastos {
namespace Droid {
namespace Service {
namespace Wallpaper {


const String CWallpaperEngineWrapper::TAG("WallpaperService");
const Boolean CWallpaperEngineWrapper::DEBUG = FALSE;

const Int32 CWallpaperEngineWrapper::DO_ATTACH = 10;
const Int32 CWallpaperEngineWrapper::DO_DETACH = 20;
const Int32 CWallpaperEngineWrapper::DO_SET_DESIRED_SIZE = 30;

const Int32 CWallpaperEngineWrapper::MSG_UPDATE_SURFACE = 10000;
const Int32 CWallpaperEngineWrapper::MSG_VISIBILITY_CHANGED = 10010;
const Int32 CWallpaperEngineWrapper::MSG_WALLPAPER_OFFSETS = 10020;
const Int32 CWallpaperEngineWrapper::MSG_WALLPAPER_COMMAND = 10025;
const Int32 CWallpaperEngineWrapper::MSG_WINDOW_RESIZED = 10030;
const Int32 CWallpaperEngineWrapper::MSG_WINDOW_MOVED = 10035;
const Int32 CWallpaperEngineWrapper::MSG_TOUCH_EVENT = 10040;

ECode CWallpaperEngineWrapper::constructor(
    /* [in] */ Handle32 context,
    /* [in] */ IWallpaperConnection* conn,
    /* [in] */ IBinder* windowToken,
    /* [in] */ Int32 windowType,
    /* [in] */ Boolean isPreview,
    /* [in] */ Int32 reqWidth,
    /* [in] */ Int32 reqHeight)
{
    mConnection = conn;
    mWindowToken = windowToken;
    mWindowType = windowType;
    mIsPreview = isPreview;
    mShownReported = FALSE;
    mReqWidth = reqWidth;
    mReqHeight = reqHeight;
    mOwner = (WallpaperService*)context;

//    if (WallpaperService::DEBUG && mCallbackLooper != NULL)
//    {
//        mCallbackLooper.setMessageLogging(new LogPrinter(Logger::VERBOSE, WallpaperService::TAG));
//    }

    AutoPtr<ILooper> looper = mOwner->mCallbackLooper;
    if (looper == NULL) {
        mOwner->GetMainLooper((ILooper**)&looper);
    }

    mCaller = new HandlerCaller((IContext*)(mOwner->Probe(EIID_IContext)),
        looper, THIS_PROBE(IHandlerCallerCallback), FALSE);

    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_ATTACH, (IMessage**)&msg);
    return mCaller->SendMessage(msg);
}

ECode CWallpaperEngineWrapper::SetDesiredSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_SET_DESIRED_SIZE, width, height, (IMessage**)&msg);
    return mCaller->SendMessage(msg);
}

ECode CWallpaperEngineWrapper::SetVisibility(
    /* [in] */ Boolean visible)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(MSG_VISIBILITY_CHANGED, visible ? 1 : 0, (IMessage**)&msg);
    return mCaller->SendMessage(msg);
}

ECode CWallpaperEngineWrapper::DispatchPointer(
    /* [in] */ IMotionEvent* event)
{
    if (mEngine != NULL) {
        mEngine->DispatchPointer(event);
    }
    else {
        event->Recycle();
    }
    return NOERROR;
}

ECode CWallpaperEngineWrapper::DispatchWallpaperCommand(
    /* [in] */ const String& action,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ IBundle* extras)
{
    if (mEngine != NULL) {
        mEngine->mWindow->DispatchWallpaperCommand(action, x, y, z, extras, FALSE);
    }
    return NOERROR;
}

ECode CWallpaperEngineWrapper::ReportShown()
{
    if (!mShownReported) {
        mShownReported = TRUE;
        //try {
            mConnection->EngineShown(this);
        //} catch (RemoteException e) {
        //    Logger::w(WallpaperService::TAG, "Wallpaper host disappeared", e);
        //    return;
        //}
    }
    return NOERROR;
}

ECode CWallpaperEngineWrapper::Destroy()
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_DETACH, (IMessage**)&msg);
    return mCaller->SendMessage(msg);
}

void CWallpaperEngineWrapper::DoAttach()
{
    if (FAILED(mConnection->AttachEngine(this))) {
        Logger::W(WallpaperService::TAG, "Wallpaper host disappeared");
        return;
    }
    AutoPtr<WallpaperService::Engine> engine = mOwner->OnCreateEngine();
    mEngine = engine;
    mOwner->mActiveEngines.PushBack(engine);
    engine->Attach(this);
}

void CWallpaperEngineWrapper::DoDetach()
{
    mOwner->mActiveEngines.Remove(mEngine);
    mEngine->Detach();
    // break the circular reference
    mEngine = NULL;
}

ECode CWallpaperEngineWrapper::ExecuteMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);

    if (what != DO_ATTACH && what != DO_DETACH && mEngine == NULL) {
        return NOERROR;
    }

    switch (what) {
        case DO_ATTACH: {
            DoAttach();
            return NOERROR;
        }
        case DO_DETACH: {
            DoDetach();
            return NOERROR;
        }
        case DO_SET_DESIRED_SIZE: {
            mEngine->DoDesiredSizeChanged(arg1, arg2);
            return NOERROR;
        }
        case MSG_UPDATE_SURFACE:
            mEngine->UpdateSurface(TRUE, FALSE, FALSE);
            break;
        case MSG_VISIBILITY_CHANGED:
            if (WallpaperService::DEBUG)
                Logger::V(WallpaperService::TAG, "Visibility change in %p: %d", mEngine.Get(), arg1);
            mEngine->DoVisibilityChanged(arg1);
            break;
        case MSG_WALLPAPER_OFFSETS: {
            mEngine->DoOffsetsChanged(TRUE);
        } break;
        case MSG_WALLPAPER_COMMAND: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            WallpaperService::WallpaperCommand* cmd = (WallpaperService::WallpaperCommand*)obj.Get();
            mEngine->DoCommand(cmd);
        } break;
        case MSG_WINDOW_RESIZED: {
            Boolean reportDraw = arg1 != 0;
            mEngine->UpdateSurface(TRUE, FALSE, reportDraw);
            mEngine->DoOffsetsChanged(TRUE);
        } break;
        case MSG_WINDOW_MOVED: {
            // Do nothing. What does it mean for a Wallpaper to move?
        } break;
        case MSG_TOUCH_EVENT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IMotionEvent* ev = IMotionEvent::Probe(obj);
            HandleTouchEvent(ev);
        } break;
        default :
            Logger::W(TAG, "Unknown message type %d", what);
            break;
    }

    return NOERROR;
}

void CWallpaperEngineWrapper::HandleTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    Boolean skip = FALSE;
    Int32 action;
    ev->GetAction(&action);
    if (action == IMotionEvent::ACTION_MOVE) {
        AutoLock lock(mEngine->mLock);
        if (mEngine->mPendingMove.Get() == ev) {
            mEngine->mPendingMove = NULL;
        }
        else {
            // this is not the motion event we are looking for....
            skip = TRUE;
        }
    }

    if (!skip) {
        if (WallpaperService::DEBUG)
            Logger::V(WallpaperService::TAG, "Delivering touch event: %p", ev);
        mEngine->OnTouchEvent(ev);
    }

    ev->Recycle();
}

ECode CWallpaperEngineWrapper::ToString(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWallpaperEngineWrapper::GetHandlerCaller(
    /* [out] */ IHandlerCaller** caller)
{
    VALIDATE_NOT_NULL(caller);
    *caller = mCaller;
    REFCOUNT_ADD(*caller);
    return NOERROR;
}

ECode CWallpaperEngineWrapper::GetConnection(
    /* [out] */ IWallpaperConnection** connection)
{
    VALIDATE_NOT_NULL(connection);
    *connection = mConnection;
    REFCOUNT_ADD(*connection);

    return NOERROR;
}

ECode CWallpaperEngineWrapper::GetWindowToken(
    /* [out] */ IBinder** windowToken)
{
    VALIDATE_NOT_NULL(windowToken);
    *windowToken = mWindowToken;
    REFCOUNT_ADD(*windowToken);

    return NOERROR;
}

ECode CWallpaperEngineWrapper::SetWindowType(
    /* [in] */ Int32 windowType)
{
    mWindowType = windowType;
    return NOERROR;
}

ECode CWallpaperEngineWrapper::GetWindowType(
    /* [out] */ Int32* windowType)
{
    VALIDATE_NOT_NULL(windowType);
    *windowType = mWindowType;
    return NOERROR;
}

ECode CWallpaperEngineWrapper::SetIsPreview(
    /* [in] */ Boolean isPreview)
{
    mIsPreview = isPreview;
    return NOERROR;
}

ECode CWallpaperEngineWrapper::GetIsPreview(
    /* [out] */ Boolean* isPreview)
{
    VALIDATE_NOT_NULL(isPreview);
    *isPreview = mIsPreview;
    return NOERROR;
}

ECode CWallpaperEngineWrapper::SetShownReported(
    /* [in] */ Boolean shownReported)
{
    mShownReported = shownReported;
    return NOERROR;
}

ECode CWallpaperEngineWrapper::GetShownReported(
    /* [out] */ Boolean* shownReported)
{
    VALIDATE_NOT_NULL(shownReported);
    *shownReported = mShownReported;
    return NOERROR;
}

ECode CWallpaperEngineWrapper::SetReqWidth(
    /* [in] */ Int32 reqWidth)
{
    mReqWidth = reqWidth;
    return NOERROR;
}

ECode CWallpaperEngineWrapper::GetReqWidth(
    /* [out] */ Int32* reqWidth)
{
    VALIDATE_NOT_NULL(reqWidth);
    *reqWidth = mReqWidth;
    return NOERROR;
}

ECode CWallpaperEngineWrapper::SetReqHeight(
    /* [in] */ Int32 reqHeight)
{
    mReqHeight = reqHeight;
    return NOERROR;
}

ECode CWallpaperEngineWrapper::GetReqHeight(
    /* [out] */ Int32* reqHeight)
{
    VALIDATE_NOT_NULL(reqHeight);
    *reqHeight = mReqHeight;
    return NOERROR;
}

} // namespace Wallpaper
} // namespace Service
} // namepsace Droid
} // namespace Elastos
