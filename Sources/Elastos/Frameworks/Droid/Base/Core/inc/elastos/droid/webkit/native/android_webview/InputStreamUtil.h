#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_INPUTSTREAMUTIL_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_INPUTSTREAMUTIL_H__
#include "elastos/droid/ext/frameworkext.h"

using Elastos::IO::IInputStream;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

// import java.io.IOException;
// import java.io.InputStream;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * Utility methods for calling InputStream methods. These take care of exception handling.
 */
//@JNINamespace("android_webview")
class InputStreamUtil
:public Object
{
public:
    //@CalledByNative
    static CARAPI_(void) Close(
        /* [in] */ IInputStream* stream);

    //@CalledByNative
    static CARAPI_(Int32) Available(
        /* [in] */ IInputStream* stream);

    //@CalledByNative
    static CARAPI_(Int32) Read(
        /* [in] */ IInputStream* stream,
        /* [in] */ ArrayOf<Byte>* b,
        /* [in] */ Int32 off,
        /* [in] */ Int32 len);

    //@CalledByNative
    static CARAPI_(Int64) Skip(
        /* [in] */ IInputStream* stream,
        /* [in] */ Int64 n);

private:
    static const String LOGTAG;
    // The InputStream APIs return -1 in some cases. In order to convey the extra information that
    // the call had failed due to an exception being thrown we simply map all negative return values
    // from the original calls to -1 and make -2 mean that an exception has been thrown.
    static const Int32 CALL_FAILED_STATUS = -1;
    static const Int32 EXCEPTION_THROWN_STATUS = -2;

    static CARAPI_(String) LogMessage(
        /* [in] */ const String& method);
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_INPUTSTREAMUTIL_H__
