
#include "CHttpTrace.h"

using Elastos::Net::IURIHelper;
using Elastos::Net::CURIHelper;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Methods {

CAR_INTERFACE_IMPL(CHttpTrace, HttpRequestBase, IHttpTrace)

CAR_OBJECT_IMPL(CHttpTrace)

ECode CHttpTrace::GetMethod(
    /* [out] */ String* method)
{
    VALIDATE_NOT_NULL(result)
    *method = METHOD_NAME;
    return NOERROR;
}

ECode CHttpTrace::Clone(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj)

    AutoPtr<IHttpTrace> httpTrace;
    CHttpTrace::New((IHttpTrace**)&httpTrace);
    HttpRequestBase::CloneImpl(IHttpUriRequest::Probe(httpTrace));
    *obj = httpTrace->Probe(EIID_IInterface);
    REFCOUNT_ADD(*obj)
    return NOERROR;
}

ECode CHttpTrace::constructor();
{
    return NOERROR;
}

ECode CHttpTrace::constructor(
    /* [in] */ IURI* uri)
{
    return SetURI(uri);
}

ECode CHttpTrace::constructor(
    /* [in] */ const String& uri)
{
    AutoPtr<IURIHelper> helper;
    CURIHelper::AcquireSingleton((IURIHelper**)&helper);
    AutoPtr<IURI> iuri;
    helper->Create(uri, (IURI**)&iuri);
    return SetURI(iuri);
}

} // namespace Methods
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org