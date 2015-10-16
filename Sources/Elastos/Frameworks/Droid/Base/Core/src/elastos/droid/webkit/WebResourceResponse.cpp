
#include "elastos/droid/webkit/WebResourceResponse.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Constructs a resource response with the given MIME type, encoding, and
 * input stream. Callers must implement
 * {@link InputStream#read(byte[]) InputStream.read(byte[])} for the input
 * stream.
 *
 * @param mimeType the resource response's MIME type, for example text/html
 * @param encoding the resource response's encoding
 * @param data the input stream that provides the resource response's data
 */
WebResourceResponse::WebResourceResponse(
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding,
    /* [in] */ IInputStream* data)
{
    Init(mimeType, encoding, data);
}

WebResourceResponse::WebResourceResponse()
{
}

ECode WebResourceResponse::Init(
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding,
    /* [in] */ IInputStream* data)
{
    mMimeType = mimeType;
    mEncoding = encoding;
    mInputStream = data;
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
String WebResourceResponse::GetMimeType()
{
    return mMimeType;
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
String WebResourceResponse::GetEncoding()
{
    return mEncoding;
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
AutoPtr<IInputStream> WebResourceResponse::GetData()
{
    return mInputStream;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos