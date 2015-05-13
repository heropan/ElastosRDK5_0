
#include "animation/Int32KeyframeSet.h"

using Elastos::Core::INumber;

namespace Elastos {
namespace Droid {
namespace Animation {

CAR_INTERFACE_IMPL_2(Int32KeyframeSet, IInt32KeyframeSet, IKeyframeSet)

Int32KeyframeSet::Int32KeyframeSet(
    /* [in] */ ArrayOf<IInt32Keyframe*>* keyframes)
    : KeyframeSetBase((ArrayOf<IKeyframe*>*)keyframes)
    , mFirstValue(0)
    , mLastValue(0)
    , mDeltaValue(0)
    , mFirstTime(TRUE)
{}

ECode Int32KeyframeSet::SetEvaluator(
    /* [in] */ ITypeEvaluator* evaluator)
{
    return KeyframeSetBase::SetEvaluator(evaluator);
}

/**
 * Gets the animated value, given the elapsed fraction of the animation (interpolated by the
 * animation's interpolator) and the evaluator used to calculate in-between values. This
 * function maps the input fraction to the appropriate keyframe interval and a fraction
 * between them and returns the interpolated value. Note that the input fraction may fall
 * outside the [0-1] bounds, if the animation's interpolator made that happen (e.g., a
 * spring interpolation that might send the fraction past 1.0). We handle this situation by
 * just using the two keyframes at the appropriate end when the value is outside those bounds.
 *
 * @param fraction The elapsed fraction of the animation
 * @return The animated value.
 */
ECode Int32KeyframeSet::GetValue(
    /* [in] */ Float fraction,
    /* [out] */ IInterface** value)
{
    Int32 vTemp;
    GetIntValue(fraction, &vTemp);
    AutoPtr<IInterface> iv;
    CInteger32::New(vTemp, (IInteger32**)&iv);
    *value = iv;
    INTERFACE_ADDREF(*value);
    return NOERROR;
}

ECode Int32KeyframeSet::GetIntValue(
    /* [in] */ Float frac,
    /* [out] */ Int32* value)
{
    Float fraction = frac;
    if (mNumKeyframes == 2) {
        if (mFirstTime) {
            mFirstTime = FALSE;
            AutoPtr<IInt32Keyframe> frameTmp = (IInt32Keyframe*)((*mKeyframes)[0]->Probe(EIID_IInt32Keyframe));
            frameTmp->GetInt32Value(&mFirstValue);

            frameTmp = (IInt32Keyframe*)((*mKeyframes)[1]->Probe(EIID_IInt32Keyframe));
            frameTmp->GetInt32Value(&mLastValue);;
            mDeltaValue = mLastValue - mFirstValue;
        }

        if (mInterpolator != NULL) {
            mInterpolator->GetInterpolation(fraction, &fraction);
        }

        if (mEvaluator == NULL) {
            *value = mFirstValue + (Int32)(fraction * mDeltaValue);
            return NOERROR;
        } else {
            AutoPtr<IInteger32> obj, fValue, lValue;
            CInteger32::New(mFirstValue, (IInteger32**)&fValue);
            CInteger32::New(mLastValue, (IInteger32**)&lValue);
            mEvaluator->Evaluate(fraction, fValue, lValue, (IInterface**)&obj);
            assert(obj != NULL);
            obj->GetValue(value);
            return NOERROR;
        }
    }

    if (fraction <= 0.0f) {
        AutoPtr<IInt32Keyframe> prevKeyframe = (IInt32Keyframe*)((*mKeyframes)[0]->Probe(EIID_IInt32Keyframe));
        AutoPtr<IInt32Keyframe> nextKeyframe = (IInt32Keyframe*)((*mKeyframes)[1]->Probe(EIID_IInt32Keyframe));
        Int32 prevValue;
        prevKeyframe->GetInt32Value(&prevValue);
        Int32 nextValue;
        nextKeyframe->GetInt32Value(&nextValue);
        Float prevFraction;
        prevKeyframe->GetFraction(&prevFraction);
        Float nextFraction;
        nextKeyframe->GetFraction(&nextFraction);
        AutoPtr<ITimeInterpolator> interpolator;
        nextKeyframe->GetInterpolator((ITimeInterpolator**)&interpolator);
        if (interpolator != NULL) {
            interpolator->GetInterpolation(fraction, &fraction);
        }

        Float intervalFraction = (fraction - prevFraction) / (nextFraction - prevFraction);
        if (mEvaluator == NULL) {
            *value = prevValue + (Int32)(intervalFraction * (nextValue - prevValue)) ;
            return NOERROR;
        }

        AutoPtr<IInteger32> obj, pValue, nValue;
        CInteger32::New(prevValue, (IInteger32**)&pValue);
        CInteger32::New(nextValue, (IInteger32**)&nValue);
        mEvaluator->Evaluate(intervalFraction, pValue, nValue, (IInterface**)&obj);

        assert(obj != NULL);
        obj->GetValue(value);
        return NOERROR;
    } else if (fraction >= 1.0f) {
        AutoPtr<IInt32Keyframe> prevKeyframe =
                (IInt32Keyframe*)((*mKeyframes)[mNumKeyframes - 2]->Probe(EIID_IInt32Keyframe));
        AutoPtr<IInt32Keyframe> nextKeyframe =
                (IInt32Keyframe*)((*mKeyframes)[mNumKeyframes - 1]->Probe(EIID_IInt32Keyframe));
        Int32 prevValue;
        prevKeyframe->GetInt32Value(&prevValue);
        Int32 nextValue;
        nextKeyframe->GetInt32Value(&nextValue);
        Float prevFraction;
        prevKeyframe->GetFraction(&prevFraction);
        Float nextFraction;
        nextKeyframe->GetFraction(&nextFraction);
        AutoPtr<ITimeInterpolator> interpolator;
        nextKeyframe->GetInterpolator((ITimeInterpolator**)&interpolator);
        if (interpolator != NULL) {
            interpolator->GetInterpolation(fraction, &fraction);
        }

        Float intervalFraction = (fraction - prevFraction) / (nextFraction - prevFraction);
        if (mEvaluator == NULL) {
            *value = prevValue + (Int32)(intervalFraction * (nextValue - prevValue));
            return NOERROR;
        }

        AutoPtr<IInteger32> obj, pValue, nValue;
        CInteger32::New(prevValue, (IInteger32**)&pValue);
        CInteger32::New(nextValue, (IInteger32**)&nValue);
        mEvaluator->Evaluate(intervalFraction, pValue, nValue, (IInterface**)&obj);

        assert(obj != NULL);
        obj->GetValue(value);
        return NOERROR;
    }

    AutoPtr<IInt32Keyframe> prevKeyframe = (IInt32Keyframe*)((*mKeyframes)[0]->Probe(EIID_IInt32Keyframe));
    for (Int32 i = 1; i < mNumKeyframes; ++i) {
        AutoPtr<IInt32Keyframe> nextKeyframe = (IInt32Keyframe*)((*mKeyframes)[i]->Probe(EIID_IInt32Keyframe));
        Float nFraction;
        nextKeyframe->GetFraction(&nFraction);
        if (fraction < nFraction) {
            AutoPtr<ITimeInterpolator> interpolator;
            nextKeyframe->GetInterpolator((ITimeInterpolator**)&interpolator);
            if (interpolator != NULL) {
                interpolator->GetInterpolation(fraction, &fraction);
            }

            Float pFraction, nFraction;
            prevKeyframe->GetFraction(&pFraction);
            nextKeyframe->GetFraction(&nFraction);
            Float intervalFraction = (fraction - pFraction) / (nFraction - pFraction);
            Int32 prevValue;
            prevKeyframe->GetInt32Value(&prevValue);
            Int32 nextValue;
            nextKeyframe->GetInt32Value(&nextValue);

            if (mEvaluator == NULL) {
                *value = prevValue + (Int32)(intervalFraction * (nextValue - prevValue));
                return NOERROR;
            }

            AutoPtr<IInteger32> obj, pValue, nValue;
            CInteger32::New(prevValue, (IInteger32**)&pValue);
            CInteger32::New(nextValue, (IInteger32**)&nValue);
            mEvaluator->Evaluate(intervalFraction, pValue, nValue, (IInterface**)&obj);

            assert(obj != NULL);
            obj->GetValue(value);
            return NOERROR;
        }

        prevKeyframe = nextKeyframe;
    }

    // shouldn't get here
    AutoPtr<IInt32Keyframe> obj = (IInt32Keyframe*)((*mKeyframes)[mNumKeyframes - 1]->Probe(EIID_IInt32Keyframe));
    AutoPtr<INumber> rst;
    obj->GetValue((IInterface**)&rst);
    rst->Int32Value(value);
    return NOERROR;
}

ECode Int32KeyframeSet::Clone(
    /* [out] */ IKeyframeSet** obj)
{
    Int32 numKeyframes = mKeyframes->GetLength();
    AutoPtr<ArrayOf<IInt32Keyframe*> > newKeyframes = ArrayOf<IInt32Keyframe*>::Alloc(numKeyframes);
    for (Int32 i = 0; i < numKeyframes; ++i) {
        AutoPtr<IInt32Keyframe> newone;
        (*mKeyframes)[i]->Clone((IKeyframe**)&newone);
        newKeyframes->Set(i, newone);
    }
    AutoPtr<IInt32KeyframeSet> newSet = new Int32KeyframeSet(newKeyframes);
    *obj = newSet;
    INTERFACE_ADDREF(*obj);
    return NOERROR;
}

ECode Int32KeyframeSet::GetKeyframes(
    /* [out, callee] */ ArrayOf<IKeyframe*>** frames)
{
    VALIDATE_NOT_NULL(frames);
    *frames = mKeyframes;
    INTERFACE_ADDREF(*frames);
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
