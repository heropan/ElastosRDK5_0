
#ifndef __ORG_APACHE_HTTP_IMPL_CONN_TSCCM_BASICPOOLENTRY_H__
#define __ORG_APACHE_HTTP_IMPL_CONN_TSCCM_BASICPOOLENTRY_H__

#include "AbstractPoolEntry.h"
#include "BasicPoolEntryRef.h"

using Elastos::Utility::IList;
using Org::Apache::Http::Conn::Routing::IHttpRoute;
using Org::Apache::Http::Conn::IOperatedClientConnection;
using Org::Apache::Http::Conn::IClientConnectionOperator;
using Org::Apache::Http::Impl::Conn::AbstractPoolEntry;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Conn {
namespace Tsccm {

extern "C" const InterfaceID EIID_BasicPoolEntry;

/**
 * Basic implementation of a connection pool entry.
 */
class BasicPoolEntry : public AbstractPoolEntry
{
public:
    /**
     * Creates a new pool entry.
     *
     * @param op      the connection operator
     * @param route   the planned route for the connection
     * @param queue   the reference queue for tracking GC of this entry,
     *                or <code>null</code>
     */
    BasicPoolEntry(
        /* [in] */ IClientConnectionOperator* op,
        /* [in] */ IHttpRoute* route,
        /* [in] */ IList* queue);/* ReferenceQueue<Object> */

    CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

protected:
    CARAPI_(AutoPtr<IOperatedClientConnection>) GetConnection();

    CARAPI_(AutoPtr<IHttpRoute>) GetPlannedRoute();

    CARAPI_(AutoPtr<BasicPoolEntryRef>) GetWeakRef();

private:
    /**
     * A weak reference to <code>this</code> used to detect GC of entries.
     * Pool entries can only be GCed when they are allocated by an application
     * and therefore not referenced with a hard link in the manager.
     */
    AutoPtr<BasicPoolEntryRef> mReference;

};

} // namespace Tsccm
} // namespace Conn
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_IMPL_CONN_TSCCM_BASICPOOLENTRY_H__
