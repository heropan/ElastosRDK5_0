
#include "power/ElectronBeam.h"
#include <elastos/Math.h>
#include <elastos/Slogger.h>
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::IO::IByteBufferHelper;
using Elastos::IO::CByteBufferHelper;
using Elastos::IO::ByteOrder;
using Elastos::IO::IByteOrderHelper;
using Elastos::IO::CByteOrderHelper;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::View::CDisplayInfo;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::View::ISurfaceHelper;
using Elastos::Droid::View::CSurfaceHelper;
using Elastos::Droid::View::CSurface;
using Elastos::Droid::View::CSurfaceSession;
using Elastos::Droid::Server::Display::EIID_IDisplayTransactionListener;
using Elastos::Droid::Opengl::IGLES10;
using Elastos::Droid::Opengl::CGLES10;
using Elastos::Droid::Opengl::IEGL14;
using Elastos::Droid::Opengl::CEGL14;
using Elastos::Droid::Opengl::IEGL14Helper;
using Elastos::Droid::Opengl::CEGL14Helper;
using Elastos::Droid::Opengl::IGLUtils;
using Elastos::Droid::Opengl::CGLUtils;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

ElectronBeam::NaturalSurfaceLayout::NaturalSurfaceLayout(
    /* [in] */ CDisplayManagerService* displayManager,
    /* [in] */ ISurface* surface)
    : mDisplayManager(displayManager)
    , mSurface(surface)
{}

CAR_INTERFACE_IMPL(ElectronBeam::NaturalSurfaceLayout, IDisplayTransactionListener)

void ElectronBeam::NaturalSurfaceLayout::Dispose()
{
    {
        Mutex::Autolock lock(mLock);
        mSurface = NULL;
    }
    mDisplayManager->UnregisterDisplayTransactionListener((IDisplayTransactionListener*)this);
}

ECode ElectronBeam::NaturalSurfaceLayout::OnDisplayTransaction()
{
    Mutex::Autolock lock(mLock);
    if (mSurface == NULL) {
        return NOERROR;
    }

    AutoPtr<IDisplayInfo> displayInfo;
    mDisplayManager->GetDisplayInfo(IDisplay::DEFAULT_DISPLAY, (IDisplayInfo**)&displayInfo);
    Int32 rotation;
    displayInfo->GetRotation(&rotation);
    switch (rotation) {
        case ISurface::ROTATION_0:
            mSurface->SetPosition(0, 0);
            mSurface->SetMatrix(1, 0, 0, 1);
            break;
        case ISurface::ROTATION_90: {
            Int32 logicalH;
            displayInfo->GetLogicalHeight(&logicalH);
            mSurface->SetPosition(0, logicalH);
            mSurface->SetMatrix(0, -1, 1, 0);
            break;
        }
        case ISurface::ROTATION_180: {
            Int32 logicalHeight, logicalWidth;
            displayInfo->GetLogicalHeight(&logicalHeight);
            displayInfo->GetLogicalWidth(&logicalWidth);
            mSurface->SetPosition(logicalWidth, logicalHeight);
            mSurface->SetMatrix(-1, 0, 0, -1);
            break;
        }
        case ISurface::ROTATION_270: {
            Int32 logicalWidth;
            displayInfo->GetLogicalWidth(&logicalWidth);
            mSurface->SetPosition(logicalWidth, 0);
            mSurface->SetMatrix(0, 1, -1, 0);
            break;
        }
    }
    return NOERROR;
}


const Int32 ElectronBeam::MODE_WARM_UP = 0;
const Int32 ElectronBeam::MODE_COOL_DOWN = 1;
const Int32 ElectronBeam::MODE_FADE = 2;
const String ElectronBeam::TAG("ElectronBeam");
const Boolean ElectronBeam::DEBUG = FALSE;
const Int32 ElectronBeam::ELECTRON_BEAM_LAYER = 0x40000001;
const Float ElectronBeam::HSTRETCH_DURATION = 0.5f;
const Float ElectronBeam::VSTRETCH_DURATION = 1.0f - HSTRETCH_DURATION;
const Int32 ElectronBeam::DEJANK_FRAMES = 3;

ElectronBeam::ElectronBeam(
    /* [in] */ CDisplayManagerService* displayManager)
    : mPrepared(FALSE)
    , mMode(0)
    , mDisplayManager(displayManager)
    , mDisplayLayerStack(0)
    , mDisplayRotation(0)
    , mDisplayWidth(0)
    , mDisplayHeight(0)
    , mSurfaceVisible(FALSE)
    , mSurfaceAlpha(0.0f)
    , mTexNamesGenerated(FALSE)
{
    mTexNames = ArrayOf<Int32>::Alloc(1);
    mVertexBuffer = CreateNativeFloatBuffer(8);
    mTexCoordBuffer = CreateNativeFloatBuffer(8);
}

Boolean ElectronBeam::Prepare(
    /* [in] */ Int32 mode)
{
    if (DEBUG) {
        Slogger::D(TAG, "prepare: mode=%d", mode);
    }

    mMode = mode;

    // Get the display size and layer stack.
    // This is not expected to change while the electron beam surface is showing.
    AutoPtr<IDisplayInfo> displayInfo;
    mDisplayManager->GetDisplayInfo(IDisplay::DEFAULT_DISPLAY, (IDisplayInfo**)&displayInfo);
    displayInfo->GetLayerStack(&mDisplayLayerStack);
    displayInfo->GetNaturalWidth(&mDisplayWidth);
    displayInfo->GetNaturalHeight(&mDisplayHeight);

    // Prepare the surface for drawing.
    if (!TryPrepare()) {
        Dismiss();
        return FALSE;
    }

    // Done.
    mPrepared = TRUE;

    // Dejanking optimization.
    // Some GL drivers can introduce a lot of lag in the first few frames as they
    // initialize their state and allocate graphics buffers for rendering.
    // Work around this problem by rendering the first frame of the animation a few
    // times.  The rest of the animation should run smoothly thereafter.
    // The frames we draw here aren't visible because we are essentially just
    // painting the screenshot as-is.
    if (mode == MODE_COOL_DOWN) {
        for (Int32 i = 0; i < DEJANK_FRAMES; i++) {
            Draw(1.0f);
        }
    }
    return TRUE;
}

Boolean ElectronBeam::TryPrepare()
{
    if (CreateSurface()) {
        if (mMode == MODE_FADE) {
            return TRUE;
        }
        return CreateEglContext()
                && CreateEglSurface()
                && CaptureScreenshotTextureAndSetViewport();
    }
    return FALSE;
}

void ElectronBeam::Dismiss()
{
    if (DEBUG) {
        Slogger::D(TAG, "dismiss");
    }

    DestroyScreenshotTexture();
    DestroyEglSurface();
    DestroySurface();
    mPrepared = FALSE;
}

Boolean ElectronBeam::Draw(
    /* [in] */ Float level)
{
    if (DEBUG) {
        Slogger::D(TAG, "drawFrame: level=%f", level);
    }

    if (!mPrepared) {
        return FALSE;
    }

    if (mMode == MODE_FADE) {
        return ShowSurface(1.0f - level);
    }

    if (!AttachEglContext()) {
        return FALSE;
    }
    // try {
    // Clear frame to solid black.
    AutoPtr<IGLES10> gl10;
    CGLES10::AcquireSingleton((IGLES10**)&gl10);
    gl10->GlClearColor(0.f, 0.f, 0.f, 1.f);
    gl10->GlClear(IGLES10::_GL_COLOR_BUFFER_BIT);

    // Draw the frame.
    if (level < HSTRETCH_DURATION) {
        DrawHStretch(1.0f - (level / HSTRETCH_DURATION));
    }
    else {
        DrawVStretch(1.0f - ((level - HSTRETCH_DURATION) / VSTRETCH_DURATION));
    }
    if (CheckGlErrors(String("drawFrame"))) {
        return FALSE;
    }

    AutoPtr<IEGL14> egl14;
    CEGL14::AcquireSingleton((IEGL14**)&egl14);
    Boolean rst;
    egl14->EglSwapBuffers(mEglDisplay, mEglSurface, &rst);
    // } finally {
    DetachEglContext();
    // }
    return ShowSurface(1.0f);
}

void ElectronBeam::DrawVStretch(
    /* [in] */ Float stretch)
{
    // compute interpolation scale factors for each color channel
    Float ar = Scurve(stretch, 7.5f);
    Float ag = Scurve(stretch, 8.0f);
    Float ab = Scurve(stretch, 8.5f);
    if (DEBUG) {
        Slogger::D(TAG, "drawVStretch: stretch=%f, ar=%f, ag=%f, ab=%f"
                , stretch, ar, ag, ab);
    }

    // set blending
    AutoPtr<IGLES10> gl10;
    CGLES10::AcquireSingleton((IGLES10**)&gl10);
    gl10->GlBlendFunc(IGLES10::_GL_ONE, IGLES10::_GL_ONE);
    gl10->GlEnable(IGLES10::_GL_BLEND);

    // bind vertex buffer
    gl10->GlVertexPointer(2, IGLES10::_GL_FLOAT, 0, mVertexBuffer);
    gl10->GlEnableClientState(IGLES10::_GL_VERTEX_ARRAY);

    // bind texture and set blending for drawing planes
    gl10->GlBindTexture(IGLES10::_GL_TEXTURE_2D, (*mTexNames)[0]);
    gl10->GlTexEnvx(IGLES10::_GL_TEXTURE_ENV, IGLES10::_GL_TEXTURE_ENV_MODE,
            mMode == MODE_WARM_UP ? IGLES10::_GL_MODULATE : IGLES10::_GL_REPLACE);
    gl10->GlTexParameterx(IGLES10::_GL_TEXTURE_2D,
            IGLES10::_GL_TEXTURE_MAG_FILTER, IGLES10::_GL_LINEAR);
    gl10->GlTexParameterx(IGLES10::_GL_TEXTURE_2D,
            IGLES10::_GL_TEXTURE_MIN_FILTER, IGLES10::_GL_LINEAR);
    gl10->GlTexParameterx(IGLES10::_GL_TEXTURE_2D,
            IGLES10::_GL_TEXTURE_WRAP_S, IGLES10::_GL_CLAMP_TO_EDGE);
    gl10->GlTexParameterx(IGLES10::_GL_TEXTURE_2D,
            IGLES10::_GL_TEXTURE_WRAP_T, IGLES10::_GL_CLAMP_TO_EDGE);
    gl10->GlEnable(IGLES10::_GL_TEXTURE_2D);
    gl10->GlTexCoordPointer(2, IGLES10::_GL_FLOAT, 0, mTexCoordBuffer);
    gl10->GlEnableClientState(IGLES10::_GL_TEXTURE_COORD_ARRAY);

    //draw the red plane
    SetVStretchQuad(mVertexBuffer, mDisplayWidth, mDisplayHeight, ar);
    gl10->GlColorMask(true, false, false, true);
    gl10->GlDrawArrays(IGLES10::_GL_TRIANGLE_FAN, 0, 4);

    // draw the green plane
    SetVStretchQuad(mVertexBuffer, mDisplayWidth, mDisplayHeight, ag);
    gl10->GlColorMask(false, true, false, true);
    gl10->GlDrawArrays(IGLES10::_GL_TRIANGLE_FAN, 0, 4);

    // draw the blue plane
    SetVStretchQuad(mVertexBuffer, mDisplayWidth, mDisplayHeight, ab);
    gl10->GlColorMask(false, false, true, true);
    gl10->GlDrawArrays(IGLES10::_GL_TRIANGLE_FAN, 0, 4);

    // clean up after drawing planes
    gl10->GlDisable(IGLES10::_GL_TEXTURE_2D);
    gl10->GlDisableClientState(IGLES10::_GL_TEXTURE_COORD_ARRAY);
    gl10->GlColorMask(true, true, true, true);

    // draw the white highlight (we use the last vertices)
    if (mMode == MODE_COOL_DOWN) {
        gl10->GlColor4f(ag, ag, ag, 1.0f);
        gl10->GlDrawArrays(IGLES10::_GL_TRIANGLE_FAN, 0, 4);
    }

    // clean up
    gl10->GlDisableClientState(IGLES10::_GL_VERTEX_ARRAY);
    gl10->GlDisable(IGLES10::_GL_BLEND);
}

