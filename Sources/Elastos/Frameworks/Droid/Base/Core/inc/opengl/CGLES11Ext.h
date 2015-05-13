
#ifndef __CGLES11EXT_H__
#define __CGLES11EXT_H__

#include "_CGLES11Ext.h"

using Elastos::IO::IBuffer;

namespace Elastos {
namespace Droid {
namespace Opengl {

CarClass(CGLES11Ext)
{
public:
    CARAPI GlBlendEquationSeparateOES(
        /* [in] */ Int32 modeRGB,
        /* [in] */ Int32 modeAlpha);

    CARAPI GlBlendFuncSeparateOES(
        /* [in] */ Int32 srcRGB,
        /* [in] */ Int32 dstRGB,
        /* [in] */ Int32 srcAlpha,
        /* [in] */ Int32 dstAlpha);

    CARAPI GlBlendEquationOES(
        /* [in] */ Int32 mode);

    CARAPI GlDrawTexsOES(
        /* [in] */ Int16 x,
        /* [in] */ Int16 y,
        /* [in] */ Int16 z,
        /* [in] */ Int16 width,
        /* [in] */ Int16 height);

    CARAPI GlDrawTexiOES(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 z,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI GlDrawTexxOES(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 z,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI GlDrawTexsvOES(
        /* [in] */ ArrayOf<Int16>* coords,
        /* [in] */ Int32 offset);

    CARAPI GlDrawTexsvOESEx(
        /* [in] */ Elastos::IO::IInt16Buffer* coords);

    CARAPI GlDrawTexivOES(
        /* [in] */ ArrayOf<Int32>* coords,
        /* [in] */ Int32 offset);

    CARAPI GlDrawTexivOESEx(
        /* [in] */ Elastos::IO::IInt32Buffer* coords);

    CARAPI GlDrawTexxvOES(
        /* [in] */ ArrayOf<Int32>* coords,
        /* [in] */ Int32 offset);

    CARAPI GlDrawTexxvOESEx(
        /* [in] */ Elastos::IO::IInt32Buffer* coords);

    CARAPI GlDrawTexfOES(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float z,
        /* [in] */ Float width,
        /* [in] */ Float height);

    CARAPI GlDrawTexfvOES(
        /* [in] */ ArrayOf<Float>* coords,
        /* [in] */ Int32 offset);

    CARAPI GlDrawTexfvOESEx(
        /* [in] */ Elastos::IO::IFloatBuffer* coords);

    CARAPI GlEGLImageTargetTexture2DOES(
        /* [in] */ Int32 target,
        /* [in] */ Elastos::IO::IBuffer* image);

    CARAPI GlEGLImageTargetRenderbufferStorageOES(
        /* [in] */ Int32 target,
        /* [in] */ Elastos::IO::IBuffer* image);

    CARAPI GlAlphaFuncxOES(
        /* [in] */ Int32 func,
        /* [in] */ Int32 ref);

    CARAPI GlClearColorxOES(
        /* [in] */ Int32 red,
        /* [in] */ Int32 green,
        /* [in] */ Int32 blue,
        /* [in] */ Int32 alpha);

    CARAPI GlClearDepthxOES(
        /* [in] */ Int32 depth);

    CARAPI GlClipPlanexOES(
        /* [in] */ Int32 plane,
        /* [in] */ ArrayOf<Int32>* equation,
        /* [in] */ Int32 offset);

    CARAPI GlClipPlanexOESEx(
        /* [in] */ Int32 plane,
        /* [in] */ Elastos::IO::IInt32Buffer* equation);

    CARAPI GlColor4xOES(
        /* [in] */ Int32 red,
        /* [in] */ Int32 green,
        /* [in] */ Int32 blue,
        /* [in] */ Int32 alpha);

    CARAPI GlDepthRangexOES(
        /* [in] */ Int32 zNear,
        /* [in] */ Int32 zFar);

    CARAPI GlFogxOES(
        /* [in] */ Int32 pname,
        /* [in] */ Int32 param);

    CARAPI GlFogxvOES(
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlFogxvOESEx(
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlFrustumxOES(
        /* [in] */ Int32 left,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [in] */ Int32 top,
        /* [in] */ Int32 zNear,
        /* [in] */ Int32 zFar);

    CARAPI GlGetClipPlanexOES(
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* eqn,
        /* [in] */ Int32 offset);

    CARAPI GlGetClipPlanexOESEx(
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* eqn);

    CARAPI GlGetFixedvOES(
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlGetFixedvOESEx(
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlGetLightxvOES(
        /* [in] */ Int32 light,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlGetLightxvOESEx(
        /* [in] */ Int32 light,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlGetMaterialxvOES(
        /* [in] */ Int32 face,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlGetMaterialxvOESEx(
        /* [in] */ Int32 face,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlGetTexEnvxvOES(
        /* [in] */ Int32 env,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlGetTexEnvxvOESEx(
        /* [in] */ Int32 env,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlGetTexParameterxvOES(
        /* [in] */ Int32 target,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlGetTexParameterxvOESEx(
        /* [in] */ Int32 target,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlLightModelxOES(
        /* [in] */ Int32 pname,
        /* [in] */ Int32 param);

    CARAPI GlLightModelxvOES(
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlLightModelxvOESEx(
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlLightxOES(
        /* [in] */ Int32 light,
        /* [in] */ Int32 pname,
        /* [in] */ Int32 param);

    CARAPI GlLightxvOES(
        /* [in] */ Int32 light,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlLightxvOESEx(
        /* [in] */ Int32 light,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlLineWidthxOES(
        /* [in] */ Int32 width);

    CARAPI GlLoadMatrixxOES(
        /* [in] */ ArrayOf<Int32>* m,
        /* [in] */ Int32 offset);

    CARAPI GlLoadMatrixxOESEx(
        /* [in] */ Elastos::IO::IInt32Buffer* m);

    CARAPI GlMaterialxOES(
        /* [in] */ Int32 face,
        /* [in] */ Int32 pname,
        /* [in] */ Int32 param);

    CARAPI GlMaterialxvOES(
        /* [in] */ Int32 face,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlMaterialxvOESEx(
        /* [in] */ Int32 face,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlMultMatrixxOES(
        /* [in] */ ArrayOf<Int32>* m,
        /* [in] */ Int32 offset);

    CARAPI GlMultMatrixxOESEx(
        /* [in] */ Elastos::IO::IInt32Buffer* m);

    CARAPI GlMultiTexCoord4xOES(
        /* [in] */ Int32 target,
        /* [in] */ Int32 s,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 q);

    CARAPI GlNormal3xOES(
        /* [in] */ Int32 nx,
        /* [in] */ Int32 ny,
        /* [in] */ Int32 nz);

    CARAPI GlOrthoxOES(
        /* [in] */ Int32 left,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [in] */ Int32 top,
        /* [in] */ Int32 zNear,
        /* [in] */ Int32 zFar);

    CARAPI GlPointParameterxOES(
        /* [in] */ Int32 pname,
        /* [in] */ Int32 param);

    CARAPI GlPointParameterxvOES(
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlPointParameterxvOESEx(
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlPointSizexOES(
        /* [in] */ Int32 size);

    CARAPI GlPolygonOffsetxOES(
        /* [in] */ Int32 factor,
        /* [in] */ Int32 units);

    CARAPI GlRotatexOES(
        /* [in] */ Int32 angle,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 z);

    CARAPI GlSampleCoveragexOES(
        /* [in] */ Int32 value,
        /* [in] */ Boolean invert);

    CARAPI GlScalexOES(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 z);

    CARAPI GlTexEnvxOES(
        /* [in] */ Int32 target,
        /* [in] */ Int32 pname,
        /* [in] */ Int32 param);

    CARAPI GlTexEnvxvOES(
        /* [in] */ Int32 target,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlTexEnvxvOESEx(
        /* [in] */ Int32 target,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlTexParameterxOES(
        /* [in] */ Int32 target,
        /* [in] */ Int32 pname,
        /* [in] */ Int32 param);

    CARAPI GlTexParameterxvOES(
        /* [in] */ Int32 target,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlTexParameterxvOESEx(
        /* [in] */ Int32 target,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlTranslatexOES(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 z);

    CARAPI GlIsRenderbufferOES(
        /* [in] */ Int32 renderbuffer,
        /* [out] */ Boolean* result);

    CARAPI GlBindRenderbufferOES(
        /* [in] */ Int32 target,
        /* [in] */ Int32 renderbuffer);

    CARAPI GlDeleteRenderbuffersOES(
        /* [in] */ Int32 n,
        /* [in] */ ArrayOf<Int32>* renderbuffers,
        /* [in] */ Int32 offset);

    CARAPI GlDeleteRenderbuffersOESEx(
        /* [in] */ Int32 n,
        /* [in] */ Elastos::IO::IInt32Buffer* renderbuffers);

    CARAPI GlGenRenderbuffersOES(
        /* [in] */ Int32 n,
        /* [in] */ ArrayOf<Int32>* renderbuffers,
        /* [in] */ Int32 offset);

    CARAPI GlGenRenderbuffersOESEx(
        /* [in] */ Int32 n,
        /* [in] */ Elastos::IO::IInt32Buffer* renderbuffers);

    CARAPI GlRenderbufferStorageOES(
        /* [in] */ Int32 target,
        /* [in] */ Int32 internalformat,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI GlGetRenderbufferParameterivOES(
        /* [in] */ Int32 target,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlGetRenderbufferParameterivOESEx(
        /* [in] */ Int32 target,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlIsFramebufferOES(
        /* [in] */ Int32 framebuffer,
        /* [out] */ Boolean* result);

    CARAPI GlBindFramebufferOES(
        /* [in] */ Int32 target,
        /* [in] */ Int32 framebuffer);

    CARAPI GlDeleteFramebuffersOES(
        /* [in] */ Int32 n,
        /* [in] */ ArrayOf<Int32>* framebuffers,
        /* [in] */ Int32 offset);

    CARAPI GlDeleteFramebuffersOESEx(
        /* [in] */ Int32 n,
        /* [in] */ Elastos::IO::IInt32Buffer* framebuffers);

    CARAPI GlGenFramebuffersOES(
        /* [in] */ Int32 n,
        /* [in] */ ArrayOf<Int32>* framebuffers,
        /* [in] */ Int32 offset);

    CARAPI GlGenFramebuffersOESEx(
        /* [in] */ Int32 n,
        /* [in] */ Elastos::IO::IInt32Buffer* framebuffers);

    CARAPI GlCheckFramebufferStatusOES(
        /* [in] */ Int32 target,
        /* [out] */ Int32* status);

    CARAPI GlFramebufferRenderbufferOES(
        /* [in] */ Int32 target,
        /* [in] */ Int32 attachment,
        /* [in] */ Int32 renderbuffertarget,
        /* [in] */ Int32 renderbuffer);

    CARAPI GlFramebufferTexture2DOES(
        /* [in] */ Int32 target,
        /* [in] */ Int32 attachment,
        /* [in] */ Int32 textarget,
        /* [in] */ Int32 texture,
        /* [in] */ Int32 level);

    CARAPI GlGetFramebufferAttachmentParameterivOES(
        /* [in] */ Int32 target,
        /* [in] */ Int32 attachment,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlGetFramebufferAttachmentParameterivOESEx(
        /* [in] */ Int32 target,
        /* [in] */ Int32 attachment,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlGenerateMipmapOES(
        /* [in] */ Int32 target);

    CARAPI GlCurrentPaletteMatrixOES(
        /* [in] */ Int32 matrixpaletteindex);

    CARAPI GlLoadPaletteFromModelViewMatrixOES();

    CARAPI GlMatrixIndexPointerOES(
        /* [in] */ Int32 size,
        /* [in] */ Int32 type,
        /* [in] */ Int32 stride,
        /* [in] */ Elastos::IO::IBuffer* pointer);

    CARAPI GlWeightPointerOES(
        /* [in] */ Int32 size,
        /* [in] */ Int32 type,
        /* [in] */ Int32 stride,
        /* [in] */ Elastos::IO::IBuffer* pointer);

    CARAPI GlDepthRangefOES(
        /* [in] */ Float zNear,
        /* [in] */ Float zFar);

    CARAPI GlFrustumfOES(
        /* [in] */ Float left,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ Float top,
        /* [in] */ Float zNear,
        /* [in] */ Float zFar);

    CARAPI GlOrthofOES(
        /* [in] */ Float left,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ Float top,
        /* [in] */ Float zNear,
        /* [in] */ Float zFar);

    CARAPI GlClipPlanefOES(
        /* [in] */ Int32 plane,
        /* [in] */ ArrayOf<Float>* equation,
        /* [in] */ Int32 offset);

    CARAPI GlClipPlanefOESEx(
        /* [in] */ Int32 plane,
        /* [in] */ Elastos::IO::IFloatBuffer* equation);

    CARAPI GlGetClipPlanefOES(
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Float>* eqn,
        /* [in] */ Int32 offset);

    CARAPI GlGetClipPlanefOESEx(
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IFloatBuffer* eqn);

    CARAPI GlClearDepthfOES(
        /* [in] */ Float depth);

    CARAPI GlTexGenfOES(
        /* [in] */ Int32 coord,
        /* [in] */ Int32 pname,
        /* [in] */ Float param);

    CARAPI GlTexGenfvOES(
        /* [in] */ Int32 coord,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Float>* params,
        /* [in] */ Int32 offset);

    CARAPI GlTexGenfvOESEx(
        /* [in] */ Int32 coord,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IFloatBuffer* params);

    CARAPI GlTexGeniOES(
        /* [in] */ Int32 coord,
        /* [in] */ Int32 pname,
        /* [in] */ Int32 param);

    CARAPI GlTexGenivOES(
        /* [in] */ Int32 coord,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlTexGenivOESEx(
        /* [in] */ Int32 coord,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlTexGenxOES(
        /* [in] */ Int32 coord,
        /* [in] */ Int32 pname,
        /* [in] */ Int32 param);

    CARAPI GlTexGenxvOES(
        /* [in] */ Int32 coord,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlTexGenxvOESEx(
        /* [in] */ Int32 coord,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlGetTexGenfvOES(
        /* [in] */ Int32 coord,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Float>* params,
        /* [in] */ Int32 offset);

    CARAPI GlGetTexGenfvOESEx(
        /* [in] */ Int32 coord,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IFloatBuffer* params);

    CARAPI GlGetTexGenivOES(
        /* [in] */ Int32 coord,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlGetTexGenivOESEx(
        /* [in] */ Int32 coord,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

    CARAPI GlGetTexGenxvOES(
        /* [in] */ Int32 coord,
        /* [in] */ Int32 pname,
        /* [in] */ ArrayOf<Int32>* params,
        /* [in] */ Int32 offset);

    CARAPI GlGetTexGenxvOESEx(
        /* [in] */ Int32 coord,
        /* [in] */ Int32 pname,
        /* [in] */ Elastos::IO::IInt32Buffer* params);

private:
    CARAPI GlMatrixIndexPointerOESBounds(
        /* [in] */ Int32 size,
        /* [in] */ Int32 type,
        /* [in] */ Int32 stride,
        /* [in] */ Elastos::IO::IBuffer* pointer,
        /* [in] */ Int32 remaining);

    CARAPI GlWeightPointerOESBounds(
        /* [in] */ Int32 size,
        /* [in] */ Int32 type,
        /* [in] */ Int32 stride,
        /* [in] */ Elastos::IO::IBuffer* pointer,
        /* [in] */ Int32 remaining);

    CARAPI GetPointer(
        /* [in] */ IBuffer* buffer,
        /* [in, out] */ Handle32* array,
        /* [in, out] */ Int32* remaining,
        /* [in, out] */ Int32* offset,
        /* [out] */ Handle32* rst);

    CARAPI GetDirectBufferPointer(
        /* [in] */ IBuffer* buffer,
        /* [out] */ Handle32* result);

private:
    static const Int32 GL_BYTE = IGLES10::_GL_BYTE;
    static const Int32 GL_FIXED = IGLES10::_GL_FIXED;
    static const Int32 GL_FLOAT = IGLES10::_GL_FLOAT;
    static const Int32 GL_SHORT = IGLES10::_GL_SHORT;
    static AutoPtr<IBuffer> sMatrixIndexPointerOES;
};

} // namespace Opengl
} // namespace Droid
} // namespace Elastos

#endif // __CGLES11EXT_H__
