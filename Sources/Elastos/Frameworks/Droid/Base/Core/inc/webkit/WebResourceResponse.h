
#ifndef __ELASTOS_DROID_WEBKIT_WEBRESOURCERESPONSE_H__
#define __ELASTOS_DROID_WEBKIT_WEBRESOURCERESPONSE_H__

#include "ext/frameworkext.h"

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
{
public:
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
