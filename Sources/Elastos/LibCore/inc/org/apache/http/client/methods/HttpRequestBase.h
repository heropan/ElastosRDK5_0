
#ifndef __ORG_APACHE_HTTP_CLIENT_METHODS_HTTPREQUESTBASE_H__
#define __ORG_APACHE_HTTP_CLIENT_METHODS_HTTPREQUESTBASE_H__

#include "AbstractHttpMessage.h"

using Elastos::Core::ICloneable;
using Elastos::Net::IURI;
using Elastos::Utility::Concurrent::Locks::ILock;
using Org::Apache::Http::IProtocolVersion;
using Org::Apache::Http::Message::AbstractHttpMessage;
using Org::Apache::Http::Conn::IClientConnectionRequest;
using Org::Apache::Http::Conn::IConnectionReleaseTrigger;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Methods {

/**
 * Basic implementation of an HTTP request that can be modified.
 *
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 *
 * @version $Revision: 674186 $
 *
 * @since 4.0
 */
class HttpRequestBase
    : public AbstractHttpMessage
    , public IHttpRequest
    , public IHttpUriRequest
    , public IAbortableHttpRequest
    , public ICloneable
{
public:
    HttpRequestBase();

    virtual ~HttpRequestBase() = 0;

    CAR_INTERFACE_DECL()

    virtual CARAPI GetMethod(
        /* [out] */ String* method) = 0;

    CARAPI GetProtocolVersion(
        /* [out] */ IProtocolVersion** ver);

    CARAPI GetURI(
        /* [out] */ IURI** URI);

    CARAPI GetRequestLine(
        /* [out] */ IRequestLine** line);

    CARAPI SetURI(
        /* [in] */ IURI* uri);

    CARAPI SetConnectionRequest(
        /* [in] */ IClientConnectionRequest* connRequest);

    CARAPI SetReleaseTrigger(
        /* [in] */ IConnectionReleaseTrigger* releaseTrigger);

    CARAPI Abort();

    CARAPI IsAborted(
        /* [out] */ Boolean* isAborted);

protected:
    virtual CARAPI CloneImpl(
        /* [in] */ HttpRequestBase* clone);

private:
    AutoPtr<ILock> mAbortLock;

    Boolean mAborted;

    AutoPtr<IURI> mUri;
    AutoPtr<IClientConnectionRequest> mConnRequest;
    AutoPtr<IConnectionReleaseTrigger> mReleaseTrigger;

};

} // namespace Methods
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_CLIENT_METHODS_HTTPREQUESTBASE_H__
