
#include "elastos/droid/net/NetworkCapabilities.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(NetworkCapabilities, Object, IParcelable, INetworkCapabilities)

const String NetworkCapabilities::TAG = String("NetworkCapabilities");
const Boolean NetworkCapabilities::DBG = false;
const Int32 NetworkCapabilities::MIN_NET_CAPABILITY = INetworkCapabilities::NET_CAPABILITY_MMS;
const Int32 NetworkCapabilities::MAX_NET_CAPABILITY = INetworkCapabilities::NET_CAPABILITY_NOT_VPN;
const Int32 NetworkCapabilities::MIN_TRANSPORT = INetworkCapabilities::TRANSPORT_CELLULAR;
const Int32 NetworkCapabilities::MAX_TRANSPORT = INetworkCapabilities::TRANSPORT_VPN;

NetworkCapabilities::NetworkCapabilities()
    : mNetworkCapabilities((1 << INetworkCapabilities::NET_CAPABILITY_NOT_RESTRICTED) | (1 << INetworkCapabilities::NET_CAPABILITY_TRUSTED) | (1 << INetworkCapabilities::NET_CAPABILITY_NOT_VPN))
{}

ECode NetworkCapabilities::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below

#endif
}

ECode NetworkCapabilities::constructor(
    /* [in] */ INetworkCapabilities* nc)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (nc != null) {
            mNetworkCapabilities = nc.mNetworkCapabilities;
            mTransportTypes = nc.mTransportTypes;
            mLinkUpBandwidthKbps = nc.mLinkUpBandwidthKbps;
            mLinkDownBandwidthKbps = nc.mLinkDownBandwidthKbps;
            mNetworkSpecifier = nc.mNetworkSpecifier;
        }

#endif
}

ECode NetworkCapabilities::AddCapability(
    /* [in] */ Int32 capability,
    /* [out] */ INetworkCapabilities** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (capability < MIN_NET_CAPABILITY || capability > MAX_NET_CAPABILITY) {
            throw new IllegalArgumentException("NetworkCapability out of range");
        }
        mNetworkCapabilities |= 1 << capability;
        return this;

#endif
}

ECode NetworkCapabilities::RemoveCapability(
    /* [in] */ Int32 capability,
    /* [out] */ INetworkCapabilities** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (capability < MIN_NET_CAPABILITY || capability > MAX_NET_CAPABILITY) {
            throw new IllegalArgumentException("NetworkCapability out of range");
        }
        mNetworkCapabilities &= ~(1 << capability);
        return this;

#endif
}

ECode NetworkCapabilities::GetCapabilities(
    /* [out, callee] */ ArrayOf<Int32>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return enumerateBits(mNetworkCapabilities);

#endif
}

ECode NetworkCapabilities::HasCapability(
    /* [in] */ Int32 capability,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (capability < MIN_NET_CAPABILITY || capability > MAX_NET_CAPABILITY) {
            return false;
        }
        return ((mNetworkCapabilities & (1 << capability)) != 0);

#endif
}

ECode NetworkCapabilities::EnumerateBits(
    /* [in] */ Int64 val,
    /* [out, callee] */ ArrayOf<Int32>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        int size = Long.bitCount(val);
        int[] result = new int[size];
        int index = 0;
        int resource = 0;
        while (val > 0) {
            if ((val & 1) == 1) result[index++] = resource;
            val = val >> 1;
            resource++;
        }
        return result;

#endif
}

ECode NetworkCapabilities::CombineNetCapabilities(
    /* [in] */ INetworkCapabilities* nc)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.mNetworkCapabilities |= nc.mNetworkCapabilities;

#endif
}

ECode NetworkCapabilities::SatisfiedByNetCapabilities(
    /* [in] */ INetworkCapabilities* nc,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return ((nc.mNetworkCapabilities & this.mNetworkCapabilities) == this.mNetworkCapabilities);

#endif
}

