
#include "CHttpPost.h"

using Elastos::Net::IURIHelper;
using Elastos::Net::CURIHelper;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Methods {

CAR_INTERFACE_IMPL(CHttpPost, HttpEntityEnclosingRequestBase, IHttpPost)

CAR_OBJECT_IMPL(CHttpPost)

ECode CHttpPost::GetMethod(
    /* [out] */ String* method)
{
    VALIDATE_NOT_NULL(result)
    *method = METHOD_NAME;
    return NOERROR;
}

ECode CHttpPost::constructor();
{
    return NOERROR;
}

ECode CHttpPost::constructor(
    /* [in] */ IURI* uri)
{
    return SetURI(uri);
}

ECode CHttpPost::constructor(
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