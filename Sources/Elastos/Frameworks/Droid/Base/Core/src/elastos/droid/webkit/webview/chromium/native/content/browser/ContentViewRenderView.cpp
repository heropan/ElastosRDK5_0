
#include "elastos/droid/webkit/webview/chromium/native/content/browser/ContentViewRenderView.h"
#include "elastos/droid/webkit/webview/chromium/native/content/api/ContentViewRenderView_dec.h"
#include "elastos/droid/webkit/webview/chromium/native/ui/base/WindowElastos.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::CSurfaceView;
using Elastos::Droid::Webkit::Webview::Chromium::Ui::Base::WindowElastos;
using Elastos::Droid::Widget::IFrameLayoutLayoutParams;
using Elastos::Droid::Widget::CFrameLayoutLayoutParams;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {
namespace Content {
namespace Browser {

//===============================================================
//       ContentViewRenderView::InnerSurfaceHolderCallback
//===============================================================

ContentViewRenderView::InnerSurfaceHolderCallback::InnerSurfaceHolderCallback(
    /* [in] */ ContentViewRenderView* owner)
    : mOwner(owner)
{
}

//@Override
ECode ContentViewRenderView::InnerSurfaceHolderCallback::SurfaceChanged(
    /* [in] */ ISurfaceHolder* holder,
    /* [in] */ Int32 format,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    VALIDATE_NOT_NULL(holder);

    assert(mOwner->mNativeContentViewRenderView != 0);

    AutoPtr<ISurface> surface;
    holder->GetSurface((ISurface**)&surface);
    mOwner->NativeSurfaceChanged(mOwner->mNativeContentViewRenderView,
            format, width, height, surface);
    if (mOwner->mContentViewCore != NULL) {
        mOwner->mContentViewCore->OnPhysicalBackingSizeChanged(
                width, height);
    }

    return NOERROR;
}

//@Override
ECode ContentViewRenderView::InnerSurfaceHolderCallback::SurfaceCreated(
    /* [in] */ ISurfaceHolder* holder)
{
    VALIDATE_NOT_NULL(holder);

    assert(mOwner->mNativeContentViewRenderView != 0);
    mOwner->NativeSurfaceCreated(mOwner->mNativeContentViewRenderView);

    mOwner->OnReadyToRender();

    return NOERROR;
}

//@Override
ECode ContentViewRenderView::InnerSurfaceHolderCallback::SurfaceDestroyed(
    /* [in] */ ISurfaceHolder* holder)
{
    VALIDATE_NOT_NULL(holder);

    assert(mOwner->mNativeContentViewRenderView != 0);
    mOwner->NativeSurfaceDestroyed(mOwner->mNativeContentViewRenderView);

    return NOERROR;
}

//===============================================================
//       ContentViewRenderView::InnerContentReadbackHandler
//===============================================================

ContentViewRenderView::InnerContentReadbackHandler::InnerContentReadbackHandler(
    /* [in] */ ContentViewRenderView* owner)
    : mOwner(owner)
{
}

//@Override
Boolean ContentViewRenderView::InnerContentReadbackHandler::ReadyForReadback()
{
    return mOwner->mNativeContentViewRenderView != 0 && mOwner->mContentViewCore != NULL;
}

//===============================================================
//            ContentViewRenderView::InnerRunnable
//===============================================================

ContentViewRenderView::InnerRunnable::InnerRunnable(
    /* [in] */ ContentViewRenderView* owner)
    : mOwner(owner)
{
}

ECode ContentViewRenderView::InnerRunnable::Run()
{
    AutoPtr<IView> view = IView::Probe(mOwner);
    view->SetBackgroundResource(0);
    return NOERROR;
}

//===============================================================
//                    ContentViewRenderView
//===============================================================

/**
 * Constructs a new ContentViewRenderView.
 * This should be called and the {@link ContentViewRenderView} should be added to the view
 * hierarchy before the first draw to avoid a black flash that is seen every time a
 * {@link SurfaceView} is added.
 * @param context The context used to create this.
 */
ContentViewRenderView::ContentViewRenderView(
    /* [in] */ IContext* context)
    : mNativeContentViewRenderView(0)
{
    FrameLayout::constructor(context);

    AutoPtr<IContext> iContext;
    GetContext((IContext**)&iContext);
    mSurfaceView = CreateSurfaceView(iContext);
    mSurfaceView->SetZOrderMediaOverlay(TRUE);

    SetSurfaceViewBackgroundColor(IColor::WHITE);
    AutoPtr<IFrameLayoutLayoutParams> param;
    CFrameLayoutLayoutParams::New(
                    IViewGroupLayoutParams::MATCH_PARENT,
                    IViewGroupLayoutParams::MATCH_PARENT,
                    (IFrameLayoutLayoutParams**)&param);
    AddView(IView::Probe(mSurfaceView), IViewGroupLayoutParams::Probe(param));
    IView::Probe(mSurfaceView)->SetVisibility(GONE);
}

/**
 * Initialization that requires native libraries should be done here.
 * Native code should add/remove the layers to be rendered through the ContentViewLayerRenderer.
 * @param rootWindow The {@link WindowAndroid} this render view should be linked to.
 */
void ContentViewRenderView::OnNativeLibraryLoaded(
    /* [in] */ WindowElastos* rootWindow)
{
//    assert !mSurfaceView.getHolder().getSurface().isValid() :
//            "Surface created before native library loaded.";
    assert(rootWindow != NULL);

    mNativeContentViewRenderView = NativeInit(rootWindow->GetNativePointer());
    assert(mNativeContentViewRenderView != 0);
    mSurfaceCallback = new InnerSurfaceHolderCallback(this);
    AutoPtr<ISurfaceHolder> surfaceHolder;
    mSurfaceView->GetHolder((ISurfaceHolder**)&surfaceHolder);
    surfaceHolder->AddCallback(mSurfaceCallback);
    AutoPtr<IView> view = IView::Probe(mSurfaceView);
    view->SetVisibility(IView::VISIBLE);

    mContentReadbackHandler = new InnerContentReadbackHandler(this);

    mContentReadbackHandler->InitNativeContentReadbackHandler();
}

/**
 * @return The content readback handler.
 */
AutoPtr<ContentReadbackHandler> ContentViewRenderView::GetContentReadbackHandler()
{
    return mContentReadbackHandler;
}

/**
 * Sets the background color of the surface view.  This method is necessary because the
 * background color of ContentViewRenderView itself is covered by the background of
 * SurfaceView.
 * @param color The color of the background.
 */
void ContentViewRenderView::SetSurfaceViewBackgroundColor(
    /* [in] */ Int32 color)
{
    if (mSurfaceView != NULL) {
        AutoPtr<IView> view = IView::Probe(mSurfaceView);
        view->SetBackgroundColor(color);
    }
}

/**
 * Should be called when the ContentViewRenderView is not needed anymore so its associated
 * native resource can be freed.
 */
void ContentViewRenderView::Destroy()
{
    mContentReadbackHandler->Destroy();
    mContentReadbackHandler = NULL;
    AutoPtr<ISurfaceHolder> surfaceHolder;
    mSurfaceView->GetHolder((ISurfaceHolder**)&surfaceHolder);
    surfaceHolder->RemoveCallback(mSurfaceCallback);
    NativeDestroy(mNativeContentViewRenderView);
    mNativeContentViewRenderView = 0;
}

void ContentViewRenderView::SetCurrentContentViewCore(
    /* [in] */ ContentViewCore* contentViewCore)
{
    assert(mNativeContentViewRenderView != 0);
    mContentViewCore = contentViewCore;

    if (mContentViewCore != NULL) {
        Int32 width, height;
        GetWidth(&width);
        GetHeight(&height);
        mContentViewCore->OnPhysicalBackingSizeChanged(width, height);
        NativeSetCurrentContentViewCore(mNativeContentViewRenderView,
                                        mContentViewCore->GetNativeContentViewCore());
    }
    else {
        NativeSetCurrentContentViewCore(mNativeContentViewRenderView, 0);
    }
}

/**
 * This method should be subclassed to provide actions to be performed once the view is ready to
 * render.
 */
void ContentViewRenderView::OnReadyToRender()
{
}

/**
 * This method could be subclassed optionally to provide a custom SurfaceView object to
 * this ContentViewRenderView.
 * @param context The context used to create the SurfaceView object.
 * @return The created SurfaceView object.
 */
AutoPtr<ISurfaceView> ContentViewRenderView::CreateSurfaceView(
    /* [in] */ IContext* context)
{
    AutoPtr<ISurfaceView> view;
    CSurfaceView::New(context, (ISurfaceView**)&view);
    return view;
}

/**
 * @return whether the surface view is initialized and ready to render.
 */
Boolean ContentViewRenderView::IsInitialized()
{
    AutoPtr<ISurfaceHolder> holder;
    mSurfaceView->GetHolder((ISurfaceHolder**)&holder);
    AutoPtr<ISurface> surface;
    holder->GetSurface((ISurface**)&surface);
    return surface != NULL;
}

/**
 * Enter or leave overlay video mode.
 * @param enabled Whether overlay mode is enabled.
 */
void ContentViewRenderView::SetOverlayVideoMode(
    /* [in] */ Boolean enabled)
{
    Int32 format = enabled ? IPixelFormat::TRANSLUCENT : IPixelFormat::OPAQUE;
    AutoPtr<ISurfaceHolder> holder;
    mSurfaceView->GetHolder((ISurfaceHolder**)&holder);
    holder->SetFormat(format);
    NativeSetOverlayVideoMode(mNativeContentViewRenderView, enabled);
}

/**
 * Set the native layer tree helper for this {@link ContentViewRenderView}.
 * @param layerTreeBuildHelperNativePtr Native pointer to the layer tree build helper.
 */
void ContentViewRenderView::SetLayerTreeBuildHelper(
    /* [in] */ Int64 layerTreeBuildHelperNativePtr)
{
    NativeSetLayerTreeBuildHelper(mNativeContentViewRenderView, layerTreeBuildHelperNativePtr);
}

//@CalledByNative
void ContentViewRenderView::OnCompositorLayout()
{
}

//@CalledByNative
void ContentViewRenderView::OnSwapBuffersCompleted()
{
    AutoPtr<IDrawable> draw;
    AutoPtr<IView> view = IView::Probe(mSurfaceView);
    view->GetBackground((IDrawable**)&draw);
    if (draw != NULL) {
        AutoPtr<IRunnable> runnable = new InnerRunnable(this);
        Boolean res;
        Post(runnable, &res);
    }
}

Int64 ContentViewRenderView::NativeInit(
    /* [in] */ Int64 rootWindowNativePointer)
{
    return Elastos_ContentViewRenderView_nativeInit(THIS_PROBE(IInterface), rootWindowNativePointer);
}

void ContentViewRenderView::NativeDestroy(
    /* [in] */ Int64 nativeContentViewRenderView)
{
    Elastos_ContentViewRenderView_nativeDestroy(THIS_PROBE(IInterface), (Handle32)nativeContentViewRenderView);
}

void ContentViewRenderView::NativeSetCurrentContentViewCore(
    /* [in] */ Int64 nativeContentViewRenderView,
    /* [in] */ Int64 nativeContentViewCore)
{
    Elastos_ContentViewRenderView_nativeSetCurrentContentViewCore(THIS_PROBE(IInterface), (Handle32)nativeContentViewRenderView, nativeContentViewCore);
}

void ContentViewRenderView::NativeSetLayerTreeBuildHelper(
    /* [in] */ Int64 nativeContentViewRenderView,
    /* [in] */ Int64 buildHelperNativePtr)
{
    Elastos_ContentViewRenderView_nativeSetLayerTreeBuildHelper(THIS_PROBE(IInterface), (Handle32)nativeContentViewRenderView, buildHelperNativePtr);
}

void ContentViewRenderView::NativeSurfaceCreated(
    /* [in] */ Int64 nativeContentViewRenderView)
{
    Elastos_ContentViewRenderView_nativeSurfaceCreated(THIS_PROBE(IInterface), (Handle32)nativeContentViewRenderView);
}

void ContentViewRenderView::NativeSurfaceDestroyed(
    /* [in] */ Int64 nativeContentViewRenderView)
{
    Elastos_ContentViewRenderView_nativeSurfaceDestroyed(THIS_PROBE(IInterface), (Handle32)nativeContentViewRenderView);
}

void ContentViewRenderView::NativeSurfaceChanged(
    /* [in] */ Int64 nativeContentViewRenderView,
    /* [in] */ Int32 format,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ ISurface* surface)
{
    Elastos_ContentViewRenderView_nativeSurfaceChanged(THIS_PROBE(IInterface), (Handle32)nativeContentViewRenderView,
            format, width, height, TO_IINTERFACE(surface));
}

void ContentViewRenderView::NativeSetOverlayVideoMode(
    /* [in] */ Int64 nativeContentViewRenderView,
    /* [in] */ Boolean enabled)
{
    Elastos_ContentViewRenderView_nativeSetOverlayVideoMode(THIS_PROBE(IInterface), (Handle32)nativeContentViewRenderView, enabled);
}

void ContentViewRenderView::OnCompositorLayout(
    /* [in] */ IInterface* obj)
{
    AutoPtr<ContentViewRenderView> mObj = (ContentViewRenderView*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("ContentViewRenderView", "ContentViewRenderView::OnCompositorLayout, mObj is NULL");
        return;
    }
    mObj->OnCompositorLayout();
}

void ContentViewRenderView::OnSwapBuffersCompleted(
    /* [in] */ IInterface* obj)
{
    AutoPtr<ContentViewRenderView> mObj = (ContentViewRenderView*)(IObject::Probe(obj));
    if (NULL == mObj)
    {
        Logger::E("ContentViewRenderView", "ContentViewRenderView::OnSwapBuffersCompleted, mObj is NULL");
        return;
    }
    mObj->OnSwapBuffersCompleted();
}

} // namespace Browser
} // namespace Content
} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
