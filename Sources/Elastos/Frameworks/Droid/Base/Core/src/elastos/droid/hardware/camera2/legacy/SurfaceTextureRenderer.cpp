
#include "elastos/droid/hardware/camera2/legacy/SurfaceTextureRenderer.h"
#include "elastos/droid/hardware/camera2/legacy/LegacyCameraDevice.h"
#include "elastos/droid/hardware/camera2/legacy/PerfMeasurement.h"
#include "elastos/droid/hardware/camera2/legacy/CPerfMeasurement.h"
#include "elastos/droid/graphics/CRectF.h"
#include "elastos/droid/graphics/CMatrix.h"
#include "elastos/droid/graphics/CSurfaceTexture.h"
#include "elastos/droid/os/Environment.h"
#include "elastos/droid/os/SystemProperties.h"
 #include "elastos/droid/text/format/CTime.h"
//#include "elastos/droid/opengl/CEGL14Helper.h"
//#include "elastos/droid/opengl/Matrix.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Hardware::Camera2::Legacy::CPerfMeasurement;
using Elastos::Droid::Hardware::Camera2::ICameraCharacteristics;
using Elastos::Droid::Graphics::IMatrix;
using Elastos::Droid::Graphics::CMatrix;
using Elastos::Droid::Graphics::MatrixScaleToFit_CENTER;;
using Elastos::Droid::Graphics::IRectF;
using Elastos::Droid::Graphics::CRectF;
using Elastos::Droid::Graphics::IImageFormat;
using Elastos::Droid::Graphics::ISurfaceTexture;
using Elastos::Droid::Graphics::CSurfaceTexture;
using Elastos::Droid::Os::Environment;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Droid::Opengl::IGLES20;
using Elastos::Droid::Opengl::IEGL14;
using Elastos::Droid::Opengl::IEGL14Helper;
// using Elastos::Droid::Opengl::IEGLConfig;
// using Elastos::Droid::Opengl::IEGLDisplay;
// using Elastos::Droid::Opengl::Matrix;
// using Elastos::Droid::Opengl::CEGL14Helper;
using Elastos::Droid::Text::Format::CTime;
using Elastos::Droid::Text::Format::ITime;
using Elastos::Droid::Utility::ISize;
using Elastos::Droid::Utility::IPair;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Core::IInteger64;
using Elastos::IO::CFile;
using Elastos::IO::IFile;
using Elastos::IO::IFileHelper;
using Elastos::IO::CFileHelper;
using Elastos::IO::ByteOrder;
using Elastos::IO::IByteOrderHelper;
using Elastos::IO::CByteOrderHelper;
using Elastos::IO::IBuffer;
using Elastos::IO::IByteBuffer;
using Elastos::IO::IByteBufferHelper;
using Elastos::IO::CByteBufferHelper;
using Elastos::Utility::CArrayList;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Camera2 {
namespace Legacy {

CAR_INTERFACE_IMPL(SurfaceTextureRenderer, Object, ISurfaceTextureRenderer)

const String SurfaceTextureRenderer::TAG("SurfaceTextureRenderer");// = SurfaceTextureRenderer.class.getSimpleName();
const Boolean SurfaceTextureRenderer::DEBUG = FALSE;//Log.isLoggable(LegacyCameraDevice.DEBUG_PROP, Log.DEBUG);
const Int32 SurfaceTextureRenderer::EGL_RECORDABLE_ANDROID = 0x3142; // from EGL/eglext.h
const Int32 SurfaceTextureRenderer::GL_MATRIX_SIZE = 16;
const Int32 SurfaceTextureRenderer::VERTEX_POS_SIZE = 3;
const Int32 SurfaceTextureRenderer::VERTEX_UV_SIZE = 2;
const Int32 SurfaceTextureRenderer::EGL_COLOR_BITLENGTH = 8;
const Int32 SurfaceTextureRenderer::GLES_VERSION = 2;
const Int32 SurfaceTextureRenderer::PBUFFER_PIXEL_BYTES = 4;

const Int32 SurfaceTextureRenderer::FLOAT_SIZE_BYTES = 4;
const Int32 SurfaceTextureRenderer::TRIANGLE_VERTICES_DATA_STRIDE_BYTES = 5 * FLOAT_SIZE_BYTES;
const Int32 SurfaceTextureRenderer::TRIANGLE_VERTICES_DATA_POS_OFFSET = 0;
const Int32 SurfaceTextureRenderer::TRIANGLE_VERTICES_DATA_UV_OFFSET = 3;

static AutoPtr<ArrayOf<Float> > initFrontCameraTriangleVertices()
{
    AutoPtr<ArrayOf<Float> > array = ArrayOf<Float>::Alloc(20);

    // X, Y, Z, U, V
    (*array)[0] = -1.0f;
    (*array)[1] = -1.0f;
    (*array)[2] = 0;
    (*array)[3] = 1.f;
    (*array)[4] = 0.f;
    (*array)[5] = 1.0f;
    (*array)[6] = -1.0f;
    (*array)[7] = 0;
    (*array)[8] = 0.f;
    (*array)[9] = 0.f;
    (*array)[10] = -1.0f;
    (*array)[11] = 1.0f;
    (*array)[12] = 0;
    (*array)[13] = 1.f;
    (*array)[14] = 1.f;
    (*array)[15] = 1.0f;
    (*array)[16] = 1.0f;
    (*array)[17] = 0;
    (*array)[18] = 0.f;
    (*array)[19] = 1.f;

    return array;
}

const AutoPtr<ArrayOf<Float> > SurfaceTextureRenderer::sFrontCameraTriangleVertices =
        initFrontCameraTriangleVertices();

static AutoPtr<ArrayOf<Float> > initBackCameraTriangleVertices()
{
    AutoPtr<ArrayOf<Float> > array = ArrayOf<Float>::Alloc(20);

    // X, Y, Z, U, V
    (*array)[0] = -1.0f;
    (*array)[1] = -1.0f;
    (*array)[2] = 0;
    (*array)[3] = 0.f;
    (*array)[4] = 0.f;
    (*array)[5] = 1.0f;
    (*array)[6] = -1.0f;
    (*array)[7] = 0;
    (*array)[8] = 1.f;
    (*array)[9] = 0.f;
    (*array)[10] = -1.0f;
    (*array)[11] = 1.0f;
    (*array)[12] = 0;
    (*array)[13] = 0.f;
    (*array)[14] = 1.f;
    (*array)[15] = 1.0f;
    (*array)[16] = 1.0f;
    (*array)[17] = 0;
    (*array)[18] = 1.f;
    (*array)[19] = 1.f;

    return array;
}

const AutoPtr<ArrayOf<Float> > SurfaceTextureRenderer::sBackCameraTriangleVertices =
        initBackCameraTriangleVertices();

const String SurfaceTextureRenderer::VERTEX_SHADER(
        "uniform mat4 uMVPMatrix;\n"
        "uniform mat4 uSTMatrix;\n"
        "attribute vec4 aPosition;\n"
        "attribute vec4 aTextureCoord;\n"
        "varying vec2 vTextureCoord;\n"
        "void main() {\n"
        "    gl_Position = uMVPMatrix * aPosition;\n"
        "    vTextureCoord = (uSTMatrix * aTextureCoord).xy;\n"
        "}\n");

const String SurfaceTextureRenderer::FRAGMENT_SHADER(
        "#extension GL_OES_EGL_image_external : require\n"
        "precision mediump float;\n"
        "varying vec2 vTextureCoord;\n"
        "uniform samplerExternalOES sTexture;\n"
        "void main() {\n"
        "    gl_FragColor = texture2D(sTexture, vTextureCoord);\n"
        "}\n");

const String SurfaceTextureRenderer::LEGACY_PERF_PROPERTY("persist.camera.legacy_perf");

SurfaceTextureRenderer::SurfaceTextureRenderer()
    : mProgram(0)
    , mTextureID(0)
    , muMVPMatrixHandle(0)
    , muSTMatrixHandle(0)
    , maPositionHandle(0)
    , maTextureHandle(0)
{
    assert(0);
    // AutoPtr<IEGL14Helper> helper;
    // CEGL14Helper::AcquireSingleton((IEGL14Helper**)&helper);
    // helper->GetNoDisplay((IEGLDisplay**)&mEGLDisplay);
    // helper->GetNoContext((IEGLContext**)&mEGLContext);

    CArrayList::New((IList**)&mSurfaces);
    CArrayList::New((IList**)&mConversionSurfaces);

    mMVPMatrix = ArrayOf<Float>::Alloc(GL_MATRIX_SIZE);
    mSTMatrix = ArrayOf<Float>::Alloc(GL_MATRIX_SIZE);
}


ECode SurfaceTextureRenderer::constructor()
{
    return NOERROR;
}

ECode SurfaceTextureRenderer::constructor(
    /* [in] */Int32 facing)
{
    if (facing == ICameraMetadata::LENS_FACING_BACK) {
        AutoPtr<IByteBufferHelper> helper;
        CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&helper);
        AutoPtr<IByteBuffer> bBuffer;
        helper->AllocateDirect(sBackCameraTriangleVertices->GetLength() *
                FLOAT_SIZE_BYTES, (IByteBuffer**)&bBuffer);

        AutoPtr<IByteOrderHelper> helper2;
        CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper2);
        ByteOrder bOrder;
        helper2->GetNativeOrder(&bOrder);

        bBuffer->SetOrder(bOrder);
        bBuffer->AsFloatBuffer((IFloatBuffer**)&mTriangleVertices);

        mTriangleVertices->Put(*(sBackCameraTriangleVertices.Get()));
        IBuffer::Probe(mTriangleVertices)->SetPosition(0);
    }
    else {
        AutoPtr<IByteBufferHelper> helper;
        CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&helper);

        AutoPtr<IByteBuffer> bBuffer;
        helper->AllocateDirect(sFrontCameraTriangleVertices->GetLength() *
                FLOAT_SIZE_BYTES, (IByteBuffer**)&bBuffer);

        AutoPtr<IByteOrderHelper> helper2;
        CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper2);
        ByteOrder bOrder;
        helper2->GetNativeOrder(&bOrder);

        bBuffer->SetOrder(bOrder);
        bBuffer->AsFloatBuffer((IFloatBuffer**)&mTriangleVertices);

        mTriangleVertices->Put(*(sFrontCameraTriangleVertices.Get()));
        IBuffer::Probe(mTriangleVertices)->SetPosition(0);
    }

    assert(0 && "need opengl::Matrix");
    //Matrix::SetIdentityM(mSTMatrix, 0);
    return NOERROR;
}

