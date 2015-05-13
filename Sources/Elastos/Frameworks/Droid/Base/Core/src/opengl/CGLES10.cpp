
#include "CGLES10.h"
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <cmdef.h>
#include "elastos/Slogger.h"

#define LOGD(msg) SLOGGERD("CGLES10", msg)

using Elastos::IO::CNIOAccessHelper;
using Elastos::IO::INIOAccessHelper;

extern "C" {
GL_API void GL_APIENTRY glColorPointerBounds(GLint size, GLenum type, GLsizei stride,
        const GLvoid *ptr, GLsizei count);
GL_API void GL_APIENTRY glNormalPointerBounds(GLenum type, GLsizei stride,
        const GLvoid *pointer, GLsizei count);
GL_API void GL_APIENTRY glTexCoordPointerBounds(GLint size, GLenum type,
        GLsizei stride, const GLvoid *pointer, GLsizei count);
GL_API void GL_APIENTRY glVertexPointerBounds(GLint size, GLenum type,
        GLsizei stride, const GLvoid *pointer, GLsizei count);
}

namespace Elastos {
namespace Droid {
namespace Opengl {

ECode CGLES10::GlActiveTexture(
    /* [in] */ Int32 texture)
{
    glActiveTexture(
        (GLenum)texture
    );
    return NOERROR;
}

ECode CGLES10::GlAlphaFunc(
    /* [in] */ Int32 func,
    /* [in] */ Float ref)
{
    glAlphaFunc(
        (GLenum)func,
        (GLclampf)ref
    );
    return NOERROR;
}

ECode CGLES10::GlAlphaFuncx(
    /* [in] */ Int32 func,
    /* [in] */ Int32 ref)
{
    glAlphaFunc(
        (GLenum)func,
        (GLclampf)ref
    );
    return NOERROR;
}

ECode CGLES10::GlBindTexture(
    /* [in] */ Int32 target,
    /* [in] */ Int32 texture)
{
    glBindTexture(
        (GLenum)target,
        (GLuint)texture
    );
    return NOERROR;
}

ECode CGLES10::GlBlendFunc(
    /* [in] */ Int32 sfactor,
    /* [in] */ Int32 dfactor)
{
    glBlendFunc(
        (GLenum)sfactor,
        (GLenum)dfactor
    );
    return NOERROR;
}

ECode CGLES10::GlClear(
    /* [in] */ Int32 mask)
{
    glClear(
        (GLbitfield)mask
    );
    return NOERROR;
}

ECode CGLES10::GlClearColor(
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

ECode CGLES10::GlClearColorx(
    /* [in] */ Int32 red,
    /* [in] */ Int32 green,
    /* [in] */ Int32 blue,
    /* [in] */ Int32 alpha)
{
    glClearColorx(
        (GLclampx)red,
        (GLclampx)green,
        (GLclampx)blue,
        (GLclampx)alpha
    );
    return NOERROR;
}

ECode CGLES10::GlClearDepthf(
    /* [in] */ Float depth)
{
    glClearDepthf(
        (GLclampf)depth
    );
    return NOERROR;
}

ECode CGLES10::GlClearDepthx(
    /* [in] */ Int32 depth)
{
    glClearDepthx(
        (GLclampx)depth
    );
    return NOERROR;
}

ECode CGLES10::GlClearStencil(
    /* [in] */ Int32 s)
{
    glClearStencil(
        (GLint)s
    );
    return NOERROR;
}

ECode CGLES10::GlClientActiveTexture(
    /* [in] */ Int32 texture)
{
    glClientActiveTexture(
        (GLenum)texture
    );
    return NOERROR;
}

ECode CGLES10::GlColor4f(
    /* [in] */ Float red,
    /* [in] */ Float green,
    /* [in] */ Float blue,
    /* [in] */ Float alpha)
{
    glColor4f(
        (GLfloat)red,
        (GLfloat)green,
        (GLfloat)blue,
        (GLfloat)alpha
    );
    return NOERROR;
}

ECode CGLES10::GlColor4x(
    /* [in] */ Int32 red,
    /* [in] */ Int32 green,
    /* [in] */ Int32 blue,
    /* [in] */ Int32 alpha)
{
    glColor4x(
        (GLfixed)red,
        (GLfixed)green,
        (GLfixed)blue,
        (GLfixed)alpha
    );
    return NOERROR;
}

ECode CGLES10::GlColorMask(
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

ECode CGLES10::GlColorPointer(
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Elastos::IO::IBuffer* pointer)
{
    Int32 remaining;
    pointer->GetRemaining(&remaining);
    GlColorPointerBounds(
        size,
        type,
        stride,
        pointer,
        remaining
    );
    if ((size == 4) &&
        ((type == GL_FLOAT) ||
         (type == GL_UNSIGNED_BYTE) ||
         (type == GL_FIXED)) &&
        (stride >= 0)) {
        m_ColorPointer = pointer;
    }

    return NOERROR;
}

ECode CGLES10::GlCompressedTexImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 border,
    /* [in] */ Int32 imageSize,
    /* [in] */ Elastos::IO::IBuffer* data_buf)
{
    Handle32 _array = 0;
    Int32 _bufferOffset = 0;
    Int32 _remaining = 0;
    Handle32 data = 0;

    ASSERT_SUCCEEDED(GetPointer(data_buf, &_array, &_remaining, &_bufferOffset, &data));
    if (data == 0) {
        Handle32 _dataBase = _array;
        data = _dataBase + _bufferOffset;
    }
    glCompressedTexImage2D(
        (GLenum)target,
        (GLint)level,
        (GLenum)internalformat,
        (GLsizei)width,
        (GLsizei)height,
        (GLint)border,
        (GLsizei)imageSize,
        reinterpret_cast<GLvoid *>(data)
    );
    return NOERROR;
}

ECode CGLES10::GlCompressedTexSubImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 xoffset,
    /* [in] */ Int32 yoffset,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 format,
    /* [in] */ Int32 imageSize,
    /* [in] */ Elastos::IO::IBuffer* data_buf)
{
    Handle32 _array = 0;
    Int32 _bufferOffset = 0;
    Int32 _remaining = 0;
    Handle32 data = 0;

    ASSERT_SUCCEEDED(GetPointer(data_buf, &_array, &_remaining, &_bufferOffset, &data));
    if (data == 0) {
        Handle32 _dataBase = _array;
        data = _dataBase + _bufferOffset;
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
        reinterpret_cast<GLvoid *>(data)
    );
    return NOERROR;
}

ECode CGLES10::GlCopyTexImage2D(
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

ECode CGLES10::GlCopyTexSubImage2D(
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

ECode CGLES10::GlCullFace(
    /* [in] */ Int32 mode)
{
    glCullFace(
        (GLenum)mode
    );
    return NOERROR;
}

ECode CGLES10::GlDeleteTextures(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* textures_ref,
    /* [in] */ Int32 offset)
{
    GLuint *textures_base = (GLuint *) 0;
    Int32 _remaining;
    GLuint *textures = (GLuint *) 0;

    if (!textures_ref) {
        LOGD("GlDeleteTextures: textures == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlDeleteTextures: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = textures_ref->GetLength() - offset;
    if (_remaining < n) {
        LOGD("GlDeleteTextures: length - offset < n < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    textures_base = (GLuint *)(textures_ref->GetPayload());
    textures = textures_base + offset;

    glDeleteTextures(
        (GLsizei)n,
        (GLuint *)textures
    );
    return NOERROR;
}

ECode CGLES10::GlDeleteTexturesEx(
    /* [in] */ Int32 n,
    /* [in] */ Elastos::IO::IInt32Buffer* textures_buf)
{
    Handle32 _array = 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    Handle32 texturesTmp = 0;
    GLuint *textures = (GLuint *) 0;

    FAIL_RETURN(GetPointer(textures_buf, &_array, &_remaining, &_bufferOffset, &texturesTmp));
    textures = (GLuint*) texturesTmp;
    if (_remaining < n) {
        LOGD("GlDeleteTexturesEx: remaining() < n < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (textures == NULL) {
        char * _texturesBase = reinterpret_cast<char*>(_array);
        textures = (GLuint *) (_texturesBase + _bufferOffset);
    }
    glDeleteTextures(
        (GLsizei)n,
        (GLuint *)textures
    );
    return NOERROR;
}

ECode CGLES10::GlDepthFunc(
    /* [in] */ Int32 func)
{
    glDepthFunc(
        (GLenum)func
    );
    return NOERROR;
}

ECode CGLES10::GlDepthMask(
    /* [in] */ Boolean flag)
{
    glDepthMask(
        (GLboolean)flag
    );
    return NOERROR;
}

ECode CGLES10::GlDepthRangef(
    /* [in] */ Float zNear,
    /* [in] */ Float zFar)
{
    glDepthRangef(
        (GLclampf)zNear,
        (GLclampf)zFar
    );
    return NOERROR;
}

ECode CGLES10::GlDepthRangex(
    /* [in] */ Int32 zNear,
    /* [in] */ Int32 zFar)
{
    glDepthRangex(
        (GLclampx)zNear,
        (GLclampx)zFar
    );
    return NOERROR;
}

ECode CGLES10::GlDisable(
    /* [in] */ Int32 cap)
{
    glDisable(
        (GLenum)cap
    );
    return NOERROR;
}

ECode CGLES10::GlDisableClientState(
    /* [in] */ Int32 array)
{
    glDisableClientState(
        (GLenum)array
    );
    return NOERROR;
}

ECode CGLES10::GlDrawArrays(
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

ECode CGLES10::GlDrawElements(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 count,
    /* [in] */ Int32 type,
    /* [in] */ Elastos::IO::IBuffer* indices_buf)
{
    Handle32 _array = 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    Handle32 indicesTmp = 0;
    GLvoid* indices = (GLvoid *) 0;

    FAIL_RETURN(GetPointer(indices_buf, &_array, &_remaining, &_bufferOffset, &indicesTmp));
    indices = reinterpret_cast<GLvoid *>(indicesTmp);
    if (_remaining < count) {
        LOGD("GlDrawElements: remaining() < count < needed")
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
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

ECode CGLES10::GlEnable(
    /* [in] */ Int32 cap)
{
    glEnable(
        (GLenum)cap
    );
    return NOERROR;
}

ECode CGLES10::GlEnableClientState(
    /* [in] */ Int32 array)
{
    glEnableClientState(
        (GLenum)array
    );
    return NOERROR;
}

ECode CGLES10::GlFinish()
{
    glFinish();
    return NOERROR;
}

ECode CGLES10::GlFlush()
{
    glFlush();
    return NOERROR;
}

ECode CGLES10::GlFogf(
    /* [in] */ Int32 pname,
    /* [in] */ Float param)
{
    glFogf(
        (GLenum)pname,
        (GLfloat)param
    );
    return NOERROR;
}

ECode CGLES10::GlFogfv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    Float *params_base = (Float *) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        LOGD("GlFogfv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlFogfv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    Int32 _needed;
    switch (pname) {
#if defined(GL_FOG_MODE)
        case GL_FOG_MODE:
#endif // defined(GL_FOG_MODE)
#if defined(GL_FOG_DENSITY)
        case GL_FOG_DENSITY:
#endif // defined(GL_FOG_DENSITY)
#if defined(GL_FOG_START)
        case GL_FOG_START:
#endif // defined(GL_FOG_START)
#if defined(GL_FOG_END)
        case GL_FOG_END:
#endif // defined(GL_FOG_END)
            _needed = 1;
            break;
#if defined(GL_FOG_COLOR)
        case GL_FOG_COLOR:
#endif // defined(GL_FOG_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlFogfv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = params_ref->GetPayload();
    params = (GLfloat *)(params_base + offset);

    glFogfv(
        (GLenum)pname,
        (GLfloat *)params
    );

    return NOERROR;
}

ECode CGLES10::GlFogfvEx(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    Handle32 _array = 0;
    Int32 _bufferOffset = 0;
    Int32 _remaining;
    Handle32 paramsTmp;
    GLfloat *params = (GLfloat *) 0;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfloat *) paramsTmp;
    int _needed;
    switch (pname) {
#if defined(GL_FOG_MODE)
        case GL_FOG_MODE:
#endif // defined(GL_FOG_MODE)
#if defined(GL_FOG_DENSITY)
        case GL_FOG_DENSITY:
#endif // defined(GL_FOG_DENSITY)
#if defined(GL_FOG_START)
        case GL_FOG_START:
#endif // defined(GL_FOG_START)
#if defined(GL_FOG_END)
        case GL_FOG_END:
#endif // defined(GL_FOG_END)
            _needed = 1;
            break;
#if defined(GL_FOG_COLOR)
        case GL_FOG_COLOR:
#endif // defined(GL_FOG_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlFogfvEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glFogfv(
        (GLenum)pname,
        (GLfloat *)params
    );

    return NOERROR;
}

ECode CGLES10::GlFogx(
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    glFogx(
        (GLenum)pname,
        (GLfixed)param
    );
    return NOERROR;
}

ECode CGLES10::GlFogxv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *params_base = (GLfixed *) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;

    if (!params_ref) {
        LOGD("GlFogxv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlFogxv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    int _needed;
    switch (pname) {
#if defined(GL_FOG_MODE)
        case GL_FOG_MODE:
#endif // defined(GL_FOG_MODE)
#if defined(GL_FOG_DENSITY)
        case GL_FOG_DENSITY:
#endif // defined(GL_FOG_DENSITY)
#if defined(GL_FOG_START)
        case GL_FOG_START:
#endif // defined(GL_FOG_START)
#if defined(GL_FOG_END)
        case GL_FOG_END:
#endif // defined(GL_FOG_END)
            _needed = 1;
            break;
#if defined(GL_FOG_COLOR)
        case GL_FOG_COLOR:
#endif // defined(GL_FOG_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlFogxv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfixed *) params_ref->GetPayload();
    params = params_base + offset;

    glFogxv(
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES10::GlFogxvEx(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;
    Handle32 paramsTmp = (Handle32) 0;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfixed *) paramsTmp;
    int _needed;
    switch (pname) {
#if defined(GL_FOG_MODE)
        case GL_FOG_MODE:
#endif // defined(GL_FOG_MODE)
#if defined(GL_FOG_DENSITY)
        case GL_FOG_DENSITY:
#endif // defined(GL_FOG_DENSITY)
#if defined(GL_FOG_START)
        case GL_FOG_START:
#endif // defined(GL_FOG_START)
#if defined(GL_FOG_END)
        case GL_FOG_END:
#endif // defined(GL_FOG_END)
            _needed = 1;
            break;
#if defined(GL_FOG_COLOR)
        case GL_FOG_COLOR:
#endif // defined(GL_FOG_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlFogxvEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfixed *) (_paramsBase + _bufferOffset);
    }
    glFogxv(
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES10::GlFrontFace(
    /* [in] */ Int32 mode)
{
    glFrontFace(
        (GLenum)mode
    );
    return NOERROR;
}

ECode CGLES10::GlFrustumf(
    /* [in] */ Float left,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float top,
    /* [in] */ Float zNear,
    /* [in] */ Float zFar)
{
    glFrustumf(
        (GLfloat)left,
        (GLfloat)right,
        (GLfloat)bottom,
        (GLfloat)top,
        (GLfloat)zNear,
        (GLfloat)zFar
    );
    return NOERROR;
}

ECode CGLES10::GlFrustumx(
    /* [in] */ Int32 left,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ Int32 top,
    /* [in] */ Int32 zNear,
    /* [in] */ Int32 zFar)
{
    glFrustumx(
        (GLfixed)left,
        (GLfixed)right,
        (GLfixed)bottom,
        (GLfixed)top,
        (GLfixed)zNear,
        (GLfixed)zFar
    );
    return NOERROR;
}

ECode CGLES10::GlGenTextures(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* textures_ref,
    /* [in] */ Int32 offset)
{
    GLuint *textures_base = (GLuint *) 0;
    Int32 _remaining;
    GLuint *textures = (GLuint *) 0;

    if (!textures_ref) {
        LOGD("GlGenTextures: textures == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGenTextures: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = textures_ref->GetLength() - offset;
    if (_remaining < n) {
        LOGD("GlGenTextures: length - offset < n < needed")
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

ECode CGLES10::GlGenTexturesEx(
    /* [in] */ Int32 n,
    /* [in] */ Elastos::IO::IInt32Buffer* textures_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLuint *textures = (GLuint *) 0;
    Handle32 texturesTmp;

    FAIL_RETURN(GetPointer(textures_buf, &_array, &_remaining, &_bufferOffset, &texturesTmp));
    textures = (GLuint *) texturesTmp;
    if (_remaining < n) {
        LOGD("GlGenTexturesEx: remaining() < n < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (textures == NULL) {
        char * _texturesBase = reinterpret_cast<char *>(_array);
        textures = (GLuint *) (_texturesBase + _bufferOffset);
    }
    glGenTextures(
        (GLsizei)n,
        (GLuint *)textures
    );
    return NOERROR;
}

ECode CGLES10::GlGetError(
    /* [out] */ Int32* error)
{
    // TODO: Add your code here
    *error = glGetError();
    return *error;
}

ECode CGLES10::GlGetIntegerv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        LOGD("GlGetIntegerv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGetIntegerv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    int _needed;
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
            _needed = GetNumCompressedTextureFormats();
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetIntegerv: length - offset < needed")
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

ECode CGLES10::GlGetIntegervEx(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLint *) paramsTmp;
    int _needed;
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
            _needed = GetNumCompressedTextureFormats();
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetIntegervEx: remaining() < needed")
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

ECode CGLES10::GlGetString(
    /* [in] */ Int32 name,
    /* [out] */ String* str)
{
    const char* chars = (const char*) glGetString((GLenum) name);
    *str = String(chars);
    return NOERROR;
}

ECode CGLES10::GlHint(
    /* [in] */ Int32 target,
    /* [in] */ Int32 mode)
{
    glHint(
        (GLenum)target,
        (GLenum)mode
    );
    return NOERROR;
}

ECode CGLES10::GlLightModelf(
    /* [in] */ Int32 pname,
    /* [in] */ Float param)
{
    glLightModelf(
        (GLenum)pname,
        (GLfloat)param
    );
    return NOERROR;
}

ECode CGLES10::GlLightModelfv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        LOGD("GlLightModelfv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlLightModelfv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    int _needed;
    switch (pname) {
#if defined(GL_LIGHT_MODEL_TWO_SIDE)
        case GL_LIGHT_MODEL_TWO_SIDE:
#endif // defined(GL_LIGHT_MODEL_TWO_SIDE)
            _needed = 1;
            break;
#if defined(GL_LIGHT_MODEL_AMBIENT)
        case GL_LIGHT_MODEL_AMBIENT:
#endif // defined(GL_LIGHT_MODEL_AMBIENT)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlLightModelfv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glLightModelfv(
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES10::GlLightModelfvEx(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;
    Handle32 paramsTmp;
    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfloat *) paramsTmp;
    int _needed;
    switch (pname) {
#if defined(GL_LIGHT_MODEL_TWO_SIDE)
        case GL_LIGHT_MODEL_TWO_SIDE:
#endif // defined(GL_LIGHT_MODEL_TWO_SIDE)
            _needed = 1;
            break;
#if defined(GL_LIGHT_MODEL_AMBIENT)
        case GL_LIGHT_MODEL_AMBIENT:
#endif // defined(GL_LIGHT_MODEL_AMBIENT)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlLightModelfvEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glLightModelfv(
        (GLenum)pname,
        (GLfloat *)params
    );

    return NOERROR;
}

ECode CGLES10::GlLightModelx(
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    glLightModelx(
        (GLenum)pname,
        (GLfixed)param
    );

    return NOERROR;
}

ECode CGLES10::GlLightModelxv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *params_base = (GLfixed *) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;

    if (!params_ref) {
        LOGD("GlLightModelxv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlLightModelxv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    int _needed;
    switch (pname) {
#if defined(GL_LIGHT_MODEL_TWO_SIDE)
        case GL_LIGHT_MODEL_TWO_SIDE:
#endif // defined(GL_LIGHT_MODEL_TWO_SIDE)
            _needed = 1;
            break;
#if defined(GL_LIGHT_MODEL_AMBIENT)
        case GL_LIGHT_MODEL_AMBIENT:
#endif // defined(GL_LIGHT_MODEL_AMBIENT)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlLightModelxv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfixed *) params_ref->GetPayload();
    params = params_base + offset;

    glLightModelxv(
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES10::GlLightModelxvEx(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfixed *) paramsTmp;
    int _needed;
    switch (pname) {
#if defined(GL_LIGHT_MODEL_TWO_SIDE)
        case GL_LIGHT_MODEL_TWO_SIDE:
#endif // defined(GL_LIGHT_MODEL_TWO_SIDE)
            _needed = 1;
            break;
#if defined(GL_LIGHT_MODEL_AMBIENT)
        case GL_LIGHT_MODEL_AMBIENT:
#endif // defined(GL_LIGHT_MODEL_AMBIENT)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlLightModelxvEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfixed *) (_paramsBase + _bufferOffset);
    }
    glLightModelxv(
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES10::GlLightf(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ Float param)
{
    glLightf(
        (GLenum)light,
        (GLenum)pname,
        (GLfloat)param
    );
    return NOERROR;
}

ECode CGLES10::GlLightfv(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        LOGD("GlLightfv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlLightfv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    int _needed;
    switch (pname) {
#if defined(GL_SPOT_EXPONENT)
        case GL_SPOT_EXPONENT:
#endif // defined(GL_SPOT_EXPONENT)
#if defined(GL_SPOT_CUTOFF)
        case GL_SPOT_CUTOFF:
#endif // defined(GL_SPOT_CUTOFF)
#if defined(GL_CONSTANT_ATTENUATION)
        case GL_CONSTANT_ATTENUATION:
#endif // defined(GL_CONSTANT_ATTENUATION)
#if defined(GL_LINEAR_ATTENUATION)
        case GL_LINEAR_ATTENUATION:
#endif // defined(GL_LINEAR_ATTENUATION)
#if defined(GL_QUADRATIC_ATTENUATION)
        case GL_QUADRATIC_ATTENUATION:
#endif // defined(GL_QUADRATIC_ATTENUATION)
            _needed = 1;
            break;
#if defined(GL_SPOT_DIRECTION)
        case GL_SPOT_DIRECTION:
#endif // defined(GL_SPOT_DIRECTION)
            _needed = 3;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlLightfv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glLightfv(
        (GLenum)light,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES10::GlLightfvEx(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfloat *) paramsTmp;
    int _needed;
    switch (pname) {
#if defined(GL_SPOT_EXPONENT)
        case GL_SPOT_EXPONENT:
#endif // defined(GL_SPOT_EXPONENT)
#if defined(GL_SPOT_CUTOFF)
        case GL_SPOT_CUTOFF:
#endif // defined(GL_SPOT_CUTOFF)
#if defined(GL_CONSTANT_ATTENUATION)
        case GL_CONSTANT_ATTENUATION:
#endif // defined(GL_CONSTANT_ATTENUATION)
#if defined(GL_LINEAR_ATTENUATION)
        case GL_LINEAR_ATTENUATION:
#endif // defined(GL_LINEAR_ATTENUATION)
#if defined(GL_QUADRATIC_ATTENUATION)
        case GL_QUADRATIC_ATTENUATION:
#endif // defined(GL_QUADRATIC_ATTENUATION)
            _needed = 1;
            break;
#if defined(GL_SPOT_DIRECTION)
        case GL_SPOT_DIRECTION:
#endif // defined(GL_SPOT_DIRECTION)
            _needed = 3;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlLightfvEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glLightfv(
        (GLenum)light,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES10::GlLightx(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    glLightx(
        (GLenum)light,
        (GLenum)pname,
        (GLfixed)param
    );
    return NOERROR;
}

ECode CGLES10::GlLightxv(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *params_base = (GLfixed *) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;

    if (!params_ref) {
        LOGD("GlLightxv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlLightxv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    int _needed;
    switch (pname) {
#if defined(GL_SPOT_EXPONENT)
        case GL_SPOT_EXPONENT:
#endif // defined(GL_SPOT_EXPONENT)
#if defined(GL_SPOT_CUTOFF)
        case GL_SPOT_CUTOFF:
#endif // defined(GL_SPOT_CUTOFF)
#if defined(GL_CONSTANT_ATTENUATION)
        case GL_CONSTANT_ATTENUATION:
#endif // defined(GL_CONSTANT_ATTENUATION)
#if defined(GL_LINEAR_ATTENUATION)
        case GL_LINEAR_ATTENUATION:
#endif // defined(GL_LINEAR_ATTENUATION)
#if defined(GL_QUADRATIC_ATTENUATION)
        case GL_QUADRATIC_ATTENUATION:
#endif // defined(GL_QUADRATIC_ATTENUATION)
            _needed = 1;
            break;
#if defined(GL_SPOT_DIRECTION)
        case GL_SPOT_DIRECTION:
#endif // defined(GL_SPOT_DIRECTION)
            _needed = 3;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlLightxv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfixed *) params_ref->GetPayload();
    params = params_base + offset;

    glLightxv(
        (GLenum)light,
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES10::GlLightxvEx(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfixed *) paramsTmp;
    int _needed;
    switch (pname) {
#if defined(GL_SPOT_EXPONENT)
        case GL_SPOT_EXPONENT:
#endif // defined(GL_SPOT_EXPONENT)
#if defined(GL_SPOT_CUTOFF)
        case GL_SPOT_CUTOFF:
#endif // defined(GL_SPOT_CUTOFF)
#if defined(GL_CONSTANT_ATTENUATION)
        case GL_CONSTANT_ATTENUATION:
#endif // defined(GL_CONSTANT_ATTENUATION)
#if defined(GL_LINEAR_ATTENUATION)
        case GL_LINEAR_ATTENUATION:
#endif // defined(GL_LINEAR_ATTENUATION)
#if defined(GL_QUADRATIC_ATTENUATION)
        case GL_QUADRATIC_ATTENUATION:
#endif // defined(GL_QUADRATIC_ATTENUATION)
            _needed = 1;
            break;
#if defined(GL_SPOT_DIRECTION)
        case GL_SPOT_DIRECTION:
#endif // defined(GL_SPOT_DIRECTION)
            _needed = 3;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlLightxvEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfixed *) (_paramsBase + _bufferOffset);
    }
    glLightxv(
        (GLenum)light,
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES10::GlLineWidth(
    /* [in] */ Float width)
{
    glLineWidth(
        (GLfloat)width
    );
    return NOERROR;
}

ECode CGLES10::GlLineWidthx(
    /* [in] */ Int32 width)
{
    glLineWidthx(
        (GLfixed)width
    );
    return NOERROR;
}

ECode CGLES10::GlLoadIdentity()
{
    glLoadIdentity();
    return NOERROR;
}

ECode CGLES10::GlLoadMatrixf(
    /* [in] */ ArrayOf<Float>* m_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *m_base = (GLfloat *) 0;
    GLfloat *m = (GLfloat *) 0;

    if (!m_ref) {
        LOGD("GlLoadMatrixf: m == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlLoadMatrixf: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    m_base = (GLfloat *) m_ref->GetPayload();
    m = m_base + offset;

    glLoadMatrixf(
        (GLfloat *)m
    );
    return NOERROR;
}

ECode CGLES10::GlLoadMatrixfEx(
    /* [in] */ Elastos::IO::IFloatBuffer* m_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *m = (GLfloat *) 0;
    Handle32 mTmp;

    FAIL_RETURN(GetPointer(m_buf, &_array, &_remaining, &_bufferOffset, &mTmp));
    m = (GLfloat *) mTmp;
    if (m == NULL) {
        char * _mBase = reinterpret_cast<char *>(_array);
        m = (GLfloat *) (_mBase + _bufferOffset);
    }
    glLoadMatrixf(
        (GLfloat *)m
    );
    return NOERROR;
}

ECode CGLES10::GlLoadMatrixx(
    /* [in] */ ArrayOf<Int32>* m_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *m_base = (GLfixed *) 0;
    GLfixed *m = (GLfixed *) 0;

    if (!m_ref) {
        LOGD("GlLoadMatrixx: m == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlLoadMatrixx: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    m_base = (GLfixed *) m_ref->GetPayload();
    m = m_base + offset;

    glLoadMatrixx(
        (GLfixed *)m
    );
    return NOERROR;
}

ECode CGLES10::GlLoadMatrixxEx(
    /* [in] */ Elastos::IO::IInt32Buffer* m_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *m = (GLfixed *) 0;
    Handle32 mTmp;

    FAIL_RETURN(GetPointer(m_buf, &_array, &_remaining, &_bufferOffset, &mTmp));
    m = (GLfixed *) mTmp;
    if (m == NULL) {
        char * _mBase = reinterpret_cast<char *>(_array);
        m = (GLfixed *) (_mBase + _bufferOffset);
    }
    glLoadMatrixx(
        (GLfixed *)m
    );
    return NOERROR;
}

ECode CGLES10::GlLogicOp(
    /* [in] */ Int32 opcode)
{
    glLogicOp(
        (GLenum)opcode
    );
    return NOERROR;
}

ECode CGLES10::GlMaterialf(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ Float param)
{
    glMaterialf(
        (GLenum)face,
        (GLenum)pname,
        (GLfloat)param
    );
    return NOERROR;
}

ECode CGLES10::GlMaterialfv(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        LOGD("GlMaterialfv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlMaterialfv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    int _needed;
    switch (pname) {
#if defined(GL_SHININESS)
        case GL_SHININESS:
#endif // defined(GL_SHININESS)
            _needed = 1;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
#if defined(GL_AMBIENT_AND_DIFFUSE)
        case GL_AMBIENT_AND_DIFFUSE:
#endif // defined(GL_AMBIENT_AND_DIFFUSE)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlMaterialfv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glMaterialfv(
        (GLenum)face,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES10::GlMaterialfvEx(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfloat *) paramsTmp;
    int _needed;
    switch (pname) {
#if defined(GL_SHININESS)
        case GL_SHININESS:
#endif // defined(GL_SHININESS)
            _needed = 1;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
#if defined(GL_AMBIENT_AND_DIFFUSE)
        case GL_AMBIENT_AND_DIFFUSE:
#endif // defined(GL_AMBIENT_AND_DIFFUSE)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlMaterialfvEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glMaterialfv(
        (GLenum)face,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES10::GlMaterialx(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    glMaterialx(
        (GLenum)face,
        (GLenum)pname,
        (GLfixed)param
    );
    return NOERROR;
}

ECode CGLES10::GlMaterialxv(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *params_base = (GLfixed *) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;

    if (!params_ref) {
        LOGD("GlMaterialxv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlMaterialxv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    Int32 _needed;
    switch (pname) {
#if defined(GL_SHININESS)
        case GL_SHININESS:
#endif // defined(GL_SHININESS)
            _needed = 1;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
#if defined(GL_AMBIENT_AND_DIFFUSE)
        case GL_AMBIENT_AND_DIFFUSE:
#endif // defined(GL_AMBIENT_AND_DIFFUSE)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlMaterialxv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfixed *) params_ref->GetPayload();
    params = params_base + offset;

    glMaterialxv(
        (GLenum)face,
        (GLenum)pname,
        (GLfixed *)params
    );

    return NOERROR;
}

ECode CGLES10::GlMaterialxvEx(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfixed *) paramsTmp;
    Int32 _needed;
    switch (pname) {
#if defined(GL_SHININESS)
        case GL_SHININESS:
#endif // defined(GL_SHININESS)
            _needed = 1;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
#if defined(GL_AMBIENT_AND_DIFFUSE)
        case GL_AMBIENT_AND_DIFFUSE:
#endif // defined(GL_AMBIENT_AND_DIFFUSE)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfixed *) (_paramsBase + _bufferOffset);
    }
    glMaterialxv(
        (GLenum)face,
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES10::GlMatrixMode(
    /* [in] */ Int32 mode)
{
    glMatrixMode(
        (GLenum)mode
    );
    return NOERROR;
}

ECode CGLES10::GlMultMatrixf(
    /* [in] */ ArrayOf<Float>* m_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *m_base = (GLfloat *) 0;
    GLfloat *m = (GLfloat *) 0;

    if (!m_ref) {
        LOGD("m == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    m_base = (GLfloat *) m_ref->GetPayload();
    m = m_base + offset;

    glMultMatrixf(
        (GLfloat *)m
    );
    return NOERROR;
}

ECode CGLES10::GlMultMatrixfEx(
    /* [in] */ Elastos::IO::IFloatBuffer* m_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *m = (GLfloat *) 0;
    Handle32 mTmp;

    FAIL_RETURN(GetPointer(m_buf, &_array, &_remaining, &_bufferOffset, &mTmp));
    m = (GLfloat *) mTmp;
    if (m == NULL) {
        char * _mBase = reinterpret_cast<char *>(_array);
        m = (GLfloat *) (_mBase + _bufferOffset);
    }
    glMultMatrixf(
        (GLfloat *)m
    );

    return NOERROR;
}

ECode CGLES10::GlMultMatrixx(
    /* [in] */ ArrayOf<Int32>* m_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *m_base = (GLfixed *) 0;
    GLfixed *m = (GLfixed *) 0;

    if (!m_ref) {
        LOGD("m == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    m_base = (GLfixed *) m_ref->GetPayload();
    m = m_base + offset;

    glMultMatrixx(
        (GLfixed *)m
    );
    return NOERROR;
}

ECode CGLES10::GlMultMatrixxEx(
    /* [in] */ Elastos::IO::IInt32Buffer* m_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *m = (GLfixed *) 0;
    Handle32 mTmp;

    FAIL_RETURN(GetPointer(m_buf, &_array, &_remaining, &_bufferOffset, &mTmp));
    m = (GLfixed *) mTmp;
    if (m == NULL) {
        char * _mBase = reinterpret_cast<char *>(_array);
        m = (GLfixed *) (_mBase + _bufferOffset);
    }
    glMultMatrixx(
        (GLfixed *)m
    );

    return NOERROR;
}

ECode CGLES10::GlMultiTexCoord4f(
    /* [in] */ Int32 target,
    /* [in] */ Float s,
    /* [in] */ Float t,
    /* [in] */ Float r,
    /* [in] */ Float q)
{
    glMultiTexCoord4f(
        (GLenum)target,
        (GLfloat)s,
        (GLfloat)t,
        (GLfloat)r,
        (GLfloat)q
    );
    return NOERROR;
}

ECode CGLES10::GlMultiTexCoord4x(
    /* [in] */ Int32 target,
    /* [in] */ Int32 s,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 q)
{
    glMultiTexCoord4x(
        (GLenum)target,
        (GLfixed)s,
        (GLfixed)t,
        (GLfixed)r,
        (GLfixed)q
    );
    return NOERROR;
}

ECode CGLES10::GlNormal3f(
    /* [in] */ Float nx,
    /* [in] */ Float ny,
    /* [in] */ Float nz)
{
    glNormal3f(
        (GLfloat)nx,
        (GLfloat)ny,
        (GLfloat)nz
    );
    return NOERROR;
}

ECode CGLES10::GlNormal3x(
    /* [in] */ Int32 nx,
    /* [in] */ Int32 ny,
    /* [in] */ Int32 nz)
{
    glNormal3x(
        (GLfixed)nx,
        (GLfixed)ny,
        (GLfixed)nz
    );
    return NOERROR;
}

ECode CGLES10::GlNormalPointerBounds(
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Elastos::IO::IBuffer* pointer_buf,
    /* [in] */ Int32 remaining)
{
    GLvoid *pointer = (GLvoid *) 0;
    Handle32 pointerTmp;

    if (pointer_buf) {
        GetDirectBufferPointer(pointer_buf, &pointerTmp);
        pointer = (GLvoid *) pointerTmp;
        if ( ! pointer ) {
            return NOERROR;
        }
    }
    glNormalPointerBounds(
        (GLenum)type,
        (GLsizei)stride,
        (GLvoid *)pointer,
        (GLsizei)remaining
    );
    return NOERROR;
}

ECode CGLES10::GlNormalPointer(
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Elastos::IO::IBuffer* pointer_buf)
{
    Int32 remaining;
    pointer_buf->GetRemaining(&remaining);
    GlNormalPointerBounds(
        type,
        stride,
        pointer_buf,
        remaining
    );
    if (((type == GL_FLOAT) ||
         (type == GL_BYTE) ||
         (type == GL_SHORT) ||
         (type == GL_FIXED)) &&
        (stride >= 0)) {
        m_NormalPointer = pointer_buf;
    }
    return NOERROR;
}

ECode CGLES10::GlOrthof(
    /* [in] */ Float left,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float top,
    /* [in] */ Float zNear,
    /* [in] */ Float zFar)
{
    glOrthof(
        (GLfloat)left,
        (GLfloat)right,
        (GLfloat)bottom,
        (GLfloat)top,
        (GLfloat)zNear,
        (GLfloat)zFar
    );
    return NOERROR;
}

ECode CGLES10::GlOrthox(
    /* [in] */ Int32 left,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ Int32 top,
    /* [in] */ Int32 zNear,
    /* [in] */ Int32 zFar)
{
    glOrthox(
        (GLfixed)left,
        (GLfixed)right,
        (GLfixed)bottom,
        (GLfixed)top,
        (GLfixed)zNear,
        (GLfixed)zFar
    );
    return NOERROR;
}

ECode CGLES10::GlPixelStorei(
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    glPixelStorei(
        (GLenum)pname,
        (GLint)param
    );
    return NOERROR;
}

ECode CGLES10::GlPointSize(
    /* [in] */ Float size)
{
    glPointSize(
        (GLfloat)size
    );
    return NOERROR;
}

ECode CGLES10::GlPointSizex(
    /* [in] */ Int32 size)
{
    glPointSizex(
        (GLfixed)size
    );
    return NOERROR;
}

ECode CGLES10::GlPolygonOffset(
    /* [in] */ Float factor,
    /* [in] */ Float units)
{
    glPolygonOffset(
        (GLfixed)factor,
        (GLfixed)units
    );
    return NOERROR;
}

ECode CGLES10::GlPolygonOffsetx(
    /* [in] */ Int32 factor,
    /* [in] */ Int32 units)
{
    glPolygonOffsetx(
        (GLfixed)factor,
        (GLfixed)units
    );
    return NOERROR;
}

ECode CGLES10::GlPopMatrix()
{
    glPopMatrix();
    return NOERROR;
}

ECode CGLES10::GlPushMatrix()
{
    glPushMatrix();
    return NOERROR;
}

ECode CGLES10::GlReadPixels(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 format,
    /* [in] */ Int32 type,
    /* [in] */ Elastos::IO::IBuffer* pixels_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLvoid *pixels = (GLvoid *) 0;
    Handle32 pixelsTmp;
    FAIL_RETURN(GetPointer(pixels_buf, &_array, &_remaining, &_bufferOffset, &pixelsTmp));
    pixels = (GLvoid *) pixelsTmp;

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

ECode CGLES10::GlRotatef(
    /* [in] */ Float angle,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z)
{
    glRotatef(
        (GLfloat)angle,
        (GLfloat)x,
        (GLfloat)y,
        (GLfloat)z
    );
    return NOERROR;
}

ECode CGLES10::GlRotatex(
    /* [in] */ Int32 angle,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z)
{
    glRotatex(
        (GLfixed)angle,
        (GLfixed)x,
        (GLfixed)y,
        (GLfixed)z
    );
    return NOERROR;
}

ECode CGLES10::GlSampleCoverage(
    /* [in] */ Float value,
    /* [in] */ Boolean invert)
{
    glSampleCoverage(
        (GLclampf)value,
        (GLboolean)invert
    );
    return NOERROR;
}

ECode CGLES10::GlSampleCoveragex(
    /* [in] */ Int32 value,
    /* [in] */ Boolean invert)
{
    glSampleCoveragex(
        (GLclampx)value,
        (GLboolean)invert
    );
    return NOERROR;
}

ECode CGLES10::GlScalef(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z)
{
    glScalef(
        (GLfloat)x,
        (GLfloat)y,
        (GLfloat)z
    );
    return NOERROR;
}

ECode CGLES10::GlScalex(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z)
{
    glScalex(
        (GLfixed)x,
        (GLfixed)y,
        (GLfixed)z
    );
    return NOERROR;
}

ECode CGLES10::GlScissor(
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

ECode CGLES10::GlShadeModel(
    /* [in] */ Int32 mode)
{
    glShadeModel(
        (GLenum)mode
    );
    return NOERROR;
}

ECode CGLES10::GlStencilFunc(
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

ECode CGLES10::GlStencilMask(
    /* [in] */ Int32 mask)
{
    glStencilMask(
        (GLuint)mask
    );
    return NOERROR;
}

ECode CGLES10::GlStencilOp(
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

ECode CGLES10::GlTexCoordPointerBounds(
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Elastos::IO::IBuffer* pointer_buf,
    /* [in] */ Int32 remaining)
{
    GLvoid *pointer = (GLvoid *) 0;
    Handle32 pointerTmp;

    if (pointer_buf) {
        GetDirectBufferPointer(pointer_buf, &pointerTmp);
        pointer = (GLvoid *) pointerTmp;
        if ( ! pointer ) {
            return NOERROR;
        }
    }
    glTexCoordPointerBounds(
        (GLint)size,
        (GLenum)type,
        (GLsizei)stride,
        (GLvoid *)pointer,
        (GLsizei)remaining
    );
    return NOERROR;
}

ECode CGLES10::GlTexCoordPointer(
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Elastos::IO::IBuffer* pointer_buf)
{
    Int32 remaining;
    pointer_buf->GetRemaining(&remaining);

    GlTexCoordPointerBounds(
        size,
        type,
        stride,
        pointer_buf,
        remaining
    );
    if (((size == 2) ||
         (size == 3) ||
         (size == 4)) &&
        ((type == GL_FLOAT) ||
         (type == GL_BYTE) ||
         (type == GL_SHORT) ||
         (type == GL_FIXED)) &&
        (stride >= 0)) {
        m_TexCoordPointer = pointer_buf;
    }
    return NOERROR;
}

ECode CGLES10::GlTexEnvf(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Float param)
{
    glTexEnvf(
        (GLenum)target,
        (GLenum)pname,
        (GLfloat)param
    );
    return NOERROR;
}

ECode CGLES10::GlTexEnvfv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        LOGD("GlTexEnvfv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlTexEnvfv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    Int32 _needed;
    switch (pname) {
#if defined(GL_TEXTURE_ENV_MODE)
        case GL_TEXTURE_ENV_MODE:
#endif // defined(GL_TEXTURE_ENV_MODE)
#if defined(GL_COMBINE_RGB)
        case GL_COMBINE_RGB:
#endif // defined(GL_COMBINE_RGB)
#if defined(GL_COMBINE_ALPHA)
        case GL_COMBINE_ALPHA:
#endif // defined(GL_COMBINE_ALPHA)
            _needed = 1;
            break;
#if defined(GL_TEXTURE_ENV_COLOR)
        case GL_TEXTURE_ENV_COLOR:
#endif // defined(GL_TEXTURE_ENV_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlTexEnvfv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glTexEnvfv(
        (GLenum)target,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES10::GlTexEnvfvEx(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;
    Handle32 paramsTmp;
    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));

    params = (GLfloat *) paramsTmp;
    Int32 _needed;
    switch (pname) {
#if defined(GL_TEXTURE_ENV_MODE)
        case GL_TEXTURE_ENV_MODE:
#endif // defined(GL_TEXTURE_ENV_MODE)
#if defined(GL_COMBINE_RGB)
        case GL_COMBINE_RGB:
#endif // defined(GL_COMBINE_RGB)
#if defined(GL_COMBINE_ALPHA)
        case GL_COMBINE_ALPHA:
#endif // defined(GL_COMBINE_ALPHA)
            _needed = 1;
            break;
#if defined(GL_TEXTURE_ENV_COLOR)
        case GL_TEXTURE_ENV_COLOR:
#endif // defined(GL_TEXTURE_ENV_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlTexEnvfvEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glTexEnvfv(
        (GLenum)target,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES10::GlTexEnvx(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    glTexEnvx(
        (GLenum)target,
        (GLenum)pname,
        (GLfixed)param
    );
    return NOERROR;
}

ECode CGLES10::GlTexEnvxv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *params_base = (GLfixed *) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;

    if (!params_ref) {
        LOGD("GlTexEnvxv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlTexEnvxv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    Int32 _needed;
    switch (pname) {
#if defined(GL_TEXTURE_ENV_MODE)
        case GL_TEXTURE_ENV_MODE:
#endif // defined(GL_TEXTURE_ENV_MODE)
#if defined(GL_COMBINE_RGB)
        case GL_COMBINE_RGB:
#endif // defined(GL_COMBINE_RGB)
#if defined(GL_COMBINE_ALPHA)
        case GL_COMBINE_ALPHA:
#endif // defined(GL_COMBINE_ALPHA)
            _needed = 1;
            break;
#if defined(GL_TEXTURE_ENV_COLOR)
        case GL_TEXTURE_ENV_COLOR:
#endif // defined(GL_TEXTURE_ENV_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlTexEnvxv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfixed *) params_ref->GetPayload();
    params = params_base + offset;

    glTexEnvxv(
        (GLenum)target,
        (GLenum)pname,
        (GLfixed *)params
    );

    return NOERROR;
}

ECode CGLES10::GlTexEnvxvEx(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfixed *) paramsTmp;
    Int32 _needed;
    switch (pname) {
#if defined(GL_TEXTURE_ENV_MODE)
        case GL_TEXTURE_ENV_MODE:
#endif // defined(GL_TEXTURE_ENV_MODE)
#if defined(GL_COMBINE_RGB)
        case GL_COMBINE_RGB:
#endif // defined(GL_COMBINE_RGB)
#if defined(GL_COMBINE_ALPHA)
        case GL_COMBINE_ALPHA:
#endif // defined(GL_COMBINE_ALPHA)
            _needed = 1;
            break;
#if defined(GL_TEXTURE_ENV_COLOR)
        case GL_TEXTURE_ENV_COLOR:
#endif // defined(GL_TEXTURE_ENV_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfixed *) (_paramsBase + _bufferOffset);
    }
    glTexEnvxv(
        (GLenum)target,
        (GLenum)pname,
        (GLfixed *)params
    );

    return NOERROR;
}

ECode CGLES10::GlTexImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 border,
    /* [in] */ Int32 format,
    /* [in] */ Int32 type,
    /* [in] */ Elastos::IO::IBuffer* pixels_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLvoid *pixels = (GLvoid *) 0;
    Handle32 pixelsTmp;

    if (pixels_buf) {
        FAIL_RETURN(GetPointer(pixels_buf, &_array, &_remaining, &_bufferOffset, &pixelsTmp));
        pixels = (GLvoid *) pixelsTmp;
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

ECode CGLES10::GlTexParameterf(
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

ECode CGLES10::GlTexParameterx(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    glTexParameterx(
        (GLenum)target,
        (GLenum)pname,
        (GLfixed)param
    );
    return NOERROR;
}

ECode CGLES10::GlTexSubImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 xoffset,
    /* [in] */ Int32 yoffset,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 format,
    /* [in] */ Int32 type,
    /* [in] */ Elastos::IO::IBuffer* pixels_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLvoid *pixels = (GLvoid *) 0;
    Handle32 pixelsTmp;

    if (pixels_buf) {
        FAIL_RETURN(GetPointer(pixels_buf, &_array, &_remaining, &_bufferOffset, &pixelsTmp));
        pixels = (GLvoid *) pixelsTmp;
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

ECode CGLES10::GlTranslatef(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z)
{
    glTranslatef(
        (GLfloat)x,
        (GLfloat)y,
        (GLfloat)z
    );
    return NOERROR;
}

ECode CGLES10::GlTranslatex(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z)
{
    glTranslatex(
        (GLfixed)x,
        (GLfixed)y,
        (GLfixed)z
    );
    return NOERROR;
}

ECode CGLES10::GlVertexPointerBounds(
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Elastos::IO::IBuffer* pointer_buf,
    /* [in] */ Int32 remaining)
{
    GLvoid *pointer = (GLvoid *) 0;
    Handle32 pointerTmp;

    if (pointer_buf) {
        GetDirectBufferPointer(pointer_buf, &pointerTmp);
        pointer = (GLvoid *) pointerTmp;
        if ( ! pointer ) {
            return NOERROR;
        }
    }
    glVertexPointerBounds(
        (GLint)size,
        (GLenum)type,
        (GLsizei)stride,
        (GLvoid *)pointer,
        (GLsizei)remaining
    );
    return NOERROR;
}

ECode CGLES10::GlVertexPointer(
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Elastos::IO::IBuffer* pointer)
{
    Int32 remaining;
    pointer->GetRemaining(&remaining);
    glVertexPointerBounds(
        size,
        type,
        stride,
        pointer,
        remaining
    );
    if (((size == 2) ||
         (size == 3) ||
         (size == 4)) &&
        ((type == GL_FLOAT) ||
         (type == GL_BYTE) ||
         (type == GL_SHORT) ||
         (type == GL_FIXED)) &&
        (stride >= 0)) {
        m_VertexPointer = pointer;
    }
    return NOERROR;
}

ECode CGLES10::GlViewport(
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

ECode CGLES10::GlColorPointerBounds(
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Elastos::IO::IBuffer* pointer,
    /* [in] */ Int32 remaining)
{
    Handle32 address;
    if (pointer) {
        ASSERT_SUCCEEDED(GetDirectBufferPointer(pointer, &address));
        if (address == 0) {
            return NOERROR;
        }
    }
    glColorPointerBounds(
        (GLint)size,
        (GLenum)type,
        (GLsizei)stride,
        reinterpret_cast<GLvoid *>(address),
        (GLsizei)remaining
    );
    return NOERROR;
}

ECode CGLES10::GetDirectBufferPointer(
    /* [in] */ IBuffer* buffer,
    /* [out] */ Handle32* result)
{
    Handle32 primitiveArray;
    buffer->GetPrimitiveArray(&primitiveArray);
    if (primitiveArray != 0) {
        Int32 position, elementSizeShift;
        buffer->GetPosition(&position);
        buffer->GetElementSizeShift(&elementSizeShift);
        primitiveArray += position << elementSizeShift;
    } else {
        LOGD("GetDirectBufferPointer: Must use a native order direct Buffer")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *result = primitiveArray;
    return NOERROR;
}

ECode CGLES10::GetPointer(
    /* [in] */ IBuffer* buffer,
    /* [in, out] */ Handle32* array,
    /* [in, out] */ Int32* remaining,
    /* [in, out] */ Int32* offset,
    /* [out] */ Handle32* rst)
{
    Int32 position;
    Int32 limit;
    Int32 elementSizeShift;
    Int64 pointer;
    buffer->GetPosition(&position);
    buffer->GetLimit(&limit);
    buffer->GetElementSizeShift(&elementSizeShift);
    *remaining = (limit - position) << elementSizeShift;

    AutoPtr<INIOAccessHelper> helper;
    CNIOAccessHelper::AcquireSingleton((INIOAccessHelper**)&helper);

    helper->GetBasePointer(buffer, &pointer);
    if (pointer != 0L) {
        *array = 0;
        *rst = (Handle32)pointer;
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

Int32 CGLES10::GetNumCompressedTextureFormats()
{
    int numCompressedTextureFormats = 0;
    glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &numCompressedTextureFormats);
    return numCompressedTextureFormats;
}

} // namespace Opengl
} // namespace Droid
} // namespace Elastos

