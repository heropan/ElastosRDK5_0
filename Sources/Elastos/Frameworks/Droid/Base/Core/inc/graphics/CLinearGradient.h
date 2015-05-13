
#ifndef __CLINEARGRADIENT_H__
#define __CLINEARGRADIENT_H__

#include "_CLinearGradient.h"
#include "graphics/Shader.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CLinearGradient), public Shader
{
public:
    /** Create a shader that draws a linear gradient along a line.
     *  @param x0           The x-coordinate for the start of the gradient line
     *  @param y0           The y-coordinate for the start of the gradient line
     *  @param x1           The x-coordinate for the end of the gradient line
     *  @param y1           The y-coordinate for the end of the gradient line
     *  @param  colors      The colors to be distributed along the gradient line
     *  @param  positions   May be null. The relative positions [0..1] of
     *                      each corresponding color in the colors array. If this is null,
     *                      the the colors are distributed evenly along the gradient line.
     *  @param  tile        The Shader tiling mode
     */
    CARAPI constructor(
        /* [in] */ Float x0,
        /* [in] */ Float y0,
        /* [in] */ Float x1,
        /* [in] */ Float y1,
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ const ArrayOf<Float>* positions,
        /* [in] */ ShaderTileMode tile);

    /** Create a shader that draws a linear gradient along a line.
     *  @param x0       The x-coordinate for the start of the gradient line
     *  @param y0       The y-coordinate for the start of the gradient line
     *  @param x1       The x-coordinate for the end of the gradient line
     *  @param y1       The y-coordinate for the end of the gradient line
     *  @param  color0  The color at the start of the gradient line.
     *  @param  color1  The color at the end of the gradient line.
     *  @param  tile    The Shader tiling mode
     */
    CARAPI constructor(
        /* [in] */ Float x0,
        /* [in] */ Float y0,
        /* [in] */ Float x1,
        /* [in] */ Float y1,
        /* [in] */ Int32 color0,
        /* [in] */ Int32 color1,
        /* [in] */ ShaderTileMode tile);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetLocalMatrix(
        /* [in, out] */ IMatrix* localM,
        /* [out] */ Boolean* result);

    CARAPI SetLocalMatrix(
        /* [in] */ IMatrix* localM);

private:
    CARAPI_(Int32) NativeCreate1(
        /* [in] */ Float x0,
        /* [in] */ Float y0,
        /* [in] */ Float x1,
        /* [in] */ Float y1,
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ const ArrayOf<Float>* positions,
        /* [in] */ ShaderTileMode tileMode);

    CARAPI_(Int32) NativeCreate2(
        /* [in] */ Float x0,
        /* [in] */ Float y0,
        /* [in] */ Float x1,
        /* [in] */ Float y1,
        /* [in] */ Int32 color0,
        /* [in] */ Int32 color1,
        /* [in] */ ShaderTileMode tileMode);

    CARAPI_(Int32) NativePostCreate1(
        /* [in] */ Int32 shader,
        /* [in] */ Float x0,
        /* [in] */ Float y0,
        /* [in] */ Float x1,
        /* [in] */ Float y1,
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ const ArrayOf<Float>* positions,
        /* [in] */ ShaderTileMode tileMode);

    CARAPI_(Int32) NativePostCreate2(
        /* [in] */ Int32 shader,
        /* [in] */ Float x0,
        /* [in] */ Float y0,
        /* [in] */ Float x1,
        /* [in] */ Float y1,
        /* [in] */ Int32 color0,
        /* [in] */ Int32 color1,
        /* [in] */ ShaderTileMode tileMode);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__CLINEARGRADIENT_H__
