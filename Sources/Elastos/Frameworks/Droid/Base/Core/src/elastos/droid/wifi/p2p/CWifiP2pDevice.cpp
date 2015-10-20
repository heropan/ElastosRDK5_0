
#include "CWifiP2pDevice.h"
#include "CWifiP2pWfdInfo.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuffer.h>
#include <elastos/utility/logging/Slogger.h>
#include <Elastos.CoreLibrary.h>
#include <unistd.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuffer;
using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::Regex::IMatcher;
using Elastos::Utility::Regex::IPatternHelper;
using Elastos::Utility::Regex::CPatternHelper;

namespace Elastos {
namespace Droid {
namespace Wifi {
namespace P2p {

static Boolean DBG = FALSE;

// These definitions match the ones in wpa_supplicant
/* WPS config methods supported */
const Int32 CWifiP2pDevice::WPS_CONFIG_DISPLAY         = 0x0008;
const Int32 CWifiP2pDevice::WPS_CONFIG_PUSHBUTTON      = 0x0080;
const Int32 CWifiP2pDevice::WPS_CONFIG_KEYPAD          = 0x0100;

/* Device Capability bitmap */
const Int32 CWifiP2pDevice::DEVICE_CAPAB_SERVICE_DISCOVERY         = 1;
const Int32 CWifiP2pDevice::DEVICE_CAPAB_CLIENT_DISCOVERABILITY    = 1<<1;
const Int32 CWifiP2pDevice::DEVICE_CAPAB_CONCURRENT_OPER           = 1<<2;
const Int32 CWifiP2pDevice::DEVICE_CAPAB_INFRA_MANAGED             = 1<<3;
const Int32 CWifiP2pDevice::DEVICE_CAPAB_DEVICE_LIMIT              = 1<<4;
const Int32 CWifiP2pDevice::DEVICE_CAPAB_INVITATION_PROCEDURE      = 1<<5;

/* Group Capability bitmap */
const Int32 CWifiP2pDevice::GROUP_CAPAB_GROUP_OWNER                = 1;
const Int32 CWifiP2pDevice::GROUP_CAPAB_PERSISTENT_GROUP           = 1<<1;
const Int32 CWifiP2pDevice::GROUP_CAPAB_GROUP_LIMIT                = 1<<2;
const Int32 CWifiP2pDevice::GROUP_CAPAB_INTRA_BSS_DIST             = 1<<3;
const Int32 CWifiP2pDevice::GROUP_CAPAB_CROSS_CONN                 = 1<<4;
const Int32 CWifiP2pDevice::GROUP_CAPAB_PERSISTENT_RECONN          = 1<<5;
const Int32 CWifiP2pDevice::GROUP_CAPAB_GROUP_FORMATION            = 1<<6;


static AutoPtr<IPattern> InitPattern(
    /* [in] */ const String& p)
{
    AutoPtr<IPatternHelper> helper;
    CPatternHelper::AcquireSingleton((IPatternHelper**)&helper);
    AutoPtr<IPattern> pattern;
    helper->Compile(p, (IPattern**)&pattern);
    return pattern;
}

AutoPtr<IPattern> CWifiP2pDevice::mDetailedDevicePattern
    = InitPattern(String("((?:[0-9a-f]{2}:){5}[0-9a-f]{2}) (\\d+ )?p2p_dev_addr=((?:[0-9a-f]{2}:){5}[0-9a-f]{2}) pri_dev_type=(\\d+-[0-9a-fA-F]+-\\d+) name='(.*)' config_methods=(0x[0-9a-fA-F]+) dev_capab=(0x[0-9a-fA-F]+) group_capab=(0x[0-9a-fA-F]+)( wfd_dev_info=0x000006([0-9a-fA-F]{12}))?"));
AutoPtr<IPattern> CWifiP2pDevice::mTwoTokenPattern
    = InitPattern(String("(p2p_dev_addr=)?((?:[0-9a-f]{2}:){5}[0-9a-f]{2})"));
AutoPtr<IPattern> CWifiP2pDevice::mThreeTokenPattern
    = InitPattern(String("(?:[0-9a-f]{2}:){5}[0-9a-f]{2} p2p_dev_addr=((?:[0-9a-f]{2}:){5}[0-9a-f]{2})"));
AutoPtr<IPattern> CWifiP2pDevice::mFourTokenPattern
    = InitPattern(String("(?:[0-9a-f]{2}:){5}[0-9a-f]{2} p2p_dev_addr=((?:[0-9a-f]{2}:){5}[0-9a-f]{2}) wfd_dev_info=0x000006([0-9a-fA-F]{12})"));

CWifiP2pDevice::CWifiP2pDevice()
    : mDeviceName("")
    , mDeviceAddress("")
    , mWpsConfigMethodsSupported(0)
    , mDeviceCapability(0)
    , mGroupCapability(0)
    , mStatus(0)
{
}

ECode CWifiP2pDevice::GetDeviceName(
    /* [out] */ String* deviceName)
{
    VALIDATE_NOT_NULL(deviceName);

    *deviceName = mDeviceName;

    return NOERROR;
}

ECode CWifiP2pDevice::SetDeviceName(
    /* [in] */ const String& deviceName)
{
    mDeviceName = deviceName;

    return NOERROR;
}

ECode CWifiP2pDevice::GetDeviceAddress(
    /* [out] */ String* deviceAddress)
{
    VALIDATE_NOT_NULL(deviceAddress);

    *deviceAddress = mDeviceAddress;

    return NOERROR;
}

ECode CWifiP2pDevice::SetDeviceAddress(
    /* [in] */ const String& deviceAddress)
{
    mDeviceAddress = deviceAddress;

    return NOERROR;
}

ECode CWifiP2pDevice::GetPrimaryDeviceType(
    /* [out] */ String* primaryDeviceType)
{
    VALIDATE_NOT_NULL(primaryDeviceType);

    *primaryDeviceType = mPrimaryDeviceType;

    return NOERROR;
}

ECode CWifiP2pDevice::SetPrimaryDeviceType(
    /* [in] */ const String& primaryDeviceType)
{
    mPrimaryDeviceType = primaryDeviceType;

    return NOERROR;
}

ECode CWifiP2pDevice::GetSecondaryDeviceType(
    /* [out] */ String* secondaryDeviceType)
{
    VALIDATE_NOT_NULL(secondaryDeviceType);

    *secondaryDeviceType = mSecondaryDeviceType;

    return NOERROR;
}

ECode CWifiP2pDevice::SetSecondaryDeviceType(
    /* [in] */ const String& secondaryDeviceType)
{
    mSecondaryDeviceType = secondaryDeviceType;

    return NOERROR;
}

ECode CWifiP2pDevice::GetWpsConfigMethodsSupported(
    /* [out] */ Int32* wpsConfigMethodsSupported)
{
    VALIDATE_NOT_NULL(wpsConfigMethodsSupported);

    *wpsConfigMethodsSupported = mWpsConfigMethodsSupported;

    return NOERROR;
}

ECode CWifiP2pDevice::SetWpsConfigMethodsSupported(
    /* [in] */ Int32 wpsConfigMethodsSupported)
{
    mWpsConfigMethodsSupported = wpsConfigMethodsSupported;
    if (DBG) {
        Slogger::D("WifiP2pDevice", "SetWpsConfigMethodsSupported : %d", mWpsConfigMethodsSupported);
    }
    return NOERROR;
}

ECode CWifiP2pDevice::GetDeviceCapability(
    /* [out] */ Int32* deviceCapability)
{
    VALIDATE_NOT_NULL(deviceCapability);

    *deviceCapability = mDeviceCapability;

    return NOERROR;
}

ECode CWifiP2pDevice::SetDeviceCapability(
    /* [in] */ Int32 deviceCapability)
{
    mDeviceCapability = deviceCapability;
    if (DBG) {
        Slogger::D("WifiP2pDevice", "SetDeviceCapability : %d", deviceCapability);
    }
    return NOERROR;
}

ECode CWifiP2pDevice::GetGroupCapability(
    /* [out] */ Int32* groupCapability)
{
    VALIDATE_NOT_NULL(groupCapability);

    *groupCapability = mGroupCapability;

    return NOERROR;
}

ECode CWifiP2pDevice::SetGroupCapability(
    /* [in] */ Int32 groupCapability)
{
    mGroupCapability = groupCapability;

    return NOERROR;
}

ECode CWifiP2pDevice::GetStatus(
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);

