
#include "CHttpHead.h"

using Elastos::Net::IURIHelper;
using Elastos::Net::CURIHelper;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Methods {

CAR_INTERFACE_IMPL(CHttpHead, HttpRequestBase, IHttpHead)

CAR_OBJECT_IMPL(CHttpHead)

ECode CHttpHead::GetMethod(
    /* [out] */ String* method)
{
    VALIDATE_NOT_NULL(result)
    *method = METHOD_NAME;
    return NOERROR;
}

ECode CHttpHead::Clone(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj)

    AutoPtr<IHttpHead> httpHead;
    CHttpHead::New((IHttpHead**)&httpHead);
    HttpRequestBase::CloneImpl(IHttpUriRequest::Probe(httpHead));
    *obj = httpHead->Probe(EIID_IInterface);
    REFCOUNT_ADD(*obj)
    return NOERROR;
}

ECode CHttpHead::constructor();
{
    return NOERROR;
}

ECode CHttpHead::constructor(
    /* [in] */ IURI* uri)
{
    return SetURI(uri);
}

ECode CHttpHead::constructor(
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