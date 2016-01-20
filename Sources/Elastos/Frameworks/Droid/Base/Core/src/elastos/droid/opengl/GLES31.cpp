#include "Elastos.Droid.View.h"
#include "elastos/droid/opengl/GLES31.h"

namespace Elastos {
namespace Droid {
namespace Opengl {

CAR_INTERFACE_IMPL(GLES31, GLES30, IGLES31)
// C function void glDispatchCompute ( GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z )

ECode GLES31::GlDispatchCompute(
    /* [in] */ Int32 num_groups_x,
    /* [in] */ Int32 num_groups_y,
    /* [in] */ Int32 num_groups_z)
{
    return NOERROR;
}

// C function void glDispatchComputeIndirect ( GLintptr indirect );

ECode GLES31::GlDispatchComputeIndirect(
    /* [in] */ Int64 indirect)
{
    return NOERROR;
}

// C function void glDrawArraysIndirect ( GLenum mode, const void *indirect );

ECode GLES31::GlDrawArraysIndirect(
    /* [in] */ Int32 mode,
    /* [in] */ Int64 indirect)
{
    return NOERROR;
}

// C function glDrawElementsIndirect ( GLenum mode, GLenum type, const void *indirect );

ECode GLES31::GlDrawElementsIndirect(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 type,
    /* [in] */ Int64 indirect)
{
    return NOERROR;
}

// C function void glFramebufferParameteri ( GLenum target, GLenum pname, GLint param )

ECode GLES31::GlFramebufferParameteri(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    return NOERROR;
}

// C function void glGetFramebufferParameteriv ( GLenum target, GLenum pname, GLint *params )

ECode GLES31::GlGetFramebufferParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetFramebufferParameteriv ( GLenum target, GLenum pname, GLint *params )

ECode GLES31::GlGetFramebufferParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function void glGetProgramInterfaceiv ( GLuint program, GLenum programInterface, GLenum pname, GLint *params )

ECode GLES31::GlGetProgramInterfaceiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 programInterface,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetProgramInterfaceiv ( GLuint program, GLenum programInterface, GLenum pname, GLint *params )

ECode GLES31::GlGetProgramInterfaceiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 programInterface,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function GLuint glGetProgramResourceIndex ( GLuint program, GLenum programInterface, const GLchar *name )

ECode GLES31::GlGetProgramResourceIndex(
    /* [in] */ Int32 program,
    /* [in] */ Int32 programInterface,
    /* [in] */ const String& name,
    /* [out] */ Int32* index)
{
    return NOERROR;
}

// C function void glGetProgramResourceName ( GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name )

ECode GLES31::GlGetProgramResourceName(
    /* [in] */ Int32 program,
    /* [in] */ Int32 programInterface,
    /* [in] */ Int32 index,
    /* [out] */ String* result)
{
    return NOERROR;
}

// C function void glGetProgramResourceiv ( GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params )

ECode GLES31::GlGetProgramResourceiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 programInterface,
    /* [in] */ Int32 index,
    /* [in] */ Int32 propCount,
    /* [in] */ ArrayOf<Int32>* props,
    /* [in] */ Int32 propsOffset,
    /* [in] */ Int32 bufSize,
    /* [in] */ ArrayOf<Int32>* length,
    /* [in] */ Int32 lengthOffset,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 paramsOffset)
{
    return NOERROR;
}

// C function void glGetProgramResourceiv ( GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params )

ECode GLES31::GlGetProgramResourceiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 programInterface,
    /* [in] */ Int32 index,
    /* [in] */ Int32 propCount,
    /* [in] */ IInt32Buffer* props,
    /* [in] */ Int32 bufSize,
    /* [in] */ IInt32Buffer* length,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function GLint glGetProgramResourceLocation ( GLuint program, GLenum programInterface, const GLchar *name )

ECode GLES31::GlGetProgramResourceLocation(
    /* [in] */ Int32 program,
    /* [in] */ Int32 programInterface,
    /* [in] */ const String& name,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

// C function void glUseProgramStages ( GLuint pipeline, GLbitfield stages, GLuint program )

ECode GLES31::GlUseProgramStages(
    /* [in] */ Int32 pipeline,
    /* [in] */ Int32 stages,
    /* [in] */ Int32 program)
{
    return NOERROR;
}

// C function void glActiveShaderProgram ( GLuint pipeline, GLuint program )

ECode GLES31::GlActiveShaderProgram(
    /* [in] */ Int32 pipeline,
    /* [in] */ Int32 program)
{
    return NOERROR;
}

// C function GLuint glCreateShaderProgramv ( GLenum type, GLsizei count, const GLchar *const *strings )

ECode GLES31::GlCreateShaderProgramv(
    /* [in] */ Int32 type,
    /* [in] */ ArrayOf<String>* strings,
    /* [out] */ Int32* result)
{
    return NOERROR;
}

// C function void glBindProgramPipeline ( GLuint pipeline )

ECode GLES31::GlBindProgramPipeline(
    /* [in] */ Int32 pipeline)
{
    return NOERROR;
}

// C function void glDeleteProgramPipelines ( GLsizei n, const GLuint *pipelines )

ECode GLES31::GlDeleteProgramPipelines(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* pipelines,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glDeleteProgramPipelines ( GLsizei n, const GLuint *pipelines )

ECode GLES31::GlDeleteProgramPipelines(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* pipelines)
{
    return NOERROR;
}

// C function void glGenProgramPipelines ( GLsizei n, GLuint *pipelines )

ECode GLES31::GlGenProgramPipelines(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* pipelines,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGenProgramPipelines ( GLsizei n, GLuint *pipelines )

ECode GLES31::GlGenProgramPipelines(
    /* [in] */ Int32 n,
    /* [in] */ IInt32Buffer* pipelines)
{
    return NOERROR;
}

// C function GLboolean glIsProgramPipeline ( GLuint pipeline )

ECode GLES31::GlIsProgramPipeline(
    /* [in] */ Int32 pipeline,
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

// C function void glGetProgramPipelineiv ( GLuint pipeline, GLenum pname, GLint *params )

ECode GLES31::GlGetProgramPipelineiv(
    /* [in] */ Int32 pipeline,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetProgramPipelineiv ( GLuint pipeline, GLenum pname, GLint *params )

ECode GLES31::GlGetProgramPipelineiv(
    /* [in] */ Int32 pipeline,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function void glProgramUniform1i ( GLuint program, GLint location, GLint v0 )

ECode GLES31::GlProgramUniform1i(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 v0)
{
    return NOERROR;
}

// C function void glProgramUniform2i ( GLuint program, GLint location, GLint v0, GLint v1 )

ECode GLES31::GlProgramUniform2i(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 v0,
    /* [in] */ Int32 v1)
{
    return NOERROR;
}

// C function void glProgramUniform3i ( GLuint program, GLint location, GLint v0, GLint v1, GLint v2 )

ECode GLES31::GlProgramUniform3i(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 v0,
    /* [in] */ Int32 v1,
    /* [in] */ Int32 v2)
{
    return NOERROR;
}

// C function void glProgramUniform4i ( GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3 )

ECode GLES31::GlProgramUniform4i(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 v0,
    /* [in] */ Int32 v1,
    /* [in] */ Int32 v2,
    /* [in] */ Int32 v3)
{
    return NOERROR;
}

// C function void glProgramUniform1ui ( GLuint program, GLint location, GLuint v0 )

ECode GLES31::GlProgramUniform1ui(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 v0)
{
    return NOERROR;
}

// C function void glProgramUniform2ui ( GLuint program, GLint location, GLuint v0, GLuint v1 )

ECode GLES31::GlProgramUniform2ui(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 v0,
    /* [in] */ Int32 v1)
{
    return NOERROR;
}

// C function void glProgramUniform3ui ( GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2 )

ECode GLES31::GlProgramUniform3ui(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 v0,
    /* [in] */ Int32 v1,
    /* [in] */ Int32 v2)
{
    return NOERROR;
}

// C function void glProgramUniform4ui ( GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3 )

ECode GLES31::GlProgramUniform4ui(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 v0,
    /* [in] */ Int32 v1,
    /* [in] */ Int32 v2,
    /* [in] */ Int32 v3)
{
    return NOERROR;
}

// C function void glProgramUniform1f ( GLuint program, GLint location, GLfloat v0 )

ECode GLES31::GlProgramUniform1f(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Float v0)
{
    return NOERROR;
}

// C function void glProgramUniform2f ( GLuint program, GLint location, GLfloat v0, GLfloat v1 )

ECode GLES31::GlProgramUniform2f(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Float v0,
    /* [in] */ Float v1)
{
    return NOERROR;
}

// C function void glProgramUniform3f ( GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2 )

ECode GLES31::GlProgramUniform3f(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Float v0,
    /* [in] */ Float v1,
    /* [in] */ Float v2)
{
    return NOERROR;
}

// C function void glProgramUniform4f ( GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3 )

ECode GLES31::GlProgramUniform4f(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Float v0,
    /* [in] */ Float v1,
    /* [in] */ Float v2,
    /* [in] */ Float v3)
{
    return NOERROR;
}

// C function void glProgramUniform1iv ( GLuint program, GLint location, GLsizei count, const GLint *value )

ECode GLES31::GlProgramUniform1iv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniform1iv ( GLuint program, GLint location, GLsizei count, const GLint *value )

ECode GLES31::GlProgramUniform1iv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* value)
{
    return NOERROR;
}

// C function void glProgramUniform2iv ( GLuint program, GLint location, GLsizei count, const GLint *value )

ECode GLES31::GlProgramUniform2iv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniform2iv ( GLuint program, GLint location, GLsizei count, const GLint *value )

ECode GLES31::GlProgramUniform2iv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* value)
{
    return NOERROR;
}

// C function void glProgramUniform3iv ( GLuint program, GLint location, GLsizei count, const GLint *value )

ECode GLES31::GlProgramUniform3iv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniform3iv ( GLuint program, GLint location, GLsizei count, const GLint *value )

ECode GLES31::GlProgramUniform3iv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* value)
{
    return NOERROR;
}

// C function void glProgramUniform4iv ( GLuint program, GLint location, GLsizei count, const GLint *value )

ECode GLES31::GlProgramUniform4iv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniform4iv ( GLuint program, GLint location, GLsizei count, const GLint *value )

ECode GLES31::GlProgramUniform4iv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* value)
{
    return NOERROR;
}

// C function void glProgramUniform1uiv ( GLuint program, GLint location, GLsizei count, const GLuint *value )

ECode GLES31::GlProgramUniform1uiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniform1uiv ( GLuint program, GLint location, GLsizei count, const GLuint *value )

ECode GLES31::GlProgramUniform1uiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* value)
{
    return NOERROR;
}

// C function void glProgramUniform2uiv ( GLuint program, GLint location, GLsizei count, const GLuint *value )

ECode GLES31::GlProgramUniform2uiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniform2uiv ( GLuint program, GLint location, GLsizei count, const GLuint *value )

ECode GLES31::GlProgramUniform2uiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* value)
{
    return NOERROR;
}

// C function void glProgramUniform3uiv ( GLuint program, GLint location, GLsizei count, const GLuint *value )

ECode GLES31::GlProgramUniform3uiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniform3uiv ( GLuint program, GLint location, GLsizei count, const GLuint *value )

ECode GLES31::GlProgramUniform3uiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* value)
{
    return NOERROR;
}

// C function void glProgramUniform4uiv ( GLuint program, GLint location, GLsizei count, const GLuint *value )

ECode GLES31::GlProgramUniform4uiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Int32>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniform4uiv ( GLuint program, GLint location, GLsizei count, const GLuint *value )

ECode GLES31::GlProgramUniform4uiv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IInt32Buffer* value)
{
    return NOERROR;
}

