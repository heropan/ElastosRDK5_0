
#include "power/DisplayPowerRequest.h"

using Elastos::Droid::Os::IPowerManager;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

const Int32 DisplayPowerRequest::SCREEN_STATE_OFF;
const Int32 DisplayPowerRequest::SCREEN_STATE_DIM;
const Int32 DisplayPowerRequest::SCREEN_STATE_BRIGHT;

DisplayPowerRequest::DisplayPowerRequest()
    : mScreenState(SCREEN_STATE_BRIGHT)
    , mUseProximitySensor(FALSE)
    , mScreenBrightness(IPowerManager::BRIGHTNESS_ON)
    , mScreenAutoBrightnessAdjustment(0.0f)
    , mUseAutoBrightness(FALSE)
    , mBlockScreenOn(FALSE)
{}

DisplayPowerRequest::DisplayPowerRequest(
    /* [in] */ DisplayPowerRequest* other)
{
    CopyFrom(other);
}

void DisplayPowerRequest::CopyFrom(
    /* [in] */ DisplayPowerRequest* other)
{
    mScreenState = other->mScreenState;
    mUseProximitySensor = other->mUseProximitySensor;
    mScreenBrightness = other->mScreenBrightness;
    mScreenAutoBrightnessAdjustment = other->mScreenAutoBrightnessAdjustment;
    mUseAutoBrightness = other->mUseAutoBrightness;
    mBlockScreenOn = other->mBlockScreenOn;
}

Boolean DisplayPowerRequest::Equals(
    /* [in] */ DisplayPowerRequest* other)
{
    return other != NULL
        && mScreenState == other->mScreenState
        && mUseProximitySensor == other->mUseProximitySensor
        && mScreenBrightness == other->mScreenBrightness
        && mScreenAutoBrightnessAdjustment == other->mScreenAutoBrightnessAdjustment
        && mUseAutoBrightness == other->mUseAutoBrightness
        && mBlockScreenOn == other->mBlockScreenOn;
}

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos
