
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

const AutoPtr<ArrayOf<IWifiDisplay*> > CWifiDisplay::EMPTY_ARRAY = ArrayOf<IWifiDisplay*>::Alloc(0);

ECode CWifiDisplay::constructor()
{
    return NOERROR;
}

ECode CWifiDisplay::constructor(
    /* [in] */ const String& deviceAddress,
    /* [in] */ const String& deviceName,
    /* [in] */ const String& deviceAlias)
{
    if (deviceAddress.IsNullOrEmpty()) {
        Logger::E("CWifiDisplay", "deviceAddress must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (deviceName.IsNullOrEmpty()) {
        Logger::E("CWifiDisplay", "deviceName must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mDeviceAddress = deviceAddress;
    mDeviceName = deviceName;
    mDeviceAlias = deviceAlias;

    return NOERROR;
}

/**
 * Gets the MAC address of the Wifi display device.
 */
ECode CWifiDisplay::GetDeviceAddress(
    /* [out] */ String* deviceAddress)
{
    VALIDATE_NOT_NULL(deviceAddress);
    *deviceAddress = mDeviceAddress;

    return NOERROR;
}

/**
 * Gets the name of the Wifi display device.
 */
ECode CWifiDisplay::GetDeviceName(
    /* [out] */ String* deviceName)
{
    VALIDATE_NOT_NULL(deviceName);
    *deviceName = mDeviceName;

    return NOERROR;
}

/**
 * Gets the user-specified alias of the Wifi display device, or null if none.
 * <p>
 * The alias should be used in the UI whenever available.  It is the value
 * provided by the user when renaming the device.
 * </p>
 */
ECode CWifiDisplay::GetDeviceAlias(
    /* [out] */ String* deviceAlias)
{
    VALIDATE_NOT_NULL(deviceAlias);
    *deviceAlias = mDeviceAlias;

    return NOERROR;
}

/**
 * Gets the name to show in the UI.
 * Uses the device alias if available, otherwise uses the device name.
 */
ECode CWifiDisplay::GetFriendlyDisplayName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mDeviceAlias.IsNull() ? mDeviceName : mDeviceAlias;

    return NOERROR;
}

// //@Override
// public boolean equals(Object o)
// {
//     return o instanceof WifiDisplay && equals((WifiDisplay)o);
// }

ECode CWifiDisplay::EqualsEx(
    /* [in] */ IWifiDisplay* other,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;

    if (other != NULL) {
        String addr, name, alias;
        other->GetDeviceAddress(&addr);
        other->GetDeviceName(&name);
        other->GetDeviceAlias(&alias);
        *res = mDeviceAddress.Equals(addr)
            && mDeviceName.Equals(name)
            && mDeviceAlias.Equals(alias);
    }

    return NOERROR;
}

ECode CWifiDisplay::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return EqualsEx(IWifiDisplay::Probe(other), res);
}

ECode CWifiDisplay::GetHashCode(
    /* [out] */ Int32* code)
{
    // The address on its own should be sufficiently unique for hashing purposes.
    VALIDATE_NOT_NULL(code);
    *code = mDeviceAddress.GetHashCode();
    return NOERROR;
}

ECode CWifiDisplay::HasSameAddress(
    /* [in] */ IWifiDisplay* other,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;

    if (other != NULL) {
        String addr;
        other->GetDeviceAddress(&addr);
        *res = mDeviceAddress.Equals(addr);
    }

    return NOERROR;
}

ECode CWifiDisplay::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(source->ReadString(&mDeviceAddress));
    FAIL_RETURN(source->ReadString(&mDeviceName));
    FAIL_RETURN(source->ReadString(&mDeviceAlias));

    return NOERROR;
}

//@Override
ECode CWifiDisplay::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteString(mDeviceAddress));
    FAIL_RETURN(dest->WriteString(mDeviceName));
    FAIL_RETURN(dest->WriteString(mDeviceAlias));

    return NOERROR;
}

ECode CWifiDisplay::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);

    StringBuilder sb(mDeviceName);
    sb += " (";
    sb += mDeviceAddress;
    sb += ")";
    if (!mDeviceAlias.IsNull()) {
        sb += ", alias ";
        sb += mDeviceAlias;
    }

    *info = sb.ToString();
    return NOERROR;
}

} // namespace Display
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos
