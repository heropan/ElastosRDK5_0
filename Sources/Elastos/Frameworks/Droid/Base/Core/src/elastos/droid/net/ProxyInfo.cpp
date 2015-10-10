
#include "elastos/droid/net/ProxyInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(ProxyInfo, Object, IParcelable, IProxyInfo)

ECode ProxyInfo::BuildDirectProxy(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [out] */ IProxyInfo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return new ProxyInfo(host, port, null);

#endif
}

ECode ProxyInfo::BuildDirectProxy(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ IList* exclList,
    /* [out] */ IProxyInfo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String[] array = exclList.toArray(new String[exclList.size()]);
        return new ProxyInfo(host, port, TextUtils.join(",", array), array);

#endif
}

ECode ProxyInfo::BuildPacProxy(
    /* [in] */ IUri* pacUri,
    /* [out] */ IProxyInfo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return new ProxyInfo(pacUri);

#endif
}

ECode ProxyInfo::constructor(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& exclList)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mHost = host;
        mPort = port;
        setExclusionList(exclList);
        mPacFileUrl = Uri.EMPTY;

#endif
}

ECode ProxyInfo::constructor(
    /* [in] */ IUri* pacFileUrl)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mHost = LOCAL_HOST;
        mPort = LOCAL_PORT;
        setExclusionList(LOCAL_EXCL_LIST);
        if (pacFileUrl == null) {
            throw new NullPointerException();
        }
        mPacFileUrl = pacFileUrl;

#endif
}

ECode ProxyInfo::constructor(
    /* [in] */ const String& pacFileUrl)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mHost = LOCAL_HOST;
        mPort = LOCAL_PORT;
        setExclusionList(LOCAL_EXCL_LIST);
        mPacFileUrl = Uri.parse(pacFileUrl);

#endif
}

ECode ProxyInfo::constructor(
    /* [in] */ IUri* pacFileUrl,
    /* [in] */ Int32 localProxyPort)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mHost = LOCAL_HOST;
        mPort = localProxyPort;
        setExclusionList(LOCAL_EXCL_LIST);
        if (pacFileUrl == null) {
            throw new NullPointerException();
        }
        mPacFileUrl = pacFileUrl;

#endif
}

ProxyInfo::ProxyInfo(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& exclList,
    /* [in] */ ArrayOf<String>* parsedExclList)
{
#if 0 // TODO: Translate codes below
        mHost = host;
        mPort = port;
        mExclusionList = exclList;
        mParsedExclusionList = parsedExclList;
        mPacFileUrl = Uri.EMPTY;
#endif
}

ECode ProxyInfo::constructor(
    /* [in] */ IProxyInfo* source)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (source != null) {
            mHost = source.getHost();
            mPort = source.getPort();
            mPacFileUrl = source.mPacFileUrl;
            mExclusionList = source.getExclusionListAsString();
            mParsedExclusionList = source.mParsedExclusionList;
        } else {
            mPacFileUrl = Uri.EMPTY;
        }

#endif
}

ECode ProxyInfo::GetSocketAddress(
    /* [out] */ IInetSocketAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        InetSocketAddress inetSocketAddress = null;
        try {
            inetSocketAddress = new InetSocketAddress(mHost, mPort);
        } catch (IllegalArgumentException e) { }
        return inetSocketAddress;

#endif
}

ECode ProxyInfo::GetPacFileUrl(
    /* [out] */ IUri** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mPacFileUrl;

#endif
}

ECode ProxyInfo::GetHost(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mHost;

#endif
}

ECode ProxyInfo::GetPort(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mPort;

#endif
}

ECode ProxyInfo::GetExclusionList(
    /* [out, callee] */ ArrayOf<String>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mParsedExclusionList;

#endif
}

ECode ProxyInfo::GetExclusionListAsString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mExclusionList;

#endif
}

ECode ProxyInfo::SetExclusionList(
    /* [in] */ const String& exclusionList)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mExclusionList = exclusionList;
        if (mExclusionList == null) {
            mParsedExclusionList = new String[0];
        } else {
            mParsedExclusionList = exclusionList.toLowerCase(Locale.ROOT).split(",");
        }

#endif
}

ECode ProxyInfo::IsValid(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (!Uri.EMPTY.equals(mPacFileUrl)) return true;
        return Proxy.PROXY_VALID == Proxy.validate(mHost == null ? "" : mHost,
                                                mPort == 0 ? "" : Integer.toString(mPort),
                                                mExclusionList == null ? "" : mExclusionList);

#endif
}

