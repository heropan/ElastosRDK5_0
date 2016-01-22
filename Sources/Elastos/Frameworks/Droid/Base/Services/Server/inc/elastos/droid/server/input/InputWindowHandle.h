#ifndef __ELASTOS_DROID_Server_Input_InputWindowHandle_H__
#define __ELASTOS_DROID_Server_Input_InputWindowHandle_H__

#include "_Elastos.Droid.Server.h"
#include "elastos/core/Object.h"
#include "elastos/droid/server/input/InputApplicationHandle.h"
#include "Elastos.Droid.View.h"
#include "Elastos.Droid.Graphics.h"

using Elastos::Droid::Graphics::IRegion;
using Elastos::Droid::Server::Input::IInputWindowHandle;
using Elastos::Droid::View::IInputChannel;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Input {

/**
 * Functions as a handle for a window that can receive input.
 * Enables the native input dispatcher to refer indirectly to the window manager's window state.
 * @hide
 */
class InputWindowHandle
    : public Object
    , public IInputWindowHandle
{
public:
    CAR_INTERFACE_DECL();

    InputWindowHandle();

    virtual ~InputWindowHandle();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IInputApplicationHandle* inputApplicationHandle,
        /* [in] */ IInterface* windowState,
        /* [in] */ Int32 displayId);

public:
    // The input application handle.
    AutoPtr<IInputApplicationHandle> mInputApplicationHandle;

    // The window manager's window state.
    AutoPtr<IInterface> mWindowState;

    // The input channel associated with the window.
    AutoPtr<IInputChannel> mInputChannel;

    // The window name.
    String mName;

    // Window layout params attributes.  (WindowManager.LayoutParams)
    Int32 mLayoutParamsFlags;
    Int32 mLayoutParamsPrivateFlags;
    Int32 mLayoutParamsType;

    // Dispatching timeout.
    Int64 mDispatchingTimeoutNanos;

    // Window frame.
    Int64 mFrameLeft;
    Int64 mFrameTop;
    Int64 mFrameRight;
    Int64 mFrameBottom;

    // Global scaling factor applied to touch events when they are dispatched
    // to the window
    Float mScaleFactor;

    // Window touchable region.
    AutoPtr<IRegion> mTouchableRegion;       // = new Region();

    // Window is visible.
    Boolean mVisible;

    // Window can receive keys.
    Boolean mCanReceiveKeys;

    // Window has focus.
    Boolean mHasFocus;

    // Window has wallpaper.  (window is the current wallpaper target)
    Boolean mHasWallpaper;

    // Input event dispatching is paused.
    Boolean mPaused;

    // Window layer.
    Int32 mLayer;

    // Id of process and user that owns the window.
    Int32 mOwnerPid;
    Int32 mOwnerUid;

    // Window input features.
    Int32 mInputFeatures;

    // Display this input is on.
    Int32 mDisplayId;

private:
    // Pointer to the native input window handle.
    // This field is lazily initialized via JNI.
    //@SuppressWarnings("unused")
    Int64 mPtr;

private:
    void NativeDispose();
};

} // Input
} // Server
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_Server_Input_InputWindowHandle_H__
