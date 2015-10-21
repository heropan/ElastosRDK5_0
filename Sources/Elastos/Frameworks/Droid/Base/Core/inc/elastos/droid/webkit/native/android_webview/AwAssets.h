#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWASSETS_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWASSETS_H__
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Content::IContext;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

// import java.io.IOException;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * A utility class to retrieve references to uncompressed assets insides the apk. A reference is
 * defined as tuple (file descriptor, offset, size) enabling direct mapping without deflation.
 */
//@JNINamespace("android_webview")
class AwAssets
{
public:
    //@CalledByNative
    static CARAPI_(AutoPtr<ArrayOf<Int64> >) OpenAsset(
        /* [in] */ IContext* context,
        /* [in] */ const String& fileName);

private:
    static const String LOGTAG;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWASSETS_H__
