
#include "view/CSurfaceViewWindow.h"
#include "os/Handler.h"
#include "view/CSurfaceView.h"
#include "view/SurfaceView.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::EIID_IBinder;

namespace Elastos {
namespace Droid {
namespace View {

CSurfaceViewWindow::CSurfaceViewWindow()
    : mCurWidth(-1)
    , mCurHeight(-1)
    , mSeq(0)
{
}

ECode CSurfaceViewWindow::constructor(
    /* [in] */ Handle32 surfaceView)
{
    mSurfaceView = (SurfaceView*)surfaceView;
    //mSurfaceView = new WeakReference<SurfaceView>(surfaceView);
    return NOERROR;
}

ECode CSurfaceViewWindow::SetSession(
   /* [in] */ IWindowSession* session)
{
    mSession = session;
    return NOERROR;
}

ECode CSurfaceViewWindow::ExecuteCommand(
    /* [in] */ const String& command,
    /* [in] */ const String& parameters,
    /* [in] */ IParcelFileDescriptor* out)
{
    return NOERROR;
}

ECode CSurfaceViewWindow::Resized(
    /* [in] */ IRect* frame,
    /* [in] */ IRect* contentInsets,
    /* [in] */ IRect* visibleInsets,
    /* [in] */ Boolean reportDraw,
    /* [in] */ IConfiguration* newConfig)
{
    SurfaceView* surfaceView = mSurfaceView;
    if (surfaceView != NULL) {
        Int32 w, h, winW, winH;
        frame->GetWidth(&w);
        frame->GetHeight(&h);
        surfaceView->mWinFrame->GetWidth(&winW);
        surfaceView->mWinFrame->GetHeight(&winH);

        if (SurfaceView::DEBUG) {
            Logger::V("SurfaceView", "surfaceView %p got resized: w=%d h=%d, cur w=%d h=%d\n",
                surfaceView, w, h, mCurWidth, mCurHeight);
        }

        {
            Mutex::Autolock lock(surfaceView->mSurfaceLock);
            Boolean result;
//        try {
            if (reportDraw) {
                surfaceView->mUpdateWindowNeeded = TRUE;
                surfaceView->mReportDrawNeeded = TRUE;
                surfaceView->mHandler->SendEmptyMessage(
                    SurfaceView::UPDATE_WINDOW_MSG, &result);
            }
            else if (winW != w || winH != h) {
                surfaceView->mUpdateWindowNeeded = TRUE;
                surfaceView->mHandler->SendEmptyMessage(
                    SurfaceView::UPDATE_WINDOW_MSG, &result);
            }

//        } finally {
//            surfaceView.mSurfaceLock.unlock();
//        }
        }
    }
    return NOERROR;
}

ECode CSurfaceViewWindow::Moved(
    /* [in] */ Int32 newX,
    /* [in] */ Int32 newY)
{
    return NOERROR;
}

ECode CSurfaceViewWindow::DispatchAppVisibility(
    /* [in] */ Boolean visible)
{
    // The point of SurfaceView is to let the app control the surface.
    return NOERROR;
}

ECode CSurfaceViewWindow::DispatchGetNewSurface()
{
    if (mSurfaceView != NULL) {
        Boolean result;
        mSurfaceView->mHandler->SendEmptyMessage(
            SurfaceView::GET_NEW_SURFACE_MSG, &result);
    }
    return NOERROR;
}

ECode CSurfaceViewWindow::DispatchScreenState(
     /* [in] */ Boolean on)
{
    return NOERROR;
}

ECode CSurfaceViewWindow::WindowFocusChanged(
    /* [in] */ Boolean hasFocus,
    /* [in] */ Boolean touchEnabled)
{
    Logger::W("CSurfaceViewWindow", "Unexpected focus in surface: focus=%d, touchEnabled=%d\n",
            hasFocus, touchEnabled);
    return NOERROR;
}

ECode CSurfaceViewWindow::CloseSystemDialogs(
    /* [in] */ const String& reason)
{
    return NOERROR;
}

ECode CSurfaceViewWindow::DispatchWallpaperOffsets(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float xStep,
    /* [in] */ Float yStep,
    /* [in] */ Boolean sync)
{
    if (sync) {
//        try {
        mSession->WallpaperOffsetsComplete(THIS_PROBE(IBinder));
//        } catch (RemoteException e) {
//        }
    }
    return NOERROR;
}

ECode CSurfaceViewWindow::DispatchWallpaperCommand(
    /* [in] */ const String& action,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean sync)
{
    if (sync) {
//        try {
        mSession->WallpaperCommandComplete(THIS_PROBE(IBinder), NULL);
//        } catch (RemoteException e) {
//        }
    }
    return NOERROR;
}

ECode CSurfaceViewWindow::DispatchDragEvent(
     /* [in] */ IDragEvent* event)
{
    return NOERROR;
}

ECode CSurfaceViewWindow::DispatchSystemUiVisibilityChanged(
     /* [in] */ Int32 seq,
     /* [in] */ Int32 globalVisibility,
     /* [in] */ Int32 localValue,
     /* [in] */ Int32 localChanges)
{
    mSeq = seq;
    return NOERROR;
}

ECode CSurfaceViewWindow::DoneAnimating()
{
    return NOERROR;
}

ECode CSurfaceViewWindow::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    StringBuilder sb("CSurfaceViewWindow: ");
    sb += (Int32)this;
    *str = sb.ToString();
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