ECode ProxyInfo::MakeProxy(
    /* [out] */ Elastos::Net::IProxy** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        java.net.Proxy proxy = java.net.Proxy.NO_PROXY;
        if (mHost != null) {
            try {
                InetSocketAddress inetSocketAddress = new InetSocketAddress(mHost, mPort);
                proxy = new java.net.Proxy(java.net.Proxy.Type.HTTP, inetSocketAddress);
            } catch (IllegalArgumentException e) {
            }
        }
        return proxy;

#endif
}

ECode ProxyInfo::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        StringBuilder sb = new StringBuilder();
        if (!Uri.EMPTY.equals(mPacFileUrl)) {
            sb.append("PAC Script: ");
            sb.append(mPacFileUrl);
        } else if (mHost != null) {
            sb.append("[");
            sb.append(mHost);
            sb.append("] ");
            sb.append(Integer.toString(mPort));
            if (mExclusionList != null) {
                    sb.append(" xl=").append(mExclusionList);
            }
        } else {
            sb.append("[ProxyProperties.mHost == null]");
        }
        return sb.toString();

#endif
}

ECode ProxyInfo::Equals(
    /* [in] */ IObject* o,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (!(o instanceof ProxyInfo)) return false;
        ProxyInfo p = (ProxyInfo)o;
        // If PAC URL is present in either then they must be equal.
        // Other parameters will only be for fall back.
        if (!Uri.EMPTY.equals(mPacFileUrl)) {
            return mPacFileUrl.equals(p.getPacFileUrl()) && mPort == p.mPort;
        }
        if (!Uri.EMPTY.equals(p.mPacFileUrl)) {
            return false;
        }
        if (mExclusionList != null && !mExclusionList.equals(p.getExclusionListAsString())) {
            return false;
        }
        if (mHost != null && p.getHost() != null && mHost.equals(p.getHost()) == false) {
            return false;
        }
        if (mHost != null && p.mHost == null) return false;
        if (mHost == null && p.mHost != null) return false;
        if (mPort != p.mPort) return false;
        return true;

#endif
}

ECode ProxyInfo::DescribeContents(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return 0;

#endif
}

ECode ProxyInfo::HashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return ((null == mHost) ? 0 : mHost.hashCode())
        + ((null == mExclusionList) ? 0 : mExclusionList.hashCode())
        + mPort;

#endif
}

ECode ProxyInfo::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (!Uri.EMPTY.equals(mPacFileUrl)) {
            dest.writeByte((byte)1);
            mPacFileUrl.writeToParcel(dest, 0);
            dest.writeInt(mPort);
            return;
        } else {
            dest.writeByte((byte)0);
        }
        if (mHost != null) {
            dest.writeByte((byte)1);
            dest.writeString(mHost);
            dest.writeInt(mPort);
        } else {
            dest.writeByte((byte)0);
        }
        dest.writeString(mExclusionList);
        dest.writeStringArray(mParsedExclusionList);

#endif
}

ECode ProxyInfo::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public ProxyInfo createFromParcel(Parcel in) {
                String host = null;
                int port = 0;
                if (in.readByte() != 0) {
                    Uri url = Uri.CREATOR.createFromParcel(in);
                    int localPort = in.readInt();
                    return new ProxyInfo(url, localPort);
                }
                if (in.readByte() != 0) {
                    host = in.readString();
                    port = in.readInt();
                }
                String exclList = in.readString();
                String[] parsedExclList = in.readStringArray();
                ProxyInfo proxyProperties =
                        new ProxyInfo(host, port, exclList, parsedExclList);
                return proxyProperties;
            }
            public ProxyInfo[] newArray(int size) {
                return new ProxyInfo[size];
            }

#endif
}

ECode ProxyInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public ProxyInfo createFromParcel(Parcel in) {
                String host = null;
                int port = 0;
                if (in.readByte() != 0) {
                    Uri url = Uri.CREATOR.createFromParcel(in);
                    int localPort = in.readInt();
                    return new ProxyInfo(url, localPort);
                }
                if (in.readByte() != 0) {
                    host = in.readString();
                    port = in.readInt();
                }
                String exclList = in.readString();
                String[] parsedExclList = in.readStringArray();
                ProxyInfo proxyProperties =
                        new ProxyInfo(host, port, exclList, parsedExclList);
                return proxyProperties;
            }
            public ProxyInfo[] newArray(int size) {
                return new ProxyInfo[size];
            }

#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
