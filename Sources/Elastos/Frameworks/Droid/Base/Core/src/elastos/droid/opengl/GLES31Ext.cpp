#include "elastos/droid/opengl/GLES31Ext.h"

namespace Elastos {
namespace Droid {
namespace Opengl {

CAR_INTERFACE_IMPL(GLES31Ext, Singleton, IGLES31Ext)

ECode GLES31Ext::GlBlendBarrierKHR()
{
    return NOERROR;
}

// C function void glDebugMessageControlKHR ( GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled )

ECode GLES31Ext::GlDebugMessageControlKHR(
    /* [in] */ Int32 source,
    /* [in] */ Int32 type,
    /* [in] */ Int32 severity,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* ids,
    /* [in] */ Int32 offset,
    /* [in] */ Boolean enabled)
{
    return NOERROR;
}

// C function void glDebugMessageControlKHR ( GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled )

ECode GLES31Ext::GlDebugMessageControlKHR(
    /* [in] */ Int32 source,
    /* [in] */ Int32 type,
    /* [in] */ Int32 severity,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* ids,
    /* [in] */ Boolean enabled)
{
    return NOERROR;
}

// C function void glDebugMessageInsertKHR ( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf )

ECode GLES31Ext::GlDebugMessageInsertKHR(
    /* [in] */ Int32 source,
    /* [in] */ Int32 type,
    /* [in] */ Int32 id,
    /* [in] */ Int32 severity,
    /* [in] */ const String& buf)
{
    return NOERROR;
}

// C function void glDebugMessageCallbackKHR ( GLDEBUGPROCKHR callback, const void *userParam )

ECode GLES31Ext::GlDebugMessageCallbackKHR(
    /* [in] */ IDebugProcKHR* cb)
{
    return NOERROR;
}

// C function GLuint glGetDebugMessageLogKHR ( GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog )

ECode GLES31Ext::GlGetDebugMessageLogKHR(
    /* [in] */ Int32 count,
    /* [in] */ Int32 bufSize,
    /* [in] */ ArrayOf<Int32>* sources,
    /* [in] */ Int32 sourcesOffset,
    /* [in] */ ArrayOf<Int32>* types,
    /* [in] */ Int32 typesOffset,
    /* [in] */ ArrayOf<Int32>* ids,
    /* [in] */ Int32 idsOffset,
    /* [in] */ ArrayOf<Int32>* severities,
    /* [in] */ Int32 severitiesOffset,
    /* [in] */ ArrayOf<Int32>* lengths,
    /* [in] */ Int32 lengthsOffset,
    /* [in] */ ArrayOf<Byte>* messageLog,
    /* [in] */ Int32 messageLogOffset,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

// C function GLuint glGetDebugMessageLogKHR ( GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog )

ECode GLES31Ext::GlGetDebugMessageLogKHR(
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* sources,
    /* [in] */ IInt32Buffer* types,
    /* [in] */ IInt32Buffer* ids,
    /* [in] */ IInt32Buffer* severities,
    /* [in] */ IInt32Buffer* lengths,
    /* [in] */ IByteBuffer* messageLog,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

// C function GLuint glGetDebugMessageLogKHR ( GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog )

ECode GLES31Ext::GlGetDebugMessageLogKHR(
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* sources,
    /* [in] */ Int32 sourcesOffset,
    /* [in] */ ArrayOf<Int32>* types,
    /* [in] */ Int32 typesOffset,
    /* [in] */ ArrayOf<Int32>* ids,
    /* [in] */ Int32 idsOffset,
    /* [in] */ ArrayOf<Int32>* severities,
    /* [in] */ Int32 severitiesOffset,
    /* [out, callee] */ ArrayOf<String>** result)
{
    return NOERROR;
}

// C function GLuint glGetDebugMessageLogKHR ( GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog )

ECode GLES31Ext::GlGetDebugMessageLogKHR(
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* sources,
    /* [in] */ IInt32Buffer* types,
    /* [in] */ IInt32Buffer* ids,
    /* [in] */ IInt32Buffer* severities,
    /* [out, callee] */ ArrayOf<String>** result)
{
    return NOERROR;
}

// C function void glPushDebugGroupKHR ( GLenum source, GLuint id, GLsizei length, const GLchar *message )

ECode GLES31Ext::GlPushDebugGroupKHR(
    /* [in] */ Int32 source,
    /* [in] */ Int32 id,
    /* [in] */ Int32 length,
    /* [in] */ const String& message)
{
    return NOERROR;
}

// C function void glPopDebugGroupKHR ( void )

ECode GLES31Ext::GlPopDebugGroupKHR()
{
    return NOERROR;
}

// C function void glObjectLabelKHR ( GLenum identifier, GLuint name, GLsizei length, const GLchar *label )

ECode GLES31Ext::GlObjectLabelKHR(
    /* [in] */ Int32 identifier,
    /* [in] */ Int32 name,
    /* [in] */ Int32 length,
    /* [in] */ const String& label)
{
    return NOERROR;
}

// C function void glGetObjectLabelKHR ( GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label )

ECode GLES31Ext::GlGetObjectLabelKHR(
    /* [in] */ Int32 identifier,
    /* [in] */ Int32 name,
    /* [out] */ String* result)
{
    return NOERROR;
}

// C function void glObjectPtrLabelKHR ( const void *ptr, GLsizei length, const GLchar *label )

ECode GLES31Ext::GlObjectPtrLabelKHR(
    /* [in] */ Int64 ptr,
    /* [in] */ const String& label)
{
    return NOERROR;
}

// C function void glGetObjectPtrLabelKHR ( const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label )

ECode GLES31Ext::GlGetObjectPtrLabelKHR(
    /* [in] */ Int64 ptr,
    /* [out] */ String* label)
{
    return NOERROR;
}

// C function void glGetPointervKHR ( GLenum pname, void **params )

ECode GLES31Ext::GlGetDebugMessageCallbackKHR(
    /* [out] */ IDebugProcKHR** khr)
{
    return NOERROR;
}

// C function void glMinSampleShadingOES ( GLfloat value )

ECode GLES31Ext::GlMinSampleShadingOES(
    /* [in] */ Float value)
{
    return NOERROR;
}

// C function void glTexStorage3DMultisampleOES ( GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations )

ECode GLES31Ext::GlTexStorage3DMultisampleOES(
    /* [in] */ Int32 target,
    /* [in] */ Int32 samples,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 depth,
    /* [in] */ Boolean fixedsamplelocations)
{
    return NOERROR;
}

// C function void glCopyImageSubDataEXT ( GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth )

ECode GLES31Ext::GlCopyImageSubDataEXT(
    /* [in] */ Int32 srcName,
    /* [in] */ Int32 srcTarget,
    /* [in] */ Int32 srcLevel,
    /* [in] */ Int32 srcX,
    /* [in] */ Int32 srcY,
    /* [in] */ Int32 srcZ,
    /* [in] */ Int32 dstName,
    /* [in] */ Int32 dstTarget,
    /* [in] */ Int32 dstLevel,
    /* [in] */ Int32 dstX,
    /* [in] */ Int32 dstY,
    /* [in] */ Int32 dstZ,
    /* [in] */ Int32 srcWidth,
    /* [in] */ Int32 srcHeight,
    /* [in] */ Int32 srcDepth)
{
    return NOERROR;
}

// C function void glEnableiEXT ( GLenum target, GLuint index )

ECode GLES31Ext::GlEnableiEXT(
    /* [in] */ Int32 target,
    /* [in] */ Int32 index)
{
    return NOERROR;
}

// C function void glDisableiEXT ( GLenum target, GLuint index )

ECode GLES31Ext::GlDisableiEXT(
    /* [in] */ Int32 target,
    /* [in] */ Int32 index)
{
    return NOERROR;
}

// C function void glBlendEquationiEXT ( GLuint buf, GLenum mode )

ECode GLES31Ext::GlBlendEquationiEXT(
    /* [in] */ Int32 buf,
    /* [in] */ Int32 mode)
{
    return NOERROR;
}

// C function void glBlendEquationSeparateiEXT ( GLuint buf, GLenum modeRGB, GLenum modeAlpha )

ECode GLES31Ext::GlBlendEquationSeparateiEXT(
    /* [in] */ Int32 buf,
    /* [in] */ Int32 modeRGB,
    /* [in] */ Int32 modeAlpha)
{
    return NOERROR;
}

// C function void glBlendFunciEXT ( GLuint buf, GLenum src, GLenum dst )

ECode GLES31Ext::GlBlendFunciEXT(
    /* [in] */ Int32 buf,
    /* [in] */ Int32 src,
    /* [in] */ Int32 dst)
{
    return NOERROR;
}

// C function void glBlendFuncSeparateiEXT ( GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha )

ECode GLES31Ext::GlBlendFuncSeparateiEXT(
    /* [in] */ Int32 buf,
    /* [in] */ Int32 srcRGB,
    /* [in] */ Int32 dstRGB,
    /* [in] */ Int32 srcAlpha,
    /* [in] */ Int32 dstAlpha)
{
    return NOERROR;
}

// C function void glColorMaskiEXT ( GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a )

ECode GLES31Ext::GlColorMaskiEXT(
    /* [in] */ Int32 index,
    /* [in] */ Boolean r,
    /* [in] */ Boolean g,
    /* [in] */ Boolean b,
    /* [in] */ Boolean a)
{
    return NOERROR;
}

// C function GLboolean glIsEnablediEXT ( GLenum target, GLuint index )

ECode GLES31Ext::GlIsEnablediEXT(
    /* [in] */ Int32 target,
    /* [in] */ Int32 index,
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

// C function void glFramebufferTextureEXT ( GLenum target, GLenum attachment, GLuint texture, GLint level )

ECode GLES31Ext::GlFramebufferTextureEXT(
    /* [in] */ Int32 target,
    /* [in] */ Int32 attachment,
    /* [in] */ Int32 texture,
    /* [in] */ Int32 level)
{
    return NOERROR;
}

// C function void glPrimitiveBoundingBoxEXT ( GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW )

ECode GLES31Ext::GlPrimitiveBoundingBoxEXT(
    /* [in] */ Float minX,
    /* [in] */ Float minY,
    /* [in] */ Float minZ,
    /* [in] */ Float minW,
    /* [in] */ Float maxX,
    /* [in] */ Float maxY,
    /* [in] */ Float maxZ,
    /* [in] */ Float maxW)
{
    return NOERROR;
}

// C function void glPatchParameteriEXT ( GLenum pname, GLint value )

ECode GLES31Ext::GlPatchParameteriEXT(
    /* [in] */ Int32 pname,
    /* [in] */ Int32 value)
{
    return NOERROR;
}

// C function void glTexParameterIivEXT ( GLenum target, GLenum pname, const GLint *params )

ECode GLES31Ext::GlTexParameterIivEXT(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glTexParameterIivEXT ( GLenum target, GLenum pname, const GLint *params )

ECode GLES31Ext::GlTexParameterIivEXT(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function void glTexParameterIuivEXT ( GLenum target, GLenum pname, const GLuint *params )

ECode GLES31Ext::GlTexParameterIuivEXT(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glTexParameterIuivEXT ( GLenum target, GLenum pname, const GLuint *params )

ECode GLES31Ext::GlTexParameterIuivEXT(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function void glGetTexParameterIivEXT ( GLenum target, GLenum pname, GLint *params )

ECode GLES31Ext::GlGetTexParameterIivEXT(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetTexParameterIivEXT ( GLenum target, GLenum pname, GLint *params )

ECode GLES31Ext::GlGetTexParameterIivEXT(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function void glGetTexParameterIuivEXT ( GLenum target, GLenum pname, GLuint *params )

ECode GLES31Ext::GlGetTexParameterIuivEXT(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetTexParameterIuivEXT ( GLenum target, GLenum pname, GLuint *params )

ECode GLES31Ext::GlGetTexParameterIuivEXT(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function void glSamplerParameterIivEXT ( GLuint sampler, GLenum pname, const GLint *param )

ECode GLES31Ext::GlSamplerParameterIivEXT(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glSamplerParameterIivEXT ( GLuint sampler, GLenum pname, const GLint *param )

ECode GLES31Ext::GlSamplerParameterIivEXT(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* param)
{
    return NOERROR;
}

// C function void glSamplerParameterIuivEXT ( GLuint sampler, GLenum pname, const GLuint *param )

ECode GLES31Ext::GlSamplerParameterIuivEXT(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glSamplerParameterIuivEXT ( GLuint sampler, GLenum pname, const GLuint *param )

ECode GLES31Ext::GlSamplerParameterIuivEXT(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* param)
{
    return NOERROR;
}

// C function void glGetSamplerParameterIivEXT ( GLuint sampler, GLenum pname, GLint *params )

ECode GLES31Ext::GlGetSamplerParameterIivEXT(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetSamplerParameterIivEXT ( GLuint sampler, GLenum pname, GLint *params )

ECode GLES31Ext::GlGetSamplerParameterIivEXT(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function void glGetSamplerParameterIuivEXT ( GLuint sampler, GLenum pname, GLuint *params )

ECode GLES31Ext::GlGetSamplerParameterIuivEXT(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetSamplerParameterIuivEXT ( GLuint sampler, GLenum pname, GLuint *params )

ECode GLES31Ext::GlGetSamplerParameterIuivEXT(
    /* [in] */ Int32 sampler,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function void glTexBufferEXT ( GLenum target, GLenum internalformat, GLuint buffer )

ECode GLES31Ext::GlTexBufferEXT(
    /* [in] */ Int32 target,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 buffer)
{
    return NOERROR;
}

// C function void glTexBufferRangeEXT ( GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size )

ECode GLES31Ext::GlTexBufferRangeEXT(
    /* [in] */ Int32 target,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 size)
{
    return NOERROR;
}

} // namespace Opengl
} // namespace Droid
} // namespace Elastos
