
#include "elastos/droid/app/UiAutomationConnection.h"

// using Elastos::Droid::accessibilityservice.AccessibilityServiceInfo;
// using Elastos::Droid::accessibilityservice.IAccessibilityServiceClient;
// using Elastos::Droid::content.Context;
// using Elastos::Droid::graphics.Bitmap;
// using Elastos::Droid::hardware.input.InputManager;
// using Elastos::Droid::os.Binder;
// using Elastos::Droid::os.IBinder;
// using Elastos::Droid::os.ParcelFileDescriptor;
// using Elastos::Droid::os.Process;
// using Elastos::Droid::os.RemoteException;
// using Elastos::Droid::os.ServiceManager;
// using Elastos::Droid::view.IWindowManager;
// using Elastos::Droid::view.InputEvent;
// using Elastos::Droid::view.SurfaceControl;
// using Elastos::Droid::view.WindowAnimationFrameStats;
// using Elastos::Droid::view.WindowContentFrameStats;
// using Elastos::Droid::view.accessibility.AccessibilityEvent;
// using Elastos::Droid::view.accessibility.IAccessibilityManager;
// import libcore.io.IoUtils;

// import java.io.FileOutputStream;
// import java.io.IOException;
// import java.io.InputStream;
// import java.io.OutputStream;


