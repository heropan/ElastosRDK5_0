
#ifndef __ELASTOS_DROID_VIEW_THREADEDRENDERER_H__
#define __ELASTOS_DROID_VIEW_THREADEDRENDERER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/HardwareRenderer.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::IBitmap;

using Elastos::IO::IPrintWriter;
using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Droid {
namespace View {

/**
 * Hardware renderer that proxies the rendering to a render thread. Most calls
 * are currently synchronous.
 *
 * The UI thread can block on the RenderThread, but RenderThread must never
 * block on the UI thread.
 *
 * ThreadedRenderer creates an instance of RenderProxy. RenderProxy in turn creates
 * and manages a CanvasContext on the RenderThread. The CanvasContext is fully managed
 * by the lifecycle of the RenderProxy.
 *
 * Note that although currently the EGL context & surfaces are created & managed
 * by the render thread, the goal is to move that into a shared structure that can
 * be managed by both threads. EGLSurface creation & deletion should ideally be
 * done on the UI thread and not the RenderThread to avoid stalling the
 * RenderThread with surface buffer allocation.
 *
 * @hide
 */
class ThreadedRenderer
    : public HardwareRenderer
{
private:
    class AtlasInitializer
        : public Object
    {
    public:
        CARAPI_(void) Init(
            /* [in] */ IContext* context,
            /* [in] */ Int64 renderProxy);

    private:
        AtlasInitializer();

        static CARAPI_(void) ValidateMap(
            /* [in] */ IContext* context,
            /* [in] */ ArrayOf<Int64>* map);

    public:
        static AutoPtr<AtlasInitializer> sInstance;

    private:
        Boolean mInitialized;
    };

public:
    ThreadedRenderer();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Boolean translucent);

    CARAPI_(void) Destroy();

    CARAPI_(Boolean) Initialize(
        /* [in] */ ISurface* surface);

    CARAPI UpdateSurface(
        /* [in] */ ISurface* surface);

    CARAPI_(void) PauseSurface(
        /* [in] */ ISurface* surface);

    CARAPI_(void) DestroyHardwareResources(
        /* [in] */ IView* view);

    CARAPI_(void) Invalidate(
        /* [in] */ ISurface* surface);

    CARAPI_(void) DetachSurfaceTexture(
        /* [in] */ Int64 hardwareLayer);

    CARAPI_(void) Setup(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ IRect* surfaceInsets);

    CARAPI_(void) SetOpaque(
        /* [in] */ Boolean opaque);

    CARAPI_(Int32) GetWidth();

    CARAPI_(Int32) GetHeight();

    CARAPI_(void) DumpGfxInfo(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ IFileDescriptor* fd);

    CARAPI_(Boolean) LoadSystemProperties();

    CARAPI_(void) InvalidateRoot();

    // CARAPI Draw(
    //     /* [in] */ IView* view,
    //     /* [in] */ IAttachInfo* attachInfo,
    //     /* [in] */ IHardwareDrawCallbacks* callbacks);

    static CARAPI_(void) InvokeFunctor(
        /* [in] */ Int64 functor,
        /* [in] */ Boolean waitForCompletion);

    CARAPI_(AutoPtr<IHardwareLayer>) CreateTextureLayer();

    CARAPI_(void) BuildLayer(
        /* [in] */ IRenderNode* node);

    CARAPI_(Boolean) CopyLayerInto(
        /* [in] */ IHardwareLayer* layer,
        /* [in] */ IBitmap* bitmap);

    CARAPI_(void) PushLayerUpdate(
        /* [in] */ IHardwareLayer* layer);

    CARAPI_(void) OnLayerDestroyed(
        /* [in] */ IHardwareLayer* layer);

    CARAPI_(void) SetName(
        /* [in] */ const String& name);

    CARAPI_(void) Fence();

    CARAPI_(void) StopDrawing();

    CARAPI_(void) NotifyFramePending();

    CARAPI_(void) RegisterAnimatingRenderNode(
        /* [in] */ IRenderNode* animator);

    static CARAPI_(void) TrimMemory(
        /* [in] */ Int32 level);

    static CARAPI_(void) NativeSetupShadersDiskCache(
        /* [in] */ const String& cacheFile);

    CARAPI Finalize();

private:
    CARAPI_(void) UpdateEnabledState(
        /* [in] */ ISurface* surface);

    static CARAPI_(void) DestroyResources(
        /* [in] */ IView* view);

    static CARAPI_(Int32) Search(
        /* [in] */ ArrayOf<String>* values,
        /* [in] */ const String& value);

    static CARAPI_(Boolean) CheckIfProfilingRequested();

    CARAPI_(void) UpdateViewTreeDisplayList(
        /* [in] */ IView* view);

    // CARAPI_(void) UpdateRootDisplayList(
    //     /* [in] */ IView* view,
    //     /* [in] */ IHardwareDrawCallbacks* callbacks);

    static CARAPI_(void) NativeSetAtlas(
        /* [in] */ Int64 nativeProxy,
        /* [in] */ IGraphicBuffer* buffer,
        /* [in] */ ArrayOf<Int64>* map);

    static CARAPI_(Int64) NativeCreateRootRenderNode();

    static CARAPI_(Int64) NativeCreateProxy(
        /* [in] */ Boolean translucent,
        /* [in] */ Int64 rootRenderNode);

    static CARAPI_(void) NativeDeleteProxy(
        /* [in] */ Int64 nativeProxy);

    static CARAPI_(void) NativeSetFrameInterval(
        /* [in] */ Int64 nativeProxy,
        /* [in] */ Int64 frameIntervalNanos);

    static CARAPI_(Boolean) NativeLoadSystemProperties(
        /* [in] */ Int64 nativeProxy);

    static CARAPI_(Boolean) NativeInitialize(
        /* [in] */ Int64 nativeProxy,
        /* [in] */ ISurface* window);

    static CARAPI_(void) NativeUpdateSurface(
        /* [in] */ Int64 nativeProxy,
        /* [in] */ ISurface* window);

    static CARAPI_(void) NativePauseSurface(
        /* [in] */ Int64 nativeProxy,
        /* [in] */ ISurface* window);

    static CARAPI_(void) NativeSetup(
        /* [in] */ Int64 nativeProxy,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Float lightX,
        /* [in] */ Float lightY,
        /* [in] */ Float lightZ,
        /* [in] */ Float lightRadius,
        /* [in] */ Int32 ambientShadowAlpha,
        /* [in] */ Int32 spotShadowAlpha);

    static CARAPI_(void) NativeSetOpaque(
        /* [in] */ Int64 nativeProxy,
        /* [in] */ Boolean opaque);

    static CARAPI_(Int32) NativeSyncAndDrawFrame(
        /* [in] */ Int64 nativeProxy,
        /* [in] */ Int64 frameTimeNanos,
        /* [in] */ Int64 recordDuration,
        /* [in] */ Float density);

    static CARAPI_(void) NativeDestroy(
        /* [in] */ Int64 nativeProxy);

    static CARAPI_(void) NativeRegisterAnimatingRenderNode(
        /* [in] */ Int64 rootRenderNode,
        /* [in] */ Int64 animatingNode);

    static CARAPI_(void) NativeInvokeFunctor(
        /* [in] */ Int64 functor,
        /* [in] */ Boolean waitForCompletion);

    static CARAPI_(Int64) NativeCreateTextureLayer(
        /* [in] */ Int64 nativeProxy);

    static CARAPI_(void) NativeBuildLayer(
        /* [in] */ Int64 nativeProxy,
        /* [in] */ Int64 node);

    static CARAPI_(Boolean) NativeCopyLayerInto(
        /* [in] */ Int64 nativeProxy,
        /* [in] */ Int64 layer,
        /* [in] */ Int64 bitmap);

    static CARAPI_(void) NativePushLayerUpdate(
        /* [in] */ Int64 nativeProxy,
        /* [in] */ Int64 layer);

    static CARAPI_(void) NativeCancelLayerUpdate(
        /* [in] */ Int64 nativeProxy,
        /* [in] */ Int64 layer);

    static CARAPI_(void) NativeDetachSurfaceTexture(
        /* [in] */ Int64 nativeProxy,
        /* [in] */ Int64 layer);

    static CARAPI_(void) NativeDestroyHardwareResources(
        /* [in] */ Int64 nativeProxy);

    static CARAPI_(void) NativeTrimMemory(
        /* [in] */ Int32 level);

    static CARAPI_(void) NativeFence(
        /* [in] */ Int64 nativeProxy);

    static CARAPI_(void) NativeStopDrawing(
        /* [in] */ Int64 nativeProxy);

    static CARAPI_(void) NativeNotifyFramePending(
        /* [in] */ Int64 nativeProxy);

    static CARAPI_(void) NativeDumpProfileInfo(
        /* [in] */ Int64 nativeProxy,
        /* [in] */ IFileDescriptor* fd);

private:
    static String LOGTAG;

    // Keep in sync with DrawFrameTask.h SYNC_* flags
    // Nothing interesting to report
    static Int32 SYNC_OK;
    // Needs a ViewRoot invalidate
    static Int32 SYNC_INVALIDATE_REQUIRED;

    static AutoPtr<ArrayOf<String> > VISUALIZERS;

    // Size of the rendered content.
    Int32 mWidth, mHeight;

    // Actual size of the drawing surface.
    Int32 mSurfaceWidth, mSurfaceHeight;

    // Insets between the drawing surface and rendered content. These are
    // applied as translation when updating the root render node.
    Int32 mInsetTop, mInsetLeft;

    // Whether the surface has insets. Used to protect opacity.
    Boolean mHasInsets;

    // Light and shadow properties specified by the theme.
    Float mLightY;
    Float mLightZ;
    Float mLightRadius;
    Int32 mAmbientShadowAlpha;
    Int32 mSpotShadowAlpha;

    Int64 mNativeProxy;
    Boolean mInitialized;
    AutoPtr<IRenderNode> mRootNode;
    AutoPtr<IChoreographer> mChoreographer;
    Boolean mProfilingEnabled;
    Boolean mRootNodeNeedsUpdate;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_THREADEDRENDERER_H__