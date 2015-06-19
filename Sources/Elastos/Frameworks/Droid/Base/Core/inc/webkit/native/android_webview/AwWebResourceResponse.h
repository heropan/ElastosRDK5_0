
#ifndef __AWWEBRESOURCERESPONSE_H__
#define __AWWEBRESOURCERESPONSE_H__

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

// import java.io.InputStream;
// import java.util.Map;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * The response information that is to be returned for a particular resource fetch.
 */
//@JNINamespace("android_webview")
class AwWebResourceResponse
{
public:
    AwWebResourceResponse(
        /* [in] */ String mimeType,
        /* [in] */ String encoding,
        /* [in] */ InputStream data);

    AwWebResourceResponse(
        /* [in] */ String mimeType,
        /* [in] */ String encoding,
        /* [in] */ IInputStream* data,
        /* [in] */ Int32 statusCode,
        /* [in] */ String reasonPhrase,
        /* [in] */ Map<String, String> responseHeaders);

    //@CalledByNative
    virtual CARAPI_(String) GetMimeType();

    //@CalledByNative
    virtual CARAPI_(String) GetCharset();

    //@CalledByNative
    virtual CARAPI_(AutoPtr<IInputStream>) GetData();

    //@CalledByNative
    virtual CARAPI_(Int32) GetStatusCode();

    //@CalledByNative
    virtual CARAPI_(String) GetReasonPhrase();

    //@CalledByNative
    virtual CARAPI_(AutoPtr< ArrayOf<String> >) GetResponseHeaderNames();

    //@CalledByNative
    virtual CARAPI_(AutoPtr< ArrayOf<String> >) GetResponseHeaderValues();

private:
    String mMimeType;
    String mCharset;
    AutoPtr<IInputStream> mData;
    Int32 mStatusCode;
    String mReasonPhrase;
    AutoPtr< ArrayOf<String> > mResponseHeaderNames;
    AutoPtr< ArrayOf<String> > mResponseHeaderValues;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__AWWEBRESOURCERESPONSE_H__
