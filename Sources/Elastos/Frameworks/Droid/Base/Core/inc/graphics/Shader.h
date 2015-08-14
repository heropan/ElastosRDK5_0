
#ifndef __ELASTOS_DROID_GRAPHICS_SHADER_H__
#define __ELASTOS_DROID_GRAPHICS_SHADER_H__

#include "ext/frameworkext.h"
#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

extern "C" const InterfaceID EIID_Shader;

/**
 * Shader is the based class for objects that return horizontal spans of colors
 * during drawing. A subclass of Shader is installed in a Paint calling
 * paint.setShader(shader). After that any object (other than a bitmap) that is
 * drawn with that paint will get its color(s) from the shader.
 */
class Shader
{
public:
    ~Shader();

    /**
     * Return true if the shader has a non-identity local matrix.
     * @param localM If not null, it is set to the shader's local matrix.
     * @return true if the shader has a non-identity local matrix
     */
    virtual CARAPI_(Boolean) GetLocalMatrix(
        /* [in, out] */ IMatrix* localM);

    /**
     * Set the shader's local matrix. Passing null will reset the shader's
     * matrix to identity
     * @param localM The shader's new local matrix, or null to specify identity
     */
    virtual CARAPI SetLocalMatrix(
        /* [in] */ IMatrix* localM);

private:
    static CARAPI_(void) NativeDestructor(
        /* [in] */ Int32 shader,
        /* [in] */ Int32 skiaShader);

    static CARAPI_(void) NativeSetLocalMatrix(
        /* [in] */ Int32 shader,
        /* [in] */ Int32 skiaShader,
        /* [in] */ Int32 matrix);

public:
    Int32 mNativeInstance;
    Int32 mNativeShader;

private:
    AutoPtr<IMatrix> mLocalMatrix;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_SHADER_H__
