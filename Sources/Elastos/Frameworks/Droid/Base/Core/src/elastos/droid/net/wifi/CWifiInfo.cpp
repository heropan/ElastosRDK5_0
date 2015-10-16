
#include "elastos/droid/net/wifi/CWifiInfo.h"
#include "elastos/droid/net/wifi/CWifiSsidHelper.h"
#include "elastos/droid/net/wifi/CWifiSsid.h"
#include "elastos/droid/net/NetworkUtils.h"
#include "elastos/droid/text/TextUtils.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuffer.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuffer;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Net::IInet6Address;
using Elastos::Net::IInetAddressHelper;
using Elastos::Net::CInetAddressHelper;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Net::NetworkUtils;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

String CWifiInfo::TAG("WifiInfo");
HashMap<SupplicantState, NetworkInfoDetailedState> CWifiInfo::mStateMap;
String CWifiInfo::LINK_SPEED_UNITS = String("Mbps");

Int32 CWifiInfo::mInitFlag = InternalInit();

CWifiInfo::CWifiInfo()
    : mNetworkId(0)
    , mHiddenSSID(FALSE)
    , mRssi(0)
    , mLinkSpeed(0)
    , mMeteredHint(FALSE)
{}

ECode CWifiInfo::constructor()
{
    mWifiSsid = NULL;
    mBSSID = NULL;
    mNetworkId = -1;
    mSupplicantState = SupplicantState_UNINITIALIZED;
    mRssi = -9999;
    mLinkSpeed = -1;
    mHiddenSSID = FALSE;
    return NOERROR;
}

ECode CWifiInfo::constructor(
    /* [in] */ Boolean fake)
{
    mNetworkId = (-1);
    mHiddenSSID = (FALSE);
    mRssi = (-9999);
    mLinkSpeed = (-1);
    mMeteredHint = (FALSE);

    AutoPtr<IWifiSsidHelper> helper;
    CWifiSsidHelper::AcquireSingleton((IWifiSsidHelper**)&helper);
    helper->CreateFromAsciiEncoded(String(""), (IWifiSsid**)&mWifiSsid);
    mBSSID = String("");
    mSupplicantState = SupplicantState_UNINITIALIZED;
    return NOERROR;
}

ECode CWifiInfo::constructor(
    /* [in] */ IWifiInfo* source)
{
    if (source != NULL) {
        source->GetSupplicantState(&mSupplicantState);
        source->GetBSSID(&mBSSID);
        source->GetWifiSsid((IWifiSsid**)&mWifiSsid);
        source->GetNetworkId(&mNetworkId);
        source->GetHiddenSSID(&mHiddenSSID);
        source->GetRssi(&mRssi);
        source->GetLinkSpeed(&mLinkSpeed);
        Int32 ipAddres;
        source->GetIpAddress(&ipAddres);
        if (ipAddres != 0) {
            NetworkUtils::Int32ToInetAddress(ipAddres, (IInetAddress**)&mIpAddress);
        }
        source->GetMacAddress(&mMacAddress);
        source->GetMeteredHint(&mMeteredHint);
    }
    return NOERROR;
}

ECode CWifiInfo::SetSSID(
    /* [in] */ IWifiSsid* wifiSsid)
{
    mWifiSsid = wifiSsid;
    // network is considered not hidden by default
    mHiddenSSID = FALSE;
    return NOERROR;
}

ECode CWifiInfo::GetSSID(
    /* [out] */ String* ssid)
{
    VALIDATE_NOT_NULL(ssid);

    if (mWifiSsid != NULL) {
        String unicode;
        mWifiSsid->ToString(&unicode);

        AutoPtr<ICharSequence> tmp;
        CString::New(unicode, (ICharSequence**)&tmp);
        if (!TextUtils::IsEmpty(tmp)) {
            *ssid = String("\"") + unicode + String("\"");
            return NOERROR;
        } else {
            return mWifiSsid->GetHexString(ssid);
        }
    }

    *ssid = IWifiSsid::NONE;
    return NOERROR;
}

ECode CWifiInfo::GetWifiSsid(
    /* [in] */ IWifiSsid** ssid)
{
    VALIDATE_NOT_NULL(ssid);
    *ssid = mWifiSsid;
    REFCOUNT_ADD(*ssid);
    return NOERROR;
}

ECode CWifiInfo::SetBSSID(
    /* [in] */ const String& BSSID)
{
    mBSSID = BSSID;
    return NOERROR;
}

ECode CWifiInfo::GetBSSID(
    /* [out] */ String* bssid)
{
    VALIDATE_NOT_NULL(bssid);
    *bssid = mBSSID;
    return NOERROR;
}

