
#include "graphics/CColorMatrixColorFilter.h"
#include <skia/effects/SkColorMatrixFilter.h>
#include <skia/core/SkColorFilter.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

CAR_OBJECT_IMPL(CColorMatrixColorFilter);
CAR_INTERFACE_IMPL(CColorMatrixColorFilter, ColorFilter, IColorMatrixColorFilter);
ECode CColorMatrixColorFilter::constructor(
    /* [in] */ IColorMatrix* matrix)
{
    AutoPtr< ArrayOf<Float> > array;
    matrix->GetArray((ArrayOf<Float>**)&array);
    mNativeInstance = NativeColorMatrixFilter(*array);
    mNativeColorFilter = NColorMatrixFilter(mNativeInstance, *array);
    return NOERROR;
}

ECode CColorMatrixColorFilter::constructor(
    /* [in] */ const ArrayOf<Float>& array)
{
    if (array.GetLength() < 20) {
        // throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    mNativeInstance = NativeColorMatrixFilter(array);
    mNativeColorFilter = NColorMatrixFilter(mNativeInstance, array);
    return NOERROR;
}

PInterface CColorMatrixColorFilter::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_ColorFilter) {
        return reinterpret_cast<PInterface>((ColorFilter*)this);
    }
    else if (riid == EIID_IColorMatrixColorFilter) {
        return (IColorMatrixColorFilter*)this;
    }
    return ColorFilter::Probe(riid);
}

ECode CColorMatrixColorFilter::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);

    if (object == reinterpret_cast<PInterface>((ColorFilter*)this)) {
        *iid = EIID_ColorFilter;
        return NOERROR;
    }
    else if (object == (IColorMatrixColorFilter*)this) {
        *iid = EIID_IColorMatrixColorFilter;
        return NOERROR;
    }
    return ColorFilter::GetInterfaceID(object, iid);
}

Int32 CColorMatrixColorFilter::NativeColorMatrixFilter(
    /* [in] */ const ArrayOf<Float>& _array)
{
    if (_array.GetLength() < 20) {
        sk_throw();
    }
    const Float* src = _array.GetPayload();

#ifdef SK_SCALAR_IS_FIXED
    SkFixed array[20];
    for (Int32 i = 0; i < 20; i++) {
        array[i] = SkFloatToScalar(src[i]);
    }
    return (Int32)new SkColorMatrixFilter(array);
#else
    return (Int32)new SkColorMatrixFilter(src);
#endif
}

Int32 CColorMatrixColorFilter::NColorMatrixFilter(
    /* [in] */ Int32 nativeFilter,
    /* [in] */ const ArrayOf<Float>& array)
{
#ifdef USE_OPENGL_RENDERER
    const Float* src = array.GetPayload();

    float* colorMatrix = new float[16];
    memcpy(colorMatrix, src, 4 * sizeof(float));
    memcpy(&colorMatrix[4], &src[5], 4 * sizeof(float));
    memcpy(&colorMatrix[8], &src[10], 4 * sizeof(float));
    memcpy(&colorMatrix[12], &src[15], 4 * sizeof(float));

    float* colorVector = new float[4];
    colorVector[0] = src[4];
    colorVector[1] = src[9];
    colorVector[2] = src[14];
    colorVector[3] = src[19];

    return (Int32)new SkiaColorMatrixFilter((SkColorFilter*)nativeFilter, colorMatrix, colorVector);
#else
    return 0;
#endif
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
