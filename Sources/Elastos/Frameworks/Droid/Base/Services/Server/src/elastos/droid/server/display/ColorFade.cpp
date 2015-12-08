
#include "elastos/droid/server/display/ColorFade.h"
//#include "elastos/droid/server/LocalServices.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::Graphics::ISurfaceTexture;
using Elastos::Droid::Hardware::Display::IDisplayManagerInternal;
using Elastos::Droid::Hardware::Display::IDisplayTransactionListener;
using Elastos::Droid::Opengl::IEGL14;
using Elastos::Droid::Opengl::IEGLConfig;
using Elastos::Droid::Opengl::IEGLContext;
using Elastos::Droid::Opengl::IEGLDisplay;
using Elastos::Droid::Opengl::IEGLSurface;
using Elastos::Droid::Opengl::IGLES20;
using Elastos::Droid::Opengl::IGLES11Ext;
using Elastos::Droid::Utility::FloatMath;
using Elastos::Droid::View::IDisplayInfo;
using Elastos::Droid::View::ISurface;
using Elastos::Droid::View::ISurfaceControl;
using Elastos::Droid::View::ISurfaceSession;

using Elastos::IO::IInputStream;
using Elastos::IO::IInputStreamReader;
using Elastos::IO::IPrintWriter;
using Elastos::IO::IByteBuffer;
using Elastos::IO::IByteOrder;
using Elastos::IO::IFloatBuffer;
using Libcore::IO::IStreams;
using Elastos::Utility::Logging::Slogger;


namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

//=====================================================================
// ColorFade::NaturalSurfaceLayout
//=====================================================================

CAR_INTERFACE_IMPL(ColorFade::NaturalSurfaceLayout, Object, IDisplayTransactionListener)

ColorFade::NaturalSurfaceLayout::NaturalSurfaceLayout(
    /* [in ]*/ IDisplayManagerInternal* displayManagerInternal,
    /* [in ]*/ Int32 displayId,
    /* [in ]*/ ISurfaceControl* surfaceControl)
    : mDisplayId(0)
{
    mDisplayManagerInternal = displayManagerInternal;
    mDisplayId = displayId;
    mSurfaceControl = surfaceControl;
    mDisplayManagerInternal->RegisterDisplayTransactionListener(this);
}

ColorFade::NaturalSurfaceLayout::~NaturalSurfaceLayout()
{
    Dispose();
}

ColorFade::NaturalSurfaceLayout::Dispose()
{
    synchronized(this) {
        mSurfaceControl = NULL;
    }
    mDisplayManagerInternal->UnregisterDisplayTransactionListener(this);
}

ECode ColorFade::NaturalSurfaceLayout::OnDisplayTransaction()
{
    synchronized(this) {
        if (mSurfaceControl == NULL) {
            return;
        }

        AutoPtr<IDisplayInfo> displayInfo;
        mDisplayManagerInternal->GetDisplayInfo(mDisplayId, (IDisplayInfo**)&displayInfo);
        Int32 rotation, lw, lh;
        displayInfo->GetRotation(&rotation);
        displayInfo->GetLogicalWidth(&lw);
        displayInfo->GetLogicalHeight(&lh);
        switch (rotation) {
            case ISurface::ROTATION_0:
                mSurfaceControl->SetPosition(0, 0);
                mSurfaceControl->SetMatrix(1, 0, 0, 1);
                break;
            case ISurface::ROTATION_90:
                mSurfaceControl->SetPosition(0, lh);
                mSurfaceControl->SetMatrix(0, -1, 1, 0);
                break;
            case ISurface::ROTATION_180:
                mSurfaceControl->SetPosition(lw, lh);
                mSurfaceControl->SetMatrix(-1, 0, 0, -1);
                break;
            case ISurface::ROTATION_270:
                mSurfaceControl->SetPosition(lw, 0);
                mSurfaceControl->SetMatrix(0, 1, -1, 0);
                break;
        }
    }
    return NOERROR;
}

//=====================================================================
// ColorFade
//=====================================================================

const Int32 ColorFade::MODE_WARM_UP = 0;
const Int32 ColorFade::MODE_COOL_DOWN = 1;
const Int32 ColorFade::MODE_FADE = 2;
const String ColorFade::TAG("ColorFade");
const Boolean ColorFade::DEBUG = FALSE;
const Int32 ColorFade::COLOR_FADE_LAYER = 0x40000001;
const Int32 ColorFade::DEJANK_FRAMES = 3;

ColorFade::ColorFade(
    /* [in] */ Int32 displayId)
    : mPrepared(FALSE)
    , mMode(0)
    , mDisplayLayerStack(0)
    , mDisplayWidth(0)
    , mDisplayHeight(0)
    , mSurfaceVisible(FALSE)
    , mSurfaceAlpha(0)
    , mTexNamesGenerated(FALSE)
    , mTexCoordLoc(0)
    , mVertexLoc(0)
    , mTexUnitLoc(0)
    , mProjMatrixLoc(0)
    , mTexMatrixLoc(0)
    , mOpacityLoc(0)
    , mScaleLoc(0)
    , mGammaLoc(0)
    , mSaturationLoc(0)
    , mProgram(0)
{
    mDisplayId = displayId;
    assert(0 && "TODO");
    // mDisplayManagerInternal = LocalServices::GetService(DisplayManagerInternal.class);

    mTexNames = ArrayOf<Int32>::Alloc(1);
    mTexMatrix = ArrayOf<Float>::Alloc(16);
    mProjMatrix = ArrayOf<Float>::Alloc(16);
    mGLBuffers = ArrayOf<Int32>::Alloc(2);

    mVertexBuffer = CreateNativeFloatBuffer(8);
    mTexCoordBuffer = CreateNativeFloatBuffer(8);
}

Boolean ColorFade::Prepare(
    /* [in] */ IContext* context,
    /* [in] */ Int32 mode)
{
    if (DEBUG) {
        Slogger::D(TAG, "prepare: mode=%d", mode);
    }

    mMode = mode;

    // Get the display size and layer stack.
    // This is not expected to change while the color fade surface is showing.
    DisplayInfo displayInfo = mDisplayManagerInternal->getDisplayInfo(mDisplayId);
    mDisplayLayerStack = displayInfo.layerStack;
    mDisplayWidth = displayInfo.getNaturalWidth();
    mDisplayHeight = displayInfo.getNaturalHeight();

    // Prepare the surface for drawing.
    if (!(CreateSurface() && CreateEglContext() && CreateEglSurface() &&
          CaptureScreenshotTextureAndSetViewport())) {
        Dismiss();
        return FALSE;
    }

    // Init GL
    if (!AttachEglContext()) {
        return FALSE;
    }
    try {
        if(!InitGLShaders(context) || !InitGLBuffers() || CheckGlErrors("prepare")) {
            DetachEglContext();
            Dismiss();
            return FALSE;
        }
    } finally {
        DetachEglContext();
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

String ColorFade::ReadFile(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceId)
{
    try{
        InputStream stream = context.getResources().openRawResource(resourceId);
        return new String(Streams.readFully(new InputStreamReader(stream)));
    }
    catch (IOException e) {
        Slogger::E(TAG, "Unrecognized shader " + Integer.toString(resourceId));
        throw new RuntimeException(e);
    }
}

Int32 ColorFade::LoadShader(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceId,
    /* [in] */ Int32 type)
{
    String source = ReadFile(context, resourceId);

    Int32 shader = GLES20.glCreateShader(type);

    GLES20.glShaderSource(shader, source);
    GLES20.glCompileShader(shader);

    Int32[] compiled = new Int32[1];
    GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
    if (compiled[0] == 0) {
        Slogger::E(TAG, "Could not compile shader " + shader + ", " + type + ":");
        Slogger::E(TAG, GLES20.glGetShaderSource(shader));
        Slogger::E(TAG, GLES20.glGetShaderInfoLog(shader));
        GLES20.glDeleteShader(shader);
        shader = 0;
    }

    return shader;
}

Boolean ColorFade::InitGLShaders(
    /* [in] */ IContext* context)
{
    Int32 vshader = loadShader(context, com.android.internal.R.raw.color_fade_vert,
            GLES20.GL_VERTEX_SHADER);
    Int32 fshader = loadShader(context, com.android.internal.R.raw.color_fade_frag,
            GLES20.GL_FRAGMENT_SHADER);
    GLES20.glReleaseShaderCompiler();
    if (vshader == 0 || fshader == 0) return FALSE;

    mProgram = GLES20.glCreateProgram();

    GLES20.glAttachShader(mProgram, vshader);
    GLES20.glAttachShader(mProgram, fshader);
    GLES20.glDeleteShader(vshader);
    GLES20.glDeleteShader(fshader);

    GLES20.glLinkProgram(mProgram);

    mVertexLoc = GLES20.glGetAttribLocation(mProgram, "position");
    mTexCoordLoc = GLES20.glGetAttribLocation(mProgram, "uv");

    mProjMatrixLoc = GLES20.glGetUniformLocation(mProgram, "proj_matrix");
    mTexMatrixLoc = GLES20.glGetUniformLocation(mProgram, "tex_matrix");

    mOpacityLoc = GLES20.glGetUniformLocation(mProgram, "opacity");
    mGammaLoc = GLES20.glGetUniformLocation(mProgram, "gamma");
    mSaturationLoc = GLES20.glGetUniformLocation(mProgram, "saturation");
    mScaleLoc = GLES20.glGetUniformLocation(mProgram, "scale");
    mTexUnitLoc = GLES20.glGetUniformLocation(mProgram, "texUnit");

    GLES20.glUseProgram(mProgram);
    GLES20.glUniform1i(mTexUnitLoc, 0);
    GLES20.glUseProgram(0);

    return TRUE;
}

void ColorFade::DestroyGLShaders()
{
    GLES20.glDeleteProgram(mProgram);
    CheckGlErrors("glDeleteProgram");
}

Boolean ColorFade::InitGLBuffers()
{
    //Fill vertices
    SetQuad(mVertexBuffer, 0, 0, mDisplayWidth, mDisplayHeight);

    // Setup GL Textures
    GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, mTexNames[0]);
    GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MAG_FILTER,
            GLES20.GL_NEAREST);
    GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MIN_FILTER,
            GLES20.GL_NEAREST);
    GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_S,
            GLES20.GL_CLAMP_TO_EDGE);
    GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_T,
            GLES20.GL_CLAMP_TO_EDGE);
    GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, 0);

    // Setup GL Buffers
    GLES20.glGenBuffers(2, mGLBuffers, 0);

    // fill vertex buffer
    GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, mGLBuffers[0]);
    GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER, mVertexBuffer.capacity() * 4,
                        mVertexBuffer, GLES20.GL_STATIC_DRAW);

    // fill tex buffer
    GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, mGLBuffers[1]);
    GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER, mTexCoordBuffer.capacity() * 4,
                        mTexCoordBuffer, GLES20.GL_STATIC_DRAW);

    GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);

    return TRUE;
}

void ColorFade::DestroyGLBuffers()
{
    GLES20.glDeleteBuffers(2, mGLBuffers, 0);
    CheckGlErrors("glDeleteBuffers");
}

void ColorFade::SetQuad(
    /* [in] */ IFloatBuffer* vtx,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float w,
    /* [in] */ Float h)
{
    if (DEBUG) {
        Slogger::D(TAG, "SetQuad: x=" + x + ", y=" + y + ", w=" + w + ", h=" + h);
    }
    vtx.put(0, x);
    vtx.put(1, y);
    vtx.put(2, x);
    vtx.put(3, y + h);
    vtx.put(4, x + w);
    vtx.put(5, y + h);
    vtx.put(6, x + w);
    vtx.put(7, y);
}

void ColorFade::Dismiss()
{
    if (DEBUG) {
        Slogger::D(TAG, "dismiss");
    }

    if (mPrepared) {
        AttachEglContext();
        try {
            DestroyScreenshotTexture();
            DestroyGLShaders();
            destroyGLBuffers();
            DestroyEglSurface();
        } finally {
            DetachEglContext();
        }
        DestroySurface();
        GLES20.glFlush();
        mPrepared = FALSE;
    }
}

Boolean ColorFade::Draw(
    /* [in] */ Float level)
{
    if (DEBUG) {
        Slogger::D(TAG, "drawFrame: level=" + level);
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
    try {
        // Clear frame to solid black.
        GLES20.glClearColor(0f, 0f, 0f, 1f);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);

        // Draw the frame.
        Float one_minus_level = 1 - level;
        Float cos = FloatMath.cos((Float)Math.PI * one_minus_level);
        Float sign = cos < 0 ? -1 : 1;
        Float opacity = -FloatMath.pow(one_minus_level, 2) + 1;
        Float saturation = FloatMath.pow(level, 4);
        Float scale = (-FloatMath.pow(one_minus_level, 2) + 1) * 0.1f + 0.9f;
        Float gamma = (0.5f * sign * FloatMath.pow(cos, 2) + 0.5f) * 0.9f + 0.1f;
        DrawFaded(opacity, 1.f / gamma, saturation, scale);
        if (CheckGlErrors("drawFrame")) {
            return FALSE;
        }

        EGL14.eglSwapBuffers(mEglDisplay, mEglSurface);
    } finally {
        DetachEglContext();
    }
    return ShowSurface(1.0f);
}