void ElectronBeam::DrawHStretch(
       /* [in] */ Float stretch)
{
    // compute interpolation scale factor
    Float ag = Scurve(stretch, 8.0f);
    if (DEBUG) {
        Slogger::D(TAG, "drawHStretch: stretch=%f, ag=%f", stretch, ag);
    }

    if (stretch < 1.0f) {
        // bind vertex buffer
        AutoPtr<IGLES10> gl10;
        CGLES10::AcquireSingleton((IGLES10**)&gl10);
        gl10->GlVertexPointer(2, IGLES10::_GL_FLOAT, 0, mVertexBuffer);
        gl10->GlEnableClientState(IGLES10::_GL_VERTEX_ARRAY);

        // draw narrow fading white line
        SetHStretchQuad(mVertexBuffer, mDisplayWidth, mDisplayHeight, ag);
        gl10->GlColor4f(1.0f - ag, 1.0f - ag, 1.0f - ag, 1.0f);
        gl10->GlDrawArrays(IGLES10::_GL_TRIANGLE_FAN, 0, 4);

        // clean up
        gl10->GlDisableClientState(IGLES10::_GL_VERTEX_ARRAY);
    }
}

void ElectronBeam::SetVStretchQuad(
    /* [in] */ IFloatBuffer* vtx,
    /* [in] */ Float dw,
    /* [in] */ Float dh,
    /* [in] */ Float a)
{
    Float w = dw + (dw * a);
    Float h = dh - (dh * a);
    Float x = (dw - w) * 0.5f;
    Float y = (dh - h) * 0.5f;
    SetQuad(vtx, x, y, w, h);
}

void ElectronBeam::SetHStretchQuad(
    /* [in] */ IFloatBuffer* vtx,
    /* [in] */ Float dw,
    /* [in] */ Float dh,
    /* [in] */ Float a)
{
    Float w = dw + (dw * a);
    Float h = 1.0f;
    Float x = (dw - w) * 0.5f;
    Float y = (dh - h) * 0.5f;
    SetQuad(vtx, x, y, w, h);
}

void ElectronBeam::SetQuad(
    /* [in] */ IFloatBuffer* vtx,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float w,
    /* [in] */ Float h)
{
    if (DEBUG) {
        Slogger::D(TAG, "setQuad: x=%f, y=%f, w=%f, h=%f", x, y, w, h);
    }
    vtx->PutFloatEx(0, x);
    vtx->PutFloatEx(1, y);
    vtx->PutFloatEx(2, x);
    vtx->PutFloatEx(3, y + h);
    vtx->PutFloatEx(4, x + w);
    vtx->PutFloatEx(5, y + h);
    vtx->PutFloatEx(6, x + w);
    vtx->PutFloatEx(7, y);
}

Boolean ElectronBeam::CaptureScreenshotTextureAndSetViewport()
{
    // TODO: Use a SurfaceTexture to avoid the extra texture upload.
    AutoPtr<ISurfaceHelper> helper;
    ASSERT_SUCCEEDED(CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&helper));
    AutoPtr<IBitmap> bitmap;
    helper->ScreenshotEx(mDisplayWidth, mDisplayHeight,
            0, ELECTRON_BEAM_LAYER - 1, (IBitmap**)&bitmap);
    if (bitmap == NULL) {
        Slogger::E(TAG, "Could not take a screenshot!");
        return FALSE;
    }
    // try {
    if (!AttachEglContext()) {
        return FALSE;
    }
    // try {
    // assert(0);
    AutoPtr<IGLES10> gl10;
    CGLES10::AcquireSingleton((IGLES10**)&gl10);
    AutoPtr<IGLUtils> glUtils;
    CGLUtils::AcquireSingleton((IGLUtils**)&glUtils);

    if (!mTexNamesGenerated) {
        gl10->GlGenTextures(1, mTexNames, 0);
        if (CheckGlErrors(String("glGenTextures"))) {
            return FALSE;
        }
        mTexNamesGenerated = TRUE;
    }

    gl10->GlBindTexture(IGLES10::_GL_TEXTURE_2D, (*mTexNames)[0]);
    if (CheckGlErrors(String("glBindTexture"))) {
        return FALSE;
    }

    Float u = 1.0f;
    Float v = 1.0f;
    glUtils->TexImage2DEx2(IGLES10::_GL_TEXTURE_2D, 0, bitmap, 0);
    if (CheckGlErrors(String("glTexImage2D), first try"), FALSE)) {
        // Try a power of two size texture instead.
        Int32 tw = NextPowerOfTwo(mDisplayWidth);
        Int32 th = NextPowerOfTwo(mDisplayHeight);
        Int32 format;
        glUtils->GetInternalFormat(bitmap, &format);
        gl10->GlTexImage2D(IGLES10::_GL_TEXTURE_2D, 0,
                format, tw, th, 0,
                format, IGLES10::_GL_UNSIGNED_BYTE, NULL);
        if (CheckGlErrors(String("glTexImage2D, second try"))) {
            return FALSE;
        }

        glUtils->TexSubImage2D(IGLES10::_GL_TEXTURE_2D, 0, 0, 0, bitmap);
        if (CheckGlErrors(String("glTexSubImage2D"))) {
            return FALSE;
        }

        u = (Float)mDisplayWidth / tw;
        v = (Float)mDisplayHeight / th;
    }

    // Set up texture coordinates for a quad.
    // We might need to change this if the texture ends up being
    // a different size from the display for some reason.
    mTexCoordBuffer->PutFloatEx(0, 0.f);
    mTexCoordBuffer->PutFloatEx(1, v);
    mTexCoordBuffer->PutFloatEx(2, 0.f);
    mTexCoordBuffer->PutFloatEx(3, 0.f);
    mTexCoordBuffer->PutFloatEx(4, u);
    mTexCoordBuffer->PutFloatEx(5, 0.f);
    mTexCoordBuffer->PutFloatEx(6, u);
    mTexCoordBuffer->PutFloatEx(7, v);

    // Set up our viewport.
    gl10->GlViewport(0, 0, mDisplayWidth, mDisplayHeight);
    gl10->GlMatrixMode(IGLES10::_GL_PROJECTION);
    gl10->GlLoadIdentity();
    gl10->GlOrthof(0, mDisplayWidth, 0, mDisplayHeight, 0, 1);
    gl10->GlMatrixMode(IGLES10::_GL_MODELVIEW);
    gl10->GlLoadIdentity();
    gl10->GlMatrixMode(IGLES10::_GL_TEXTURE);
    gl10->GlLoadIdentity();
    // } finally {
    DetachEglContext();
        // }
    // } finally {
    bitmap->Recycle();
    // }
    return TRUE;
}

void ElectronBeam::DestroyScreenshotTexture()
{
    if (mTexNamesGenerated) {
        mTexNamesGenerated = FALSE;
        if (AttachEglContext()) {
            // try {
            AutoPtr<IGLES10> gl10;
            CGLES10::AcquireSingleton((IGLES10**)&gl10);
            gl10->GlDeleteTextures(1, mTexNames, 0);
            CheckGlErrors(String("glDeleteTextures"));
            // } finally {
            DetachEglContext();
            // }
        }
    }
}

Boolean ElectronBeam::CreateEglContext()
{
    AutoPtr<IEGL14> egl14;
    CEGL14::AcquireSingleton((IEGL14**)&egl14);
    AutoPtr<IEGL14Helper> eglHelper;
    CEGL14Helper::AcquireSingleton((IEGL14Helper**)&eglHelper);
    if (mEglDisplay == NULL) {
        egl14->EglGetDisplay(IEGL14::_EGL_DEFAULT_DISPLAY, (IEGLDisplay**)&mEglDisplay);
        AutoPtr<IEGLDisplay> noDisplay;
        eglHelper->GetNoDisplay((IEGLDisplay**)&noDisplay);
        if (mEglDisplay == noDisplay) {
            SLOGGERE("ElectronBeam","eglGetDisplay error");
            return FALSE;
        }

        AutoPtr< ArrayOf<Int32> > version = ArrayOf<Int32>::Alloc(2);
        Boolean rst;
        egl14->EglInitialize(mEglDisplay, version, 0, version, 1, &rst);
        if (!rst) {
            mEglDisplay = NULL;
            SLOGGERE("ElectronBeam","eglInitialize error")
            return FALSE;
        }
    }

    if (mEglConfig == NULL) {
        AutoPtr<ArrayOf<Int32> > eglConfigAttribList = ArrayOf<Int32>::Alloc(9);
        (*eglConfigAttribList)[0] = IEGL14::_EGL_RED_SIZE;
        (*eglConfigAttribList)[1] = 8;
        (*eglConfigAttribList)[2] = IEGL14::_EGL_GREEN_SIZE;
        (*eglConfigAttribList)[3] = 8;
        (*eglConfigAttribList)[4] = IEGL14::_EGL_BLUE_SIZE;
        (*eglConfigAttribList)[5] = 8;
        (*eglConfigAttribList)[6] = IEGL14::_EGL_ALPHA_SIZE;
        (*eglConfigAttribList)[7] = 8;
        (*eglConfigAttribList)[8] = IEGL14::_EGL_NONE;
        AutoPtr<ArrayOf<Int32> > numEglConfigs = ArrayOf<Int32>::Alloc(1);
        AutoPtr<ArrayOf<IEGLConfig*> > eglConfigs = ArrayOf<IEGLConfig*>::Alloc(1);
        Boolean rst;
        egl14->EglChooseConfig(mEglDisplay, eglConfigAttribList, 0,
                eglConfigs, 0, eglConfigs->GetLength(), numEglConfigs, 0, &rst);
        if (!rst) {
            SLOGGERE("ElectronBeam","eglChooseConfig error")
            return FALSE;
        }
        mEglConfig = (*eglConfigs)[0];
    }

    if (mEglContext == NULL) {
        AutoPtr<ArrayOf<Int32> > eglContextAttribList = ArrayOf<Int32>::Alloc(1);
        (*eglContextAttribList)[0] = IEGL14::_EGL_NONE;
        AutoPtr<IEGLContext> noCtx;
        eglHelper->GetNoContext((IEGLContext**)&noCtx);
        egl14->EglCreateContext(mEglDisplay, mEglConfig,
                noCtx, eglContextAttribList, 0, (IEGLContext**)&mEglContext);
        if (mEglContext == NULL) {
            SLOGGERE("ElectronBeam","eglCreateContext error")
            return FALSE;
        }
    }
    return TRUE;
}

Boolean ElectronBeam::CreateSurface()
{
    if (mSurfaceSession == NULL) {
        ASSERT_SUCCEEDED(CSurfaceSession::New((ISurfaceSession**)&mSurfaceSession));
    }

    AutoPtr<ISurfaceHelper> helper;
    ASSERT_SUCCEEDED(CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&helper));
    helper->OpenTransaction();
    // try {
    if (mSurface == NULL) {
        // try {
        Int32 flags;
        if (mMode == MODE_FADE) {
            flags = ISurface::FX_SURFACE_DIM | ISurface::HIDDEN;
        }
        else {
            flags = ISurface::OPAQUE | ISurface::HIDDEN;
        }
        ECode ec = CSurface::New(mSurfaceSession,
                String("ElectronBeam"), mDisplayWidth, mDisplayHeight,
                IPixelFormat::OPAQUE, flags, (ISurface**)&mSurface);
        if (ec == (ECode)E_OUT_OF_RESOURCES_EXCEPTION) {
            Slogger::E(TAG, "Unable to create surface.0x%08x", ec);
            helper->CloseTransaction();
            return FALSE;
        }
        // } catch (Surface.OutOfResourcesException ex) {
        //     Slog.e(TAG, "Unable to create surface.", ex);
        //     return false;
        // }
    }

    mSurface->SetLayerStack(mDisplayLayerStack);
    mSurface->SetSize(mDisplayWidth, mDisplayHeight);

    mSurfaceLayout = new NaturalSurfaceLayout(mDisplayManager, mSurface);
    mSurfaceLayout->OnDisplayTransaction();
    // } finally {
    helper->CloseTransaction();
    // }
    return TRUE;
}