    *status = mStatus;

    return NOERROR;
}

ECode CWifiP2pDevice::SetStatus(
    /* [in] */ Int32 status)
{
    mStatus = status;

    return NOERROR;
}

ECode CWifiP2pDevice::GetWfdInfo(
    /* [out] */ IWifiP2pWfdInfo** wfdInfo)
{
    VALIDATE_NOT_NULL(wfdInfo);

    *wfdInfo = mWfdInfo;
    REFCOUNT_ADD(*wfdInfo);

    return NOERROR;
}

ECode CWifiP2pDevice::SetWfdInfo(
    /* [in] */ IWifiP2pWfdInfo* wfdInfo)
{
    mWfdInfo = wfdInfo;

    return NOERROR;
}

ECode CWifiP2pDevice::WpsPbcSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    *supported = ((mWpsConfigMethodsSupported & WPS_CONFIG_PUSHBUTTON) != 0);

    return NOERROR;
}

ECode CWifiP2pDevice::WpsKeypadSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    *supported = ((mWpsConfigMethodsSupported & WPS_CONFIG_KEYPAD) != 0);

    return NOERROR;
}

ECode CWifiP2pDevice::WpsDisplaySupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    *supported = ((mWpsConfigMethodsSupported & WPS_CONFIG_DISPLAY) != 0);

    return NOERROR;
}

ECode CWifiP2pDevice::IsServiceDiscoveryCapable(
    /* [out] */ Boolean* capable)
{
    VALIDATE_NOT_NULL(capable);

    *capable = ((mDeviceCapability & DEVICE_CAPAB_SERVICE_DISCOVERY) != 0);

    return NOERROR;
}

ECode CWifiP2pDevice::IsInvitationCapable(
    /* [out] */ Boolean* capable)
{
    VALIDATE_NOT_NULL(capable);

    *capable = ((mDeviceCapability & DEVICE_CAPAB_INVITATION_PROCEDURE) != 0);

    return NOERROR;
}

ECode CWifiP2pDevice::IsDeviceLimit(
    /* [out] */ Boolean* isDeviceLimit)
{
    VALIDATE_NOT_NULL(isDeviceLimit);

    *isDeviceLimit = ((mDeviceCapability & DEVICE_CAPAB_DEVICE_LIMIT) != 0);

    return NOERROR;
}

ECode CWifiP2pDevice::IsGroupOwner(
    /* [out] */ Boolean* isGroupOwner)
{
    VALIDATE_NOT_NULL(isGroupOwner);

    *isGroupOwner = ((mGroupCapability & GROUP_CAPAB_GROUP_OWNER) != 0);

    return NOERROR;
}

ECode CWifiP2pDevice::IsGroupLimit(
    /* [out] */ Boolean* isGroupLimit)
{
    VALIDATE_NOT_NULL(isGroupLimit);

    *isGroupLimit = ((mGroupCapability & GROUP_CAPAB_GROUP_LIMIT) != 0);

    return NOERROR;
}

ECode CWifiP2pDevice::Update(
    /* [in] */ IWifiP2pDevice* device)
{
    if (device == NULL) return NOERROR;

    String temp;
    FAIL_RETURN(device->GetDeviceAddress(&temp));
    if (temp.IsNull()) return NOERROR;

    FAIL_RETURN(device->GetDeviceName(&mDeviceName));
    FAIL_RETURN(device->GetPrimaryDeviceType(&mPrimaryDeviceType));
    FAIL_RETURN(device->GetSecondaryDeviceType(&mSecondaryDeviceType));
    FAIL_RETURN(device->GetWpsConfigMethodsSupported(&mWpsConfigMethodsSupported));
    FAIL_RETURN(device->GetDeviceCapability(&mDeviceCapability));
    FAIL_RETURN(device->GetGroupCapability(&mGroupCapability));

    AutoPtr<IWifiP2pWfdInfo> wfdInfo;
    FAIL_RETURN(device->GetWfdInfo((IWifiP2pWfdInfo**)&wfdInfo));
    mWfdInfo = wfdInfo;

    if (DBG) {
        ToString(&temp);
        Slogger::D("WifiP2pDevice", "Update : %s", temp.string());
    }
    return NOERROR;
}

ECode CWifiP2pDevice::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);

    if (this->Probe(EIID_IInterface) == obj) {
        *isEqual = TRUE;
        return NOERROR;
    }

    IWifiP2pDevice* other = IWifiP2pDevice::Probe(obj);
    if (other == NULL) {
        *isEqual = FALSE;
        return NOERROR;
    }

    String oa;
    other->GetDeviceAddress(&oa);
    if (oa.IsNull()) {
        *isEqual = mDeviceAddress.IsNull();
        return NOERROR;
    }

    *isEqual = oa.Equals(mDeviceAddress);
    return NOERROR;
}

ECode CWifiP2pDevice::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuffer sbuf;
    sbuf.AppendCStr("Device: ");
    sbuf.AppendString(mDeviceName);
    sbuf.AppendCStr("\n deviceAddress: ");
    sbuf.AppendString(mDeviceAddress);
    sbuf.AppendCStr("\n primary type: ");
    sbuf.AppendString(mPrimaryDeviceType);
    sbuf.AppendCStr("\n secondary type: ");
    sbuf.AppendString(mSecondaryDeviceType);
    sbuf.AppendCStr("\n wps: ");
    sbuf.AppendInt32(mWpsConfigMethodsSupported);
    sbuf.AppendCStr("\n grpcapab: ");
    sbuf.AppendInt32(mGroupCapability);
    sbuf.AppendCStr("\n devcapab: ");
    sbuf.AppendInt32(mDeviceCapability);
    sbuf.AppendCStr("\n status: ");
    sbuf.AppendInt32(mStatus);
    sbuf.AppendCStr("\n wfdInfo:");
    String temp("NULL");
    if (mWfdInfo != NULL) {
        FAIL_RETURN(mWfdInfo->ToString(&temp));
    }
    sbuf.AppendString(temp);
    *str = sbuf.ToString();
    return NOERROR;
}

ECode CWifiP2pDevice::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);

    FAIL_RETURN(source->ReadString(&mDeviceName));
    FAIL_RETURN(source->ReadString(&mDeviceAddress));
    FAIL_RETURN(source->ReadString(&mPrimaryDeviceType));
    FAIL_RETURN(source->ReadString(&mSecondaryDeviceType));
    FAIL_RETURN(source->ReadInt32(&mWpsConfigMethodsSupported));
    FAIL_RETURN(source->ReadInt32(&mDeviceCapability));
    FAIL_RETURN(source->ReadInt32(&mGroupCapability));
    FAIL_RETURN(source->ReadInt32(&mStatus));
    Int32 temp;
    FAIL_RETURN(source->ReadInt32(&temp));
    mWfdInfo = NULL;
    if (temp == 1) {
        FAIL_RETURN(source->ReadInterfacePtr((Handle32*)&mWfdInfo));
    }

    return NOERROR;
}