ECode NetworkCapabilities::EqualsNetCapabilities(
    /* [in] */ INetworkCapabilities* nc,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return (nc.mNetworkCapabilities == this.mNetworkCapabilities);

#endif
}

ECode NetworkCapabilities::AddTransportType(
    /* [in] */ Int32 transportType,
    /* [out] */ INetworkCapabilities** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (transportType < MIN_TRANSPORT || transportType > MAX_TRANSPORT) {
            throw new IllegalArgumentException("TransportType out of range");
        }
        mTransportTypes |= 1 << transportType;
        setNetworkSpecifier(mNetworkSpecifier); // used for exception checking
        return this;

#endif
}

ECode NetworkCapabilities::RemoveTransportType(
    /* [in] */ Int32 transportType,
    /* [out] */ INetworkCapabilities** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (transportType < MIN_TRANSPORT || transportType > MAX_TRANSPORT) {
            throw new IllegalArgumentException("TransportType out of range");
        }
        mTransportTypes &= ~(1 << transportType);
        setNetworkSpecifier(mNetworkSpecifier); // used for exception checking
        return this;

#endif
}

ECode NetworkCapabilities::GetTransportTypes(
    /* [out, callee] */ ArrayOf<Int32>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return enumerateBits(mTransportTypes);

#endif
}

ECode NetworkCapabilities::HasTransport(
    /* [in] */ Int32 transportType,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (transportType < MIN_TRANSPORT || transportType > MAX_TRANSPORT) {
            return false;
        }
        return ((mTransportTypes & (1 << transportType)) != 0);

#endif
}

ECode NetworkCapabilities::CombineTransportTypes(
    /* [in] */ INetworkCapabilities* nc)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.mTransportTypes |= nc.mTransportTypes;

#endif
}

ECode NetworkCapabilities::SatisfiedByTransportTypes(
    /* [in] */ INetworkCapabilities* nc,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return ((this.mTransportTypes == 0) ||
                ((this.mTransportTypes & nc.mTransportTypes) != 0));

#endif
}

ECode NetworkCapabilities::EqualsTransportTypes(
    /* [in] */ INetworkCapabilities* nc,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return (nc.mTransportTypes == this.mTransportTypes);

#endif
}

ECode NetworkCapabilities::SetLinkUpstreamBandwidthKbps(
    /* [in] */ Int32 upKbps)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mLinkUpBandwidthKbps = upKbps;

#endif
}

ECode NetworkCapabilities::GetLinkUpstreamBandwidthKbps(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mLinkUpBandwidthKbps;

#endif
}

ECode NetworkCapabilities::SetLinkDownstreamBandwidthKbps(
    /* [in] */ Int32 downKbps)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mLinkDownBandwidthKbps = downKbps;

#endif
}

ECode NetworkCapabilities::GetLinkDownstreamBandwidthKbps(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mLinkDownBandwidthKbps;

#endif
}

ECode NetworkCapabilities::CombineLinkBandwidths(
    /* [in] */ INetworkCapabilities* nc)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.mLinkUpBandwidthKbps =
                Math.max(this.mLinkUpBandwidthKbps, nc.mLinkUpBandwidthKbps);
        this.mLinkDownBandwidthKbps =
                Math.max(this.mLinkDownBandwidthKbps, nc.mLinkDownBandwidthKbps);

#endif
}

ECode NetworkCapabilities::SatisfiedByLinkBandwidths(
    /* [in] */ INetworkCapabilities* nc,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return !(this.mLinkUpBandwidthKbps > nc.mLinkUpBandwidthKbps ||
                this.mLinkDownBandwidthKbps > nc.mLinkDownBandwidthKbps);

#endif
}

ECode NetworkCapabilities::EqualsLinkBandwidths(
    /* [in] */ INetworkCapabilities* nc,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return (this.mLinkUpBandwidthKbps == nc.mLinkUpBandwidthKbps &&
                this.mLinkDownBandwidthKbps == nc.mLinkDownBandwidthKbps);

#endif
}

ECode NetworkCapabilities::SetNetworkSpecifier(
    /* [in] */ const String& networkSpecifier)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (TextUtils.isEmpty(networkSpecifier) == false && Long.bitCount(mTransportTypes) != 1) {
            throw new IllegalStateException("Must have a single transport specified to use " +
                    "setNetworkSpecifier");
        }
        mNetworkSpecifier = networkSpecifier;

#endif
}

ECode NetworkCapabilities::GetNetworkSpecifier(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mNetworkSpecifier;

#endif
}

ECode NetworkCapabilities::CombineSpecifiers(
    /* [in] */ INetworkCapabilities* nc)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String otherSpecifier = nc.getNetworkSpecifier();
        if (TextUtils.isEmpty(otherSpecifier)) return;
        if (TextUtils.isEmpty(mNetworkSpecifier) == false) {
            throw new IllegalStateException("Can't combine two networkSpecifiers");
        }
        setNetworkSpecifier(otherSpecifier);

#endif
}

ECode NetworkCapabilities::SatisfiedBySpecifier(
    /* [in] */ INetworkCapabilities* nc,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return (TextUtils.isEmpty(mNetworkSpecifier) ||
                mNetworkSpecifier.equals(nc.mNetworkSpecifier));

#endif
}

ECode NetworkCapabilities::EqualsSpecifier(
    /* [in] */ INetworkCapabilities* nc,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (TextUtils.isEmpty(mNetworkSpecifier)) {
            return TextUtils.isEmpty(nc.mNetworkSpecifier);
        } else {
            return mNetworkSpecifier.equals(nc.mNetworkSpecifier);
        }

#endif
}

ECode NetworkCapabilities::CombineCapabilities(
    /* [in] */ INetworkCapabilities* nc)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        combineNetCapabilities(nc);
        combineTransportTypes(nc);
        combineLinkBandwidths(nc);
        combineSpecifiers(nc);

#endif
}

ECode NetworkCapabilities::SatisfiedByNetworkCapabilities(
    /* [in] */ INetworkCapabilities* nc,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return (nc != null &&
                satisfiedByNetCapabilities(nc) &&
                satisfiedByTransportTypes(nc) &&
                satisfiedByLinkBandwidths(nc) &&
                satisfiedBySpecifier(nc));

#endif
}

ECode NetworkCapabilities::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (obj == null || (obj instanceof NetworkCapabilities == false)) return false;
        NetworkCapabilities that = (NetworkCapabilities)obj;
        return (equalsNetCapabilities(that) &&
                equalsTransportTypes(that) &&
                equalsLinkBandwidths(that) &&
                equalsSpecifier(that));

#endif
}

ECode NetworkCapabilities::HashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return ((int)(mNetworkCapabilities & 0xFFFFFFFF) +
                ((int)(mNetworkCapabilities >> 32) * 3) +
                ((int)(mTransportTypes & 0xFFFFFFFF) * 5) +
                ((int)(mTransportTypes >> 32) * 7) +
                (mLinkUpBandwidthKbps * 11) +
                (mLinkDownBandwidthKbps * 13) +
                (TextUtils.isEmpty(mNetworkSpecifier) ? 0 : mNetworkSpecifier.hashCode() * 17));

#endif
}

ECode NetworkCapabilities::DescribeContents(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return 0;

#endif
}

ECode NetworkCapabilities::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        dest.writeLong(mNetworkCapabilities);
        dest.writeLong(mTransportTypes);
        dest.writeInt(mLinkUpBandwidthKbps);
        dest.writeInt(mLinkDownBandwidthKbps);
        dest.writeString(mNetworkSpecifier);

#endif
}

