
#include "BasicResponseHandler.h"
#include "EntityUtils.h"
#include <elastos/Logger.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IStatusLine;
using Org::Apache::Http::IHttpEntity;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Client {

CAR_INTERFACE_DECL(BasicResponseHandler, Object, IResponseHandler)

ECode BasicResponseHandler::HandleResponse(
    /* [in] */ IHttpResponse* response,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;
    AutoPtr<IStatusLine> statusLine;
    response->GetStatusLine((IStatusLine**)&statusLine);
    Int32 code;
    if (statusLine->GetStatusCode(&code), code >= 300) {
        String phrase;
        statusLine->GetreasonPhrase(&phrase);
        Logger::E("BasicResponseHandler", "%d, %s", code, phrase.string());
        return E_HTTP_RESPONSE_EXCEPTION;
    }

    AutoPtr<IHttpEntity> entity;
    response->GetEntity((IHttpEntity**)&entity);
    if (entity != NULL) {
        String str;
        EntityUtils::ToString(entity, &str);
        AutoPtr<ICharSequence> cs;
        CString::New(str, (ICharSequence**)&cs);
        *value = (IInterface*)cs.Get();
        REFCOUNT_ADD(*value)
    }
    return NOERROR;
}

} // namespace Client
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org