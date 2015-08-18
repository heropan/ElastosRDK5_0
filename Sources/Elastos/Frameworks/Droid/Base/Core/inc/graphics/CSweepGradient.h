
#ifndef __ELASTOS_DROID_GRAPHICS_CSWEEPGRADIENT_H__
#define __ELASTOS_DROID_GRAPHICS_CSWEEPGRADIENT_H__

#include "_Elastos_Droid_Graphics_CSweepGradient.h"
#include "graphics/Shader.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CSweepGradient), public Shader
{
public:
    /**
     * A subclass of Shader that draws a sweep gradient around a center point.
     *
     * @param cx       The x-coordinate of the center
     * @param cy       The y-coordinate of the center
     * @param colors   The colors to be distributed between around the center.
     *                 There must be at least 2 colors in the array.
     * @param positions May be NULL. The relative position of
     *                 each corresponding color in the colors array, beginning
     *                 with 0 and ending with 1.0. If the values are not
     *                 monotonic, the drawing may produce unexpected results.
     *                 If positions is NULL, then the colors are automatically
     *                 spaced evenly.
     */
    CARAPI constructor(
        /* [in] */ Float cx,
        /* [in] */ Float cy,
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ ArrayOf<Float>* positions);

    /**
     * A subclass of Shader that draws a sweep gradient around a center point.
     *
     * @param cx       The x-coordinate of the center
     * @param cy       The y-coordinate of the center
     * @param color0   The color to use at the start of the sweep
     * @param color1   The color to use at the end of the sweep
     */
    CARAPI constructor(
        /* [in] */ Float cx,
        /* [in] */ Float cy,
        /* [in] */ Int32 color0,
        /* [in] */ Int32 color1);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetLocalMatrix(
        /* [in ,out] */ IMatrix* localM,
        /* [out] */ Boolean* result);

    CARAPI SetLocalMatrix(
        /* [in] */ IMatrix* localM);

private:
    static CARAPI_(Int32) NativeCreate1(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ ArrayOf<Float>* positions);

    static CARAPI_(Int32) NativeCreate2(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 color0,
        /* [in] */ Int32 color1);

    static CARAPI_(Int32) NativePostCreate1(
        /* [in] */ Int32 nativeShader,
        /* [in] */ Float cx,
        /* [in] */ Float cy,
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ ArrayOf<Float>* positions);

    static CARAPI_(Int32) NativePostCreate2(
        /* [in] */ Int32 nativeShader,
        /* [in] */ Float cx,
        /* [in] */ Float cy,
        /* [in] */ Int32 color0,
        /* [in] */ Int32 color1);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_CSWEEPGRADIENT_H__
