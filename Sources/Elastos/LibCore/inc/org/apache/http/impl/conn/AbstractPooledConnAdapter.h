
#ifndef __ORG_APACHE_HTTP_IMPL_CONN_ABSTRACTPOOLEDCONNADAPTER_H__
#define __ORG_APACHE_HTTP_IMPL_CONN_ABSTRACTPOOLEDCONNADAPTER_H__

#include "AbstractClientConnAdapter.h"
#include "AbstractPoolEntry.h"

using Org::Apache::Http::IHttpHost;
using Org::Apache::Http::Params::IHttpParams;
using Org::Apache::Http::Conn::Routing::IHttpRoute;
using Org::Apache::Http::Protocol::IHttpContext;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Conn {

/**
 * Abstract adapter from pool {@link AbstractPoolEntry entries} to
 * {@link org.apache.http.conn.ManagedClientConnection managed}
 * client connections.
 * The connection in the pool entry is used to initialize the base class.
 * In addition, methods to establish a route are delegated to the
 * pool entry. {@link #shutdown shutdown} and {@link #close close}
 * will clear the tracked route in the pool entry and call the
 * respective method of the wrapped connection.
 *
 * @author <a href="mailto:rolandw at apache.org">Roland Weber</a>
 *
 *
 * <!-- empty lines to avoid svn diff problems -->
 * @version   $Revision: 658775 $ $Date: 2008-05-21 10:30:45 -0700 (Wed, 21 May 2008) $
 *
 * @since 4.0
 */
class AbstractPooledConnAdapter : public AbstractClientConnAdapter
{
public:
    virtual ~AbstractPooledConnAdapter() = 0;

    CARAPI GetRoute(
        /* [out] */ IHttpRoute** route);

    CARAPI Open(
        /* [in] */ IHttpRoute* route,
        /* [in] */ IHttpContext* context,
        /* [in] */ IHttpParams* params);

    CARAPI TunnelTarget(
        /* [in] */ Boolean secure,
        /* [in] */ IHttpParams* params);

    CARAPI TunnelProxy(
        /* [in] */ IHttpHost* next,
        /* [in] */ Boolean secure,
        /* [in] */ IHttpParams* params);

    CARAPI LayerProtocol(
        /* [in] */ IHttpContext* context,
        /* [in] */ IHttpParams* params);

    CARAPI Close();

    CARAPI Shutdown();

    CARAPI GetState(
        /* [out] */ IObject** state);

    CARAPI SetState(
        /* [in] */ IObject* state);

protected:
    AbstractPooledConnAdapter(
        /* [in] */ IClientConnectionManager* manager,
        /* [in] */ AbstractPoolEntry* entry);

    /**
     * Asserts that this adapter is still attached.
     *
     * @throws IllegalStateException
     *      if it is {@link #detach detach}ed
     */
    CARAPI AssertAttached();

    /**
     * Detaches this adapter from the wrapped connection.
     * This adapter becomes useless.
     */
    CARAPI_(void) Detach();

protected:
    /** The wrapped pool entry. */
    AutoPtr<AbstractPoolEntry> mPoolEntry;
};

} // namespace Conn
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_IMPL_CONN_ABSTRACTPOOLEDCONNADAPTER_H__
