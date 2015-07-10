
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Framehost {

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.content_public.browser.NavigationController;

/**
 * The NavigationControllerImpl Java wrapper to allow communicating with the native
 * NavigationControllerImpl object.
 */
//@JNINamespace("content")
// TODO(tedchoc): Remove the package restriction once this class moves to a non-public content
//                package whose visibility will be enforced via DEPS.
/* package */
class NavigationControllerImpl : public NavigationController
{
public:
    //@Override
    CARAPI_(Boolean) CanGoBack();

    //@Override
    CARAPI_(Boolean) CanGoForward();

    //@Override
    CARAPI_(Boolean) CanGoToOffset(int offset);

    //@Override
    CARAPI_(void) GoToOffset(int offset);

    //@Override
    CARAPI_(void) GoToNavigationIndex(int index);

    //@Override
    CARAPI_(void) GoBack();

    //@Override
    CARAPI_(void) GoForward();

private:
    NavigationControllerImpl(long nativeNavigationControllerAndroid);

    //@CalledByNative
    static CARAPI_(AutoPtr<NavigationControllerImpl>) Create(long nativeNavigationControllerAndroid);

    //@CalledByNative
    CARAPI_(void) Destroy();

    CARAPI_(Boolean) NativeCanGoBack(long nativeNavigationControllerAndroid);

    CARAPI_(Boolean) NativeCanGoForward(long nativeNavigationControllerAndroid);

    CARAPI_(Boolean) NativeCanGoToOffset(
            long nativeNavigationControllerAndroid, int offset);

    CARAPI_(void) NativeGoBack(long nativeNavigationControllerAndroid);

    CARAPI_(void) NativeGoForward(long nativeNavigationControllerAndroid);

    CARAPI_(void) NativeGoToOffset(long nativeNavigationControllerAndroid, int offset);

    CARAPI_(void) NativeGoToNavigationIndex(
            long nativeNavigationControllerAndroid, int index);

private:
    Int64 mNativeNavigationControllerAndroid;
};

} // namespace Framehost
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
