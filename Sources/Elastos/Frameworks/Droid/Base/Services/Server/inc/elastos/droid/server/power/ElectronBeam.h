#ifndef __ELASTOS_DROID_SERVER_POWER_ELECTRONBEAM_H__
#define __ELASTOS_DROID_SERVER_POWER_ELECTRONBEAM_H__

#include "elastos/droid/ext/frameworkdef.h"
#include "_Elastos.Droid.Server.h"
#include "display/CDisplayManagerService.h"

using Elastos::IO::IFloatBuffer;
using Elastos::IO::IByteBuffer;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::IDisplayInfo;
using Elastos::Droid::View::ISurface;
using Elastos::Droid::View::ISurfaceSession;
using Elastos::Droid::Server::Display::IDisplayTransactionListener;
using Elastos::Droid::Server::Display::CDisplayManagerService;
using Elastos::Droid::Opengl::IEGLDisplay;
using Elastos::Droid::Opengl::IEGLConfig;
using Elastos::Droid::Opengl::IEGLContext;
using Elastos::Droid::Opengl::IEGLSurface;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

/**
 * Bzzzoooop!  *crackle*
 * <p>
 * Animates a screen transition from on to off or off to on by applying
 * some GL transformations to a screenshot.
 * </p><p>
 * This component must only be created or accessed by the {@link Looper} thread
 * that belongs to the {@link DisplayPowerController}.
 * </p>
 */
class ElectronBeam : public ElRefBase
{
private:
    class NaturalSurfaceLayout
        : public ElRefBase
        , public IDisplayTransactionListener
    {
    public:
        NaturalSurfaceLayout(
            /* [in] */ CDisplayManagerService* displayManager,
            /* [in] */ ISurface* surface);

        CAR_INTERFACE_DECL()

        CARAPI_(void) Dispose();

        CARAPI OnDisplayTransaction();

    private:
        AutoPtr<CDisplayManagerService> mDisplayManager;
        AutoPtr<ISurface> mSurface;
        Object mLock;
    };

public:
    ElectronBeam(
        /* [in] */ CDisplayManagerService* displayManager);

    /**
     * Warms up the electron beam in preparation for turning on or off.
     * This method prepares a GL context, and captures a screen shot.
     *
     * @param mode The desired mode for the upcoming animation.
     * @return True if the electron beam is ready, false if it is uncontrollable.
     */
    CARAPI_(Boolean) Prepare(
        /* [in] */ Int32 mode);

    /**
     * Dismisses the electron beam animation surface and cleans up.
     *
     * To prevent stray photons from leaking out after the electron beam has been
     * turned off, it is a good idea to defer dismissing the animation until the
     * electron beam has been turned back on fully.
     */
    CARAPI_(void) Dismiss();

    /**
     * Draws an animation frame showing the electron beam activated at the
     * specified level.
     *
     * @param level The electron beam level.
     * @return True if successful.
     */
    CARAPI_(Boolean) Draw(
        /* [in] */ Float level);

    // CARAPI_(void) Dump(
    //     /* [in] */ IPrintWriter* pw);

private:
    CARAPI_(Boolean) TryPrepare();

    /**
     * Draws a frame where the content of the electron beam is collapsing inwards upon
     * itself vertically with red / green / blue channels dispersing and eventually
     * merging down to a single horizontal line.
     *
     * @param stretch The stretch factor.  0.0 is no collapse, 1.0 is full collapse.
     */
    CARAPI_(void) DrawVStretch(
        /* [in] */ Float stretch);

    /**
     * Draws a frame where the electron beam has been stretched out into
     * a thin white horizontal line that fades as it expands outwards.
     *
     * @param stretch The stretch factor.  0.0 is no stretch / no fade,
     * 1.0 is maximum stretch / maximum fade.
     */
    CARAPI_(void) DrawHStretch(
        /* [in] */ Float stretch);

    static CARAPI_(void) SetVStretchQuad(
        /* [in] */ IFloatBuffer* vtx,
        /* [in] */ Float dw,
        /* [in] */ Float dh,
        /* [in] */ Float a);

    static CARAPI_(void) SetHStretchQuad(
        /* [in] */ IFloatBuffer* vtx,
        /* [in] */ Float dw,
        /* [in] */ Float dh,
        /* [in] */ Float a);

    static CARAPI_(void) SetQuad(
        /* [in] */ IFloatBuffer* vtx,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float w,
        /* [in] */ Float h);

    CARAPI_(Boolean) CaptureScreenshotTextureAndSetViewport();

    CARAPI_(void) DestroyScreenshotTexture();

    CARAPI_(Boolean) CreateEglContext();

    CARAPI_(Boolean) CreateSurface();

    CARAPI_(Boolean) CreateEglSurface();

    CARAPI_(void) DestroyEglSurface();

    CARAPI_(void) DestroySurface();

    CARAPI_(Boolean) ShowSurface(
        /* [in] */ Float alpha);

    CARAPI_(Boolean) AttachEglContext();

    CARAPI_(void) DetachEglContext();

    /**
     * Interpolates a value in the range 0 .. 1 along a sigmoid curve
     * yielding a result in the range 0 .. 1 scaled such that:
     * scurve(0) == 0, scurve(0.5) == 0.5, scurve(1) == 1.
     */
    static CARAPI_(Float) Scurve(
        /* [in] */ Float value,
        /* [in] */ Float s);

    static CARAPI_(Float) Sigmoid(
        /* [in] */ Float x,
        /* [in] */ Float s);

    static CARAPI_(Int32) NextPowerOfTwo(
        /* [in] */ Int32 value);

    static CARAPI_(AutoPtr<IFloatBuffer>) CreateNativeFloatBuffer(
        /* [in] */ Int32 size);

    static CARAPI_(void) LogEglError(
        /* [in] */ const String& func);

    static CARAPI_(Boolean) CheckGlErrors(
        /* [in] */ const String& func);

    static CARAPI_(Boolean) CheckGlErrors(
        /* [in] */ const String& func,
        /* [in] */ Boolean log);

public:
    /**
     * Animates an electron beam warming up.
     */
    static const Int32 MODE_WARM_UP;

    /**
     * Animates an electron beam shutting off.
     */
    static const Int32 MODE_COOL_DOWN;

    /**
     * Animates a simple dim layer to fade the contents of the screen in or out progressively.
     */
    static const Int32 MODE_FADE;

private:
    static const String TAG;

    static const Boolean DEBUG;

    // The layer for the electron beam surface.
    // This is currently hardcoded to be one layer above the boot animation.
    static const Int32 ELECTRON_BEAM_LAYER;

    // The relative proportion of the animation to spend performing
    // the horizontal stretch effect.  The remainder is spent performing
    // the vertical stretch effect.
    static const Float HSTRETCH_DURATION;
    static const Float VSTRETCH_DURATION;

    // The number of frames to draw when preparing the animation so that it will
    // be ready to run smoothly.  We use 3 frames because we are triple-buffered.
    // See code for details.
    static const Int32 DEJANK_FRAMES;

    // Set to true when the animation context has been fully prepared.
    Boolean mPrepared;
    Int32 mMode;

    AutoPtr<CDisplayManagerService> mDisplayManager;
    Int32 mDisplayLayerStack;// layer stack associated with primary display
    Int32 mDisplayRotation;
    Int32 mDisplayWidth;// real width, not rotated
    Int32 mDisplayHeight;// real height, not rotated
    AutoPtr<ISurfaceSession> mSurfaceSession;
    AutoPtr<ISurface> mSurface;
    AutoPtr<NaturalSurfaceLayout> mSurfaceLayout;
    AutoPtr<IEGLDisplay> mEglDisplay;
    AutoPtr<IEGLConfig> mEglConfig;
    AutoPtr<IEGLContext> mEglContext;
    AutoPtr<IEGLSurface> mEglSurface;
    Boolean mSurfaceVisible;
    Float mSurfaceAlpha;

    // Texture names.  We only use one texture, which contains the screenshot.
    AutoPtr< ArrayOf<Int32> > mTexNames;
    Boolean mTexNamesGenerated;

    // Vertex and corresponding texture coordinates.
    // We have 4 2D vertices, so 8 elements.  The vertices form a quad.
    AutoPtr<IFloatBuffer> mVertexBuffer;
    AutoPtr<IFloatBuffer> mTexCoordBuffer;
};

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_POWER_ELECTRONBEAM_H__