ECode CWifiInfo::GetRssi(
    /* [out] */ Int32* rssi)
{
    VALIDATE_NOT_NULL(rssi);
    *rssi = mRssi;
    return NOERROR;
}

ECode CWifiInfo::SetRssi(
    /* [in] */ Int32 rssi)
{
    mRssi = rssi;
    return NOERROR;
}

ECode CWifiInfo::GetLinkSpeed(
    /* [out] */ Int32* speed)
{
    VALIDATE_NOT_NULL(speed);
    *speed = mLinkSpeed;
    return NOERROR;
}

ECode CWifiInfo::SetLinkSpeed(
    /* [in] */ Int32 linkSpeed)
{
    mLinkSpeed = linkSpeed;
    return NOERROR;
}

ECode CWifiInfo::SetMacAddress(
    /* [in] */ const String& macAddress)
{
    mMacAddress = macAddress;
    return NOERROR;
}

ECode CWifiInfo::GetMacAddress(
    /* [in] */ String* address)
{
    VALIDATE_NOT_NULL(address);
    *address = mMacAddress;
    return NOERROR;
}

ECode CWifiInfo::SetMeteredHint(
    /* [in] */ Boolean meteredHint)
{
    mMeteredHint = meteredHint;
    return NOERROR;
}

ECode CWifiInfo::GetMeteredHint(
    /* [out] */ Boolean* hint)
{
    VALIDATE_NOT_NULL(hint);
    *hint = mMeteredHint;
    return NOERROR;
}

ECode CWifiInfo::SetNetworkId(
    /* [in] */ Int32 id)
{
    mNetworkId = id;
    return NOERROR;
}

ECode CWifiInfo::GetNetworkId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mNetworkId;
    return NOERROR;
}

ECode CWifiInfo::GetSupplicantState(
    /* [out] */ SupplicantState* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mSupplicantState;
    return NOERROR;
}

ECode CWifiInfo::SetSupplicantState(
    /* [in] */ SupplicantState state)
{
    mSupplicantState = state;
    return NOERROR;
}

ECode CWifiInfo::SetInetAddress(
    /* [in] */ IInetAddress* address)
{
    mIpAddress = address;
    return NOERROR;
}

ECode CWifiInfo::GetIpAddress(
    /* [out] */ Int32* address)
{
    VALIDATE_NOT_NULL(address);

    if (mIpAddress == NULL || IInet6Address::Probe(mIpAddress) != NULL) {
        *address = 0;
        return NOERROR;
    }

    return NetworkUtils::InetAddressToInt32(mIpAddress, address);
}

ECode CWifiInfo::GetHiddenSSID(
    /* [out] */ Boolean* ssid)
{
    VALIDATE_NOT_NULL(ssid);
    *ssid = mHiddenSSID;
    return NOERROR;
}

ECode CWifiInfo::SetHiddenSSID(
    /* [in] */ Boolean hiddenSSID)
{
    mHiddenSSID = hiddenSSID;
    return NOERROR;
}

NetworkInfoDetailedState CWifiInfo::GetDetailedStateOf(
    /* [in] */ SupplicantState suppState)
{
    return mStateMap[suppState];
}

void CWifiInfo::SetSupplicantState(
    /* [in] */ const String& stateName)
{
    mSupplicantState = ValueOf(stateName);
}

SupplicantState CWifiInfo::ValueOf(
    /* [in] */ const String& stateName)
{
    if (String("4WAY_HANDSHAKE").EqualsIgnoreCase(stateName) == 0)
        return SupplicantState_FOUR_WAY_HANDSHAKE;
    else {
        // try {
        /* SupplicantState.valueOf(stateName.ToUpperCase())*/
        String tmp = stateName.ToUpperCase();
        return StringUtils::ParseInt32(tmp);
        // } catch (IllegalArgumentException e) {
        //     return SupplicantState.INVALID;
        // }
    }
}

ECode CWifiInfo::RemoveDoubleQuotes(
    /* [in] */ const String& str,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);

    if (str.IsNull()) return NOERROR;
    AutoPtr<ArrayOf<Char32> > charArray = str.GetChars();
    const Int32 length = charArray->GetLength();
    if ((length > 1) && ((*charArray)[0] == '"') && ((*charArray)[length - 1] == '"')) {
        *value = str.Substring(1, length - 1);
        return NOERROR;
    }
    *value = str;
    return NOERROR;
}

// @Override
ECode CWifiInfo::ToString(
    /* [out] */ String* str)
{
    StringBuffer sb;
    String none("<none>");

    sb.Append("SSID: ");
    String sidStr;
    mWifiSsid->ToString(&sidStr);
    sb.Append(mWifiSsid == NULL ? IWifiSsid::NONE : sidStr);
    sb.Append(", BSSID: ");
    sb.Append(mBSSID.IsNull() ? none : mBSSID);
    sb.Append(", MAC: ");
    sb.Append(mMacAddress.IsNull() ? none : mMacAddress);
    sb.Append(", Supplicant state: ");
    if (mSupplicantState == NULL) sb.Append(none);
    sb.Append(mSupplicantState);
    sb.Append(", RSSI: ");
    sb.Append(mRssi);
    sb.Append(", Link speed: ");
    sb.Append(mLinkSpeed);
    sb.Append(", Net ID: ");
    sb.Append(mNetworkId);
    sb.Append(", Metered hint: ");
    sb.AppendBoolean(mMeteredHint);

    *str = sb.ToString();
    return NOERROR;
}

/** Implement the Parcelable interface {@hide} */
Int32 CWifiInfo::DescribeContents()
{
    return 0;
}

/** Implement the Parcelable interface {@hide} */
ECode CWifiInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mNetworkId);
    dest->WriteInt32(mRssi);
    dest->WriteInt32(mLinkSpeed);
    if (mIpAddress != NULL) {
        dest->WriteByte((byte)1);
        AutoPtr<ArrayOf<Byte> > address;
        mIpAddress->GetAddress((ArrayOf<Byte>**)&address);
        dest->WriteArrayOf((Handle32)address.Get());
    }
    else {
        dest->WriteByte((byte)0);
    }

    if (mWifiSsid != NULL) {
        dest->WriteInt32(1);
        IParcelable::Probe(mWifiSsid.Get())->WriteToParcel(dest);
    }
    else {
        dest->WriteInt32(0);
    }

    dest->WriteString(mBSSID);
    dest->WriteString(mMacAddress);
    dest->WriteBoolean(mMeteredHint);
    dest->WriteInt32(mSupplicantState);
    return NOERROR;
}

ECode CWifiInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt32(&mNetworkId);
    source->ReadInt32(&mRssi);
    source->ReadInt32(&mLinkSpeed);
    Byte b;
    source->ReadByte(&b);
    if (b == (byte)1) {
        AutoPtr<ArrayOf<Byte> > address;
        source->ReadArrayOf((Handle32*)&address);

        AutoPtr<IInetAddressHelper> helpler;
        CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&helpler);
        helpler->GetByAddress(address, (IInetAddress**)&mIpAddress);
    }
    Int32 value;
    source->ReadInt32(&value);
    mWifiSsid = NULL;
    if (value == 1) {
        CWifiSsid::New((IWifiSsid**)&mWifiSsid);
        IParcelable::Probe(mWifiSsid.Get())->ReadFromParcel(source);
    }
    source->ReadString(&mBSSID);
    source->ReadString(&mMacAddress);
    source->ReadBoolean(&mMeteredHint);
    source->ReadInt32(&mSupplicantState);

    return NOERROR;
}

Int32 CWifiInfo::InternalInit()
{
    mStateMap[SupplicantState_DISCONNECTED] = NetworkInfoDetailedState_DISCONNECTED;
    mStateMap[SupplicantState_INTERFACE_DISABLED] = NetworkInfoDetailedState_DISCONNECTED;
    mStateMap[SupplicantState_INACTIVE] = NetworkInfoDetailedState_IDLE;
    mStateMap[SupplicantState_SCANNING] = NetworkInfoDetailedState_SCANNING;
    mStateMap[SupplicantState_AUTHENTICATING] = NetworkInfoDetailedState_CONNECTING;
    mStateMap[SupplicantState_ASSOCIATING] = NetworkInfoDetailedState_CONNECTING;
    mStateMap[SupplicantState_ASSOCIATED] = NetworkInfoDetailedState_CONNECTING;
    mStateMap[SupplicantState_FOUR_WAY_HANDSHAKE] = NetworkInfoDetailedState_AUTHENTICATING;
    mStateMap[SupplicantState_GROUP_HANDSHAKE] = NetworkInfoDetailedState_AUTHENTICATING;
    mStateMap[SupplicantState_COMPLETED] = NetworkInfoDetailedState_OBTAINING_IPADDR;
    mStateMap[SupplicantState_DORMANT] = NetworkInfoDetailedState_DISCONNECTED;
    mStateMap[SupplicantState_UNINITIALIZED] = NetworkInfoDetailedState_IDLE;
    mStateMap[SupplicantState_INVALID] = NetworkInfoDetailedState_FAILED;
    return 0;
}

} // namespace Wifi
}
} // namespace Droid
} // namespace Elastos
