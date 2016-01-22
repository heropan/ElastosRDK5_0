#include "Elastos.Droid.View.h"
#include "elastos/droid/opengl/GLES30.h"

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <utils/misc.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::IO::CNIOAccess;
using Elastos::IO::INIOAccess;

/* special calls implemented in Android's GLES wrapper used to more
 * efficiently bound-check passed arrays */
extern "C" {
#ifdef GL_VERSION_ES_CM_1_1
GL_API void GL_APIENTRY glColorPointerBounds(GLint size, GLenum type, GLsizei stride,
        const GLvoid *ptr, GLsizei count);
GL_API void GL_APIENTRY glNormalPointerBounds(GLenum type, GLsizei stride,
        const GLvoid *pointer, GLsizei count);
GL_API void GL_APIENTRY glTexCoordPointerBounds(GLint size, GLenum type,
        GLsizei stride, const GLvoid *pointer, GLsizei count);
GL_API void GL_APIENTRY glVertexPointerBounds(GLint size, GLenum type,
        GLsizei stride, const GLvoid *pointer, GLsizei count);
GL_API void GL_APIENTRY glPointSizePointerOESBounds(GLenum type,
        GLsizei stride, const GLvoid *pointer, GLsizei count);
GL_API void GL_APIENTRY glMatrixIndexPointerOESBounds(GLint size, GLenum type,
        GLsizei stride, const GLvoid *pointer, GLsizei count);
GL_API void GL_APIENTRY glWeightPointerOESBounds(GLint size, GLenum type,
        GLsizei stride, const GLvoid *pointer, GLsizei count);
#endif
#ifdef GL_ES_VERSION_2_0
static void glVertexAttribPointerBounds(GLuint indx, GLint size, GLenum type,
        GLboolean normalized, GLsizei stride, const GLvoid *pointer, GLsizei count) {
    glVertexAttribPointer(indx, size, type, normalized, stride, pointer);
}
#endif
#ifdef GL_ES_VERSION_3_0
static void glVertexAttribIPointerBounds(GLuint indx, GLint size, GLenum type,
        GLsizei stride, const GLvoid *pointer, GLsizei count) {
    glVertexAttribIPointer(indx, size, type, stride, pointer);
}
#endif
}

static ECode GetPointer(
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
        *rst = (Handle64)(pointer);
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

static ECode GetDirectBufferPointer(
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
        SLOGGERD("GLES30", "GetDirectBufferPointer: Must use a native order direct Buffer")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *result = effectiveDirectAddress;
    return NOERROR;
}

// --------------------------------------------------------------------------

/*
 * returns the number of values glGet returns for a given pname.
 *
 * The code below is written such that pnames requiring only one values
 * are the default (and are not explicitely tested for). This makes the
 * checking code much shorter/readable/efficient.
 *
 * This means that unknown pnames (e.g.: extensions) will default to 1. If
 * that unknown pname needs more than 1 value, then the validation check
 * is incomplete and the app may crash if it passed the wrong number params.
 */
static int getNeededCount(GLint pname) {
    int needed = 1;
#ifdef GL_ES_VERSION_2_0
    // GLES 2.x pnames
    switch (pname) {
        case GL_ALIASED_LINE_WIDTH_RANGE:
        case GL_ALIASED_POINT_SIZE_RANGE:
            needed = 2;
            break;

        case GL_BLEND_COLOR:
        case GL_COLOR_CLEAR_VALUE:
        case GL_COLOR_WRITEMASK:
        case GL_SCISSOR_BOX:
        case GL_VIEWPORT:
            needed = 4;
            break;

        case GL_COMPRESSED_TEXTURE_FORMATS:
            glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &needed);
            break;

        case GL_SHADER_BINARY_FORMATS:
            glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &needed);
            break;
    }
#endif

#ifdef GL_VERSION_ES_CM_1_1
    // GLES 1.x pnames
    switch (pname) {
        case GL_ALIASED_LINE_WIDTH_RANGE:
        case GL_ALIASED_POINT_SIZE_RANGE:
        case GL_DEPTH_RANGE:
        case GL_SMOOTH_LINE_WIDTH_RANGE:
        case GL_SMOOTH_POINT_SIZE_RANGE:
            needed = 2;
            break;

        case GL_CURRENT_NORMAL:
        case GL_POINT_DISTANCE_ATTENUATION:
            needed = 3;
            break;

        case GL_COLOR_CLEAR_VALUE:
        case GL_COLOR_WRITEMASK:
        case GL_CURRENT_COLOR:
        case GL_CURRENT_TEXTURE_COORDS:
        case GL_FOG_COLOR:
        case GL_LIGHT_MODEL_AMBIENT:
        case GL_SCISSOR_BOX:
        case GL_VIEWPORT:
            needed = 4;
            break;

        case GL_MODELVIEW_MATRIX:
        case GL_PROJECTION_MATRIX:
        case GL_TEXTURE_MATRIX:
            needed = 16;
            break;

        case GL_COMPRESSED_TEXTURE_FORMATS:
            glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &needed);
            break;
    }
#endif
    return needed;
}