ECode SurfaceTextureRenderer::LoadShader(
    /* [in] */ Int32 shaderType,
    /* [in] */ const String& source,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    assert(0 && "need opengl::GLES20");
    // Int32 shader;
    // GLES20::GlCreateShader(shaderType, &shader);
    // StringBuilder sb;
    // sb += "glCreateShader type=";
    // sb += shaderType;
    // FAIL_RETURN(CheckGlError(sb.ToString()))
    // GLES20::GlShaderSource(shader, source);
    // GLES20::GlCompileShader(shader);
    // AutoPtr<ArrayOf<Int32> > compiled = ArrayOf<Int32>::Alloc(1);
    // GLES20::GlGetShaderiv(shader, IGLES20::GL_COMPILE_STATUS, compiled, 0);
    // if ((*compiled)[0] == 0) {
    //     Slogger::E(TAG, "Could not compile shader %d:", shaderType);
    //     String str;
    //     GLES20::GlGetShaderInfoLog(shader, &str);
    //     Slogger::E(TAG, " %s", str.string());
    //     GLES20::GlDeleteShader(shader);
    //     // TODO: handle this more gracefully
    //     //throw new IllegalStateException("Could not compile shader " + shaderType);
    //     Slogger::E(TAG, "Could not compile shader %d", shaderType);
    //     return E_ILLEGAL_STATE_EXCEPTION;;
    // }
    // *result = shader;
    return NOERROR;
}

ECode SurfaceTextureRenderer::CreateProgram(
    /* [in] */ const String& vertexSource,
    /* [in] */ const String& fragmentSource,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    Int32 vertexShader;
    FAIL_RETURN(LoadShader(IGLES20::_GL_VERTEX_SHADER, vertexSource, &vertexShader))
    if (vertexShader == 0) {
        *result = 0;
        return NOERROR;
    }
    Int32 pixelShader;
    FAIL_RETURN(LoadShader(IGLES20::_GL_FRAGMENT_SHADER, fragmentSource, &pixelShader))
    if (pixelShader == 0) {
        *result = 0;
        return NOERROR;
    }

    assert(0 && "need opengl::GLES20");
    Int32 program;
    // GLES20::GlCreateProgram(&program);
    // FAIL_RETURN(CheckGlError("glCreateProgram"))
    // if (program == 0) {
    //     Slogger::E(TAG, "Could not create program");
    // }
    // GLES20::GlAttachShader(program, vertexShader);
    // FAIL_RETURN(CheckGlError("glAttachShader"))
    // GLES20::GlAttachShader(program, pixelShader);
    // FAIL_RETURN(CheckGlError("glAttachShader"))
    // GLES20::GlLinkProgram(program);
    // int[] linkStatus = new int[1];
    // AutoPtr<ArrayOf<Int32> > linkStatus = ArrayOf<Int32>::Alloc(1);
    // GLES20::GlGetProgramiv(program, IGLES20::GL_LINK_STATUS, linkStatus, 0);
    // if ((*linkStatus)[0] != IGLES20::GL_TRUE) {
    //     Slogger::E(TAG, "Could not link program: ");
    //     String str;
    //     GLES20::GlGetProgramInfoLog(program, &str);
    //     Slogger::E(TAG, str);
    //     GLES20::GlDeleteProgram(program);
    //     // TODO: handle this more gracefully
    //     //throw new IllegalStateException("Could not link program");
    //     Slogger::E(TAG, "Could not link program");
    //     return E_ILLEGAL_STATE_EXCEPTION;;
    // }
    *result = program;
    return NOERROR;
}