ECode CWifiP2pDevice::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);

    FAIL_RETURN(dest->WriteString(mDeviceName));
    FAIL_RETURN(dest->WriteString(mDeviceAddress));
    FAIL_RETURN(dest->WriteString(mPrimaryDeviceType));
    FAIL_RETURN(dest->WriteString(mSecondaryDeviceType));
    FAIL_RETURN(dest->WriteInt32(mWpsConfigMethodsSupported));
    FAIL_RETURN(dest->WriteInt32(mDeviceCapability));
    FAIL_RETURN(dest->WriteInt32(mGroupCapability));
    FAIL_RETURN(dest->WriteInt32(mStatus));
    if (mWfdInfo != NULL) {
        FAIL_RETURN(dest->WriteInt32(1));
        FAIL_RETURN(dest->WriteInterfacePtr(mWfdInfo.Get()));
    } else {
        FAIL_RETURN(dest->WriteInt32(0));
    }

    return NOERROR;
}

ECode CWifiP2pDevice::constructor()
{
    return NOERROR;
}

ECode CWifiP2pDevice::constructor(
    /* [in] */ const String& dataString)
{
    if (DBG) Slogger::D("WifiP2pDevice", "new CWifiP2pDevice: [%s]", dataString.string());
    AutoPtr<IPatternHelper> helper;
    CPatternHelper::AcquireSingleton((IPatternHelper**)&helper);
    AutoPtr<IPattern> pattern;
    FAIL_RETURN(helper->Compile(String("[ \n]"), (IPattern**)&pattern));

    AutoPtr<ArrayOf<String> > tokens;
    FAIL_RETURN(pattern->Split(dataString, (ArrayOf<String>**)&tokens));

    AutoPtr<IMatcher> match;
    Boolean ret = FALSE;

    if ((tokens == NULL) || (tokens->GetLength() < 1)) {
        Slogger::E("WifiP2pDevice", "E_ILLEGAL_ARGUMENT_EXCEPTION:Malformed supplicant event %s", dataString.string());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (DBG) {
        Slogger::D("WifiP2pDevice", "new CWifiP2pDevice: tokens length: %d", tokens->GetLength());
        for (Int32 i = 0; i < tokens->GetLength(); ++i) {
            Slogger::D("WifiP2pDevice", " > token %d: [%s]", i, (*tokens)[i].string());
        }
    }

    switch (tokens->GetLength()) {
        case 1:
            /* Just a device address */
            mDeviceAddress = dataString;
            return NOERROR;
        case 2:
            FAIL_RETURN(mTwoTokenPattern->Matcher(dataString, (IMatcher**)&match));
            FAIL_RETURN(match->Find(&ret));
            if (!ret) {
                Slogger::E("WifiP2pDevice", "E_ILLEGAL_ARGUMENT_EXCEPTION:Malformed supplicant event %s", dataString.string());
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }

            FAIL_RETURN(match->Group(2, &mDeviceAddress));
            return NOERROR;
        case 3:
            FAIL_RETURN(mThreeTokenPattern->Matcher(dataString, (IMatcher**)&match));
            FAIL_RETURN(match->Find(&ret));
            if (!ret) {
                Slogger::E("WifiP2pDevice", "E_ILLEGAL_ARGUMENT_EXCEPTION:Malformed supplicant event %s", dataString.string());
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }

            FAIL_RETURN(match->Group(1, &mDeviceAddress));
            return NOERROR;
        case 4:
            FAIL_RETURN(mFourTokenPattern->Matcher(
                    dataString, (IMatcher**)&match));
            FAIL_RETURN(match->Find(&ret));
            if (!ret) {
                Slogger::E("WifiP2pDevice", "E_ILLEGAL_ARGUMENT_EXCEPTION:Malformed supplicant event %s", dataString.string());
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }

            FAIL_RETURN(match->Group(1, &mDeviceAddress));
            {
                String str;
                FAIL_RETURN(match->Group(2, &str));
                if (!str.IsNull()) {
                    String str1 = str.Substring(0, 4);
                    String str2 = str.Substring(4, 8);
                    String str3 = str.Substring(8, 12);
                    Int32 a = ParseHex(str1);
                    Int32 b = ParseHex(str2);
                    Int32 c = ParseHex(str3);
                    FAIL_RETURN(CWifiP2pWfdInfo::New(a, b, c, (IWifiP2pWfdInfo**)&mWfdInfo));
                }
            }
            return NOERROR;
        default:
            FAIL_RETURN(mDetailedDevicePattern->Matcher(dataString, (IMatcher**)&match));
            FAIL_RETURN(match->Find(&ret));
            if (!ret) {
                Slogger::E("WifiP2pDevice", "E_ILLEGAL_ARGUMENT_EXCEPTION:Malformed supplicant event %s", dataString.string());
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }

            FAIL_RETURN(match->Group(3, &mDeviceAddress));
            FAIL_RETURN(match->Group(4, &mPrimaryDeviceType));
            FAIL_RETURN(match->Group(5, &mDeviceName));

            String temp;
            FAIL_RETURN(match->Group(6, &temp));
            mWpsConfigMethodsSupported = ParseHex(temp);
            FAIL_RETURN(match->Group(7, &temp));
            mDeviceCapability = ParseHex(temp);
            FAIL_RETURN(match->Group(8, &temp));
            mGroupCapability = ParseHex(temp);

            FAIL_RETURN(match->Group(9, &temp));
            if (!temp.IsNull()) {
                String str;
                FAIL_RETURN(match->Group(10, &str));

                String str1 = str.Substring(0, 4);
                String str2 = str.Substring(4, 8);
                String str3 = str.Substring(8, 12);
                Int32 a = ParseHex(str1);
                Int32 b = ParseHex(str2);
                Int32 c = ParseHex(str3);
                FAIL_RETURN(CWifiP2pWfdInfo::New(a, b, c, (IWifiP2pWfdInfo**)&mWfdInfo));
            }
            break;
    }

    if ((*tokens)[0].StartWith("P2P-DEVICE-FOUND")) {
        mStatus = IWifiP2pDevice::AVAILABLE;
    }

    if (DBG) {
        String info;
        ToString(&info);
        Slogger::D("WifiP2pDevice", "new CWifiP2pDevice:\n %s", info.string());
    }
    return NOERROR;
}

ECode CWifiP2pDevice::constructor(
    /* [in] */ IWifiP2pDevice* source)
{
    if (source != NULL) {
        FAIL_RETURN(source->GetDeviceName(&mDeviceName));
        FAIL_RETURN(source->GetDeviceAddress(&mDeviceAddress));
        FAIL_RETURN(source->GetPrimaryDeviceType(&mPrimaryDeviceType));
        FAIL_RETURN(source->GetSecondaryDeviceType(&mSecondaryDeviceType));
        FAIL_RETURN(source->GetWpsConfigMethodsSupported(&mWpsConfigMethodsSupported));
        FAIL_RETURN(source->GetDeviceCapability(&mDeviceCapability));
        FAIL_RETURN(source->GetGroupCapability(&mGroupCapability));
        FAIL_RETURN(source->GetStatus(&mStatus));
        FAIL_RETURN(source->GetWfdInfo((IWifiP2pWfdInfo**)&mWfdInfo));

        if (DBG) {
            String info;
            ToString(&info);
            Slogger::D("WifiP2pDevice", "new CWifiP2pDevice copy from other device:\n %s", info.string());
        }
    }

    return NOERROR;
}

//supported formats: 0x1abc, 0X1abc, 1abc
Int32 CWifiP2pDevice::ParseHex(
    /* [in] */ const String& str)
{
    Int32 num = 0;
    String hexStr = str;
    if (hexStr.StartWith("0x") || hexStr.StartWith("0X")) {
        hexStr = str.Substring(2);
    }

    // try {
    ECode ec = StringUtils::ParseInt32(hexStr, 16, &num);
    if (FAILED(ec)) {
        Slogger::E("WifiP2pDevice", "Failed to parse hex string %s", str.string());
    }
    // } catch(NumberFormatException e) {
    //     Log.e(TAG, "Failed to parse hex string " + hexString);
    // }

    return num;
}

}
}
}
}
