#ifndef __ELASTOS_DROID_SERVER_POWER_DISPLAYPOWERSTATE_H__
#define __ELASTOS_DROID_SERVER_POWER_DISPLAYPOWERSTATE_H__

#include "elastos/droid/ext/frameworkdef.h"
#include "Elastos.Droid.Server_server.h"
#include "power/ElectronBeam.h"
#include "LightsService.h"

using Elastos::Core::IRunnable;
using Elastos::IO::IPrintWriter;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::View::IChoreographer;
using Elastos::Droid::Utility::IFloatProperty;
using Elastos::Droid::Utility::IInt32Property;
using Elastos::Droid::Utility::EIID_IFloatProperty;
using Elastos::Droid::Utility::EIID_IInt32Property;
using Elastos::Droid::Utility::EIID_IProperty;
using Elastos::Core::ECLSID_CFloat;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

/**
 * Controls the display power state.
 * <p>
 * This component is similar in nature to a {@link android.view.View} except that it
 * describes the properties of a display.  When properties are changed, the component
 * invalidates itself and posts a callback to apply the changes in a consistent order.
 * This mechanism enables multiple properties of the display power state to be animated
 * together smoothly by the animation framework.  Some of the work to blank or unblank
 * the display is done on a separate thread to avoid blocking the looper.
 * </p><p>
 * This component must only be created or accessed by the {@link Looper} thread
 * that belongs to the {@link DisplayPowerController}.
 * </p><p>
 * We don't need to worry about holding a suspend blocker here because the
 * power manager does that for us whenever there is a change in progress.
 * </p>
 */
class DisplayPowerState
    : public Object
{
private:
    /**
     * Updates the state of the screen and backlight asynchronously on a separate thread.
     */
    class PhotonicModulator
        : public Handler
    {
    private:
        class TaskRunnable
            : public IRunnable
        {
        public:
            TaskRunnable(
                /* [in] */ PhotonicModulator* host);

            CAR_INTERFACE_DECL();

            ECode Run();

        private:
            PhotonicModulator* mHost;
        };

    public:
        PhotonicModulator(
            /* [in] */ DisplayPowerState* host);

        CARAPI_(Boolean) SetState(
            /* [in] */ Boolean on,
            /* [in] */ Int32 backlight);

        // CARAPI_(void) Dump(
        //     /* [in] */ IPrintWriter* pw);

        void RequestDisplayState(
            /* [in] */ Int32 state);

        void SetBrightness(
            /* [in] */ Int32 backlight);

    private:
        static const Int32 INITIAL_SCREEN_ON;// = FALSE;    // unknown, assume off
        static const Int32 INITIAL_BACKLIGHT;// = -1;// unknown

        Object mLock;

        Int32 mPendingState;
        Int32 mPendingBacklight;
        Int32 mActualState;
        Int32 mActualBacklight;
        Boolean mChangeInProgress;
        AutoPtr<IRunnable> mTask;
        DisplayPowerState* mHost;
    };

    class ScreenUpdateRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        ScreenUpdateRunnable(
            /* [in] */ DisplayPowerState* host);

        CAR_INTERFACE_DECL();

        CARAPI Run();

    private:
        DisplayPowerState* mHost;
    };

    class ColorFadeDrawRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        ColorFadeDrawRunnable(
            /* [in] */ DisplayPowerState* host);

        CAR_INTERFACE_DECL();

        CARAPI Run();
    private:
        DisplayPowerState* mHost;
    };

    class DisplayPowerStateFloatProperty
        : public ElRefBase
        , public IFloatProperty
    {
    public :
        CAR_INTERFACE_DECL()

        DisplayPowerStateFloatProperty(
            /* [in] */ const String& name);

        CARAPI IsReadOnly(
            /* [out] */ Boolean* readOnly);

        CARAPI Set(
            /* [in] */ IInterface* obj,
            /* [in] */ IInterface* value);

        CARAPI Get(
            /* [in] */ IInterface* obj,
            /* [out] */ IInterface** rst);

        CARAPI GetName(
            /* [out] */ String* name);

        CARAPI GetType(
            /* [out] */ ClassID* id);

        CARAPI IsWriteOnly(
            /* [out] */ Boolean* writeOnly);

        CARAPI ForbiddenOperate(
            /* [out] */ Boolean* forbidden);

        CARAPI SetValue(
            /* [in] */ IInterface* obj,
            /* [in] */ Float value);
    private:
        String mName;
        ClassID mClsId;
    };

    class DisplayPowerStateInt32Property
        : public ElRefBase
        , public IInt32Property
    {
    public :
        CAR_INTERFACE_DECL()

        DisplayPowerStateInt32Property(
            /* [in] */ const String& name);

        CARAPI IsReadOnly(
            /* [out] */ Boolean* readOnly);

        CARAPI Set(
            /* [in] */ IInterface* obj,
            /* [in] */ IInterface* value);

        CARAPI Get(
            /* [in] */ IInterface* obj,
            /* [out] */ IInterface** rst);

        CARAPI GetName(
            /* [out] */ String* name);

        CARAPI GetType(
            /* [out] */ ClassID* info);

        CARAPI IsWriteOnly(
            /* [out] */ Boolean* writeOnly);

        CARAPI ForbiddenOperate(
            /* [out] */ Boolean* forbidden);

        CARAPI SetValue(
            /* [in] */ IInterface* obj,
            /* [in] */ Int32 value);
    private:
        String mName;
        ClassID mClsId;
    };

public:
    CAR_INTERFACE_DECL()

    DisplayPowerState(
        /* [in] */ IDisplayBlanker* blanker,
        /* [in] */ LightsService::Light* backlight,
        /* [in] */ ColorFade* electronBeam);

    /**
     * Gets the desired screen state.
     */
    CARAPI_(void) SetScreenState(
        /* [in] */ Int32 state);

    /**
     * Returns true if the screen is on.
     */
    CARAPI_(Int32) GetScreenState();

    /**
     * Sets the display brightness.
     *
     * @param brightness The brightness, ranges from 0 (minimum / off) to 255 (brightest).
     */
    CARAPI_(void) SetScreenBrightness(
        /* [in] */ Int32 brightness);

    /**
     * Gets the screen brightness.
     */
    CARAPI_(Int32) GetScreenBrightness();

    /**
     * Prepares the electron beam to turn on or off.
     * This method should be called before starting an animation because it
     * can take a fair amount of time to prepare the electron beam surface.
     *
     * @param mode The electron beam animation mode to prepare.
     * @return True if the electron beam was prepared.
     */
    CARAPI_(Boolean) PrepareColorFade(
        /* [in] */ IContext* context,
        /* [in] */ Int32 mode);

    /**
     * Dismisses the electron beam surface.
     */
    CARAPI_(void) DismissColorFade();

    /**
     * Sets the level of the electron beam steering current.
     *
     * The display is blanked when the level is 0.0.  In normal use, the electron
     * beam should have a value of 1.0.  The electron beam is unstable in between
     * these states and the picture quality may be compromised.  For best effect,
     * the electron beam should be warmed up or cooled off slowly.
     *
     * Warning: Electron beam emits harmful radiation.  Avoid direct exposure to
     * skin or eyes.
     *
     * @param level The level, ranges from 0.0 (full off) to 1.0 (full on).
     */
    CARAPI_(void) SetColorFadeLevel(
        /* [in] */ Float level);

    /**
     * Gets the level of the electron beam steering current.
     */
    CARAPI_(Float) GetColorFadeLevel();

    /**
     * Returns true if no properties have been invalidated.
     * Otherwise, returns false and promises to invoke the specified listener
     * when the properties have all been applied.
     * The listener always overrides any previously set listener.
     */
    CARAPI_(Boolean) WaitUntilClean(
        /* [in] */ IRunnable* listener);

    // CARAPI_(void) Dump(
    //     /* [in] */ IPrintWriter* pw);

private:
    CARAPI_(void) ScheduleScreenUpdate();

    CARAPI_(void) PostScreenUpdateThreadSafe();

    CARAPI_(void) ScheduleColorFadeDraw();

    CARAPI_(void) InvokeCleanListenerIfNeeded();

public:
    const static String fName;
    const static String iName;

    static AutoPtr<IFloatProperty> ELECTRON_BEAM_LEVEL;
    static AutoPtr<IInt32Property> SCREEN_BRIGHTNESS;

    // static final FloatProperty<DisplayPowerState> ELECTRON_BEAM_LEVEL =
    //         new FloatProperty<DisplayPowerState>("electronBeamLevel") {
    //     @Override
    //     public void setValue(DisplayPowerState object, float value) {
    //         object.setColorFadeLevel(value);
    //     }

    //     @Override
    //     public Float get(DisplayPowerState object) {
    //         return object.getColorFadeLevel();
    //     }
    // };

    // static final IntProperty<DisplayPowerState> SCREEN_BRIGHTNESS =
    //         new IntProperty<DisplayPowerState>("screenBrightness") {
    //     @Override
    //     public void setValue(DisplayPowerState object, int value) {
    //         object.setScreenBrightness(value);
    //     }

    //     @Override
    //     public Integer get(DisplayPowerState object) {
    //         return object.getScreenBrightness();
    //     }
    // };

private:
    static const String TAG;// = "DisplayPowerState";

    static Boolean DEBUG;// = FALSE;

    AutoPtr<IHandler> mHandler;
    AutoPtr<IChoreographer> mChoreographer;
    AutoPtr<IDisplayBlanker> mBlanker;
    AutoPtr<LightsService::Light> mBacklight;
    AutoPtr<ColorFade> mColorFade;
    AutoPtr<IPhotonicModulator> mPhotonicModulator;

    Int32 mScreenState;
    Int32 mScreenBrightness;
    Boolean mScreenReady;
    Boolean mScreenUpdatePending;

    Boolean mColorFadePrepared;
    Float mColorFadeLevel;
    Boolean mColorFadeReady;
    Boolean mColorFadeDrawPending;

    AutoPtr<IRunnable> mCleanListener;
    AutoPtr<IRunnable> mScreenUpdateRunnable;
    AutoPtr<IRunnable> mColorFadeDrawRunnable;

    friend class PhotonicModulator;
    friend class ScreenUpdateRunnable;
    friend class ColorFadeDrawRunnable;
};

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_POWER_DISPLAYPOWERSTATE_H__
