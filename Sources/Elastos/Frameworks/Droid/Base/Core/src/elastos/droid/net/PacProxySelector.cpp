
#include "elastos/droid/net/PacProxySelector.h"
#include "elastos/droid/net/ReturnOutValue.h"
#include "elastos/droid/os/ServiceManager.h"
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Net::IIProxyService;
using Elastos::Droid::Os::ServiceManager;

using Elastos::Core::StringUtils;
using Elastos::Net::CProxy;
using Elastos::Net::CInetSocketAddress;
using Elastos::Net::CInetSocketAddressHelper;
using Elastos::Net::IInetSocketAddress;
using Elastos::Net::IInetSocketAddressHelper;
using Elastos::Utility::CArrayList;
using Elastos::Utility::IArrayList;
using Elastos::Utility::IList;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(PacProxySelector, ProxySelector, IPacProxySelector)

const String PacProxySelector::TAG("PacProxySelector");
const String PacProxySelector::SOCKS("SOCKS ");
const String PacProxySelector::PROXY("PROXY ");

ECode PacProxySelector::constructor()
{
    mProxyService = IIProxyService::Probe(
            ServiceManager::GetService(PROXY_SERVICE));
    if (mProxyService == NULL) {
        // Added because of b10267814 where mako is restarting.
        Logger::E(TAG, "PacManager: no proxy service");
    }
    AutoPtr<Elastos::Net::IProxyHelper> helper;
    Elastos::Net::CProxyHelper::AcquireSingleton((Elastos::Net::IProxyHelper**)&helper);
    AutoPtr<Elastos::Net::IProxy> noProxy;
    helper->GetNO_PROXY((Elastos::Net::IProxy**)&noProxy);
    CArrayList::New((IArrayList**)&mDefaultList);
    mDefaultList->Add(noProxy);
    return NOERROR;
}

ECode PacProxySelector::Select(
    /* [in] */ IURI* uri,
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result)

    if (mProxyService == NULL) {
        mProxyService = IIProxyService::Probe(
                ServiceManager::GetService(PROXY_SERVICE));
    }
    if (mProxyService == NULL) {
        Logger::E(TAG, "select: no proxy service return NO_PROXY");
        AutoPtr<Elastos::Net::IProxyHelper> helper;
        Elastos::Net::CProxyHelper::AcquireSingleton((Elastos::Net::IProxyHelper**)&helper);
        AutoPtr<Elastos::Net::IProxy> noProxy;
        helper->GetNO_PROXY((Elastos::Net::IProxy**)&noProxy);
        CArrayList::New(result);
        (*result)->Add(noProxy);
        return NOERROR;
    }
    String response(NULL);
    String urlString;
    // try {
        ECode ec = Ptr(uri)->Func(uri->ToURL)->ToString(&urlString);
    // } catch (MalformedURLException e) {
        if (ec == (ECode)E_MALFORMED_URL_EXCEPTION) {
            uri->GetHost(&urlString);
        }
        else if (FAILED(ec)) return ec;
    // }
    // try {
        // TODO: Waiting for IIProxyService
        assert(0);
        // mProxyService->ResolvePacFile(Ptr(uri)->Func(uri->GetHost), urlString, &response);
    // } catch (RemoteException e) {
        if (FAILED(ec)) {
            if (ec == (ECode)E_REMOTE_EXCEPTION) {
                // e.printStackTrace();
            }
            else {
                return ec;
            }
        }
    // }
    if (response == NULL) {
        FUNC_RETURN(mDefaultList)
    }
    *result = ParseResponse(response);
    return NOERROR;
}

AutoPtr<IList> PacProxySelector::ParseResponse(
    /* [in] */ const String& response)
{
    AutoPtr<ArrayOf<String> > split;
    StringUtils::Split(response, String(";"), (ArrayOf<String>**)&split);
    AutoPtr<IList> ret;
    CArrayList::New((IList**)&ret);
    for (Int32 i = 0; i < split->GetLength(); ++i) {
        String s = (*split)[i];
        String trimmed = s.Trim();
        if (trimmed.Equals("DIRECT")) {
            AutoPtr<Elastos::Net::IProxyHelper> helper;
            Elastos::Net::CProxyHelper::AcquireSingleton((Elastos::Net::IProxyHelper**)&helper);
            AutoPtr<Elastos::Net::IProxy> noProxy;
            helper->GetNO_PROXY((Elastos::Net::IProxy**)&noProxy);
            ret->Add(noProxy);
        } else if (trimmed.StartWith(PROXY)) {
            AutoPtr<Elastos::Net::IProxy> proxy;
            ProxyFromHostPort(Elastos::Net::ProxyType_HTTP, trimmed.Substring(PROXY.GetLength()), (Elastos::Net::IProxy**)&proxy);
            if (proxy != NULL) {
                ret->Add(proxy);
            }
        } else if (trimmed.StartWith(SOCKS)) {
            AutoPtr<Elastos::Net::IProxy> proxy;
            ProxyFromHostPort(Elastos::Net::ProxyType_SOCKS, trimmed.Substring(SOCKS.GetLength()), (Elastos::Net::IProxy**)&proxy);
            if (proxy != NULL) {
                ret->Add(proxy);
            }
        }
    }
    if (Ptr(ret)->Func(ret->GetSize) == 0) {
        AutoPtr<Elastos::Net::IProxyHelper> helper;
        Elastos::Net::CProxyHelper::AcquireSingleton((Elastos::Net::IProxyHelper**)&helper);
        AutoPtr<Elastos::Net::IProxy> noProxy;
        helper->GetNO_PROXY((Elastos::Net::IProxy**)&noProxy);
        ret->Add(noProxy);
    }
    return ret;
}

ECode PacProxySelector::ProxyFromHostPort(
    /* [in] */ ProxyType type,
    /* [in] */ const String& hostPortString,
    /* [out] */ Elastos::Net::IProxy** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

        // try {
    AutoPtr<ArrayOf<String> > hostPort;
    StringUtils::Split(hostPortString, String(":"), (ArrayOf<String>**)&hostPort);
    String host = (*hostPort)[0];
    Int32 port = StringUtils::ParseInt32((*hostPort)[1]);
    AutoPtr<IInetSocketAddressHelper> helper;
    CInetSocketAddressHelper::AcquireSingleton((IInetSocketAddressHelper**)&helper);
    AutoPtr<IInetSocketAddress> address;
    ECode ec = helper->CreateUnresolved(host, port, (IInetSocketAddress**)&address);
    if (FAILED(ec)) {
        if (ec == (ECode)E_NUMBER_FORMAT_EXCEPTION|| ec == (ECode)E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION) {
            Logger::D(TAG, "Unable to parse proxy %s %d", hostPortString.string(), ec);
            *result = NULL;
            return NOERROR;
        }
        return ec;
    }
    AutoPtr<Elastos::Net::IProxy> rev;
    ec = CProxy::New(type, ISocketAddress::Probe(address), (Elastos::Net::IProxy**)&rev);
    if (FAILED(ec)) {
        if (ec == (ECode)E_NUMBER_FORMAT_EXCEPTION|| ec == (ECode)E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION) {
            Logger::D(TAG, "Unable to parse proxy %s %d", hostPortString.string(), ec);
            *result = NULL;
            return NOERROR;
        }
        return ec;
    }
    FUNC_RETURN(rev)
        // } catch (NumberFormatException|ArrayIndexOutOfBoundsException e) {
        // }
}

ECode PacProxySelector::ConnectFailed(
    /* [in] */ IURI* uri,
    /* [in] */ ISocketAddress* address,
    /* [in] */ ECode failure)
{
    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
