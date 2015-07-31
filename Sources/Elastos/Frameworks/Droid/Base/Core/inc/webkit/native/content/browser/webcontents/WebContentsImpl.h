
#ifndef __WEBCONTENTSIMPL_H__
#define __WEBCONTENTSIMPL_H__

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.content_public.browser.NavigationController;
// import org.chromium.content_public.browser.WebContents;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Webcontents {

/**
 * The WebContentsImpl Java wrapper to allow communicating with the native WebContentsImpl
 * object.
 */
//@JNINamespace("content")
//TODO(tedchoc): Remove the package restriction once this class moves to a non-public content
//               package whose visibility will be enforced via DEPS.
/* package */
class WebContentsImpl : public WebContents
{
public:
    //@Override
    CARAPI_(AutoPtr<NavigationController>) GetNavigationController();

    //@Override
    CARAPI_(String) GetTitle();

    //@Override
    CARAPI_(String) GetVisibleUrl();

    //@Override
    CARAPI_(void) Stop();

private:
    WebContentsImpl(
        /* [in] */ Int64 nativeWebContentsAndroid,
        /* [in] */ NavigationController* navigationController);

    //@CalledByNative
    static CARAPI_(AutoPtr<WebContentsImpl>) Create(
        /* [in] */ Int64 nativeWebContentsAndroid,
        /* [in] */ NavigationController* navigationController);

    //@CalledByNative
    CARAPI_(void) Destroy();

    //@CalledByNative
    CARAPI_(Int64) GetNativePointer();

    CARAPI_(String) NativeGetTitle(
        /* [in] */ Int64 nativeWebContentsAndroid);

    CARAPI_(String) NativeGetVisibleURL(
        /* [in] */ Int64 nativeWebContentsAndroid);

    CARAPI_(void) NativeStop(
        /* [in] */ Int64 nativeWebContentsAndroid);

private:
    Int64 mNativeWebContentsAndroid;
    AutoPtr<NavigationController> mNavigationController;
};

} // namespace Webcontents
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__WEBCONTENTSIMPL_H__
