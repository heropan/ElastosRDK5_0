
#ifndef __ELASTOS_DROID_WEBKIT_WEBRESOURCERESPONSE_H__
#define __ELASTOS_DROID_WEBKIT_WEBRESOURCERESPONSE_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Utility::IMap;
using Elastos::IO::IInputStream;

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Encapsulates a resource response. Applications can return an instance of this
 * class from {@link WebViewClient#shouldInterceptRequest} to provide a custom
 * response when the WebView requests a particular resource.
 */
class WebResourceResponse
    : public Object
    , public IWebResourceResponse
{
public:
    CAR_INTERFACE_DECL()

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
    WebResourceResponse(
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding,
        /* [in] */ IInputStream* data);

    WebResourceResponse(
        /* [in] */ String mimeType,
        /* [in] */ String encoding,
        /* [in] */ Int32 statusCode,
        /* [in] */ String reasonPhrase,
        /* [in] */ IMap* responseHeaders,
        /* [in] */ IInputStream* data);

    CARAPI constructor(
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding,
        /* [in] */ IInputStream* data);

    CARAPI constructor(
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding,
        /* [in] */ Int32 statusCode,
        /* [in] */ const String& reasonPhrase,
        /* [in] */ IMap* responseHeaders,
        /* [in] */ IInputStream* data);

    /**
     * Sets the resource response's MIME type, for example text/html.
     *
     * @param mimeType the resource response's MIME type
     */
    virtual CARAPI SetMimeType(
        /* [in] */ const String& mimeType);

    /**
     * Gets the resource response's MIME type.
     *
     * @return the resource response's MIME type
     */
    virtual CARAPI_(String) GetMimeType();

    /**
     * Sets the resource response's encoding, for example UTF-8. This is used
     * to decode the data from the input stream.
     *
     * @param encoding the resource response's encoding
     */
    virtual CARAPI SetEncoding(
        /* [in] */ const String& encoding);

    /**
     * Gets the resource response's encoding.
     *
     * @return the resource response's encoding
     */
    virtual CARAPI_(String) GetEncoding();

    /**
     * Sets the resource response's status code and reason phrase.
     *
     * @param statusCode the status code needs to be in the ranges [100, 299], [400, 599].
     *                   Causing a redirect by specifying a 3xx code is not supported.
     * @param reasonPhrase the phrase describing the status code, for example "OK". Must be non-null
     *                     and not empty.
     */
    virtual CARAPI SetStatusCodeAndReasonPhrase(
        /* [in] */ Int32 statusCode,
        /* [in] */ const String& reasonPhrase);

    /**
     * Gets the resource response's status code.
     *
     * @return the resource response's status code.
     */
    virtual CARAPI GetStatusCode(
        /* [out] */ Int32* statusCode);

    /**
     * Gets the description of the resource response's status code.
     *
     * @return the description of the resource response's status code.
     */
    virtual CARAPI GetReasonPhrase(
        /* [out] */ String* reasonPhrase);

    /**
     * Sets the headers for the resource response.
     *
     * @param headers mapping of header name -> header value.
     */
    virtual CARAPI SetResponseHeaders(
        /* [in] */ IMap* headers);

    /**
     * Gets the headers for the resource response.
     *
     * @return the headers for the resource response.
     */
    virtual CARAPI GetResponseHeaders(
        /* [out] */ IMap** headers);

    /**
     * Sets the input stream that provides the resource respone's data. Callers
     * must implement {@link InputStream#read(byte[]) InputStream.read(byte[])}.
     *
     * @param data the input stream that provides the resource response's data
     */
    virtual CARAPI SetData(
        /* [in] */ IInputStream* data);

    /**
     * Gets the input stream that provides the resource respone's data.
     *
     * @return the input stream that provides the resource response's data
     */
    virtual CARAPI_(AutoPtr<IInputStream>) GetData();

    CARAPI ToString(
        /* [out] */ String* info);

protected:
    WebResourceResponse();

    CARAPI Init(
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding,
        /* [in] */ IInputStream* data);

protected:
    // Accessed by jni, do not rename without modifying the jni code.
    String mMimeType;
    String mEncoding;
    AutoPtr<IInputStream> mInputStream;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBRESOURCERESPONSE_H__