template <typename ATYPE, typename CTYPE, void GET(GLenum, CTYPE*)>
static ECode
get
  (Int32 pname, ArrayOf<ATYPE>* params_ref, Int32 offset) {
    Int32 _exception = 0;
    ECode _exceptionType = NOERROR;
    const char * _exceptionMessage;
    CTYPE *params_base = (CTYPE *) 0;
    Int32 _remaining;
    CTYPE *params = (CTYPE *) 0;
    Int32 _needed = 0;

    if (!params_ref) {
        _exception = 1;
        _exceptionType = E_ILLEGAL_ARGUMENT_EXCEPTION;
        _exceptionMessage = "params == null";
        goto exit;
    }
    if (offset < 0) {
        _exception = 1;
        _exceptionType = E_ILLEGAL_ARGUMENT_EXCEPTION;
        _exceptionMessage = "offset < 0";
        goto exit;
    }
    _remaining = params_ref->GetLength() - offset;
    _needed = getNeededCount(pname);
    // if we didn't find this pname, we just assume the user passed
    // an array of the right size -- this might happen with extensions
    // or if we forget an enum here.
    if (_remaining < _needed) {
        _exception = 1;
        _exceptionType = E_ILLEGAL_ARGUMENT_EXCEPTION;
        _exceptionMessage = "length - offset < needed";
        goto exit;
    }
    params_base = (CTYPE *)(params_ref->GetPayload());
    params = params_base + offset;

    GET(
        (GLenum)pname,
        (CTYPE *)params
    );

exit:
    if (_exception) {
        SLOGGERD("CGLES30", _exceptionMessage)
    }
    return _exceptionType;
}


template <typename CTYPE, void GET(GLenum, CTYPE*)>
static ECode
getarray
  (Int32 pname, IBuffer* params_buf) {
    Int32 _exception = 0;
    ECode _exceptionType = NOERROR;
    const char * _exceptionMessage;
    Handle64 _array = (Handle64) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    Handle64 data;
    CTYPE *params = (CTYPE *) 0;
    Int32 _needed = 0;

    if (params_buf)
    {
        FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &data))
        params = (CTYPE*)data;
    }
    _remaining /= sizeof(CTYPE);    // convert from bytes to item count
    _needed = getNeededCount(pname);
    // if we didn't find this pname, we just assume the user passed
    // an array of the right size -- this might happen with extensions
    // or if we forget an enum here.
    if (_needed>0 && _remaining < _needed) {
        _exception = 1;
        _exceptionType = E_ILLEGAL_ARGUMENT_EXCEPTION;
        _exceptionMessage = "remaining() < needed";
        goto exit;
    }

    if (params_buf && params == NULL) {
        char * _pBase = reinterpret_cast<char *>(_array);
        params = (CTYPE *) (_pBase + _bufferOffset);
    }

    GET(
        (GLenum)pname,
        (CTYPE *)params
    );

exit:
    if (_exception) {
        SLOGGERD("CGLES30", _exceptionMessage)
    }
    return _exceptionType;
}

namespace Elastos {
namespace Droid {
namespace Opengl {

CAR_INTERFACE_IMPL(GLES30, GLES20, IGLES30)

ECode GLES30::GlReadBuffer(
    /* [in] */ Int32 mode)
{
    glReadBuffer(
        (GLenum)mode
    );
    return NOERROR;
}

// C function void glDrawRangeElements ( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices )

ECode GLES30::GlDrawRangeElements(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 count,
    /* [in] */ Int32 type,
    /* [in] */ IBuffer* indices_buf)
{
    Handle64 _array = 0;
    Int32 _bufferOffset = 0;
    Int32 _remaining;
    GLvoid *indices = (GLvoid *) 0;

    if (indices_buf) {
        Handle64 tmp;
        FAIL_RETURN(GetPointer(indices_buf, &_array, &_remaining, &_bufferOffset, &tmp));
        indices = (GLvoid *) tmp;
    }
    if (indices_buf && indices == NULL) {
        char * _indicesBase = reinterpret_cast<char *>(_array);
        indices = (GLvoid *) (_indicesBase + _bufferOffset);
    }

    glDrawRangeElements(
        (GLenum)mode,
        (GLuint)start,
        (GLuint)end,
        (GLsizei)count,
        (GLenum)type,
        (GLvoid *)indices
    );
    return NOERROR;
}

// C function void glDrawRangeElements ( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, GLsizei offset )

ECode GLES30::GlDrawRangeElements(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 count,
    /* [in] */ Int32 type,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glTexImage3D ( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels )

ECode GLES30::GlTexImage3D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 depth,
    /* [in] */ Int32 border,
    /* [in] */ Int32 format,
    /* [in] */ Int32 type,
    /* [in] */ IBuffer* pixels)
{
    return NOERROR;
}

// C function void glTexImage3D ( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, GLsizei offset )

ECode GLES30::GlTexImage3D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 depth,
    /* [in] */ Int32 border,
    /* [in] */ Int32 format,
    /* [in] */ Int32 type,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glTexSubImage3D ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels )

ECode GLES30::GlTexSubImage3D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 xoffset,
    /* [in] */ Int32 yoffset,
    /* [in] */ Int32 zoffset,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 depth,
    /* [in] */ Int32 format,
    /* [in] */ Int32 type,
    /* [in] */ IBuffer* pixels)
{
    return NOERROR;
}

// C function void glTexSubImage3D ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei offset )

ECode GLES30::GlTexSubImage3D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 xoffset,
    /* [in] */ Int32 yoffset,
    /* [in] */ Int32 zoffset,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 depth,
    /* [in] */ Int32 format,
    /* [in] */ Int32 type,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glCopyTexSubImage3D ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height )

ECode GLES30::GlCopyTexSubImage3D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 xoffset,
    /* [in] */ Int32 yoffset,
    /* [in] */ Int32 zoffset,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return NOERROR;
}

// C function void glCompressedTexImage3D ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data )

ECode GLES30::GlCompressedTexImage3D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 depth,
    /* [in] */ Int32 border,
    /* [in] */ Int32 imageSize,
    /* [in] */ IBuffer* data)
{
    return NOERROR;
}

// C function void glCompressedTexImage3D ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, GLsizei offset )

ECode GLES30::GlCompressedTexImage3D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 depth,
    /* [in] */ Int32 border,
    /* [in] */ Int32 imageSize,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glCompressedTexSubImage3D ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data )

ECode GLES30::GlCompressedTexSubImage3D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 xoffset,
    /* [in] */ Int32 yoffset,
    /* [in] */ Int32 zoffset,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 depth,
    /* [in] */ Int32 format,
    /* [in] */ Int32 imageSize,
    /* [in] */ IBuffer* data)
{
    return NOERROR;
}

// C function void glCompressedTexSubImage3D ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, GLsizei offset )

ECode GLES30::GlCompressedTexSubImage3D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 xoffset,
    /* [in] */ Int32 yoffset,
    /* [in] */ Int32 zoffset,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 depth,
    /* [in] */ Int32 format,
    /* [in] */ Int32 imageSize,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGenQueries ( GLsizei n, GLuint *ids )

ECode GLES30::GlGenQueries(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* ids,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGenQueries ( GLsizei n, GLuint *ids )

ECode GLES30::GlGenQueries(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* ids)
{
    return NOERROR;
}

// C function void glDeleteQueries ( GLsizei n, const GLuint *ids )

ECode GLES30::GlDeleteQueries(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* ids,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glDeleteQueries ( GLsizei n, const GLuint *ids )

ECode GLES30::GlDeleteQueries(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* ids)
{
    return NOERROR;
}

// C function GLboolean glIsQuery ( GLuint id
ECode GLES30::GlIsQuery(
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

// C function void glBeginQuery ( GLenum target, GLuint id )

ECode GLES30::GlBeginQuery(
    /* [in] */ Int32 target,
    /* [in] */ Int32 id)
{
    return NOERROR;
}

// C function void glEndQuery ( GLenum target )

ECode GLES30::GlEndQuery(
    /* [in] */ Int32 target)
{
    return NOERROR;
}

// C function void glGetQueryiv ( GLenum target, GLenum pname, GLint *params )

ECode GLES30::GlGetQueryiv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetQueryiv ( GLenum target, GLenum pname, GLint *params )

ECode GLES30::GlGetQueryiv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function void glGetQueryObjectuiv ( GLuint id, GLenum pname, GLuint *params )

ECode GLES30::GlGetQueryObjectuiv(
    /* [in] */ Int32 id,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetQueryObjectuiv ( GLuint id, GLenum pname, GLuint *params )

ECode GLES30::GlGetQueryObjectuiv(
    /* [in] */ Int32 id,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function GLboolean glUnmapBuffer ( GLenum target )

ECode GLES30::GlUnmapBuffer(
    /* [in] */ Int32 target,
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

// C function void glGetBufferPointerv ( GLenum target, GLenum pname, GLvoid** params )

ECode GLES30::GlGetBufferPointerv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [out] */ IBuffer** buffer)
{
    return NOERROR;
}

// C function void glDrawBuffers ( GLsizei n, const GLenum *bufs )

ECode GLES30::GlDrawBuffers(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* bufs,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glDrawBuffers ( GLsizei n, const GLenum *bufs )

ECode GLES30::GlDrawBuffers(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* bufs)
{
    return NOERROR;
}

// C function void glUniformMatrix2x3fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES30::GlUniformMatrix2x3fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glUniformMatrix2x3fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES30::GlUniformMatrix2x3fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glUniformMatrix3x2fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES30::GlUniformMatrix3x2fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glUniformMatrix3x2fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES30::GlUniformMatrix3x2fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glUniformMatrix2x4fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES30::GlUniformMatrix2x4fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glUniformMatrix2x4fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES30::GlUniformMatrix2x4fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glUniformMatrix4x2fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES30::GlUniformMatrix4x2fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glUniformMatrix4x2fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES30::GlUniformMatrix4x2fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glUniformMatrix3x4fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES30::GlUniformMatrix3x4fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glUniformMatrix3x4fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES30::GlUniformMatrix3x4fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glUniformMatrix4x3fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES30::GlUniformMatrix4x3fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glUniformMatrix4x3fv ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES30::GlUniformMatrix4x3fv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glBlitFramebuffer ( GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter )

ECode GLES30::GlBlitFramebuffer(
    /* [in] */ Int32 srcX0,
    /* [in] */ Int32 srcY0,
    /* [in] */ Int32 srcX1,
    /* [in] */ Int32 srcY1,
    /* [in] */ Int32 dstX0,
    /* [in] */ Int32 dstY0,
    /* [in] */ Int32 dstX1,
    /* [in] */ Int32 dstY1,
    /* [in] */ Int32 mask,
    /* [in] */ Int32 filter)
{
    return NOERROR;
}

// C function void glRenderbufferStorageMultisample ( GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height )

ECode GLES30::GlRenderbufferStorageMultisample(
    /* [in] */ Int32 target,
    /* [in] */ Int32 samples,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return NOERROR;
}

// C function void glFramebufferTextureLayer ( GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer )

ECode GLES30::GlFramebufferTextureLayer(
    /* [in] */ Int32 target,
    /* [in] */ Int32 attachment,
    /* [in] */ Int32 texture,
    /* [in] */ Int32 level,
    /* [in] */ Int32 layer)
{
    return NOERROR;
}

// C function GLvoid * glMapBufferRange ( GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access )

ECode GLES30::GlMapBufferRange(
    /* [in] */ Int32 target,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Int32 access,
    /* [out] */ IBuffer** buffer)
{
    return NOERROR;
}

// C function void glFlushMappedBufferRange ( GLenum target, GLintptr offset, GLsizeiptr length )

ECode GLES30::GlFlushMappedBufferRange(
    /* [in] */ Int32 target,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return NOERROR;
}

// C function void glBindVertexArray ( GLuint array )

ECode GLES30::GlBindVertexArray(
    /* [in] */ Int32 array)
{
    return NOERROR;
}

// C function void glDeleteVertexArrays ( GLsizei n, const GLuint *arrays )

ECode GLES30::GlDeleteVertexArrays(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* arrays,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glDeleteVertexArrays ( GLsizei n, const GLuint *arrays )

ECode GLES30::GlDeleteVertexArrays(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* arrays)
{
    return NOERROR;
}

// C function void glGenVertexArrays ( GLsizei n, GLuint *arrays )

ECode GLES30::GlGenVertexArrays(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* arrays,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGenVertexArrays ( GLsizei n, GLuint *arrays )

ECode GLES30::GlGenVertexArrays(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* arrays)
{
    return NOERROR;
}

// C function GLboolean glIsVertexArray ( GLuint array )

ECode GLES30::GlIsVertexArray(
    /* [in] */ Int32 array,
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

// C function void glGetIntegeri_v ( GLenum target, GLuint index, GLint *data )

ECode GLES30::GlGetIntegeri_v(
    /* [in] */ Int32 target,
    /* [in] */ Int32 index,
    /* [in] */ ArrayOf<Int32>* data,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetIntegeri_v ( GLenum target, GLuint index, GLint *data )

ECode GLES30::GlGetIntegeri_v(
    /* [in] */ Int32 target,
    /* [in] */ Int32 index,
    /* [in] */ IInt32Buffer* data)
{
    return NOERROR;
}

// C function void glBeginTransformFeedback ( GLenum primitiveMode )

ECode GLES30::GlBeginTransformFeedback(
    /* [in] */ Int32 primitiveMode)
{
    return NOERROR;
}

// C function void glEndTransformFeedback ( void )

ECode GLES30::GlEndTransformFeedback()
{
    return NOERROR;
}

// C function void glBindBufferRange ( GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size )

ECode GLES30::GlBindBufferRange(
    /* [in] */ Int32 target,
    /* [in] */ Int32 index,
    /* [in] */ Int32 buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 size)
{
    return NOERROR;
}

// C function void glBindBufferBase ( GLenum target, GLuint index, GLuint buffer )

ECode GLES30::GlBindBufferBase(
    /* [in] */ Int32 target,
    /* [in] */ Int32 index,
    /* [in] */ Int32 buffer)
{
    return NOERROR;
}

// C function void glTransformFeedbackVaryings ( GLuint program, GLsizei count, const GLchar *varyings, GLenum bufferMode )

ECode GLES30::GlTransformFeedbackVaryings(
    /* [in] */ Int32 program,
    /* [in] */ ArrayOf<String>* varyings,
    /* [in] */ Int32 bufferMode)
{
    return NOERROR;
}

// C function void glGetTransformFeedbackVarying ( GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name )

ECode GLES30::GlGetTransformFeedbackVarying(
    /* [in] */ Int32 program,
    /* [in] */ Int32 index,
    /* [in] */ Int32 bufsize,
    /* [in] */ ArrayOf<Int32>* length,
    /* [in] */ Int32 lengthOffset,
    /* [in] */ ArrayOf<Int32>* size,
    /* [in] */ Int32 sizeOffset,
    /* [in] */ ArrayOf<Int32>* type,
    /* [in] */ Int32 typeOffset,
    /* [in] */ ArrayOf<Byte>* name,
    /* [in] */ Int32 nameOffset)
{
    return NOERROR;
}

// C function void glGetTransformFeedbackVarying ( GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name )

ECode GLES30::GlGetTransformFeedbackVarying(
    /* [in] */ Int32 program,
    /* [in] */ Int32 index,
    /* [in] */ Int32 bufsize,
    /* [in] */ IInt32Buffer* length,
    /* [in] */ IInt32Buffer* size,
    /* [in] */ IInt32Buffer* type,
    /* [in] */ Byte name)
{
    return NOERROR;
}

// C function void glGetTransformFeedbackVarying ( GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name )

ECode GLES30::GlGetTransformFeedbackVarying(
    /* [in] */ Int32 program,
    /* [in] */ Int32 index,
    /* [in] */ ArrayOf<Int32>* size,
    /* [in] */ Int32 sizeOffset,
    /* [in] */ ArrayOf<Int32>* type,
    /* [in] */ Int32 typeOffset,
    /* [out] */ String* result)
{
    return NOERROR;
}

// C function void glGetTransformFeedbackVarying ( GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name )

ECode GLES30::GlGetTransformFeedbackVarying(
    /* [in] */ Int32 program,
    /* [in] */ Int32 index,
    /* [in] */ IInt32Buffer* size,
    /* [in] */ IInt32Buffer* type,
    /* [out] */ String* result)
{
    return NOERROR;
}

// C function void glVertexAttribIPointer ( GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer )

ECode GLES30::GlVertexAttribIPointer(
    /* [in] */ Int32 index,
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ IBuffer* pointer)
{
    return NOERROR;
}

// C function void glVertexAttribIPointer ( GLuint index, GLint size, GLenum type, GLsizei stride, GLsizei offset )

ECode GLES30::GlVertexAttribIPointer(
    /* [in] */ Int32 index,
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetVertexAttribIiv ( GLuint index, GLenum pname, GLint *params )

ECode GLES30::GlGetVertexAttribIiv(
    /* [in] */ Int32 index,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetVertexAttribIiv ( GLuint index, GLenum pname, GLint *params )

ECode GLES30::GlGetVertexAttribIiv(
    /* [in] */ Int32 index,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function void glGetVertexAttribIuiv ( GLuint index, GLenum pname, GLuint *params )

ECode GLES30::GlGetVertexAttribIuiv(
    /* [in] */ Int32 index,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetVertexAttribIuiv ( GLuint index, GLenum pname, GLuint *params )

ECode GLES30::GlGetVertexAttribIuiv(
    /* [in] */ Int32 index,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function void glVertexAttribI4i ( GLuint index, GLint x, GLint y, GLint z, GLint w )

ECode GLES30::GlVertexAttribI4i(
    /* [in] */ Int32 index,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ Int32 w)
{
    return NOERROR;
}

// C function void glVertexAttribI4ui ( GLuint index, GLuint x, GLuint y, GLuint z, GLuint w )

ECode GLES30::GlVertexAttribI4ui(
    /* [in] */ Int32 index,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ Int32 w)
{
    return NOERROR;
}

// C function void glVertexAttribI4iv ( GLuint index, const GLint *v )

ECode GLES30::GlVertexAttribI4iv(
    /* [in] */ Int32 index,
    /* [in] */ ArrayOf<Int32>* v,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glVertexAttribI4iv ( GLuint index, const GLint *v )

ECode GLES30::GlVertexAttribI4iv(
    /* [in] */ Int32 index,
    /* [in] */ IInt32Buffer* v)
{
    return NOERROR;
}

// C function void glVertexAttribI4uiv ( GLuint index, const GLuint *v )

ECode GLES30::GlVertexAttribI4uiv(
    /* [in] */ Int32 index,
    /* [in] */ ArrayOf<Int32>* v,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glVertexAttribI4uiv ( GLuint index, const GLuint *v )

ECode GLES30::GlVertexAttribI4uiv(
    /* [in] */ Int32 index,
    /* [in] */ IInt32Buffer* v)
{
    return NOERROR;
}

// C function void glGetUniformuiv ( GLuint program, GLint location, GLuint *params )

ECode GLES30::GlGetUniformuiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetUniformuiv ( GLuint program, GLint location, GLuint *params )

ECode GLES30::GlGetUniformuiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function GLint glGetFragDataLocation ( GLuint program, const GLchar *name )

ECode GLES30::GlGetFragDataLocation(
    /* [in] */ Int32 program,
    /* [in] */ const String& name,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

// C function void glUniform1ui ( GLint location, GLuint v0 )

ECode GLES30::GlUniform1ui(
    /* [in] */ Int32 location,
    /* [in] */ Int32 v0)
{
    return NOERROR;
}

// C function void glUniform2ui ( GLint location, GLuint v0, GLuint v1 )

ECode GLES30::GlUniform2ui(
    /* [in] */ Int32 location,
    /* [in] */ Int32 v0,
    /* [in] */ Int32 v1)
{
    return NOERROR;
}

// C function void glUniform3ui ( GLint location, GLuint v0, GLuint v1, GLuint v2 )

ECode GLES30::GlUniform3ui(
    /* [in] */ Int32 location,
    /* [in] */ Int32 v0,
    /* [in] */ Int32 v1,
    /* [in] */ Int32 v2)
{
    return NOERROR;
}

// C function void glUniform4ui ( GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3 )

ECode GLES30::GlUniform4ui(
    /* [in] */ Int32 location,
    /* [in] */ Int32 v0,
    /* [in] */ Int32 v1,
    /* [in] */ Int32 v2,
    /* [in] */ Int32 v3)
{
    return NOERROR;
}

// C function void glUniform1uiv ( GLint location, GLsizei count, const GLuint *value )

ECode GLES30::GlUniform1uiv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glUniform1uiv ( GLint location, GLsizei count, const GLuint *value )

ECode GLES30::GlUniform1uiv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* value)
{
    return NOERROR;
}

// C function void glUniform2uiv ( GLint location, GLsizei count, const GLuint *value )

ECode GLES30::GlUniform2uiv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glUniform2uiv ( GLint location, GLsizei count, const GLuint *value )

ECode GLES30::GlUniform2uiv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* value)
{
    return NOERROR;
}

// C function void glUniform3uiv ( GLint location, GLsizei count, const GLuint *value )

ECode GLES30::GlUniform3uiv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glUniform3uiv ( GLint location, GLsizei count, const GLuint *value )

ECode GLES30::GlUniform3uiv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* value)
{
    return NOERROR;
}

// C function void glUniform4uiv ( GLint location, GLsizei count, const GLuint *value )

ECode GLES30::GlUniform4uiv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glUniform4uiv ( GLint location, GLsizei count, const GLuint *value )

ECode GLES30::GlUniform4uiv(
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* value)
{
    return NOERROR;
}

// C function void glClearBufferiv ( GLenum buffer, GLint drawbuffer, const GLint *value )

ECode GLES30::GlClearBufferiv(
    /* [in] */ Int32 buffer,
    /* [in] */ Int32 drawbuffer,
    /* [in] */ ArrayOf<Int32>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glClearBufferiv ( GLenum buffer, GLint drawbuffer, const GLint *value )

ECode GLES30::GlClearBufferiv(
    /* [in] */ Int32 buffer,
    /* [in] */ Int32 drawbuffer,
    /* [in] */ IInt32Buffer* value)
{
    return NOERROR;
}

// C function void glClearBufferuiv ( GLenum buffer, GLint drawbuffer, const GLuint *value )

ECode GLES30::GlClearBufferuiv(
    /* [in] */ Int32 buffer,
    /* [in] */ Int32 drawbuffer,
    /* [in] */ ArrayOf<Int32>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glClearBufferuiv ( GLenum buffer, GLint drawbuffer, const GLuint *value )

ECode GLES30::GlClearBufferuiv(
    /* [in] */ Int32 buffer,
    /* [in] */ Int32 drawbuffer,
    /* [in] */ IInt32Buffer* value)
{
    return NOERROR;
}

// C function void glClearBufferfv ( GLenum buffer, GLint drawbuffer, const GLfloat *value )

ECode GLES30::GlClearBufferfv(
    /* [in] */ Int32 buffer,
    /* [in] */ Int32 drawbuffer,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glClearBufferfv ( GLenum buffer, GLint drawbuffer, const GLfloat *value )

ECode GLES30::GlClearBufferfv(
    /* [in] */ Int32 buffer,
    /* [in] */ Int32 drawbuffer,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glClearBufferfi ( GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil )

ECode GLES30::GlClearBufferfi(
    /* [in] */ Int32 buffer,
    /* [in] */ Int32 drawbuffer,
    /* [in] */ Float depth,
    /* [in] */ Int32 stencil)
{
    return NOERROR;
}

// C function const GLubyte * glGetStringi ( GLenum name, GLuint index )

ECode GLES30::GlGetStringi(
    /* [in] */ Int32 name,
    /* [in] */ Int32 index,
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

// C function void glCopyBufferSubData ( GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size )

ECode GLES30::GlCopyBufferSubData(
    /* [in] */ Int32 readTarget,
    /* [in] */ Int32 writeTarget,
    /* [in] */ Int32 readOffset,
    /* [in] */ Int32 writeOffset,
    /* [in] */ Int32 size)
{
    return NOERROR;
}

// C function void glGetUniformIndices ( GLuint program, GLsizei uniformCount, const GLchar *const *uniformNames, GLuint *uniformIndices )

ECode GLES30::GlGetUniformIndices(
    /* [in] */ Int32 program,
    /* [in] */ ArrayOf<String>* uniformNames,
    /* [in] */ ArrayOf<Int32>* uniformIndices,
    /* [in] */ Int32 uniformIndicesOffset)
{
    return NOERROR;
}

// C function void glGetUniformIndices ( GLuint program, GLsizei uniformCount, const GLchar *const *uniformNames, GLuint *uniformIndices )

ECode GLES30::GlGetUniformIndices(
    /* [in] */ Int32 program,
    /* [in] */ ArrayOf<String>* uniformNames,
    /* [in] */ IInt32Buffer* uniformIndices)
{
    return NOERROR;
}

// C function void glGetActiveUniformsiv ( GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params )

ECode GLES30::GlGetActiveUniformsiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 uniformCount,
    /* [in] */ ArrayOf<Int32>* uniformIndices,
    /* [in] */ Int32 uniformIndicesOffset,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 paramsOffset)
{
    return NOERROR;
}

// C function void glGetActiveUniformsiv ( GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params )

ECode GLES30::GlGetActiveUniformsiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 uniformCount,
    /* [in] */ IInt32Buffer* uniformIndices,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function GLuint glGetUniformBlockIndex ( GLuint program, const GLchar *uniformBlockName )

ECode GLES30::GlGetUniformBlockIndex(
    /* [in] */ Int32 program,
    /* [in] */ const String& uniformBlockName,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

// C function void glGetActiveUniformBlockiv ( GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params )

ECode GLES30::GlGetActiveUniformBlockiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 uniformBlockIndex,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetActiveUniformBlockiv ( GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params )

ECode GLES30::GlGetActiveUniformBlockiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 uniformBlockIndex,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function void glGetActiveUniformBlockName ( GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName )

ECode GLES30::GlGetActiveUniformBlockName(
    /* [in] */ Int32 program,
    /* [in] */ Int32 uniformBlockIndex,
    /* [in] */ Int32 bufSize,
    /* [in] */ ArrayOf<Int32>* length,
    /* [in] */ Int32 lengthOffset,
    /* [in] */ ArrayOf<Byte>* uniformBlockName,
    /* [in] */ Int32 uniformBlockNameOffset)
{
    return NOERROR;
}

// C function void glGetActiveUniformBlockName ( GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName )

ECode GLES30::GlGetActiveUniformBlockName(
    /* [in] */ Int32 program,
    /* [in] */ Int32 uniformBlockIndex,
    /* [in] */ IBuffer* length,
    /* [in] */ IBuffer* uniformBlockName)
{
    return NOERROR;
}

// C function void glGetActiveUniformBlockName ( GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName )

ECode GLES30::GlGetActiveUniformBlockName(
    /* [in] */ Int32 program,
    /* [in] */ Int32 uniformBlockIndex,
    /* [out] */ String* result)
{
    return NOERROR;
}

// C function void glUniformBlockBinding ( GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding )

ECode GLES30::GlUniformBlockBinding(
    /* [in] */ Int32 program,
    /* [in] */ Int32 uniformBlockIndex,
    /* [in] */ Int32 uniformBlockBinding)
{
    return NOERROR;
}

// C function void glDrawArraysInstanced ( GLenum mode, GLint first, GLsizei count, GLsizei instanceCount )

ECode GLES30::GlDrawArraysInstanced(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 first,
    /* [in] */ Int32 count,
    /* [in] */ Int32 instanceCount)
{
    return NOERROR;
}

// C function void glDrawElementsInstanced ( GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei instanceCount )

ECode GLES30::GlDrawElementsInstanced(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 count,
    /* [in] */ Int32 type,
    /* [in] */ IBuffer* indices,
    /* [in] */ Int32 instanceCount)
{
    return NOERROR;
}

// C function void glDrawElementsInstanced ( GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei instanceCount )

ECode GLES30::GlDrawElementsInstanced(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 count,
    /* [in] */ Int32 type,
    /* [in] */ Int32 indicesOffset,
    /* [in] */ Int32 instanceCount)
{
    return NOERROR;
}

// C function GLsync glFenceSync ( GLenum condition, GLbitfield flags )

ECode GLES30::GlFenceSync(
    /* [in] */ Int32 condition,
    /* [in] */ Int32 flags,
    /* [out] */ Int64* result)
{
    return NOERROR;
}

// C function GLboolean glIsSync ( GLsync sync )

ECode GLES30::GlIsSync(
    /* [in] */ Int64 sync,
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

// C function void glDeleteSync ( GLsync sync )

ECode GLES30::GlDeleteSync(
    /* [in] */ Int64 sync)
{
    return NOERROR;
}

// C function GLenum glClientWaitSync ( GLsync sync, GLbitfield flags, GLuint64 timeout )

ECode GLES30::GlClientWaitSync(
    /* [in] */ Int64 sync,
    /* [in] */ Int32 flags,
    /* [in] */ Int64 timeout,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

// C function void glWaitSync ( GLsync sync, GLbitfield flags, GLuint64 timeout )

ECode GLES30::GlWaitSync(
    /* [in] */ Int64 sync,
    /* [in] */ Int32 flags,
    /* [in] */ Int64 timeout)
{
    return NOERROR;
}

// C function void glGetInteger64v ( GLenum pname, GLint64 *params )

ECode GLES30::GlGetInteger64v(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int64>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetInteger64v ( GLenum pname, GLint64 *params )

ECode GLES30::GlGetInteger64v(
    /* [in] */ Int32 pname,
    /* [in] */ IInt64Buffer*  params)
{
    return NOERROR;
}

// C function void glGetSynciv ( GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values )

ECode GLES30::GlGetSynciv(
    /* [in] */ Int64 sync,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 bufSize,
    /* [in] */ ArrayOf<Int32>* length,
    /* [in] */ Int32 lengthOffset,
    /* [in] */ ArrayOf<Int32>* values,
    /* [in] */ Int32 valuesOffset)
{
    return NOERROR;
}

// C function void glGetSynciv ( GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values )

ECode GLES30::GlGetSynciv(
    /* [in] */ Int64 sync,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 bufSize,
    /* [in] */ IInt32Buffer* length,
    /* [in] */ IInt32Buffer* values)
{
    return NOERROR;
}

// C function void glGetInteger64i_v ( GLenum target, GLuint index, GLint64 *data )

ECode GLES30::GlGetInteger64i_v(
    /* [in] */ Int32 target,
    /* [in] */ Int32 index,
    /* [in] */ ArrayOf<Int64>* data,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetInteger64i_v ( GLenum target, GLuint index, GLint64 *data )

ECode GLES30::GlGetInteger64i_v(
    /* [in] */ Int32 target,
    /* [in] */ Int32 index,
    /* [in] */ IInt64Buffer*  data)
{
    return NOERROR;
}

// C function void glGetBufferParameteri64v ( GLenum target, GLenum pname, GLint64 *params )

ECode GLES30::GlGetBufferParameteri64v(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int64>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetBufferParameteri64v ( GLenum target, GLenum pname, GLint64 *params )

ECode GLES30::GlGetBufferParameteri64v(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ IInt64Buffer*  params)
{
    return NOERROR;
}

// C function void glGenSamplers ( GLsizei count, GLuint *samplers )

ECode GLES30::GlGenSamplers(
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* samplers,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGenSamplers ( GLsizei count, GLuint *samplers )

ECode GLES30::GlGenSamplers(
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* samplers)
{
    return NOERROR;
}

// C function void glDeleteSamplers ( GLsizei count, const GLuint *samplers )

ECode GLES30::GlDeleteSamplers(
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* samplers,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glDeleteSamplers ( GLsizei count, const GLuint *samplers )

ECode GLES30::GlDeleteSamplers(
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* samplers)
{
    return NOERROR;
}

// C function GLboolean glIsSampler ( GLuint sampler )

ECode GLES30::GlIsSampler(
    /* [in] */ Int32 sampler,
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

// C function void glBindSampler ( GLuint unit, GLuint sampler )

ECode GLES30::GlBindSampler(
    /* [in] */ Int32 unit,
    /* [in] */ Int32 sampler)
{
    return NOERROR;
}

// C function void glSamplerParameteri ( GLuint sampler, GLenum pname, GLint param )

ECode GLES30::GlSamplerParameteri(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    return NOERROR;
}

// C function void glSamplerParameteriv ( GLuint sampler, GLenum pname, const GLint *param )

ECode GLES30::GlSamplerParameteriv(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glSamplerParameteriv ( GLuint sampler, GLenum pname, const GLint *param )

ECode GLES30::GlSamplerParameteriv(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* param)
{
    return NOERROR;
}

// C function void glSamplerParameterf ( GLuint sampler, GLenum pname, GLfloat param )

ECode GLES30::GlSamplerParameterf(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ Float param)
{
    return NOERROR;
}

// C function void glSamplerParameterfv ( GLuint sampler, GLenum pname, const GLfloat *param )

ECode GLES30::GlSamplerParameterfv(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* param,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glSamplerParameterfv ( GLuint sampler, GLenum pname, const GLfloat *param )

ECode GLES30::GlSamplerParameterfv(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ IFloatBuffer* param)
{
    return NOERROR;
}

// C function void glGetSamplerParameteriv ( GLuint sampler, GLenum pname, GLint *params )

ECode GLES30::GlGetSamplerParameteriv(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetSamplerParameteriv ( GLuint sampler, GLenum pname, GLint *params )

ECode GLES30::GlGetSamplerParameteriv(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function void glGetSamplerParameterfv ( GLuint sampler, GLenum pname, GLfloat *params )

ECode GLES30::GlGetSamplerParameterfv(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetSamplerParameterfv ( GLuint sampler, GLenum pname, GLfloat *params )

ECode GLES30::GlGetSamplerParameterfv(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ IFloatBuffer* params)
{
    return NOERROR;
}

// C function void glVertexAttribDivisor ( GLuint index, GLuint divisor )

ECode GLES30::GlVertexAttribDivisor(
    /* [in] */ Int32 index,
    /* [in] */ Int32 divisor)
{
    return NOERROR;
}

// C function void glBindTransformFeedback ( GLenum target, GLuint id )

ECode GLES30::GlBindTransformFeedback(
    /* [in] */ Int32 target,
    /* [in] */ Int32 id)
{
    return NOERROR;
}

// C function void glDeleteTransformFeedbacks ( GLsizei n, const GLuint *ids )

ECode GLES30::GlDeleteTransformFeedbacks(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* ids,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glDeleteTransformFeedbacks ( GLsizei n, const GLuint *ids )

ECode GLES30::GlDeleteTransformFeedbacks(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* ids)
{
    return NOERROR;
}

// C function void glGenTransformFeedbacks ( GLsizei n, GLuint *ids )

ECode GLES30::GlGenTransformFeedbacks(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* ids,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGenTransformFeedbacks ( GLsizei n, GLuint *ids )

ECode GLES30::GlGenTransformFeedbacks(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* ids)
{
    return NOERROR;
}

// C function GLboolean glIsTransformFeedback ( GLuint id )

ECode GLES30::GlIsTransformFeedback(
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

// C function void glPauseTransformFeedback ( void )

ECode GLES30::GlPauseTransformFeedback()
{
    return NOERROR;
}

// C function void glResumeTransformFeedback ( void )

ECode GLES30::GlResumeTransformFeedback()
{
    return NOERROR;
}

// C function void glGetProgramBinary ( GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary )

ECode GLES30::GlGetProgramBinary(
    /* [in] */ Int32 program,
    /* [in] */ Int32 bufSize,
    /* [in] */ ArrayOf<Int32>* length,
    /* [in] */ Int32 lengthOffset,
    /* [in] */ ArrayOf<Int32>* binaryFormat,
    /* [in] */ Int32 binaryFormatOffset,
    /* [in] */ IBuffer* binary)
{
    return NOERROR;
}

// C function void glGetProgramBinary ( GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary )

ECode GLES30::GlGetProgramBinary(
    /* [in] */ Int32 program,
    /* [in] */ Int32 bufSize,
    /* [in] */ IInt32Buffer* length,
    /* [in] */ IInt32Buffer* binaryFormat,
    /* [in] */ IBuffer* binary)
{
    return NOERROR;
}

// C function void glProgramBinary ( GLuint program, GLenum binaryFormat, const GLvoid *binary, GLsizei length )

ECode GLES30::GlProgramBinary(
    /* [in] */ Int32 program,
    /* [in] */ Int32 binaryFormat,
    /* [in] */ IBuffer* binary,
    /* [in] */ Int32 length)
{
    return NOERROR;
}

// C function void glProgramParameteri ( GLuint program, GLenum pname, GLint value )

ECode GLES30::GlProgramParameteri(
    /* [in] */ Int32 program,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 value)
{
    return NOERROR;
}

// C function void glInvalidateFramebuffer ( GLenum target, GLsizei numAttachments, const GLenum *attachments )

ECode GLES30::GlInvalidateFramebuffer(
    /* [in] */ Int32 target,
    /* [in] */ Int32 numAttachments,
    /* [in] */ ArrayOf<Int32>* attachments,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glInvalidateFramebuffer ( GLenum target, GLsizei numAttachments, const GLenum *attachments )

ECode GLES30::GlInvalidateFramebuffer(
    /* [in] */ Int32 target,
    /* [in] */ Int32 numAttachments,
    /* [in] */ IInt32Buffer* attachments)
{
    return NOERROR;
}

// C function void glInvalidateSubFramebuffer ( GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height )

ECode GLES30::GlInvalidateSubFramebuffer(
    /* [in] */ Int32 target,
    /* [in] */ Int32 numAttachments,
    /* [in] */ ArrayOf<Int32>* attachments,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return NOERROR;
}

// C function void glInvalidateSubFramebuffer ( GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height )

ECode GLES30::GlInvalidateSubFramebuffer(
    /* [in] */ Int32 target,
    /* [in] */ Int32 numAttachments,
    /* [in] */ IInt32Buffer* attachments,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return NOERROR;
}

// C function void glTexStorage2D ( GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height )

ECode GLES30::GlTexStorage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 levels,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return NOERROR;
}

// C function void glTexStorage3D ( GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth )

ECode GLES30::GlTexStorage3D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 levels,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 depth)
{
    return NOERROR;
}

// C function void glGetInternalformativ ( GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params )

ECode GLES30::GlGetInternalformativ(
    /* [in] */ Int32 target,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 bufSize,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetInternalformativ ( GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params )

ECode GLES30::GlGetInternalformativ(
    /* [in] */ Int32 target,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 bufSize,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

} // namespace Opengl
} // namespace Droid
} // namespace Elastos