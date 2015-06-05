
#include "CHttpDelete.h"

using Elastos::Net::IURIHelper;
using Elastos::Net::CURIHelper;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Methods {

CAR_INTERFACE_IMPL(CHttpDelete, HttpRequestBase, IHttpDelete)

CAR_OBJECT_IMPL(CHttpDelete)

ECode CHttpDelete::GetMethod(
    /* [out] */ String* method)
{
    VALIDATE_NOT_NULL(result)
    *method = METHOD_NAME;
    return NOERROR;
}

ECode CHttpDelete::constructor();
{
    return NOERROR;
}

ECode CHttpDelete::constructor(
    /* [in] */ IURI* uri)
{
    return SetURI(uri);
}

ECode CHttpDelete::constructor(
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