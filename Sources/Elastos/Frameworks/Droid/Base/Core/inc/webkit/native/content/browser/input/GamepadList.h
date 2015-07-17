
#ifndef __GAMEPADLIST_H__
#define __GAMEPADLIST_H__

// import android.annotation.SuppressLint;
// import android.annotation.TargetApi;
// import android.content.Context;
// import android.hardware.input.InputManager;
// import android.hardware.input.InputManager.InputDeviceListener;
// import android.os.Build;
// import android.view.InputDevice;
// import android.view.InputEvent;
// import android.view.KeyEvent;
// import android.view.MotionEvent;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.base.ThreadUtils;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

/**
 * Class to manage connected gamepad devices list.
 *
 * It is a Java counterpart of GamepadPlatformDataFetcherAndroid and feeds Gamepad API with input
 * data.
 */
//@JNINamespace("content")
class GamepadList
{
private:
    class LazyHolder
    {
    private:
        static const AutoPtr<GamepadList> INSTANCE;
    };

    class InnerInputDeviceListener
        : public Object
        , public IInputDeviceListener
    {
    public:
        InnerInputDeviceListener(
            /* [in] */ GamepadList* owner);

        //@Override
        CARAPI OnInputDeviceChanged(
            /* [in] */ Int32 deviceId);

        //@Override
        CARAPI OnInputDeviceRemoved(
            /* [in] */ Int32 deviceId);

        //@Override
        CARAPI OnInputDeviceAdded(
            /* [in] */ Int32 deviceId);

    private:
        GamepadList* mOwner;
    };

public:
    /**
     * Notifies the GamepadList that a {@link ContentView} is attached to a window and it should
     * prepare itself for gamepad input. It must be called before {@link onGenericMotionEvent} and
     * {@link dispatchKeyEvent}.
     */
    static CARAPI_(void) OnAttachedToWindow(
        /* [in] */ IContext* context);

    /**
     * Notifies the GamepadList that a {@link ContentView} is detached from it's window.
     */
    //@SuppressLint("MissingSuperCall")
    static CARAPI_(void) OnDetachedFromWindow();

    /**
     * Handles key events from the gamepad devices.
     * @return True if the event has been consumed.
     */
    static CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    /**
     * Handles motion events from the gamepad devices.
     * @return True if the event has been consumed.
     */
    static CARAPI_(Boolean) OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event);

    /**
     * @return True if the motion event corresponds to a gamepad event.
     */
    static CARAPI_(Boolean) IsGamepadEvent(
        /* [in] */ IMotionEvent* event);

    /**
     * @return True if event's keycode corresponds to a gamepad key.
     */
    static CARAPI_(Boolean) IsGamepadEvent(
        /* [in] */ IKeyEvent* event);

    //@CalledByNative
    static CARAPI_(void) UpdateGamepadData(
        /* [in] */ Int64 webGamepadsPtr);

    //@CalledByNative
    static CARAPI_(void) NotifyForGamepadsAccess(
        /* [in] */ Boolean isAccessPaused);

private:
    //@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    GamepadList();

    //@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    CARAPI_(void) InitializeDevices();

    //@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    CARAPI_(void) AttachedToWindow(
        /* [in] */ IContext* context);

    //@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    CARAPI_(void) DetachedFromWindow();

    // ------------------------------------------------------------

    CARAPI_(void) OnInputDeviceChangedImpl(
        /* [in] */ Int32 deviceId);

    CARAPI_(void) OnInputDeviceRemovedImpl(
        /* [in] */ Int32 deviceId);

    CARAPI_(void) OnInputDeviceAddedImpl(
        /* [in] */ Int32 deviceId);

    // ------------------------------------------------------------

    static CARAPI_(AutoPtr<GamepadList>) GetInstance();

    CARAPI_(Int32) GetDeviceCount();

    CARAPI_(Boolean) IsDeviceConnected(
        /* [in] */ Int32 index);

    CARAPI_(AutoPtr<GamepadDevice>) GetDeviceById(
        /* [in] */ Int32 deviceId);

    CARAPI_(AutoPtr<GamepadDevice>) GetDevice(
        /* [in] */ Int32 index);

    CARAPI_(Boolean) HandleKeyEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) HandleMotionEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI_(Int32) GetNextAvailableIndex();

    CARAPI_(Boolean) RegisterGamepad(
        /* [in] */ IInputDevice* inputDevice);

    CARAPI_(void) UnregisterGamepad(
        /* [in] */ Int32 deviceId);

    static CARAPI_(Boolean) IsGamepadDevice(
        /* [in] */ IInputDevice* inputDevice);

    CARAPI_(AutoPtr<GamepadDevice>) GetGamepadForEvent(
        /* [in] */ IInputEvent* event);

    static CARAPI_(Boolean) IsGamepadSupported();

    CARAPI_(void) GrabGamepadData(
        /* [in] */ Int64 webGamepadsPtr);

    CARAPI_(void) SetIsGamepadAccessed(
        /* [in] */ Boolean isGamepadAccessed);

    CARAPI_(void) NativeSetGamepadData(
        /* [in] */ Int64 webGamepadsPtr,
        /* [in] */ Int32 index,
        /* [in] */ Boolean mapping,
        /* [in] */ Boolean connected,
        /* [in] */ String devicename,
        /* [in] */ Int64 timestamp,
        /* [in] */ ArrayOf<Float>* axes,
        /* [in] */ ArrayOf<Float>* buttons);

private:
    static const Int32 MAX_GAMEPADS = 4;

    const Object mLock;

    AutoPtr< ArrayOf<GamepadDevice> > mGamepadDevices;
    AutoPtr<IInputManager> mInputManager;
    Int32 mAttachedToWindowCounter;
    Boolean mIsGamepadAccessed;
    AutoPtr<IInputDeviceListener> mInputDeviceListener;
};

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__GAMEPADLIST_H__
