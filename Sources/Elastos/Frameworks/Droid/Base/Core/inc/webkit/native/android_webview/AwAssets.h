
#ifndef __AWASSETS_H__
#define __AWASSETS_H__

// import android.content.Context;
// import android.content.res.AssetFileDescriptor;
// import android.content.res.AssetManager;
// import android.util.Log;

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
    static CARAPI_(AutoPtr< ArrayOf<Int64> >) OpenAsset(
        /* [in] */ IContext* context,
        /* [in] */ String fileName);

private:
    static const String LOGTAG;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__AWASSETS_H__
