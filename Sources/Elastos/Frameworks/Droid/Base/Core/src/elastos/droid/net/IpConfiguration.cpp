
#include "elastos/droid/net/IpConfiguration.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(IpConfiguration, Object, IIpConfiguration, IParcelable)

const String IpConfiguration::sTAG = String("IpConfiguration");

ECode IpConfiguration::Init(
    /* [in] */ IpConfigurationIpAssignment ipAssignment,
    /* [in] */ IpConfigurationProxySettings proxySettings,
    /* [in] */ IStaticIpConfiguration* staticIpConfiguration,
    /* [in] */ IProxyInfo* httpProxy)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.ipAssignment = ipAssignment;
        this.proxySettings = proxySettings;
        this.staticIpConfiguration = (staticIpConfiguration == null) ?
                null : new StaticIpConfiguration(staticIpConfiguration);
        this.httpProxy = (httpProxy == null) ?
                null : new ProxyInfo(httpProxy);

#endif
}

ECode IpConfiguration::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        init(IpAssignment.UNASSIGNED, ProxySettings.UNASSIGNED, null, null);

#endif
}

ECode IpConfiguration::constructor(
    /* [in] */ IpConfigurationIpAssignment ipAssignment,
    /* [in] */ IpConfigurationProxySettings proxySettings,
    /* [in] */ IStaticIpConfiguration* staticIpConfiguration,
    /* [in] */ IProxyInfo* httpProxy)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        init(ipAssignment, proxySettings, staticIpConfiguration, httpProxy);

#endif
}

ECode IpConfiguration::constructor(
    /* [in] */ IIpConfiguration* source)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this();
        if (source != null) {
            init(source.ipAssignment, source.proxySettings,
                 source.staticIpConfiguration, source.httpProxy);
        }

#endif
}

ECode IpConfiguration::GetIpAssignment(
    /* [out] */ IpConfigurationIpAssignment* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mIpAssignment;

    return NOERROR;
}

ECode IpConfiguration::SetIpAssignment(
    /* [in] */ IpConfigurationIpAssignment ipAssignment)
{
    mIpAssignment = ipAssignment;
    return NOERROR;
}

ECode IpConfiguration::GetStaticIpConfiguration(
    /* [out] */ IStaticIpConfiguration** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = mStaticIpConfiguration;

    return NOERROR;
}

ECode IpConfiguration::SetStaticIpConfiguration(
    /* [in] */ IStaticIpConfiguration* staticIpConfiguration)
{
    VALIDATE_NOT_NULL(staticIpConfiguration)

    mStaticIpConfiguration = staticIpConfiguration;
    return NOERROR;
}

ECode IpConfiguration::GetProxySettings(
    /* [out] */ IpConfigurationProxySettings* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mProxySettings;

    return NOERROR;
}

ECode IpConfiguration::SetProxySettings(
    /* [in] */ IpConfigurationProxySettings proxySettings)
{
    mProxySettings = proxySettings;
    return NOERROR;
}

ECode IpConfiguration::GetHttpProxy(
    /* [out] */ IProxyInfo** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = mHttpProxy;
    return NOERROR;
}

ECode IpConfiguration::SetHttpProxy(
    /* [in] */ IProxyInfo* httpProxy)
{
    VALIDATE_NOT_NULL(httpProxy)

    mHttpProxy = httpProxy;
    return NOERROR;
}

ECode IpConfiguration::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        StringBuilder sbuf = new StringBuilder();
        sbuf.append("IP assignment: " + ipAssignment.toString());
        sbuf.append("\n");
        if (staticIpConfiguration != null) {
            sbuf.append("Static configuration: " + staticIpConfiguration.toString());
            sbuf.append("\n");
        }
        sbuf.append("Proxy settings: " + proxySettings.toString());
        sbuf.append("\n");
        if (httpProxy != null) {
            sbuf.append("HTTP proxy: " + httpProxy.toString());
            sbuf.append("\n");
        }
        return sbuf.toString();

#endif
}

ECode IpConfiguration::Equals(
    /* [in] */ IObject* o,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (o == this) {
            return true;
        }
        if (!(o instanceof IpConfiguration)) {
            return false;
        }
        IpConfiguration other = (IpConfiguration) o;
        return this.ipAssignment == other.ipAssignment &&
                this.proxySettings == other.proxySettings &&
                Objects.equals(this.staticIpConfiguration, other.staticIpConfiguration) &&
                Objects.equals(this.httpProxy, other.httpProxy);

#endif
}

ECode IpConfiguration::HashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return 13 + (staticIpConfiguration != null ? staticIpConfiguration.hashCode() : 0) +
               17 * ipAssignment.ordinal() +
               47 * proxySettings.ordinal() +
               83 * httpProxy.hashCode();

#endif
}

ECode IpConfiguration::DescribeContents(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = 0;
    return NOERROR;
}

ECode IpConfiguration::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        dest.writeString(ipAssignment.name());
        dest.writeString(proxySettings.name());
        dest.writeParcelable(staticIpConfiguration, flags);
        dest.writeParcelable(httpProxy, flags);

#endif
}

ECode IpConfiguration::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public IpConfiguration createFromParcel(Parcel in) {
                IpConfiguration config = new IpConfiguration();
                config.ipAssignment = IpAssignment.valueOf(in.readString());
                config.proxySettings = ProxySettings.valueOf(in.readString());
                config.staticIpConfiguration = in.readParcelable(null);
                config.httpProxy = in.readParcelable(null);
                return config;
            }
            public IpConfiguration[] newArray(int size) {
                return new IpConfiguration[size];
            }

#endif

}

ECode IpConfiguration::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public IpConfiguration createFromParcel(Parcel in) {
                IpConfiguration config = new IpConfiguration();
                config.ipAssignment = IpAssignment.valueOf(in.readString());
                config.proxySettings = ProxySettings.valueOf(in.readString());
                config.staticIpConfiguration = in.readParcelable(null);
                config.httpProxy = in.readParcelable(null);
                return config;
            }
            public IpConfiguration[] newArray(int size) {
                return new IpConfiguration[size];
            }

#endif
}



} // namespace Net
} // namespace Droid
} // namespace Elastos
