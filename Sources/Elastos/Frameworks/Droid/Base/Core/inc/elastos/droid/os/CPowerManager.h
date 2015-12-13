
#ifndef __ELASTOS_DROID_OS_CPOWERMANAGER_H__
#define __ELASTOS_DROID_OS_CPOWERMANAGER_H__

#include "Elastos.Droid.Content.h"
#include "_Elastos_Droid_Os_CPowerManager.h"
#include "elastos/droid/os/CPowerManagerWakeLock.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * This class gives you control of the power state of the device.
 *
 * <p>
 * <b>Device battery life will be significantly affected by the use of this API.</b>
 * Do not acquire {@link WakeLock}s unless you really need them, use the minimum levels
 * possible, and be sure to release them as soon as possible.
 * </p><p>
 * You can obtain an instance of this class by calling
 * {@link android.content.Context#getSystemService(java.lang.String) Context.getSystemService()}.
 * </p><p>
 * The primary API you'll use is {@link #newWakeLock(int, String) newWakeLock()}.
 * This will create a {@link PowerManager.WakeLock} object.  You can then use methods
 * on the wake lock object to control the power state of the device.
 * </p><p>
 * In practice it's quite simple:
 * {@samplecode
 * PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
 * PowerManager.WakeLock wl = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "My Tag");
 * wl.acquire();
 *   ..screen will stay on during this section..
 * wl.release();
 * }
 * </p><p>
 * The following wake lock levels are defined, with varying effects on system power.
 * <i>These levels are mutually exclusive - you may only specify one of them.</i>
 *
 * <table>
 *     <tr><th>Flag Value</th>
 *     <th>CPU</th> <th>Screen</th> <th>Keyboard</th></tr>
 *
 *     <tr><td>{@link #PARTIAL_WAKE_LOCK}</td>
 *         <td>On*</td> <td>Off</td> <td>Off</td>
 *     </tr>
 *
 *     <tr><td>{@link #SCREEN_DIM_WAKE_LOCK}</td>
 *         <td>On</td> <td>Dim</td> <td>Off</td>
 *     </tr>
 *
 *     <tr><td>{@link #SCREEN_BRIGHT_WAKE_LOCK}</td>
 *         <td>On</td> <td>Bright</td> <td>Off</td>
 *     </tr>
 *
 *     <tr><td>{@link #FULL_WAKE_LOCK}</td>
 *         <td>On</td> <td>Bright</td> <td>Bright</td>
 *     </tr>
 * </table>
 * </p><p>
 * *<i>If you hold a partial wake lock, the CPU will continue to run, regardless of any
 * display timeouts or the state of the screen and even after the user presses the power button.
 * In all other wake locks, the CPU will run, but the user can still put the device to sleep
 * using the power button.</i>
 * </p><p>
 * In addition, you can add two more flags, which affect behavior of the screen only.
 * <i>These flags have no effect when combined with a {@link #PARTIAL_WAKE_LOCK}.</i></p>
 *
 * <table>
 *     <tr><th>Flag Value</th> <th>Description</th></tr>
 *
 *     <tr><td>{@link #ACQUIRE_CAUSES_WAKEUP}</td>
 *         <td>Normal wake locks don't actually turn on the illumination.  Instead, they cause
 *         the illumination to remain on once it turns on (e.g. from user activity).  This flag
 *         will force the screen and/or keyboard to turn on immediately, when the WakeLock is
 *         acquired.  A typical use would be for notifications which are important for the user to
 *         see immediately.</td>
 *     </tr>
 *
 *     <tr><td>{@link #ON_AFTER_RELEASE}</td>
 *         <td>If this flag is set, the user activity timer will be reset when the WakeLock is
 *         released, causing the illumination to remain on a bit longer.  This can be used to
 *         reduce flicker if you are cycling between wake lock conditions.</td>
 *     </tr>
 * </table>
 * <p>
 * Any application using a WakeLock must request the {@code android.permission.WAKE_LOCK}
 * permission in an {@code &lt;uses-permission&gt;} element of the application's manifest.
 * </p>
 */
CarClass(CPowerManager)
    , public Object
    , public IPowerManager
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    /**
     * Gets the minimum supported screen brightness setting.
     * The screen may be allowed to become dimmer than this value but
     * this is the minimum value that can be set by the user.
     * @hide
     */
    CARAPI GetMinimumScreenBrightnessSetting(
        /* [out] */ Int32* screenBrightness);

    /**
     * Gets the maximum supported screen brightness setting.
     * The screen may be allowed to become dimmer than this value but
     * this is the maximum value that can be set by the user.
     * @hide
     */
    CARAPI GetMaximumScreenBrightnessSetting(
        /* [out] */ Int32* screenBrightness);

    /**
     * Gets the default screen brightness setting.
     * @hide
     */
    CARAPI GetDefaultScreenBrightnessSetting(
        /* [out] */ Int32* screenBrightness);

    /**
     * Returns true if the twilight service should be used to adjust screen brightness
     * policy.  This setting is experimental and disabled by default.
     * @hide
     */
    static CARAPI_(Boolean) UseTwilightAdjustmentFeature();

    /**
     * Creates a new wake lock with the specified level and flags.
     * <p>
     * The {@code levelAndFlags} parameter specifies a wake lock level and optional flags
     * combined using the logical OR operator.
     * </p><p>
     * The wake lock levels are: {@link #PARTIAL_WAKE_LOCK},
     * {@link #FULL_WAKE_LOCK}, {@link #SCREEN_DIM_WAKE_LOCK}
     * and {@link #SCREEN_BRIGHT_WAKE_LOCK}.  Exactly one wake lock level must be
     * specified as part of the {@code levelAndFlags} parameter.
     * </p><p>
     * The wake lock flags are: {@link #ACQUIRE_CAUSES_WAKEUP}
     * and {@link #ON_AFTER_RELEASE}.  Multiple flags can be combined as part of the
     * {@code levelAndFlags} parameters.
     * </p><p>
     * Call {@link WakeLock#acquire() acquire()} on the object to acquire the
     * wake lock, and {@link WakeLock#release release()} when you are done.
     * </p><p>
     * {@samplecode
     * PowerManager pm = (PowerManager)mContext.getSystemService(
     *                                          Context.POWER_SERVICE);
     * PowerManager.WakeLock wl = pm.newWakeLock(
     *                                      PowerManager.SCREEN_DIM_WAKE_LOCK
     *                                      | PowerManager.ON_AFTER_RELEASE,
     *                                      TAG);
     * wl.acquire();
     * // ... do work...
     * wl.release();
     * }
     * </p><p>
     * Although a wake lock can be created without special permissions,
     * the {@link android.Manifest.permission#WAKE_LOCK} permission is
     * required to actually acquire or release the wake lock that is returned.
     * </p><p class="note">
     * If using this to keep the screen on, you should strongly consider using
     * {@link android.view.WindowManager.LayoutParams#FLAG_KEEP_SCREEN_ON} instead.
     * This window flag will be correctly managed by the platform
     * as the user moves between applications and doesn't require a special permission.
     * </p>
     *
     * @param levelAndFlags Combination of wake lock level and flag values defining
     * the requested behavior of the WakeLock.
     * @param tag Your class name (or other tag) for debugging purposes.
     *
     * @see WakeLock#acquire()
     * @see WakeLock#release()
     * @see #PARTIAL_WAKE_LOCK
     * @see #FULL_WAKE_LOCK
     * @see #SCREEN_DIM_WAKE_LOCK
     * @see #SCREEN_BRIGHT_WAKE_LOCK
     * @see #ACQUIRE_CAUSES_WAKEUP
     * @see #ON_AFTER_RELEASE
     */
    CARAPI NewWakeLock(
        /* [in] */ Int32 levelAndFlags,
        /* [in] */ const String& tag,
        /* [out] */ IPowerManagerWakeLock** wakeLock);

    static CARAPI ValidateWakeLockParameters(
        /* [in] */ Int32 levelAndFlags,
        /* [in] */ const String& tag);

    /**
     * Notifies the power manager that user activity happened.
     * <p>
     * Resets the auto-off timer and brightens the screen if the device
     * is not asleep.  This is what happens normally when a key or the touch
     * screen is pressed or when some other user activity occurs.
     * This method does not wake up the device if it has been put to sleep.
     * </p><p>
     * Requires the {@link android.Manifest.permission#DEVICE_POWER} permission.
     * </p>
     *
     * @param when The time of the user activity, in the {@link SystemClock#uptimeMillis()}
     * time base.  This timestamp is used to correctly order the user activity request with
     * other power management functions.  It should be set
     * to the timestamp of the input event that caused the user activity.
     * @param noChangeLights If true, does not cause the keyboard backlight to turn on
     * because of this event.  This is set when the power key is pressed.
     * We want the device to stay on while the button is down, but we're about
     * to turn off the screen so we don't want the keyboard backlight to turn on again.
     * Otherwise the lights flash on and then off and it looks weird.
     *
     * @see #wakeUp
     * @see #goToSleep
     */
    CARAPI UserActivity(
        /* [in] */ Int64 when,
        /* [in] */ Boolean noChangeLights);

    /**
     * Notifies the power manager that user activity happened.
     * <p>
     * Resets the auto-off timer and brightens the screen if the device
     * is not asleep.  This is what happens normally when a key or the touch
     * screen is pressed or when some other user activity occurs.
     * This method does not wake up the device if it has been put to sleep.
     * </p><p>
     * Requires the {@link android.Manifest.permission#DEVICE_POWER} or
     * {@link android.Manifest.permission#USER_ACTIVITY} permission.
     * </p>
     *
     * @param when The time of the user activity, in the {@link SystemClock#uptimeMillis()}
     * time base.  This timestamp is used to correctly order the user activity request with
     * other power management functions.  It should be set
     * to the timestamp of the input event that caused the user activity.
     * @param event The user activity event.
     * @param flags Optional user activity flags.
     *
     * @see #wakeUp
     * @see #goToSleep
     *
     * @hide Requires signature or system permission.
     */
    //@SystemApi
    CARAPI UserActivity(
        /* [in] */ Int64 when,
        /* [in] */ Int32 event,
        /* [in] */ Int32 flags);

    /**
     * Forces the device to go to sleep.
     * <p>
     * Overrides all the wake locks that are held.
     * This is what happens when the power key is pressed to turn off the screen.
     * </p><p>
     * Requires the {@link android.Manifest.permission#DEVICE_POWER} permission.
     * </p>
     *
     * @param time The time when the request to go to sleep was issued, in the
     * {@link SystemClock#uptimeMillis()} time base.  This timestamp is used to correctly
     * order the go to sleep request with other power management functions.  It should be set
     * to the timestamp of the input event that caused the request to go to sleep.
     *
     * @see #userActivity
     * @see #wakeUp
     *
     * @removed Requires signature permission.
     */
    CARAPI GoToSleep(
        /* [in] */ Int64 eventTime);

    /**
     * Forces the device to go to sleep.
     * <p>
     * Overrides all the wake locks that are held.
     * This is what happens when the power key is pressed to turn off the screen.
     * </p><p>
     * Requires the {@link android.Manifest.permission#DEVICE_POWER} permission.
     * </p>
     *
     * @param time The time when the request to go to sleep was issued, in the
     * {@link SystemClock#uptimeMillis()} time base.  This timestamp is used to correctly
     * order the go to sleep request with other power management functions.  It should be set
     * to the timestamp of the input event that caused the request to go to sleep.
     * @param reason The reason the device is going to sleep.
     * @param flags Optional flags to apply when going to sleep.
     *
     * @see #userActivity
     * @see #wakeUp
     *
     * @hide Requires signature permission.
     */
    CARAPI GoToSleep(
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 reason,
        /* [in] */ Int32 flags);

    /**
     * Forces the device to wake up from sleep.
     * <p>
     * If the device is currently asleep, wakes it up, otherwise does nothing.
     * This is what happens when the power key is pressed to turn on the screen.
     * </p><p>
     * Requires the {@link android.Manifest.permission#DEVICE_POWER} permission.
     * </p>
     *
     * @param time The time when the request to wake up was issued, in the
     * {@link SystemClock#uptimeMillis()} time base.  This timestamp is used to correctly
     * order the wake up request with other power management functions.  It should be set
     * to the timestamp of the input event that caused the request to wake up.
     *
     * @see #userActivity
     * @see #goToSleep
     *
     * @removed Requires signature permission.
     */
    CARAPI WakeUp(
        /* [in] */ Int64 eventTime);

    /**
     * Forces the device to start napping.
     * <p>
     * If the device is currently awake, starts dreaming, otherwise does nothing.
     * When the dream ends or if the dream cannot be started, the device will
     * either wake up or go to sleep depending on whether there has been recent
     * user activity.
     * </p><p>
     * Requires the {@link android.Manifest.permission#DEVICE_POWER} permission.
     * </p>
     *
     * @param time The time when the request to nap was issued, in the
     * {@link SystemClock#uptimeMillis()} time base.  This timestamp is used to correctly
     * order the nap request with other power management functions.  It should be set
     * to the timestamp of the input event that caused the request to nap.
     *
     * @see #wakeUp
     * @see #goToSleep
     *
     * @hide Requires signature permission.
     */
    CARAPI Nap(
        /* [in] */ Int64 time);

    /**
     * Sets the brightness of the backlights (screen, keyboard, button).
     * <p>
     * Requires the {@link android.Manifest.permission#DEVICE_POWER} permission.
     * </p>
     *
     * @param brightness The brightness value from 0 to 255.
     *
     * {@hide} Requires signature permission.
     */
    CARAPI SetBacklightBrightness(
        /* [in] */ Int32 brightness);

    /**
     * Returns true if the specified wake lock level is supported.
     *
     * @param level The wake lock level to check.
     * @return True if the specified wake lock level is supported.
     *
     * {@hide}
     */
    CARAPI IsWakeLockLevelSupported(
        /* [in] */ Int32 level,
        /* [out] */ Boolean* isSupported);

    /**
      * Returns true if the device is in an interactive state.
      * <p>
      * For historical reasons, the name of this method refers to the power state of
      * the screen but it actually describes the overall interactive state of
      * the device.  This method has been replaced by {@link #isInteractive}.
      * </p><p>
      * The value returned by this method only indicates whether the device is
      * in an interactive state which may have nothing to do with the screen being
      * on or off.  To determine the actual state of the screen,
      * use {@link android.view.Display#getState}.
      * </p>
      *
      * @return True if the device is in an interactive state.
      *
      * @deprecated Use {@link #isInteractive} instead.
      */
    CARAPI IsScreenOn(
        /* [out] */ Boolean* isScreenOn);

    /**
     * Returns true if the device is in an interactive state.
     * <p>
     * When this method returns true, the device is awake and ready to interact
     * with the user (although this is not a guarantee that the user is actively
     * interacting with the device just this moment).  The main screen is usually
     * turned on while in this state.  Certain features, such as the proximity
     * sensor, may temporarily turn off the screen while still leaving the device in an
     * interactive state.  Note in particular that the device is still considered
     * to be interactive while dreaming (since dreams can be interactive) but not
     * when it is dozing or asleep.
     * </p><p>
     * When this method returns false, the device is dozing or asleep and must
     * be awoken before it will become ready to interact with the user again.  The
     * main screen is usually turned off while in this state.  Certain features,
     * such as "ambient mode" may cause the main screen to remain on (albeit in a
     * low power state) to display system-provided content while the device dozes.
     * </p><p>
     * The system will send a {@link android.content.Intent#ACTION_SCREEN_ON screen on}
     * or {@link android.content.Intent#ACTION_SCREEN_OFF screen off} broadcast
     * whenever the interactive state of the device changes.  For historical reasons,
     * the names of these broadcasts refer to the power state of the screen
     * but they are actually sent in response to changes in the overall interactive
     * state of the device, as described by this method.
     * </p><p>
     * Services may use the non-interactive state as a hint to conserve power
     * since the user is not present.
     * </p>
     *
     * @return True if the device is in an interactive state.
     *
     * @see android.content.Intent#ACTION_SCREEN_ON
     * @see android.content.Intent#ACTION_SCREEN_OFF
     */
    CARAPI IsInteractive(
        /* [out] */ Boolean* result);

    /**
     * Reboot the device.  Will not return if the reboot is successful.
     * <p>
     * Requires the {@link android.Manifest.permission#REBOOT} permission.
     * </p>
     *
     * @param reason code to pass to the kernel (e.g., "recovery") to
     *               request special boot modes, or null.
     */
    CARAPI Reboot(
        /* [in] */ const String& reason);

    /**
     * Returns true if the device is currently in power save mode.  When in this mode,
     * applications should reduce their functionality in order to conserve battery as
     * much as possible.  You can monitor for changes to this state with
     * {@link #ACTION_POWER_SAVE_MODE_CHANGED}.
     *
     * @return Returns true if currently in low power mode, else false.
     */
    CARAPI IsPowerSaveMode(
        /* [out] */ Boolean* result);

    /**
     * Set the current power save mode.
     *
     * @return True if the set was allowed.
     *
     * @see #isPowerSaveMode()
     *
     * @hide
     */
    CARAPI SetPowerSaveMode(
        /* [in] */ Boolean mode,
        /* [out] */ Boolean* result);


    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IIPowerManager* service,
        /* [in] */ IHandler* handler);

private:
    static const String TAG;

    AutoPtr<IContext> mContext;
    AutoPtr<IIPowerManager> mService;
    AutoPtr<IHandler> mHandler;

    friend class CPowerManagerWakeLock;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_OS_CPOWERMANAGER_H__