void ColorFade::DrawFaded(
    /* [in] */ Float opacity,
    /* [in] */ Float gamma,
    /* [in] */ Float saturation,
    /* [in] */ Float scale)
{
    if (DEBUG) {
        Slogger::D(TAG, "DrawFaded: opacity=" + opacity + ", gamma=" + gamma +
                    ", saturation=" + saturation + ", scale=" + scale);
    }
    // Use shaders
    GLES20.glUseProgram(mProgram);

    // Set Uniforms
    GLES20.glUniformMatrix4fv(mProjMatrixLoc, 1, FALSE, mProjMatrix, 0);
    GLES20.glUniformMatrix4fv(mTexMatrixLoc, 1, FALSE, mTexMatrix, 0);
    GLES20.glUniform1f(mOpacityLoc, opacity);
    GLES20.glUniform1f(mGammaLoc, gamma);
    GLES20.glUniform1f(mSaturationLoc, saturation);
    GLES20.glUniform1f(mScaleLoc, scale);

    // Use textures
    GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
    GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, mTexNames[0]);

    // draw the plane
    GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, mGLBuffers[0]);
    GLES20.glEnableVertexAttribArray(mVertexLoc);
    GLES20.glVertexAttribPointer(mVertexLoc, 2, GLES20.GL_FLOAT, FALSE, 0, 0);

    GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, mGLBuffers[1]);
    GLES20.glEnableVertexAttribArray(mTexCoordLoc);
    GLES20.glVertexAttribPointer(mTexCoordLoc, 2, GLES20.GL_FLOAT, FALSE, 0, 0);

    GLES20.glDrawArrays(GLES20.GL_TRIANGLE_FAN, 0, 4);

    // clean up
    GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, 0);
    GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
}

void ColorFade::Ortho(
    /* [in] */ Float left,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float top,
    /* [in] */ Float znear,
    /* [in] */ Float zfar)
{
    mProjMatrix[0] = 2f / (right - left);
    mProjMatrix[1] = 0;
    mProjMatrix[2] = 0;
    mProjMatrix[3] = 0;
    mProjMatrix[4] = 0;
    mProjMatrix[5] = 2f / (top - bottom);
    mProjMatrix[6] = 0;
    mProjMatrix[7] = 0;
    mProjMatrix[8] = 0;
    mProjMatrix[9] = 0;
    mProjMatrix[10] = -2f / (zfar - znear);
    mProjMatrix[11] = 0;
    mProjMatrix[12] = -(right + left) / (right - left);
    mProjMatrix[13] = -(top + bottom) / (top - bottom);
    mProjMatrix[14] = -(zfar + znear) / (zfar - znear);
    mProjMatrix[15] = 1f;
}

Boolean ColorFade::CaptureScreenshotTextureAndSetViewport()
{
    if (!AttachEglContext()) {
        return FALSE;
    }
    try {
        if (!mTexNamesGenerated) {
            GLES20.glGenTextures(1, mTexNames, 0);
            if (CheckGlErrors("glGenTextures")) {
                return FALSE;
            }
            mTexNamesGenerated = TRUE;
        }

        final SurfaceTexture st = new SurfaceTexture(mTexNames[0]);
        final Surface s = new Surface(st);
        try {
            SurfaceControl->screenshot(SurfaceControl->getBuiltInDisplay(
                    SurfaceControl->BUILT_IN_DISPLAY_ID_MAIN), s);
        } finally {
            s.release();
        }

        st.updateTexImage();
        st.getTransformMatrix(mTexMatrix);

        // Set up texture coordinates for a quad.
        // We might need to change this if the texture ends up being
        // a different size from the display for some reason.
        mTexCoordBuffer.put(0, 0f); mTexCoordBuffer.put(1, 0f);
        mTexCoordBuffer.put(2, 0f); mTexCoordBuffer.put(3, 1f);
        mTexCoordBuffer.put(4, 1f); mTexCoordBuffer.put(5, 1f);
        mTexCoordBuffer.put(6, 1f); mTexCoordBuffer.put(7, 0f);

        // Set up our viewport.
        GLES20.glViewport(0, 0, mDisplayWidth, mDisplayHeight);
        Ortho(0, mDisplayWidth, 0, mDisplayHeight, -1, 1);
    } finally {
        DetachEglContext();
    }
    return TRUE;
}

