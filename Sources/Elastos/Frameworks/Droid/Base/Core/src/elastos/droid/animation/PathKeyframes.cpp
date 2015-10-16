
#include "elastos/droid/animation/PathKeyframes.h"
#include <elastos/core/Math.h>

// using Elastos::Droid::Graphics::CPointF;
using Elastos::Droid::Graphics::EIID_IPointF;
using Elastos::Core::CFloat;
using Elastos::Core::IFloat;
using Elastos::Core::EIID_IFloat;
using Elastos::Core::CInteger32;
using Elastos::Core::IInteger32;
using Elastos::Core::EIID_IInteger32;
using Elastos::Core::EIID_ICloneable;

namespace Elastos {
namespace Droid {
namespace Animation {

CAR_INTERFACE_IMPL(PathKeyframes::SimpleKeyframes, Object, IKeyframes);
PathKeyframes::SimpleKeyframes::~SimpleKeyframes()
{
}

ECode PathKeyframes::SimpleKeyframes::SetEvaluator(
    /* [in] */ ITypeEvaluator* evaluator)
{
    return NOERROR;
}

ECode PathKeyframes::SimpleKeyframes::InvalidateCache()
{
    return NOERROR;
}

ECode PathKeyframes::SimpleKeyframes::GetKeyframes(
    /* [out, callee] */ ArrayOf<IKeyframe*>** list)
{
    VALIDATE_NOT_NULL(list);
    *list = EMPTY_KEYFRAMES;
    REFCOUNT_ADD(*list);
    return NOERROR;
}

ECode PathKeyframes::SimpleKeyframes::Clone(
    /* [out] */ IInterface** values)
{
    VALIDATE_NOT_NULL(values);
    AutoPtr<IKeyframes> clone;
    assert(0 && "TODO");
    // try {
    //     clone = (Keyframes) super.clone();
    // } catch (CloneNotSupportedException e) {}
    *values = clone;
    REFCOUNT_ADD(*values);
    return NOERROR;
}

CAR_INTERFACE_IMPL(PathKeyframes::Int32KeyframesBase, SimpleKeyframes, IInt32Keyframes);
PathKeyframes::Int32KeyframesBase::~Int32KeyframesBase()
{
}

ECode PathKeyframes::Int32KeyframesBase::GetType(
    /* [out] */ InterfaceID* type)
{
    VALIDATE_NOT_NULL(type);
    *type = EIID_IInteger32;
    return NOERROR;
}

ECode PathKeyframes::Int32KeyframesBase::GetValue(
    /* [in] */ Float fraction,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);
    Int32 v = 0;
    GetInt32Value(fraction, &v);
    return CInteger32::New(v, (IInteger32**)value);
}

CAR_INTERFACE_IMPL(PathKeyframes::FloatKeyframesBase, SimpleKeyframes, IFloatKeyframes);
PathKeyframes::FloatKeyframesBase::~FloatKeyframesBase()
{
}

ECode PathKeyframes::FloatKeyframesBase::GetType(
    /* [out] */ InterfaceID* type)
{
    VALIDATE_NOT_NULL(type);
    *type = EIID_IFloat;
    return NOERROR;
}

ECode PathKeyframes::FloatKeyframesBase::GetValue(
    /* [in] */ Float fraction,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);
    Float f = 0.f;
    GetFloatValue(fraction, &f);
    return CFloat::New(f, (IFloat**)value);
}

ECode PathKeyframes::FloatKeyframesBase1::GetFloatValue(
    /* [in] */ Float fraction,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<IPointF> pointF;
    GetValue(fraction, (IInterface**)&pointF);
    return pointF->GetX(value);
}

ECode PathKeyframes::FloatKeyframesBase2::GetFloatValue(
    /* [in] */ Float fraction,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<IPointF> pointF;
    GetValue(fraction, (IInterface**)&pointF);
    return pointF->GetY(value);
}

ECode PathKeyframes::Int32KeyframesBase1::GetInt32Value(
    /* [in] */ Float fraction,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<IPointF> pointF;
    GetValue(fraction, (IInterface**)&pointF);
    Float v = 0.f;
    pointF->GetX(&v);
    *value = Elastos::Core::Math::Round(v);
    return NOERROR;
}

ECode PathKeyframes::Int32KeyframesBase2::GetInt32Value(
    /* [in] */ Float fraction,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<IPointF> pointF;
    GetValue(fraction, (IInterface**)&pointF);
    Float v = 0.f;
    pointF->GetY(&v);
    *value = Elastos::Core::Math::Round(v);
    return NOERROR;
}


const Int32 PathKeyframes::FRACTION_OFFSET = 0;
const Int32 PathKeyframes::X_OFFSET = 1;
const Int32 PathKeyframes::Y_OFFSET = 2;
const Int32 PathKeyframes::NUM_COMPONENTS = 3;
AutoPtr<ArrayOf<IKeyframe*> > PathKeyframes::EMPTY_KEYFRAMES;
Boolean PathKeyframes::sInit = svInit();

CAR_INTERFACE_IMPL_3(PathKeyframes, Object, IKeyframes, IPathKeyframes, ICloneable);
PathKeyframes::PathKeyframes(
    /* [in] */ IPath* path)
{
    constructor(path, 0.5f);
}

PathKeyframes::PathKeyframes(
    /* [in] */ IPath* path,
    /* [in] */ Float error)
{
    constructor(path, error);
}

ECode PathKeyframes::constructor(
    /* [in] */ IPath* path,
    /* [in] */ Float error)
{
    assert(0 && "TODO");
    // CPointF::New((IPointF**)&mTempPointF);
    Boolean isEmpty = FALSE;
    if (path == NULL || (path->IsEmpty(&isEmpty), isEmpty)) {
        // throw new IllegalArgumentException("The path must not be null or empty");
        assert(0);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return path->Approximate(error, (ArrayOf<Float>**)&mKeyframeData);
}

ECode PathKeyframes::GetKeyframes(
    /* [out, callee] */ ArrayOf<IKeyframe*>** list)
{
    VALIDATE_NOT_NULL(list);
    *list = EMPTY_KEYFRAMES;
    REFCOUNT_ADD(*list);
    return NOERROR;
}

ECode PathKeyframes::GetValue(
    /* [in] */ Float fraction,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);
    Int32 numPoints = mKeyframeData->GetLength() / 3;
    if (fraction < 0) {
        return InterpolateInRange(fraction, 0, 1, (IPointF**)value);
    } else if (fraction > 1) {
        return InterpolateInRange(fraction, numPoints - 2, numPoints - 1, (IPointF**)value);
    } else if (fraction == 0) {
        return PointForIndex(0, (IPointF**)value);
    } else if (fraction == 1) {
        return PointForIndex(numPoints - 1, (IPointF**)value);
    } else {
        // Binary search for the correct section
        Int32 low = 0;
        Int32 high = numPoints - 1;

        while (low <= high) {
            Int32 mid = (low + high) / 2;
            Float midFraction = (*mKeyframeData)[(mid * NUM_COMPONENTS) + FRACTION_OFFSET];

            if (fraction < midFraction) {
                high = mid - 1;
            } else if (fraction > midFraction) {
                low = mid + 1;
            } else {
                return PointForIndex(mid, (IPointF**)value);
            }
        }

        // now high is below the fraction and low is above the fraction
        return InterpolateInRange(fraction, high, low, (IPointF**)value);
    }
}

ECode PathKeyframes::InterpolateInRange(
    /* [in] */ Float fraction,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [out] */ IPointF** value)
{
    VALIDATE_NOT_NULL(value);
    Int32 startBase = (startIndex * NUM_COMPONENTS);
    Int32 endBase = (endIndex * NUM_COMPONENTS);

    Float startFraction = (*mKeyframeData)[startBase + FRACTION_OFFSET];
    Float endFraction = (*mKeyframeData)[endBase + FRACTION_OFFSET];

    Float intervalFraction = (fraction - startFraction)/(endFraction - startFraction);

    Float startX = (*mKeyframeData)[startBase + X_OFFSET];
    Float endX = (*mKeyframeData)[endBase + X_OFFSET];
    Float startY = (*mKeyframeData)[startBase + Y_OFFSET];
    Float endY = (*mKeyframeData)[endBase + Y_OFFSET];

    Float x = Interpolate(intervalFraction, startX, endX);
    Float y = Interpolate(intervalFraction, startY, endY);

    mTempPointF->Set(x, y);
    *value = mTempPointF;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode PathKeyframes::InvalidateCache()
{
    return NOERROR;
}

ECode PathKeyframes::SetEvaluator(
    /* [in] */ ITypeEvaluator* evaluator)
{
    return NOERROR;
}

ECode PathKeyframes::GetType(
    /* [out] */ InterfaceID* type)
{
    VALIDATE_NOT_NULL(type);
    *type = EIID_IPointF;
    return NOERROR;
}

ECode PathKeyframes::Clone(
    /* [out] */ IInterface** frames)
{
    VALIDATE_NOT_NULL(frames);
    AutoPtr<IKeyframes> clone;
    assert(0 && "TODO");
    // try {
    //     clone = (Keyframes) super.clone();
    // } catch (CloneNotSupportedException e) {}
    *frames = clone;
    REFCOUNT_ADD(*frames);
    return NOERROR;
}

ECode PathKeyframes::PointForIndex(
    /* [in] */ Int32 index,
    /* [out] */ IPointF** pf)
{
    VALIDATE_NOT_NULL(pf);
    Int32 base = (index * NUM_COMPONENTS);
    Int32 xOffset = base + X_OFFSET;
    Int32 yOffset = base + Y_OFFSET;
    mTempPointF->Set((*mKeyframeData)[xOffset], (*mKeyframeData)[yOffset]);
    *pf = mTempPointF;
    REFCOUNT_ADD(*pf);
    return NOERROR;
}

Float PathKeyframes::Interpolate(
    /* [in] */ Float fraction,
    /* [in] */ Float startValue,
    /* [in] */ Float endValue)
{
    Float diff = endValue - startValue;
    return startValue + (diff * fraction);
}

AutoPtr<IFloatKeyframes> PathKeyframes::CreateXFloatKeyframes()
{
    return new FloatKeyframesBase1();
}

AutoPtr<IFloatKeyframes> PathKeyframes::CreateYFloatKeyframes()
{
    return new FloatKeyframesBase2();
}

AutoPtr<IInt32Keyframes> PathKeyframes::CreateXInt32Keyframes()
{
    return new Int32KeyframesBase1();
}

AutoPtr<IInt32Keyframes> PathKeyframes::CreateYInt32Keyframes()
{
    return new Int32KeyframesBase2();
}

Boolean PathKeyframes::svInit()
{
    EMPTY_KEYFRAMES = ArrayOf<IKeyframe*>::Alloc(0);
    return TRUE;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
