
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
    if (statusCode < 100) {
        assert(0);
        //throw new IllegalArgumentException("statusCode can't be less than 100.");
    }
    if (statusCode > 599) {
        assert(0);
        //throw new IllegalArgumentException("statusCode can't be greater than 599.");
    }
    if (statusCode > 299 && statusCode < 400) {
        assert(0);
        //throw new IllegalArgumentException("statusCode can't be in the [300, 399] range.");
    }
    if (reasonPhrase == NULL) {
        assert(0);
        //throw new IllegalArgumentException("reasonPhrase can't be null.");
    }
    if (reasonPhrase.Trim().IsEmpty()) {
        assert(0);
        //throw new IllegalArgumentException("reasonPhrase can't be empty.");
    }

    Int32 length = reasonPhrase.GetLength();
    for (Int32 i = 0; i < length; i++) {
        Int32 c = reasonPhrase.GetChar(i);
        if (c > 0x7F) {
            assert(0);
            // throw new IllegalArgumentException(
            //         "reasonPhrase can't contain non-ASCII characters.");
        }
    }

    mStatusCode = statusCode;
    mReasonPhrase = reasonPhrase;

    return NOERROR;
}

ECode WebResourceResponse::GetStatusCode(
    /* [out] */ Int32* statusCode)
{
    VALIDATE_NOT_NULL(statusCode);
    *statusCode = mStatusCode;
    return NOERROR;
}

/**
 * Gets the description of the resource response's status code.
 *
 * @return the description of the resource response's status code.
 */
ECode WebResourceResponse::GetReasonPhrase(
    /* [out] */ String* reasonPhrase)
{
    VALIDATE_NOT_NULL(reasonPhrase);
    *reasonPhrase = mReasonPhrase;
    return NOERROR;
}

ECode WebResourceResponse::SetResponseHeaders(
    /* [in] */ IMap* headers)
{
    mResponseHeaders = headers;
    return NOERROR;
}

/**
 * Gets the headers for the resource response.
 *
 * @return the headers for the resource response.
 */
ECode WebResourceResponse::GetResponseHeaders(
    /* [out] */ IMap** headers)
{
    VALIDATE_NOT_NULL(headers);
    *headers = mResponseHeaders;
    return NOERROR;
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