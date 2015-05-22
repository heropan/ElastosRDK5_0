
#ifndef __HTTPCONNECTIONPOOL_H__
#define __HTTPCONNECTIONPOOL_H__

#include "cmdef.h"
#include "Elastos.CoreLibrary_server.h"
#include "elastos/HashMap.h"
#include "elastos/List.h"
#include "elastos/Mutex.h"

using Elastos::Utility::HashMap;
using Elastos::Utility::List;
using Elastos::Core::Threading::Mutex;

namespace Elastos {
namespace Net {
namespace Http {

class HttpConnectionPool
    : public ElRefBase
    , public IHttpConnectionPool
{
public:
    CAR_INTERFACE_DECL()

    CARAPI Get(
        /* [in] */ IHttpConnectionAddress* address,
        /* [in] */ Int32 connectTimeout,
        /* [out] */ IHttpConnection** hc);

    CARAPI Recycle(
        /* [in] */ IHttpConnection* connection);

    static CARAPI_(AutoPtr<IHttpConnectionPool>) GetInstance();

private:
    HttpConnectionPool();

private:
    static Mutex sLock;
    static AutoPtr<IHttpConnectionPool> INSTANCE;

    Int32 mMaxConnections;
    HashMap<AutoPtr<IHttpConnectionAddress>, AutoPtr<List<AutoPtr<IHttpConnection> > > > mConnectionPool; // = new HashMap<HttpConnection.Address, List<HttpConnection>>();
    Mutex mLock;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__HTTPCONNECTIONPOOL_H__