ECode NetworkCapabilities::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public NetworkCapabilities createFromParcel(Parcel in) {
                NetworkCapabilities netCap = new NetworkCapabilities();
                netCap.mNetworkCapabilities = in.readLong();
                netCap.mTransportTypes = in.readLong();
                netCap.mLinkUpBandwidthKbps = in.readInt();
                netCap.mLinkDownBandwidthKbps = in.readInt();
                netCap.mNetworkSpecifier = in.readString();
                return netCap;
            }
            public NetworkCapabilities[] newArray(int size) {
                return new NetworkCapabilities[size];
            }
#endif
}

ECode NetworkCapabilities::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public NetworkCapabilities createFromParcel(Parcel in) {
                NetworkCapabilities netCap = new NetworkCapabilities();
                netCap.mNetworkCapabilities = in.readLong();
                netCap.mTransportTypes = in.readLong();
                netCap.mLinkUpBandwidthKbps = in.readInt();
                netCap.mLinkDownBandwidthKbps = in.readInt();
                netCap.mNetworkSpecifier = in.readString();
                return netCap;
            }
            public NetworkCapabilities[] newArray(int size) {
                return new NetworkCapabilities[size];
            }
#endif
}

ECode NetworkCapabilities::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        int[] types = getTransportTypes();
        String transports = (types.length > 0 ? " Transports: " : "");
        for (int i = 0; i < types.length;) {
            switch (types[i]) {
                case TRANSPORT_CELLULAR:    transports += "CELLULAR"; break;
                case TRANSPORT_WIFI:        transports += "WIFI"; break;
                case TRANSPORT_BLUETOOTH:   transports += "BLUETOOTH"; break;
                case TRANSPORT_ETHERNET:    transports += "ETHERNET"; break;
                case TRANSPORT_VPN:         transports += "VPN"; break;
            }
            if (++i < types.length) transports += "|";
        }
        types = getCapabilities();
        String capabilities = (types.length > 0 ? " Capabilities: " : "");
        for (int i = 0; i < types.length; ) {
            switch (types[i]) {
                case NET_CAPABILITY_MMS:            capabilities += "MMS"; break;
                case NET_CAPABILITY_SUPL:           capabilities += "SUPL"; break;
                case NET_CAPABILITY_DUN:            capabilities += "DUN"; break;
                case NET_CAPABILITY_FOTA:           capabilities += "FOTA"; break;
                case NET_CAPABILITY_IMS:            capabilities += "IMS"; break;
                case NET_CAPABILITY_CBS:            capabilities += "CBS"; break;
                case NET_CAPABILITY_WIFI_P2P:       capabilities += "WIFI_P2P"; break;
                case NET_CAPABILITY_IA:             capabilities += "IA"; break;
                case NET_CAPABILITY_RCS:            capabilities += "RCS"; break;
                case NET_CAPABILITY_XCAP:           capabilities += "XCAP"; break;
                case NET_CAPABILITY_EIMS:           capabilities += "EIMS"; break;
                case NET_CAPABILITY_NOT_METERED:    capabilities += "NOT_METERED"; break;
                case NET_CAPABILITY_INTERNET:       capabilities += "INTERNET"; break;
                case NET_CAPABILITY_NOT_RESTRICTED: capabilities += "NOT_RESTRICTED"; break;
                case NET_CAPABILITY_TRUSTED:        capabilities += "TRUSTED"; break;
                case NET_CAPABILITY_NOT_VPN:        capabilities += "NOT_VPN"; break;
            }
            if (++i < types.length) capabilities += "&";
        }
        String upBand = ((mLinkUpBandwidthKbps > 0) ? " LinkUpBandwidth>=" +
                mLinkUpBandwidthKbps + "Kbps" : "");
        String dnBand = ((mLinkDownBandwidthKbps > 0) ? " LinkDnBandwidth>=" +
                mLinkDownBandwidthKbps + "Kbps" : "");
        String specifier = (mNetworkSpecifier == null ?
                "" : " Specifier: <" + mNetworkSpecifier + ">");
        return "[" + transports + capabilities + upBand + dnBand + specifier + "]";

#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