ECode SurfaceTextureRenderer::DrawFrame(
    /* [in] */ ISurfaceTexture* st,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    FAIL_RETURN(CheckGlError(String("onDrawFrame start")))
    st->GetTransformMatrix(mSTMatrix);

    assert(0 && "need opengl::Matrix");
    //Matrix::SetIdentityM(mMVPMatrix, /*smOffset*/0);

    // Find intermediate buffer dimensions
    AutoPtr<ISize> dimens;
    //try {
    ECode ec = LegacyCameraDevice::GetTextureSize(st, (ISize**)&dimens);
    //} catch (LegacyExceptionUtils.BufferQueueAbandonedException e) {
    if (FAILED(ec)) {
        // Should never hit this.
        //throw new IllegalStateException("Surface abandoned, skipping drawFrame...", e);
        Slogger::E(TAG, "Surface abandoned, skipping drawFrame... %d", ec);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    //}
    Int32 texWidth;
    dimens->GetWidth(&texWidth);
    Int32 texHeight;
    dimens->GetHeight(&texHeight);

    if (texWidth <= 0 || texHeight <= 0) {
        //throw new IllegalStateException("Illegal intermediate texture with dimension of 0");
        Slogger::E(TAG, "Illegal intermediate texture with dimension of 0");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // Letterbox or pillerbox output dimensions into intermediate dimensions.
    AutoPtr<IRectF> intermediate;
    CRectF::New(/*left*/0, /*top*/0, /*right*/texWidth, /*bottom*/texHeight, (IRectF**)&intermediate);
    AutoPtr<IRectF> output;
    CRectF::New(/*left*/0, /*top*/0, /*right*/width, /*bottom*/height, (IRectF**)&output);
    AutoPtr<IMatrix> boxingXform;
    CMatrix::New((IMatrix**)&boxingXform);
    Boolean result;
    boxingXform->SetRectToRect(output, intermediate, MatrixScaleToFit_CENTER, &result);
    boxingXform->MapRect(output, &result);

    // Find scaling factor from pillerboxed/letterboxed output dimensions to intermediate
    // buffer dimensions.
    Float width2;
    intermediate->GetWidth(&width2);
    Float width3;
    output->GetWidth(&width3);
    Float height2;
    intermediate->GetHeight(&height2);
    Float height3;
    output->GetHeight(&height3);

    Float scaleX = width2 / width3;
    Float scaleY = height2 / height3;

    // Scale opposite dimension in clip coordinates so output is letterboxed/pillerboxed into
    // the intermediate dimensions (rather than vice-versa).
    assert(0 && "need opengl::Matrix && GLES20");
    // Matrix::ScaleM(mMVPMatrix, /*offset*/0, /*x*/scaleY, /*y*/scaleX, /*z*/1);

    // if (DEBUG) {
    //     Slogger::D(TAG, "Scaling factors (S_x = %f,S_y = %f) used for %fx%f surface,"
    //             "intermediate buffer size is %fx%f", scaleX, scaleY, width,
    //             height, texWidth, texHeight);
    // }

    // // Set viewport to be output buffer dimensions
    // GLES20::GlViewport(0, 0, width, height);

    // if (DEBUG) {
    //     GLES20::GlClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    //     GLES20::GlClear(IGLES20::GL_DEPTH_BUFFER_BIT | IGLES20::GL_COLOR_BUFFER_BIT);
    // }

    // GLES20::GlUseProgram(mProgram);
    // FAIL_RETURN(CheckGlError("glUseProgram"))

    // GLES20::GlActiveTexture(IGLES20::GL_TEXTURE0);
    // GLES20::GlBindTexture(IGLES11Ext::GL_TEXTURE_EXTERNAL_OES, mTextureID);

    // mTriangleVertices->Position(TRIANGLE_VERTICES_DATA_POS_OFFSET);
    // GLES20::GlVertexAttribPointer(maPositionHandle, VERTEX_POS_SIZE, IGLES20::GL_FLOAT,
    //         /*normalized*/ FALSE, TRIANGLE_VERTICES_DATA_STRIDE_BYTES, mTriangleVertices);
    // FAIL_RETURN(CheckGlError("glVertexAttribPointer maPosition"))
    // GLES20::GlEnableVertexAttribArray(maPositionHandle);
    // FAIL_RETURN(CheckGlError("glEnableVertexAttribArray maPositionHandle"))

    // mTriangleVertices->Position(TRIANGLE_VERTICES_DATA_UV_OFFSET);
    // GLES20::GlVertexAttribPointer(maTextureHandle, VERTEX_UV_SIZE, IGLES20::GL_FLOAT,
    //         /*normalized*/ FALSE, TRIANGLE_VERTICES_DATA_STRIDE_BYTES, mTriangleVertices);
    // FAIL_RETURN(CheckGlError("glVertexAttribPointer maTextureHandle"))
    // GLES20::GlEnableVertexAttribArray(maTextureHandle))
    // FAIL_RETURN(CheckGlError("glEnableVertexAttribArray maTextureHandle"))

    // GLES20::GlUniformMatrix4fv(muMVPMatrixHandle, /*count*/ 1, /*transpose*/ false, mMVPMatrix,
    //         /*offset*/ 0);
    // GLES20::GlUniformMatrix4fv(muSTMatrixHandle, /*count*/ 1, /*transpose*/ false, mSTMatrix,
    //         /*offset*/ 0);

    // GLES20::GlDrawArrays(GLES20.GL_TRIANGLE_STRIP, /*offset*/ 0, /*count*/ 4);
    return CheckGlError(String("glDrawArrays"));
}

ECode SurfaceTextureRenderer::InitializeGLState()
{
    FAIL_RETURN(CreateProgram(VERTEX_SHADER, FRAGMENT_SHADER, &mProgram))
    if (mProgram == 0) {
        //throw new IllegalStateException("failed creating program");
        Slogger::E(TAG, "failed creating program");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    assert(0 && "need opengl:: GLES20");
    // GLES20::GlGetAttribLocation(mProgram, String("aPosition"), &maPositionHandle);
    // FAIL_RETURN(CheckGlError("glGetAttribLocation aPosition"))
    // if (maPositionHandle == -1) {
    //     //throw new IllegalStateException("Could not get attrib location for aPosition");
    //     Slogger::E(TAG, "Could not get attrib location for aPosition");
    //     return E_ILLEGAL_STATE_EXCEPTION;
    // }
    // GLES20::GlGetAttribLocation(mProgram, String("aTextureCoord"), &maTextureHandle);
    // FAIL_RETURN(CheckGlError("glGetAttribLocation aTextureCoord"))
    // if (maTextureHandle == -1) {
    //     //throw new IllegalStateException("Could not get attrib location for aTextureCoord");
    //     Slogger::E(TAG, "Could not get attrib location for aTextureCoord");
    //     return E_ILLEGAL_STATE_EXCEPTION;
    // }

    // GLES20::GlGetUniformLocation(mProgram, String("uMVPMatrix"), &muMVPMatrixHandle);
    // FAIL_RETURN(CheckGlError("glGetUniformLocation uMVPMatrix"))
    // if (muMVPMatrixHandle == -1) {
    //     //throw new IllegalStateException("Could not get attrib location for uMVPMatrix");
    //     Slogger::E(TAG, "Could not get attrib location for uMVPMatrix");
    //     return E_ILLEGAL_STATE_EXCEPTION;
    // }

    // GLES20::GlGetUniformLocation(mProgram, String("uSTMatrix"), &muSTMatrixHandle);
    // FAIL_RETURN(CheckGlError("glGetUniformLocation uSTMatrix"))
    // if (muSTMatrixHandle == -1) {
    //     //throw new IllegalStateException("Could not get attrib location for uSTMatrix");
    //     Slogger::E(TAG, "Could not get attrib location for uSTMatrix");
    //     return E_ILLEGAL_STATE_EXCEPTION;
    // }

    // AutoPtr<ArrayOf<Int32> > textures = ArrayOf<Int32>::Alloc(1);
    // GLES20::GlGenTextures(/*n*/ 1, textures, /*offset*/ 0);

    // mTextureID = (*textures)[0];
    // GLES20::GlBindTexture(IGLES11Ext::GL_TEXTURE_EXTERNAL_OES, mTextureID);
    // FAIL_RETURN(CheckGlError("glBindTexture mTextureID"))

    // GLES20::GlTexParameterf(IGLES11Ext::GL_TEXTURE_EXTERNAL_OES, IGLES20::GL_TEXTURE_MIN_FILTER,
    //         IGLES20::GL_NEAREST);
    // GLES20::GlTexParameterf(IGLES11Ext::GL_TEXTURE_EXTERNAL_OES, IGLES20::GL_TEXTURE_MAG_FILTER,
    //         IGLES20::GL_LINEAR);
    // GLES20::GlTexParameteri(IGLES11Ext::GL_TEXTURE_EXTERNAL_OES, IGLES20::GL_TEXTURE_WRAP_S,
    //         IGLES20::GL_CLAMP_TO_EDGE);
    // GLES20::GlTexParameteri(IGLES11Ext::GL_TEXTURE_EXTERNAL_OES, IGLES20::GL_TEXTURE_WRAP_T,
    //         IGLES20::GL_CLAMP_TO_EDGE);
    return CheckGlError(String("glTexParameter"));
}

Int32 SurfaceTextureRenderer::GetTextureId()
{
    return mTextureID;
}

void SurfaceTextureRenderer::ClearState()
{
    mSurfaces->Clear();
    mConversionSurfaces->Clear();
    mPBufferPixels = NULL;
    if (mSurfaceTexture != NULL) {
        mSurfaceTexture->ReleaseBuffers();
    }
    mSurfaceTexture = NULL;
    return;
}

ECode SurfaceTextureRenderer::ConfigureEGLContext()
{
    assert(0 && "need opengl:: EGL14");
    // EGL14::EglGetDisplay(IEGL14::_EGL_DEFAULT_DISPLAY, (IEGLDisplay**)&mEGLDisplay);
    // AutoPtr<IEGL14Helper> helper;
    // CEGL14Helper::AcquireSingleton((IEGL14Helper**)&helper);
    // AutoPtr<IEGLDisplay> display;
    // helper->GetNoDisplay((IEGLDisplay**)&display);
    // if (mEGLDisplay == display) {
    //     //throw new IllegalStateException("No EGL14 display");
    //     Slogger::E(TAG, "No EGL14 display");
    //     return E_ILLEGAL_STATE_EXCEPTION;
    // }

    AutoPtr<ArrayOf<Int32> > version = ArrayOf<Int32>::Alloc(2);
    Boolean result;
    assert(0 && "need opengl:: EGL14");
    //EGL14::EglInitialize(mEGLDisplay, version, /*offset*/ 0, version, /*offset*/ 1, &result);
    if (!result) {
        //throw new IllegalStateException("Cannot initialize EGL14");
        Slogger::E(TAG, "Cannot initialize EGL14");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<ArrayOf<Int32> > attribList = ArrayOf<Int32>::Alloc(13);
    (*attribList)[0] = IEGL14::EGL_RED_SIZE;
    (*attribList)[1] = EGL_COLOR_BITLENGTH;
    (*attribList)[2] = IEGL14::EGL_GREEN_SIZE;
    (*attribList)[3] = EGL_COLOR_BITLENGTH;
    (*attribList)[4] = IEGL14::EGL_BLUE_SIZE;
    (*attribList)[5] = EGL_COLOR_BITLENGTH;
    (*attribList)[6] = IEGL14::EGL_RENDERABLE_TYPE;
    (*attribList)[7] = IEGL14::EGL_OPENGL_ES2_BIT;
    (*attribList)[8] = EGL_RECORDABLE_ANDROID;
    (*attribList)[9] = 1;
    (*attribList)[10] = IEGL14::EGL_SURFACE_TYPE;
    (*attribList)[11] = IEGL14::EGL_PBUFFER_BIT | IEGL14::EGL_WINDOW_BIT;
    (*attribList)[12] = IEGL14::EGL_NONE;

    AutoPtr<ArrayOf<IEGLConfig*> > configs = ArrayOf<IEGLConfig*>::Alloc(1);
    AutoPtr<ArrayOf<Int32> > numConfigs = ArrayOf<Int32>::Alloc(1);
    assert(0 && "need opengl:: EGL14");
    //EGL14::EglChooseConfig(mEGLDisplay, attribList, /*offset*/ 0, configs, /*offset*/ 0,
    //        configs->GetLength(), numConfigs, /*offset*/ 0);
    FAIL_RETURN(CheckEglError(String("eglCreateContext RGB888+recordable ES2")))
    mConfigs = (*configs)[0];

    AutoPtr<ArrayOf<Int32> > attrib_list = ArrayOf<Int32>::Alloc(3);
    (*attrib_list)[0] = IEGL14::EGL_CONTEXT_CLIENT_VERSION;
    (*attrib_list)[1] = GLES_VERSION;
    (*attrib_list)[2] = IEGL14::EGL_NONE;

    assert(0 && "need opengl:: EGL14");
    //EGL14::EglCreateContext(mEGLDisplay, configs[0], EGL14.EGL_NO_CONTEXT,
    //        attrib_list, /*offset*/ 0, (IEGLContext**)&mEGLContext);
    FAIL_RETURN(CheckEglError(String("eglCreateContext")))
    // AutoPtr<IEGLContext> context;
    // helper->GetNoContext((IEGLContext**)&context);
    // if(mEGLContext == context) {
    //     //throw new IllegalStateException("No EGLContext could be made");
    //     Slogger::E(TAG, "No EGLContext could be made");
    //     return E_ILLEGAL_STATE_EXCEPTION;
    // }
    return NOERROR;
}

ECode SurfaceTextureRenderer::ConfigureEGLOutputSurfaces(
    /* [in] */ ICollection* surfaces)
{

    if (surfaces == NULL) {
        //throw new IllegalStateException("No Surfaces were provided to draw to");
        Slogger::E(TAG, "No Surfaces were provided to draw to");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    else {
        Int32 size;
        surfaces->GetSize(&size);
        if (size == 0) {
            //throw new IllegalStateException("No Surfaces were provided to draw to");
            Slogger::E(TAG, "No Surfaces were provided to draw to");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
    }
    AutoPtr<ArrayOf<Int32> > surfaceAttribs = ArrayOf<Int32>::Alloc(1);
    (*surfaceAttribs)[0] = IEGL14::EGL_NONE;

    AutoPtr<ArrayOf<IInterface*> > array;
    surfaces->ToArray((ArrayOf<IInterface*>**)&array);
    for (Int32 i = 0; i < array->GetLength(); i++) {
        AutoPtr<IInterface> obj = (*array)[i];
        AutoPtr<EGLSurfaceHolder> holder = (EGLSurfaceHolder*)IObject::Probe(obj);

        //try {
        AutoPtr<ISize> size;
        LegacyCameraDevice::GetSurfaceSize(holder->mSurface, (ISize**)&size);
        Int32 width;
        size->GetWidth(&width);
        holder->mWidth = width;
        Int32 height;
        size->GetHeight(&height);
        holder->mHeight = height;

        AutoPtr<IEGLSurface> eglSurface;
        ECode ec;
        assert(0 && "neeed EGL14");
        // ec = EGL14::EglCreateWindowSurface(mEGLDisplay, mConfigs,
        //         surface, surfaceAttribs, /*offset*/ 0, (IEGLSurface**)&eglSurface);
        holder->mEglSurface = eglSurface;
        FAIL_RETURN(CheckEglError(String("eglCreateWindowSurface")))
        //} catch (LegacyExceptionUtils.BufferQueueAbandonedException e) {
        if (FAILED(ec)) {
            Slogger::W(TAG, "Surface abandoned, skipping...%d", ec);
        }
        //}
    }
    return NOERROR;
}

ECode SurfaceTextureRenderer::ConfigureEGLPbufferSurfaces(
        /* [in] */ ICollection* surfaces)
{
    if (surfaces == NULL) {
        //throw new IllegalStateException("No Surfaces were provided to draw to");
        Slogger::E(TAG, "No Surfaces were provided to draw to");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    else {
        Int32 size;
        surfaces->GetSize(&size);
        if (size == 0) {
            //throw new IllegalStateException("No Surfaces were provided to draw to");
            Slogger::E(TAG, "No Surfaces were provided to draw to");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
    }

    Int32 maxLength = 0;
    AutoPtr<ArrayOf<IInterface*> > array;
    surfaces->ToArray((ArrayOf<IInterface*>**)&array);
    for (Int32 i = 0; i < array->GetLength(); i++) {
        AutoPtr<IInterface> obj = (*array)[i];
        AutoPtr<EGLSurfaceHolder> holder = (EGLSurfaceHolder*)IObject::Probe(obj);

        //try {
        AutoPtr<ISize> size;
        LegacyCameraDevice::GetSurfaceSize(holder->mSurface, (ISize**)&size);
        Int32 width;
        size->GetWidth(&width);
        Int32 height;
        size->GetHeight(&height);
        Int32 length = width * height;
        // Find max surface size, ensure PBuffer can hold this many pixels
        maxLength = (length > maxLength) ? length : maxLength;
        AutoPtr<ArrayOf<Int32> > surfaceAttribs = ArrayOf<Int32>::Alloc(5);
        (*surfaceAttribs)[0] = IEGL14::EGL_WIDTH;
        (*surfaceAttribs)[1] = width;
        (*surfaceAttribs)[2] = IEGL14::EGL_HEIGHT;
        (*surfaceAttribs)[3] = height;
        (*surfaceAttribs)[4] = IEGL14::EGL_NONE;

        holder->mWidth = width;
        holder->mHeight = height;

        AutoPtr<IEGLSurface> eglSurface;
        ECode ec;
        assert(0 && "need EGL14");
        // ec = EGL14::EglCreatePbufferSurface(mEGLDisplay, mConfigs,
        //         surfaceAttribs, 0, (IEGLSurface**)&eglSurface);
        holder->mEglSurface = eglSurface;
        FAIL_RETURN(CheckEglError(String("eglCreatePbufferSurface")))
        //} catch (LegacyExceptionUtils.BufferQueueAbandonedException e) {
        if (FAILED(ec)) {
            Slogger::W(TAG, "Surface abandoned, skipping...%d", ec);
        }
        //}
    }

    AutoPtr<IByteBufferHelper> helper;
    CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&helper);
    AutoPtr<IByteBuffer> bBuffer;
    helper->AllocateDirect(maxLength * PBUFFER_PIXEL_BYTES,
            (IByteBuffer**)&bBuffer);

    AutoPtr<IByteOrderHelper> helper2;
    CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper2);
    ByteOrder bOrder;
    helper2->GetNativeOrder(&bOrder);

    bBuffer->SetOrder(bOrder);
    mPBufferPixels = bBuffer;
    return NOERROR;
}

void SurfaceTextureRenderer::ReleaseEGLContext()
{
    assert(0 && "need IEGL14Helper && EGL14");
    // AutoPtr<IEGL14Helper> helper;
    // CEGL14Helper::AcquireSingleton((IEGL14Helper**)&helper);
    // AutoPtr<IEGLDisplay> display;
    // helper->GetNoDisplay((IEGLDisplay**)&display);
    // if (mEGLDisplay != display) {
    //     Boolean result;
    //     EGL14::EglMakeCurrent(mEGLDisplay, IEGL14::EGL_NO_SURFACE, IEGL14::EGL_NO_SURFACE,
    //             IEGL14::EGL_NO_CONTEXT, &result);
    //     DumpGlTiming();
    //     if (mSurfaces != NULL) {
    //         Int32 size;
    //         mSurfaces->GetSize(&size);
    //         for (Int32 i = 0; i < size; i++) {
    //             AutoPtr<IInterface> obj;
    //             mSurfaces->Get(i, (IInterface**)&obj);
    //             AutoPtr<EGLSurfaceHolder> holder = (EGLSurfaceHolder*)IObject::Probe(obj);

    //             if (holder->mSurface != NULL) {
    //                 EGL14::EglDestroySurface(mEGLDisplay, holder->mSurface);
    //             }

    //         }
    //     }
    //     if (mConversionSurfaces != NULL) {
    //         Int32 size;
    //         mConversionSurfaces->GetSize(&size);
    //         for (Int32 i = 0; i < size; i++) {
    //             AutoPtr<IInterface> obj;
    //             mConversionSurfaces->Get(i, (IInterface**)&obj);
    //             AutoPtr<EGLSurfaceHolder> holder = (EGLSurfaceHolder*)IObject::Probe(obj);
    //             AutoPtr<IEGLSurface> surface = holder->mEglSurface;
    //             if (surface != NULL) {
    //                 EGL14::EglDestroySurface(mEGLDisplay, surface);
    //             }
    //         }
    //     }
    //     EGL14::EglDestroyContext(mEGLDisplay, mEGLContext);
    //     EGL14::EglReleaseThread();
    //     EGL14::EglTerminate(mEGLDisplay);
    // }

    // mConfigs = NULL;
    // mEGLDisplay = EGL14::EGL_NO_DISPLAY;
    // mEGLContext = EGL14::EGL_NO_CONTEXT;
    // ClearState();
    return;
}

ECode SurfaceTextureRenderer::MakeCurrent(
    /* [in] */ IEGLSurface* surface)
{
    assert(0 && "need EGL14");
    //EGL14::EglMakeCurrent(mEGLDisplay, surface, surface, mEGLContext);
    return CheckEglError(String("makeCurrent"));
}

ECode SurfaceTextureRenderer::SwapBuffers(
    /* [in] */ IEGLSurface* surface,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    assert(0 && "need EGL14");
    //EGL14::EglSwapBuffers(mEGLDisplay, surface, &result);
    return CheckEglError(String("swapBuffers"));
}

ECode SurfaceTextureRenderer::CheckEglError(
    /* [in] */ const String& msg)
{
    Int32 error;
    assert(0 && "need EGL14");
    //EGL14::EglGetError(&error);
    if (error != IEGL14::EGL_SUCCESS) {
        //throw new IllegalStateException(msg + ": EGL error: 0x" + Integer.toHexString(error));
        Slogger::E(TAG, "%s: EGL error: 0x%x", msg.string(), StringUtils::ToHexString(error).string());
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode SurfaceTextureRenderer::CheckGlError(
    /* [in] */ const String& msg)
{
    Int32 error;
    assert(0 && "need GLES20");
    //GLES20::GlGetError(&error);
    if (error != IGLES20::_GL_NO_ERROR) {
        //throw new IllegalStateException(msg + ": GLES20 error: 0x" + Integer.toHexString(error));
        Slogger::E(TAG, "%s: GLES20 error: 0x%x", msg.string(), StringUtils::ToHexString(error).string());
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

void SurfaceTextureRenderer::DumpGlTiming()
{
    if (mPerfMeasurer == NULL) return;

    AutoPtr<IFile> file = Environment::GetExternalStorageDirectory();
    AutoPtr<IFile> legacyStorageDir;
    CFile::New(file, String("CameraLegacy"), (IFile**)&legacyStorageDir);
    Boolean result;
    legacyStorageDir->Exists(&result);
    if (!result){
        legacyStorageDir->Mkdirs(&result);
        if (!result){
            Slogger::E(TAG, "Failed to create directory for data dump");
            return;
        }
    }

    String _path;
    legacyStorageDir->GetPath(&_path);
    StringBuilder path;
    path += _path;

    AutoPtr<IFileHelper> helper;
    CFileHelper::AcquireSingleton((IFileHelper**)&helper);
    String separator;
    helper->GetSeparator(&separator);

    path += separator;
    path += "durations_";

    AutoPtr<ITime> now;
    CTime::New((ITime**)&now);
    now->SetToNow();
    String format;
    now->Format2445(&format);
    path += format;
    path += "_S";

    Int32 size;
    mSurfaces->GetSize(&size);
    for (Int32 i = 0; i < size; i ++) {
        AutoPtr<IInterface> obj;
        mSurfaces->Get(i, (IInterface**)&obj);
        AutoPtr<EGLSurfaceHolder> surface = (EGLSurfaceHolder*)IObject::Probe(obj);

        StringBuilder sb;
        sb += "_";
        sb += surface->mWidth;
        sb += "_";
        sb += surface->mHeight;
        path += sb.ToString();
    }

    path += "_C";
    mConversionSurfaces->GetSize(&size);
    for (Int32 i = 0; i < size; i ++) {
        AutoPtr<IInterface> obj;
        mConversionSurfaces->Get(i, (IInterface**)&obj);
        AutoPtr<EGLSurfaceHolder> surface = (EGLSurfaceHolder*)IObject::Probe(obj);

        StringBuilder sb;
        sb += "_";
        sb += surface->mWidth;
        sb += "_";
        sb += surface->mHeight;
        path += sb.ToString();
    }

    path += ".txt";
    mPerfMeasurer->DumpPerformanceData(path.ToString());
    return;
}

void SurfaceTextureRenderer::SetupGlTiming()
{
    Boolean result;
    PerfMeasurement::IsGlTimingSupported(&result);
    if (result) {
        Slogger::D(TAG, "Enabling GL performance measurement");
        CPerfMeasurement::New((IPerfMeasurement**)&mPerfMeasurer);
    }
    else {
        Slogger::D(TAG, "GL performance measurement not supported on this device");
        mPerfMeasurer = NULL;
    }
    return;
}

void SurfaceTextureRenderer::BeginGlTiming()
{
    if (mPerfMeasurer == NULL) {
        return;
    }
    mPerfMeasurer->StartTimer();
    return;
}

void SurfaceTextureRenderer::AddGlTimestamp(
    /* [in] */ Int64 timestamp)
{
    if (mPerfMeasurer == NULL) return;
    mPerfMeasurer->AddTimestamp(timestamp);
    return;
}

void SurfaceTextureRenderer::EndGlTiming()
{
    if (mPerfMeasurer == NULL) return;
    mPerfMeasurer->StopTimer();
    return;
}

ECode SurfaceTextureRenderer::GetSurfaceTexture(
    /* [out] */ ISurfaceTexture** outst)
{
    VALIDATE_NOT_NULL(outst);

    *outst = mSurfaceTexture;
    REFCOUNT_ADD(*outst);
    return NOERROR;
}

ECode SurfaceTextureRenderer::ConfigureSurfaces(
    /* [in] */ ICollection* surfaces)
{
    ReleaseEGLContext();

    if (surfaces == NULL) {
        Slogger::W(TAG, "No output surfaces configured for GL drawing.");
        return NOERROR;
    }
    else {
        Int32 size;
        surfaces->GetSize(&size);
        if (size == 0) {
            Slogger::W(TAG, "No output surfaces configured for GL drawing.");
            return NOERROR;
        }
    }

    AutoPtr<ArrayOf<IInterface*> > array;
    surfaces->ToArray((ArrayOf<IInterface*>**)&array);
    for (Int32 i = 0; i < array->GetLength(); i++) {
        AutoPtr<IInterface> obj = (*array)[i];
        AutoPtr<ISurface> s = ISurface::Probe(obj);

        // If pixel conversions aren't handled by egl, use a pbuffer
        //try {
        Boolean result = LegacyCameraDevice::NeedsConversion(s);
        ECode ec = NOERROR;
        if (result) {
            // Always override to YV12 output for YUV surface formats.
            ec = LegacyCameraDevice::SetSurfaceFormat(s, IImageFormat::YV12);
            AutoPtr<EGLSurfaceHolder> holder = new EGLSurfaceHolder();
            holder->mSurface = s;
            mConversionSurfaces->Add(TO_IINTERFACE(holder));
        }
        else {
            AutoPtr<EGLSurfaceHolder> holder = new EGLSurfaceHolder();
            holder->mSurface = s;
            mSurfaces->Add(TO_IINTERFACE(holder));
        }
        //} catch (LegacyExceptionUtils.BufferQueueAbandonedException e) {
        if (FAILED(ec)) {
            Slogger::W(TAG, "Surface abandoned, skipping configuration... %d", ec);
        }
        //}
    }

    // Set up egl display
    ConfigureEGLContext();

    // Set up regular egl surfaces if needed
    Int32 size;
    mSurfaces->GetSize(&size);
    if (size > 0) {
        ConfigureEGLOutputSurfaces(ICollection::Probe(mSurfaces));
    }

    // Set up pbuffer surface if needed
    Int32 size2;
    mConversionSurfaces->GetSize(&size2);
    if (size2 > 0) {
        ConfigureEGLPbufferSurfaces(ICollection::Probe(mConversionSurfaces));
    }
    if (size > 0) {
        AutoPtr<IInterface> obj;
        mSurfaces->Get(0, (IInterface**)&obj);
        MakeCurrent(((EGLSurfaceHolder*)IObject::Probe(obj))->mEglSurface);
    }
    else {
        AutoPtr<IInterface> obj;
        mConversionSurfaces->Get(0, (IInterface**)&obj);
        MakeCurrent(((EGLSurfaceHolder*)IObject::Probe(obj))->mEglSurface);
    }
    InitializeGLState();
    CSurfaceTexture::New(GetTextureId(), (ISurfaceTexture**)&mSurfaceTexture);

    // Set up performance tracking if enabled
    Boolean result;
    SystemProperties::GetBoolean(LEGACY_PERF_PROPERTY, FALSE, &result);
    if (result) {
        SetupGlTiming();
    }
    return NOERROR;
}

ECode SurfaceTextureRenderer::DrawIntoSurfaces(
    /* [in] */ ICaptureCollector* targetCollector)
{
    Int32 size;
    mSurfaces->GetSize(&size);
    Int32 size2;
    mConversionSurfaces->GetSize(&size2);
    if ((mSurfaces == NULL || size == 0)
            && (mConversionSurfaces == NULL || size2 == 0)) {
        return NOERROR;
    }

    Boolean doTiming;
    targetCollector->HasPendingPreviewCaptures(&doTiming);
    FAIL_RETURN(CheckGlError(String("before updateTexImage")))

    if (doTiming) {
        BeginGlTiming();
    }

    mSurfaceTexture->UpdateTexImage();

    Int64 timestamp;
    mSurfaceTexture->GetTimestamp(&timestamp);

    AutoPtr<IPair> captureHolder;
    targetCollector->PreviewCaptured(timestamp, (IPair**)&captureHolder);

    // No preview request queued, drop frame.
    if (captureHolder == NULL) {
        if (DEBUG) {
            Slogger::D(TAG, "Dropping preview frame.");
        }
        if (doTiming) {
            EndGlTiming();
        }
        return NOERROR;
    }

    AutoPtr<IInterface> obj;
    captureHolder->GetFirst((IInterface**)&obj);
    AutoPtr<IRequestHolder> request = IRequestHolder::Probe(obj);

    AutoPtr<ICollection> targetSurfaces;
    request->GetHolderTargets((ICollection**)&targetSurfaces);
    if (doTiming) {
        AddGlTimestamp(timestamp);
    }

    AutoPtr<IList> targetSurfaceIds;
    LegacyCameraDevice::GetSurfaceIds(targetSurfaces, (IList**)&targetSurfaceIds);

    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> obj;
        mSurfaces->Get(i, (IInterface**)&obj);
        AutoPtr<EGLSurfaceHolder> holder = (EGLSurfaceHolder*)IObject::Probe(obj);

        Boolean result = LegacyCameraDevice::ContainsSurfaceId(holder->mSurface,
                ICollection::Probe(targetSurfaceIds));
        if (result) {
            MakeCurrent(holder->mEglSurface);
            //try {
            LegacyCameraDevice::SetSurfaceDimens(holder->mSurface, holder->mWidth,
                    holder->mHeight);
            AutoPtr<IInterface> obj;
            captureHolder->GetSecond((IInterface**)&obj);
            AutoPtr<IInteger64> num = IInteger64::Probe(obj);
            Int64 second;
            num->GetValue(&second);
            ECode ec = LegacyCameraDevice::SetNextTimestamp(holder->mSurface, second);
            DrawFrame(mSurfaceTexture, holder->mWidth, holder->mHeight);
            Boolean res;
            SwapBuffers(holder->mEglSurface, &res);
            //} catch (LegacyExceptionUtils.BufferQueueAbandonedException e) {
            if (FAILED(ec)) {
                Slogger::W(TAG, "Surface abandoned, dropping frame. %d", ec);
            }
            //}
        }
    }

    mConversionSurfaces->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> obj;
        mConversionSurfaces->Get(i, (IInterface**)&obj);
        AutoPtr<EGLSurfaceHolder> holder = (EGLSurfaceHolder*)IObject::Probe(obj);

        Boolean result = LegacyCameraDevice::ContainsSurfaceId(holder->mSurface,
                ICollection::Probe(targetSurfaceIds));
        if (result) {
            MakeCurrent(holder->mEglSurface);
            DrawFrame(mSurfaceTexture, holder->mWidth, holder->mHeight);
            IBuffer::Probe(mPBufferPixels)->Clear();
            assert(0 && "need GLES20");
            // GLES20::GlReadPixels(/*x*/ 0, /*y*/ 0, holder->mWidth, holder->mHeight,
            //         IGLES20::GL_RGBA, IGLES20::GL_UNSIGNED_BYTE, mPBufferPixels);
            FAIL_RETURN(CheckGlError(String("glReadPixels")))

            //try {
            Int32 format;
            ECode ec = LegacyCameraDevice::DetectSurfaceType(holder->mSurface, &format);
            if (FAILED(ec)) {
                Slogger::W(TAG, "Surface abandoned, dropping frame. %d", ec);
            }
            AutoPtr<IInterface> obj;
            captureHolder->GetSecond((IInterface**)&obj);
            AutoPtr<IInteger64> num = IInteger64::Probe(obj);
            Int64 second;
            num->GetValue(&second);
            ec = LegacyCameraDevice::SetNextTimestamp(holder->mSurface, second);
            if (FAILED(ec)) {
                Slogger::W(TAG, "Surface abandoned, dropping frame. %d", ec);
            }
            AutoPtr<ArrayOf<Byte> > array;
            mPBufferPixels->GetArray((ArrayOf<Byte>**)&array);
            ec = LegacyCameraDevice::ProduceFrame(holder->mSurface, array,
                    holder->mWidth, holder->mHeight, format);
            if (FAILED(ec)) {
                Slogger::W(TAG, "Surface abandoned, dropping frame. %d", ec);
            }
            Boolean res;
            SwapBuffers(holder->mEglSurface, &res);
            //} catch (LegacyExceptionUtils.BufferQueueAbandonedException e) {
            if (FAILED(ec)) {
                Slogger::W(TAG, "Surface abandoned, dropping frame. %d", ec);
            }
            //}
        }
    }

    AutoPtr<IRequestHolder> tmp;
    targetCollector->PreviewProduced((IRequestHolder**)&tmp);

    if (doTiming) {
        EndGlTiming();
    }
    return NOERROR;
}

ECode SurfaceTextureRenderer::CleanupEGLContext()
{
    ReleaseEGLContext();
    return NOERROR;
}

ECode SurfaceTextureRenderer::Flush()
{
    // TODO: implement flush
    Slogger::E(TAG, "Flush not yet implemented.");
    return NOERROR;
}

} // namespace Legacy
} // namespace Camera2
} // namespace Hardware
} // namespace Droid
} // namespace Elastos
