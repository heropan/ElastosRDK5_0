
#ifndef __CSURFACE_H__
#define __CSURFACE_H__

#include "_CSurface.h"
#include "graphics/Canvas.h"

#include <gui/Surface.h>

using namespace Elastos::Droid::Graphics;
using Elastos::Droid::Content::Res::ICompatibilityInfoTranslator;
using Elastos::Droid::Os::IBinder;

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CSurface)
{
private:
    /**
     * A Canvas class that can handle the compatibility mode.
     * This does two things differently.
     * <ul>
     * <li>Returns the width and height of the target metrics, rather than
     * native. For example, the canvas returns 320x480 even if an app is running
     * in WVGA high density.
     * <li>Scales the matrix in setMatrix by the application scale, except if
     * the matrix looks like obtained from getMatrix. This is a hack to handle
     * the case that an application uses getMatrix to keep the original matrix,
     * set matrix of its own, then set the original matrix back. There is no
     * perfect solution that works for all cases, and there are a lot of cases
     * that this model does not work, but we hope this works for many apps.
     * </ul>
     */

    class _CompatibleCanvas : public Canvas
    {
    public:
        _CompatibleCanvas(
            /* [in] */ CSurface* owner);

        CARAPI GetWidth(
            /* [out] */ Int32* width);

        CARAPI GetHeight(
            /* [out] */ Int32* height);

        CARAPI SetMatrix(
            /* [in] */ IMatrix* matrix);

        CARAPI GetMatrix(
            /* [out] */ IMatrix** ctm);

    private:
        AutoPtr<IMatrix> mOrigMatrix;
        CSurface* mOwner;
    };

    class CompatibleCanvas
        : public ElRefBase
        , public _CompatibleCanvas
        , public ICanvas
    {
    public:
        CompatibleCanvas(
            /* [in] */ CSurface* owner);

        ICANVAS_METHODS_DECL();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
    };

public:
    static CARAPI Screenshot(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ IBitmap** bitmap);

    static CARAPI Screenshot(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 minLayer,
        /* [in] */ Int32 maxLayer,
        /* [out] */ IBitmap** bitmap);

    /*
     * set surface parameters.
     * needs to be inside open/closeTransaction block
     */

    /** start a transaction @hide */
    static CARAPI OpenTransaction();

    /** end a transaction @hide */
    static CARAPI CloseTransaction();

    /** flag the transaction as an animation @hide */
    static CARAPI SetAnimationTransaction();

    /** @hide */
    static CARAPI GetBuiltInDisplay(
        /* [in] */ Int32 builtInDisplayId,
        /* [out] */ IBinder** binder);

    /** @hide */
    static CARAPI CreateDisplay(
        /* [in] */ const String& name,
        /* [in] */ Boolean secure,
        /* [out] */ IBinder** binder);

    /** @hide */
    static CARAPI SetDisplaySurface(
        /* [in] */ IBinder* displayToken,
        /* [in] */ ISurface* surface);

    /** @hide */
    static CARAPI SetDisplayLayerStack(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 layerStack);

    /** @hide */
    static CARAPI SetDisplayProjection(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 orientation,
        /* [in] */ IRect* layerStackRect,
        /* [in] */ IRect* displayRect);

    /** @hide */
    static CARAPI GetDisplayInfo(
        /* [in] */ IBinder* displayToken,
        /* [in] */ IPhysicalDisplayInfo* outInfo,
        /* [out] */ Boolean* result);

    /** @hide */
    static CARAPI BlankDisplay(
        /* [in] */ IBinder* displayToken);

    /** @hide */
    static CARAPI UnblankDisplay(
        /* [in] */ IBinder* displayToken);

private:
    static CARAPI NativeSetAnimationTransaction();

    static CARAPI_(AutoPtr<IBitmap>) NativeScreenshot(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 minLayer,
        /* [in] */ Int32 maxLayer,
        /* [in] */ Boolean allLayers);

    static CARAPI_(AutoPtr<IBinder>) NativeGetBuiltInDisplay(
        /* [in] */ Int32 physicalDisplayId);

    static CARAPI_(AutoPtr<IBinder>) NativeCreateDisplay(
        /* [in] */ const String& name,
        /* [in] */ Boolean secure);

    static CARAPI NativeSetDisplaySurface(
        /* [in] */ IBinder* displayToken,
        /* [in] */ ISurface* surface);

    static CARAPI NativeSetDisplayLayerStack(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 layerStack);

    static CARAPI NativeSetDisplayProjection(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 orientation,
        /* [in] */ IRect* layerStackRect,
        /* [in] */ IRect* displayRect);

    static CARAPI_(Boolean) NativeGetDisplayInfo(
        /* [in] */ IBinder* displayToken,
        /* [in] */ IPhysicalDisplayInfo* outInfo);

    static CARAPI NativeBlankDisplay(
        /* [in] */ IBinder* displayToken);

    static CARAPI NativeUnblankDisplay(
        /* [in] */ IBinder* displayToken);

public:
    CSurface();

    ~CSurface();

    /**
     * Create an empty surface, which will later be filled in by readFromParcel().
     * @hide
     */
    CARAPI constructor();

    /**
     * Create a surface with a name.
     *
     * The surface creation flags specify what kind of surface to create and
     * certain options such as whether the surface can be assumed to be opaque
     * and whether it should be initially hidden.  Surfaces should always be
     * created with the {@link #HIDDEN} flag set to ensure that they are not
     * made visible prematurely before all of the surface's properties have been
     * configured.
     *
     * Good practice is to first create the surface with the {@link #HIDDEN} flag
     * specified, open a transaction, set the surface layer, layer stack, alpha,
     * and position, call {@link #show} if appropriate, and close the transaction.
     *
     * @param session The surface session, must not be null.
     * @param name The surface name, must not be null.
     * @param w The surface initial width.
     * @param h The surface initial height.
     * @param flags The surface creation flags.  Should always include {@link #HIDDEN}
     * in the creation flags.
     * @hide
     */
    CARAPI constructor(
        /* [in] */ ISurfaceSession* session,
        /* [in] */ const String& name,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 format,
        /* [in] */ Int32 flags);

    /**
     * Create Surface from a {@link SurfaceTexture}.
     *
     * Images drawn to the Surface will be made available to the {@link
     * SurfaceTexture}, which can attach them to an OpenGL ES texture via {@link
     * SurfaceTexture#updateTexImage}.
     *
     * @param surfaceTexture The {@link SurfaceTexture} that is updated by this
     * Surface.
     */
    CARAPI constructor(
        /* [in] */ ISurfaceTexture* surfaceTexture);

    CARAPI ReleaseSurface();

    CARAPI Destroy();

    CARAPI IsValid(
        /* [out] */ Boolean* valid);

    CARAPI GetGenerationId(
        /* [out] */ Int32* generationId);

    CARAPI IsConsumerRunningBehind(
        /* [out] */ Boolean* consumer);

    CARAPI LockCanvas(
        /* [in] */ IRect * pDirty,
        /* [out] */ ICanvas ** ppCanvas);

    CARAPI UnlockCanvasAndPost(
        /* [in] */ ICanvas * pCanvas);

    CARAPI UnlockCanvas(
        /* [in] */ ICanvas * pCanvas);

    /** @hide */
    CARAPI SetLayer(
        /* [in] */ Int32 zorder);

    /** @hide */
    CARAPI SetPosition(
        /* [in] */ Float x,
        /* [in] */ Float y);

    /** @hide */
    CARAPI SetSize(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    CARAPI GetWidth(
        /* [out] */ Int32* w);

    CARAPI GetHeight(
        /* [out] */ Int32* h);

    /** @hide */
    CARAPI Hide();

    /** @hide */
    CARAPI Show();

    /** @hide */
    CARAPI SetTransparentRegionHint(
        /* [in] */ IRegion* region);

    /** @hide */
    CARAPI SetAlpha(
        /* [in] */ Float alpha);

    /** @hide */
    CARAPI SetMatrix(
        /* [in] */ Float dsdx,
        /* [in] */ Float dtdx,
        /* [in] */ Float dsdy,
        /* [in] */ Float dtdy);

    /** @hide */
    CARAPI SetFlags(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 mask);

    /** @hide */
    CARAPI SetWindowCrop(
        /* [in] */ IRect* crop);

    /** @hide */
    CARAPI SetLayerStack(
        /* [in] */ Int32 layerStack);

    /**
     * Copy another surface to this one.  This surface now holds a reference
     * to the same data as the original surface, and is -not- the owner.
     * This is for use by the window manager when returning a window surface
     * back from a client, converting it from the representation being managed
     * by the window manager to the representation the client uses to draw
     * in to it.
     * @hide
     */
    CARAPI CopyFrom(
        /* [in] */ ISurface* other);

    /**
     * Transfer the native state from 'other' to this surface, releasing it
     * from 'other'.  This is for use in the client side for drawing into a
     * surface; not guaranteed to work on the window manager side.
     * This is for use by the client to move the underlying surface from
     * one Surface object to another, in particular in SurfaceFlinger.
     * @hide.
     */
    CARAPI TransferFrom(
        /* [in] */ ISurface* other);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetSurface(
        /* [out] */ Handle32* surface);

    CARAPI SetSurface(
        /* [in] */ Handle32 surface);

    CARAPI GetCanvas(
        /* [out] */ ICanvas** canvas);

    CARAPI GetCanvasSaveCount(
        /* [out] */ Int32* count);

    CARAPI GetNativeSurfaceControl(
        /* [out] */ Handle32* surfaceControl);
//protected:
public:
    /**
     * Sets the translator used to scale canvas's width/height in compatibility
     * mode.
     */
    CARAPI SetCompatibilityTranslator(
        /* [in] */ ICompatibilityInfoTranslator* translator);

private:
    static CARAPI CheckHeadless();

private:
    CARAPI NativeCreate(
        /* [in] */ ISurfaceSession* session,
        /* [in] */ const String& name,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 format,
        /* [in] */ Int32 flags);

    CARAPI NativeCreateFromSurfaceTexture(
        /* [in] */ ISurfaceTexture* surfaceTexture);

    CARAPI NativeRelease();

    CARAPI NativeDestroy();

    CARAPI_(Boolean) NativeIsValid();

    CARAPI_(Int32) NativeGetIdentity();

    CARAPI_(Boolean) NativeIsConsumerRunningBehind();

    CARAPI NativeLockCanvas(
        /* [in] */ IRect* dirty,
        /* [out] */ ICanvas** canvas);

    CARAPI NativeUnlockCanvasAndPost(
        /* [in] */ ICanvas* canvas);

    static CARAPI NativeOpenTransaction();

    static CARAPI NativeCloseTransaction();

    CARAPI NativeSetLayer(
        /* [in] */ Int32 zorder);

    CARAPI NativeSetPosition(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI NativeSetSize(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    CARAPI NativeSetTransparentRegionHint(
        /* [in] */ IRegion* region);

    CARAPI NativeSetAlpha(
        /* [in] */ Float alpha);

    CARAPI NativeSetMatrix(
        /* [in] */ Float dsdx,
        /* [in] */ Float dtdx,
        /* [in] */ Float dsdy,
        /* [in] */ Float dtdy);

    CARAPI NativeSetFlags(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 mask);

    CARAPI NativeSetWindowCrop(
        /* [in] */ IRect* crop);

    CARAPI NativeSetLayerStack(
        /* [in] */ Int32 layerStack);

    CARAPI NativeCopyFrom(
        /* [in] */ ISurface* other);

    CARAPI NativeTransferFrom(
        /* [in] */ ISurface* other);

    CARAPI NativeReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI NativeWriteToParcel(
        /* [in] */ IParcel* dest);

    void SetSurfaceControl(
        /* [in] */ const android::sp<android::SurfaceControl>& surface);

    CARAPI_(void) GetSurface();

    CARAPI_(void) SetSurface(
        /* [in] */ const android::sp<android::Surface>& surface);

    static CARAPI_(Boolean) GetResult();

private:
    static const char* TAG;
    static Boolean HEADLESS;

    //AutoPtr<ICloseGuard> mCloseGuard = CloseGuard.get();
    String mName;

    // Note: These fields are accessed by native code.
    // The mSurfaceControl will only be present for Surfaces used by the window
    // server or system processes. When this class is parceled we defer to the
    // mSurfaceControl to do the parceling. Otherwise we parcel the
    // mNativeSurface.
    android::sp<android::Surface> mNativeSurface;
    android::sp<android::SurfaceControl> mNativeSurfaceControl;
    Int32 mGenerationId; // incremented each time mNativeSurface changes
    AutoPtr<ICanvas> mCanvas;
    Int32 mCanvasSaveCount; // Canvas save count at time of lockCanvas()

    // The Translator for density compatibility mode.  This is used for scaling
    // the canvas to perform the appropriate density transformation.
    AutoPtr<ICompatibilityInfoTranslator> mCompatibilityTranslator;

    // A matrix to scale the matrix set by application. This is set to null for
    // non compatibility mode.
    AutoPtr<IMatrix> mCompatibleMatrix;

    Int32 mWidth;
    Int32 mHeight;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif // __CSURFACE_H__
