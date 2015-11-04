
#include "elastos/droid/webkit/CWebResourceResponse.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CAR_OBJECT_IMPL(CWebResourceResponse);

struct ElaWebResourceResponseCallback CWebResourceResponse::sWebResourceResponseCallback =
{
    &CWebResourceResponse::GetMimeType,
    &CWebResourceResponse::GetEncoding,
    &CWebResourceResponse::GetData,
};

String CWebResourceResponse::GetMimeType(
    /* [in] */ IInterface* obj)
{
    String result;
    IWebResourceResponse::Probe(obj)->GetMimeType(&result);
    return result;
}

String CWebResourceResponse::GetEncoding(
    /* [in] */ IInterface* obj)
{
    String result;
    IWebResourceResponse::Probe(obj)->GetEncoding(&result);
    return result;
}

AutoPtr<IInterface> CWebResourceResponse::GetData(
    /* [in] */ IInterface* obj)
{
    AutoPtr<IInputStream> inputStream;
    IWebResourceResponse::Probe(obj)->GetData((IInputStream**)&inputStream);
    return inputStream;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
