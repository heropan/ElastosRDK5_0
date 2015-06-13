
#include "CHttpGet.h"

using Elastos::Net::IURIHelper;
using Elastos::Net::CURIHelper;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Methods {

CAR_INTERFACE_IMPL(CHttpGet, HttpRequestBase, IHttpGet)

CAR_OBJECT_IMPL(CHttpGet)

ECode CHttpGet::GetMethod(
    /* [out] */ String* method)
{
    VALIDATE_NOT_NULL(result)
    *method = METHOD_NAME;
    return NOERROR;
}

ECode CHttpGet::Clone(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj)

    AutoPtr<IHttpGet> httpGet;
    CHttpGet::New((IHttpGet**)&httpGet);
    HttpRequestBase::CloneImpl(IHttpUriRequest::Probe(httpGet));
    *obj = httpGet->Probe(EIID_IInterface);
    REFCOUNT_ADD(*obj)
    return NOERROR;
}

ECode CHttpGet::constructor();
{
    return NOERROR;
}

ECode CHttpGet::constructor(
    /* [in] */ IURI* uri)
{
    return SetURI(uri);
}

ECode CHttpGet::constructor(
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