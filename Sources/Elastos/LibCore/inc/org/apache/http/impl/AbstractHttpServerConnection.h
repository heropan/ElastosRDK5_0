
#ifndef __ORG_APACHE_HTTP_IMPL_ABSTRACTHTTPSERVERCONNECTION_H__
#define __ORG_APACHE_HTTP_IMPL_ABSTRACTHTTPSERVERCONNECTION_H__

#include "org/apache/http/impl/HttpConnectionMetricsImpl.h"
#include "org/apache/http/impl/entity/EntityDeserializer.h"
#include "org/apache/http/impl/entity/EntitySerializer.h"

using Org::Apache::Http::IHttpServerConnection;
using Org::Apache::Http::IHttpConnection;
using Org::Apache::Http::IHttpEntityEnclosingRequest;
using Org::Apache::Http::IHttpRequestFactory;
using Org::Apache::Http::IHttpRequest;
using Org::Apache::Http::IHttpResponse;
using Org::Apache::Http::IHttpConnectionMetrics;
using Org::Apache::Http::Impl::Entity::EntitySerializer;
using Org::Apache::Http::Impl::Entity::EntityDeserializer;
using Org::Apache::Http::IO::IHttpMessageParser;
using Org::Apache::Http::IO::IHttpMessageWriter;
using Org::Apache::Http::IO::ISessionInputBuffer;
using Org::Apache::Http::IO::ISessionOutputBuffer;
using Org::Apache::Http::Params::IHttpParams;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {

/**
 * Abstract server-side HTTP connection capable of transmitting and receiving data
 * using arbitrary {@link SessionInputBuffer} and {@link SessionOutputBuffer}
 *
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 *
 * @version $Revision: 618017 $
 *
 * @since 4.0
 */
class AbstractHttpServerConnection
    : public Object
    , public IHttpServerConnection
    , public IHttpConnection
{
public:
    AbstractHttpServerConnection();

    virtual ~AbstractHttpServerConnection() = 0;

    CAR_INTERFACE_DECL()

    CARAPI ReceiveRequestHeader(
        /* [out] */ IHttpRequest** request);

    CARAPI ReceiveRequestEntity(
        /* [in] */ IHttpEntityEnclosingRequest* request);

    CARAPI Flush();

    CARAPI SendResponseHeader(
        /* [in] */ IHttpResponse* response);

    CARAPI SendResponseEntity(
        /* [in] */ IHttpResponse* response);

    CARAPI IsStale(
        /* [out] */ Boolean* isStale);

    CARAPI GetMetrics(
        /* [out] */ IHttpConnectionMetrics** metrics);

protected:
    virtual CARAPI AssertOpen() = 0;

    CARAPI_(AutoPtr<EntityDeserializer>) CreateEntityDeserializer();

    CARAPI_(AutoPtr<EntitySerializer>) CreateEntitySerializer();

    CARAPI_(AutoPtr<IHttpRequestFactory>) CreateHttpRequestFactory();

    CARAPI_(AutoPtr<IHttpMessageParser>) CreateRequestParser(
        /* [in] */ ISessionInputBuffer* buffer,
        /* [in] */ IHttpRequestFactory* requestFactory,
        /* [in] */ IHttpParams* params);

    CARAPI_(AutoPtr<IHttpMessageWriter>) CreateResponseWriter(
        /* [in] */ ISessionOutputBuffer* buffer,
        /* [in] */ IHttpParams* params);

    CARAPI Init(
        /* [in] */ ISessionInputBuffer* inbuffer,
        /* [in] */ ISessionOutputBuffer* outbuffer,
        /* [in] */ IHttpParams* params);

    CARAPI DoFlush();

private:
    AutoPtr<EntitySerializer> mEntityserializer;
    AutoPtr<EntityDeserializer> mEntitydeserializer;

    AutoPtr<ISessionInputBuffer> mInbuffer;
    AutoPtr<ISessionOutputBuffer> mOutbuffer;
    AutoPtr<IHttpMessageParser> mRequestParser;
    AutoPtr<IHttpMessageWriter> mResponseWriter;
    AutoPtr<HttpConnectionMetricsImpl> mMetrics;
};

} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_IMPL_ABSTRACTHTTPSERVERCONNECTION_H__
