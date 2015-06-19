
#ifndef __AWWEBCONTENTSDELEGATE_H__
#define __AWWEBCONTENTSDELEGATE_H__

// import com.google.common.annotations.VisibleForTesting;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.components.web_contents_delegate_android.WebContentsDelegateAndroid;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * WebView-specific WebContentsDelegate.
 * This file is the Java version of the native class of the same name.
 * It should contain abstract WebContentsDelegate methods to be implemented by the embedder.
 * These methods belong to WebView but are not shared with the Chromium Android port.
 */
//@VisibleForTesting
//@JNINamespace("android_webview")
class AwWebContentsDelegate : public WebContentsDelegateAndroid
{
public:
    // Callback filesSelectedInChooser() when done.
    //@CalledByNative
    virtual CARAPI_(void) RunFileChooser(
        /* [in] */ Int32 processId,
        /* [in] */ Int32 renderId,
        /* [in] */ Int32 mode_flags,
        /* [in] */ String acceptTypes,
        /* [in] */ String title,
        /* [in] */ String defaultFilename,
        /* [in] */ Boolean capture) = 0;

    //@CalledByNative
    virtual CARAPI_(Boolean) AddNewContents(
        /* [in] */ Boolean isDialog,
        /* [in] */ Boolean isUserGesture) = 0;

    //@Override
    //@CalledByNative
    CARAPI_(void) CloseContents() = 0;

    //@Override
    //@CalledByNative
    CARAPI_(void) ActivateContents() = 0;

    //@Override
    //@CalledByNative
    CARAPI_(void) ToggleFullscreenModeForTab(
        /* [in] */ Boolean enterFullscreen) = 0;

protected:
    // Call in response to a prior runFileChooser call.
    static CARAPI_(void) NativeFilesSelectedInChooser(
        /* [in] */ Int32 processId,
        /* [in] */ Int32 renderId,
        /* [in] */ Int32 mode_flags,
        /* [in] */ ArrayOf<String>* filePath,
        /* [in] */ ArrayOf<String>* displayName);
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__AWWEBCONTENTSDELEGATE_H__
