
#include "elastos/droid/server/display/VirtualDisplayAdapter.h"

using Elastos::Droid::Hardware::Display::IDisplayManager;
using Elastos::Droid::Utility::CArrayMap;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::ISurface;
using Elastos::Droid::View::ISurfaceControl;
using Elastos::Droid::View::ISurfaceControlHelper;
using Elastos::Droid::View::CSurfaceControlHelper;

using Elastos::IO::IPrintWriter;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

//===================================================================
// VirtualDisplayAdapter::MediaProjectionCallback
//===================================================================

CAR_INTERFACE_IMPL_2(VirtualDisplayAdapter::MediaProjectionCallback, Object, IIMediaProjectionCallback, IBinder)

VirtualDisplayAdapter::MediaProjectionCallback::MediaProjectionCallback()
{}

VirtualDisplayAdapter::MediaProjectionCallback::~MediaProjectionCallback()
{}

ECode VirtualDisplayAdapter::MediaProjectionCallback::constructor(
    /* [in] */ IBinder* appToken,
    /* [out] */ IObject* displayAdapter)
{
    mAppToken = appToken;.
    mDisplayAdapter = (VirtualDisplayAdapter*)displayAdapter;
}

ECode VirtualDisplayAdapter::MediaProjectionCallback::OnStop()
{
    Object* obj = mDisplayAdapter->GetSyncRoot();
    synchronized(obj) {
        mDisplayAdapter->HandleMediaProjectionStoppedLocked(mAppToken);
    }
    return NOERROR;
}

//===================================================================
// VirtualDisplayAdapter::Callback
//===================================================================
const Int32 VirtualDisplayAdapter::Callback::MSG_ON_DISPLAY_PAUSED = 0;
const Int32 VirtualDisplayAdapter::Callback::MSG_ON_DISPLAY_RESUMED = 1;
const Int32 VirtualDisplayAdapter::Callback::MSG_ON_DISPLAY_STOPPED = 2;

VirtualDisplayAdapter::Callback::Callback(
    /* [in] */ IIVirtualDisplayCallback* callback,
    /* [in] */ IHandler* handler,
    /* [in] */ VirtualDisplayAdapter* host)
{
    AutoPtr<ILooper> looper;
    handler->GetLooper((ILooper**)&looper);
    Handler::constructor(looper);
    mCallback = callback;
    mHost = host;
}

ECode VirtualDisplayAdapter::Callback::HandleMessage(
    /* [in] */ IMessage* msg)
{
    try {
        switch (msg.what) {
            case MSG_ON_DISPLAY_PAUSED:
                mCallback->OnPaused();
                break;
            case MSG_ON_DISPLAY_RESUMED:
                mCallback->OnResumed();
                break;
            case MSG_ON_DISPLAY_STOPPED:
                mCallback->OnStopped();
                break;
        }
    } catch (RemoteException e) {
        Slog.w(TAG, "Failed to notify listener of virtual display event.", e);
    }
    return NOERROR;
}

ECode VirtualDisplayAdapter::Callback::DispatchDisplayPaused()
{
    return SendEmptyMessage(MSG_ON_DISPLAY_PAUSED);
}

ECode VirtualDisplayAdapter::Callback::DispatchDisplayResumed()
{
    return SendEmptyMessage(MSG_ON_DISPLAY_RESUMED);
}

ECode VirtualDisplayAdapter::Callback::DispatchDisplayStopped()
{
    return SendEmptyMessage(MSG_ON_DISPLAY_STOPPED);
}

//===================================================================
// VirtualDisplayAdapter::VirtualDisplayDevice
//===================================================================
const Int32 VirtualDisplayAdapter::VirtualDisplayDevice::PENDING_SURFACE_CHANGE = 0x01;
const Int32 VirtualDisplayAdapter::VirtualDisplayDevice::PENDING_RESIZE = 0x02;

CAR_INTERFACE_IMPL(VirtualDisplayAdapter::VirtualDisplayDevice, DisplayDevice, IProxyDeathRecipient)

VirtualDisplayAdapter::VirtualDisplayDevice::VirtualDisplayDevice(
    /* [in] */ IBinder* displayToken,
    /* [in] */ IBinder* appToken,
    /* [in] */ Int32 ownerUid,
    /* [in] */ const String& ownerPackageName,
    /* [in] */ const String& name,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 densityDpi,
    /* [in] */ ISurface* surface,
    /* [in] */ Int32 flags,
    /* [in] */ Callback* callback,
    /* [in] */ VirtualDisplayAdapter* host)
    : DisplayDevice((DisplayAdapter*)host, displayToken);
{
    mAppToken = appToken;
    mOwnerUid = ownerUid;
    mOwnerPackageName = ownerPackageName;
    mName = name;
    mWidth = width;
    mHeight = height;
    mDensityDpi = densityDpi;
    mSurface = surface;
    mFlags = flags;
    mCallback = callback;
    mDisplayState = Display.STATE_UNKNOWN;
    mPendingChanges |= PENDING_SURFACE_CHANGE;
    mStopped = FALSE;
}

ECode VirtualDisplayAdapter::VirtualDisplayDevice::BinderDied()
{
    synchronized (GetSyncRoot()) {
        if (mSurface != NULL) {
            HandleBinderDiedLocked(mAppToken);
        }
    }
    return NOERROR;
}

ECode VirtualDisplayAdapter::VirtualDisplayDevice::DestroyLocked()
{
    if (mSurface != NULL) {
        mSurface->ReleaseSurface();
        mSurface = NULL;
    }
    SurfaceControl.destroyDisplay(getDisplayTokenLocked());
    mCallback.DispatchDisplayStopped();
    return NOERROR;
}

AutoPtr<IRunnable> VirtualDisplayAdapter::VirtualDisplayDevice::RequestDisplayStateLocked(
    /* [in] */ Int32 state)
{
    if (state != mDisplayState) {
        mDisplayState = state;
        if (state == Display.STATE_OFF) {
            mCallback.DispatchDisplayPaused();
        } else {
            mCallback.DispatchDisplayResumed();
        }
    }
    return NULL;
}

void VirtualDisplayAdapter::VirtualDisplayDevice::PerformTraversalInTransactionLocked()
{
    if ((mPendingChanges & PENDING_RESIZE) != 0) {
        SurfaceControl.setDisplaySize(getDisplayTokenLocked(), mWidth, mHeight);
    }
    if ((mPendingChanges & PENDING_SURFACE_CHANGE) != 0) {
        setSurfaceInTransactionLocked(mSurface);
    }
    mPendingChanges = 0;
}

ECode VirtualDisplayAdapter::VirtualDisplayDevice::SetSurfaceLocked(
    /* [in] */ ISurface* surface)
{
    if (!mStopped && mSurface != surface) {
        if ((mSurface != NULL) != (surface != NULL)) {
            SendDisplayDeviceEventLocked(this, DISPLAY_DEVICE_EVENT_CHANGED);
        }
        SendTraversalRequestLocked();
        mSurface = surface;
        mInfo = NULL;
        mPendingChanges |= PENDING_SURFACE_CHANGE;
    }
    return NOERROR;
}

ECode VirtualDisplayAdapter::VirtualDisplayDevice::ResizeLocked(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 densityDpi)
{
    if (mWidth != width || mHeight != height || mDensityDpi != densityDpi) {
        SendDisplayDeviceEventLocked(this, DISPLAY_DEVICE_EVENT_CHANGED);
        SendTraversalRequestLocked();
        mWidth = width;
        mHeight = height;
        mDensityDpi = densityDpi;
        mInfo = NULL;
        mPendingChanges |= PENDING_RESIZE;
    }
    return NOERROR;
}

ECode VirtualDisplayAdapter::VirtualDisplayDevice::StopLocked()
{
    SetSurfaceLocked(NULL);
    mStopped = true;
    return NOERROR;
}

void VirtualDisplayAdapter::VirtualDisplayDevice::DumpLocked(
    /* [in ]*/ IPrintWriter* pw)
{
    // super.dumpLocked(pw);
    // pw.println("mFlags=" + mFlags);
    // pw.println("mDisplayState=" + Display.stateToString(mDisplayState));
    // pw.println("mStopped=" + mStopped);
}

AutoPtr<DisplayDeviceInfo> VirtualDisplayAdapter::VirtualDisplayDevice::GetDisplayDeviceInfoLocked()
{
    if (mInfo == NULL) {
        mInfo = new DisplayDeviceInfo();
        mInfo.name = mName;
        mInfo.width = mWidth;
        mInfo.height = mHeight;
        mInfo.refreshRate = 60;
        mInfo.densityDpi = mDensityDpi;
        mInfo.xDpi = mDensityDpi;
        mInfo.yDpi = mDensityDpi;
        mInfo.presentationDeadlineNanos = 1000000000L / (Int32) mInfo.refreshRate; // 1 frame
        mInfo.flags = 0;
        if ((mFlags & DisplayManager.VIRTUAL_DISPLAY_FLAG_PUBLIC) == 0) {
            mInfo.flags |= DisplayDeviceInfo.FLAG_PRIVATE
                    | DisplayDeviceInfo.FLAG_NEVER_BLANK;
        }
        if ((mFlags & DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR) != 0) {
            mInfo.flags &= ~DisplayDeviceInfo.FLAG_NEVER_BLANK;
        } else {
            mInfo.flags |= DisplayDeviceInfo.FLAG_OWN_CONTENT_ONLY;
        }

        if ((mFlags & DisplayManager.VIRTUAL_DISPLAY_FLAG_SECURE) != 0) {
            mInfo.flags |= DisplayDeviceInfo.FLAG_SECURE;
        }
        if ((mFlags & DisplayManager.VIRTUAL_DISPLAY_FLAG_PRESENTATION) != 0) {
            mInfo.flags |= DisplayDeviceInfo.FLAG_PRESENTATION;
        }
        mInfo.type = Display.TYPE_VIRTUAL;
        mInfo.touch = DisplayDeviceInfo.TOUCH_NONE;
        mInfo.state = mSurface != NULL ? Display.STATE_ON : Display.STATE_OFF;
        mInfo.ownerUid = mOwnerUid;
        mInfo.ownerPackageName = mOwnerPackageName;
    }
    return mInfo;
}


//===================================================================
// VirtualDisplayAdapter
//===================================================================
const String VirtualDisplayAdapter::TAG("VirtualDisplayAdapter");
const Boolean VirtualDisplayAdapter::DEBUG = FALSE;

VirtualDisplayAdapter::VirtualDisplayAdapter(
    /* [in] */ CDisplayManagerService.SyncRoot syncRoot,
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler,
    /* [in] */ IDisplayAdapterListener* listener)
    : DisplayAdapter(syncRoot, context, handler, listener, TAG);
{
    mHandler = handler;
    CArrayMap::New((IArrayMap**)&mVirtualDisplayDevices)
}

AutoPtr<DisplayDevice> VirtualDisplayAdapter::CreateVirtualDisplayLocked(
    /* [in] */ IIVirtualDisplayCallback* callback,
    /* [in] */ IIMediaProjection* projection,
    /* [in] */ Int32 ownerUid,
     /* [in] */ const String& ownerPackageName,
     /* [in] */ const String& name,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 densityDpi,
    /* [in] */ ISurface* surface,
    /* [in] */ Int32 flags)
{
    Boolean secure = (flags & DisplayManager.VIRTUAL_DISPLAY_FLAG_SECURE) != 0;
    IBinder appToken = callback.asBinder();
    IBinder displayToken = SurfaceControl.createDisplay(name, secure);
    VirtualDisplayDevice device = new VirtualDisplayDevice(displayToken, appToken,
            ownerUid, ownerPackageName, name, width, height, densityDpi, surface, flags,
            new Callback(callback, mHandler));

    mVirtualDisplayDevices.put(appToken, device);

    try {
        if (projection != NULL) {
            projection.registerCallback(new MediaProjectionCallback(appToken));
        }
        appToken.linkToDeath(device, 0);
    } catch (RemoteException ex) {
        mVirtualDisplayDevices.remove(appToken);
        device.DestroyLocked();
        return NULL;
    }

    // Return the display device without actually sending the event indicating
    // that it was added.  The caller will handle it.
    return device;
}

ECode VirtualDisplayAdapter::ResizeVirtualDisplayLocked(
    /* [in] */ IBinder* appToken,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 densityDpi)
{
    VirtualDisplayDevice device = mVirtualDisplayDevices.get(appToken);
    if (device != NULL) {
        device.ResizeLocked(width, height, densityDpi);
    }
    return NOERROR;
}

ECode VirtualDisplayAdapter::SetVirtualDisplaySurfaceLocked(
    /* [in] */ IBinder* appToken,
    /* [in] */ ISurface* surface)
{
    VirtualDisplayDevice device = mVirtualDisplayDevices.get(appToken);
    if (device != NULL) {
        device.SetSurfaceLocked(surface);
    }
    return NOERROR;
}

AutoPtr<DisplayDevice> VirtualDisplayAdapter::ReleaseVirtualDisplayLocked(
    /* [in] */ IBinder* appToken)
{
    VirtualDisplayDevice device = mVirtualDisplayDevices.remove(appToken);
    if (device != NULL) {
        device.DestroyLocked();
        appToken.unlinkToDeath(device, 0);
    }

    // Return the display device that was removed without actually sending the
    // event indicating that it was removed.  The caller will handle it.
    return device;
}

void VirtualDisplayAdapter::HandleBinderDiedLocked(
    /* [in] */ IBinder* appToken)
{
    VirtualDisplayDevice device = mVirtualDisplayDevices.remove(appToken);
    if (device != NULL) {
        Slog.i(TAG, "Virtual display device released because application token died: "
                + device.mOwnerPackageName);
        device.DestroyLocked();
        SendDisplayDeviceEventLocked(device, DISPLAY_DEVICE_EVENT_REMOVED);
    }
}

void VirtualDisplayAdapter::HandleMediaProjectionStoppedLocked(
    /* [in] */ IBinder* appToken)
{
    VirtualDisplayDevice device = mVirtualDisplayDevices.remove(appToken);
    if (device != NULL) {
        Slog.i(TAG, "Virtual display device released because media projection stopped: "
                + device.mName);
        device.StopLocked();
    }
}

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos
