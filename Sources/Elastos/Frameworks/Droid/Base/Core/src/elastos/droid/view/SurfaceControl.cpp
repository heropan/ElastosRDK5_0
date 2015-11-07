#include "elastos/droid/view/SurfaceControl.h"

namespace Elastos {
namespace Droid {
namespace View {

SurfaceControl::SurfaceControl()
    : mNativeObject(0)
    , mName(NULL)
{}

SurfaceControl::~SurfaceControl()
{
    // try {
        if (mCloseGuard != NULL) {
            mCloseGuard->WarnIfOpen();
        }
        if (mNativeObject != 0) {
            NativeRelease(mNativeObject);
        }
    // } finally {
        // super.finalize();
    // }
}

ECode SurfaceControl::ReleaseSurfaceControl()
{
    if (mNativeObject != 0) {
        NativeRelease(mNativeObject);
        mNativeObject = 0;
    }
    mCloseGuard->Close();
    return NOERROR;
}

ECode SurfaceControl::SetLayer(
    /* [in] */ Int32 zorder)
{
    FAIL_RETURN(CheckNotReleased())
    NativeSetLayer(mNativeObject, zorder);
    return NOERROR;
}

ECode SurfaceControl::Destroy()
{
    if (mNativeObject != 0) {
        NativeDestroy(mNativeObject);
        mNativeObject = 0;
    }
    mCloseGuard->Close();
    return NOERROR;
}

ECode SurfaceControl::SetPosition(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    FAIL_RETURN(CheckNotReleased())
    NativeSetPosition(mNativeObject, x, y);
    return NOERROR;
}

ECode SurfaceControl::SetSize(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    FAIL_RETURN(CheckNotReleased())
    NativeSetSize(mNativeObject, w, h);
    return NOERROR;
}

ECode SurfaceControl::Hide()
{
    FAIL_RETURN(CheckNotReleased())
    NativeSetFlags(mNativeObject, SURFACE_HIDDEN, SURFACE_HIDDEN);
    return NOERROR;
}

ECode SurfaceControl::Show()
{
    FAIL_RETURN(CheckNotReleased())
    NativeSetFlags(mNativeObject, 0, SURFACE_HIDDEN);
    return NOERROR;
}

ECode SurfaceControl::SetTransparentRegionHint(
    /* [in] */ IRegion* region)
{
    FAIL_RETURN(CheckNotReleased())
    NativeSetTransparentRegionHint(mNativeObject, region);
    return NOERROR;
}

ECode SurfaceControl::ClearContentFrameStats(
    /* [out] */ Boolean* result)
{
    FAIL_RETURN(CheckNotReleased())
    *result = NativeClearContentFrameStats(mNativeObject);
    return NOERROR;
}

ECode SurfaceControl::GetContentFrameStats(
    /* [in] */ IWindowContentFrameStats* outStats,
    /* [out] */ Boolean* result)
{
    FAIL_RETURN(CheckNotReleased())
    *result = NativeGetContentFrameStats(mNativeObject, outStats);
    return NOERROR;
}

ECode SurfaceControl::SetAlpha(
    /* [in] */ Float alpha)
{
    FAIL_RETURN(CheckNotReleased())
    NativeSetAlpha(mNativeObject, alpha);
    return NOERROR;
}

ECode SurfaceControl::SetMatrix(
    /* [in] */ Float dsdx,
    /* [in] */ Float dtdx,
    /* [in] */ Float dsdy,
    /* [in] */ Float dtdy)
{
    FAIL_RETURN(CheckNotReleased())
    NativeSetMatrix(mNativeObject, dsdx, dtdx, dsdy, dtdy);
    return NOERROR;
}

ECode SurfaceControl::SetWindowCrop(
    /* [in] */ IRect* crop)
{
    Int32 left, top, right, bottom;
    crop->GetLeft(&left);
    crop->GetTop(&top);
    crop->GetRight(&right);
    crop->GetBottom(&bottom);

    FAIL_RETURN(CheckNotReleased())
    if (crop != NULL) {
        NativeSetWindowCrop(mNativeObject, left, top, right, bottom);
    } else {
        NativeSetWindowCrop(mNativeObject, 0, 0, 0, 0);
    }
    return NOERROR;
}

ECode SurfaceControl::SetLayerStack(
    /* [in] */ Int32 layerStack)
{
    FAIL_RETURN(CheckNotReleased())
    NativeSetLayerStack(mNativeObject, layerStack);
    return NOERROR;
}

ECode SurfaceControl::SetOpaque(
    /* [in] */ Boolean isOpaque)
{
    FAIL_RETURN(CheckNotReleased())
    if (isOpaque) {
        NativeSetFlags(mNativeObject, SURFACE_OPAQUE, SURFACE_OPAQUE);
    } else {
        NativeSetFlags(mNativeObject, 0, SURFACE_OPAQUE);
    }
    return NOERROR;
}

ECode SurfaceControl::ToString(
    /* [out] */ String* str)
{
    *str = String("Surface(name=") + mName + ")";
    return NOERROR;
}

ECode SurfaceControl::SetDisplayPowerMode(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 mode)
{
    if (displayToken == NULL) {
        // throw new IllegalArgumentException("displayToken must not be null");
        SLOGGERE(TAG, "displayToken must not be null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    NativeSetDisplayPowerMode(displayToken, mode);
    return NOERROR;
}

AutoPtr<ArrayOf<IPhysicalDisplayInfo*> > SurfaceControl::GetDisplayConfigs(
    /* [in] */ IBinder* displayToken)
{
    if (displayToken == NULL) {
        // throw new IllegalArgumentException("displayToken must not be null");
        SLOGGERE(TAG, "displayToken must not be null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NativeGetDisplayConfigs(displayToken);
}

Int32 SurfaceControl::GetActiveConfig(
    /* [in] */ IBinder* displayToken)
{
    if (displayToken == NULL) {
        // throw new IllegalArgumentException("displayToken must not be null");
        SLOGGERE(TAG, "displayToken must not be null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NativeGetActiveConfig(displayToken);
}

Boolean SurfaceControl::SetActiveConfig(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 id)
{
    if (displayToken == NULL) {
        // throw new IllegalArgumentException("displayToken must not be null");
        SLOGGERE(TAG, "displayToken must not be null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NativeSetActiveConfig(displayToken, id);
}

ECode SurfaceControl::SetDisplayProjection(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 orientation,
    /* [in] */ IRect* layerStackRect,
    /* [in] */ IRect* displayRect)
{
    if (displayToken == NULL) {
        // throw new IllegalArgumentException("displayToken must not be null");
        SLOGGERE(TAG, "displayToken must not be null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (layerStackRect == NULL) {
        // throw new IllegalArgumentException("layerStackRect must not be null");
        SLOGGERE(TAG, "layerStackRect must not be null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (displayRect == NULL) {
        // throw new IllegalArgumentException("displayRect must not be null");
        SLOGGERE(TAG, "displayRect must not be null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 lLeft, dLeft, lRight, dRight, lTop, dTop, lBottom, dBottom;

    layerStackRect->GetLeft(&lLeft);
    layerStackRect->GetTop(&lTop);
    layerStackRect->GetRight(&lRight);
    layerStackRect->GetBottom(&lBottom);

    displayRect->GetLeft(&dLeft);
    displayRect->GetTop(&dTop);
    displayRect->GetRight(&dRight);
    displayRect->GetBottom(&dBottom);

    NativeSetDisplayProjection(displayToken, orientation,
            lLeft, lTop, lRight, lBottom,
            dLeft, dTop, dRight, dBottom);
}

ECode SurfaceControl::SetDisplayLayerStack(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 layerStack)
{
    if (displayToken == NULL) {
        // throw new IllegalArgumentException("displayToken must not be null");
        SLOGGERE(TAG, "displayToken must not be null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    NativeSetDisplayLayerStack(displayToken, layerStack);
    return NOERROR;
}

ECode SurfaceControl::SetDisplaySurface(
    /* [in] */ IBinder* displayToken,
    /* [in] */ ISurface* surface)
{
    if (displayToken == NULL) {
        // throw new IllegalArgumentException("displayToken must not be null");
        SLOGGERE(TAG, "displayToken must not be null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (surface != NULL) {
        AutoLock lock(this);
        Surface* imp = (Surface*)surface;
        NativeSetDisplaySurface(displayToken, imp->mNativeObject);
    } else {
        NativeSetDisplaySurface(displayToken, 0);
    }
    return NOERROR;
}

ECode SurfaceControl::SetDisplaySize(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (displayToken == NULL) {
        // throw new IllegalArgumentException("displayToken must not be null");
        SLOGGERE(TAG, "displayToken must not be null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (width <= 0 || height <= 0) {
        // throw new IllegalArgumentException("width and height must be positive");
        SLOGGERE(TAG, "width and height must be positive")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    NativeSetDisplaySize(displayToken, width, height);
    return NOERROR;
}

ECode SurfaceControl::CreateDisplay(
    /* [in] */ const String& name,
    /* [in] */ Boolean secure,
    /* [out] */ IBinder** token)
{
    if (name == NULL) {
        throw new IllegalArgumentException("name must not be null");
        SLOGGERE(TAG, "name must not be null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *token = NativeCreateDisplay(name, secure);
    REFCOUNT_ADD(*token)
    return NOERROR;
}

ECode SurfaceControl::DestroyDisplay(
    /* [in] */ IBinder* displayToken)
{
    if (displayToken == NULL) {
        // throw new IllegalArgumentException("displayToken must not be null");
        SLOGGERE(TAG, "displayToken must not be null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    NativeDestroyDisplay(displayToken);
    return NOERROR;
}

AutoPtr<IBinder> SurfaceControl::GetBuiltInDisplay(
    /* [in] */ Int32 builtInDisplayId)
{
    return NativeGetBuiltInDisplay(builtInDisplayId);
}
/**
 * Copy the current screen contents into the provided {@link Surface}
 *
 * @param display The display to take the screenshot of.
 * @param consumer The {@link Surface} to take the screenshot into.
 * @param width The desired width of the returned bitmap; the raw
 * screen will be scaled down to this size.
 * @param height The desired height of the returned bitmap; the raw
 * screen will be scaled down to this size.
 * @param minLayer The lowest (bottom-most Z order) surface layer to
 * include in the screenshot.
 * @param maxLayer The highest (top-most Z order) surface layer to
 * include in the screenshot.
 * @param useIdentityTransform Replace whatever transformation (rotation,
 * scaling, translation) the surface layers are currently using with the
 * identity transformation while taking the screenshot.
 */
 void SurfaceControl::Screenshot(
    /* [in] */ IBinder* display,
    /* [in] */ ISurface* consumer,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 minLayer,
    /* [in] */ Int32 maxLayer,
    /* [in] */ Boolean useIdentityTransform)
 {
    AutoPtr<IRect> r;
    CRect::New((IRect**)&r);
    Screenshot(display, consumer, r, width, height, minLayer, maxLayer,
            FALSE, useIdentityTransform);
 }

/**
 * Copy the current screen contents into the provided {@link Surface}
 *
 * @param display The display to take the screenshot of.
 * @param consumer The {@link Surface} to take the screenshot into.
 * @param width The desired width of the returned bitmap; the raw
 * screen will be scaled down to this size.
 * @param height The desired height of the returned bitmap; the raw
 * screen will be scaled down to this size.
 */
 void SurfaceControl::Screenshot(
    /* [in] */ IBinder* display,
    /* [in] */ ISurface* consumer,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
 {
    AutoPtr<IRect> r;
    CRect::New((IRect**)&r);
    Screenshot(display, consumer, r, width, height, 0, 0, TRUE, FALSE);
 }

/**
 * Copy the current screen contents into the provided {@link Surface}
 *
 * @param display The display to take the screenshot of.
 * @param consumer The {@link Surface} to take the screenshot into.
 */
 void SurfaceControl::Screenshot(
    /* [in] */ IBinder* display,
    /* [in] */ ISurface* consumer)
 {
    AutoPtr<IRect> r;
    CRect::New((IRect**)&r);
    Screenshot(display, consumer, r, 0, 0, 0, 0, TRUE, FALSE);
 }

/**
 * Copy the current screen contents into a bitmap and return it.
 *
 * CAVEAT: Versions of screenshot that return a {@link Bitmap} can
 * be extremely slow; avoid use unless absolutely necessary; prefer
 * the versions that use a {@link Surface} instead, such as
 * {@link SurfaceControl#screenshot(IBinder, Surface)}.
 *
 * @param sourceCrop The portion of the screen to capture into the Bitmap;
 * caller may pass in 'new Rect()' if no cropping is desired.
 * @param width The desired width of the returned bitmap; the raw
 * screen will be scaled down to this size.
 * @param height The desired height of the returned bitmap; the raw
 * screen will be scaled down to this size.
 * @param minLayer The lowest (bottom-most Z order) surface layer to
 * include in the screenshot.
 * @param maxLayer The highest (top-most Z order) surface layer to
 * include in the screenshot.
 * @param useIdentityTransform Replace whatever transformation (rotation,
 * scaling, translation) the surface layers are currently using with the
 * identity transformation while taking the screenshot.
 * @param rotation Apply a custom clockwise rotation to the screenshot, i.e.
 * Surface.ROTATION_0,90,180,270. Surfaceflinger will always take
 * screenshots in its native portrait orientation by default, so this is
 * useful for returning screenshots that are independent of device
 * orientation.
 * @return Returns a Bitmap containing the screen contents, or null
 * if an error occurs. Make sure to call Bitmap.recycle() as soon as
 * possible, once its content is not needed anymore.
 */
 AutoPtr<Bitmap> SurfaceControl::Screenshot(
    /* [in] */ IRect* sourceCrop,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 minLayer,
    /* [in] */ Int32 maxLayer,
    /* [in] */ Boolean useIdentityTransform,
    /* [in] */ Int32 rotation)
{
    // TODO: should take the display as a parameter
    AutoPtr<IBinder> displayToken = GetBuiltInDisplay(
            BUILT_IN_DISPLAY_ID_MAIN);
    return NativeScreenshot(displayToken, sourceCrop, width, height,
            minLayer, maxLayer, FALSE, useIdentityTransform, rotation);
}
/**
 * Like {@link SurfaceControl#screenshot(int, int, int, int, boolean)} but
 * includes all Surfaces in the screenshot.
 *
 * @param width The desired width of the returned bitmap; the raw
 * screen will be scaled down to this size.
 * @param height The desired height of the returned bitmap; the raw
 * screen will be scaled down to this size.
 * @return Returns a Bitmap containing the screen contents, or null
 * if an error occurs. Make sure to call Bitmap.recycle() as soon as
 * possible, once its content is not needed anymore.
 */
 AutoPtr<Bitmap> SurfaceControl::Screenshot(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    // TODO: should take the display as a parameter
    AutoPtr<IBinder>  displayToken = GetBuiltInDisplay(
            BUILT_IN_DISPLAY_ID_MAIN);
    AutoPtr<IRect> r;
    CRect::New((IRect**)&r);
    return NativeScreenshot(displayToken, r, width, height, 0, 0, TRUE,
            FALSE, ISurface::ROTATION_0);
}

Boolean SurfaceControl::ClearAnimationFrameStas()
{
    return NativeClearAnimationFrameStats();
}

Boolean SurfaceControl::GetAnimationFrameStats(
    /* [in] */ IWindowAnimationFrameStats* outStats)
{
    return NativeGetAnimationFrameStats(outStats);
}
/** start a transaction */

void SurfaceControl::OpenTransaction()
{
    NativeOpenTransaction();
}

/** end a transaction */
void SurfaceControl::CloseTransaction()
{
    NativeCloseTransaction();
}

/** flag the transaction as an animation */
void SurfaceControl::SetAnimationTransaction()
{
    NativeSetAnimationTransaction();
}

/**
 * Create a surface with a name.
 * <p>
 * The surface creation flags specify what kind of surface to create and
 * certain options such as whether the surface can be assumed to be opaque
 * and whether it should be initially hidden.  Surfaces should always be
 * created with the {@link #HIDDEN} flag set to ensure that they are not
 * made visible prematurely before all of the surface's properties have been
 * configured.
 * <p>
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
 *
 * @throws throws OutOfResourcesException If the SurfaceControl cannot be created.
 */
ECode SurfaceControl::constructor(
    /* [in] */ ISurfaceSession* session,
    /* [in] */ const String& name,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 format,
    /* [in] */ Int32 flags);

ECode SurfaceControl::CheckNotReleased()
{
    if (mNativeObject == 0) {
        SLOGGERE(TAG, "mNativeObject is null. Have you called release() already?")
        return E_NULL_POINTER_EXCEPTION;
    }
}

void SurfaceControl::Screenshot(
    /* [in] */ IBinder display,
    /* [in] */ ISurface* consumer,
    /* [in] */ IRect* sourceCrop,
    /* [in] */ width,
    /* [in] */ Int32  height,
    /* [in] */ Int32  minLayer,
    /* [in] */ Int32  maxLayer, boolean allLayers,
    /* [in] */ Boolean useIdentityTransform)
{
    if (display == NULL) {
        // throw new IllegalArgumentException("displayToken must not be null");
        SLOGGERE(TAG, "displayToken must not be null")
    }
    if (consumer == NULL) {
        // throw new IllegalArgumentException("consumer must not be null");
        SLOGGERE(TAG, "consumer must not be null")
    }
    NativeScreenshot(display, consumer, sourceCrop, width, height,
            minLayer, maxLayer, allLayers, useIdentityTransform);
}

Int64 SurfaceControl::NativeCreate(
    /* [in] */ ISurfaceSession* session,
    /* [in] */ const String& name,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 format,
    /* [in] */ Int32 flags)
{
    ScopedUtfChars name(env, nameStr);
    return reinterpret_cast<SurfaceComposerClient*>(
            env->GetLongField(surfaceSessionObj, gSurfaceSessionClassInfo.mNativeClient));

    sp<SurfaceComposerClient> client(android_view_SurfaceSession_getClient(env, sessionObj));
    sp<SurfaceControl> surface = client->createSurface(
            String8(name.c_str()), w, h, format, flags);
    if (surface == NULL) {
        jniThrowException(env, OutOfResourcesException, NULL);
        return 0;
    }
    surface->incStrong((void *)nativeCreate);
    return reinterpret_cast<jlong>(surface.get());
}

CARAPI_(void) NativeRelease(
    /* [in] */ Int64 nativeObject);

CARAPI_(void) NativeDestroy(
    /* [in] */ Int64 nativeObject);

CARAPI_(AutoPtr<IBitmap>) NativeScreenshot(
    /* [in] */ IBinder* displayToken,
    /* [in] */ IRect* sourceCrop,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 minLayer,
    /* [in] */ Int32 maxLayer,
    /* [in] */ Boolean allLayers,
    /* [in] */ Boolean useIdentityTransform,
    /* [in] */ Int32 rotation);

CARAPI_(void) NativeScreenshot(
    /* [in] */ IBinder* displayToken,
    /* [in] */ ISurface* consumer,
    /* [in] */ IRect* sourceCrop,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 minLayer,
    /* [in] */ Int32 maxLayer,
    /* [in] */ Boolean allLayers,
    /* [in] */ Boolean useIdentityTransform);

CARAPI_(void) NativeOpenTransaction();

CARAPI_(void) NativeCloseTransaction();

CARAPI_(void) NativeSetAnimationTransaction();

CARAPI_(void) NativeSetLayer(
    /* [in] */ Int64 nativeObject,
    /* [in] */ Int32 zorder);

CARAPI_(void) NativeSetPosition(
    /* [in] */ Int64 nativeObject,
    /* [in] */ Float x,
    /* [in] */ Float y);

CARAPI_(void) NativeSetSize(
    /* [in] */ Int64 nativeObject,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h);

CARAPI_(void) NativeSetTransparentRegionHint(
    /* [in] */ Int64 nativeObject,
    /* [in] */ IRegion* region);

CARAPI_(void) NativeSetAlpha(
    /* [in] */ Int64 nativeObject,
    /* [in] */ Float alpha);

CARAPI_(void) NativeSetMatrix(
    /* [in] */ Int64 nativeObject,
    /* [in] */ Float dsdx,
    /* [in] */ Float dtdx,
    /* [in] */ Float dsdy,
    /* [in] */ Float dtdy);

CARAPI_(void) NativeSetFlags(
    /* [in] */ Int64 nativeObject,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mask);

CARAPI_(void) NativeSetWindowCrop(
    /* [in] */ Int64 nativeObject,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b);

CARAPI_(void) NativeSetLayerStack(
    /* [in] */ Int64 nativeObject,
    /* [in] */ Int32 layerStack);

CARAPI_(Boolean) NativeClearContentFrameStats(
    /* [in] */ Int64 nativeObject);

CARAPI_(Boolean) NativeGetContentFrameStats(
    /* [in] */ Int64 nativeObject,
    /* [in] */ IWindowContentFrameStats* outStats);

CARAPI_(Boolean) NativeClearAnimationFrameStats();

CARAPI_(Boolean) NativeGetAnimationFrameStats(
    /* [in] */ IWindowAnimationFrameStats* outStats);


CARAPI_(AutoPtr<IBinder>) NativeGetBuiltInDisplay(
    /* [in] */ Int32 physicalDisplayId);

CARAPI_(AutoPtr<IBinder>) NativeCreateDisplay(
    /* [in] */ const String& name,
    /* [in] */ Boolean secure);

CARAPI_(void) NativeDestroyDisplay(
    /* [in] */ IBinder* displayToken);

CARAPI_(void) NativeSetDisplaySurface(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int64 nativeSurfaceObject);

CARAPI_(void) NativeSetDisplayLayerStack(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 layerStack);

CARAPI_(void) NativeSetDisplayProjection(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 orientation,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b,
    /* [in] */ Int32 L,
    /* [in] */ Int32 T,
    /* [in] */ Int32 R,
    /* [in] */ Int32 B);

CARAPI_(void) NativeSetDisplaySize(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height);

CARAPI_(AutoPtr<ArrayOf<IPhysicalDisplayInfo*> >) NativeGetDisplayConfigs(
    /* [in] */ IBinder* displayToken);

CARAPI_(Int32) NativeGetActiveConfig(
    /* [in] */ IBinder* displayToken);

CARAPI_(Boolean) NativeSetActiveConfig(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 id);

CARAPI_(void) NativeSetDisplayPowerMode(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 mode);
} // namespace View
} // namespace Droid
} // namespace Elastos
