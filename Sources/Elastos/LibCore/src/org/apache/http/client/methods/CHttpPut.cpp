
#include "CHttpPut.h"

using Elastos::Net::IURIHelper;
using Elastos::Net::CURIHelper;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Methods {

CAR_INTERFACE_IMPL(CHttpPut, HttpEntityEnclosingRequestBase, IHttpPut)

CAR_OBJECT_IMPL(CHttpPut)

ECode CHttpPut::GetMethod(
    /* [out] */ String* method)
{
    VALIDATE_NOT_NULL(result)
    *method = METHOD_NAME;
    return NOERROR;
}

ECode CHttpPut::constructor();
{
    return NOERROR;
}

ECode CHttpPut::constructor(
    /* [in] */ IURI* uri)
{
    return SetURI(uri);
}

ECode CHttpPut::constructor(
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