// C function void glProgramUniform1fv ( GLuint program, GLint location, GLsizei count, const GLfloat *value )

ECode GLES31::GlProgramUniform1fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniform1fv ( GLuint program, GLint location, GLsizei count, const GLfloat *value )

ECode GLES31::GlProgramUniform1fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glProgramUniform2fv ( GLuint program, GLint location, GLsizei count, const GLfloat *value )

ECode GLES31::GlProgramUniform2fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniform2fv ( GLuint program, GLint location, GLsizei count, const GLfloat *value )

ECode GLES31::GlProgramUniform2fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glProgramUniform3fv ( GLuint program, GLint location, GLsizei count, const GLfloat *value )

ECode GLES31::GlProgramUniform3fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniform3fv ( GLuint program, GLint location, GLsizei count, const GLfloat *value )

ECode GLES31::GlProgramUniform3fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glProgramUniform4fv ( GLuint program, GLint location, GLsizei count, const GLfloat *value )

ECode GLES31::GlProgramUniform4fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniform4fv ( GLuint program, GLint location, GLsizei count, const GLfloat *value )

ECode GLES31::GlProgramUniform4fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix2fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix2fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix2fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix2fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix3fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix3fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix3fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix3fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix4fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix4fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix4fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix4fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix2x3fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix2x3fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix2x3fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix2x3fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix3x2fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix3x2fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix3x2fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix3x2fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix2x4fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix2x4fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix2x4fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix2x4fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix4x2fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix4x2fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix4x2fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix4x2fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix3x4fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix3x4fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix3x4fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix3x4fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix4x3fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix4x3fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ ArrayOf<Float>* value,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glProgramUniformMatrix4x3fv ( GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )

ECode GLES31::GlProgramUniformMatrix4x3fv(
    /* [in] */ Int32 program,
    /* [in] */ Int32 location,
    /* [in] */ Int32 count,
    /* [in] */ Boolean transpose,
    /* [in] */ IFloatBuffer* value)
{
    return NOERROR;
}

// C function void glValidateProgramPipeline ( GLuint pipeline )

ECode GLES31::GlValidateProgramPipeline(
    /* [in] */ Int32 pipeline)
{
    return NOERROR;
}

// C function void glGetProgramPipelineInfoLog( GLuint program, GLsizei maxLength, GLsizei * length, GLchar * infoLog);

ECode GLES31::GlGetProgramPipelineInfoLog(
    /* [in] */ Int32 program,
    /* [out] */ String* result)
{
    return NOERROR;
}

// C function void glBindImageTexture ( GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format )

ECode GLES31::GlBindImageTexture(
    /* [in] */ Int32 unit,
    /* [in] */ Int32 texture,
    /* [in] */ Int32 level,
    /* [in] */ Boolean layered,
    /* [in] */ Int32 layer,
    /* [in] */ Int32 access,
    /* [in] */ Int32 format)
{
    return NOERROR;
}

// C function void glGetBooleani_v ( GLenum target, GLuint index, GLboolean *data )

