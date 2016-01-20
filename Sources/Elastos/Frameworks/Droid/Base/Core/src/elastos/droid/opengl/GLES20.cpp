
#include "Elastos.Droid.View.h"
#include "elastos/droid/opengl/GLES20.h"
#include "elastos/droid/opengl/gles/CEGLContextImpl.h"
#include "elastos/droid/opengl/gles/CEGL10Helper.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <elastos/utility/logging/Slogger.h>
#include <cutils/log.h>

using Elastos::Droid::Opengl::Gles::CEGLContextImpl;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Opengl::Gles::CEGL10Helper;
using Elastosx::Microedition::Khronos::Egl::IEGL10Helper;
using Elastos::Droid::View::ISurface;

using Elastos::IO::CNIOAccess;
using Elastos::IO::INIOAccess;

namespace Elastos {
namespace Droid {
namespace Opengl {
//begin a temporal workaroud for egl initialize
AutoPtr<IEGL10> GLES20::mEgl;
AutoPtr<XIEGLDisplay> GLES20::mEglDisplay;
AutoPtr<XIEGLSurface> GLES20::mEglSurface;
AutoPtr<XIEGLConfig> GLES20::mEglConfig;
AutoPtr<XIEGLContext> GLES20::mEglContext;
//end a temporal workaroud for egl initialize

static int
getNumCompressedTextureFormats() {
    int numCompressedTextureFormats = 0;
    glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &numCompressedTextureFormats);
    return numCompressedTextureFormats;
}

static void glVertexAttribPointerBounds(GLuint indx, GLint size, GLenum type,
        GLboolean normalized, GLsizei stride, const GLvoid *pointer, GLsizei count) {
    glVertexAttribPointer(indx, size, type, normalized, stride, pointer);
}

CAR_INTERFACE_IMPL(GLES20, Singleton, IGLES20)

ECode GLES20::GlActiveTexture(
    /* [in] */ Int32 texture)
{
    glActiveTexture(
        (GLenum)texture
    );
    return NOERROR;
}

// C function void glAttachShader ( GLuint program, GLuint shader )

ECode GLES20::GlAttachShader(
    /* [in] */ Int32 program,
    /* [in] */ Int32 shader)
{
    glAttachShader(
        (GLuint)program,
        (GLuint)shader
    );
    return NOERROR;
}

// C function void glBindAttribLocation ( GLuint program, GLuint index, const char *name )

ECode GLES20::GlBindAttribLocation(
    /* [in] */ Int32 program,
    /* [in] */ Int32 index,
    /* [in] */ const String& name)
{
    const char* _nativename = 0;

    if (!name) {
        SLOGGERD("GLES20", "GlBindAttribLocation: name == NULL")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _nativename = name.string();

    glBindAttribLocation(
        (GLuint)program,
        (GLuint)index,
        (char *)_nativename
    );
    return NOERROR;
}

// C function void glBindBuffer ( GLenum target, GLuint buffer )

ECode GLES20::GlBindBuffer(
    /* [in] */ Int32 target,
    /* [in] */ Int32 buffer)
{
    glBindBuffer(
        (GLenum)target,
        (GLuint)buffer
    );
    return NOERROR;
}

// C function void glBindFramebuffer ( GLenum target, GLuint framebuffer )

ECode GLES20::GlBindFramebuffer(
    /* [in] */ Int32 target,
    /* [in] */ Int32 framebuffer)
{
    glBindFramebuffer(
        (GLenum)target,
        (GLuint)framebuffer
    );
    return NOERROR;
}

// C function void glBindRenderbuffer ( GLenum target, GLuint renderbuffer )

ECode GLES20::GlBindRenderbuffer(
    /* [in] */ Int32 target,
    /* [in] */ Int32 renderbuffer)
{
    glBindRenderbuffer(
        (GLenum)target,
        (GLuint)renderbuffer
    );
    return NOERROR;
}

// C function void glBindTexture ( GLenum target, GLuint texture )

ECode GLES20::GlBindTexture(
    /* [in] */ Int32 target,
    /* [in] */ Int32 texture)
{
    glBindTexture(
        (GLenum)target,
        (GLuint)texture
    );
    return NOERROR;
}

// C function void glBlendColor ( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )

ECode GLES20::GlBlendColor(
    /* [in] */ Float red,
    /* [in] */ Float green,
    /* [in] */ Float blue,
    /* [in] */ Float alpha)
{
    glBlendColor(
        (GLclampf)red,
        (GLclampf)green,
        (GLclampf)blue,
        (GLclampf)alpha
    );
    return NOERROR;
}

// C function void glBlendEquation ( GLenum mode )

ECode GLES20::GlBlendEquation(
    /* [in] */ Int32 mode)
{
    glBlendEquation(
        (GLenum)mode
    );
    return NOERROR;
}

// C function void glBlendEquationSeparate ( GLenum modeRGB, GLenum modeAlpha )

ECode GLES20::GlBlendEquationSeparate(
    /* [in] */ Int32 modeRGB,
    /* [in] */ Int32 modeAlpha)
{
    glBlendEquationSeparate(
        (GLenum)modeRGB,
        (GLenum)modeAlpha
    );
    return NOERROR;
}

// C function void glBlendFunc ( GLenum sfactor, GLenum dfactor )

ECode GLES20::GlBlendFunc(
    /* [in] */ Int32 sfactor,
    /* [in] */ Int32 dfactor)
{
    glBlendFunc(
        (GLenum)sfactor,
        (GLenum)dfactor
    );
    return NOERROR;
}

// C function void glBlendFuncSeparate ( GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha )

ECode GLES20::GlBlendFuncSeparate(
    /* [in] */ Int32 srcRGB,
    /* [in] */ Int32 dstRGB,
    /* [in] */ Int32 srcAlpha,
    /* [in] */ Int32 dstAlpha)
{
    glBlendFuncSeparate(
        (GLenum)srcRGB,
        (GLenum)dstRGB,
        (GLenum)srcAlpha,
        (GLenum)dstAlpha
    );
    return NOERROR;
}

// C function void glBufferData ( GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage )

ECode GLES20::GlBufferData(
    /* [in] */ Int32 target,
    /* [in] */ Int32 size,
    /* [in] */ IBuffer* data_buf,
    /* [in] */ Int32 usage)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLvoid *data = (GLvoid *) 0;

    if (data_buf) {
        Handle64 tmp;
        FAIL_RETURN(GetPointer(data_buf, &_array, &_remaining, &_bufferOffset, &tmp));
        data = (GLvoid *) tmp;
        if (_remaining < size) {
            SLOGGERD("GLES20", "GlBufferData: remaining() < size < needed")
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
    if (data_buf && data == NULL) {
        char * _dataBase = reinterpret_cast<char *>(_array);
        data = (GLvoid *) (_dataBase + _bufferOffset);
    }
    glBufferData(
        (GLenum)target,
        (GLsizeiptr)size,
        (GLvoid *)data,
        (GLenum)usage
    );
    return NOERROR;
}

// C function void glBufferSubData ( GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data )

ECode GLES20::GlBufferSubData(
    /* [in] */ Int32 target,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 size,
    /* [in] */ IBuffer* data_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLvoid *data = (GLvoid *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(data_buf, &_array, &_remaining, &_bufferOffset, &tmp));
    data = (GLvoid *) tmp;
    if (_remaining < size) {
        SLOGGERD("GLES20", "GlBufferSubData: remaining() < size < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (data == NULL) {
        char * _dataBase = reinterpret_cast<char *>(_array);
        data = (GLvoid *) (_dataBase + _bufferOffset);
    }
    glBufferSubData(
        (GLenum)target,
        (GLintptr)offset,
        (GLsizeiptr)size,
        (GLvoid *)data
    );
    return NOERROR;
}

// C function GLenum glCheckFramebufferStatus ( GLenum target )

ECode GLES20::GlCheckFramebufferStatus(
    /* [in] */ Int32 target,
    /* [out] */ Int32* rst)
{
    GLenum _returnValue;
    _returnValue = glCheckFramebufferStatus(
        (GLenum)target
    );
    *rst = _returnValue;
    return NOERROR;
}

// C function void glClear ( GLbitfield mask )

ECode GLES20::GlClear(
    /* [in] */ Int32 mask)
{
    glClear(
        (GLbitfield)mask
    );
    return NOERROR;
}

// C function void glClearColor ( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )

ECode GLES20::GlClearColor(
    /* [in] */ Float red,
    /* [in] */ Float green,
    /* [in] */ Float blue,
    /* [in] */ Float alpha)
{
    glClearColor(
        (GLclampf)red,
        (GLclampf)green,
        (GLclampf)blue,
        (GLclampf)alpha
    );
    return NOERROR;
}

// C function void glClearDepthf ( GLclampf depth )

ECode GLES20::GlClearDepthf(
    /* [in] */ Float depth)
{
    glClearDepthf(
        (GLclampf)depth
    );
    return NOERROR;
}

// C function void glClearStencil ( GLint s )

ECode GLES20::GlClearStencil(
    /* [in] */ Int32 s)
{
    glClearStencil(
        (GLint)s
    );
    return NOERROR;
}

// C function void glColorMask ( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha )

ECode GLES20::GlColorMask(
    /* [in] */ Boolean red,
    /* [in] */ Boolean green,
    /* [in] */ Boolean blue,
    /* [in] */ Boolean alpha)
{
    glColorMask(
        (GLboolean)red,
        (GLboolean)green,
        (GLboolean)blue,
        (GLboolean)alpha
    );
    return NOERROR;
}

// C function void glCompileShader ( GLuint shader )

ECode GLES20::GlCompileShader(
    /* [in] */ Int32 shader)
{
    glCompileShader(
        (GLuint)shader
    );
    return NOERROR;
}

// C function void glCompressedTexImage2D ( GLenum target, GLint level, GLenum internalformat,
// GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data )

ECode GLES20::GlCompressedTexImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 border,
    /* [in] */ Int32 imageSize,
    /* [in] */ IBuffer* data_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLvoid *data = (GLvoid *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(data_buf, &_array, &_remaining, &_bufferOffset, &tmp));
    data = (GLvoid *) tmp;
    if (data == NULL) {
        char * _dataBase = reinterpret_cast<char *>(_array);
        data = (GLvoid *) (_dataBase + _bufferOffset);
    }
    glCompressedTexImage2D(
        (GLenum)target,
        (GLint)level,
        (GLenum)internalformat,
        (GLsizei)width,
        (GLsizei)height,
        (GLint)border,
        (GLsizei)imageSize,
        (GLvoid *)data
    );

    return NOERROR;
}

// C function void glCompressedTexSubImage2D ( GLenum target, GLint level, GLint xoffset, GLint yoffset,
// GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data )

ECode GLES20::GlCompressedTexSubImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 xoffset,
    /* [in] */ Int32 yoffset,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 format,
    /* [in] */ Int32 imageSize,
    /* [in] */ IBuffer* data_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLvoid *data = (GLvoid *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(data_buf, &_array, &_remaining, &_bufferOffset, &tmp));
    data = (GLvoid *) tmp;
    if (data == NULL) {
        char * _dataBase = reinterpret_cast<char *>(_array);
        data = (GLvoid *) (_dataBase + _bufferOffset);
    }
    glCompressedTexSubImage2D(
        (GLenum)target,
        (GLint)level,
        (GLint)xoffset,
        (GLint)yoffset,
        (GLsizei)width,
        (GLsizei)height,
        (GLenum)format,
        (GLsizei)imageSize,
        (GLvoid *)data
    );
    return NOERROR;
}

// C function void glCopyTexImage2D ( GLenum target, GLint level, GLenum internalformat, GLint x,
// GLint y, GLsizei width, GLsizei height, GLint border )

ECode GLES20::GlCopyTexImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 border)
{
    glCopyTexImage2D(
        (GLenum)target,
        (GLint)level,
        (GLenum)internalformat,
        (GLint)x,
        (GLint)y,
        (GLsizei)width,
        (GLsizei)height,
        (GLint)border
    );
    return NOERROR;
}

// C function void glCopyTexSubImage2D ( GLenum target, GLint level, GLint xoffset, GLint yoffset,
// GLint x, GLint y, GLsizei width, GLsizei height )

ECode GLES20::GlCopyTexSubImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 xoffset,
    /* [in] */ Int32 yoffset,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    glCopyTexSubImage2D(
        (GLenum)target,
        (GLint)level,
        (GLint)xoffset,
        (GLint)yoffset,
        (GLint)x,
        (GLint)y,
        (GLsizei)width,
        (GLsizei)height
    );
    return NOERROR;
}

// C function GLuint glCreateProgram ( void )

ECode GLES20::GlCreateProgram(
    /* [out] */ Int32* rst)
{
    GLuint _returnValue;
    _returnValue = glCreateProgram();
    *rst = _returnValue;
    return NOERROR;
}

// C function GLuint glCreateShader ( GLenum type )

ECode GLES20::GlCreateShader(
    /* [in] */ Int32 type,
    /* [out] */ Int32* rst)
{
    GLuint _returnValue;
    _returnValue = glCreateShader(
        (GLenum)type
    );
    *rst = _returnValue;
    return NOERROR;
}

// C function void glCullFace ( GLenum mode )

ECode GLES20::GlCullFace(
    /* [in] */ Int32 mode)
{
    glCullFace(
        (GLenum)mode
    );
    return NOERROR;
}

// C function void glDeleteBuffers ( GLsizei n, const GLuint *buffers )

ECode GLES20::GlDeleteBuffers(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* buffers_ref,
    /* [in] */ Int32 offset)
{
    GLuint *buffers_base = (GLuint *) 0;
    Int32 _remaining;
    GLuint *buffers = (GLuint *) 0;

    if (!buffers_ref) {
        SLOGGERD("GLES20", "GlDeleteBuffers: buffers == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlDeleteBuffers: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = buffers_ref->GetLength() - offset;
    if (_remaining < n) {
        SLOGGERD("GLES20", "GlDeleteBuffers: length - offset < n < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    buffers_base = (GLuint *) buffers_ref->GetPayload();
    buffers = buffers_base + offset;

    glDeleteBuffers(
        (GLsizei)n,
        (GLuint *)buffers
    );
    return NOERROR;
}

// C function void glDeleteBuffers ( GLsizei n, const GLuint *buffers )

ECode GLES20::GlDeleteBuffers(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* buffers_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLvoid *buffers = (GLvoid *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(buffers_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    buffers = (GLvoid *) tmp;
    if (_remaining < n) {
        SLOGGERD("GLES20", "GlDeleteBuffersEx: remaining() < n < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (buffers == NULL) {
        char * _buffersBase = reinterpret_cast<char *>(_array);
        buffers = (GLuint *) (_buffersBase + _bufferOffset);
    }
    glDeleteBuffers(
        (GLsizei)n,
        (GLuint *)buffers
    );
    return NOERROR;
}

// C function void glDeleteFramebuffers ( GLsizei n, const GLuint *framebuffers )

ECode GLES20::GlDeleteFramebuffers(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* framebuffers_ref,
    /* [in] */ Int32 offset)
{
    GLuint *framebuffers_base = (GLuint *) 0;
    GLuint *framebuffers = (GLuint *) 0;

    if (!framebuffers_ref) {
        SLOGGERD("GLES20", "GlDeleteFramebuffers: framebuffers == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlDeleteFramebuffers: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    framebuffers_base = (GLuint *) framebuffers_ref->GetPayload();
    framebuffers = framebuffers_base + offset;

    glDeleteFramebuffers(
        (GLsizei)n,
        (GLuint *)framebuffers
    );
    return NOERROR;
}

// C function void glDeleteFramebuffers ( GLsizei n, const GLuint *framebuffers )

ECode GLES20::GlDeleteFramebuffers(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* framebuffers_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLuint *framebuffers = (GLuint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(framebuffers_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    framebuffers = (GLuint *) tmp;
    if (framebuffers == NULL) {
        char * _framebuffersBase = reinterpret_cast<char *>(_array);
        framebuffers = (GLuint *) (_framebuffersBase + _bufferOffset);
    }
    glDeleteFramebuffers(
        (GLsizei)n,
        (GLuint *)framebuffers
    );
    return NOERROR;
}

// C function void glDeleteProgram ( GLuint program )

ECode GLES20::GlDeleteProgram(
    /* [in] */ Int32 program)
{
    glDeleteProgram(
        (GLuint)program
    );
    return NOERROR;
}

// C function void glDeleteRenderbuffers ( GLsizei n, const GLuint *renderbuffers )

ECode GLES20::GlDeleteRenderbuffers(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* renderbuffers_ref,
    /* [in] */ Int32 offset)
{
    GLuint *renderbuffers_base = (GLuint *) 0;
    GLuint *renderbuffers = (GLuint *) 0;

    if (!renderbuffers_ref) {
        SLOGGERD("GLES20", "GlDeleteRenderbuffers: renderbuffers == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlDeleteRenderbuffers: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    renderbuffers_base = (GLuint *) renderbuffers_ref->GetPayload();
    renderbuffers = renderbuffers_base + offset;

    glDeleteRenderbuffers(
        (GLsizei)n,
        (GLuint *)renderbuffers
    );
    return NOERROR;
}

// C function void glDeleteRenderbuffers ( GLsizei n, const GLuint *renderbuffers )

ECode GLES20::GlDeleteRenderbuffers(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* renderbuffers_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLuint *renderbuffers = (GLuint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(renderbuffers_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    renderbuffers = (GLuint *) tmp;
    if (renderbuffers == NULL) {
        char * _renderbuffersBase = reinterpret_cast<char *>(_array);
        renderbuffers = (GLuint *) (_renderbuffersBase + _bufferOffset);
    }
    glDeleteRenderbuffers(
        (GLsizei)n,
        (GLuint *)renderbuffers
    );
    return NOERROR;
}

// C function void glDeleteShader ( GLuint shader )

ECode GLES20::GlDeleteShader(
    /* [in] */ Int32 shader)
{
    glDeleteShader(
        (GLuint)shader
    );
    return NOERROR;
}

// C function void glDeleteTextures ( GLsizei n, const GLuint *textures )

ECode GLES20::GlDeleteTextures(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* textures_ref,
    /* [in] */ Int32 offset)
{
    GLuint *textures_base = (GLuint *) 0;
    Int32 _remaining;
    GLuint *textures = (GLuint *) 0;

    if (!textures_ref) {
        SLOGGERD("GLES20", "GlDeleteTextures: textures == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlDeleteTextures: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = textures_ref->GetLength() - offset;
    if (_remaining < n) {
        SLOGGERD("GLES20", "GlDeleteTextures: length - offset < n < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    textures_base = (GLuint *) textures_ref->GetPayload();
    textures = textures_base + offset;

    glDeleteTextures(
        (GLsizei)n,
        (GLuint *)textures
    );
    return NOERROR;
}

// C function void glDeleteTextures ( GLsizei n, const GLuint *textures )

ECode GLES20::GlDeleteTextures(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* textures_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLuint *textures = (GLuint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(textures_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    textures = (GLuint *)tmp;
    if (_remaining < n) {
        SLOGGERD("GLES20", "GlDeleteTexturesEx: remaining() < n < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (textures == NULL) {
        char * _texturesBase = reinterpret_cast<char *>(_array);
        textures = (GLuint *) (_texturesBase + _bufferOffset);
    }
    glDeleteTextures(
        (GLsizei)n,
        (GLuint *)textures
    );
    return NOERROR;
}

// C function void glDepthFunc ( GLenum func )

ECode GLES20::GlDepthFunc(
    /* [in] */ Int32 func)
{
    glDepthFunc(
        (GLenum)func
    );
    return NOERROR;
}

// C function void glDepthMask ( GLboolean flag )

ECode GLES20::GlDepthMask(
    /* [in] */ Boolean flag)
{
    glDepthMask(
        (GLboolean)flag
    );
    return NOERROR;
}

// C function void glDepthRangef ( GLclampf zNear, GLclampf zFar )

ECode GLES20::GlDepthRangef(
    /* [in] */ Float zNear,
    /* [in] */ Float zFar)
{
    glDepthRangef(
        (GLclampf)zNear,
        (GLclampf)zFar
    );
    return NOERROR;
}

// C function void glDetachShader ( GLuint program, GLuint shader )

ECode GLES20::GlDetachShader(
    /* [in] */ Int32 program,
    /* [in] */ Int32 shader)
{
    glDetachShader(
        (GLuint)program,
        (GLuint)shader
    );
    return NOERROR;
}

// C function void glDisable ( GLenum cap )

ECode GLES20::GlDisable(
    /* [in] */ Int32 cap)
{
    glDisable(
        (GLenum)cap
    );
    return NOERROR;
}

// C function void glDisableVertexAttribArray ( GLuint index )

ECode GLES20::GlDisableVertexAttribArray(
    /* [in] */ Int32 index)
{
    glDisableVertexAttribArray(
        (GLuint)index
    );
    return NOERROR;
}

// C function void glDrawArrays ( GLenum mode, GLint first, GLsizei count )

ECode GLES20::GlDrawArrays(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 first,
    /* [in] */ Int32 count)
{
    glDrawArrays(
        (GLenum)mode,
        (GLint)first,
        (GLsizei)count
    );
    return NOERROR;
}

// C function void glDrawElements ( GLenum mode, GLsizei count, GLenum type, GLint offset )

ECode GLES20::GlDrawElements(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 count,
    /* [in] */ Int32 type,
    /* [in] */ Int32 offset)
{
    glDrawElements(
        (GLenum)mode,
        (GLsizei)count,
        (GLenum)type,
        (const GLvoid *)offset
    );
    return NOERROR;
}

// C function void glDrawElements ( GLenum mode, GLsizei count, GLenum type, const GLvoid *indices )

ECode GLES20::GlDrawElements(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 count,
    /* [in] */ Int32 type,
    /* [in] */ IBuffer* indices_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLvoid *indices = (GLvoid *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(indices_buf, &_array, &_remaining, &_bufferOffset, &tmp));
    indices = (GLvoid *)tmp;
    if (_remaining < count) {
        SLOGGERD("GLES20", "GlDrawElementsEx: remaining() < count < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (indices == NULL) {
        char * _indicesBase = reinterpret_cast<char *>(_array);
        indices = (GLvoid *) (_indicesBase + _bufferOffset);
    }
    glDrawElements(
        (GLenum)mode,
        (GLsizei)count,
        (GLenum)type,
        (GLvoid *)indices
    );
    return NOERROR;
}

// C function void glEnable ( GLenum cap )

ECode GLES20::GlEnable(
    /* [in] */ Int32 cap)
{
    glEnable(
        (GLenum)cap
    );
    return NOERROR;
}

// C function void glEnableVertexAttribArray ( GLuint index )

ECode GLES20::GlEnableVertexAttribArray(
    /* [in] */ Int32 index)
{
    glEnableVertexAttribArray(
        (GLuint)index
    );
    return NOERROR;
}

// C function void glFinish ( void )

ECode GLES20::GlFinish()
{
    glFinish();
    return NOERROR;
}

// C function void glFlush ( void )

ECode GLES20::GlFlush()
{
    glFlush();
    return NOERROR;
}

// C function void glFramebufferRenderbuffer ( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )

ECode GLES20::GlFramebufferRenderbuffer(
    /* [in] */ Int32 target,
    /* [in] */ Int32 attachment,
    /* [in] */ Int32 renderbuffertarget,
    /* [in] */ Int32 renderbuffer)
{
    glFramebufferRenderbuffer(
        (GLenum)target,
        (GLenum)attachment,
        (GLenum)renderbuffertarget,
        (GLuint)renderbuffer
    );
    return NOERROR;
}

// C function void glFramebufferTexture2D ( GLenum target, GLenum attachment, GLenum textarget,
// GLuint texture, GLint level )

ECode GLES20::GlFramebufferTexture2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 attachment,
    /* [in] */ Int32 textarget,
    /* [in] */ Int32 texture,
    /* [in] */ Int32 level)
{
    glFramebufferTexture2D(
        (GLenum)target,
        (GLenum)attachment,
        (GLenum)textarget,
        (GLuint)texture,
        (GLint)level
    );
    return NOERROR;
}

// C function void glFrontFace ( GLenum mode )

ECode GLES20::GlFrontFace(
    /* [in] */ Int32 mode)
{
    glFrontFace(
        (GLenum)mode
    );
    return NOERROR;
}

// C function void glGenBuffers ( GLsizei n, GLuint *buffers )

ECode GLES20::GlGenBuffers(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* buffers_ref,
    /* [in] */ Int32 offset)
{
    GLuint *buffers_base = (GLuint *) 0;
    Int32 _remaining;
    GLuint *buffers = (GLuint *) 0;

    if (!buffers_ref) {
        SLOGGERD("GLES20", "GlGenBuffers: buffers == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGenBuffers: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = buffers_ref->GetLength() - offset;
    if (_remaining < n) {
        SLOGGERD("GLES20", "GlGenBuffers: length - offset < n < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    buffers_base = (GLuint *) buffers_ref->GetPayload();
    buffers = buffers_base + offset;

    glGenBuffers(
        (GLsizei)n,
        (GLuint *)buffers
    );
    return NOERROR;
}

// C function void glGenBuffers ( GLsizei n, GLuint *buffers )

ECode GLES20::GlGenBuffers(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* buffers_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLuint *buffers = (GLuint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(buffers_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    buffers = (GLuint *)tmp;
    if (_remaining < n) {
        SLOGGERD("GLES20", "GlGenBuffersEx: remaining() < n < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (buffers == NULL) {
        char * _buffersBase = reinterpret_cast<char *>(_array);
        buffers = (GLuint *) (_buffersBase + _bufferOffset);
    }
    glGenBuffers(
        (GLsizei)n,
        (GLuint *)buffers
    );
    return NOERROR;
}

// C function void glGenerateMipmap ( GLenum target )

ECode GLES20::GlGenerateMipmap(
    /* [in] */ Int32 target)
{
    glGenerateMipmap(
        (GLenum)target
    );
    return NOERROR;
}

// C function void glGenFramebuffers ( GLsizei n, GLuint *framebuffers )

ECode GLES20::GlGenFramebuffers(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* framebuffers_ref,
    /* [in] */ Int32 offset)
{
    GLuint *framebuffers_base = (GLuint *) 0;
    GLuint *framebuffers = (GLuint *) 0;

    if (!framebuffers_ref) {
        SLOGGERD("GLES20", "GlGenFramebuffers: framebuffers == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGenFramebuffers: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    framebuffers_base = (GLuint *) framebuffers_ref->GetPayload();
    framebuffers = framebuffers_base + offset;

    glGenFramebuffers(
        (GLsizei)n,
        (GLuint *)framebuffers
    );
    return NOERROR;
}

// C function void glGenFramebuffers ( GLsizei n, GLuint *framebuffers )

ECode GLES20::GlGenFramebuffers(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* framebuffers_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLuint *framebuffers = (GLuint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(framebuffers_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    framebuffers = (GLuint *)tmp;
    if (framebuffers == NULL) {
        char * _framebuffersBase = reinterpret_cast<char *>(_array);
        framebuffers = (GLuint *) (_framebuffersBase + _bufferOffset);
    }
    glGenFramebuffers(
        (GLsizei)n,
        (GLuint *)framebuffers
    );
    return NOERROR;
}

// C function void glGenRenderbuffers ( GLsizei n, GLuint *renderbuffers )

ECode GLES20::GlGenRenderbuffers(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* renderbuffers_ref,
    /* [in] */ Int32 offset)
{
    GLuint *renderbuffers_base = (GLuint *) 0;
    GLuint *renderbuffers = (GLuint *) 0;

    if (!renderbuffers_ref) {
        SLOGGERD("GLES20", "GlGenRenderbuffers: renderbuffers == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGenRenderbuffers: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    renderbuffers_base = (GLuint *) renderbuffers_ref->GetPayload();
    renderbuffers = renderbuffers_base + offset;

    glGenRenderbuffers(
        (GLsizei)n,
        (GLuint *)renderbuffers
    );
    return NOERROR;
}

// C function void glGenRenderbuffers ( GLsizei n, GLuint *renderbuffers )

ECode GLES20::GlGenRenderbuffers(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* renderbuffers_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLuint *renderbuffers = (GLuint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(renderbuffers_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    renderbuffers = (GLuint *)tmp;
    if (renderbuffers == NULL) {
        char * _renderbuffersBase = reinterpret_cast<char *>(_array);
        renderbuffers = (GLuint *) (_renderbuffersBase + _bufferOffset);
    }
    glGenRenderbuffers(
        (GLsizei)n,
        (GLuint *)renderbuffers
    );
    return NOERROR;
}

// C function void glGenTextures ( GLsizei n, GLuint *textures )

ECode GLES20::GlGenTextures(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* textures_ref,
    /* [in] */ Int32 offset)
{
    GLuint *textures_base = (GLuint *) 0;
    Int32 _remaining;
    GLuint *textures = (GLuint *) 0;

    if (!textures_ref) {
        SLOGGERD("GLES20", "GlGenTextures: textures == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGenTextures: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = textures_ref->GetLength() - offset;
    if (_remaining < n) {
        SLOGGERD("GLES20", "GlGenTextures: length - offset < n < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    textures_base = (GLuint *) textures_ref->GetPayload();
    textures = textures_base + offset;

    glGenTextures(
        (GLsizei)n,
        (GLuint *)textures
    );
    return NOERROR;
}

// C function void glGenTextures ( GLsizei n, GLuint *textures )

ECode GLES20::GlGenTextures(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* textures_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLuint *textures = (GLuint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(textures_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    textures = (GLuint *)tmp;
    if (_remaining < n) {
        SLOGGERD("GLES20", "GlGenTexturesEx: remaining() < n < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (textures == NULL) {
        char * _texturesBase = reinterpret_cast<char *>(_array);;
        textures = (GLuint *) (_texturesBase + _bufferOffset);
    }
    glGenTextures(
        (GLsizei)n,
        (GLuint *)textures
    );
    return NOERROR;
}

// C function void glGetActiveAttrib ( GLuint program, GLuint index, GLsizei bufsize, GLsizei *length,
// GLint *size, GLenum *type, char *name )

ECode GLES20::GlGetActiveAttrib(
    /* [in] */ Int32 program,
    /* [in] */ Int32 index,
    /* [in] */ Int32 bufsize,
    /* [in] */ ArrayOf<Int32>* length_ref,
    /* [in] */ Int32 lengthOffset,
    /* [in] */ ArrayOf<Int32>* size_ref,
    /* [in] */ Int32 sizeOffset,
    /* [in] */ ArrayOf<Int32>* type_ref,
    /* [in] */ Int32 typeOffset,
    /* [in] */ ArrayOf<Byte>* name_ref,
    /* [in] */ Int32 nameOffset)
{
    GLsizei *length_base = (GLsizei *) 0;
    GLsizei *length = (GLsizei *) 0;
    GLint *size_base = (GLint *) 0;
    GLint *size = (GLint *) 0;
    GLenum *type_base = (GLenum *) 0;
    GLenum *type = (GLenum *) 0;
    char *name_base = (char *) 0;
    char *name = (char *) 0;

    if (!length_ref) {
        SLOGGERD("GLES20", "GlGetActiveAttrib: length == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (lengthOffset < 0) {
        SLOGGERD("GLES20", "GlGetActiveAttrib: lengthOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    length_base = (GLsizei *) length_ref->GetPayload();
    length = length_base + lengthOffset;

    if (!size_ref) {
        SLOGGERD("GLES20", "GlGetActiveAttrib: size == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (sizeOffset < 0) {
        SLOGGERD("GLES20", "GlGetActiveAttrib: sizeOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    size_base = (GLint *) size_ref->GetPayload();
    size = size_base + sizeOffset;

    if (!type_ref) {
        SLOGGERD("GLES20", "GlGetActiveAttrib: type == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (typeOffset < 0) {
        SLOGGERD("GLES20", "GlGetActiveAttrib: typeOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    type_base = (GLenum *) type_ref->GetPayload();
    type = type_base + typeOffset;

    if (!name_ref) {
        SLOGGERD("GLES20", "GlGetActiveAttrib: name == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (nameOffset < 0) {
        SLOGGERD("GLES20", "GlGetActiveAttrib: nameOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    name_base = (char *) name_ref->GetPayload();
    name = name_base + nameOffset;

    glGetActiveAttrib(
        (GLuint)program,
        (GLuint)index,
        (GLsizei)bufsize,
        (GLsizei *)length,
        (GLint *)size,
        (GLenum *)type,
        (char *)name
    );
    return NOERROR;
}

// C function void glGetActiveAttrib ( GLuint program, GLuint index, GLsizei bufsize, GLsizei *length,
// GLint *size, GLenum *type, char *name )

ECode GLES20::GlGetActiveAttrib(
    /* [in] */ Int32 program,
    /* [in] */ Int32 index,
    /* [in] */ Int32 bufsize,
    /* [in] */ IInt32Buffer* length_buf,
    /* [in] */ IInt32Buffer* size_buf,
    /* [in] */ IInt32Buffer* type_buf,
    /* [in] */ Byte name)
{
    Handle64 _lengthArray = (Handle64) 0;
    Int32 _lengthBufferOffset = (Int32) 0;
    Handle64 _sizeArray = (Handle64) 0;
    Int32 _sizeBufferOffset = (Int32) 0;
    Handle64 _typeArray = (Handle64) 0;
    Int32 _typeBufferOffset = (Int32) 0;
    Int32 _lengthRemaining;
    GLsizei *length = (GLsizei *) 0;
    Int32 _sizeRemaining;
    GLint *size = (GLint *) 0;
    Int32 _typeRemaining;
    GLenum *type = (GLenum *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(length_buf), &_lengthArray, &_lengthRemaining, &_lengthBufferOffset, &tmp));
    length = (GLsizei *) tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(size_buf), &_sizeArray, &_sizeRemaining, &_sizeBufferOffset, &tmp));
    size = (GLint *) tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(type_buf), &_typeArray, &_typeRemaining, &_typeBufferOffset, &tmp));
    type = (GLenum *) tmp;
    if (length == NULL) {
        char * _lengthBase = reinterpret_cast<char *>(_lengthArray);
        length = (GLsizei *) (_lengthBase + _lengthBufferOffset);
    }
    if (size == NULL) {
        char * _sizeBase = reinterpret_cast<char *>(_sizeArray);
        size = (GLint *) (_sizeBase + _sizeBufferOffset);
    }
    if (type == NULL) {
        char * _typeBase = reinterpret_cast<char *>(_typeArray);
        type = (GLenum *) (_typeBase + _typeBufferOffset);
    }
    glGetActiveAttrib(
        (GLuint)program,
        (GLuint)index,
        (GLsizei)bufsize,
        (GLsizei *)length,
        (GLint *)size,
        (GLenum *)type,
        reinterpret_cast<char *>(name)
    );
    return NOERROR;
}

// C function void glGetActiveAttrib ( GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, char *name )

ECode GLES20::GlGetActiveAttrib(
    /* [in] */ Int32 program,
    /* [in] */ Int32 index,
    /* [in] */ ArrayOf<Int32>* size_ref,
    /* [in] */ Int32 sizeOffset,
    /* [in] */ ArrayOf<Int32>* type_ref,
    /* [in] */ Int32 typeOffset,
    /* [out] */ String* str)
{
    GLint *size_base = (GLint *) 0;
    GLint *size = (GLint *) 0;
    GLenum *type_base = (GLenum *) 0;
    GLenum *type = (GLenum *) 0;

    String result;

    GLint len = 0;
    glGetProgramiv((GLuint)program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &len);
    if (!len) {
        *str = String("");
        return NOERROR;
    }
    char* buf = (char*) malloc(len);

    if (buf == NULL) {
        *str = String(NULL);
        SLOGGERD("GLES20", "GlGetActiveAttribEx2: out of memory")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (!size_ref) {
        *str = String("");
        SLOGGERD("GLES20", "GlGetActiveAttribEx2: size == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (sizeOffset < 0) {
        *str = String("");
        SLOGGERD("GLES20", "GlGetActiveAttribEx2: sizeOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    size_base = (GLint *)size_ref->GetPayload();
    size = size_base + sizeOffset;

    if (!type_ref) {
        *str = String("");
        SLOGGERD("GLES20", "GlGetActiveAttribEx2: type == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (typeOffset < 0) {
        *str = String("");
        SLOGGERD("GLES20", "GlGetActiveAttribEx2: typeOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    type_base = (GLenum *)type_ref->GetPayload();
    type = type_base + typeOffset;

    glGetActiveAttrib(
        (GLuint)program,
        (GLuint)index,
        (GLsizei)len,
        NULL,
        (GLint *)size,
        (GLenum *)type,
        (char *)buf
    );
    *str = String(buf);
    if (buf) free(buf);
    if (*str == NULL) {
        *str = String("");
    }
    return NOERROR;
}

// C function void glGetActiveAttrib ( GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, char *name )

ECode GLES20::GlGetActiveAttrib(
    /* [in] */ Int32 program,
    /* [in] */ Int32 index,
    /* [in] */ IInt32Buffer* size_buf,
    /* [in] */ IInt32Buffer* type_buf,
    /* [out] */ String* str)
{
    Handle64 _sizeArray = (Handle64) 0;
    Int32 _sizeBufferOffset = (Int32) 0;
    Handle64 _typeArray = (Handle64) 0;
    Int32 _typeBufferOffset = (Int32) 0;
    Int32 _sizeRemaining;
    GLint *size = (GLint *) 0;
    Int32 _typeRemaining;
    GLenum *type = (GLenum *) 0;

    String result;

    GLint len = 0;
    glGetProgramiv((GLuint)program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &len);
    if (!len) {
        *str = String("");
        return NOERROR;
    }
    char* buf = (char*) malloc(len);

    if (buf == NULL) {
        *str = String(NULL);
        SLOGGERD("GLES20", "GlGetActiveAttribEx3: out of memory")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(size_buf), &_sizeArray, &_sizeRemaining, &_sizeBufferOffset, &tmp));
    size = (GLint *)tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(type_buf), &_typeArray, &_typeRemaining, &_typeBufferOffset, &tmp));
    type = (GLenum *)tmp;
    if (size == NULL) {
        char * _sizeBase = reinterpret_cast<char *>(_sizeArray);
        size = (GLint *) (_sizeBase + _sizeBufferOffset);
    }
    if (type == NULL) {
        char * _typeBase = reinterpret_cast<char *>(_typeArray);
        type = (GLenum *) (_typeBase + _typeBufferOffset);
    }
    glGetActiveAttrib(
        (GLuint)program,
        (GLuint)index,
        (GLsizei)len,
        NULL,
        (GLint *)size,
        (GLenum *)type,
        (char *)buf
    );
    *str = String(buf);
    if (buf) free(buf);
    return NOERROR;
}
// C function void glGetActiveUniform ( GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, char *name )

ECode GLES20::GlGetActiveUniform(
    /* [in] */ Int32 program,
    /* [in] */ Int32 index,
    /* [in] */ Int32 bufsize,
    /* [in] */ ArrayOf<Int32>* length_ref,
    /* [in] */ Int32 lengthOffset,
    /* [in] */ ArrayOf<Int32>* size_ref,
    /* [in] */ Int32 sizeOffset,
    /* [in] */ ArrayOf<Int32>* type_ref,
    /* [in] */ Int32 typeOffset,
    /* [in] */ ArrayOf<Byte>* name_ref,
    /* [in] */ Int32 nameOffset)
{
    GLsizei *length_base = (GLsizei *) 0;
    GLsizei *length = (GLsizei *) 0;
    GLint *size_base = (GLint *) 0;
    GLint *size = (GLint *) 0;
    GLenum *type_base = (GLenum *) 0;
    GLenum *type = (GLenum *) 0;
    char *name_base = (char *) 0;
    char *name = (char *) 0;

    if (!length_ref) {
        SLOGGERD("GLES20", "GlGetActiveUniform: length == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (lengthOffset < 0) {
        SLOGGERD("GLES20", "GlGetActiveUniform: lengthOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    length_base = (GLsizei *) length_ref->GetPayload();
    length = length_base + lengthOffset;

    if (!size_ref) {
        SLOGGERD("GLES20", "GlGetActiveUniform: size == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (sizeOffset < 0) {
        SLOGGERD("GLES20", "GlGetActiveUniform: sizeOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    size_base = (GLint *) size_ref->GetPayload();
    size = size_base + sizeOffset;

    if (!type_ref) {
        SLOGGERD("GLES20", "GlGetActiveUniform: type == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (typeOffset < 0) {
        SLOGGERD("GLES20", "GlGetActiveUniform: typeOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    type_base = (GLenum *) type_ref->GetPayload();
    type = type_base + typeOffset;

    if (!name_ref) {
        SLOGGERD("GLES20", "GlGetActiveUniform: name == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (nameOffset < 0) {
        SLOGGERD("GLES20", "GlGetActiveUniform: nameOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    name_base = (char *) name_ref->GetPayload();
    name = name_base + nameOffset;

    glGetActiveUniform(
        (GLuint)program,
        (GLuint)index,
        (GLsizei)bufsize,
        (GLsizei *)length,
        (GLint *)size,
        (GLenum *)type,
        (char *)name
    );
    return NOERROR;
}

// C function void glGetActiveUniform ( GLuint program, GLuint index, GLsizei bufsize, GLsizei *length,
// GLint *size, GLenum *type, char *name )

ECode GLES20::GlGetActiveUniform(
    /* [in] */ Int32 program,
    /* [in] */ Int32 index,
    /* [in] */ Int32 bufsize,
    /* [in] */ IInt32Buffer* length_buf,
    /* [in] */ IInt32Buffer* size_buf,
    /* [in] */ IInt32Buffer* type_buf,
    /* [in] */ Byte name)
{
    Handle64 _lengthArray = (Handle64) 0;
    Int32 _lengthBufferOffset = (Int32) 0;
    Handle64 _sizeArray = (Handle64) 0;
    Int32 _sizeBufferOffset = (Int32) 0;
    Handle64 _typeArray = (Handle64) 0;
    Int32 _typeBufferOffset = (Int32) 0;
    Int32 _lengthRemaining;
    GLsizei *length = (GLsizei *) 0;
    Int32 _sizeRemaining;
    GLint *size = (GLint *) 0;
    Int32 _typeRemaining;
    GLenum *type = (GLenum *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(length_buf), &_lengthArray, &_lengthRemaining, &_lengthBufferOffset, &tmp));

    length = (GLsizei *) tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(size_buf), &_sizeArray, &_sizeRemaining, &_sizeBufferOffset, &tmp));
    size = (GLint *) tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(type_buf), &_typeArray, &_typeRemaining, &_typeBufferOffset, &tmp));
    type = (GLenum *) tmp;
    if (length == NULL) {
        char * _lengthBase = reinterpret_cast<char *>(_lengthArray);
        length = (GLsizei *) (_lengthBase + _lengthBufferOffset);
    }
    if (size == NULL) {
        char * _sizeBase = reinterpret_cast<char *>(_sizeArray);
        size = (GLint *) (_sizeBase + _sizeBufferOffset);
    }
    if (type == NULL) {
        char * _typeBase = reinterpret_cast<char *>(_typeArray);
        type = (GLenum *) (_typeBase + _typeBufferOffset);
    }
    glGetActiveUniform(
        (GLuint)program,
        (GLuint)index,
        (GLsizei)bufsize,
        (GLsizei *)length,
        (GLint *)size,
        (GLenum *)type,
        reinterpret_cast<char *>(name)
    );

    return NOERROR;
}
// C function void glGetActiveUniform ( GLuint program, GLuint index, GLsizei bufsize, GLsizei *length,
// GLint *size, GLenum *type, char *name )

ECode GLES20::GlGetActiveUniform(
    /* [in] */ Int32 program,
    /* [in] */ Int32 index,
    /* [in] */ ArrayOf<Int32>* size_ref,
    /* [in] */ Int32 sizeOffset,
    /* [in] */ ArrayOf<Int32>* type_ref,
    /* [in] */ Int32 typeOffset,
    /* [out] */ String* rst)
{
    GLint *size_base = (GLint *) 0;
    GLint *size = (GLint *) 0;

    GLenum *type_base = (GLenum *) 0;
    GLenum *type = (GLenum *) 0;

    String result;

    GLint len = 0;
    glGetProgramiv((GLuint)program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &len);
    if (!len) {
        *rst = String("");
        return NOERROR;
    }
    char* buf = (char*) malloc(len);

    if (buf == NULL) {
        *rst = String(NULL);
        SLOGGERD("GLES20", "GlGetActiveUniformEx2: out of memory")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (!size_ref) {
        *rst = String("");
        SLOGGERD("GLES20", "GlGetActiveUniformEx2: size == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (sizeOffset < 0) {
        *rst = String("");
        SLOGGERD("GLES20", "GlGetActiveUniformEx2: sizeOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    size_base = (GLint *) size_ref->GetPayload();
    size = size_base + sizeOffset;

    if (!type_ref) {
        *rst = String("");
        SLOGGERD("GLES20", "GlGetActiveUniformEx2: type == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (typeOffset < 0) {
        *rst = String("");
        SLOGGERD("GLES20", "GlGetActiveUniformEx2: typeOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    type_base = (GLenum *) type_ref->GetPayload();
    type = type_base + typeOffset;

    glGetActiveUniform(
        (GLuint)program,
        (GLuint)index,
        (GLsizei)len,
        NULL,
        (GLint *)size,
        (GLenum *)type,
        (char *)buf
    );
    return NOERROR;
}

// C function void glGetActiveUniform ( GLuint program, GLuint index, GLsizei bufsize, GLsizei *length,
// GLint *size, GLenum *type, char *name )

ECode GLES20::GlGetActiveUniform(
    /* [in] */ Int32 program,
    /* [in] */ Int32 index,
    /* [in] */ IInt32Buffer* size_buf,
    /* [in] */ IInt32Buffer* type_buf,
    /* [out] */ String* str)
{
    Handle64 _sizeArray = (Handle64) 0;
    Int32 _sizeBufferOffset = (Int32) 0;
    Handle64 _typeArray = (Handle64) 0;
    Int32 _typeBufferOffset = (Int32) 0;
    Int32 _sizeRemaining;
    GLint *size = (GLint *) 0;
    Int32 _typeRemaining;
    GLenum *type = (GLenum *) 0;

    String result(NULL);
    GLint len = 0;
    glGetProgramiv((GLuint)program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &len);
    if (!len) {
        *str = String("");
        return NOERROR;
    }
    char* buf = (char*) malloc(len);

    if (buf == NULL) {
        SLOGGERD("GLES20", "GlGetActiveUniformEx3: out of memory")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(size_buf), &_sizeArray, &_sizeRemaining, &_sizeBufferOffset, &tmp));
    size = (GLint *) tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(type_buf), &_typeArray, &_typeRemaining, &_typeBufferOffset, &tmp));
    type = (GLenum *) tmp;

    if (size == NULL) {
        char * _sizeBase = reinterpret_cast<char *>(_sizeArray);
        size = (GLint *) (_sizeBase + _sizeBufferOffset);
    }
    if (type == NULL) {
        char * _typeBase = reinterpret_cast<char *>(_typeArray);
        type = (GLenum *) (_typeBase + _typeBufferOffset);
    }
    glGetActiveUniform(
        (GLuint)program,
        (GLuint)index,
        len,
        NULL,
        (GLint *)size,
        (GLenum *)type,
        (char *)buf
    );

    *str = String(buf);
    if (buf) free(buf);
    return NOERROR;
}
// C function void glGetAttachedShaders ( GLuint program, GLsizei maxcount, GLsizei *count, GLuint *shaders )

ECode GLES20::GlGetAttachedShaders(
    /* [in] */ Int32 program,
    /* [in] */ Int32 maxcount,
    /* [in] */ ArrayOf<Int32>* count_ref,
    /* [in] */ Int32 countOffset,
    /* [in] */ ArrayOf<Int32>* shaders_ref,
    /* [in] */ Int32 shadersOffset)
{
    GLsizei *count_base = (GLsizei *) 0;
    GLsizei *count = (GLsizei *) 0;
    GLuint *shaders_base = (GLuint *) 0;
    GLuint *shaders = (GLuint *) 0;

    if (!count_ref) {
        SLOGGERD("GLES20", "GlGetAttachedShaders: count == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (countOffset < 0) {
        SLOGGERD("GLES20", "GlGetAttachedShaders: countOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    count_base = (GLsizei *) count_ref->GetPayload();
    count = count_base + countOffset;

    if (!shaders_ref) {
        SLOGGERD("GLES20", "GlGetAttachedShaders: shaders == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (shadersOffset < 0) {
        SLOGGERD("GLES20", "GlGetAttachedShaders: shadersOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    shaders_base = (GLuint *) shaders_ref->GetPayload();
    shaders = shaders_base + shadersOffset;

    glGetAttachedShaders(
        (GLuint)program,
        (GLsizei)maxcount,
        (GLsizei *)count,
        (GLuint *)shaders
    );
    return NOERROR;
}

// C function void glGetAttachedShaders ( GLuint program, GLsizei maxcount, GLsizei *count, GLuint *shaders )

ECode GLES20::GlGetAttachedShaders(
    /* [in] */ Int32 program,
    /* [in] */ Int32 maxcount,
    /* [in] */ IInt32Buffer* count_buf,
    /* [in] */ IInt32Buffer* shaders_buf)
{
    Handle64 _countArray = (Handle64) 0;
    Int32 _countBufferOffset = (Int32) 0;
    Handle64 _shadersArray = (Handle64) 0;
    Int32 _shadersBufferOffset = (Int32) 0;
    Int32 _countRemaining;
    GLsizei *count = (GLsizei *) 0;
    Int32 _shadersRemaining;
    GLuint *shaders = (GLuint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(count_buf), &_countArray, &_countRemaining, &_countBufferOffset, &tmp));
    count = (GLsizei *) tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(shaders_buf), &_shadersArray, &_shadersRemaining, &_shadersBufferOffset, &tmp));
    shaders = (GLuint *) tmp;
    if (count == NULL) {
        char * _countBase = reinterpret_cast<char *>(_countArray);
        count = (GLsizei *) (_countBase + _countBufferOffset);
    }
    if (shaders == NULL) {
        char * _shadersBase = reinterpret_cast<char *>(_shadersArray);
        shaders = (GLuint *) (_shadersBase + _shadersBufferOffset);
    }
    glGetAttachedShaders(
        (GLuint)program,
        (GLsizei)maxcount,
        (GLsizei *)count,
        (GLuint *)shaders
    );
    return NOERROR;
}

// C function int glGetAttribLocation ( GLuint program, const char *name )

ECode GLES20::GlGetAttribLocation(
    /* [in] */ Int32 program,
    /* [in] */ const String& name,
    /* [out] */ Int32* rst)
{
    Int32 _returnValue = 0;
    const char* _nativename = 0;

    if (!name) {
        SLOGGERD("GLES20", "GlGetAttribLocation:   name == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _nativename = name.string();

    _returnValue = glGetAttribLocation(
        (GLuint)program,
        (char *)_nativename
    );

    *rst = _returnValue;
    return NOERROR;
}

// C function void glGetBooleanv ( GLenum pname, GLboolean *params )

ECode GLES20::GlGetBooleanv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Boolean>* params_ref,
    /* [in] */ Int32 offset)
{
    GLboolean *params_base = (GLboolean *) 0;
    GLboolean *params = (GLboolean *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlGetBooleanv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGetBooleanv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLboolean *) params_ref->GetPayload();
    params = params_base + offset;

    glGetBooleanv(
        (GLenum)pname,
        (GLboolean *)params
    );
    return NOERROR;
}

// C function void glGetBooleanv ( GLenum pname, GLboolean *params )

ECode GLES20::GlGetBooleanv(
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLboolean *params = (GLboolean *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params =(GLboolean *) tmp;
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);;
        params = (GLboolean *) (_paramsBase + _bufferOffset);
    }
    glGetBooleanv(
        (GLenum)pname,
        (GLboolean *)params
    );
    return NOERROR;
}

// C function void glGetBufferParameteriv ( GLenum target, GLenum pname, GLint *params )

ECode GLES20::GlGetBufferParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlGetBufferParameteriv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGetBufferParameteriv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    if (_remaining < 1) {
        SLOGGERD("GLES20", "GlGetBufferParameteriv: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLint *) params_ref->GetPayload();
    params = params_base + offset;

    glGetBufferParameteriv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

// C function void glGetBufferParameteriv ( GLenum target, GLenum pname, GLint *params )

ECode GLES20::GlGetBufferParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params = (GLint *) tmp;
    if (_remaining < 1) {
        SLOGGERD("GLES20", "GlGetBufferParameterivEx: remaining() < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLint *) (_paramsBase + _bufferOffset);
    }
    glGetBufferParameteriv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

// C function GLenum glGetError ( void )

ECode GLES20::GlGetError(
    /* [out] */ Int32* rst)
{
    GLenum _returnValue;
    _returnValue = glGetError();
    *rst = _returnValue;
    return NOERROR;
}

// C function void glGetFloatv ( GLenum pname, GLfloat *params )

ECode GLES20::GlGetFloatv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlGetFloatv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGetFloatv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glGetFloatv(
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

// C function void glGetFloatv ( GLenum pname, GLfloat *params )

ECode GLES20::GlGetFloatv(
    /* [in] */ Int32 pname,
    /* [in] */ IFloatBuffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params = (GLfloat *) tmp;
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glGetFloatv(
        (GLenum)pname,
        (GLfloat *)params
    );

    return NOERROR;
}

// C function void glGetFramebufferAttachmentParameteriv ( GLenum target, GLenum attachment,
// GLenum pname, GLint *params )

ECode GLES20::GlGetFramebufferAttachmentParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 attachment,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlGetFramebufferAttachmentParameteriv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGetFramebufferAttachmentParameteriv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLint *) params_ref->GetPayload();
    params = params_base + offset;

    glGetFramebufferAttachmentParameteriv(
        (GLenum)target,
        (GLenum)attachment,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

// C function void glGetFramebufferAttachmentParameteriv ( GLenum target, GLenum attachment, GLenum pname,
// GLint *params )

ECode GLES20::GlGetFramebufferAttachmentParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 attachment,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params = (GLint *) tmp;
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);;
        params = (GLint *) (_paramsBase + _bufferOffset);
    }
    glGetFramebufferAttachmentParameteriv(
        (GLenum)target,
        (GLenum)attachment,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

// C function void glGetIntegerv ( GLenum pname, GLint *params )

ECode GLES20::GlGetIntegerv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlGetIntegerv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGetIntegerv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    Int32 _needed;
    switch (pname) {
#if defined(GL_ALPHA_BITS)
        case GL_ALPHA_BITS:
#endif // defined(GL_ALPHA_BITS)
#if defined(GL_ALPHA_TEST_FUNC)
        case GL_ALPHA_TEST_FUNC:
#endif // defined(GL_ALPHA_TEST_FUNC)
#if defined(GL_ALPHA_TEST_REF)
        case GL_ALPHA_TEST_REF:
#endif // defined(GL_ALPHA_TEST_REF)
#if defined(GL_BLEND_DST)
        case GL_BLEND_DST:
#endif // defined(GL_BLEND_DST)
#if defined(GL_BLUE_BITS)
        case GL_BLUE_BITS:
#endif // defined(GL_BLUE_BITS)
#if defined(GL_COLOR_ARRAY_BUFFER_BINDING)
        case GL_COLOR_ARRAY_BUFFER_BINDING:
#endif // defined(GL_COLOR_ARRAY_BUFFER_BINDING)
#if defined(GL_COLOR_ARRAY_SIZE)
        case GL_COLOR_ARRAY_SIZE:
#endif // defined(GL_COLOR_ARRAY_SIZE)
#if defined(GL_COLOR_ARRAY_STRIDE)
        case GL_COLOR_ARRAY_STRIDE:
#endif // defined(GL_COLOR_ARRAY_STRIDE)
#if defined(GL_COLOR_ARRAY_TYPE)
        case GL_COLOR_ARRAY_TYPE:
#endif // defined(GL_COLOR_ARRAY_TYPE)
#if defined(GL_CULL_FACE)
        case GL_CULL_FACE:
#endif // defined(GL_CULL_FACE)
#if defined(GL_DEPTH_BITS)
        case GL_DEPTH_BITS:
#endif // defined(GL_DEPTH_BITS)
#if defined(GL_DEPTH_CLEAR_VALUE)
        case GL_DEPTH_CLEAR_VALUE:
#endif // defined(GL_DEPTH_CLEAR_VALUE)
#if defined(GL_DEPTH_FUNC)
        case GL_DEPTH_FUNC:
#endif // defined(GL_DEPTH_FUNC)
#if defined(GL_DEPTH_WRITEMASK)
        case GL_DEPTH_WRITEMASK:
#endif // defined(GL_DEPTH_WRITEMASK)
#if defined(GL_FOG_DENSITY)
        case GL_FOG_DENSITY:
#endif // defined(GL_FOG_DENSITY)
#if defined(GL_FOG_END)
        case GL_FOG_END:
#endif // defined(GL_FOG_END)
#if defined(GL_FOG_MODE)
        case GL_FOG_MODE:
#endif // defined(GL_FOG_MODE)
#if defined(GL_FOG_START)
        case GL_FOG_START:
#endif // defined(GL_FOG_START)
#if defined(GL_FRONT_FACE)
        case GL_FRONT_FACE:
#endif // defined(GL_FRONT_FACE)
#if defined(GL_GREEN_BITS)
        case GL_GREEN_BITS:
#endif // defined(GL_GREEN_BITS)
#if defined(GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES)
        case GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES:
#endif // defined(GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES)
#if defined(GL_IMPLEMENTATION_COLOR_READ_TYPE_OES)
        case GL_IMPLEMENTATION_COLOR_READ_TYPE_OES:
#endif // defined(GL_IMPLEMENTATION_COLOR_READ_TYPE_OES)
#if defined(GL_LIGHT_MODEL_COLOR_CONTROL)
        case GL_LIGHT_MODEL_COLOR_CONTROL:
#endif // defined(GL_LIGHT_MODEL_COLOR_CONTROL)
#if defined(GL_LIGHT_MODEL_LOCAL_VIEWER)
        case GL_LIGHT_MODEL_LOCAL_VIEWER:
#endif // defined(GL_LIGHT_MODEL_LOCAL_VIEWER)
#if defined(GL_LIGHT_MODEL_TWO_SIDE)
        case GL_LIGHT_MODEL_TWO_SIDE:
#endif // defined(GL_LIGHT_MODEL_TWO_SIDE)
#if defined(GL_LINE_SMOOTH_HINT)
        case GL_LINE_SMOOTH_HINT:
#endif // defined(GL_LINE_SMOOTH_HINT)
#if defined(GL_LINE_WIDTH)
        case GL_LINE_WIDTH:
#endif // defined(GL_LINE_WIDTH)
#if defined(GL_LOGIC_OP_MODE)
        case GL_LOGIC_OP_MODE:
#endif // defined(GL_LOGIC_OP_MODE)
#if defined(GL_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES)
        case GL_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES:
#endif // defined(GL_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES)
#if defined(GL_MATRIX_INDEX_ARRAY_SIZE_OES)
        case GL_MATRIX_INDEX_ARRAY_SIZE_OES:
#endif // defined(GL_MATRIX_INDEX_ARRAY_SIZE_OES)
#if defined(GL_MATRIX_INDEX_ARRAY_STRIDE_OES)
        case GL_MATRIX_INDEX_ARRAY_STRIDE_OES:
#endif // defined(GL_MATRIX_INDEX_ARRAY_STRIDE_OES)
#if defined(GL_MATRIX_INDEX_ARRAY_TYPE_OES)
        case GL_MATRIX_INDEX_ARRAY_TYPE_OES:
#endif // defined(GL_MATRIX_INDEX_ARRAY_TYPE_OES)
#if defined(GL_MATRIX_MODE)
        case GL_MATRIX_MODE:
#endif // defined(GL_MATRIX_MODE)
#if defined(GL_MAX_CLIP_PLANES)
        case GL_MAX_CLIP_PLANES:
#endif // defined(GL_MAX_CLIP_PLANES)
#if defined(GL_MAX_ELEMENTS_INDICES)
        case GL_MAX_ELEMENTS_INDICES:
#endif // defined(GL_MAX_ELEMENTS_INDICES)
#if defined(GL_MAX_ELEMENTS_VERTICES)
        case GL_MAX_ELEMENTS_VERTICES:
#endif // defined(GL_MAX_ELEMENTS_VERTICES)
#if defined(GL_MAX_LIGHTS)
        case GL_MAX_LIGHTS:
#endif // defined(GL_MAX_LIGHTS)
#if defined(GL_MAX_MODELVIEW_STACK_DEPTH)
        case GL_MAX_MODELVIEW_STACK_DEPTH:
#endif // defined(GL_MAX_MODELVIEW_STACK_DEPTH)
#if defined(GL_MAX_PALETTE_MATRICES_OES)
        case GL_MAX_PALETTE_MATRICES_OES:
#endif // defined(GL_MAX_PALETTE_MATRICES_OES)
#if defined(GL_MAX_PROJECTION_STACK_DEPTH)
        case GL_MAX_PROJECTION_STACK_DEPTH:
#endif // defined(GL_MAX_PROJECTION_STACK_DEPTH)
#if defined(GL_MAX_TEXTURE_SIZE)
        case GL_MAX_TEXTURE_SIZE:
#endif // defined(GL_MAX_TEXTURE_SIZE)
#if defined(GL_MAX_TEXTURE_STACK_DEPTH)
        case GL_MAX_TEXTURE_STACK_DEPTH:
#endif // defined(GL_MAX_TEXTURE_STACK_DEPTH)
#if defined(GL_MAX_TEXTURE_UNITS)
        case GL_MAX_TEXTURE_UNITS:
#endif // defined(GL_MAX_TEXTURE_UNITS)
#if defined(GL_MAX_VERTEX_UNITS_OES)
        case GL_MAX_VERTEX_UNITS_OES:
#endif // defined(GL_MAX_VERTEX_UNITS_OES)
#if defined(GL_MODELVIEW_STACK_DEPTH)
        case GL_MODELVIEW_STACK_DEPTH:
#endif // defined(GL_MODELVIEW_STACK_DEPTH)
#if defined(GL_NORMAL_ARRAY_BUFFER_BINDING)
        case GL_NORMAL_ARRAY_BUFFER_BINDING:
#endif // defined(GL_NORMAL_ARRAY_BUFFER_BINDING)
#if defined(GL_NORMAL_ARRAY_STRIDE)
        case GL_NORMAL_ARRAY_STRIDE:
#endif // defined(GL_NORMAL_ARRAY_STRIDE)
#if defined(GL_NORMAL_ARRAY_TYPE)
        case GL_NORMAL_ARRAY_TYPE:
#endif // defined(GL_NORMAL_ARRAY_TYPE)
#if defined(GL_NUM_COMPRESSED_TEXTURE_FORMATS)
        case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
#endif // defined(GL_NUM_COMPRESSED_TEXTURE_FORMATS)
#if defined(GL_PACK_ALIGNMENT)
        case GL_PACK_ALIGNMENT:
#endif // defined(GL_PACK_ALIGNMENT)
#if defined(GL_PERSPECTIVE_CORRECTION_HINT)
        case GL_PERSPECTIVE_CORRECTION_HINT:
#endif // defined(GL_PERSPECTIVE_CORRECTION_HINT)
#if defined(GL_POINT_SIZE)
        case GL_POINT_SIZE:
#endif // defined(GL_POINT_SIZE)
#if defined(GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES)
        case GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES:
#endif // defined(GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES)
#if defined(GL_POINT_SIZE_ARRAY_STRIDE_OES)
        case GL_POINT_SIZE_ARRAY_STRIDE_OES:
#endif // defined(GL_POINT_SIZE_ARRAY_STRIDE_OES)
#if defined(GL_POINT_SIZE_ARRAY_TYPE_OES)
        case GL_POINT_SIZE_ARRAY_TYPE_OES:
#endif // defined(GL_POINT_SIZE_ARRAY_TYPE_OES)
#if defined(GL_POINT_SMOOTH_HINT)
        case GL_POINT_SMOOTH_HINT:
#endif // defined(GL_POINT_SMOOTH_HINT)
#if defined(GL_POLYGON_OFFSET_FACTOR)
        case GL_POLYGON_OFFSET_FACTOR:
#endif // defined(GL_POLYGON_OFFSET_FACTOR)
#if defined(GL_POLYGON_OFFSET_UNITS)
        case GL_POLYGON_OFFSET_UNITS:
#endif // defined(GL_POLYGON_OFFSET_UNITS)
#if defined(GL_PROJECTION_STACK_DEPTH)
        case GL_PROJECTION_STACK_DEPTH:
#endif // defined(GL_PROJECTION_STACK_DEPTH)
#if defined(GL_RED_BITS)
        case GL_RED_BITS:
#endif // defined(GL_RED_BITS)
#if defined(GL_SHADE_MODEL)
        case GL_SHADE_MODEL:
#endif // defined(GL_SHADE_MODEL)
#if defined(GL_STENCIL_BITS)
        case GL_STENCIL_BITS:
#endif // defined(GL_STENCIL_BITS)
#if defined(GL_STENCIL_CLEAR_VALUE)
        case GL_STENCIL_CLEAR_VALUE:
#endif // defined(GL_STENCIL_CLEAR_VALUE)
#if defined(GL_STENCIL_FAIL)
        case GL_STENCIL_FAIL:
#endif // defined(GL_STENCIL_FAIL)
#if defined(GL_STENCIL_FUNC)
        case GL_STENCIL_FUNC:
#endif // defined(GL_STENCIL_FUNC)
#if defined(GL_STENCIL_PASS_DEPTH_FAIL)
        case GL_STENCIL_PASS_DEPTH_FAIL:
#endif // defined(GL_STENCIL_PASS_DEPTH_FAIL)
#if defined(GL_STENCIL_PASS_DEPTH_PASS)
        case GL_STENCIL_PASS_DEPTH_PASS:
#endif // defined(GL_STENCIL_PASS_DEPTH_PASS)
#if defined(GL_STENCIL_REF)
        case GL_STENCIL_REF:
#endif // defined(GL_STENCIL_REF)
#if defined(GL_STENCIL_VALUE_MASK)
        case GL_STENCIL_VALUE_MASK:
#endif // defined(GL_STENCIL_VALUE_MASK)
#if defined(GL_STENCIL_WRITEMASK)
        case GL_STENCIL_WRITEMASK:
#endif // defined(GL_STENCIL_WRITEMASK)
#if defined(GL_SUBPIXEL_BITS)
        case GL_SUBPIXEL_BITS:
#endif // defined(GL_SUBPIXEL_BITS)
#if defined(GL_TEXTURE_BINDING_2D)
        case GL_TEXTURE_BINDING_2D:
#endif // defined(GL_TEXTURE_BINDING_2D)
#if defined(GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING)
        case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
#endif // defined(GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING)
#if defined(GL_TEXTURE_COORD_ARRAY_SIZE)
        case GL_TEXTURE_COORD_ARRAY_SIZE:
#endif // defined(GL_TEXTURE_COORD_ARRAY_SIZE)
#if defined(GL_TEXTURE_COORD_ARRAY_STRIDE)
        case GL_TEXTURE_COORD_ARRAY_STRIDE:
#endif // defined(GL_TEXTURE_COORD_ARRAY_STRIDE)
#if defined(GL_TEXTURE_COORD_ARRAY_TYPE)
        case GL_TEXTURE_COORD_ARRAY_TYPE:
#endif // defined(GL_TEXTURE_COORD_ARRAY_TYPE)
#if defined(GL_TEXTURE_STACK_DEPTH)
        case GL_TEXTURE_STACK_DEPTH:
#endif // defined(GL_TEXTURE_STACK_DEPTH)
#if defined(GL_UNPACK_ALIGNMENT)
        case GL_UNPACK_ALIGNMENT:
#endif // defined(GL_UNPACK_ALIGNMENT)
#if defined(GL_VERTEX_ARRAY_BUFFER_BINDING)
        case GL_VERTEX_ARRAY_BUFFER_BINDING:
#endif // defined(GL_VERTEX_ARRAY_BUFFER_BINDING)
#if defined(GL_VERTEX_ARRAY_SIZE)
        case GL_VERTEX_ARRAY_SIZE:
#endif // defined(GL_VERTEX_ARRAY_SIZE)
#if defined(GL_VERTEX_ARRAY_STRIDE)
        case GL_VERTEX_ARRAY_STRIDE:
#endif // defined(GL_VERTEX_ARRAY_STRIDE)
#if defined(GL_VERTEX_ARRAY_TYPE)
        case GL_VERTEX_ARRAY_TYPE:
#endif // defined(GL_VERTEX_ARRAY_TYPE)
#if defined(GL_WEIGHT_ARRAY_BUFFER_BINDING_OES)
        case GL_WEIGHT_ARRAY_BUFFER_BINDING_OES:
#endif // defined(GL_WEIGHT_ARRAY_BUFFER_BINDING_OES)
#if defined(GL_WEIGHT_ARRAY_SIZE_OES)
        case GL_WEIGHT_ARRAY_SIZE_OES:
#endif // defined(GL_WEIGHT_ARRAY_SIZE_OES)
#if defined(GL_WEIGHT_ARRAY_STRIDE_OES)
        case GL_WEIGHT_ARRAY_STRIDE_OES:
#endif // defined(GL_WEIGHT_ARRAY_STRIDE_OES)
#if defined(GL_WEIGHT_ARRAY_TYPE_OES)
        case GL_WEIGHT_ARRAY_TYPE_OES:
#endif // defined(GL_WEIGHT_ARRAY_TYPE_OES)
            _needed = 1;
            break;
#if defined(GL_ALIASED_POINT_SIZE_RANGE)
        case GL_ALIASED_POINT_SIZE_RANGE:
#endif // defined(GL_ALIASED_POINT_SIZE_RANGE)
#if defined(GL_ALIASED_LINE_WIDTH_RANGE)
        case GL_ALIASED_LINE_WIDTH_RANGE:
#endif // defined(GL_ALIASED_LINE_WIDTH_RANGE)
#if defined(GL_DEPTH_RANGE)
        case GL_DEPTH_RANGE:
#endif // defined(GL_DEPTH_RANGE)
#if defined(GL_MAX_VIEWPORT_DIMS)
        case GL_MAX_VIEWPORT_DIMS:
#endif // defined(GL_MAX_VIEWPORT_DIMS)
#if defined(GL_SMOOTH_LINE_WIDTH_RANGE)
        case GL_SMOOTH_LINE_WIDTH_RANGE:
#endif // defined(GL_SMOOTH_LINE_WIDTH_RANGE)
#if defined(GL_SMOOTH_POINT_SIZE_RANGE)
        case GL_SMOOTH_POINT_SIZE_RANGE:
#endif // defined(GL_SMOOTH_POINT_SIZE_RANGE)
            _needed = 2;
            break;
#if defined(GL_COLOR_CLEAR_VALUE)
        case GL_COLOR_CLEAR_VALUE:
#endif // defined(GL_COLOR_CLEAR_VALUE)
#if defined(GL_COLOR_WRITEMASK)
        case GL_COLOR_WRITEMASK:
#endif // defined(GL_COLOR_WRITEMASK)
#if defined(GL_FOG_COLOR)
        case GL_FOG_COLOR:
#endif // defined(GL_FOG_COLOR)
#if defined(GL_LIGHT_MODEL_AMBIENT)
        case GL_LIGHT_MODEL_AMBIENT:
#endif // defined(GL_LIGHT_MODEL_AMBIENT)
#if defined(GL_SCISSOR_BOX)
        case GL_SCISSOR_BOX:
#endif // defined(GL_SCISSOR_BOX)
#if defined(GL_VIEWPORT)
        case GL_VIEWPORT:
#endif // defined(GL_VIEWPORT)
            _needed = 4;
            break;
#if defined(GL_MODELVIEW_MATRIX)
        case GL_MODELVIEW_MATRIX:
#endif // defined(GL_MODELVIEW_MATRIX)
#if defined(GL_MODELVIEW_MATRIX_FLOAT_AS_INT_BITS_OES)
        case GL_MODELVIEW_MATRIX_FLOAT_AS_INT_BITS_OES:
#endif // defined(GL_MODELVIEW_MATRIX_FLOAT_AS_INT_BITS_OES)
#if defined(GL_PROJECTION_MATRIX)
        case GL_PROJECTION_MATRIX:
#endif // defined(GL_PROJECTION_MATRIX)
#if defined(GL_PROJECTION_MATRIX_FLOAT_AS_INT_BITS_OES)
        case GL_PROJECTION_MATRIX_FLOAT_AS_INT_BITS_OES:
#endif // defined(GL_PROJECTION_MATRIX_FLOAT_AS_INT_BITS_OES)
#if defined(GL_TEXTURE_MATRIX)
        case GL_TEXTURE_MATRIX:
#endif // defined(GL_TEXTURE_MATRIX)
#if defined(GL_TEXTURE_MATRIX_FLOAT_AS_INT_BITS_OES)
        case GL_TEXTURE_MATRIX_FLOAT_AS_INT_BITS_OES:
#endif // defined(GL_TEXTURE_MATRIX_FLOAT_AS_INT_BITS_OES)
            _needed = 16;
            break;
#if defined(GL_COMPRESSED_TEXTURE_FORMATS)
        case GL_COMPRESSED_TEXTURE_FORMATS:
#endif // defined(GL_COMPRESSED_TEXTURE_FORMATS)
            _needed = getNumCompressedTextureFormats();
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        SLOGGERD("GLES20", "GlGetIntegerv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLint *) params_ref->GetPayload();
    params = params_base + offset;

    glGetIntegerv(
        (GLenum)pname,
        (GLint *)params
    );

    return NOERROR;
}

// C function void glGetIntegerv ( GLenum pname, GLint *params )

ECode GLES20::GlGetIntegerv(
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params = (GLint *) tmp;
    Int32 _needed;
    switch (pname) {
#if defined(GL_ALPHA_BITS)
        case GL_ALPHA_BITS:
#endif // defined(GL_ALPHA_BITS)
#if defined(GL_ALPHA_TEST_FUNC)
        case GL_ALPHA_TEST_FUNC:
#endif // defined(GL_ALPHA_TEST_FUNC)
#if defined(GL_ALPHA_TEST_REF)
        case GL_ALPHA_TEST_REF:
#endif // defined(GL_ALPHA_TEST_REF)
#if defined(GL_BLEND_DST)
        case GL_BLEND_DST:
#endif // defined(GL_BLEND_DST)
#if defined(GL_BLUE_BITS)
        case GL_BLUE_BITS:
#endif // defined(GL_BLUE_BITS)
#if defined(GL_COLOR_ARRAY_BUFFER_BINDING)
        case GL_COLOR_ARRAY_BUFFER_BINDING:
#endif // defined(GL_COLOR_ARRAY_BUFFER_BINDING)
#if defined(GL_COLOR_ARRAY_SIZE)
        case GL_COLOR_ARRAY_SIZE:
#endif // defined(GL_COLOR_ARRAY_SIZE)
#if defined(GL_COLOR_ARRAY_STRIDE)
        case GL_COLOR_ARRAY_STRIDE:
#endif // defined(GL_COLOR_ARRAY_STRIDE)
#if defined(GL_COLOR_ARRAY_TYPE)
        case GL_COLOR_ARRAY_TYPE:
#endif // defined(GL_COLOR_ARRAY_TYPE)
#if defined(GL_CULL_FACE)
        case GL_CULL_FACE:
#endif // defined(GL_CULL_FACE)
#if defined(GL_DEPTH_BITS)
        case GL_DEPTH_BITS:
#endif // defined(GL_DEPTH_BITS)
#if defined(GL_DEPTH_CLEAR_VALUE)
        case GL_DEPTH_CLEAR_VALUE:
#endif // defined(GL_DEPTH_CLEAR_VALUE)
#if defined(GL_DEPTH_FUNC)
        case GL_DEPTH_FUNC:
#endif // defined(GL_DEPTH_FUNC)
#if defined(GL_DEPTH_WRITEMASK)
        case GL_DEPTH_WRITEMASK:
#endif // defined(GL_DEPTH_WRITEMASK)
#if defined(GL_FOG_DENSITY)
        case GL_FOG_DENSITY:
#endif // defined(GL_FOG_DENSITY)
#if defined(GL_FOG_END)
        case GL_FOG_END:
#endif // defined(GL_FOG_END)
#if defined(GL_FOG_MODE)
        case GL_FOG_MODE:
#endif // defined(GL_FOG_MODE)
#if defined(GL_FOG_START)
        case GL_FOG_START:
#endif // defined(GL_FOG_START)
#if defined(GL_FRONT_FACE)
        case GL_FRONT_FACE:
#endif // defined(GL_FRONT_FACE)
#if defined(GL_GREEN_BITS)
        case GL_GREEN_BITS:
#endif // defined(GL_GREEN_BITS)
#if defined(GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES)
        case GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES:
#endif // defined(GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES)
#if defined(GL_IMPLEMENTATION_COLOR_READ_TYPE_OES)
        case GL_IMPLEMENTATION_COLOR_READ_TYPE_OES:
#endif // defined(GL_IMPLEMENTATION_COLOR_READ_TYPE_OES)
#if defined(GL_LIGHT_MODEL_COLOR_CONTROL)
        case GL_LIGHT_MODEL_COLOR_CONTROL:
#endif // defined(GL_LIGHT_MODEL_COLOR_CONTROL)
#if defined(GL_LIGHT_MODEL_LOCAL_VIEWER)
        case GL_LIGHT_MODEL_LOCAL_VIEWER:
#endif // defined(GL_LIGHT_MODEL_LOCAL_VIEWER)
#if defined(GL_LIGHT_MODEL_TWO_SIDE)
        case GL_LIGHT_MODEL_TWO_SIDE:
#endif // defined(GL_LIGHT_MODEL_TWO_SIDE)
#if defined(GL_LINE_SMOOTH_HINT)
        case GL_LINE_SMOOTH_HINT:
#endif // defined(GL_LINE_SMOOTH_HINT)
#if defined(GL_LINE_WIDTH)
        case GL_LINE_WIDTH:
#endif // defined(GL_LINE_WIDTH)
#if defined(GL_LOGIC_OP_MODE)
        case GL_LOGIC_OP_MODE:
#endif // defined(GL_LOGIC_OP_MODE)
#if defined(GL_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES)
        case GL_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES:
#endif // defined(GL_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES)
#if defined(GL_MATRIX_INDEX_ARRAY_SIZE_OES)
        case GL_MATRIX_INDEX_ARRAY_SIZE_OES:
#endif // defined(GL_MATRIX_INDEX_ARRAY_SIZE_OES)
#if defined(GL_MATRIX_INDEX_ARRAY_STRIDE_OES)
        case GL_MATRIX_INDEX_ARRAY_STRIDE_OES:
#endif // defined(GL_MATRIX_INDEX_ARRAY_STRIDE_OES)
#if defined(GL_MATRIX_INDEX_ARRAY_TYPE_OES)
        case GL_MATRIX_INDEX_ARRAY_TYPE_OES:
#endif // defined(GL_MATRIX_INDEX_ARRAY_TYPE_OES)
#if defined(GL_MATRIX_MODE)
        case GL_MATRIX_MODE:
#endif // defined(GL_MATRIX_MODE)
#if defined(GL_MAX_CLIP_PLANES)
        case GL_MAX_CLIP_PLANES:
#endif // defined(GL_MAX_CLIP_PLANES)
#if defined(GL_MAX_ELEMENTS_INDICES)
        case GL_MAX_ELEMENTS_INDICES:
#endif // defined(GL_MAX_ELEMENTS_INDICES)
#if defined(GL_MAX_ELEMENTS_VERTICES)
        case GL_MAX_ELEMENTS_VERTICES:
#endif // defined(GL_MAX_ELEMENTS_VERTICES)
#if defined(GL_MAX_LIGHTS)
        case GL_MAX_LIGHTS:
#endif // defined(GL_MAX_LIGHTS)
#if defined(GL_MAX_MODELVIEW_STACK_DEPTH)
        case GL_MAX_MODELVIEW_STACK_DEPTH:
#endif // defined(GL_MAX_MODELVIEW_STACK_DEPTH)
#if defined(GL_MAX_PALETTE_MATRICES_OES)
        case GL_MAX_PALETTE_MATRICES_OES:
#endif // defined(GL_MAX_PALETTE_MATRICES_OES)
#if defined(GL_MAX_PROJECTION_STACK_DEPTH)
        case GL_MAX_PROJECTION_STACK_DEPTH:
#endif // defined(GL_MAX_PROJECTION_STACK_DEPTH)
#if defined(GL_MAX_TEXTURE_SIZE)
        case GL_MAX_TEXTURE_SIZE:
#endif // defined(GL_MAX_TEXTURE_SIZE)
#if defined(GL_MAX_TEXTURE_STACK_DEPTH)
        case GL_MAX_TEXTURE_STACK_DEPTH:
#endif // defined(GL_MAX_TEXTURE_STACK_DEPTH)
#if defined(GL_MAX_TEXTURE_UNITS)
        case GL_MAX_TEXTURE_UNITS:
#endif // defined(GL_MAX_TEXTURE_UNITS)
#if defined(GL_MAX_VERTEX_UNITS_OES)
        case GL_MAX_VERTEX_UNITS_OES:
#endif // defined(GL_MAX_VERTEX_UNITS_OES)
#if defined(GL_MODELVIEW_STACK_DEPTH)
        case GL_MODELVIEW_STACK_DEPTH:
#endif // defined(GL_MODELVIEW_STACK_DEPTH)
#if defined(GL_NORMAL_ARRAY_BUFFER_BINDING)
        case GL_NORMAL_ARRAY_BUFFER_BINDING:
#endif // defined(GL_NORMAL_ARRAY_BUFFER_BINDING)
#if defined(GL_NORMAL_ARRAY_STRIDE)
        case GL_NORMAL_ARRAY_STRIDE:
#endif // defined(GL_NORMAL_ARRAY_STRIDE)
#if defined(GL_NORMAL_ARRAY_TYPE)
        case GL_NORMAL_ARRAY_TYPE:
#endif // defined(GL_NORMAL_ARRAY_TYPE)
#if defined(GL_NUM_COMPRESSED_TEXTURE_FORMATS)
        case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
#endif // defined(GL_NUM_COMPRESSED_TEXTURE_FORMATS)
#if defined(GL_PACK_ALIGNMENT)
        case GL_PACK_ALIGNMENT:
#endif // defined(GL_PACK_ALIGNMENT)
#if defined(GL_PERSPECTIVE_CORRECTION_HINT)
        case GL_PERSPECTIVE_CORRECTION_HINT:
#endif // defined(GL_PERSPECTIVE_CORRECTION_HINT)
#if defined(GL_POINT_SIZE)
        case GL_POINT_SIZE:
#endif // defined(GL_POINT_SIZE)
#if defined(GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES)
        case GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES:
#endif // defined(GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES)
#if defined(GL_POINT_SIZE_ARRAY_STRIDE_OES)
        case GL_POINT_SIZE_ARRAY_STRIDE_OES:
#endif // defined(GL_POINT_SIZE_ARRAY_STRIDE_OES)
#if defined(GL_POINT_SIZE_ARRAY_TYPE_OES)
        case GL_POINT_SIZE_ARRAY_TYPE_OES:
#endif // defined(GL_POINT_SIZE_ARRAY_TYPE_OES)
#if defined(GL_POINT_SMOOTH_HINT)
        case GL_POINT_SMOOTH_HINT:
#endif // defined(GL_POINT_SMOOTH_HINT)
#if defined(GL_POLYGON_OFFSET_FACTOR)
        case GL_POLYGON_OFFSET_FACTOR:
#endif // defined(GL_POLYGON_OFFSET_FACTOR)
#if defined(GL_POLYGON_OFFSET_UNITS)
        case GL_POLYGON_OFFSET_UNITS:
#endif // defined(GL_POLYGON_OFFSET_UNITS)
#if defined(GL_PROJECTION_STACK_DEPTH)
        case GL_PROJECTION_STACK_DEPTH:
#endif // defined(GL_PROJECTION_STACK_DEPTH)
#if defined(GL_RED_BITS)
        case GL_RED_BITS:
#endif // defined(GL_RED_BITS)
#if defined(GL_SHADE_MODEL)
        case GL_SHADE_MODEL:
#endif // defined(GL_SHADE_MODEL)
#if defined(GL_STENCIL_BITS)
        case GL_STENCIL_BITS:
#endif // defined(GL_STENCIL_BITS)
#if defined(GL_STENCIL_CLEAR_VALUE)
        case GL_STENCIL_CLEAR_VALUE:
#endif // defined(GL_STENCIL_CLEAR_VALUE)
#if defined(GL_STENCIL_FAIL)
        case GL_STENCIL_FAIL:
#endif // defined(GL_STENCIL_FAIL)
#if defined(GL_STENCIL_FUNC)
        case GL_STENCIL_FUNC:
#endif // defined(GL_STENCIL_FUNC)
#if defined(GL_STENCIL_PASS_DEPTH_FAIL)
        case GL_STENCIL_PASS_DEPTH_FAIL:
#endif // defined(GL_STENCIL_PASS_DEPTH_FAIL)
#if defined(GL_STENCIL_PASS_DEPTH_PASS)
        case GL_STENCIL_PASS_DEPTH_PASS:
#endif // defined(GL_STENCIL_PASS_DEPTH_PASS)
#if defined(GL_STENCIL_REF)
        case GL_STENCIL_REF:
#endif // defined(GL_STENCIL_REF)
#if defined(GL_STENCIL_VALUE_MASK)
        case GL_STENCIL_VALUE_MASK:
#endif // defined(GL_STENCIL_VALUE_MASK)
#if defined(GL_STENCIL_WRITEMASK)
        case GL_STENCIL_WRITEMASK:
#endif // defined(GL_STENCIL_WRITEMASK)
#if defined(GL_SUBPIXEL_BITS)
        case GL_SUBPIXEL_BITS:
#endif // defined(GL_SUBPIXEL_BITS)
#if defined(GL_TEXTURE_BINDING_2D)
        case GL_TEXTURE_BINDING_2D:
#endif // defined(GL_TEXTURE_BINDING_2D)
#if defined(GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING)
        case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
#endif // defined(GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING)
#if defined(GL_TEXTURE_COORD_ARRAY_SIZE)
        case GL_TEXTURE_COORD_ARRAY_SIZE:
#endif // defined(GL_TEXTURE_COORD_ARRAY_SIZE)
#if defined(GL_TEXTURE_COORD_ARRAY_STRIDE)
        case GL_TEXTURE_COORD_ARRAY_STRIDE:
#endif // defined(GL_TEXTURE_COORD_ARRAY_STRIDE)
#if defined(GL_TEXTURE_COORD_ARRAY_TYPE)
        case GL_TEXTURE_COORD_ARRAY_TYPE:
#endif // defined(GL_TEXTURE_COORD_ARRAY_TYPE)
#if defined(GL_TEXTURE_STACK_DEPTH)
        case GL_TEXTURE_STACK_DEPTH:
#endif // defined(GL_TEXTURE_STACK_DEPTH)
#if defined(GL_UNPACK_ALIGNMENT)
        case GL_UNPACK_ALIGNMENT:
#endif // defined(GL_UNPACK_ALIGNMENT)
#if defined(GL_VERTEX_ARRAY_BUFFER_BINDING)
        case GL_VERTEX_ARRAY_BUFFER_BINDING:
#endif // defined(GL_VERTEX_ARRAY_BUFFER_BINDING)
#if defined(GL_VERTEX_ARRAY_SIZE)
        case GL_VERTEX_ARRAY_SIZE:
#endif // defined(GL_VERTEX_ARRAY_SIZE)
#if defined(GL_VERTEX_ARRAY_STRIDE)
        case GL_VERTEX_ARRAY_STRIDE:
#endif // defined(GL_VERTEX_ARRAY_STRIDE)
#if defined(GL_VERTEX_ARRAY_TYPE)
        case GL_VERTEX_ARRAY_TYPE:
#endif // defined(GL_VERTEX_ARRAY_TYPE)
#if defined(GL_WEIGHT_ARRAY_BUFFER_BINDING_OES)
        case GL_WEIGHT_ARRAY_BUFFER_BINDING_OES:
#endif // defined(GL_WEIGHT_ARRAY_BUFFER_BINDING_OES)
#if defined(GL_WEIGHT_ARRAY_SIZE_OES)
        case GL_WEIGHT_ARRAY_SIZE_OES:
#endif // defined(GL_WEIGHT_ARRAY_SIZE_OES)
#if defined(GL_WEIGHT_ARRAY_STRIDE_OES)
        case GL_WEIGHT_ARRAY_STRIDE_OES:
#endif // defined(GL_WEIGHT_ARRAY_STRIDE_OES)
#if defined(GL_WEIGHT_ARRAY_TYPE_OES)
        case GL_WEIGHT_ARRAY_TYPE_OES:
#endif // defined(GL_WEIGHT_ARRAY_TYPE_OES)
            _needed = 1;
            break;
#if defined(GL_ALIASED_POINT_SIZE_RANGE)
        case GL_ALIASED_POINT_SIZE_RANGE:
#endif // defined(GL_ALIASED_POINT_SIZE_RANGE)
#if defined(GL_ALIASED_LINE_WIDTH_RANGE)
        case GL_ALIASED_LINE_WIDTH_RANGE:
#endif // defined(GL_ALIASED_LINE_WIDTH_RANGE)
#if defined(GL_DEPTH_RANGE)
        case GL_DEPTH_RANGE:
#endif // defined(GL_DEPTH_RANGE)
#if defined(GL_MAX_VIEWPORT_DIMS)
        case GL_MAX_VIEWPORT_DIMS:
#endif // defined(GL_MAX_VIEWPORT_DIMS)
#if defined(GL_SMOOTH_LINE_WIDTH_RANGE)
        case GL_SMOOTH_LINE_WIDTH_RANGE:
#endif // defined(GL_SMOOTH_LINE_WIDTH_RANGE)
#if defined(GL_SMOOTH_POINT_SIZE_RANGE)
        case GL_SMOOTH_POINT_SIZE_RANGE:
#endif // defined(GL_SMOOTH_POINT_SIZE_RANGE)
            _needed = 2;
            break;
#if defined(GL_COLOR_CLEAR_VALUE)
        case GL_COLOR_CLEAR_VALUE:
#endif // defined(GL_COLOR_CLEAR_VALUE)
#if defined(GL_COLOR_WRITEMASK)
        case GL_COLOR_WRITEMASK:
#endif // defined(GL_COLOR_WRITEMASK)
#if defined(GL_FOG_COLOR)
        case GL_FOG_COLOR:
#endif // defined(GL_FOG_COLOR)
#if defined(GL_LIGHT_MODEL_AMBIENT)
        case GL_LIGHT_MODEL_AMBIENT:
#endif // defined(GL_LIGHT_MODEL_AMBIENT)
#if defined(GL_SCISSOR_BOX)
        case GL_SCISSOR_BOX:
#endif // defined(GL_SCISSOR_BOX)
#if defined(GL_VIEWPORT)
        case GL_VIEWPORT:
#endif // defined(GL_VIEWPORT)
            _needed = 4;
            break;
#if defined(GL_MODELVIEW_MATRIX)
        case GL_MODELVIEW_MATRIX:
#endif // defined(GL_MODELVIEW_MATRIX)
#if defined(GL_MODELVIEW_MATRIX_FLOAT_AS_INT_BITS_OES)
        case GL_MODELVIEW_MATRIX_FLOAT_AS_INT_BITS_OES:
#endif // defined(GL_MODELVIEW_MATRIX_FLOAT_AS_INT_BITS_OES)
#if defined(GL_PROJECTION_MATRIX)
        case GL_PROJECTION_MATRIX:
#endif // defined(GL_PROJECTION_MATRIX)
#if defined(GL_PROJECTION_MATRIX_FLOAT_AS_INT_BITS_OES)
        case GL_PROJECTION_MATRIX_FLOAT_AS_INT_BITS_OES:
#endif // defined(GL_PROJECTION_MATRIX_FLOAT_AS_INT_BITS_OES)
#if defined(GL_TEXTURE_MATRIX)
        case GL_TEXTURE_MATRIX:
#endif // defined(GL_TEXTURE_MATRIX)
#if defined(GL_TEXTURE_MATRIX_FLOAT_AS_INT_BITS_OES)
        case GL_TEXTURE_MATRIX_FLOAT_AS_INT_BITS_OES:
#endif // defined(GL_TEXTURE_MATRIX_FLOAT_AS_INT_BITS_OES)
            _needed = 16;
            break;
#if defined(GL_COMPRESSED_TEXTURE_FORMATS)
        case GL_COMPRESSED_TEXTURE_FORMATS:
#endif // defined(GL_COMPRESSED_TEXTURE_FORMATS)
            _needed = getNumCompressedTextureFormats();
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        SLOGGERD("GLES20", "GlGetIntegervEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLint *) (_paramsBase + _bufferOffset);
    }
    glGetIntegerv(
        (GLenum)pname,
        (GLint *)params
    );

    return NOERROR;
}

// C function void glGetProgramiv ( GLuint program, GLenum pname, GLint *params )

ECode GLES20::GlGetProgramiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlGetProgramiv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGetProgramiv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLint *) params_ref->GetPayload();
    params = params_base + offset;

    glGetProgramiv(
        (GLuint)program,
        (GLenum)pname,
        (GLint *)params
    );

    return NOERROR;
}

// C function void glGetProgramiv ( GLuint program, GLenum pname, GLint *params )

ECode GLES20::GlGetProgramiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params = (GLint *) tmp;
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);;
        params = (GLint *) (_paramsBase + _bufferOffset);
    }
    glGetProgramiv(
        (GLuint)program,
        (GLenum)pname,
        (GLint *)params
    );

    return NOERROR;
}

ECode GLES20::GlGetProgramInfoLog(
    /* [in] */ Int32 shader,
    /* [out] */ String* str)
{
    GLint infoLen = 0;
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    if (!infoLen) {
        *str = String("");
        return NOERROR;
    }
    char* buf = (char*) malloc(infoLen);
    if (buf == NULL) {
        *str = String(NULL);
        SLOGGERD("GLES20", "GlGetProgramInfoLog: out of memory")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    glGetProgramInfoLog(shader, infoLen, NULL, buf);
    *str = String(buf);
    if (buf) free(buf);
    return NOERROR;
}
// C function void glGetRenderbufferParameteriv ( GLenum target, GLenum pname, GLint *params )

ECode GLES20::GlGetRenderbufferParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlGetRenderbufferParameteriv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGetRenderbufferParameteriv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLint *) params_ref->GetPayload();
    params = params_base + offset;

    glGetRenderbufferParameteriv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

// C function void glGetRenderbufferParameteriv ( GLenum target, GLenum pname, GLint *params )

ECode GLES20::GlGetRenderbufferParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params = (GLint *) tmp;
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLint *) (_paramsBase + _bufferOffset);
    }
    glGetRenderbufferParameteriv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

// C function void glGetShaderiv ( GLuint shader, GLenum pname, GLint *params )

ECode GLES20::GlGetShaderiv(
    /* [in] */ Int32 shader,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlGetShaderiv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGetShaderiv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLint *) params_ref->GetPayload();
    params = params_base + offset;

    glGetShaderiv(
        (GLuint)shader,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

// C function void glGetShaderiv ( GLuint shader, GLenum pname, GLint *params )

ECode GLES20::GlGetShaderiv(
    /* [in] */ Int32 shader,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params = (GLint *) tmp;
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLint *) (_paramsBase + _bufferOffset);
    }
    glGetShaderiv(
        (GLuint)shader,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

// C function void glGetShaderInfoLog( GLuint shader, GLsizei maxLength, GLsizei * length,
//     ECode GLES20::Glchar * infoLog)


ECode GLES20::GlGetShaderInfoLog(
    /* [in] */ Int32 shader,
    /* [out] */ String* str)
{
    GLint infoLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    if (!infoLen) {
        *str = String("");
        return NOERROR;
    }
    char* buf = (char*) malloc(infoLen);
    if (buf == NULL) {
        *str = String(NULL);
        SLOGGERD("GLES20", "GlGetShaderInfoLog: out of memory")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    glGetShaderInfoLog(shader, infoLen, NULL, buf);
    *str = String(buf);
    free(buf);
    return NOERROR;
}

// C function void glGetShaderPrecisionFormat ( GLenum shadertype, GLenum precisiontype,
// GLint *range, GLint *precision )

ECode GLES20::GlGetShaderPrecisionFormat(
    /* [in] */ Int32 shadertype,
    /* [in] */ Int32 precisiontype,
    /* [in] */ ArrayOf<Int32>* range_ref,
    /* [in] */ Int32 rangeOffset,
    /* [in] */ ArrayOf<Int32>* precision_ref,
    /* [in] */ Int32 precisionOffset)
{
    GLint *range_base = (GLint *) 0;
    GLint *range = (GLint *) 0;
    GLint *precision_base = (GLint *) 0;
    GLint *precision = (GLint *) 0;

    if (!range_ref) {
        SLOGGERD("GLES20", "GlGetShaderPrecisionFormat: range == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (rangeOffset < 0) {
        SLOGGERD("GLES20", "GlGetShaderPrecisionFormat: rangeOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    range_base = (GLint *) range_ref->GetPayload();
    range = range_base + rangeOffset;

    if (!precision_ref) {
        SLOGGERD("GLES20", "GlGetShaderPrecisionFormat: precision == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (precisionOffset < 0) {
        SLOGGERD("GLES20", "GlGetShaderPrecisionFormat: precisionOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    precision_base = (GLint *) precision_ref->GetPayload();
    precision = precision_base + precisionOffset;

    glGetShaderPrecisionFormat(
        (GLenum)shadertype,
        (GLenum)precisiontype,
        (GLint *)range,
        (GLint *)precision
    );
    return NOERROR;
}

// C function void glGetShaderPrecisionFormat ( GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision )

ECode GLES20::GlGetShaderPrecisionFormat(
    /* [in] */ Int32 shadertype,
    /* [in] */ Int32 precisiontype,
    /* [in] */ IInt32Buffer* range_buf,
    /* [in] */ IInt32Buffer* precision_buf)
{
    Handle64 _rangeArray = (Handle64) 0;
    Int32 _rangeBufferOffset = (Int32) 0;
    Handle64 _precisionArray = (Handle64) 0;
    Int32 _precisionBufferOffset = (Int32) 0;
    Int32 _rangeRemaining;
    GLint *range = (GLint *) 0;
    Int32 _precisionRemaining;
    GLint *precision = (GLint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(range_buf), &_rangeArray, &_rangeRemaining, &_rangeBufferOffset, &tmp));
    range = (GLint *) tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(precision_buf), &_precisionArray, &_precisionRemaining, &_precisionBufferOffset, &tmp));
    precision = (GLint *) tmp;
    if (range == NULL) {
        char * _rangeBase = reinterpret_cast<char *>(_rangeArray);
        range = (GLint *) (_rangeBase + _rangeBufferOffset);
    }
    if (precision == NULL) {
        char * _precisionBase = reinterpret_cast<char *>(_precisionArray);
        precision = (GLint *) (_precisionBase + _precisionBufferOffset);
    }
    glGetShaderPrecisionFormat(
        (GLenum)shadertype,
        (GLenum)precisiontype,
        (GLint *)range,
        (GLint *)precision
    );
    return NOERROR;
}

// C function void glGetShaderSource ( GLuint shader, GLsizei bufsize, GLsizei *length, char *source )

ECode GLES20::GlGetShaderSource(
    /* [in] */ Int32 shader,
    /* [in] */ Int32 bufsize,
    /* [in] */ ArrayOf<Int32>* length_ref,
    /* [in] */ Int32 lengthOffset,
    /* [in] */ ArrayOf<Byte>* source_ref,
    /* [in] */ Int32 sourceOffset)
{
    GLsizei *length_base = (GLsizei *) 0;
    GLsizei *length = (GLsizei *) 0;
    char *source_base = (char *) 0;
    char *source = (char *) 0;

    if (!length_ref) {
        SLOGGERD("GLES20", "GlGetShaderSource: length == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (lengthOffset < 0) {
        SLOGGERD("GLES20", "GlGetShaderSource: lengthOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    length_base = (GLsizei *) length_ref->GetPayload();
    length = length_base + lengthOffset;

    if (!source_ref) {
        SLOGGERD("GLES20", "GlGetShaderSource: source == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (sourceOffset < 0) {
        SLOGGERD("GLES20", "GlGetShaderSource: sourceOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    source_base = (char *)  source_ref->GetPayload();
    source = source_base + sourceOffset;

    glGetShaderSource(
        (GLuint)shader,
        (GLsizei)bufsize,
        (GLsizei *)length,
        (char *)source
    );
    return NOERROR;
}

// C function void glGetShaderSource ( GLuint shader, GLsizei bufsize, GLsizei *length, char *source )

ECode GLES20::GlGetShaderSource(
    /* [in] */ Int32 shader,
    /* [in] */ Int32 bufsize,
    /* [in] */ IInt32Buffer* length_buf,
    /* [in] */ Byte source)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLsizei *length = (GLsizei *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(length_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    length = (GLsizei *) tmp;
    if (length == NULL) {
        char * _lengthBase = reinterpret_cast<char *>(_array);
        length = (GLsizei *) (_lengthBase + _bufferOffset);
    }
    glGetShaderSource(
        (GLuint)shader,
        (GLsizei)bufsize,
        (GLsizei *)length,
        reinterpret_cast<char *>(source)
    );
    return NOERROR;
}

// C function void glGetShaderSource ( GLuint shader, GLsizei bufsize, GLsizei *length, char *source )

ECode GLES20::GlGetShaderSource(
    /* [in] */ Int32 shader,
    /* [out] */ String* str)
{
    GLint shaderLen = 0;
    glGetShaderiv((GLuint)shader, GL_SHADER_SOURCE_LENGTH, &shaderLen);
    if (!shaderLen) {
        *str = String("");
        return NOERROR;
    }
    char* buf = (char*) malloc(shaderLen);
    if (buf == NULL) {
        *str = String(NULL);
        SLOGGERD("GLES20", "GlGetShaderSourceEx2: out of memory")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    glGetShaderSource(shader, shaderLen, NULL, buf);
    *str = String(buf);
    free(buf);
    return NOERROR;
}


// C function const GLubyte * glGetString ( GLenum name )

ECode GLES20::GlGetString(
    /* [in] */ Int32 name,
    /* [out] */ String* str)
{
    const char* chars = (const char*) glGetString((GLenum) name);
    *str = String(chars);
    return NOERROR;
}
// C function void glGetTexParameterfv ( GLenum target, GLenum pname, GLfloat *params )

ECode GLES20::GlGetTexParameterfv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlGetTexParameterfv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGetTexParameterfv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    if (_remaining < 1) {
        SLOGGERD("GLES20", "GlGetTexParameterfv: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glGetTexParameterfv(
        (GLenum)target,
        (GLenum)pname,
        (GLfloat *)params
    );

    return NOERROR;
}

// C function void glGetTexParameterfv ( GLenum target, GLenum pname, GLfloat *params )

ECode GLES20::GlGetTexParameterfv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ IFloatBuffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params = (GLfloat *) tmp;
    if (_remaining < 1) {
        SLOGGERD("GLES20", "GlGetTexParameterfvEx: remaining() < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glGetTexParameterfv(
        (GLenum)target,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

// C function void glGetTexParameteriv ( GLenum target, GLenum pname, GLint *params )

ECode GLES20::GlGetTexParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlGetTexParameteriv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGetTexParameteriv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    if (_remaining < 1) {
        SLOGGERD("GLES20", "GlGetTexParameteriv: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLint *) params_ref->GetPayload();
    params = params_base + offset;

    glGetTexParameteriv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

// C function void glGetTexParameteriv ( GLenum target, GLenum pname, GLint *params )

ECode GLES20::GlGetTexParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params = (GLint *) tmp;
    if (_remaining < 1) {
        SLOGGERD("GLES20", "GlGetTexParameterivEx: remaining() < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLint *) (_paramsBase + _bufferOffset);
    }
    glGetTexParameteriv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );

    return NOERROR;
}

// C function void glGetUniformfv ( GLuint program, GLint location, GLfloat *params )

ECode GLES20::GlGetUniformfv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlGetUniformfv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGetUniformfv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glGetUniformfv(
        (GLuint)program,
        (GLint)location,
        (GLfloat *)params
    );
    return NOERROR;
}

// C function void glGetUniformfv ( GLuint program, GLint location, GLfloat *params )

ECode GLES20::GlGetUniformfv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ IFloatBuffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params = (GLfloat *) tmp;
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glGetUniformfv(
        (GLuint)program,
        (GLint)location,
        (GLfloat *)params
    );
    return NOERROR;
}

// C function void glGetUniformiv ( GLuint program, GLint location, GLint *params )

ECode GLES20::GlGetUniformiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlGetUniformiv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGetUniformiv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLint *) params_ref->GetPayload();
    params = params_base + offset;

    glGetUniformiv(
        (GLuint)program,
        (GLint)location,
        (GLint *)params
    );
    return NOERROR;
}

// C function void glGetUniformiv ( GLuint program, GLint location, GLint *params )

ECode GLES20::GlGetUniformiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ IInt32Buffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params = (GLint *) tmp;
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLint *) (_paramsBase + _bufferOffset);
    }
    glGetUniformiv(
        (GLuint)program,
        (GLint)location,
        (GLint *)params
    );
    return NOERROR;
}

// C function int glGetUniformLocation ( GLuint program, const char *name )

ECode GLES20::GlGetUniformLocation(
    /* [in] */ Int32 program,
    /* [in] */ const String& name,
    /* [out] */ Int32* rst)
{
    *rst = 0;
    const char* _nativename = 0;

    if (!name) {
        SLOGGERD("GLES20", "GlGetUniformLocation: name == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _nativename = name.string();

    *rst = glGetUniformLocation(
        (GLuint)program,
        (char *)_nativename
    );
    return NOERROR;
}

// C function void glGetVertexAttribfv ( GLuint index, GLenum pname, GLfloat *params )

ECode GLES20::GlGetVertexAttribfv(
    /* [in] */ Int32 index,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlGetVertexAttribfv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGetVertexAttribfv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glGetVertexAttribfv(
        (GLuint)index,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

// C function void glGetVertexAttribfv ( GLuint index, GLenum pname, GLfloat *params )

ECode GLES20::GlGetVertexAttribfv(
    /* [in] */ Int32 index,
    /* [in] */ Int32 pname,
    /* [in] */ IFloatBuffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params = (GLfloat *) tmp;
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);;
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glGetVertexAttribfv(
        (GLuint)index,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

// C function void glGetVertexAttribiv ( GLuint index, GLenum pname, GLint *params )

ECode GLES20::GlGetVertexAttribiv(
    /* [in] */ Int32 index,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlGetVertexAttribiv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlGetVertexAttribiv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLint *) params_ref->GetPayload();
    params = params_base + offset;

    glGetVertexAttribiv(
        (GLuint)index,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

// C function void glGetVertexAttribiv ( GLuint index, GLenum pname, GLint *params )

ECode GLES20::GlGetVertexAttribiv(
    /* [in] */ Int32 index,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params = (GLint *) tmp;
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLint *) (_paramsBase + _bufferOffset);
    }
    glGetVertexAttribiv(
        (GLuint)index,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

// C function void glHint ( GLenum target, GLenum mode )

ECode GLES20::GlHint(
    /* [in] */ Int32 target,
    /* [in] */ Int32 mode)
{
    glHint(
        (GLenum)target,
        (GLenum)mode
    );
    return NOERROR;
}

// C function GLboolean glIsBuffer ( GLuint buffer )

ECode GLES20::GlIsBuffer(
    /* [in] */ Int32 buffer,
    /* [out] */ Boolean* rst)
{
    GLboolean _returnValue;
    _returnValue = glIsBuffer(
        (GLuint)buffer
    );
    *rst = _returnValue;
    return NOERROR;
}

// C function GLboolean glIsEnabled ( GLenum cap )

ECode GLES20::GlIsEnabled(
    /* [in] */ Int32 cap,
    /* [out] */ Boolean* rst)
{
    GLboolean _returnValue;
    _returnValue = glIsEnabled(
        (GLenum)cap
    );
    *rst = _returnValue;
    return NOERROR;
}

// C function GLboolean glIsFramebuffer ( GLuint framebuffer )

ECode GLES20::GlIsFramebuffer(
    /* [in] */ Int32 framebuffer,
    /* [out] */ Boolean* rst)
{
    GLboolean _returnValue;
    _returnValue = glIsFramebuffer(
        (GLuint)framebuffer
    );
    *rst = _returnValue;
    return NOERROR;
}

// C function GLboolean glIsProgram ( GLuint program )

ECode GLES20::GlIsProgram(
    /* [in] */ Int32 program,
    /* [out] */ Boolean* rst)
{
    GLboolean _returnValue;
    _returnValue = glIsProgram(
        (GLuint)program
    );
    *rst = _returnValue;
    return NOERROR;
}

// C function GLboolean glIsRenderbuffer ( GLuint renderbuffer )

ECode GLES20::GlIsRenderbuffer(
    /* [in] */ Int32 renderbuffer,
    /* [out] */ Boolean* rst)
{
    GLboolean _returnValue;
    _returnValue = glIsRenderbuffer(
        (GLuint)renderbuffer
    );
    *rst = _returnValue;
    return NOERROR;
}

// C function GLboolean glIsShader ( GLuint shader )

ECode GLES20::GlIsShader(
    /* [in] */ Int32 shader,
    /* [out] */ Boolean* rst)
{
    GLboolean _returnValue;
    _returnValue = glIsShader(
        (GLuint)shader
    );
    *rst = _returnValue;
    return NOERROR;
}

// C function GLboolean glIsTexture ( GLuint texture )

ECode GLES20::GlIsTexture(
    /* [in] */ Int32 texture,
    /* [out] */ Boolean* rst)
{
    GLboolean _returnValue;
    _returnValue = glIsTexture(
        (GLuint)texture
    );
    *rst = _returnValue;
    return NOERROR;
}

// C function void glLineWidth ( GLfloat width )

ECode GLES20::GlLineWidth(
    /* [in] */ Float width)
{
    glLineWidth(
        (GLfloat)width
    );
    return NOERROR;
}

// C function void glLinkProgram ( GLuint program )

ECode GLES20::GlLinkProgram(
    /* [in] */ Int32 program)
{
    glLinkProgram(
        (GLuint)program
    );
    return NOERROR;
}

// C function void glPixelStorei ( GLenum pname, GLint param )

ECode GLES20::GlPixelStorei(
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    glPixelStorei(
        (GLenum)pname,
        (GLint)param
    );
    return NOERROR;
}

// C function void glPolygonOffset ( GLfloat factor, GLfloat units )

ECode GLES20::GlPolygonOffset(
    /* [in] */ Float factor,
    /* [in] */ Float units)
{
    glPolygonOffset(
        (GLfloat)factor,
        (GLfloat)units
    );
    return NOERROR;
}

// C function void glReadPixels ( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format,
// GLenum type, GLvoid *pixels )

ECode GLES20::GlReadPixels(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 format,
    /* [in] */ Int32 type,
    /* [in] */ IBuffer* pixels_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLvoid *pixels = (GLvoid *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(pixels_buf, &_array, &_remaining, &_bufferOffset, &tmp));
    pixels = (GLvoid *) tmp;
    if (pixels == NULL) {
        char * _pixelsBase = reinterpret_cast<char *>(_array);
        pixels = (GLvoid *) (_pixelsBase + _bufferOffset);
    }
    glReadPixels(
        (GLint)x,
        (GLint)y,
        (GLsizei)width,
        (GLsizei)height,
        (GLenum)format,
        (GLenum)type,
        (GLvoid *)pixels
    );
    return NOERROR;
}

// C function void glReleaseShaderCompiler ( void )

ECode GLES20::GlReleaseShaderCompiler()
{
    glReleaseShaderCompiler();
    return NOERROR;
}

// C function void glRenderbufferStorage ( GLenum target, GLenum internalformat, GLsizei width, GLsizei height )

ECode GLES20::GlRenderbufferStorage(
    /* [in] */ Int32 target,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    glRenderbufferStorage(
        (GLenum)target,
        (GLenum)internalformat,
        (GLsizei)width,
        (GLsizei)height
    );
    return NOERROR;
}

// C function void glSampleCoverage ( GLclampf value, GLboolean invert )

ECode GLES20::GlSampleCoverage(
    /* [in] */ Float value,
    /* [in] */ Boolean invert)
{
    glSampleCoverage(
        (GLclampf)value,
        (GLboolean)invert
    );
    return NOERROR;
}

// C function void glScissor ( GLint x, GLint y, GLsizei width, GLsizei height )

ECode GLES20::GlScissor(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    glScissor(
        (GLint)x,
        (GLint)y,
        (GLsizei)width,
        (GLsizei)height
    );
    return NOERROR;
}

// C function void glShaderBinary ( GLsizei n, const GLuint *shaders, GLenum binaryformat,
// const GLvoid *binary, GLsizei length )

ECode GLES20::GlShaderBinary(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* shaders_ref,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 binaryformat,
    /* [in] */ IBuffer* binary_buf,
    /* [in] */ Int32 length)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    GLuint *shaders_base = (GLuint *) 0;
    GLuint *shaders = (GLuint *) 0;
    Int32 _binaryRemaining;
    GLvoid *binary = (GLvoid *) 0;

    if (!shaders_ref) {
        SLOGGERD("GLES20", "GlShaderBinary: shaders == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlShaderBinary: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    shaders_base = (GLuint *) shaders_ref->GetPayload();
    shaders = shaders_base + offset;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(binary_buf, &_array, &_binaryRemaining, &_bufferOffset, &tmp));
    binary = (GLvoid *) tmp;
    if (binary == NULL) {
        char * _binaryBase = reinterpret_cast<char *>(_array);;
        binary = (GLvoid *) (_binaryBase + _bufferOffset);
    }
    glShaderBinary(
        (GLsizei)n,
        (GLuint *)shaders,
        (GLenum)binaryformat,
        (GLvoid *)binary,
        (GLsizei)length
    );

    return NOERROR;
}

// C function void glShaderBinary ( GLsizei n, const GLuint *shaders, GLenum binaryformat,
// const GLvoid *binary, GLsizei length )

ECode GLES20::GlShaderBinary(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* shaders_buf,
    /* [in] */ Int32 binaryformat,
    /* [in] */ IBuffer* binary_buf,
    /* [in] */ Int32 length)
{
    Handle64 _shadersArray = (Handle64) 0;
    Int32 _shadersBufferOffset = (Int32) 0;
    Handle64 _binaryArray = (Handle64) 0;
    Int32 _binaryBufferOffset = (Int32) 0;
    Int32 _shadersRemaining;
    GLuint *shaders = (GLuint *) 0;
    Int32 _binaryRemaining;
    GLvoid *binary = (GLvoid *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(shaders_buf), &_shadersArray, &_shadersRemaining, &_shadersBufferOffset, &tmp));
    shaders = (GLuint *) tmp;
    FAIL_RETURN(GetPointer(binary_buf, &_binaryArray, &_binaryRemaining, &_binaryBufferOffset, &tmp));
    binary = (GLvoid *) tmp;
    if (shaders == NULL) {
        char * _shadersBase = reinterpret_cast<char *>(_shadersArray);
        shaders = (GLuint *) (_shadersBase + _shadersBufferOffset);
    }
    if (binary == NULL) {
        char * _binaryBase = reinterpret_cast<char *>(_binaryArray);
        binary = (GLvoid *) (_binaryBase + _binaryBufferOffset);
    }
    glShaderBinary(
        (GLsizei)n,
        (GLuint *)shaders,
        (GLenum)binaryformat,
        (GLvoid *)binary,
        (GLsizei)length
    );
    return NOERROR;
}

// C function void glShaderSource ( GLuint shader, GLsizei count, const GLchar ** string, const GLint* length )

ECode GLES20::GlShaderSource(
    /* [in] */ Int32 shader,
    /* [in] */ const String& string)
{

    if (string == NULL) {
        SLOGGERD("GLES20", "GlShaderSource: string == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    const char* nativeString = string.string();
    const char* strings[] = {nativeString};
    glShaderSource(shader, 1, strings, 0);
    return NOERROR;
}
// C function void glStencilFunc ( GLenum func, GLint ref, GLuint mask )

ECode GLES20::GlStencilFunc(
    /* [in] */ Int32 func,
    /* [in] */ Int32 ref,
    /* [in] */ Int32 mask)
{
    glStencilFunc(
        (GLenum)func,
        (GLint)ref,
        (GLuint)mask
    );
    return NOERROR;
}

// C function void glStencilFuncSeparate ( GLenum face, GLenum func, GLint ref, GLuint mask )

ECode GLES20::GlStencilFuncSeparate(
    /* [in] */ Int32 face,
    /* [in] */ Int32 func,
    /* [in] */ Int32 ref,
    /* [in] */ Int32 mask)
{
    glStencilFuncSeparate(
        (GLenum)face,
        (GLenum)func,
        (GLint)ref,
        (GLuint)mask
    );
    return NOERROR;
}

// C function void glStencilMask ( GLuint mask )

ECode GLES20::GlStencilMask(
    /* [in] */ Int32 mask)
{
    glStencilMask(
        (GLuint)mask
    );
    return NOERROR;
}

// C function void glStencilMaskSeparate ( GLenum face, GLuint mask )

ECode GLES20::GlStencilMaskSeparate(
    /* [in] */ Int32 face,
    /* [in] */ Int32 mask)
{
    glStencilMaskSeparate(
        (GLenum)face,
        (GLuint)mask
    );
    return NOERROR;
}

// C function void glStencilOp ( GLenum fail, GLenum zfail, GLenum zpass )

ECode GLES20::GlStencilOp(
    /* [in] */ Int32 fail,
    /* [in] */ Int32 zfail,
    /* [in] */ Int32 zpass)
{
    glStencilOp(
        (GLenum)fail,
        (GLenum)zfail,
        (GLenum)zpass
    );
    return NOERROR;
}

// C function void glStencilOpSeparate ( GLenum face, GLenum fail, GLenum zfail, GLenum zpass )

ECode GLES20::GlStencilOpSeparate(
    /* [in] */ Int32 face,
    /* [in] */ Int32 fail,
    /* [in] */ Int32 zfail,
    /* [in] */ Int32 zpass)
{
    glStencilOpSeparate(
        (GLenum)face,
        (GLenum)fail,
        (GLenum)zfail,
        (GLenum)zpass
    );
    return NOERROR;
}

// C function void glTexImage2D ( GLenum target, GLint level, GLint internalformat, GLsizei width,
// GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels )

ECode GLES20::GlTexImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 border,
    /* [in] */ Int32 format,
    /* [in] */ Int32 type,
    /* [in] */ IBuffer* pixels_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLvoid *pixels = (GLvoid *) 0;

    if (pixels_buf) {
        Handle64 tmp;
        FAIL_RETURN(GetPointer(pixels_buf, &_array, &_remaining, &_bufferOffset, &tmp));
        pixels = (GLvoid *) tmp;
    }
    if (pixels_buf && pixels == NULL) {
        char * _pixelsBase = reinterpret_cast<char *>(_array);
        pixels = (GLvoid *) (_pixelsBase + _bufferOffset);
    }
    glTexImage2D(
        (GLenum)target,
        (GLint)level,
        (GLint)internalformat,
        (GLsizei)width,
        (GLsizei)height,
        (GLint)border,
        (GLenum)format,
        (GLenum)type,
        (GLvoid *)pixels
    );
    return NOERROR;
}

// C function void glTexParameterf ( GLenum target, GLenum pname, GLfloat param )

ECode GLES20::GlTexParameterf(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Float param)
{
    glTexParameterf(
        (GLenum)target,
        (GLenum)pname,
        (GLfloat)param
    );
    return NOERROR;
}

// C function void glTexParameterfv ( GLenum target, GLenum pname, const GLfloat *params )

ECode GLES20::GlTexParameterfv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlTexParameterfv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlTexParameterfv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    if (_remaining < 1) {
        SLOGGERD("GLES20", "GlTexParameterfv: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glTexParameterfv(
        (GLenum)target,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

// C function void glTexParameterfv ( GLenum target, GLenum pname, const GLfloat *params )

ECode GLES20::GlTexParameterfv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ IFloatBuffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params = (GLfloat *) tmp;
    if (_remaining < 1) {
        SLOGGERD("GLES20", "GlTexParameterfvEx: remaining() < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glTexParameterfv(
        (GLenum)target,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

// C function void glTexParameteri ( GLenum target, GLenum pname, GLint param )

ECode GLES20::GlTexParameteri(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    glTexParameteri(
        (GLenum)target,
        (GLenum)pname,
        (GLint)param
    );
    return NOERROR;
}

// C function void glTexParameteriv ( GLenum target, GLenum pname, const GLint *params )

ECode GLES20::GlTexParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        SLOGGERD("GLES20", "GlTexParameteriv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlTexParameteriv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    if (_remaining < 1) {
        SLOGGERD("GLES20", "GlTexParameteriv: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLint *) params_ref->GetPayload();
    params = params_base + offset;

    glTexParameteriv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

// C function void glTexParameteriv ( GLenum target, GLenum pname, const GLint *params )

ECode GLES20::GlTexParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(params_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    params = (GLint *) tmp;
    if (_remaining < 1) {
        SLOGGERD("GLES20", "GlTexParameterivEx: remaining() < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLint *) (_paramsBase + _bufferOffset);
    }
    glTexParameteriv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

// C function void glTexSubImage2D ( GLenum target, GLint level, GLint xoffset,
// GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels )

ECode GLES20::GlTexSubImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 xoffset,
    /* [in] */ Int32 yoffset,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 format,
    /* [in] */ Int32 type,
    /* [in] */ IBuffer* pixels_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLvoid *pixels = (GLvoid *) 0;

    if (pixels_buf) {
        Handle64 tmp;
        FAIL_RETURN(GetPointer(pixels_buf, &_array, &_remaining, &_bufferOffset, &tmp));
    }
    if (pixels_buf && pixels == NULL) {
        char * _pixelsBase = reinterpret_cast<char *>(_array);
        pixels = (GLvoid *) (_pixelsBase + _bufferOffset);
    }
    glTexSubImage2D(
        (GLenum)target,
        (GLint)level,
        (GLint)xoffset,
        (GLint)yoffset,
        (GLsizei)width,
        (GLsizei)height,
        (GLenum)format,
        (GLenum)type,
        (GLvoid *)pixels
    );
    return NOERROR;
}

// C function void glUniform1f ( GLint location, GLfloat x )

ECode GLES20::GlUniform1f(
    /* [in] */ Int32 location,
    /* [in] */ Float x)
{
    glUniform1f(
        (GLint)location,
        (GLfloat)x
    );
    return NOERROR;
}

// C function void glUniform1fv ( GLint location, GLsizei count, const GLfloat *v )

ECode GLES20::GlUniform1fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Float>* v_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *v_base = (GLfloat *) 0;
    GLfloat *v = (GLfloat *) 0;

    if (!v_ref) {
        SLOGGERD("GLES20", "GlUniform1fv: v == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlUniform1fv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    v_base = (GLfloat *) v_ref->GetPayload();
    v = v_base + offset;

    glUniform1fv(
        (GLint)location,
        (GLsizei)count,
        (GLfloat *)v
    );
    return NOERROR;
}

// C function void glUniform1fv ( GLint location, GLsizei count, const GLfloat *v )

ECode GLES20::GlUniform1fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IFloatBuffer* v_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *v = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(v_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    v = (GLfloat *) tmp;
    if (v == NULL) {
        char * _vBase = reinterpret_cast<char *>(_array);
        v = (GLfloat *) (_vBase + _bufferOffset);
    }
    glUniform1fv(
        (GLint)location,
        (GLsizei)count,
        (GLfloat *)v
    );
    return NOERROR;
}

// C function void glUniform1i ( GLint location, GLint x )

ECode GLES20::GlUniform1i(
    /* [in] */ Int32 location,
    /* [in] */ Int32 x)
{
    glUniform1i(
        (GLint)location,
        (GLint)x
    );
    return NOERROR;
}

// C function void glUniform1iv ( GLint location, GLsizei count, const GLint *v )

ECode GLES20::GlUniform1iv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* v_ref,
    /* [in] */ Int32 offset)
{
    GLint *v_base = (GLint *) 0;
    GLint *v = (GLint *) 0;

    if (!v_ref) {
        SLOGGERD("GLES20", "GlUniform1iv: v == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlUniform1iv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    v_base = (GLint *) v_ref->GetPayload();
    v = v_base + offset;

    glUniform1iv(
        (GLint)location,
        (GLsizei)count,
        (GLint *)v
    );
    return NOERROR;
}

// C function void glUniform1iv ( GLint location, GLsizei count, const GLint *v )

ECode GLES20::GlUniform1iv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* v_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *v = (GLint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(v_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    v = (GLint *) tmp;
    if (v == NULL) {
        char * _vBase = reinterpret_cast<char *>(_array);
        v = (GLint *) (_vBase + _bufferOffset);
    }
    glUniform1iv(
        (GLint)location,
        (GLsizei)count,
        (GLint *)v
    );
    return NOERROR;
}

// C function void glUniform2f ( GLint location, GLfloat x, GLfloat y )

ECode GLES20::GlUniform2f(
    /* [in] */ Int32 location,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    glUniform2f(
        (GLint)location,
        (GLfloat)x,
        (GLfloat)y
    );
    return NOERROR;
}

// C function void glUniform2fv ( GLint location, GLsizei count, const GLfloat *v )

ECode GLES20::GlUniform2fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Float>* v_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *v_base = (GLfloat *) 0;
    GLfloat *v = (GLfloat *) 0;

    if (!v_ref) {
        SLOGGERD("GLES20", "GlUniform2fv: v == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlUniform2fv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    v_base = (GLfloat *) v_ref->GetPayload();
    v = v_base + offset;

    glUniform2fv(
        (GLint)location,
        (GLsizei)count,
        (GLfloat *)v
    );
    return NOERROR;
}

// C function void glUniform2fv ( GLint location, GLsizei count, const GLfloat *v )

ECode GLES20::GlUniform2fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IFloatBuffer* v_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *v = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(v_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    v = (GLfloat *) tmp;
    if (v == NULL) {
        char * _vBase = reinterpret_cast<char *>(_array);
        v = (GLfloat *) (_vBase + _bufferOffset);
    }
    glUniform2fv(
        (GLint)location,
        (GLsizei)count,
        (GLfloat *)v
    );
    return NOERROR;
}

// C function void glUniform2i ( GLint location, GLint x, GLint y )

ECode GLES20::GlUniform2i(
    /* [in] */ Int32 location,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    glUniform2i(
        (GLint)location,
        (GLint)x,
        (GLint)y
    );
    return NOERROR;
}

// C function void glUniform2iv ( GLint location, GLsizei count, const GLint *v )

ECode GLES20::GlUniform2iv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* v_ref,
    /* [in] */ Int32 offset)
{
    GLint *v_base = (GLint *) 0;
    GLint *v = (GLint *) 0;

    if (!v_ref) {
        SLOGGERD("GLES20", "GlUniform2iv: v == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlUniform2iv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    v_base = (GLint *) v_ref->GetPayload();
    v = v_base + offset;

    glUniform2iv(
        (GLint)location,
        (GLsizei)count,
        (GLint *)v
    );
    return NOERROR;
}

// C function void glUniform2iv ( GLint location, GLsizei count, const GLint *v )

ECode GLES20::GlUniform2iv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* v_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *v = (GLint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(v_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    v = (GLint *) tmp;
    if (v == NULL) {
        char * _vBase = reinterpret_cast<char *>(_array);
        v = (GLint *) (_vBase + _bufferOffset);
    }
    glUniform2iv(
        (GLint)location,
        (GLsizei)count,
        (GLint *)v
    );
    return NOERROR;
}

// C function void glUniform3f ( GLint location, GLfloat x, GLfloat y, GLfloat z )

ECode GLES20::GlUniform3f(
    /* [in] */ Int32 location,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z)
{
    glUniform3f(
        (GLint)location,
        (GLfloat)x,
        (GLfloat)y,
        (GLfloat)z
    );
    return NOERROR;
}

// C function void glUniform3fv ( GLint location, GLsizei count, const GLfloat *v )

ECode GLES20::GlUniform3fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Float>* v_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *v_base = (GLfloat *) 0;
    GLfloat *v = (GLfloat *) 0;

    if (!v_ref) {
        SLOGGERD("GLES20", "GlUniform3fv: v == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlUniform3fv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    v_base = (GLfloat *) v_ref->GetPayload();
    v = v_base + offset;

    glUniform3fv(
        (GLint)location,
        (GLsizei)count,
        (GLfloat *)v
    );
    return NOERROR;
}

// C function void glUniform3fv ( GLint location, GLsizei count, const GLfloat *v )

ECode GLES20::GlUniform3fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IFloatBuffer* v_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *v = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(v_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    v = (GLfloat *) tmp;
    if (v == NULL) {
        char * _vBase = reinterpret_cast<char *>(_array);
        v = (GLfloat *) (_vBase + _bufferOffset);
    }
    glUniform3fv(
        (GLint)location,
        (GLsizei)count,
        (GLfloat *)v
    );
    return NOERROR;
}

// C function void glUniform3i ( GLint location, GLint x, GLint y, GLint z )

ECode GLES20::GlUniform3i(
    /* [in] */ Int32 location,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z)
{
    glUniform3i(
        (GLint)location,
        (GLint)x,
        (GLint)y,
        (GLint)z
    );
    return NOERROR;
}

// C function void glUniform3iv ( GLint location, GLsizei count, const GLint *v )

ECode GLES20::GlUniform3iv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* v_ref,
    /* [in] */ Int32 offset)
{
    GLint *v_base = (GLint *) 0;
    GLint *v = (GLint *) 0;

    if (!v_ref) {
        SLOGGERD("GLES20", "GlUniform3iv: v == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlUniform3iv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    v_base = (GLint *) v_ref->GetPayload();
    v = v_base + offset;

    glUniform3iv(
        (GLint)location,
        (GLsizei)count,
        (GLint *)v
    );
    return NOERROR;
}

// C function void glUniform3iv ( GLint location, GLsizei count, const GLint *v )

ECode GLES20::GlUniform3iv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* v_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *v = (GLint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(v_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    v = (GLint *) tmp;
    if (v == NULL) {
        char * _vBase = reinterpret_cast<char *>(_array);
        v = (GLint *) (_vBase + _bufferOffset);
    }
    glUniform3iv(
        (GLint)location,
        (GLsizei)count,
        (GLint *)v
    );
    return NOERROR;
}

// C function void glUniform4f ( GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w )

ECode GLES20::GlUniform4f(
    /* [in] */ Int32 location,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z,
    /* [in] */ Float w)
{
    glUniform4f(
        (GLint)location,
        (GLfloat)x,
        (GLfloat)y,
        (GLfloat)z,
        (GLfloat)w
    );
    return NOERROR;
}

// C function void glUniform4fv ( GLint location, GLsizei count, const GLfloat *v )

ECode GLES20::GlUniform4fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Float>* v_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *v_base = (GLfloat *) 0;
    GLfloat *v = (GLfloat *) 0;

    if (!v_ref) {
        SLOGGERD("GLES20", "GlUniform4fv: v == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlUniform4fv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    v_base = (GLfloat *) v_ref->GetPayload();
    v = v_base + offset;

    glUniform4fv(
        (GLint)location,
        (GLsizei)count,
        (GLfloat *)v
    );
    return NOERROR;
}

// C function void glUniform4fv ( GLint location, GLsizei count, const GLfloat *v )

ECode GLES20::GlUniform4fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IFloatBuffer* v_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *v = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(v_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    v = (GLfloat *) tmp;
    if (v == NULL) {
        char * _vBase = reinterpret_cast<char *>(_array);
        v = (GLfloat *) (_vBase + _bufferOffset);
    }
    glUniform4fv(
        (GLint)location,
        (GLsizei)count,
        (GLfloat *)v
    );
    return NOERROR;
}

// C function void glUniform4i ( GLint location, GLint x, GLint y, GLint z, GLint w )

ECode GLES20::GlUniform4i(
    /* [in] */ Int32 location,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ Int32 w)
{
    glUniform4i(
        (GLint)location,
        (GLint)x,
        (GLint)y,
        (GLint)z,
        (GLint)w
    );
    return NOERROR;
}

// C function void glUniform4iv ( GLint location, GLsizei count, const GLint *v )

ECode GLES20::GlUniform4iv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* v_ref,
    /* [in] */ Int32 offset)
{
    GLint *v_base = (GLint *) 0;
    GLint *v = (GLint *) 0;

    if (!v_ref) {
        SLOGGERD("GLES20", "GlUniform4iv: v == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlUniform4iv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    v_base = (GLint *) v_ref->GetPayload();
    v = v_base + offset;

    glUniform4iv(
        (GLint)location,
        (GLsizei)count,
        (GLint *)v
    );
    return NOERROR;
}

// C function void glUniform4iv ( GLint location, GLsizei count, const GLint *v )

ECode GLES20::GlUniform4iv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* v_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *v = (GLint *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(v_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    v = (GLint *) tmp;
    if (v == NULL) {
        char * _vBase = reinterpret_cast<char *>(_array);
        v = (GLint *) (_vBase + _bufferOffset);
    }
    glUniform4iv(
        (GLint)location,
        (GLsizei)count,
        (GLint *)v
    );
    return NOERROR;
}

// C function void glUniformMatrix2fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES20::GlUniformMatrix2fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *value_base = (GLfloat *) 0;
    GLfloat *value = (GLfloat *) 0;

    if (!value_ref) {
        SLOGGERD("GLES20", "GlUniformMatrix2fv:: value == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlUniformMatrix2fv:: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    value_base = (GLfloat *) value_ref->GetPayload();
    value = value_base + offset;

    glUniformMatrix2fv(
        (GLint)location,
        (GLsizei)count,
        (GLboolean)transpose,
        (GLfloat *)value
    );
    return NOERROR;
}

// C function void glUniformMatrix2fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES20::GlUniformMatrix2fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *value = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(value_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    value = (GLfloat *) tmp;
    if (value == NULL) {
        char * _valueBase = reinterpret_cast<char *>(_array);
        value = (GLfloat *) (_valueBase + _bufferOffset);
    }
    glUniformMatrix2fv(
        (GLint)location,
        (GLsizei)count,
        (GLboolean)transpose,
        (GLfloat *)value
    );
    return NOERROR;
}

// C function void glUniformMatrix3fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES20::GlUniformMatrix3fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *value_base = (GLfloat *) 0;
    GLfloat *value = (GLfloat *) 0;

    if (!value_ref) {
        SLOGGERD("GLES20", "GlUniformMatrix3fv: value == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlUniformMatrix3fv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    value_base = (GLfloat *) value_ref->GetPayload();
    value = value_base + offset;

    glUniformMatrix3fv(
        (GLint)location,
        (GLsizei)count,
        (GLboolean)transpose,
        (GLfloat *)value
    );

    return NOERROR;
}

// C function void glUniformMatrix3fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES20::GlUniformMatrix3fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *value = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(value_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    value = (GLfloat *) tmp;
    if (value == NULL) {
        char * _valueBase = reinterpret_cast<char *>(_array);
        value = (GLfloat *) (_valueBase + _bufferOffset);
    }
    glUniformMatrix3fv(
        (GLint)location,
        (GLsizei)count,
        (GLboolean)transpose,
        (GLfloat *)value
    );
    return NOERROR;
}

// C function void glUniformMatrix4fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES20::GlUniformMatrix4fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *value_base = (GLfloat *) 0;
    GLfloat *value = (GLfloat *) 0;

    if (!value_ref) {
        SLOGGERD("GLES20", "GlUniformMatrix4fv: value == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlUniformMatrix4fv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    value_base = (GLfloat *) value_ref->GetPayload();
    value = value_base + offset;

    glUniformMatrix4fv(
        (GLint)location,
        (GLsizei)count,
        (GLboolean)transpose,
        (GLfloat *)value
    );
    return NOERROR;
}

// C function void glUniformMatrix4fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES20::GlUniformMatrix4fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *value = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(value_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    value = (GLfloat *) tmp;
    if (value == NULL) {
        char * _valueBase = reinterpret_cast<char *>(_array);
        value = (GLfloat *) (_valueBase + _bufferOffset);
    }
    glUniformMatrix4fv(
        (GLint)location,
        (GLsizei)count,
        (GLboolean)transpose,
        (GLfloat *)value
    );
    return NOERROR;
}

// C function void glUseProgram ( GLuint program )

ECode GLES20::GlUseProgram(
    /* [in] */ Int32 program)
{
    glUseProgram(
        (GLuint)program
    );
    return NOERROR;
}

// C function void glValidateProgram ( GLuint program )

ECode GLES20::GlValidateProgram(
    /* [in] */ Int32 program)
{
    glValidateProgram(
        (GLuint)program
    );
    return NOERROR;
}

// C function void glVertexAttrib1f ( GLuint indx, GLfloat x )

ECode GLES20::GlVertexAttrib1f(
    /* [in] */ Int32 indx,
    /* [in] */ Float x)
{
    glVertexAttrib1f(
        (GLuint)indx,
        (GLfloat)x
    );
    return NOERROR;
}

// C function void glVertexAttrib1fv ( GLuint indx, const GLfloat *values )

ECode GLES20::GlVertexAttrib1fv(
    /* [in] */ Int32 indx,
    /* [in] */ ArrayOf<Float>* values_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *values_base = (GLfloat *) 0;
    GLfloat *values = (GLfloat *) 0;

    if (!values_ref) {
        SLOGGERD("GLES20", "GlVertexAttrib1fv: values == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlVertexAttrib1fv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    values_base = (GLfloat *) values_ref->GetPayload();
    values = values_base + offset;

    glVertexAttrib1fv(
        (GLuint)indx,
        (GLfloat *)values
    );
    return NOERROR;
}

// C function void glVertexAttrib1fv ( GLuint indx, const GLfloat *values )

ECode GLES20::GlVertexAttrib1fv(
    /* [in] */ Int32 indx,
    /* [in] */ IFloatBuffer* values_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *values = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(values_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    values = (GLfloat *) tmp;
    if (values == NULL) {
        char * _valuesBase = reinterpret_cast<char *>(_array);
        values = (GLfloat *) (_valuesBase + _bufferOffset);
    }
    glVertexAttrib1fv(
        (GLuint)indx,
        (GLfloat *)values
    );
    return NOERROR;
}

// C function void glVertexAttrib2f ( GLuint indx, GLfloat x, GLfloat y )

ECode GLES20::GlVertexAttrib2f(
    /* [in] */ Int32 indx,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    glVertexAttrib2f(
        (GLuint)indx,
        (GLfloat)x,
        (GLfloat)y
    );
    return NOERROR;
}

// C function void glVertexAttrib2fv ( GLuint indx, const GLfloat *values )

ECode GLES20::GlVertexAttrib2fv(
    /* [in] */ Int32 indx,
    /* [in] */ ArrayOf<Float>* values_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *values_base = (GLfloat *) 0;
    GLfloat *values = (GLfloat *) 0;

    if (!values_ref) {
        SLOGGERD("GLES20", "GlVertexAttrib2fv: values == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlVertexAttrib2fv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    values_base = (GLfloat *) values_ref->GetPayload();
    values = values_base + offset;

    glVertexAttrib2fv(
        (GLuint)indx,
        (GLfloat *)values
    );
    return NOERROR;
}

// C function void glVertexAttrib2fv ( GLuint indx, const GLfloat *values )

ECode GLES20::GlVertexAttrib2fv(
    /* [in] */ Int32 indx,
    /* [in] */ IFloatBuffer* values_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *values = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(values_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    values = (GLfloat *) tmp;
    if (values == NULL) {
        char * _valuesBase = reinterpret_cast<char *>(_array);
        values = (GLfloat *) (_valuesBase + _bufferOffset);
    }
    glVertexAttrib2fv(
        (GLuint)indx,
        (GLfloat *)values
    );
    return NOERROR;
}

// C function void glVertexAttrib3f ( GLuint indx, GLfloat x, GLfloat y, GLfloat z )

ECode GLES20::GlVertexAttrib3f(
    /* [in] */ Int32 indx,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z)
{
    glVertexAttrib3f(
        (GLuint)indx,
        (GLfloat)x,
        (GLfloat)y,
        (GLfloat)z
    );
    return NOERROR;
}

// C function void glVertexAttrib3fv ( GLuint indx, const GLfloat *values )

ECode GLES20::GlVertexAttrib3fv(
    /* [in] */ Int32 indx,
    /* [in] */ ArrayOf<Float>* values_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *values_base = (GLfloat *) 0;
    GLfloat *values = (GLfloat *) 0;

    if (!values_ref) {
        SLOGGERD("GLES20", "GlVertexAttrib3fv: values == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlVertexAttrib3fv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    values_base = (GLfloat *) values_ref->GetPayload();
    values = values_base + offset;

    glVertexAttrib3fv(
        (GLuint)indx,
        (GLfloat *)values
    );
    return NOERROR;
}

// C function void glVertexAttrib3fv ( GLuint indx, const GLfloat *values )

ECode GLES20::GlVertexAttrib3fv(
    /* [in] */ Int32 indx,
    /* [in] */ IFloatBuffer* values_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *values = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(values_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    values = (GLfloat *) tmp;
    if (values == NULL) {
        char * _valuesBase = reinterpret_cast<char *>(_array);
        values = (GLfloat *) (_valuesBase + _bufferOffset);
    }
    glVertexAttrib3fv(
        (GLuint)indx,
        (GLfloat *)values
    );
    return NOERROR;
}

// C function void glVertexAttrib4f ( GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w )

ECode GLES20::GlVertexAttrib4f(
    /* [in] */ Int32 indx,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z,
    /* [in] */ Float w)
{
    glVertexAttrib4f(
        (GLuint)indx,
        (GLfloat)x,
        (GLfloat)y,
        (GLfloat)z,
        (GLfloat)w
    );
    return NOERROR;
}

// C function void glVertexAttrib4fv ( GLuint indx, const GLfloat *values )

ECode GLES20::GlVertexAttrib4fv(
    /* [in] */ Int32 indx,
    /* [in] */ ArrayOf<Float>* values_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *values_base = (GLfloat *) 0;
    GLfloat *values = (GLfloat *) 0;

    if (!values_ref) {
        SLOGGERD("GLES20", "GlVertexAttrib4fv: values == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        SLOGGERD("GLES20", "GlVertexAttrib4fv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    values_base = (GLfloat *) values_ref->GetPayload();
    values = values_base + offset;

    glVertexAttrib4fv(
        (GLuint)indx,
        (GLfloat *)values
    );
    return NOERROR;
}

// C function void glVertexAttrib4fv ( GLuint indx, const GLfloat *values )

ECode GLES20::GlVertexAttrib4fv(
    /* [in] */ Int32 indx,
    /* [in] */ IFloatBuffer* values_buf)
{
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *values = (GLfloat *) 0;

    Handle64 tmp;
    FAIL_RETURN(GetPointer(IBuffer::Probe(values_buf), &_array, &_remaining, &_bufferOffset, &tmp));
    values = (GLfloat *) tmp;
    if (values == NULL) {
        char * _valuesBase = reinterpret_cast<char *>(_array);
        values = (GLfloat *) (_valuesBase + _bufferOffset);
    }
    glVertexAttrib4fv(
        (GLuint)indx,
        (GLfloat *)values
    );

    return NOERROR;
}

// C function void glVertexAttribPointer ( GLuint indx, GLint size, GLenum type, GLboolean normalized,
// GLsizei stride, GLint offset )

ECode GLES20::GlVertexAttribPointer(
    /* [in] */ Int32 indx,
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Boolean normalized,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 offset)
{
    glVertexAttribPointer(
        (GLuint)indx,
        (GLint)size,
        (GLenum)type,
        (GLboolean)normalized,
        (GLsizei)stride,
        (const GLvoid *)offset
    );
    return NOERROR;
}

// C function void glVertexAttribPointer ( GLuint indx, GLint size, GLenum type,
// GLboolean normalized, GLsizei stride, const GLvoid *ptr )

ECode GLES20::GlVertexAttribPointer(
    /* [in] */ Int32 indx,
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Boolean normalized,
    /* [in] */ Int32 stride,
    /* [in] */ IBuffer* ptr)
{
    Int32 remaining;
    ptr->GetRemaining(&remaining);
    GlVertexAttribPointerBounds(
        indx,
        size,
        type,
        normalized,
        stride,
        ptr,
        remaining);

    return NOERROR;
}

// C function void glViewport ( GLint x, GLint y, GLsizei width, GLsizei height )

ECode GLES20::GlViewport(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    glViewport(
        (GLint)x,
        (GLint)y,
        (GLsizei)width,
        (GLsizei)height
    );
    return NOERROR;
}

ECode GLES20::GlVertexAttribPointerBounds(
        /* [in] */ Int32 indx,
        /* [in] */ Int32 size,
        /* [in] */ Int32 type,
        /* [in] */ Boolean normalized,
        /* [in] */ Int32 stride,
        /* [in] */ IBuffer* ptr_buf,
        /* [in] */ Int32 remaining)
{
    GLvoid *ptr = (GLvoid *) 0;
    if (ptr_buf) {
        Handle64 tmp;
        FAIL_RETURN(GetDirectBufferPointer(ptr_buf, &tmp));
        ptr = (GLvoid *) tmp;
        if ( ! ptr ) {
            return NOERROR;
        }
    }

    glVertexAttribPointerBounds(
        (GLuint)indx,
        (GLint)size,
        (GLenum)type,
        (GLboolean)normalized,
        (GLsizei)stride,
        (GLvoid *)ptr,
        (GLsizei)remaining
    );
    return NOERROR;
}

Int32 GLES20::GetNumCompressedTextureFormats() {
    int numCompressedTextureFormats = 0;
    glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &numCompressedTextureFormats);
    return numCompressedTextureFormats;
}


ECode GLES20::GetDirectBufferPointer(
    /* [in] */ IBuffer* buffer,
    /* [out] */ Handle64* result)
{
    Handle64 effectiveDirectAddress;
    buffer->GetEffectiveDirectAddress(&effectiveDirectAddress);
    if (effectiveDirectAddress != 0) {
        Int32 position, elementSizeShift;
        buffer->GetPosition(&position);
        buffer->GetElementSizeShift(&elementSizeShift);
        effectiveDirectAddress += (position << elementSizeShift);
    } else {
        SLOGGERD("GLES20", "GetDirectBufferPointer: Must use a native order direct Buffer")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *result = effectiveDirectAddress;
    return NOERROR;
}

ECode GLES20::GetPointer(
    /* [in] */ IBuffer* buffer,
    /* [in, out] */ Handle64* array,
    /* [in, out] */ Int32* remaining,
    /* [in, out] */ Int32* offset,
    /* [out] */ Handle64* rst)
{
    Int32 position;
    Int32 limit;
    Int32 elementSizeShift;
    Int64 pointer;
    buffer->GetPosition(&position);
    buffer->GetLimit(&limit);
    buffer->GetElementSizeShift(&elementSizeShift);
    *remaining = (limit - position) << elementSizeShift;

    AutoPtr<INIOAccess> helper;
    CNIOAccess::AcquireSingleton((INIOAccess**)&helper);

    helper->GetBasePointer(buffer, &pointer);
    if (pointer != 0L) {
        *array = 0;
        *rst = (Handle64)pointer;
        return NOERROR;
    }

    Boolean hasArray;
    if (buffer->HasArray(&hasArray), hasArray) {
        buffer->GetPrimitiveArray(array);
    } else {
        *array = 0;
    }
    helper->GetBaseArrayOffset(buffer, offset);

    *rst = 0;
    return NOERROR;
}

ECode GLES20::InitGL(IInterface* surface)
{
    if (!mEgl)
    {
        Slogger::E("GLES20", "sur,ISurface:%p", surface);

        mEgl = IEGL10::Probe(CEGLContextImpl::GetEGL());
        mEgl->EglGetDisplay(NULL, (XIEGLDisplay**)&mEglDisplay);

        AutoPtr<IEGL10Helper> egl10Helper;
        CEGL10Helper::AcquireSingleton((IEGL10Helper**)&egl10Helper);
        AutoPtr<XIEGLDisplay> noDisplay;
        egl10Helper->GetNoDisplay((XIEGLDisplay**)&noDisplay);
        if (mEglDisplay == noDisplay) {
            Slogger::E("GLES20", "eglGetDisplay failed");
            return E_RUNTIME_EXCEPTION;
        }

        AutoPtr<ArrayOf<Int32> > version = ArrayOf<Int32>::Alloc(2);
        Boolean isInit;
        mEgl->EglInitialize(mEglDisplay, version, &isInit);
        if(!isInit) {
            Slogger::E("GLES20", "eglInitialize failed");
            return E_RUNTIME_EXCEPTION;
        }

        AutoPtr<ArrayOf<Int32> > num_config = ArrayOf<Int32>::Alloc(1);
        Boolean eglChooseConfig;
        AutoPtr<ArrayOf<XIEGLConfig*> > configs = ArrayOf<XIEGLConfig*>::Alloc(1);
        AutoPtr<ArrayOf<Int32> > configSpec = ArrayOf<Int32>::Alloc(15);
        (*configSpec)[0] = IEGL10::_EGL_RENDERABLE_TYPE;
        (*configSpec)[1] = 4;
        (*configSpec)[2] = IEGL10::_EGL_RED_SIZE;
        (*configSpec)[3] = 8;
        (*configSpec)[4] = IEGL10::_EGL_GREEN_SIZE;
        (*configSpec)[5] = 8;
        (*configSpec)[6] = IEGL10::_EGL_BLUE_SIZE;
        (*configSpec)[7] = 8;
        (*configSpec)[8] = IEGL10::_EGL_ALPHA_SIZE;
        (*configSpec)[9] = 8;
        (*configSpec)[10] = IEGL10::_EGL_DEPTH_SIZE;
        (*configSpec)[11] = 0;
        (*configSpec)[12] = IEGL10::_EGL_STENCIL_SIZE;
        (*configSpec)[13] = 0;
        (*configSpec)[14] = IEGL10::_EGL_NONE;

        mEgl->EglChooseConfig(mEglDisplay, configSpec, configs, 1, num_config, &eglChooseConfig);
        if (!eglChooseConfig) {
            Slogger::E("GLES20", "eglChooseConfig failed");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        Int32 numConfigs = (*num_config)[0];

        if (numConfigs <= 0) {
            Slogger::E("GLES20", "No configs match configSpec");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        mEglConfig = (*configs)[0];
        AutoPtr<ArrayOf<Int32> > attribs = ArrayOf<Int32>::Alloc(3);
        (*attribs)[0] = 0x3098;
        (*attribs)[1] = 2;
        (*attribs)[2] = IEGL10::_EGL_NONE;

        AutoPtr<XIEGLContext> noContext;
        egl10Helper->GetNoContext((XIEGLContext**)&noContext);

        mEgl->EglCreateContext(mEglDisplay, mEglConfig, noContext, attribs, (XIEGLContext**)&mEglContext);

        if (mEglContext == NULL || mEglContext == noContext) {
            mEglContext = NULL;
            Slogger::E("GLES20", "can not get the context");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        Slogger::E("GLES20", "context, context:%p", mEglContext.Get());

        ECode ec = mEgl->EglCreateWindowSurface(mEglDisplay, mEglConfig, ISurface::Probe(surface), NULL, (XIEGLSurface**)&mEglSurface);
        if (ec != NOERROR) {
            Slogger::E("GLES20", "eglCreateWindowSurface error");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        Boolean r;
        mEgl->EglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext, &r);
        if(!r)
        {
            Slogger::E("GLES20", "MakeCurrent error");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }


    return NOERROR;
}

} // namespace Opengl
} // namespace Droid
} // namespace Elastos
