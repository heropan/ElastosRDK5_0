
#ifndef __ELASTOS_DROID_SERVER_LIGHTSSERVICE_H__
#define __ELASTOS_DROID_SERVER_LIGHTSSERVICE_H__

#include "ext/frameworkext.h"
#include "os/HandlerBase.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IHardwareService;
namespace Elastos {
namespace Droid {
namespace Server {

class LightsService
    : public ElRefBase
{
public:
    // conflict with macro of android
    static const Int32 _LIGHT_ID_BACKLIGHT = 0;
    static const Int32 _LIGHT_ID_KEYBOARD = 1;
    static const Int32 _LIGHT_ID_BUTTONS = 2;
    static const Int32 _LIGHT_ID_BATTERY = 3;
    static const Int32 _LIGHT_ID_NOTIFICATIONS = 4;
    static const Int32 _LIGHT_ID_ATTENTION = 5;
    static const Int32 _LIGHT_ID_BLUETOOTH = 6;
    static const Int32 _LIGHT_ID_WIFI = 7;
    static const Int32 _LIGHT_ID_COUNT = 8;

    static const Int32 _LIGHT_FLASH_NONE = 0;
    static const Int32 _LIGHT_FLASH_TIMED = 1;
    static const Int32 _LIGHT_FLASH_HARDWARE = 2;

    /**
     * Light brightness is managed by a user setting.
     */
    static const Int32 _BRIGHTNESS_MODE_USER = 0;

    /**
     * Light brightness is managed by a light sensor.
     */
    static const Int32 _BRIGHTNESS_MODE_SENSOR = 1;

public:
    class H : public HandlerBase
    {
    public:
        H(
            /* [in] */ LightsService* host)
            : mOwner(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        LightsService* mOwner;
    };

    class Light
        : public ElRefBase
        , public IInterface
    {
        friend class LightsService;
    private:
        Light(
            /* [in] */ Int32 id,
            /* [in] */ LightsService* owner);

    public:
        CAR_INTERFACE_DECL()

        CARAPI SetBrightness(
            /* [in] */ Int32 brightness);

        CARAPI SetBrightness(
            /* [in] */ Int32 brightness,
            /* [in] */ Int32 brightnessMode);

        CARAPI SetColor(
            /* [in] */ Int32 color);

        CARAPI SetFlashing(
            /* [in] */ Int32 color,
            /* [in] */ Int32 mode,
            /* [in] */ Int32 onMS,
            /* [in] */ Int32 offMS);

        CARAPI Pulse();

        CARAPI Pulse(
            /* [in] */ Int32 color,
            /* [in] */ Int32 onMS);

        CARAPI TurnOff();

        CARAPI StopFlashing();

        CARAPI SetLightLocked(
            /* [in] */ Int32 color,
            /* [in] */ Int32 mode,
            /* [in] */ Int32 onMS,
            /* [in] */ Int32 offMS,
            /* [in] */ Int32 brightnessMode);

    private:
        Int32 mId;
        Int32 mColor;
        Int32 mMode;
        Int32 mOnMS;
        Int32 mOffMS;
        Boolean mFlashing;
        Object mSelfLock;
        LightsService* mOwner;
    };

public:
    LightsService(
        /* [in] */ IContext* context);

    virtual ~LightsService();

    CARAPI_(AutoPtr<Light>) GetLight(
        /* [in] */ Int32 id);

private:
    CARAPI_(void) Init_native();

    CARAPI_(void) Finalize_native();

    CARAPI_(void) SetLight_native(
        /* [in] */ Int32 light,
        /* [in] */ Int32 color,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 onMS,
        /* [in] */ Int32 offMS,
        /* [in] */ Int32 brightnessMode);

private:
    static const String TAG;
    static const Boolean DEBUG;

    AutoPtr<ArrayOf<Light* > > mLights;

    AutoPtr<IContext> mContext;
    Int32 mNativePointer;

    AutoPtr<IHandler> mH;
    AutoPtr<IHardwareService> mLegacyFlashlightHack;
};

}
}
}

#endif //__ELASTOS_DROID_SERVER_LIGHTSSERVICE_H__
