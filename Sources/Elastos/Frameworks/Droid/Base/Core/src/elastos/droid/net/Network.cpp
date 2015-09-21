
#include "Network.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(Network, Object, INetwork, IParcelable)

Boolean Network::GetHTTP_KEEP_ALIVE()
{
#if 0 // TODO: Translate codes below.
// = Boolean.parseBoolean(System.getProperty("http.keepAlive", "true"));
#else
    assert(0);
#endif
    return FALSE;
}

Int32 Network::GetHTTP_MAX_CONNECTIONS()
{
#if 0 // TODO: Translate codes below.
// = httpKeepAlive ? Integer.parseInt(System.getProperty("http.maxConnections", "5")) : 0;
#else
    assert(0);
#endif
    return 0;
}

Int32 Network::GetHTTP_KEEP_ALIVE_DURATION_MS()
{
#if 0 // TODO: Translate codes below.
// = Long.parseLong(System.getProperty("http.keepAliveDuration", "300000"));  // 5 minutes.
#else
    assert(0);
#endif
    return 0;
}

const Boolean Network::HTTP_KEEP_ALIVE = GetHTTP_KEEP_ALIVE();
const Int32 Network::HTTP_MAX_CONNECTIONS = GetHTTP_MAX_CONNECTIONS();
const Int64 Network::HTTP_KEEP_ALIVE_DURATION_MS = GetHTTP_KEEP_ALIVE_DURATION_MS();

Network::Network()
    : mNetworkBoundSocketFactory(NULL)
//    , mConnectionPool(NULL)
//    , mHostResolver(NULL)
{}

ECode Network::constructor(
    /* [in] */ Int32 netId)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.netId = netId;

#endif
}

ECode Network::constructor(
    /* [in] */ INetwork* that)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.netId = that.netId;

#endif
}

ECode Network::GetAllByName(
    /* [in] */ const String& host,
    /* [out, callee] */ ArrayOf<IInetAddress*>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return InetAddress.getAllByNameOnNet(host, netId);

#endif
}

ECode Network::GetByName(
    /* [in] */ const String& host,
    /* [out] */ IInetAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return InetAddress.getByNameOnNet(host, netId);

#endif
}

ECode Network::GetSocketFactory(
    /* [out] */ ISocketFactory** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (mNetworkBoundSocketFactory == null) {
            synchronized (mLock) {
                if (mNetworkBoundSocketFactory == null) {
                    mNetworkBoundSocketFactory = new NetworkBoundSocketFactory(netId);
                }
            }
        }
        return mNetworkBoundSocketFactory;

#endif
}

ECode Network::MaybeInitHttpClient()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (mLock) {
            if (mHostResolver == null) {
                mHostResolver = new HostResolver() {
                    @Override
                    public InetAddress[] getAllByName(String host) throws UnknownHostException {
                        return Network.this.getAllByName(host);
                    }
                };
            }
            if (mConnectionPool == null) {
                mConnectionPool = new ConnectionPool(httpMaxConnections,
                        httpKeepAliveDurationMs);
            }
        }

#endif
}

ECode Network::OpenConnection(
    /* [in] */ IURL* url,
    /* [out] */ IURLConnection** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        maybeInitHttpClient();
        String protocol = url.getProtocol();
        OkHttpClient client;
        // TODO: HttpHandler creates OkHttpClients that share the default ResponseCache.
        // Could this cause unexpected behavior?
        // TODO: Should the network's proxy be specified?
        if (protocol.equals("http")) {
            client = HttpHandler.createHttpOkHttpClient(null /* proxy */);
        } else if (protocol.equals("https")) {
            client = HttpsHandler.createHttpsOkHttpClient(null /* proxy */);
        } else {
            // OkHttpClient only supports HTTP and HTTPS and returns a null URLStreamHandler if
            // passed another protocol.
            throw new MalformedURLException("Invalid URL or unrecognized protocol " + protocol);
        }
        return client.setSocketFactory(getSocketFactory())
                .setHostResolver(mHostResolver)
                .setConnectionPool(mConnectionPool)
                .open(url);

#endif
}

ECode Network::BindSocket(
    /* [in] */ ISocket* socket)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (socket.isConnected()) {
            throw new SocketException("Socket is connected");
        }
        // Query a property of the underlying socket to ensure the underlying
        // socket exists so a file descriptor is available to bind to a network.
        socket.getReuseAddress();
        int err = NetworkUtils.bindSocketToNetwork(socket.getFileDescriptor$().getInt$(), netId);
        if (err != 0) {
            // bindSocketToNetwork returns negative errno.
            throw new ErrnoException("Binding socket to network " + netId, -err)
                    .rethrowAsSocketException();
        }

#endif
}

ECode Network::DescribeContents(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return 0;

#endif
}

ECode Network::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        dest.writeInt(netId);

#endif
}

ECode Network::Equals(
    /* [in] */ IObject* obj,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (obj instanceof Network == false) return false;
        Network other = (Network)obj;
        return this.netId == other.netId;

#endif
}

ECode Network::HashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return netId * 11;

#endif
}

ECode Network::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return Integer.toString(netId);

#endif
}

ECode Network::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Program code below
#endif
}

ECode Network::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Program code below
#endif
}

ECode Network::GetNetId(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mNetId;
    return NOERROR;
}

//==============================================================
// Network::NetworkBoundSocketFactory
//==============================================================
Network::NetworkBoundSocketFactory::NetworkBoundSocketFactory(
    /* [in] */ Int32 netId)
    : mNetId(netId)
{
#if 0 // TODO: Translate codes below
                super();
                mNetId = netId;

#endif
}

ECode Network::NetworkBoundSocketFactory::ConnectToHost(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ ISocketAddress* localAddress,
    /* [out] */ ISocket** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                // Lookup addresses only on this Network.
                InetAddress[] hostAddresses = getAllByName(host);
                // Try all addresses.
                for (int i = 0; i < hostAddresses.length; i++) {
                    try {
                        Socket socket = createSocket();
                        if (localAddress != null) socket.bind(localAddress);
                        socket.connect(new InetSocketAddress(hostAddresses[i], port));
                        return socket;
                    } catch (IOException e) {
                        if (i == (hostAddresses.length - 1)) throw e;
                    }
                }
                throw new UnknownHostException(host);

#endif
}

ECode Network::NetworkBoundSocketFactory::CreateSocket(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ IInetAddress* localHost,
    /* [in] */ Int32 localPort,
    /* [out] */ ISocket** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                return connectToHost(host, port, new InetSocketAddress(localHost, localPort));

#endif
}

ECode Network::NetworkBoundSocketFactory::CreateSocket(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port,
    /* [in] */ IInetAddress* localAddress,
    /* [in] */ Int32 localPort,
    /* [out] */ ISocket** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                Socket socket = createSocket();
                socket.bind(new InetSocketAddress(localAddress, localPort));
                socket.connect(new InetSocketAddress(address, port));
                return socket;

#endif
}

ECode Network::NetworkBoundSocketFactory::CreateSocket(
    /* [in] */ IInetAddress* host,
    /* [in] */ Int32 port,
    /* [out] */ ISocket** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                Socket socket = createSocket();
                socket.connect(new InetSocketAddress(host, port));
                return socket;

#endif
}

ECode Network::NetworkBoundSocketFactory::CreateSocket(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [out] */ ISocket** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                return connectToHost(host, port, null);

#endif
}

ECode Network::NetworkBoundSocketFactory::CreateSocket(
    /* [out] */ ISocket** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                Socket socket = new Socket();
                bindSocket(socket);
                return socket;

#endif
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
