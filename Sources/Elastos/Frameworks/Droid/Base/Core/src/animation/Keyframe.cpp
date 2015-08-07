
#include "animation/Keyframe.h"

using Elastos::Core::ECLSID_CFloat;
using Elastos::Core::ECLSID_CInteger32;

namespace Elastos {
namespace Droid {
namespace Animation {

//==============================================================================
//                  Keyframe
//==============================================================================
CAR_INTERFACE_IMPL(Keyframe, Object, IKeyframe);
Keyframe::Keyframe()
    : mFraction(0.0f)
    , mHasValue(FALSE)
{
}

ECode Keyframe::HasValue(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mHasValue;
    return NOERROR;
}

ECode Keyframe::GetFraction(
    /* [out] */ Float* fraction)
{
    VALIDATE_NOT_NULL(fraction);
    *fraction = mFraction;
    return NOERROR;
}

ECode Keyframe::SetFraction(
    /* [in] */ Float fraction)
{
    mFraction = fraction;
    return NOERROR;
}

ECode Keyframe::GetInterpolator(
    /* [out] */ ITimeInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    *interpolator = mInterpolator;
    REFCOUNT_ADD(*interpolator);
    return NOERROR;
}

ECode Keyframe::SetInterpolator(
    /* [in] */ ITimeInterpolator* interpolator)
{
    mInterpolator = interpolator;
    return NOERROR;
}

ECode Keyframe::GetType(
    /* [out] */ ClassID* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mValueType;
    return NOERROR;
}

/**
 * Constructs a Keyframe object with the given time and value. The time defines the
 * time, as a proportion of an overall animation's duration, at which the value will hold true
 * for the animation. The value for the animation between keyframes will be calculated as
 * an interpolation between the values at those keyframes.
 *
 * @param fraction The time, expressed as a value between 0 and 1, representing the fraction
 * of time elapsed of the overall animation duration.
 * @param value The value that the object will animate to as the animation time approaches
 * the time in this keyframe, and the the value animated from as the time passes the time in
 * this keyframe.
 */
ECode Keyframe::OfInt32(
    /* [in] */ Float fraction,
    /* [in] */ Int32 value,
    /* [out] */ IKeyframe** obj)
{
    AutoPtr<IInt32Keyframe> rTmp = new Int32Keyframe(fraction, value);
    *obj = rTmp.Get();
    REFCOUNT_ADD(*obj)
    return NOERROR;
}

/**
 * Constructs a Keyframe object with the given time. The value at this time will be derived
 * from the target object when the animation first starts (note that this implies that keyframes
 * with no initial value must be used as part of an ObjectAnimator).
 * The time defines the
 * time, as a proportion of an overall animation's duration, at which the value will hold true
 * for the animation. The value for the animation between keyframes will be calculated as
 * an interpolation between the values at those keyframes.
 *
 * @sa ObjectAnimator
 *
 * @param fraction The time, expressed as a value between 0 and 1, representing the fraction
 * of time elapsed of the overall animation duration.
 */
ECode Keyframe::OfInt32(
    /* [in] */ Float fraction,
    /* [out] */ IKeyframe** obj)
{
    AutoPtr<IInt32Keyframe> rTmp = new Int32Keyframe(fraction);
    *obj = rTmp.Get();
    REFCOUNT_ADD(*obj)
    return NOERROR;
}

/**
 * Constructs a Keyframe object with the given time and value. The time defines the
 * time, as a proportion of an overall animation's duration, at which the value will hold true
 * for the animation. The value for the animation between keyframes will be calculated as
 * an interpolation between the values at those keyframes.
 *
 * @param fraction The time, expressed as a value between 0 and 1, representing the fraction
 * of time elapsed of the overall animation duration.
 * @param value The value that the object will animate to as the animation time approaches
 * the time in this keyframe, and the the value animated from as the time passes the time in
 * this keyframe.
 */
ECode Keyframe::OfFloat(
    /* [in] */ Float fraction,
    /* [in] */ Float value,
    /* [out] */ IKeyframe** obj)
{
    AutoPtr<IFloatKeyframe> rTmp = new FloatKeyframe(fraction, value);
    *obj = rTmp.Get();
    REFCOUNT_ADD(*obj)
    return NOERROR;
}

/**
 * Constructs a Keyframe object with the given time. The value at this time will be derived
 * from the target object when the animation first starts (note that this implies that keyframes
 * with no initial value must be used as part of an ObjectAnimator).
 * The time defines the
 * time, as a proportion of an overall animation's duration, at which the value will hold true
 * for the animation. The value for the animation between keyframes will be calculated as
 * an interpolation between the values at those keyframes.
 *
 * @sa ObjectAnimator
 *
 * @param fraction The time, expressed as a value between 0 and 1, representing the fraction
 * of time elapsed of the overall animation duration.
 */
ECode Keyframe::OfFloat(
    /* [in] */ Float fraction,
    /* [out] */ IKeyframe** obj)
{
    AutoPtr<IFloatKeyframe> rTmp = new FloatKeyframe(fraction);
    *obj = rTmp.Get();
    REFCOUNT_ADD(*obj)
    return NOERROR;
}

/**
 * Constructs a Keyframe object with the given time and value. The time defines the
 * time, as a proportion of an overall animation's duration, at which the value will hold true
 * for the animation. The value for the animation between keyframes will be calculated as
 * an interpolation between the values at those keyframes.
 *
 * @param fraction The time, expressed as a value between 0 and 1, representing the fraction
 * of time elapsed of the overall animation duration.
 * @param value The value that the object will animate to as the animation time approaches
 * the time in this keyframe, and the the value animated from as the time passes the time in
 * this keyframe.
 */
ECode Keyframe::OfObject(
    /* [in] */ Float fraction,
    /* [in] */ IInterface* value,
    /* [out] */ IKeyframe** obj)
{
    AutoPtr<IObjectKeyframe> rTmp = new ObjectKeyframe(fraction, value);
    *obj = rTmp.Get();
    REFCOUNT_ADD(*obj)
    return NOERROR;
}

/**
 * Constructs a Keyframe object with the given time. The value at this time will be derived
 * from the target object when the animation first starts (note that this implies that keyframes
 * with no initial value must be used as part of an ObjectAnimator).
 * The time defines the
 * time, as a proportion of an overall animation's duration, at which the value will hold true
 * for the animation. The value for the animation between keyframes will be calculated as
 * an interpolation between the values at those keyframes.
 *
 * @sa ObjectAnimator
 *
 * @param fraction The time, expressed as a value between 0 and 1, representing the fraction
 * of time elapsed of the overall animation duration.
 */
ECode Keyframe::OfObject(
    /* [in] */ Float fraction,
    /* [out] */ IKeyframe** obj)
{
    AutoPtr<IObjectKeyframe> rTmp = new ObjectKeyframe(fraction, NULL);
    *obj = rTmp.Get();
    REFCOUNT_ADD(*obj)
    return NOERROR;
}

//==============================================================================
//                  ObjectKeyframe
//==============================================================================
ObjectKeyframe::ObjectKeyframe(
    /* [in] */ Float fraction,
    /* [in] */ IInterface* value)
{
    mFraction = fraction;
    mValue = value;
    mHasValue = (mValue != NULL);
    if (mHasValue) {
        AutoPtr<IObject> obj = IObject::Probe(value);
        obj->GetClassID(&mValueType);
    } else {
        mValueType = ECLSID_CDummyObject;
    }
}

ECode ObjectKeyframe::GetValue(
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode ObjectKeyframe::SetValue(
    /* [in] */ IInterface* value)
{
    mValue = value;
    mHasValue = (mValue != NULL);
    return NOERROR;
}

ECode ObjectKeyframe::Clone(
    /* [out] */ IKeyframe** object)
{
    AutoPtr<IObjectKeyframe> kfClone = new ObjectKeyframe(mFraction, mValue);
    kfClone->SetInterpolator(Keyframe::GetInterpolator());
    *object = kfClone.Get();
    REFCOUNT_ADD(*object);
    return NOERROR;
}


//==============================================================================
//                  Int32Keyframe
//==============================================================================

Int32Keyframe::Int32Keyframe(
    /* [in] */ Float fraction,
    /* [in] */ Int32 value)
{
    mFraction = fraction;
    CInteger32::New(value, (IInteger32**)&mValue);
    mValueType = ECLSID_CFloat;
    mHasValue = TRUE;
}

Int32Keyframe::Int32Keyframe(
    /* [in] */ Float fraction)
{
    CInteger32::New(0, (IInteger32**)&mValue);
    mFraction = fraction;
    mValueType = ECLSID_CInteger32;
}

ECode Int32Keyframe::GetInt32Value(
    /* [in] */ Int32* value)
{
    mValue->GetValue(value);
    return NOERROR;
}

ECode Int32Keyframe::GetValue(
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode Int32Keyframe::SetValue(
    /* [in] */ IInterface* value)
{
    if (value != NULL && IInteger32::Probe(value) != NULL) {
        mValue = IInteger32::Probe(value);
        mHasValue = TRUE;
    } else{
        mHasValue = FALSE;
    }
    return NOERROR;
}

ECode Int32Keyframe::Clone(
    /* [out] */ IKeyframe** object)
{
    AutoPtr<IInt32Keyframe> kfClone;
    if(mHasValue)
    {
        Int32 value;
        mValue->GetValue(&value);
        kfClone = new Int32Keyframe(mFraction, value);
    } else {
        kfClone = new Int32Keyframe(mFraction);
    }
    kfClone->SetInterpolator(Keyframe::GetInterpolator());
    *object = kfClone.Get();
    REFCOUNT_ADD(*object)
    return NOERROR;
}

//==============================================================================
//                  FloatKeyframe
//==============================================================================

FloatKeyframe::FloatKeyframe(
    /* [in] */ Float fraction,
    /* [in] */ Float value)
{
    mFraction = fraction;
    CFloat::New(value, (IFloat**)&mValue);
    mValueType = ECLSID_CFloat;
    mHasValue = TRUE;
}

FloatKeyframe::FloatKeyframe(
    /* [in] */ Float fraction)
{
    CFloat::New(0.0, (IFloat**)&mValue);
    mFraction = fraction;
    mValueType = ECLSID_CFloat;
}

ECode FloatKeyframe::GetFloatValue(
    /* [in] */ Float* value)
{
    mValue->GetValue(value);
    return NOERROR;
}

ECode FloatKeyframe::GetValue(
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode FloatKeyframe::SetValue(
    /* [in] */ IInterface* value)
{
    if (value != NULL && IFloat::Probe(value) != NULL) {
        mValue = IFloat::Probe(value);
        mHasValue = TRUE;
    } else {
        mHasValue = FALSE;
    }
    return NOERROR;
}

ECode FloatKeyframe::Clone(
    /* [out] */ IKeyframe** object)
{
    AutoPtr<IFloatKeyframe> kfClone;
    if(mHasValue)
    {
        Float value;
        mValue->GetValue(&value);
        kfClone = new FloatKeyframe(mFraction, value);
    } else {
        kfClone = new FloatKeyframe(mFraction);
    }
    kfClone->SetInterpolator(Keyframe::GetInterpolator());
    *object = kfClone.Get();
    REFCOUNT_ADD(*object)
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