void ColorFade::DestroyScreenshotTexture()
{
    if (mTexNamesGenerated) {
        mTexNamesGenerated = FALSE;
        GLES20.glDeleteTextures(1, mTexNames, 0);
        CheckGlErrors("glDeleteTextures");
    }
}

Boolean ColorFade::CreateEglContext()
{
    if (mEglDisplay == NULL) {
        mEglDisplay = EGL14.eglGetDisplay(EGL14.EGL_DEFAULT_DISPLAY);
        if (mEglDisplay == EGL14.EGL_NO_DISPLAY) {
            LogEglError("eglGetDisplay");
            return FALSE;
        }

        Int32[] version = new Int32[2];
        if (!EGL14.eglInitialize(mEglDisplay, version, 0, version, 1)) {
            mEglDisplay = NULL;
            LogEglError("eglInitialize");
            return FALSE;
        }
    }

    if (mEglConfig == NULL) {
        Int32[] eglConfigAttribList = new Int32[] {
                EGL14.EGL_RENDERABLE_TYPE,
                EGL14.EGL_OPENGL_ES2_BIT,
                EGL14.EGL_RED_SIZE, 8,
                EGL14.EGL_GREEN_SIZE, 8,
                EGL14.EGL_BLUE_SIZE, 8,
                EGL14.EGL_ALPHA_SIZE, 8,
                EGL14.EGL_NONE
        };
        Int32[] numEglConfigs = new Int32[1];
        EGLConfig[] eglConfigs = new EGLConfig[1];
        if (!EGL14.eglChooseConfig(mEglDisplay, eglConfigAttribList, 0,
                eglConfigs, 0, eglConfigs.length, numEglConfigs, 0)) {
            LogEglError("eglChooseConfig");
            return FALSE;
        }
        mEglConfig = eglConfigs[0];
    }

    if (mEglContext == NULL) {
        Int32[] eglContextAttribList = new Int32[] {
                EGL14.EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL14.EGL_NONE
        };
        mEglContext = EGL14.eglCreateContext(mEglDisplay, mEglConfig,
                EGL14.EGL_NO_CONTEXT, eglContextAttribList, 0);
        if (mEglContext == NULL) {
            LogEglError("eglCreateContext");
            return FALSE;
        }
    }
    return TRUE;
}

Boolean ColorFade::CreateSurface()
{
    if (mSurfaceSession == NULL) {
        mSurfaceSession = new SurfaceSession();
    }

    SurfaceControl->OpenTransaction();
    try {
        if (mSurfaceControl == NULL) {
            try {
                Int32 flags;
                if (mMode == MODE_FADE) {
                    flags = SurfaceControl->FX_SURFACE_DIM | SurfaceControl->HIDDEN;
                } else {
                    flags = SurfaceControl->OPAQUE | SurfaceControl->HIDDEN;
                }
                mSurfaceControl = new SurfaceControl(mSurfaceSession,
                        "ColorFade", mDisplayWidth, mDisplayHeight,
                        PixelFormat.OPAQUE, flags);
            } catch (OutOfResourcesException ex) {
                Slogger::E(TAG, "Unable to create surface.", ex);
                return FALSE;
            }
        }

        mSurfaceControl->SetLayerStack(mDisplayLayerStack);
        mSurfaceControl->SetSize(mDisplayWidth, mDisplayHeight);
        mSurface = new Surface();
        mSurface.copyFrom(mSurfaceControl);

        mSurfaceLayout = new NaturalSurfaceLayout(mDisplayManagerInternal,
                mDisplayId, mSurfaceControl);
        mSurfaceLayout.onDisplayTransaction();
    } finally {
        SurfaceControl->CloseTransaction();
    }
    return TRUE;
}

