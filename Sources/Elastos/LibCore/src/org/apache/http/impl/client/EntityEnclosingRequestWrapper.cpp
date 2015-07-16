
#include "EntityEnclosingRequestWrapper.h"

using Org::Apache::Http::IHeader;
using Org::Apache::Http::Protocol::IHTTP;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Client {

EntityEnclosingRequestWrapper::EntityEnclosingRequestWrapper(
    /* [in] */ IHttpEntityEnclosingRequest* request)
    : RequestWrapper(IHttpRequest::Probe(request))
    , mOriginal(request)
    , mExecCount(0)
{
    request->GetEntity((IHttpEntity**)&mEntity);
}

CAR_INTERFACE_IMPL(EntityEnclosingRequestWrapper, RequestWrapper, IHttpEntityEnclosingRequest)

ECode EntityEnclosingRequestWrapper::SetEntity(
    /* [in] */ IHttpEntity* entity)
{
    mEntity = entity;
    return NOERROR;
}

ECode EntityEnclosingRequestWrapper::GetEntity(
    /* [out] */ IHttpEntity** entity)
{
    VALIDATE_NOT_NULL(entity)
    *entity = mEntity;
    REFCOUNT_ADD(*entity)
    return NOERROR;
}

ECode EntityEnclosingRequestWrapper::ExpectContinue(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    AutoPtr<IHeader> expect;
    GetFirstHeader(IHTTP::EXPECT_DIRECTIVE, (IHeader**)&expect);
    if (expect != NULL) {
        String value;
        expect->GetValue(&value);
        *result = IHTTP::EXPECT_CONTINUE.EqualsIgnoreCase(value);
    }
    return NOERROR;
}

Boolean EntityEnclosingRequestWrapper::IsRepeatable()
{
    if (mEntity == NULL) {
        return TRUE;
    }
    else (mEntity != NULL) {
        Boolean isRepeatable;
        mEntity->IsRepeatable(&isRepeatable);
        return isRepeatable;
    }
}

} // namespace Client
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org