namespace Elastos {
namespace Droid {
namespace App {

const Int32 UiAutomationConnection::INITIAL_FROZEN_ROTATION_UNSPECIFIED = -1;


CAR_INTERFACE_IMPL_2(UiAutomationConnection, Object, IIUiAutomationConnection, IBinder)

UiAutomationConnection::UiAutomationConnection()
    : mInitialFrozenRotation(INITIAL_FROZEN_ROTATION_UNSPECIFIED)
    , mIsShutdown(FALSE)
    , mOwningUid(0)
{
    AutoPtr<IIWindowManager> mWindowManager;
    // = IWindowManager.Stub.asInterface(ServiceManager.getService(Service.WINDOW_SERVICE));

    AutoPtr<IIAccessibilityManager> mAccessibilityManager;
    // = IAccessibilityManager.Stub.asInterface(ServiceManager.getService(Service.ACCESSIBILITY_SERVICE));

}

UiAutomationConnection::~UiAutomationConnection()
{}

ECode UiAutomationConnection::Connect(
    /* [in] */ IIAccessibilityServiceClient* client)
{
    if (client == null) {
        throw new IllegalArgumentException("Client cannot be null!");
    }
    synchronized (mLock) {
        throwIfShutdownLocked();
        if (isConnectedLocked()) {
            throw new IllegalStateException("Already connected.");
        }
        mOwningUid = Binder.getCallingUid();
        registerUiTestAutomationServiceLocked(client);
        storeRotationStateLocked();
    }
}

ECode UiAutomationConnection::Disconnect()
{
    synchronized (mLock) {
        throwIfCalledByNotTrustedUidLocked();
        throwIfShutdownLocked();
        if (!isConnectedLocked()) {
            throw new IllegalStateException("Already disconnected.");
        }
        mOwningUid = -1;
        unregisterUiTestAutomationServiceLocked();
        restoreRotationStateLocked();
    }
}

ECode UiAutomationConnection::InjectInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Boolean sync,
    /* [out] */ Boolean* result)
{
    synchronized (mLock) {
        throwIfCalledByNotTrustedUidLocked();
        throwIfShutdownLocked();
        throwIfNotConnectedLocked();
    }
    final Int32 mode = (sync) ? InputManager.INJECT_INPUT_EVENT_MODE_WAIT_FOR_FINISH
            : InputManager.INJECT_INPUT_EVENT_MODE_ASYNC;
    final long identity = Binder.clearCallingIdentity();
    try {
        return InputManager.getInstance().injectInputEvent(event, mode);
    } finally {
        Binder.restoreCallingIdentity(identity);
    }
}

ECode UiAutomationConnection::SetRotation(
    /* [in] */ Int32 rotation,
    /* [out] */ Boolean** result)
{
    synchronized (mLock) {
        throwIfCalledByNotTrustedUidLocked();
        throwIfShutdownLocked();
        throwIfNotConnectedLocked();
    }
    final long identity = Binder.clearCallingIdentity();
    try {
        if (rotation == UiAutomation.ROTATION_UNFREEZE) {
            mWindowManager.thawRotation();
        } else {
            mWindowManager.freezeRotation(rotation);
        }
        return true;
    } catch (RemoteException re) {
        /* ignore */
    } finally {
        Binder.restoreCallingIdentity(identity);
    }
    return false;
}

ECode UiAutomationConnection::TakeScreenshot(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [out] */ IBitmap** bmp)
{
    synchronized (mLock) {
        throwIfCalledByNotTrustedUidLocked();
        throwIfShutdownLocked();
        throwIfNotConnectedLocked();
    }
    final long identity = Binder.clearCallingIdentity();
    try {
        return SurfaceControl.screenshot(width, height);
    } finally {
        Binder.restoreCallingIdentity(identity);
    }
}

ECode UiAutomationConnection::ClearWindowContentFrameStats(
    /* [in] */ Int32 windowId,
    /* [out] */ Boolean* result)
{
    synchronized (mLock) {
        throwIfCalledByNotTrustedUidLocked();
        throwIfShutdownLocked();
        throwIfNotConnectedLocked();
    }
    final long identity = Binder.clearCallingIdentity();
    try {
        IBinder token = mAccessibilityManager.getWindowToken(windowId);
        if (token == null) {
            return false;
        }
        return mWindowManager.clearWindowContentFrameStats(token);
    } finally {
        Binder.restoreCallingIdentity(identity);
    }
}

ECode UiAutomationConnection::GetWindowContentFrameStats(
    /* [in] */ Int32 windowId,
    /* [out] */ IWindowContentFrameStats** stats)
{
    synchronized (mLock) {
        throwIfCalledByNotTrustedUidLocked();
        throwIfShutdownLocked();
        throwIfNotConnectedLocked();
    }
    final long identity = Binder.clearCallingIdentity();
    try {
        IBinder token = mAccessibilityManager.getWindowToken(windowId);
        if (token == null) {
            return null;
        }
        return mWindowManager.getWindowContentFrameStats(token);
    } finally {
        Binder.restoreCallingIdentity(identity);
    }
}

ECode UiAutomationConnection::ClearWindowAnimationFrameStats()
{
    synchronized (mLock) {
        throwIfCalledByNotTrustedUidLocked();
        throwIfShutdownLocked();
        throwIfNotConnectedLocked();
    }
    final long identity = Binder.clearCallingIdentity();
    try {
        SurfaceControl.clearAnimationFrameStats();
    } finally {
        Binder.restoreCallingIdentity(identity);
    }
}

ECode UiAutomationConnection::GetWindowAnimationFrameStats(
    /* [out] */ IWindowAnimationFrameStats** stats)
{
    synchronized (mLock) {
        throwIfCalledByNotTrustedUidLocked();
        throwIfShutdownLocked();
        throwIfNotConnectedLocked();
    }
    final long identity = Binder.clearCallingIdentity();
    try {
        WindowAnimationFrameStats stats = new WindowAnimationFrameStats();
        SurfaceControl.getAnimationFrameStats(stats);
        return stats;
    } finally {
        Binder.restoreCallingIdentity(identity);
    }
}

ECode UiAutomationConnection::ExecuteShellCommand(
    /* [in] */ const String& command,
    /* [in] */ IParcelFileDescriptor* sink)
{
    synchronized (mLock) {
        throwIfCalledByNotTrustedUidLocked();
        throwIfShutdownLocked();
        throwIfNotConnectedLocked();
    }

    InputStream in = null;
    OutputStream out = null;

    try {
        java.lang.Process process = Runtime.getRuntime().exec(command);

        in = process.getInputStream();
        out = new FileOutputStream(sink.getFileDescriptor());

        final byte[] buffer = new byte[8192];
        while (true) {
            final Int32 readByteCount = in.read(buffer);
            if (readByteCount < 0) {
                break;
            }
            out.write(buffer, 0, readByteCount);
        }
    } catch (IOException ioe) {
        throw new RuntimeException("Error running shell command", ioe);
    } finally {
        IoUtils.closeQuietly(in);
        IoUtils.closeQuietly(out);
        IoUtils.closeQuietly(sink);
    }
}

ECode UiAutomationConnection::Shutdown();
{
    synchronized (mLock) {
        if (isConnectedLocked()) {
            throwIfCalledByNotTrustedUidLocked();
        }
        throwIfShutdownLocked();
        mIsShutdown = true;
        if (isConnectedLocked()) {
            disconnect();
        }
    }
}

private:
ECode UiAutomationConnection::RegisterUiTestAutomationServiceLocked(
    /* [in] */ IIAccessibilityServiceClient* client)
{
    IAccessibilityManager manager = IAccessibilityManager.Stub.asInterface(
            ServiceManager.getService(Context.ACCESSIBILITY_SERVICE));
    AccessibilityServiceInfo info = new AccessibilityServiceInfo();
    info.eventTypes = AccessibilityEvent.TYPES_ALL_MASK;
    info.feedbackType = AccessibilityServiceInfo.FEEDBACK_GENERIC;
    info.flags |= AccessibilityServiceInfo.FLAG_INCLUDE_NOT_IMPORTANT_VIEWS
            | AccessibilityServiceInfo.FLAG_REPORT_VIEW_IDS;
    info.setCapabilities(AccessibilityServiceInfo.CAPABILITY_CAN_RETRIEVE_WINDOW_CONTENT
            | AccessibilityServiceInfo.CAPABILITY_CAN_REQUEST_TOUCH_EXPLORATION
            | AccessibilityServiceInfo.CAPABILITY_CAN_REQUEST_ENHANCED_WEB_ACCESSIBILITY
            | AccessibilityServiceInfo.CAPABILITY_CAN_REQUEST_FILTER_KEY_EVENTS);
    try {
        // Calling out with a lock held is fine since if the system
        // process is gone the client calling in will be killed.
        manager.registerUiTestAutomationService(mToken, client, info);
        mClient = client;
    } catch (RemoteException re) {
        throw new IllegalStateException("Error while registering UiTestAutomationService.", re);
    }
}

ECode UiAutomationConnection::UnregisterUiTestAutomationServiceLocked()
{
    IAccessibilityManager manager = IAccessibilityManager.Stub.asInterface(
          ServiceManager.getService(Context.ACCESSIBILITY_SERVICE));
    try {
        // Calling out with a lock held is fine since if the system
        // process is gone the client calling in will be killed.
        manager.unregisterUiTestAutomationService(mClient);
        mClient = null;
    } catch (RemoteException re) {
        throw new IllegalStateException("Error while unregistering UiTestAutomationService",
                re);
    }
}

ECode UiAutomationConnection::StoreRotationStateLocked()
{
    try {
        if (mWindowManager.isRotationFrozen()) {
            // Calling out with a lock held is fine since if the system
            // process is gone the client calling in will be killed.
            mInitialFrozenRotation = mWindowManager.getRotation();
        }
    } catch (RemoteException re) {
        /* ignore */
    }
}

ECode UiAutomationConnection::RestoreRotationStateLocked()
{
    try {
        if (mInitialFrozenRotation != INITIAL_FROZEN_ROTATION_UNSPECIFIED) {
            // Calling out with a lock held is fine since if the system
            // process is gone the client calling in will be killed.
            mWindowManager.freezeRotation(mInitialFrozenRotation);
        } else {
            // Calling out with a lock held is fine since if the system
            // process is gone the client calling in will be killed.
            mWindowManager.thawRotation();
        }
    } catch (RemoteException re) {
        /* ignore */
    }
}

Boolean UiAutomationConnection::IsConnectedLocked()
{
    return mClient != null;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