Boolean ElectronBeam::CreateEglSurface()
{
    if (mEglSurface == NULL)
    {
        AutoPtr<ArrayOf<Int32> > eglSurfaceAttribList;
        (*eglSurfaceAttribList)[0] = IEGL14::_EGL_NONE;
        AutoPtr<IEGL14> egl14;
        CEGL14::AcquireSingleton((IEGL14**)&egl14);
        egl14->EglCreateWindowSurface(mEglDisplay, mEglConfig, mSurface,
                eglSurfaceAttribList, 0, (IEGLSurface**)&mEglSurface);
        if (mEglSurface == NULL) {
            SLOGGERE("ElectronBeam","eglCreateWindowSurface error")
            return FALSE;
        }
    }
    return TRUE;
}

void ElectronBeam::DestroyEglSurface()
{
    if (mEglSurface != NULL) {
        AutoPtr<IEGL14> egl14;
        CEGL14::AcquireSingleton((IEGL14**)&egl14);
        Boolean rst;
        egl14->EglDestroySurface(mEglDisplay, mEglSurface, &rst);
        if (rst) {
            SLOGGERE("ElectronBeam","eglDestroySurface error")
        }
        mEglSurface = NULL;
    }
}

void ElectronBeam::DestroySurface()
{
    if (mSurface != NULL) {
        mSurfaceLayout->Dispose();
        mSurfaceLayout = NULL;
        AutoPtr<ISurfaceHelper> helper;
        ASSERT_SUCCEEDED(CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&helper));
        helper->OpenTransaction();
        // try {
        mSurface->Destroy();
        // } finally {
        helper->CloseTransaction();
        // }
        mSurface = NULL;
        mSurfaceVisible = FALSE;
        mSurfaceAlpha = 0.0f;
    }
}

Boolean ElectronBeam::ShowSurface(
    /* [in] */ Float alpha)
{
    if (!mSurfaceVisible || mSurfaceAlpha != alpha) {
        AutoPtr<ISurfaceHelper> helper;
        ASSERT_SUCCEEDED(CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&helper));
        helper->OpenTransaction();
        // try {
        mSurface->SetLayer(ELECTRON_BEAM_LAYER);
        mSurface->SetAlpha(alpha);
        mSurface->Show();
        // } finally {
        helper->CloseTransaction();
        // }
        mSurfaceVisible = TRUE;
        mSurfaceAlpha = alpha;
    }
    return TRUE;
}

Boolean ElectronBeam::AttachEglContext()
{
    if (mEglSurface == NULL) {
        return FALSE;
    }
    AutoPtr<IEGL14> egl14;
    CEGL14::AcquireSingleton((IEGL14**)&egl14);
    Boolean rst;
    egl14->EglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext, &rst);
    if (!rst) {
        SLOGGERE("ElectronBeam","eglMakeCurrent error")
        return FALSE;
    }
    return TRUE;
}

void ElectronBeam::DetachEglContext()
{
    if (mEglDisplay != NULL) {
        AutoPtr<IEGL14> egl14;
        CEGL14::AcquireSingleton((IEGL14**)&egl14);
        AutoPtr<IEGL14Helper> eglHelper;
        CEGL14Helper::AcquireSingleton((IEGL14Helper**)&eglHelper);
        AutoPtr<IEGLSurface> noSurface;
        AutoPtr<IEGLContext> noContext;
        eglHelper->GetNoSurface((IEGLSurface**)&noSurface);
        eglHelper->GetNoContext((IEGLContext**)&noContext);
        Boolean rst;
        egl14->EglMakeCurrent(mEglDisplay, noSurface, noSurface, noContext, &rst);
    }
}

