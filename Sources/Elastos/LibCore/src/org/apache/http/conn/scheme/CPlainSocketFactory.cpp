
#include "CPlainSocketFactory.h"
#include "HttpConnectionParams.h"
#include <elastos/Logger.h>

using Elastos::Net::CSocket;
using Elastos::Net::IInetSocketAddress;
using Elastos::Net::CInetSocketAddress;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Conn {
namespace Scheme {

static AutoPtr<IPlainSocketFactory> InitDefaultFactory()
{
    AutoPtr<CPlainSocketFactory> factory;
    CPlainSocketFactory::NewByFriend((CPlainSocketFactory**)&factory);
    return (IPlainSocketFactory*)factory.Get();
}
const AutoPtr<IPlainSocketFactory> CPlainSocketFactory::DEFAULT_FACTORY = InitDefaultFactory();

CAR_INTERFACE_IMPL_2(CPlainSocketFactory, Object, IPlainSocketFactory, ISocketFactory)

CAR_OBJECT_IMPL(CPlainSocketFactory)

AutoPtr<IPlainSocketFactory> CPlainSocketFactory::GetSocketFactory()
{
    return DEFAULT_FACTORY;
}

ECode CPlainSocketFactory::CreateSocket(
    /* [out] */ ISocket** socket)
{
    VALIDATE_NOT_NULL(socket)
    return CSocket::New(socket);
}

ECode CPlainSocketFactory::ConnectSocket(
    /* [in] */ ISocket* sock,
    /* [in] */ String host,
    /* [in] */ Int32 port,
    /* [in] */ IInetAddress* localAddress,
    /* [in] */ Int32 localPort,
    /* [in] */ IHttpParams* params,
    /* [out] */ ISocket** socket)
{
    VALIDATE_NOT_NULL(socket)
    *socket = NULL;

    if (host == NULL) {
        Logger::E("CPlainSocketFactory", "Target host may not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        Logger::E("CPlainSocketFactory", "Parameters may not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (sock == NULL) {
        CreateSocket((ISocket**)&sock);
    }

    if ((localAddress != NULL) || (localPort > 0)) {

        // we need to bind explicitly
        if (localPort < 0)
            localPort = 0; // indicates "any"

        AutoPtr<IInetSocketAddress> isa;
        CInetSocketAddress::New(localAddress, localPort, (IInetSocketAddress**)&isa);
        sock->Bind(isa);
    }

    Int32 timeout;
    HttpConnectionParams::GetConnectionTimeout(params, &timeout);

    AutoPtr<IInetSocketAddress> remoteAddress;
    if (mNameResolver != NULL) {
        AutoPtr<IInetAddress> addr;
        mNameResolver->Resolve((IInetAddress**)&addr);
        CInetSocketAddress::New(addr, port, (IInetSocketAddress**)&remoteAddress);
    }
    else {
        CInetSocketAddress::New(host, port, (IInetSocketAddress**)&remoteAddress);
    }
    // try {
    ECode ec = sock->Connect(remoteAddress, timeout);
    if (ec == (ECode)E_SOCKET_TIMEOUT_EXCEPTION) {
        Logger::E("CPlainSocketFactory", "Connect to %p timed out", remoteAddress.Get());
        return E_CONNECT_TIMEOUT_EXCEPTION;
    }
    // } catch (SocketTimeoutException ex) {
    //     throw new ConnectTimeoutException("Connect to " + remoteAddress + " timed out");
    // }
    *socket = sock;
    REFCOUNT_ADD(*socket)
    return NOERROR;
}

ECode CPlainSocketFactory::IsSecure(
    /* [in] */ ISocket* sock,
    /* [out] */ Boolean* isSecure)
{
    VALIDATE_NOT_NULL(isSecure)
    *isSecure = FALSE;

    if (sock == NULL) {
        Logger::E("CPlainSocketFactory", "Socket may not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // This class check assumes that createSocket() calls the constructor
    // directly. If it was using javax.net.SocketFactory, we couldn't make
    // an assumption about the socket class here.
    ClassID clsid;
    if (sock->GetClassID(&clsid), clsid != ECLSID_CSocket) {
        Logger::E("MultihomePlainSocketFactory", "Socket not created by this factory.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // This check is performed last since it calls a method implemented
    // by the argument object. getClass() is final in java.lang.Object.
    Boolean isClosed;
    if (sock->IsClosed(&isClosed), isClosed) {
        Logger::E("MultihomePlainSocketFactory", "Socket is closed.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *isSecure = FALSE;
    return NOERROR;
}

ECode CPlainSocketFactory::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = (obj == this);
    return NOERROR;
}

ECode CPlainSocketFactory::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    ClassID clsid;
    GetClassID(&clsid);
    *hashCode = clsid.clsid.Data1;
    return NOERROR;
}

ECode CPlainSocketFactory::constructor(
    /* [in] */ IHostNameResolver* nameResolver)
{
    mNameResolver = nameResolver;
    return NOERROR;
}

ECode CPlainSocketFactory::constructor()
{
    return NOERROR;
}

} // namespace Scheme
} // namespace Conn
} // namespace Http
} // namespace Apache
} // namespace Org
