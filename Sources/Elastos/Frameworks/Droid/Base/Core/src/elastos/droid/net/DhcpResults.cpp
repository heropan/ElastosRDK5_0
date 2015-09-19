// DhcpResults.cpp
#include "DhcpResults.h"

namespace Elastos {
namespace Droid {
namespace Net {

#if 0 // // TODO: Waiting for StaticIpConfiguration.h
CAR_INTERFACE_IMPL(DhcpResults, StaticIpConfiguration, IDhcpResults)
#else
CAR_INTERFACE_IMPL(DhcpResults, Object, IDhcpResults)
#endif

const String DhcpResults::TAG = String("DhcpResults");

ECode DhcpResults::GetServerAddress(
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = mServerAddress;
    return NOERROR;
}

ECode DhcpResults::SetServerAddress(
    /* [in] */ IInetAddress* serverAddress)
{
    VALIDATE_NOT_NULL(serverAddress)

    mServerAddress = serverAddress;
    return NOERROR;
}

ECode DhcpResults::GetVendorInfo(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mVendorInfo;
    return NOERROR;
}

ECode DhcpResults::GetLeaseDuration(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mLeaseDuration;
    return NOERROR;
}

ECode DhcpResults::constructor()
{
#if 0 // // TODO: Waiting for StaticIpConfiguration.h
    StaticIpConfiguration::constructor();
#else
    assert(0);
#endif
    return NOERROR;
}

ECode DhcpResults::constructor(
    /* [in] */ IStaticIpConfiguration* source)
{
#if 0 // // TODO: Waiting for StaticIpConfiguration.h
    StaticIpConfiguration::constructor(source);
#else
    assert(0);
#endif
    return NOERROR;
}

ECode DhcpResults::constructor(
    /* [in] */ IDhcpResults* source)
{
#if 0 // // TODO: Waiting for StaticIpConfiguration.h
    StaticIpConfiguration::constructor(source);
#else
    assert(0);
#endif
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (source != null) {
            // All these are immutable, so no need to make copies.
            serverAddress = source.serverAddress;
            vendorInfo = source.vendorInfo;
            leaseDuration = source.leaseDuration;
        }
#endif
    return NOERROR;
}

ECode DhcpResults::UpdateFromDhcpRequest(
    /* [in] */ IDhcpResults* orig)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (orig == null) return;
        if (gateway == null) gateway = orig.gateway;
        if (dnsServers.size() == 0) {
            dnsServers.addAll(orig.dnsServers);
        }
#endif
}

ECode DhcpResults::HasMeteredHint(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (vendorInfo != null) {
            return vendorInfo.contains("ANDROID_METERED");
        } else {
            return false;
        }
#endif
}

ECode DhcpResults::Clear()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        super.clear();
        vendorInfo = null;
        leaseDuration = 0;
#endif
}

ECode DhcpResults::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        StringBuffer str = new StringBuffer(super.toString());

        str.append(" DHCP server ").append(serverAddress);
        str.append(" Vendor info ").append(vendorInfo);
        str.append(" lease ").append(leaseDuration).append(" seconds");

        return str.toString();
#endif
}

ECode DhcpResults::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (this == obj) return true;

        if (!(obj instanceof DhcpResults)) return false;

        DhcpResults target = (DhcpResults)obj;

        return super.equals((StaticIpConfiguration) obj) &&
                Objects.equals(serverAddress, target.serverAddress) &&
                Objects.equals(vendorInfo, target.vendorInfo) &&
                leaseDuration == target.leaseDuration;
#endif
}

ECode DhcpResults::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        super.writeToParcel(dest, flags);
        dest.writeInt(leaseDuration);
        NetworkUtils.parcelInetAddress(dest, serverAddress, flags);
        dest.writeString(vendorInfo);
#endif
}

ECode DhcpResults::ReadFromParcel(
    /* [in] */ IDhcpResults* dhcpResults,
    /* [in] */ IParcel* in)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        StaticIpConfiguration.readFromParcel(dhcpResults, in);
        dhcpResults.leaseDuration = in.readInt();
        dhcpResults.serverAddress = NetworkUtils.unparcelInetAddress(in);
        dhcpResults.vendorInfo = in.readString();
#endif
}

ECode DhcpResults::SetIpAddress(
    /* [in] */ const String& addrString,
    /* [in] */ Int32 prefixLength,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            Inet4Address addr = (Inet4Address) NetworkUtils.numericToInetAddress(addrString);
            ipAddress = new LinkAddress(addr, prefixLength);
        } catch (IllegalArgumentException|ClassCastException e) {
            Log.e(TAG, "setIpAddress failed with addrString " + addrString + "/" + prefixLength);
            return true;
        }
        return false;
#endif
}

ECode DhcpResults::SetGateway(
    /* [in] */ const String& addrString,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            gateway = NetworkUtils.numericToInetAddress(addrString);
        } catch (IllegalArgumentException e) {
            Log.e(TAG, "setGateway failed with addrString " + addrString);
            return true;
        }
        return false;
#endif
}

ECode DhcpResults::AddDns(
    /* [in] */ const String& addrString,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (TextUtils.isEmpty(addrString) == false) {
            try {
                dnsServers.add(NetworkUtils.numericToInetAddress(addrString));
            } catch (IllegalArgumentException e) {
                Log.e(TAG, "addDns failed with addrString " + addrString);
                return true;
            }
        }
        return false;
#endif
}

ECode DhcpResults::SetServerAddress(
    /* [in] */ const String& addrString,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            serverAddress = NetworkUtils.numericToInetAddress(addrString);
        } catch (IllegalArgumentException e) {
            Log.e(TAG, "setServerAddress failed with addrString " + addrString);
            return true;
        }
        return false;
#endif
}

ECode DhcpResults::SetLeaseDuration(
    /* [in] */ Int32 duration)
{
    mLeaseDuration = duration;
    return NOERROR;
}

ECode DhcpResults::SetVendorInfo(
    /* [in] */ const String& info)
{
    mVendorInfo = info;
    return NOERROR;
}

ECode DhcpResults::SetDomains(
    /* [in] */ const String& domains)
{
#if 0 // TODO: Waiting for c++ class StaticIpConfiguration
    this->mDomains = domains;
#else
    assert(0);
#endif
    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos