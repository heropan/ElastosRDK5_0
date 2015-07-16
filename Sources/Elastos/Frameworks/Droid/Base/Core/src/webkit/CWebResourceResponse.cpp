
#include "webkit/CWebResourceResponse.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

struct ElaWebResourceResponseCallback CWebResourceResponse::sWebResourceResponseCallback =
{
    &CWebResourceResponse::GetMimeType,
    &CWebResourceResponse::GetEncoding,
    &CWebResourceResponse::GetData,
};

ECode CWebResourceResponse::constructor(
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding,
    /* [in] */ IInputStream* data)
{
    return WebResourceResponse::Init(mimeType, encoding, data);
}

ECode CWebResourceResponse::SetMimeType(
    /* [in] */ const String& mimeType)
{
    return WebResourceResponse::SetMimeType(mimeType);
}

ECode CWebResourceResponse::GetMimeType(
    /* [out] */ String* mimeType)
{
    VALIDATE_NOT_NULL(mimeType);
    *mimeType = WebResourceResponse::GetMimeType();
    return NOERROR;
}

ECode CWebResourceResponse::SetEncoding(
    /* [in] */ const String& encoding)
{
    return WebResourceResponse::SetEncoding(encoding);
}

ECode CWebResourceResponse::GetEncoding(
    /* [out] */ String* encoding)
{
    VALIDATE_NOT_NULL(encoding);
    *encoding = WebResourceResponse::GetEncoding();
    return NOERROR;
}

ECode CWebResourceResponse::SetData(
    /* [in] */ IInputStream* data)
{
    return WebResourceResponse::SetData(data);
}

ECode CWebResourceResponse::GetData(
    /* [out] */ IInputStream** inputStream)
{
    VALIDATE_NOT_NULL(inputStream);
    AutoPtr<IInputStream> _inputStream = WebResourceResponse::GetData();
    *inputStream = _inputStream.Get();
    REFCOUNT_ADD(*inputStream);
    return NOERROR;
}

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