ECode GLES31::GlGetBooleani_v(
    /* [in] */ Int32 target,
    /* [in] */ Int32 index,
    /* [in] */ ArrayOf<Boolean>* data,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetBooleani_v ( GLenum target, GLuint index, GLboolean *data )

ECode GLES31::GlGetBooleani_v(
    /* [in] */ Int32 target,
    /* [in] */ Int32 index,
    /* [in] */ IInt32Buffer* data)
{
    return NOERROR;
}

// C function void glMemoryBarrier ( GLbitfield barriers )

ECode GLES31::GlMemoryBarrier(
    /* [in] */ Int32 barriers)
{
    return NOERROR;
}

// C function void glMemoryBarrierByRegion ( GLbitfield barriers )

ECode GLES31::GlMemoryBarrierByRegion(
    /* [in] */ Int32 barriers)
{
    return NOERROR;
}

// C function void glTexStorage2DMultisample ( GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations )

ECode GLES31::GlTexStorage2DMultisample(
    /* [in] */ Int32 target,
    /* [in] */ Int32 samples,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean fixedsamplelocations)
{
    return NOERROR;
}

// C function void glGetMultisamplefv ( GLenum pname, GLuint index, GLfloat *val )

ECode GLES31::GlGetMultisamplefv(
    /* [in] */ Int32 pname,
    /* [in] */ Int32 index,
    /* [in] */ ArrayOf<Float>* val,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetMultisamplefv ( GLenum pname, GLuint index, GLfloat *val )

ECode GLES31::GlGetMultisamplefv(
    /* [in] */ Int32 pname,
    /* [in] */ Int32 index,
    /* [in] */ IFloatBuffer* val)
{
    return NOERROR;
}

// C function void glSampleMaski ( GLuint maskNumber, GLbitfield mask )

ECode GLES31::GlSampleMaski(
    /* [in] */ Int32 maskNumber,
    /* [in] */ Int32 mask)
{
    return NOERROR;
}

// C function void glGetTexLevelParameteriv ( GLenum target, GLint level, GLenum pname, GLint *params )

ECode GLES31::GlGetTexLevelParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetTexLevelParameteriv ( GLenum target, GLint level, GLenum pname, GLint *params )

ECode GLES31::GlGetTexLevelParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 pname,
    /* [in] */ IInt32Buffer* params)
{
    return NOERROR;
}

// C function void glGetTexLevelParameterfv ( GLenum target, GLint level, GLenum pname, GLfloat *params )

ECode GLES31::GlGetTexLevelParameterfv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params,
    /* [in] */ Int32 offset)
{
    return NOERROR;
}

// C function void glGetTexLevelParameterfv ( GLenum target, GLint level, GLenum pname, GLfloat *params )

ECode GLES31::GlGetTexLevelParameterfv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 pname,
    /* [in] */ IFloatBuffer* params)
{
    return NOERROR;
}

// C function void glBindVertexBuffer ( GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride )

ECode GLES31::GlBindVertexBuffer(
    /* [in] */ Int32 bindingindex,
    /* [in] */ Int32 buffer,
    /* [in] */ Int64 offset,
    /* [in] */ Int32 stride)
{
    return NOERROR;
}

// C function void glVertexAttribFormat ( GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset )

ECode GLES31::GlVertexAttribFormat(
    /* [in] */ Int32 attribindex,
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Boolean normalized,
    /* [in] */ Int32 relativeoffset)
{
    return NOERROR;
}

// C function void glVertexAttribIFormat ( GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset )

ECode GLES31::GlVertexAttribIFormat(
    /* [in] */ Int32 attribindex,
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Int32 relativeoffset)
{
    return NOERROR;
}

// C function void glVertexAttribBinding ( GLuint attribindex, GLuint bindingindex )

ECode GLES31::GlVertexAttribBinding(
    /* [in] */ Int32 attribindex,
    /* [in] */ Int32 bindingindex)
{
    return NOERROR;
}

// C function void glVertexBindingDivisor ( GLuint bindingindex, GLuint divisor )

ECode GLES31::GlVertexBindingDivisor(
    /* [in] */ Int32 bindingindex,
    /* [in] */ Int32 divisor)
{
    return NOERROR;
}

} // namespace Opengl
} // namespace Droid
} // namespace Elastos
