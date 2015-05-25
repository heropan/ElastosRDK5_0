
#include "HttpConnectionPool.h"
#include "HttpConnection.h"
#include "CSystem.h"
#include "elastos/StringUtils.h"

using Elastos::Core::ISystem;
using Elastos::Core::StringUtils;

namespace Elastos {
namespace Net {
namespace Http {

Mutex HttpConnectionPool::sLock;
AutoPtr<IHttpConnectionPool> HttpConnectionPool::INSTANCE;

CAR_INTERFACE_IMPL(HttpConnectionPool, IHttpConnectionPool)

AutoPtr<IHttpConnectionPool> HttpConnectionPool::GetInstance()
{
    Mutex::Autolock lock(sLock);
    if (INSTANCE.Get() == NULL) {
        INSTANCE = new HttpConnectionPool();
    }
    return INSTANCE;
}

ECode HttpConnectionPool::Get(
    /* [in] */ IHttpConnectionAddress* address,
    /* [in] */ Int32 connectTimeout,
    /* [out] */ IHttpConnection** hc)
{
    VALIDATE_NOT_NULL(hc)

    assert(0 && "TODO");
    // First try to reuse an existing HTTP connection.
    // {
    //     Mutex::Autolock lock(mLock);
    //     AutoPtr<List<AutoPtr<IHttpConnection> > > connections = mConnectionPool[address];
    //     while (connections != NULL) {
    //         IHttpConnection* connection = (*connections)[connections->GetSize() - 1];
    //         connections->Remove(connections->GetSize() - 1);
    //         if (connections->IsEmpty()) {
    //             mConnectionPool.Erase(address);
    //             connections = NULL;
    //         }
    //         if (((HttpConnection*)connection)->IsEligibleForRecycling()) {
    //             // Since Socket is recycled, re-tag before using
    //             AutoPtr<ISocket> socket = ((HttpConnection*)connection)->GetSocket();
    //             SocketTagger.get().tag(socket);
    //             *hc = connection;
    //             INTERFACE_ADDREF(*hc)
    //             return NOERROR;
    //         }
    //     }
    // }

    /*
     * We couldn't find a reusable connection, so we need to create a new
     * connection. We're careful not to do so while holding a lock!
     */
    AutoPtr<IHttpConnection> httc;
    address->Connect(connectTimeout, (IHttpConnection**)&httc);
    *hc = httc;
    INTERFACE_ADDREF(*hc)

    return NOERROR;
}

ECode HttpConnectionPool::Recycle(
    /* [in] */ IHttpConnection* connection)
{
    assert(0 && "TODO");
    // Socket socket = connection.getSocket();
    // try {
    //     SocketTagger.get().untag(socket);
    // } catch (SocketException e) {
    //     // When unable to remove tagging, skip recycling and close
    //     System.logW("Unable to untagSocket(): " + e);
    //     connection.closeSocketAndStreams();
    //     return;
    // }

    // if (maxConnections > 0 && connection.isEligibleForRecycling()) {
    //     HttpConnection.Address address = connection.getAddress();
    //     synchronized (connectionPool) {
    //         List<HttpConnection> connections = connectionPool.get(address);
    //         if (connections == null) {
    //             connections = new ArrayList<HttpConnection>();
    //             connectionPool.put(address, connections);
    //         }
    //         if (connections.size() < maxConnections) {
    //             connection.setRecycled();
    //             connections.add(connection);
    //             return; // keep the connection open
    //         }
    //     }
    // }

    // // don't close streams while holding a lock!
    // connection.closeSocketAndStreams();
    return NOERROR;
}

HttpConnectionPool::HttpConnectionPool()
{
    AutoPtr<Elastos::Core::CSystem> system;
    Elastos::Core::CSystem::AcquireSingletonByFriend((Elastos::Core::CSystem**)&system);

    String keepAlive;
    system->GetProperty(String("http.keepAlive"), &keepAlive);
    if (!keepAlive.IsNull() && !StringUtils::ParseInt32(keepAlive)) {
        mMaxConnections = 0;
        return;
    }

    String maxConnectionsString;
    system->GetProperty(String("http.maxConnections"), &maxConnectionsString);
    mMaxConnections = !maxConnectionsString.IsNull() ? StringUtils::ParseInt32(maxConnectionsString) : 5;
}

} // namespace Http
} // namespace Net
} // namespace Elastos