Float ElectronBeam::Scurve(
    /* [in] */ Float value,
    /* [in] */ Float s)
{
    // A basic sigmoid has the form y = 1.0f / FloatMap.exp(-x * s).
    // Here we take the input datum and shift it by 0.5 so that the
    // domain spans the range -0.5 .. 0.5 instead of 0 .. 1.
    Float x = value - 0.5f;

    // Next apply the sigmoid function to the scaled value
    // which produces a value in the range 0 .. 1 so we subtract
    // 0.5 to get a value in the range -0.5 .. 0.5 instead.
    Float y = Sigmoid(x, s) - 0.5f;

    // To obtain the desired boundary conditions we need to scale
    // the result so that it fills a range of -1 .. 1.
    Float v = Sigmoid(0.5f, s) - 0.5f;

    // And finally remap the value back to a range of 0 .. 1.
    return y / v * 0.5f + 0.5f;
}

Float ElectronBeam::Sigmoid(
    /* [in] */ Float x,
    /* [in] */ Float s)
{
    return 1.0f / (1.0f + (Float)Elastos::Core::Math::Exp((Double)-x * s));
}

Int32 ElectronBeam::NextPowerOfTwo(
    /* [in] */ Int32 value)
{
    return 1 << (32 - Elastos::Core::Math::NumberOfLeadingZeros(value));
}

AutoPtr<IFloatBuffer> ElectronBeam::CreateNativeFloatBuffer(
    /* [in] */ Int32 size)
{
    AutoPtr<IByteBufferHelper> helper;
    ASSERT_SUCCEEDED(CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&helper));
    AutoPtr<IByteBuffer> bb;
    ASSERT_SUCCEEDED(helper->AllocateDirect(size * 4, (IByteBuffer**)&bb));
    AutoPtr<IByteOrderHelper> orderHelper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&orderHelper));
    ByteOrder order;
    orderHelper->GetNativeOrder(&order);
    bb->SetOrder(order);
    AutoPtr<IFloatBuffer> fb;
    ASSERT_SUCCEEDED(bb->AsFloatBuffer((IFloatBuffer**)&fb));
    return fb;
}

void ElectronBeam::LogEglError(
    /* [in] */ const String& func)
{
    Int32 error;
    AutoPtr<IEGL14> egl14;
    CEGL14::AcquireSingleton((IEGL14**)&egl14);
    egl14->EglGetError(&error);
    SLOGGERE(TAG, func + " failed: error " + StringUtils::Int32ToString(error))
}

Boolean ElectronBeam::CheckGlErrors(
    /* [in] */ const String& func)
{
    return CheckGlErrors(func, TRUE);
}

Boolean ElectronBeam::CheckGlErrors(
    /* [in] */ const String& func,
    /* [in] */ Boolean log)
{
    Boolean hadError = FALSE;
    Int32 error;
    AutoPtr<IGLES10> gl10;
    CGLES10::AcquireSingleton((IGLES10**)&gl10);
    gl10->GlGetError(&error);
    while (error != IGLES10::_GL_NO_ERROR) {
        if (log) {
            SLOGGERE(TAG, func + " failed: error " + StringUtils::Int32ToString(error))
        }
        hadError = TRUE;
    }
    return hadError;
}

// void ElectronBeam::Dump(
//     /* [in] */ IPrintWriter* pw)
// {
//     pw->Println();
//     pw->Println("Electron Beam State:");
//     pw->Println("  mPrepared=" + mPrepared);
//     pw->Println("  mMode=" + mMode);
//     pw->Println("  mDisplayLayerStack=" + mDisplayLayerStack);
//     pw->Println("  mDisplayWidth=" + mDisplayWidth);
//     pw->Println("  mDisplayHeight=" + mDisplayHeight);
//     pw->Println("  mSurfaceVisible=" + mSurfaceVisible);
//     pw->Println("  mSurfaceAlpha=" + mSurfaceAlpha);
// }

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos
