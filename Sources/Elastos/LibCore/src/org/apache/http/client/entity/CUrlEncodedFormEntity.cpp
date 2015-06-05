
#include "CUrlEncodedFormEntity.h"
#include "utils/URLEncodedUtils.h"

using Org::Apache::Http::Client::Utils::URLEncodedUtils;
using Org::Apache::Http::Client::Utils::IURLEncodedUtils;
using Org::Apache::Http::Protocol::IHTTP;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Entity {

CAR_OBJECT_IMPL(CUrlEncodedFormEntity)

ECode CUrlEncodedFormEntity::constructor(
    /* [in] */ IList* parameters,
    /* [in] */ const String& encoding)
{
    FAIL_RETURN(Init(URLEncodedUtils::Format(parameters, encoding), encoding))
    return SetContentType(IURLEncodedUtils::CONTENT_TYPE);
}

ECode CUrlEncodedFormEntity::constructor(
    /* [in] */ IList* parameters)
{
    FAIL_RETURN(Init(URLEncodedUtils::Format(parameters, IHTTP::DEFAULT_CONTENT_CHARSET), IHTTP::DEFAULT_CONTENT_CHARSET))
    return SetContentType(IURLEncodedUtils::CONTENT_TYPE);
}

} // namespace Entity
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org
