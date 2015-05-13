
#include "ext/frameworkext.h"
#include "webkit/CPluginData.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CPluginData::constructor(
    /* [in] */ IInputStream* stream,
    /* [in] */ Int64 length,
    /* [in] */ IMap* headers,
    /* [in] */ Int32 code)
{
    mStream = stream;
    mContentLength = length;
    mHeaders = headers;
    mStatusCode = code;
    return NOERROR;
}

ECode CPluginData::GetInputStream(
    /* [out] */ IInputStream** stream)
{
    VALIDATE_NOT_NULL(stream);
    *stream = mStream;
    INTERFACE_ADDREF(*stream);
    return NOERROR;
}

ECode CPluginData::GetContentLength(
    /* [out] */ Int64* length)
{
    VALIDATE_NOT_NULL(length);
    *length = mContentLength;
    return NOERROR;
}

ECode CPluginData::GetHeaders(
    /* [out] */ IMap** headers)
{
    VALIDATE_NOT_NULL(headers);
    *headers = mHeaders;
    INTERFACE_ADDREF(*headers);
    return NOERROR;
}

ECode CPluginData::GetStatusCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code);
    *code = mStatusCode;
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos