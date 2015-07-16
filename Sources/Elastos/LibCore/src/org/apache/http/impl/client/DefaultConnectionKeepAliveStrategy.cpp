
#include "DefaultConnectionKeepAliveStrategy.h"
#include "CBasicHeaderElementIterator.h"
#include <elastos/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHeaderIterator;
using Org::Apache::Http::IHeaderElementIterator;
using Org::Apache::Http::IHttpMessage;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Client {

CAR_INTERFACE_DECL(DefaultConnectionKeepAliveStrategy, Object, IConnectionKeepAliveStrategy)

ECode DefaultConnectionKeepAliveStrategy::GetKeepAliveDuration(
    /* [in] */ IHttpResponse* response,
    /* [in] */ IHttpContext* context,
    /* [out] */ Int64* duration)
{
    VALIDATE_NOT_NULL(duration)
    *duration = -1;

    if (response == NULL) {
        Logger::E("DefaultConnectionKeepAliveStrategy", "HTTP response may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IHttpMessage> message = IHttpMessage::Probe(response);
    AutoPtr<IHeaderIterator> hit;
    message->HeaderIterator(IHTTP::CONN_KEEP_ALIVE, (IHeaderIterator**)&hit);
    AutoPtr<IHeaderElementIterator> it;
    CBasicHeaderElementIterator::New(hit, (IHeaderElementIterator**)&it);
    Boolean hasNext;
    while (it->HasNext(&hasNext), hasNext) {
        AutoPtr<IHeaderElement> he;
        it->NextElement((IHeaderElement**)&he);
        String param, value;
        he->GetName(&param);
        he->GetValue(&value);
        if (!value.IsNull() && param.EqualsIgnoreCase(String("timeout"))) {
            // try {
            *duration = StringUtils::ParseInt64(value) * 1000;
            return NOERROR;
            // } catch(NumberFormatException ignore) {
            // }
        }
    }
    *duration = -1;
    return NOERROR;
}

} // namespace Client
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org