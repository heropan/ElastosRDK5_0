#ifndef __ELASTOS_DROID_VIEW_SURFACECONTROL_H__
#define __ELASTOS_DROID_VIEW_SURFACECONTROL_H__

#include "elastos/droid/view/Surface.h"

namespace Elastos {
namespace Droid {
namespace View {

class SurfaceControl
    : public Object
    , public ISurfaceControl
{
public:
    SurfaceControl();

    ~SurfaceControl();

    CARAPI ReleaseSurfaceControl();

    CARAPI SetLayer(
        /* [in] */ Int32 zorder);

    CARAPI Destroy();

    CARAPI SetPosition(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI SetSize(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    CARAPI Hide();

    CARAPI Show();

    CARAPI SetTransparentRegionHint(
        /* [in] */ IRegion* region);

    CARAPI ClearContentFrameStats(
        /* [out] */ Boolean* result);

    CARAPI GetContentFrameStats(
        /* [in] */ IWindowContentFrameStats* outStats,
        /* [out] */ Boolean* result);

    CARAPI SetAlpha(
        /* [in] */ Float alpha);

    CARAPI SetMatrix(
        /* [in] */ Float dsdx,
        /* [in] */ Float dtdx,
        /* [in] */ Float dsdy,
        /* [in] */ Float dtdy);

    CARAPI SetWindowCrop(
        /* [in] */ IRect* crop);

    CARAPI SetLayerStack(
        /* [in] */ Int32 layerStack);

    CARAPI SetOpaque(
        /* [in] */ Boolean isOpaque);

    CARAPI ToString(
        /* [out] */ String* str);

    static CARAPI SetDisplayPowerMode(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 mode);

    static CARAPI_(AutoPtr<ArrayOf<IPhysicalDisplayInfo*> >) GetDisplayConfigs(
        /* [in] */ IBinder* displayToken);

    static CARAPI_(Int32) GetActiveConfig(
        /* [in] */ IBinder* displayToken);

    static CARAPI_(Boolean) SetActiveConfig(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 id);

    static CARAPI SetDisplayProjection(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 orientation,
        /* [in] */ IRect* layerStackRect,
        /* [in] */ IRect* displayRect);

    static CARAPI SetDisplayLayerStack(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 layerStack);

    static CARAPI SetDisplaySurface(
        /* [in] */ IBinder* displayToken,
        /* [in] */ ISurface* surface);

    static CARAPI SetDisplaySize(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    static CARAPI CreateDisplay(
        /* [in] */ String name,
        /* [in] */ Boolean secure,
        /* [out] */ IBinder** token);

    static CARAPI DestroyDisplay(
        /* [in] */ IBinder* displayToken);

    static CARAPI_(AutoPtr<IBinder>) GetBuiltInDisplay(
        /* [in] */ Int32 builtInDisplayId);
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
    static CARAPI_(void) Screenshot(
        /* [in] */ IBinder* display,
        /* [in] */ ISurface* consumer,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 minLayer,
        /* [in] */ Int32 maxLayer,
        /* [in] */ Boolean useIdentityTransform);

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
    static CARAPI_(void) Screenshot(
        /* [in] */ IBinder* display,
        /* [in] */ ISurface* consumer,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * Copy the current screen contents into the provided {@link Surface}
     *
     * @param display The display to take the screenshot of.
     * @param consumer The {@link Surface} to take the screenshot into.
     */
    static CARAPI_(void) Screenshot(
        /* [in] */ IBinder* display,
        /* [in] */ ISurface* consumer);

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
    static CARAPI_(AutoPtr<Bitmap>) Screenshot(
        /* [in] */ IRect* sourceCrop,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 minLayer,
        /* [in] */ Int32 maxLayer,
        /* [in] */ Boolean useIdentityTransform,
        /* [in] */ Int32 rotation);
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
    static CARAPI_(AutoPtr<Bitmap>) Screenshot(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    static CARAPI_(Boolean) ClearAnimationFrameStats();

    static CARAPI_(Boolean) GetAnimationFrameStats(
        /* [in] */ IWindowAnimationFrameStats* outStats);
    /** start a transaction */

    static CARAPI_(void) OpenTransaction();

    /** end a transaction */
    static CARAPI_(void) CloseTransaction();

    /** flag the transaction as an animation */
    static CARAPI_(void) SetAnimationTransaction();

protected:
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
    CARAPI constructor(
        /* [in] */ ISurfaceSession* session,
        /* [in] */ const String& name,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 format,
        /* [in] */ Int32 flags);

private:
    CARAPI CheckNotReleased();

    static CARAPI_(void) Screenshot(
        /* [in] */ IBinder display,
        /* [in] */ ISurface* consumer,
        /* [in] */ IRect* sourceCrop,
        /* [in] */ width,
        /* [in] */ Int32  height,
        /* [in] */ Int32  minLayer,
        /* [in] */ Int32  maxLayer, boolean allLayers,
        /* [in] */ Boolean useIdentityTransform);

    CARAPI_(Int64) NativeCreate(
        /* [in] */ ISurfaceSession* session,
        /* [in] */ const String& name,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 format,
        /* [in] */ Int32 flags);

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


    static CARAPI_(AutoPtr<IBinder>) NativeGetBuiltInDisplay(
        /* [in] */ Int32 physicalDisplayId);

    static CARAPI_(AutoPtr<IBinder>) NativeCreateDisplay(
        /* [in] */ const String& name,
        /* [in] */ Boolean secure);

    static CARAPI_(void) NativeDestroyDisplay(
        /* [in] */ IBinder* displayToken);

    static CARAPI_(void) NativeSetDisplaySurface(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int64 nativeSurfaceObject);

    static CARAPI_(void) NativeSetDisplayLayerStack(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 layerStack);

    static CARAPI_(void) NativeSetDisplayProjection(
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

    static CARAPI_(void) NativeSetDisplaySize(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    static CARAPI_(AutoPtr<ArrayOf<IPhysicalDisplayInfo*> >) NativeGetDisplayConfigs(
        /* [in] */ IBinder* displayToken);

    static CARAPI_(Int32) NativeGetActiveConfig(
        /* [in] */ IBinder* displayToken);

    static CARAPI_(Boolean) NativeSetActiveConfig(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 id);

    static CARAPI_(void) NativeSetDisplayPowerMode(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 mode);

protected:
    Int64 mNativeObject; // package visibility only for Surface.java access

private:
    static const String TAG;
    AutoPtr<ICloseGuard> mCloseGuard;
    String mName;
};

} // view
} // droid
} // elastos

#endif