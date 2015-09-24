
#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_PERMISSION_AWPERMISSIONREQUEST_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_PERMISSION_AWPERMISSIONREQUEST_H__

// import android.net.Uri;
// import android.util.Log;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.base.ThreadUtils;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {
namespace Permission {

/**
 * This class wraps permission request in Chromium side, and can only be created
 * by native side.
 */
//@JNINamespace("android_webview")
public class AwPermissionRequest
{
public:
    CARAPI_(AutoPtr<IUri>) GetOrigin();

    CARAPI_(Int64) GetResources();

    CARAPI_(void) Grant();

    CARAPI_(void) Deny();

protected:
    //@Override
    //protected void finalize();
    ~AwPermissionRequest();

private:
    AwPermissionRequest(
        /* [in] */ Int64 nativeAwPermissionRequest,
        /* [in] */ IUri* origin,
        /* [in] */ Int64 resources);

    //@CalledByNative
    static CARAPI_(AutoPtr<AwPermissionRequest>) Create(
        /* [in] */ Int64 nativeAwPermissionRequest,
        /* [in] */ const String& url,
        /* [in] */ Int64 resources);

    //@CalledByNative
    CARAPI_(void) DetachNativeInstance();

    CARAPI_(void) Validate();

    CARAPI_(void) NativeOnAccept(
        /* [in] */ Int64 nativeAwPermissionRequest,
        /* [in] */ Boolean allowed);

private:
    static String TAG;

    AutoPtr<IUri> mOrigin;
    Int64 mResources;
    Boolean mProcessed;

    // AwPermissionRequest native instance.
    Int64 mNativeAwPermissionRequest;
};

} // namespace Permission
} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_PERMISSION_AWPERMISSIONREQUEST_H__
