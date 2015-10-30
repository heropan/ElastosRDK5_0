#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWWEBRESOURCERESPONSE_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWWEBRESOURCERESPONSE_H__
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/Map.h>

using Elastos::IO::IInputStream;
using Elastos::Utility::Etl::Map;


// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

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
    : public Object
{
public:
    AwWebResourceResponse(
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding,
        /* [in] */ IInputStream* data);

    AwWebResourceResponse(
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding,
        /* [in] */ IInputStream* data,
        /* [in] */ Int32 statusCode,
        /* [in] */ const String& reasonPhrase,
        /* [in] */ Map<String, String> &responseHeaders);

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
    virtual CARAPI_(AutoPtr<ArrayOf<String> >) GetResponseHeaderNames();

    //@CalledByNative
    virtual CARAPI_(AutoPtr<ArrayOf<String> >) GetResponseHeaderValues();

//callback function declaration
public:
    static CARAPI_(void*) ElaAwWebResourceResponseCallback_Init();

private:
    static CARAPI_(String) GetMimeType(
        /* [in] */ IInterface* obj);

    static CARAPI_(String) GetCharset(
        /* [in] */ IInterface* obj);

    static CARAPI_(AutoPtr<IInterface>) GetData(
        /* [in] */ IInterface* obj);

    static CARAPI_(Int32) GetStatusCode(
        /* [in] */ IInterface* obj);

    static CARAPI_(String) GetReasonPhrase(
        /* [in] */ IInterface* obj);

    static CARAPI_(AutoPtr<ArrayOf<String> >) GetResponseHeaderNames(
        /* [in] */ IInterface* obj);

    static CARAPI_(AutoPtr<ArrayOf<String> >) GetResponseHeaderValues(
        /* [in] */ IInterface* obj);

private:
    String mMimeType;
    String mCharset;
    AutoPtr<IInputStream> mData;
    Int32 mStatusCode;
    String mReasonPhrase;
    AutoPtr<ArrayOf<String> > mResponseHeaderNames;
    AutoPtr<ArrayOf<String> > mResponseHeaderValues;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWWEBRESOURCERESPONSE_H__
