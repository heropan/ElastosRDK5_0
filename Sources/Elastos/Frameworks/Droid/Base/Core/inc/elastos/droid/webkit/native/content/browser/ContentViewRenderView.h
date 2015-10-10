
#ifndef __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_CONTENTVIEWRENDERVIEW_H__
#define __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_CONTENTVIEWRENDERVIEW_H__

#include "elastos/droid/ext/frameworkext.h"
//TODO #include "widget/FrameLayout.h"
#include "os/Runnable.h"
#include "webkit/native/content/browser/ContentReadbackHandler.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IColor;
using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::View::ISurface;
using Elastos::Droid::View::ISurfaceHolder;
using Elastos::Droid::View::ISurfaceHolderCallback;
using Elastos::Droid::View::ISurfaceView;
//TODO using Elastos::Droid::Widget::FrameLayout;
using Elastos::Droid::Widget::IFrameLayout;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

class WindowAndroid;

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

using Elastos::Droid::Webkit::Ui::Base::WindowAndroid;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.ui.base.WindowAndroid;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/***
 * This view is used by a ContentView to render its content.
 * Call {@link #setCurrentContentViewCore(ContentViewCore)} with the contentViewCore that should be
 * managing the view.
 * Note that only one ContentViewCore can be shown at a time.
 */
//@JNINamespace("content")
class ContentViewRenderView
    : public Object
    //TODO : public FrameLayout
{
private:
    class InnerSurfaceHolderCallback
        : public Object
        , public ISurfaceHolderCallback
    {
    public:
        InnerSurfaceHolderCallback(
            /* [in] */ ContentViewRenderView* owner);

        CAR_INTERFACE_DECL();

        //@Override
        CARAPI SurfaceChanged(
            /* [in] */ ISurfaceHolder* holder,
            /* [in] */ Int32 format,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        //@Override
        CARAPI SurfaceCreated(
            /* [in] */ ISurfaceHolder* holder);

        //@Override
        CARAPI SurfaceDestroyed(
            /* [in] */ ISurfaceHolder* holder);

    private:
        ContentViewRenderView* mOwner;
    };

    class InnerContentReadbackHandler
        : public ContentReadbackHandler
    {
    public:
        InnerContentReadbackHandler(
            /* [in] */ ContentViewRenderView* owner);

    protected:
        //@Override
        CARAPI_(Boolean) ReadyForReadback();

    private:
        ContentViewRenderView* mOwner;
    };

    class InnerRunnable
        : public Runnable
    {
    public:
        InnerRunnable(
            /* [in] */ ContentViewRenderView* owner);

        CARAPI Run();

    private:
        ContentViewRenderView* mOwner;
    };

public:
    /**
     * Constructs a new ContentViewRenderView.
     * This should be called and the {@link ContentViewRenderView} should be added to the view
     * hierarchy before the first draw to avoid a black flash that is seen every time a
     * {@link SurfaceView} is added.
     * @param context The context used to create this.
     */
    ContentViewRenderView(
        /* [in] */ IContext* context);

    /**
     * Initialization that requires native libraries should be done here.
     * Native code should add/remove the layers to be rendered through the ContentViewLayerRenderer.
     * @param rootWindow The {@link WindowAndroid} this render view should be linked to.
     */
    CARAPI_(void) OnNativeLibraryLoaded(
        /* [in] */ WindowAndroid* rootWindow);

    /**
     * @return The content readback handler.
     */
    CARAPI_(AutoPtr<ContentReadbackHandler>) GetContentReadbackHandler();

    /**
     * Sets the background color of the surface view.  This method is necessary because the
     * background color of ContentViewRenderView itself is covered by the background of
     * SurfaceView.
     * @param color The color of the background.
     */
    CARAPI_(void) SetSurfaceViewBackgroundColor(
        /* [in] */ Int32 color);

    /**
     * Should be called when the ContentViewRenderView is not needed anymore so its associated
     * native resource can be freed.
     */
    CARAPI_(void) Destroy();

    CARAPI_(void) SetCurrentContentViewCore(
        /* [in] */ ContentViewCore* contentViewCore);

    /**
     * @return whether the surface view is initialized and ready to render.
     */
    CARAPI_(Boolean) IsInitialized();

    /**
     * Enter or leave overlay video mode.
     * @param enabled Whether overlay mode is enabled.
     */
    CARAPI_(void) SetOverlayVideoMode(
        /* [in] */ Boolean enabled);

    /**
     * Set the native layer tree helper for this {@link ContentViewRenderView}.
     * @param layerTreeBuildHelperNativePtr Native pointer to the layer tree build helper.
     */
    CARAPI_(void) SetLayerTreeBuildHelper(
        /* [in] */ Int64 layerTreeBuildHelperNativePtr);

protected:
    /**
     * This method should be subclassed to provide actions to be performed once the view is ready to
     * render.
     */
    CARAPI_(void) OnReadyToRender();

    /**
     * This method could be subclassed optionally to provide a custom SurfaceView object to
     * this ContentViewRenderView.
     * @param context The context used to create the SurfaceView object.
     * @return The created SurfaceView object.
     */
    CARAPI_(AutoPtr<ISurfaceView>) CreateSurfaceView(
        /* [in] */ IContext* context);

    //@CalledByNative
    CARAPI_(void) OnCompositorLayout();

protected:
    AutoPtr<ContentViewCore> mContentViewCore;

private:
    //@CalledByNative
    CARAPI_(void) OnSwapBuffersCompleted();

    CARAPI_(Int64) NativeInit(
        /* [in] */ Int64 rootWindowNativePointer);

    CARAPI_(void) NativeDestroy(
        /* [in] */ Int64 nativeContentViewRenderView);

    CARAPI_(void) NativeSetCurrentContentViewCore(
        /* [in] */ Int64 nativeContentViewRenderView,
        /* [in] */ Int64 nativeContentViewCore);

    CARAPI_(void) NativeSetLayerTreeBuildHelper(
        /* [in] */ Int64 nativeContentViewRenderView,
        /* [in] */ Int64 buildHelperNativePtr);

    CARAPI_(void) NativeSurfaceCreated(
        /* [in] */ Int64 nativeContentViewRenderView);

    CARAPI_(void) NativeSurfaceDestroyed(
        /* [in] */ Int64 nativeContentViewRenderView);

    CARAPI_(void) NativeSurfaceChanged(
        /* [in] */ Int64 nativeContentViewRenderView,
        /* [in] */ Int32 format,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ ISurface* surface);

    CARAPI_(void) NativeSetOverlayVideoMode(
        /* [in] */ Int64 nativeContentViewRenderView,
        /* [in] */ Boolean enabled);

private:
    // The native side of this object.
    Int64 mNativeContentViewRenderView;
    AutoPtr<ISurfaceHolderCallback> mSurfaceCallback;

    const AutoPtr<ISurfaceView> mSurfaceView;

    AutoPtr<ContentReadbackHandler> mContentReadbackHandler;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_CONTENTVIEWRENDERVIEW_H__
