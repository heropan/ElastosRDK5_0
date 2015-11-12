
#include "elastos/droid/graphics/CRadialGradient.h"
#include <skia/effects/SkGradientShader.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

const Int32 CRadialGradient::TYPE_COLORS_AND_POSITIONS = 1;
const Int32 CRadialGradient::TYPE_COLOR_CENTER_AND_COLOR_EDGE = 2;

CAR_OBJECT_IMPL(CRadialGradient);
CRadialGradient::CRadialGradient()
    : mType(0)
    , mX(0)
    , mY(0)
    , mRadius(0)
    , mCenterColor(0)
    , mEdgeColor(0)
    , mTileMode(-1)
{}

ECode CRadialGradient::constructor(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ ArrayOf<Float>* positions,
    /* [in] */ ShaderTileMode tile)
{
    if (radius <= 0) {
//        throw new IllegalArgumentException("radius must be > 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (colors.GetLength() < 2) {
//        throw new IllegalArgumentException("needs >= 2 number of colors");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (positions != NULL && colors.GetLength() != positions->GetLength()) {
//        throw new IllegalArgumentException("color and position arrays must be of equal length");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mType = TYPE_COLORS_AND_POSITIONS;
    mX = x;
    mY = y;
    mRadius = radius;
    mColors = const_cast<ArrayOf<Int32>* >(&colors);
    mPositions = positions;
    mTileMode = tile;
    Init(NativeCreate1(x, y, radius, colors, positions, tile));
    return NOERROR;
}

ECode CRadialGradient::constructor(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ Int32 centerColor,
    /* [in] */ Int32 edgeColor,
    /* [in] */ ShaderTileMode tile)
{
    if (radius <= 0) {
//        throw new IllegalArgumentException("radius must be > 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mType = TYPE_COLOR_CENTER_AND_COLOR_EDGE;
    mX = x;
    mY = y;
    mRadius = radius;
    mCenterColor = centerColor;
    mEdgeColor = edgeColor;
    mTileMode = tile;
    Init(NativeCreate2(x, y, radius, centerColor, edgeColor, tile));
    return NOERROR;
}

PInterface CRadialGradient::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Shader) {
        return reinterpret_cast<PInterface>((Shader*)this);
    }
    else if (riid == EIID_IRadialGradient) {
        return (IRadialGradient*)this;
    }
    return Shader::Probe(riid);
}

UInt32 CRadialGradient::AddRef()
{
    return Shader::AddRef();
}

UInt32 CRadialGradient::Release()
{
    return Shader::Release();
}

ECode CRadialGradient::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    return Shader::GetInterfaceID(object, iid);
}

ECode CRadialGradient::Copy(
    /* [out] */ IShader** shader)
{
    VALIDATE_NOT_NULL(shader);
    AutoPtr<IShader> copy;
    switch (mType) {
        case TYPE_COLORS_AND_POSITIONS:
            CRadialGradient::New(mX, mY, mRadius, *mColors->Clone(),
                    mPositions != NULL ? mPositions->Clone() : NULL, mTileMode, (IShader**)&copy);
            break;
        case TYPE_COLOR_CENTER_AND_COLOR_EDGE:
            CRadialGradient::New(mX, mY, mRadius, mCenterColor, mEdgeColor, mTileMode, (IShader**)&copy);
            break;
        default:
            // throw new IllegalArgumentException("RadialGradient should be created with either " +
            //         "colors and positions or center color and edge color");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    CopyLocalMatrix(copy);
    *shader = copy;
    REFCOUNT_ADD(*shader);
    return NOERROR;
}

Int64 CRadialGradient::NativeCreate1(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ ArrayOf<Float>* positions,
    /* [in] */ ShaderTileMode tile)
{
    SkPoint center;
    center.set(x, y);

    size_t count = (size_t)colors.GetLength();
    const Int32* colorValues = colors.GetPayload();

    // AutoJavaFloatArray autoPos(env, posArray, count);
    assert(positions->GetLength() >= count);
#ifdef SK_SCALAR_IS_FLOAT
    SkScalar* pos = positions->GetPayload();
#else
    #error Need to convert float array to SkScalar array before calling the following function.
#endif

    SkShader* shader = SkGradientShader::CreateRadial(center, radius,
            reinterpret_cast<const SkColor*>(colorValues), pos, count,
            static_cast<SkShader::TileMode>(tile));
    // env->ReleaseIntArrayElements(colorArray, const_cast<jint*>(colorValues),
    //                              JNI_ABORT);

    // ThrowIAE_IfNull(env, shader);
    assert(shader != NULL);
    return reinterpret_cast<Int64>(shader);
}

Int64 CRadialGradient::NativeCreate2(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ Int32 color0,
    /* [in] */ Int32 color1,
    /* [in] */ ShaderTileMode tile)
{
    SkPoint center;
    center.set(x, y);

    SkColor colors[2];
    colors[0] = color0;
    colors[1] = color1;

    SkShader* s = SkGradientShader::CreateRadial(center, radius, colors, NULL,
            2, (SkShader::TileMode)tile);
    assert(s != NULL);
    return reinterpret_cast<Int64>(s);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
