
#include "hardware/display/CWifiDisplayStatus.h"
#include "hardware/display/CWifiDisplay.h"
#include "ext/frameworkext.h"
#include <elastos/Logger.h>
#include <elastos/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Display {

ECode CWifiDisplayStatus::constructor()
{
    return constructor(
        FEATURE_STATE_UNAVAILABLE, SCAN_STATE_NOT_SCANNING,
        DISPLAY_STATE_NOT_CONNECTED, NULL,
        CWifiDisplay::EMPTY_ARRAY, CWifiDisplay::EMPTY_ARRAY);
}

ECode CWifiDisplayStatus::constructor(
    /* [in] */ Int32 featureState,
    /* [in] */ Int32 scanState,
    /* [in] */ Int32 activeDisplayState,
    /* [in] */ IWifiDisplay* activeDisplay,
    /* [in] */ ArrayOf<IWifiDisplay*>* availableDisplays,
    /* [in] */ ArrayOf<IWifiDisplay*>* rememberedDisplays)
{
    if (availableDisplays == NULL) {
        Logger::E("CWifiDisplayStatus", "availableDisplays must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (rememberedDisplays == NULL) {
        Logger::E("CWifiDisplayStatus", "rememberedDisplays must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mFeatureState = featureState;
    mScanState = scanState;
    mActiveDisplayState = activeDisplayState;
    mActiveDisplay = activeDisplay;
    mAvailableDisplays = availableDisplays;
    mRememberedDisplays = rememberedDisplays;

    return NOERROR;
}

ECode CWifiDisplayStatus::GetFeatureState(
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mFeatureState;

    return NOERROR;
}

ECode CWifiDisplayStatus::GetScanState(
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mScanState;

    return NOERROR;
}

ECode CWifiDisplayStatus::GetActiveDisplayState(
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mActiveDisplayState;

    return NOERROR;
}

ECode CWifiDisplayStatus::GetActiveDisplay(
    /* [out] */ IWifiDisplay** display)
{
    VALIDATE_NOT_NULL(display)
    *display = mActiveDisplay;
    INTERFACE_ADDREF(*display);

    return NOERROR;
}

ECode CWifiDisplayStatus::GetAvailableDisplays(
    /* [out, callee] */ ArrayOf<IWifiDisplay*>** displays)
{
    VALIDATE_NOT_NULL(displays);
    *displays = mAvailableDisplays;
    INTERFACE_ADDREF(*displays);

    return NOERROR;
}

ECode CWifiDisplayStatus::GetRememberedDisplays(
    /* [out, callee] */ ArrayOf<IWifiDisplay*>** displays)
{
    VALIDATE_NOT_NULL(displays);
    *displays = mRememberedDisplays;
    INTERFACE_ADDREF(*displays);

    return NOERROR;
}

ECode CWifiDisplayStatus::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(source->ReadInt32(&mFeatureState));
    FAIL_RETURN(source->ReadInt32(&mScanState));
    FAIL_RETURN(source->ReadInt32(&mActiveDisplayState));

    mActiveDisplay = NULL;
    Int32 active;
    FAIL_RETURN(source->ReadInt32(&active));
    if (1 == active) {
        FAIL_RETURN(CWifiDisplay::New((IWifiDisplay**)&mActiveDisplay));
        IParcelable::Probe(mActiveDisplay)->ReadFromParcel(source);
    }

    mAvailableDisplays = NULL;
    Int32 length;;
    FAIL_RETURN(source->ReadInt32(&length));
    mAvailableDisplays = ArrayOf<IWifiDisplay*>::Alloc(length);
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IWifiDisplay> temp;
        FAIL_RETURN(CWifiDisplay::New((IWifiDisplay**)&temp));
        FAIL_RETURN(IParcelable::Probe(temp)->ReadFromParcel(source));
        mAvailableDisplays->Set(i, temp);
    }

    mRememberedDisplays = NULL;
    FAIL_RETURN(source->ReadInt32(&length));
    mRememberedDisplays = ArrayOf<IWifiDisplay*>::Alloc(length);
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IWifiDisplay> temp;
        FAIL_RETURN(CWifiDisplay::New((IWifiDisplay**)&temp));
        FAIL_RETURN(IParcelable::Probe(temp)->ReadFromParcel(source));
        mRememberedDisplays->Set(i, temp);
    }

    return NOERROR;
}

ECode CWifiDisplayStatus::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteInt32(mFeatureState));
    FAIL_RETURN(dest->WriteInt32(mScanState));
    FAIL_RETURN(dest->WriteInt32(mActiveDisplayState));

    if (mActiveDisplay != NULL) {
        FAIL_RETURN(dest->WriteInt32(1));
        FAIL_RETURN(IParcelable::Probe(mActiveDisplay)->WriteToParcel(dest));
    }
    else {
        FAIL_RETURN(dest->WriteInt32(0));
    }

    Int32 length = mAvailableDisplays->GetLength();
    FAIL_RETURN(dest->WriteInt32(length));
    for (Int32 i = 0; i < length; i++) {
        FAIL_RETURN(IParcelable::Probe((*mAvailableDisplays)[i])->WriteToParcel(dest));
    }

    length = mAvailableDisplays->GetLength();
    FAIL_RETURN(dest->WriteInt32(length));
    for (Int32 i = 0; i < length; i++) {
        FAIL_RETURN(IParcelable::Probe((*mRememberedDisplays)[i])->WriteToParcel(dest));
    }

    return NOERROR;
}

ECode CWifiDisplayStatus::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);

    StringBuilder sb("WifiDisplayStatus{featureState=");
    sb += mFeatureState;
    sb += ", scanState=";
    sb += mScanState;
    sb += ", activeDisplayState=";
    sb += mActiveDisplayState;
    sb += ", activeDisplay=";
    String temp("NULL");
    if (mActiveDisplay != NULL) {
        mActiveDisplay->ToString(&temp);
    }
    sb += temp;

    Int32 size = mAvailableDisplays ? mAvailableDisplays->GetLength() : 0;
    sb += ", availableDisplays={size:";
    sb += size;
    sb += " ";
    for (Int32 i = 0; i < size; ++i) {
        if (i != 0) {
            sb += ", ";
        }
        sb += (i + 1);
        sb += " ";
        (*mAvailableDisplays)[i]->ToString(&temp);
        sb += temp;
    }

    size = mRememberedDisplays ? mRememberedDisplays->GetLength() : 0;
    sb += "}, rememberedDisplays={size:";
    sb += size;
    sb += " ";
    for (Int32 i = 0; i < size; ++i) {
        if (i != 0) {
            sb += ", ";
        }
        sb += (i + 1);
        sb += " ";
        (*mRememberedDisplays)[i]->ToString(&temp);
        sb += temp;
    }
    sb += "}}";

    *info = sb.ToString();
    return NOERROR;
}

} // namespace Display
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos
