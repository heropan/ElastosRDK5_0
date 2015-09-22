#ifndef __ELASTOS_DROID_SERVER_POWER_DISPLAYPOWERSTATE_H__
#define __ELASTOS_DROID_SERVER_POWER_DISPLAYPOWERSTATE_H__

#include "ext/frameworkdef.h"
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
 * This component is similar in nature to a {@link View} except that it describes
 * the properties of a display.  When properties are changed, the component
 * invalidates itself and posts a callback to apply the changes in a consistent order.
 * This mechanism enables multiple properties of the display power state to be animated
 * together smoothly by the animation framework.  Some of the work to blank or unblank
 * the display is done on a separate thread to avoid blocking the looper.
 * </p><p>
 * This component must only be created or accessed by the {@link Looper} thread
 * that belongs to the {@link DisplayPowerController}.
 * </p><p>
 * We don't need to worry about holding a suspend blocker here because the
 * {@link PowerManagerService} does that for us whenever there is a change
 * in progress.
 * </p>
 */
class DisplayPowerState
    : public ElRefBase
    , public IInterface
{
private:
    /**
     * Updates the state of the screen and backlight asynchronously on a separate thread.
     */
    class PhotonicModulator : public ElRefBase
    {
    private:
        class TaskRunnable
            : public ElRefBase
            , public IRunnable
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

    private:
        static const Boolean INITIAL_SCREEN_ON;// = FALSE;    // unknown, assume off
        static const Int32 INITIAL_BACKLIGHT;// = -1;// unknown

        Object mLock;

        Boolean mPendingOn;
        Int32 mPendingBacklight;
        Boolean mActualOn;
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

    class ElectronBeamDrawRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        ElectronBeamDrawRunnable(
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
        /* [in] */ ElectronBeam* electronBean,
        /* [in] */ IDisplayBlanker* displayBlanker,
        /* [in] */ LightsService::Light* backlight);

    /**
     * Sets whether the screen is on or off.
     */
    CARAPI_(void) SetScreenOn(
        /* [in] */ Boolean on);

    /**
     * Returns true if the screen is on.
     */
    CARAPI_(Boolean) IsScreenOn();

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
    CARAPI_(Boolean) PrepareElectronBeam(
        /* [in] */ Int32 mode);

    /**
     * Dismisses the electron beam surface.
     */
    CARAPI_(void) DismissElectronBeam();

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
    CARAPI_(void) SetElectronBeamLevel(
        /* [in] */ Float level);

    /**
     * Gets the level of the electron beam steering current.
     */
    CARAPI_(Float) GetElectronBeamLevel();

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

    CARAPI_(void) ScheduleElectronBeamDraw();

    CARAPI_(void) InvokeCleanListenerIfNeeded();

public:
    const static String fName;
    const static String iName;
    static AutoPtr<IFloatProperty> ELECTRON_BEAM_LEVEL;

    static AutoPtr<IInt32Property> SCREEN_BRIGHTNESS;

    static AutoPtr<IFloatProperty> InitFloatProperty(
        /* [in] */ const String& name);

    static AutoPtr<IInt32Property> InitInt32Property(
        /* [in] */ const String& name);

    // static final FloatProperty<DisplayPowerState> ELECTRON_BEAM_LEVEL =
    //         new FloatProperty<DisplayPowerState>("electronBeamLevel") {
    //     @Override
    //     public void setValue(DisplayPowerState object, float value) {
    //         object.setElectronBeamLevel(value);
    //     }

    //     @Override
    //     public Float get(DisplayPowerState object) {
    //         return object.getElectronBeamLevel();
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
    AutoPtr<ElectronBeam> mElectronBeam;
    AutoPtr<IDisplayBlanker> mDisplayBlanker;
    AutoPtr<LightsService::Light> mBacklight;
    AutoPtr<PhotonicModulator> mPhotonicModulator;

    Boolean mScreenOn;
    Int32 mScreenBrightness;
    Boolean mScreenReady;
    Boolean mScreenUpdatePending;

    Boolean mElectronBeamPrepared;
    Float mElectronBeamLevel;
    Boolean mElectronBeamReady;
    Boolean mElectronBeamDrawPending;

    AutoPtr<IRunnable> mCleanListener;
    AutoPtr<IRunnable> mScreenUpdateRunnable;
    AutoPtr<IRunnable> mElectronBeamDrawRunnable;

    friend class PhotonicModulator;
    friend class ScreenUpdateRunnable;
    friend class ElectronBeamDrawRunnable;
};

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_POWER_DISPLAYPOWERSTATE_H__
