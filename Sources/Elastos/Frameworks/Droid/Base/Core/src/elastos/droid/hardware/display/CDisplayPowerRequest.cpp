
#include "hardware/display/CDisplayPowerRequest.h"
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>
//#include "view/Display.h"

using Elastos::Droid::Os::IPowerManager;
using Elastos::Droid::View::IDisplay;
using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Display {

CAR_INTERFACE_IMPL(CDisplayPowerRequest, Object, IDisplayPowerRequest)

CAR_OBJECT_IMPL(CDisplayPowerRequest)

ECode CDisplayPowerRequest::constructor()
{
    mPolicy = IDisplayPowerRequest::POLICY_BRIGHT;
    mUseProximitySensor = FALSE;
    mScreenBrightness = IPowerManager::BRIGHTNESS_ON;
    mScreenAutoBrightnessAdjustment = 0.0F;
    mUseAutoBrightness = FALSE;
    mBlockScreenOn = FALSE;
    mDozeScreenBrightness = IPowerManager::BRIGHTNESS_DEFAULT;
    mDozeScreenState = IDisplay::STATE_UNKNOWN;
    return NOERROR;
}

ECode CDisplayPowerRequest::constructor(
    /* [in] */ IDisplayPowerRequest* other)
{
    return CopyFrom(other);
}

ECode CDisplayPowerRequest::IsBrightOrDim(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = (mPolicy == IDisplayPowerRequest::POLICY_BRIGHT || mPolicy == IDisplayPowerRequest::POLICY_DIM);
    return NOERROR;
}

ECode CDisplayPowerRequest::CopyFrom(
    /* [in] */ IDisplayPowerRequest* other)
{
    mPolicy = ((CDisplayPowerRequest*)other)->mPolicy;
    mUseProximitySensor = ((CDisplayPowerRequest*)other)->mUseProximitySensor;
    mScreenBrightness = ((CDisplayPowerRequest*)other)->mScreenBrightness;
    mScreenAutoBrightnessAdjustment = ((CDisplayPowerRequest*)other)->mScreenAutoBrightnessAdjustment;
    mUseAutoBrightness = ((CDisplayPowerRequest*)other)->mUseAutoBrightness;
    mBlockScreenOn = ((CDisplayPowerRequest*)other)->mBlockScreenOn;
    mLowPowerMode = ((CDisplayPowerRequest*)other)->mLowPowerMode;
    mDozeScreenBrightness = ((CDisplayPowerRequest*)other)->mDozeScreenBrightness;
    mDozeScreenState = ((CDisplayPowerRequest*)other)->mDozeScreenState;
    return NOERROR;
}

ECode CDisplayPowerRequest::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if(IDisplayPowerRequest::Probe(o) != NULL) {
        return Equals(IDisplayPowerRequest::Probe(o), result);
    }
    *result = FALSE;
    return NOERROR;
}

ECode CDisplayPowerRequest::Equals(
    /* [in] */ IDisplayPowerRequest* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = (other != NULL
            && mPolicy == ((CDisplayPowerRequest*)other)->mPolicy
            && mUseProximitySensor == ((CDisplayPowerRequest*)other)->mUseProximitySensor
            && mScreenBrightness == ((CDisplayPowerRequest*)other)->mScreenBrightness
            && mScreenAutoBrightnessAdjustment == ((CDisplayPowerRequest*)other)->mScreenAutoBrightnessAdjustment
            && mUseAutoBrightness == ((CDisplayPowerRequest*)other)->mUseAutoBrightness
            && mBlockScreenOn == ((CDisplayPowerRequest*)other)->mBlockScreenOn
            && mLowPowerMode == ((CDisplayPowerRequest*)other)->mLowPowerMode
            && mDozeScreenBrightness == ((CDisplayPowerRequest*)other)->mDozeScreenBrightness
            && mDozeScreenState == ((CDisplayPowerRequest*)other)->mDozeScreenState);
    return NOERROR;
}

ECode CDisplayPowerRequest::GetHashCode(
    /* [out] */ Int32* hc)
{
    VALIDATE_NOT_NULL(hc);

    *hc = 0; // don't care
    return NOERROR;
}

ECode CDisplayPowerRequest::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    StringBuilder sb;
    sb += "policy=";
    String tmp;
    PolicyToString(mPolicy, &tmp);
    sb += tmp;
    sb += ", useProximitySensor=";
    sb += StringUtils::BooleanToString(mUseProximitySensor);
    sb += ", screenBrightness=";
    sb += StringUtils::ToString(mScreenBrightness);
    sb += ", screenAutoBrightnessAdjustment=";
    sb += StringUtils::ToString(mScreenAutoBrightnessAdjustment);
    sb += ", useAutoBrightness=";
    sb += StringUtils::BooleanToString(mUseAutoBrightness);
    sb += ", blockScreenOn=";
    sb += StringUtils::BooleanToString(mBlockScreenOn);
    sb += ", lowPowerMode=";
    sb += StringUtils::BooleanToString(mLowPowerMode);
    sb += ", dozeScreenBrightness=";
    sb += StringUtils::ToString(mDozeScreenBrightness);
    sb += ", dozeScreenState=";
    assert(0 && "TODO Display::StateToString");
    //Display::StateToString(mDozeScreenState, &tmp);
    //sb += tmp;

    return sb.ToString(result);
}

ECode CDisplayPowerRequest::PolicyToString(
    /* [in] */ Int32 policy,
    /* [out] */ String* result)
{
    switch (policy) {
        case IDisplayPowerRequest::POLICY_OFF:
            *result = String("OFF");
            return NOERROR;
        case IDisplayPowerRequest::POLICY_DOZE:
            *result = String("DOZE");
            return NOERROR;
        case IDisplayPowerRequest::POLICY_DIM:
            *result = String("DIM");
            return NOERROR;
        case IDisplayPowerRequest::POLICY_BRIGHT:
            *result = String("BRIGHT");
            return NOERROR;
        default:
            *result = StringUtils::ToString(policy);
            return NOERROR;
    }
}

} // namespace Display
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos