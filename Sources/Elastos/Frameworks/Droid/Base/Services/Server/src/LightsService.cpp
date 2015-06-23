
#include "LightsService.h"
#include "CLegacyFlashlightHack.h"
#include "os/ServiceManager.h"
#include "os/Handler.h"
#include <elastos/utility/logging/Slogger.h>
#include <utils/misc.h>
#include <utils/Log.h>
#include <hardware/lights.h>

using Elastos::IO::CFileInputStream;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Os::EIID_IBinder;
using Elastos::Droid::Os::EIID_IHardwareService;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Server::CLegacyFlashlightHack;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {

const Int32 LightsService::_LIGHT_ID_BACKLIGHT;
const Int32 LightsService::_LIGHT_ID_KEYBOARD;
const Int32 LightsService::_LIGHT_ID_BUTTONS;
const Int32 LightsService::_LIGHT_ID_BATTERY;
const Int32 LightsService::_LIGHT_ID_NOTIFICATIONS;
const Int32 LightsService::_LIGHT_ID_ATTENTION;
const Int32 LightsService::_LIGHT_ID_BLUETOOTH;
const Int32 LightsService::_LIGHT_ID_WIFI;
const Int32 LightsService::_LIGHT_ID_COUNT;
const Int32 LightsService::_LIGHT_FLASH_NONE;
const Int32 LightsService::_LIGHT_FLASH_TIMED;
const Int32 LightsService::_LIGHT_FLASH_HARDWARE;
const Int32 LightsService::_BRIGHTNESS_MODE_USER;
const Int32 LightsService::_BRIGHTNESS_MODE_SENSOR;

const String LightsService::TAG("LightsService");
const Boolean LightsService::DEBUG = FALSE;

CAR_INTERFACE_IMPL(LightsService::Light, IInterface)

LightsService::Light::Light(
    /* [in] */ Int32 id,
    /* [in] */ LightsService* owner)
    : mId(id)
    , mColor(0)
    , mMode(0)
    , mOnMS(0)
    , mOffMS(0)
    , mFlashing(FALSE)
    , mOwner(owner)
{
}

ECode LightsService::Light::SetBrightness(
    /* [in] */ Int32 brightness)
{
    return SetBrightness(brightness, _BRIGHTNESS_MODE_USER);
}

ECode LightsService::Light::SetBrightness(
    /* [in] */ Int32 brightness,
    /* [in] */ Int32 brightnessMode)
{
    Mutex::Autolock lock(mSelfLock);
    Int32 color = brightness & 0x000000ff;
    color = 0xff000000 | (color << 16) | (color << 8) | color;
    return SetLightLocked(color, _LIGHT_FLASH_NONE, 0, 0, brightnessMode);
}

ECode LightsService::Light::SetColor(
    /* [in] */ Int32 color)
{
    Mutex::Autolock lock(mSelfLock);
    return SetLightLocked(color, _LIGHT_FLASH_NONE, 0, 0, 0);
}

ECode LightsService::Light::SetFlashing(
    /* [in] */ Int32 color,
    /* [in] */ Int32 mode,
    /* [in] */ Int32 onMS,
    /* [in] */ Int32 offMS)
{
    Mutex::Autolock lock(mSelfLock);
    return SetLightLocked(color, mode, onMS, offMS, _BRIGHTNESS_MODE_USER);
}

ECode LightsService::Light::Pulse()
{
    return Pulse(0x00ffffff, 7);
}

ECode LightsService::Light::Pulse(
    /* [in] */ Int32 color,
    /* [in] */ Int32 onMS)
{
    Mutex::Autolock lock(mSelfLock);
    if (mColor == 0 && !mFlashing) {
        SetLightLocked(color, _LIGHT_FLASH_HARDWARE, onMS, 1000, _BRIGHTNESS_MODE_USER);

        AutoPtr<IMessage> msg;
        mOwner->mH->ObtainMessage(1, (IInterface*)this, (IMessage**)&msg);
        Boolean result;
        mOwner->mH->SendMessageDelayed(msg, onMS, &result);
    }
    return NOERROR;
}

ECode LightsService::Light::TurnOff()
{
    Mutex::Autolock lock(mSelfLock);
    return SetLightLocked(0, _LIGHT_FLASH_NONE, 0, 0, 0);
}

ECode LightsService::Light::StopFlashing()
{
    Mutex::Autolock lock(mSelfLock);
    return SetLightLocked(mColor, _LIGHT_FLASH_NONE, 0, 0, _BRIGHTNESS_MODE_USER);
}

ECode LightsService::Light::SetLightLocked(
    /* [in] */ Int32 color,
    /* [in] */ Int32 mode,
    /* [in] */ Int32 onMS,
    /* [in] */ Int32 offMS,
    /* [in] */ Int32 brightnessMode)
{
    if (color != mColor || mode != mMode || onMS != mOnMS || offMS != mOffMS) {
        if (DEBUG)
            Slogger::V(TAG, "setLight # %d: color=#0x%x", mId, color);

        mColor = color;
        mMode = mode;
        mOnMS = onMS;
        mOffMS = offMS;
        mOwner->SetLight_native(mId, color, mode, onMS, offMS, brightnessMode);
    }
    return NOERROR;
}

ECode LightsService::H::HandleMessage(
    /* [in] */ IMessage* msg)
{
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);
    LightsService::Light* light = (LightsService::Light*)obj.Get();
    light->StopFlashing();
    return NOERROR;
}

LightsService::LightsService(
    /* [in] */ IContext *context)
    : mContext(context)
{
    mH = new H(this);
    CLegacyFlashlightHack::New(context, (IHardwareService**)&mLegacyFlashlightHack);

    Init_native();

    ServiceManager::AddService(String("hardware"), (IHardwareService*)mLegacyFlashlightHack);

    mLights = ArrayOf<Light* >::Alloc(_LIGHT_ID_COUNT);
    for (Int32 i = 0; i < _LIGHT_ID_COUNT; i++) {
        mLights->Set(i, new Light(i, this));
    }
}

LightsService::~LightsService()
{
    Finalize_native();
}

AutoPtr<LightsService::Light> LightsService::GetLight(
    /* [in] */ Int32 id)
{
    return (*mLights)[id];
}

// These values must correspond with the LIGHT_ID constants in
// LightsService.java
enum {
    LIGHT_INDEX_BACKLIGHT = 0,
    LIGHT_INDEX_KEYBOARD = 1,
    LIGHT_INDEX_BUTTONS = 2,
    LIGHT_INDEX_BATTERY = 3,
    LIGHT_INDEX_NOTIFICATIONS = 4,
    LIGHT_INDEX_ATTENTION = 5,
    LIGHT_INDEX_BLUETOOTH = 6,
    LIGHT_INDEX_WIFI = 7,
    LIGHT_COUNT
};

struct Devices {
    light_device_t* lights[LIGHT_COUNT];
};

static light_device_t* get_device(hw_module_t* module, char const* name)
{
    int err;
    hw_device_t* device;
    err = module->methods->open(module, name, &device);
    if (err == 0) {
        return (light_device_t*)device;
    } else {
        return NULL;
    }
}

void LightsService::Init_native()
{
    int err;
    hw_module_t* module;
    Devices* devices;

    devices = (Devices*)malloc(sizeof(Devices));

    err = hw_get_module(LIGHTS_HARDWARE_MODULE_ID, (hw_module_t const**)&module);
    if (err == 0) {
        devices->lights[LIGHT_INDEX_BACKLIGHT]
                = get_device(module, LIGHT_ID_BACKLIGHT);
        devices->lights[LIGHT_INDEX_KEYBOARD]
                = get_device(module, LIGHT_ID_KEYBOARD);
        devices->lights[LIGHT_INDEX_BUTTONS]
                = get_device(module, LIGHT_ID_BUTTONS);
        devices->lights[LIGHT_INDEX_BATTERY]
                = get_device(module, LIGHT_ID_BATTERY);
        devices->lights[LIGHT_INDEX_NOTIFICATIONS]
                = get_device(module, LIGHT_ID_NOTIFICATIONS);
        devices->lights[LIGHT_INDEX_ATTENTION]
                = get_device(module, LIGHT_ID_ATTENTION);
        devices->lights[LIGHT_INDEX_BLUETOOTH]
                = get_device(module, LIGHT_ID_BLUETOOTH);
        devices->lights[LIGHT_INDEX_WIFI]
                = get_device(module, LIGHT_ID_WIFI);
    }
    else {
        memset(devices, 0, sizeof(Devices));
    }

    mNativePointer = (Int32)devices;
}

void LightsService::Finalize_native()
{
    Devices* devices = (Devices*)mNativePointer;
    if (devices == NULL) {
        return;
    }

    free(devices);
}

void LightsService::SetLight_native(
    /* [in] */ Int32 light,
    /* [in] */ Int32 color,
    /* [in] */ Int32 mode,
    /* [in] */ Int32 onMS,
    /* [in] */ Int32 offMS,
    /* [in] */ Int32 brightnessMode)
{
    Devices* devices = (Devices*)mNativePointer;
    light_state_t state;

    if (light < 0 || light >= LIGHT_COUNT || devices->lights[light] == NULL) {
        return ;
    }

    memset(&state, 0, sizeof(light_state_t));
    state.color = color;
    state.flashMode = mode;
    state.flashOnMS = onMS;
    state.flashOffMS = offMS;
    state.brightnessMode = brightnessMode;

    {
        android::ALOGD_IF_SLOW(50, "Excessive delay setting light");
        devices->lights[light]->set_light(devices->lights[light], &state);
    }
}

}
}
}
