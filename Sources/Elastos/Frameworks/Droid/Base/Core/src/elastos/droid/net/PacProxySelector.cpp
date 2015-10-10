
#include "elastos/droid/net/PacProxySelector.h"

namespace Elastos {
namespace Droid {
namespace Net {

#if 0 // TODO: Waiting for outport ProxySelector
CAR_INTERFACE_IMPL(PacProxySelector, ProxySelector, IPacProxySelector)
#else
CAR_INTERFACE_IMPL(PacProxySelector, Object, IPacProxySelector)
#endif

const String PacProxySelector::TAG = String("PacProxySelector");
const String PacProxySelector::SOCKS = String("SOCKS ");
const String PacProxySelector::PROXY = String("PROXY ");

ECode PacProxySelector::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mProxyService = IProxyService.Stub.asInterface(
                ServiceManager.getService(PROXY_SERVICE));
        if (mProxyService == null) {
            // Added because of b10267814 where mako is restarting.
            Log.e(TAG, "PacManager: no proxy service");
        }
        mDefaultList = Lists.newArrayList(java.net.Proxy.NO_PROXY);

#endif
}

ECode PacProxySelector::Select(
    /* [in] */ IURI* uri,
    /* [out] */ IList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (mProxyService == null) {
            mProxyService = IProxyService.Stub.asInterface(
                    ServiceManager.getService(PROXY_SERVICE));
        }
        if (mProxyService == null) {
            Log.e(TAG, "select: no proxy service return NO_PROXY");
            return Lists.newArrayList(java.net.Proxy.NO_PROXY);
        }
        String response = null;
        String urlString;
        try {
            urlString = uri.toURL().toString();
        } catch (MalformedURLException e) {
            urlString = uri.getHost();
        }
        try {
            response = mProxyService.resolvePacFile(uri.getHost(), urlString);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        if (response == null) {
            return mDefaultList;
        }
        return parseResponse(response);

#endif
}

ECode PacProxySelector::ParseResponse(
    /* [in] */ const String& response,
    /* [out] */ IList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String[] split = response.split(";");
        List<Proxy> ret = Lists.newArrayList();
        for (String s : split) {
            String trimmed = s.trim();
            if (trimmed.equals("DIRECT")) {
                ret.add(java.net.Proxy.NO_PROXY);
            } else if (trimmed.startsWith(PROXY)) {
                Proxy proxy = proxyFromHostPort(Type.HTTP, trimmed.substring(PROXY.length()));
                if (proxy != null) {
                    ret.add(proxy);
                }
            } else if (trimmed.startsWith(SOCKS)) {
                Proxy proxy = proxyFromHostPort(Type.SOCKS, trimmed.substring(SOCKS.length()));
                if (proxy != null) {
                    ret.add(proxy);
                }
            }
        }
        if (ret.size() == 0) {
            ret.add(java.net.Proxy.NO_PROXY);
        }
        return ret;

#endif
}

ECode PacProxySelector::ProxyFromHostPort(
    /* [in] */ ProxyType type,
    /* [in] */ const String& hostPortString,
    /* [out] */ Elastos::Net::IProxy** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            String[] hostPort = hostPortString.split(":");
            String host = hostPort[0];
            int port = Integer.parseInt(hostPort[1]);
            return new Proxy(type, InetSocketAddress.createUnresolved(host, port));
        } catch (NumberFormatException|ArrayIndexOutOfBoundsException e) {
            Log.d(TAG, "Unable to parse proxy " + hostPortString + " " + e);
            return null;
        }

#endif
}

ECode PacProxySelector::ConnectFailed(
    /* [in] */ IURI* uri,
    /* [in] */ ISocketAddress* address,
    /* [in] */ ECode failure)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below

#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