Boolean ColorFade::CreateEglSurface()
{
    if (mEglSurface == NULL) {
        Int32[] eglSurfaceAttribList = new Int32[] {
                EGL14.EGL_NONE
        };
        // turn our SurfaceControl into a Surface
        mEglSurface = EGL14.eglCreateWindowSurface(mEglDisplay, mEglConfig, mSurface,
                eglSurfaceAttribList, 0);
        if (mEglSurface == NULL) {
            LogEglError("eglCreateWindowSurface");
            return FALSE;
        }
    }
    return TRUE;
}

void ColorFade::DestroyEglSurface()
{
    if (mEglSurface != NULL) {
        if (!EGL14.eglDestroySurface(mEglDisplay, mEglSurface)) {
            LogEglError("eglDestroySurface");
        }
        mEglSurface = NULL;
    }
}

void ColorFade::DestroySurface()
{
    if (mSurfaceControl != NULL) {
        mSurfaceLayout.Dispose();
        mSurfaceLayout = NULL;
        SurfaceControl->OpenTransaction();
        try {
            mSurfaceControl->destroy();
            mSurface.release();
        } finally {
            SurfaceControl->CloseTransaction();
        }
        mSurfaceControl = NULL;
        mSurfaceVisible = FALSE;
        mSurfaceAlpha = 0f;
    }
}

Boolean ColorFade::ShowSurface(
    /* [in] */ Float alpha)
{
    if (!mSurfaceVisible || mSurfaceAlpha != alpha) {
        SurfaceControl->OpenTransaction();
        try {
            mSurfaceControl->SetLayer(COLOR_FADE_LAYER);
            mSurfaceControl->SetAlpha(alpha);
            mSurfaceControl->Show();
        } finally {
            SurfaceControl->CloseTransaction();
        }
        mSurfaceVisible = TRUE;
        mSurfaceAlpha = alpha;
    }
    return TRUE;
}

Boolean ColorFade::AttachEglContext()
{
    if (mEglSurface == NULL) {
        return FALSE;
    }
    if (!EGL14.eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext)) {
        LogEglError("eglMakeCurrent");
        return FALSE;
    }
    return TRUE;
}

void ColorFade::DetachEglContext()
{
    if (mEglDisplay != NULL) {
        EGL14.eglMakeCurrent(mEglDisplay,
                EGL14.EGL_NO_SURFACE, EGL14.EGL_NO_SURFACE, EGL14.EGL_NO_CONTEXT);
    }
}

AutoPtr<IFloatBuffer> ColorFade::CreateNativeFloatBuffer(
    /* [in] */ Int32 size)
{
    ByteBuffer bb = ByteBuffer.allocateDirect(size * 4);
    bb.order(ByteOrder.nativeOrder());
    return bb.asFloatBuffer();
}

void ColorFade::LogEglError(
    /* [in] */ const String& func)
{
    AutoPtr<IEGL14> egl14;
    CEGL14::AcquireSingelton((IEGL14**)&egl14);
    Int32 error;
    egl14->EglGetError(&error);
    Slogger::E(TAG, "%s failed: error %d", func.string(), error);
}

Boolean ColorFade::CheckGlErrors(
    /* [in] */ const String& func)
{
    return CheckGlErrors(func, TRUE);
}

Boolean ColorFade::CheckGlErrors(
    /* [in] */ const String& func,
    /* [in] */ Boolean log)
{
    Boolean hadError = FALSE;
    AutoPtr<IGLES20> gles20;
    CGLES20::AcquireSingelton((IGLES20**)&gles20);

    Int32 error;
    gles20->glGetError(&error);
    while (error != IGLES20::GL_NO_ERROR) {
        if (log) {
            Slogger::E(TAG, "%s failed: error %d", func.string(), error);
        }
        hadError = TRUE;
        gles20->glGetError(&error);
    }
    return hadError;
}

void ColorFade::Dump(
    /* [in] */ IPrintWriter* pw)
{
    // pw.println();
    // pw.println("Color Fade State:");
    // pw.println("  mPrepared=" + mPrepared);
    // pw.println("  mMode=" + mMode);
    // pw.println("  mDisplayLayerStack=" + mDisplayLayerStack);
    // pw.println("  mDisplayWidth=" + mDisplayWidth);
    // pw.println("  mDisplayHeight=" + mDisplayHeight);
    // pw.println("  mSurfaceVisible=" + mSurfaceVisible);
    // pw.println("  mSurfaceAlpha=" + mSurfaceAlpha);
}


} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos

