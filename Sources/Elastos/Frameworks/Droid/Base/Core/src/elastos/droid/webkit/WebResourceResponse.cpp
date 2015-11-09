
#include "elastos/droid/webkit/WebResourceResponse.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CAR_INTERFACE_IMPL(WebResourceResponse, Object, IWebResourceResponse);

WebResourceResponse::WebResourceResponse()
{
}

ECode WebResourceResponse::constructor(
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding,
    /* [in] */ IInputStream* data)
{
    mMimeType = mimeType;
    mEncoding = encoding;
    mInputStream = data;
    return NOERROR;
}

ECode WebResourceResponse::constructor(
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding,
    /* [in] */ Int32 statusCode,
    /* [in] */ const String& reasonPhrase,
    /* [in] */ IMap* responseHeaders,
    /* [in] */ IInputStream* data)
{
    constructor(mimeType, encoding, data);
    SetStatusCodeAndReasonPhrase(statusCode, reasonPhrase);
    SetResponseHeaders(responseHeaders);
    return NOERROR;
}

/**
 * Sets the resource response's MIME type, for example text/html.
 *
 * @param mimeType the resource response's MIME type
 */
ECode WebResourceResponse::SetMimeType(
    /* [in] */ const String& mimeType)
{
    mMimeType = mimeType;
    return NOERROR;
}

/**
 * Gets the resource response's MIME type.
 *
 * @return the resource response's MIME type
 */
ECode WebResourceResponse::GetMimeType(
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mMimeType;
    return NOERROR;
}

/**
 * Sets the resource response's encoding, for example UTF-8. This is used
 * to decode the data from the input stream.
 *
 * @param encoding the resource response's encoding
 */
ECode WebResourceResponse::SetEncoding(
    /* [in] */ const String& encoding)
{
    mEncoding = encoding;
    return NOERROR;
}

/**
 * Gets the resource response's encoding.
 *
 * @return the resource response's encoding
 */
ECode WebResourceResponse::GetEncoding(
    /* [out] */ String* encoding)
{
    VALIDATE_NOT_NULL(encoding);
    *encoding = mEncoding;
    return NOERROR;
}

ECode WebResourceResponse::SetStatusCodeAndReasonPhrase(
    /* [in] */ Int32 statusCode,
    /* [in] */ const String& reasonPhrase)
{
    assert(0);
    // TODO
    return E_NOT_IMPLEMENTED;
}

ECode WebResourceResponse::GetStatusCode(
    /* [out] */ Int32* statusCode)
{
    assert(0);
    // TODO
    return E_NOT_IMPLEMENTED;
}

/**
 * Gets the description of the resource response's status code.
 *
 * @return the description of the resource response's status code.
 */
ECode WebResourceResponse::GetReasonPhrase(
    /* [out] */ String* reasonPhrase)
{
    assert(0);
    // TODO
    return E_NOT_IMPLEMENTED;
}

ECode WebResourceResponse::SetResponseHeaders(
    /* [in] */ IMap* headers)
{
    assert(0);
    // TODO
    return E_NOT_IMPLEMENTED;
}

/**
 * Gets the headers for the resource response.
 *
 * @return the headers for the resource response.
 */
ECode WebResourceResponse::GetResponseHeaders(
    /* [out] */ IMap** headers)
{
    assert(0);
    // TODO
    return E_NOT_IMPLEMENTED;
}

/**
 * Sets the input stream that provides the resource respone's data. Callers
 * must implement {@link InputStream#read(byte[]) InputStream.read(byte[])}.
 *
 * @param data the input stream that provides the resource response's data
 */
ECode WebResourceResponse::SetData(
    /* [in] */ IInputStream* data)
{
    mInputStream = data;
    return NOERROR;
}

/**
 * Gets the input stream that provides the resource respone's data.
 *
 * @return the input stream that provides the resource response's data
 */
ECode WebResourceResponse::GetData(
    /* [out] */ IInputStream** data)
{
    VALIDATE_NOT_NULL(data);
    *data = mInputStream;
    REFCOUNT_ADD(*data);
    return NOERROR;
}

ECode WebResourceResponse::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "WebResourceResponse";
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos