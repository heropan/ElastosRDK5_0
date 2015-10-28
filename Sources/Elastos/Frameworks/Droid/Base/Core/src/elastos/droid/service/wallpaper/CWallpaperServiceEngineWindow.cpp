
#include "elastos/droid/service/wallpaper/CWallpaperServiceEngineWindow.h"
#include "elastos/droid/service/wallpaper/CWallpaperEngineWrapper.h"

namespace Elastos {
namespace Droid {
namespace Service {
namespace Wallpaper {

PInterface CWallpaperServiceEngineWindow::Probe(
        /* [in]  */ REIID riid)
{
    return _CWallpaperServiceEngineWindow::Probe(riid);
}

ECode CWallpaperServiceEngineWindow::constructor(
    /* [in] */ Handle32 engine)
{
    mEngine = (WallpaperService::Engine*)engine;

    return NOERROR;
}

ECode CWallpaperServiceEngineWindow::SetSession(
        /* [in] */ IWindowSession* session)
{
    return BaseIWindow::SetSession(session);
}

ECode CWallpaperServiceEngineWindow::SetSeq(
    /* [in] */ Int32 seq)
{
    mSeq = seq;
    return NOERROR;
}

ECode CWallpaperServiceEngineWindow::GetSeq(
    /* [out] */ Int32* seq)
{
    VALIDATE_NOT_NULL(seq);
    *seq = mSeq;
    return NOERROR;
}

ECode CWallpaperServiceEngineWindow::Resized(
    /* [in] */ IRect* frame,
    /* [in] */ IRect* coveredInsets,
    /* [in] */ IRect* visibleInsets,
    /* [in] */ Boolean reportDraw,
    /* [in] */ IConfiguration* newConfig)
{
    AutoPtr<IMessage> msg;
    mEngine->mCaller->ObtainMessage(WallpaperService::MSG_WINDOW_RESIZED,
        reportDraw ? 1 : 0, (IMessage**)&msg);
    return mEngine->mCaller->SendMessage(msg);
}

ECode CWallpaperServiceEngineWindow::Moved(
    /* [in] */ Int32 newX,
    /* [in] */ Int32 newY)
{
    AutoPtr<IMessage> msg;
    mEngine->mCaller->ObtainMessage(WallpaperService::MSG_WINDOW_MOVED,
        newX, newY, (IMessage**)&msg);
    return mEngine->mCaller->SendMessage(msg);
}

ECode CWallpaperServiceEngineWindow::DispatchAppVisibility(
    /* [in] */ Boolean visible)
{
    // // We don't do this in preview mode; we'll let the preview
    // // activity tell us when to run.
    Boolean isPreView;
    mEngine->mIWallpaperEngine->GetIsPreview(&isPreView);
    if (!isPreView) {
        AutoPtr<IMessage> msg;
        mEngine->mCaller->ObtainMessage(WallpaperService::MSG_VISIBILITY_CHANGED,
            visible ? 1 : 0, (IMessage**)&msg);
        return mEngine->mCaller->SendMessage(msg);
    }

    return NOERROR;
}

ECode CWallpaperServiceEngineWindow::DispatchScreenState(
    /* [in] */ Boolean on)
{
    return BaseIWindow::DispatchScreenState(on);
}

ECode CWallpaperServiceEngineWindow::DispatchGetNewSurface()
{
    return BaseIWindow::DispatchGetNewSurface();
}

ECode CWallpaperServiceEngineWindow::WindowFocusChanged(
    /* [in] */ Boolean hasFocus,
    /* [in] */ Boolean inTouchMode)
{
    return BaseIWindow::WindowFocusChanged(hasFocus, inTouchMode);
}

ECode CWallpaperServiceEngineWindow::ExecuteCommand(
    /* [in] */ const String& command,
    /* [in] */ const String& parameters,
    /* [in] */ IParcelFileDescriptor* descriptor)
{
    return BaseIWindow::ExecuteCommand(command, parameters, descriptor);
}

ECode CWallpaperServiceEngineWindow::CloseSystemDialogs(
    /* [in] */ const String& reason)
{
    return BaseIWindow::CloseSystemDialogs(reason);
}

ECode CWallpaperServiceEngineWindow::DispatchWallpaperOffsets(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float xStep,
    /* [in] */ Float yStep,
    /* [in] */ Boolean sync)
{
    AutoLock lock(mEngine->mLock);
    //if (DEBUG) Log.v(TAG, "Dispatch wallpaper offsets: " + x + ", " + y);
    mEngine->mPendingXOffset = x;
    mEngine->mPendingYOffset = y;
    mEngine->mPendingXOffsetStep = xStep;
    mEngine->mPendingYOffsetStep = yStep;
    if (sync) {
        mEngine->mPendingSync = TRUE;
    }
    if (!mEngine->mOffsetMessageEnqueued) {
        mEngine->mOffsetMessageEnqueued = TRUE;
        AutoPtr<IMessage> msg;
        mEngine->mCaller->ObtainMessage(WallpaperService::MSG_WALLPAPER_OFFSETS,
            (IMessage**)&msg);
        return mEngine->mCaller->SendMessage(msg);
    }

    return NOERROR;
}

ECode CWallpaperServiceEngineWindow::DispatchWallpaperCommand(
    /* [in] */ const String& action,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean sync)
{
    AutoLock lock(mEngine->mLock);
    //if (DEBUG) Log.v(TAG, "Dispatch wallpaper command: " + x + ", " + y);
    AutoPtr<WallpaperService::WallpaperCommand> cmd = new WallpaperService::WallpaperCommand();
    cmd->mAction = action;
    cmd->mX = x;
    cmd->mY = y;
    cmd->mZ = z;
    cmd->mExtras = extras;
    cmd->mSync = sync;

    AutoPtr<IMessage> msg;
    mEngine->mCaller->ObtainMessage(WallpaperService::MSG_WALLPAPER_COMMAND,
        cmd, (IMessage**)&msg);
    return mEngine->mCaller->SendMessage(msg);
}

/* Drag/drop */
ECode CWallpaperServiceEngineWindow::DispatchDragEvent(
    /* [in] */ IDragEvent* event)
{
    return BaseIWindow::DispatchDragEvent(event);
}

ECode CWallpaperServiceEngineWindow::DispatchSystemUiVisibilityChanged(
    /* [in] */ Int32 seq,
    /* [in] */ Int32 globalVisibility,
    /* [in] */ Int32 localValue,
    /* [in] */ Int32 localChanges)
{
    return BaseIWindow::DispatchSystemUiVisibilityChanged(seq, globalVisibility, localValue, localChanges);
}

ECode CWallpaperServiceEngineWindow::DoneAnimating()
{
    return BaseIWindow::DoneAnimating();
}

ECode CWallpaperServiceEngineWindow::ToString(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

} // namespace Wallpaper
} // namespace Service
} // namepsace Droid
} // namespace Elastos
