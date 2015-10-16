
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/graphics/CCamera.h"
#include "elastos/droid/graphics/Matrix.h"
#include "elastos/droid/graphics/Canvas.h"
#include <skia/utils/SkCamera.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

CAR_OBJECT_IMPL(CCamera);
CAR_INTERFACE_IMPL(CCamera, Object, ICamera);
CCamera::CCamera()
{
    NativeConstructor();
}

CCamera::~CCamera()
{
    NativeDestructor();
}

ECode CCamera::Save()
{
    ((Sk3DView*)mNativeInstance)->save();
    return NOERROR;
}

ECode CCamera::Restore()
{
    ((Sk3DView*)mNativeInstance)->restore();
    return NOERROR;
}

ECode CCamera::Translate(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z)
{
    ((Sk3DView*)mNativeInstance)->translate(x, y, z);
    return NOERROR;
}

ECode CCamera::RotateX(
    /* [in] */ Float deg)
{
    ((Sk3DView*)mNativeInstance)->rotateX(deg);
    return NOERROR;
}

ECode CCamera::RotateY(
    /* [in] */ Float deg)
{
    ((Sk3DView*)mNativeInstance)->rotateY(deg);
    return NOERROR;
}

ECode CCamera::RotateZ(
    /* [in] */ Float deg)
{
    ((Sk3DView*)mNativeInstance)->rotateZ(deg);
    return NOERROR;
}

/**
 * Applies a rotation transform around all three axis.
 *
 * @param x The angle of rotation around the X axis, in degrees
 * @param y The angle of rotation around the Y axis, in degrees
 * @param z The angle of rotation around the Z axis, in degrees
 *
 * @see #rotateX(Float)
 * @see #rotateY(Float)
 * @see #rotateZ(Float)
 */
ECode CCamera::Rotate(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z)
{
    ((Sk3DView*)mNativeInstance)->rotateX(x);
    ((Sk3DView*)mNativeInstance)->rotateY(y);
    ((Sk3DView*)mNativeInstance)->rotateZ(z);
    return NOERROR;
}

/**
 * Gets the x location of the camera.
 *
 * @see #setLocation(Float, Float, Float)
 */
ECode CCamera::GetLocationX(
    /* [out] */ Float* locationX)
{
    VALIDATE_NOT_NULL(locationX);
    *locationX = SkScalarToFloat(((Sk3DView*)mNativeInstance)->getCameraLocationX());
    return NOERROR;
}


/**
 * Gets the y location of the camera.
 *
 * @see #setLocation(Float, Float, Float)
 */
ECode CCamera::GetLocationY(
    /* [out] */ Float* locationY)
{
    VALIDATE_NOT_NULL(locationY);
    *locationY = SkScalarToFloat(((Sk3DView*)mNativeInstance)->getCameraLocationY());
    return NOERROR;
}


/**
 * Gets the z location of the camera.
 *
 * @see #setLocation(Float, Float, Float)
 */
ECode CCamera::GetLocationZ(
    /* [out] */ Float* locationZ)
{
    VALIDATE_NOT_NULL(locationZ);
    *locationZ = SkScalarToFloat(((Sk3DView*)mNativeInstance)->getCameraLocationZ());
    return NOERROR;
}

/**
 * Sets the location of the camera. The default location is set at
 * 0, 0, -8.
 *
 * @param x The x location of the camera
 * @param y The y location of the camera
 * @param z The z location of the camera
 */
ECode CCamera::SetLocation(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z)
{
    ((Sk3DView*)mNativeInstance)->setCameraLocation(x, y, z);
    return NOERROR;
}

ECode CCamera::GetMatrix(
    /* [in] */ IMatrix* matrix)
{
    NativeGetMatrix(((Matrix*)(IMatrix*)matrix->Probe(EIID_Matrix))->mNativeInstance);
    return NOERROR;
}

ECode CCamera::ApplyToCanvas(
    /* [in] */ ICanvas* canvas)
{
    assert(0 && "TODO");
    // Boolean is = FALSE;
    // if (canvas->IsHardwareAccelerated(&is), is) {
    //     if (mMatrix == NULL) mMatrix = new Matrix();
    //     GetMatrix(mMatrix);
    //     canvas->Concat(mMatrix);
    // } else {
    //     NativeApplyToCanvas(canvas->GetNativeCanvasWrapper());
    // }
    return NOERROR;
}

ECode CCamera::DotWithNormal(
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [in] */ Float dz,
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);

    SkScalar dot = ((Sk3DView*)mNativeInstance)->dotWithNormal(dx, dy, dz);
    *result = SkScalarToFloat(dot);
    return NOERROR;
}

void CCamera::NativeConstructor()
{
    mNativeInstance = (Int64)new Sk3DView;
}

void CCamera::NativeDestructor()
{
    delete (Sk3DView*)mNativeInstance;
}

void CCamera::NativeGetMatrix(
    /* [in] */ Int64 nativeMatrix)
{
    SkMatrix* native_matrix =  reinterpret_cast<SkMatrix*>(nativeMatrix);
    Sk3DView* v = reinterpret_cast<Sk3DView*>(mNativeInstance);
    v->getMatrix(native_matrix);
}

void CCamera::NativeApplyToCanvas(
    /* [in] */ Int64 nativeCanvas)
{
    assert(0 && "TODO");
    // android::SkCanvas* canvas = reinterpret_cast<android::Canvas*>(nativeCanvas)->getSkCanvas();
    // Sk3DView* v = reinterpret_cast<Sk3DView*>(mNativeInstance);
    // v->applyToCanvas(canvas);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos