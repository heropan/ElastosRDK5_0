

#ifndef __ELASTOS_DROID_APP_UIAUTOMATION_CONNECTION_H__
#define __ELASTOS_DROID_APP_UIAUTOMATION_CONNECTION_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

// import android.accessibilityservice.AccessibilityServiceInfo;
// import android.accessibilityservice.IAccessibilityServiceClient;
// import android.content.Context;
// import android.graphics.Bitmap;
// import android.hardware.input.InputManager;
// import android.os.Binder;
// import android.os.IBinder;
// import android.os.ParcelFileDescriptor;
// import android.os.Process;
// import android.os.RemoteException;
// import android.os.ServiceManager;
// import android.view.IWindowManager;
// import android.view.InputEvent;
// import android.view.SurfaceControl;
// import android.view.WindowAnimationFrameStats;
// import android.view.WindowContentFrameStats;
// import android.view.accessibility.AccessibilityEvent;
// import android.view.accessibility.IAccessibilityManager;
// import libcore.io.IoUtils;

// import java.io.FileOutputStream;
// import java.io.IOException;
// import java.io.InputStream;
// import java.io.OutputStream;


namespace Elastos {
namespace Droid {
namespace App {

/**
 * This is a remote object that is passed from the shell to an instrumentation
 * for enabling access to privileged operations which the shell can do and the
 * instrumentation cannot. These privileged operations are needed for implementing
 * a {@link UiAutomation} that enables across application testing by simulating
 * user actions and performing screen introspection.
 *
 * @hide
 */
class UiAutomationConnection
    : public Object
    , public IIUiAutomationConnection
    , public IBinder
{
public:
    CAR_INTERFACE_DECL()

    UiAutomationConnection();

    virtual ~UiAutomationConnection();

    CARAPI Connect(
        /* [in] */ IIAccessibilityServiceClient* client);

    CARAPI Disconnect();

    CARAPI InjectInputEvent(
        /* [in] */ IInputEvent* event,
        /* [in] */ Boolean sync,
        /* [out] */ Boolean* result);

    CARAPI SetRotation(
        /* [in] */ Int32 rotation,
        /* [out] */ Boolean** result);

    CARAPI TakeScreenshot(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ IBitmap** bmp);

    CARAPI ClearWindowContentFrameStats(
        /* [in] */ Int32 windowId,
        /* [out] */ Boolean* result);

    CARAPI GetWindowContentFrameStats(
        /* [in] */ Int32 windowId,
        /* [out] */ IWindowContentFrameStats** stats);

    CARAPI ClearWindowAnimationFrameStats();

    CARAPI GetWindowAnimationFrameStats(
        /* [out] */ IWindowAnimationFrameStats** stats);

    CARAPI ExecuteShellCommand(
        /* [in] */ const String& command,
        /* [in] */ IParcelFileDescriptor* sink);

    CARAPI Shutdown();

private:
    CARAPI RegisterUiTestAutomationServiceLocked(
        /* [in] */ IIAccessibilityServiceClient* client);

    CARAPI UnregisterUiTestAutomationServiceLocked();

    CARAPI StoreRotationStateLocked();

    CARAPI RestoreRotationStateLocked();

    Boolean IsConnectedLocked();

    // void throwIfShutdownLocked() {
    //     if (mIsShutdown) {
    //         throw new IllegalStateException("Connection shutdown!");
    //     }
    // }

    // void throwIfNotConnectedLocked() {
    //     if (!isConnectedLocked()) {
    //         throw new IllegalStateException("Not connected!");
    //     }
    // }

    // void throwIfCalledByNotTrustedUidLocked() {
    //     final Int32 callingUid = Binder.getCallingUid();
    //     if (callingUid != mOwningUid && mOwningUid != Process.SYSTEM_UID
    //             && callingUid != 0 /*root*/) {
    //         throw new SecurityException("Calling from not trusted UID!");
    //     }
    // }

private:
    static const Int32 INITIAL_FROZEN_ROTATION_UNSPECIFIED = -1;

    AutoPtr<IIWindowManager> mWindowManager;
    // = IWindowManager.Stub.asInterface(ServiceManager.getService(Service.WINDOW_SERVICE));

    AutoPtr<IIAccessibilityManager> mAccessibilityManager;
    // = IAccessibilityManager.Stub.asInterface(ServiceManager.getService(Service.ACCESSIBILITY_SERVICE));

    Object;

    AutoPtr<IBinder> mToken;// = new Binder();

    Int32 mInitialFrozenRotation;// = INITIAL_FROZEN_ROTATION_UNSPECIFIED;

    AutoPtr<IIAccessibilityServiceClient> mClient;

    Boolean mIsShutdown;

    Int32 mOwningUid;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_UIAUTOMATION_CONNECTION_H__
