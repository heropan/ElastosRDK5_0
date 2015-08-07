
#include "animation/FloatKeyframeSet.h"

namespace Elastos {
namespace Droid {
namespace Animation {

CAR_INTERFACE_IMPL_2(FloatKeyframeSet, KeyframeSet, IFloatKeyframeSet)

FloatKeyframeSet::FloatKeyframeSet(
    /* [in] */ ArrayOf<IFloatKeyframe*>* keyframes)
    : KeyframeSet((ArrayOf<IKeyframe*>*)keyframes)
    , mFirstValue(0.0f)
    , mLastValue(0.0f)
    , mDeltaValue(0.0f)
    , mFirstTime(TRUE)
{
}

ECode FloatKeyframeSet::GetValue(
    /* [in] */ Float fraction,
    /* [out] */ IInterface** value)
{
    Float v;
    GetFloatValue(fraction, &v);
    AutoPtr<IFloat> rst;
    CFloat::New(v, (IFloat**)&rst);
    *value = rst;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode FloatKeyframeSet::Clone(
    /* [out] */ IKeyframeSet** obj)
{
    Int32 numKeyframes = mKeyframes->GetLength();
    AutoPtr<ArrayOf<IFloatKeyframe*> > newKeyframes = ArrayOf<IFloatKeyframe*>::Alloc(numKeyframes);
    for (Int32 i = 0; i < numKeyframes; ++i) {
        AutoPtr<IFloatKeyframe> frame;
        (*mKeyframes)[i]->Clone((IKeyframe**)&frame);
        newKeyframes->Set(i, frame);
    }
    AutoPtr<IFloatKeyframeSet> newSet = new FloatKeyframeSet(newKeyframes);
    *obj = newSet;
    REFCOUNT_ADD(*obj);
    return NOERROR;
}

ECode FloatKeyframeSet::GetFloatValue(
    /* [in] */ Float frc,
    /* [out] */ Float* value)
{
    Float fraction = frc;
    if (mNumKeyframes == 2) {
        if (mFirstTime) {
            mFirstTime = FALSE;
            AutoPtr<IFloatKeyframe> frameTmp = IFloatKeyframe::Probe((*mKeyframes)[0]);
            frameTmp->GetFloatValue(&mFirstValue);
            frameTmp = IFloatKeyframe::Probe((*mKeyframes)[1]);
            frameTmp->GetFloatValue(&mLastValue);
            mDeltaValue = mLastValue - mFirstValue;
        }

        if (mInterpolator != NULL) {
             mInterpolator->GetInterpolation(fraction, &fraction);
        }
        if (mEvaluator == NULL) {
            *value = mFirstValue + fraction * mDeltaValue;
            return NOERROR;
        } else {
            AutoPtr<IFloat> obj, fValue, lValue;
            CFloat::New(mFirstValue, (IFloat**)&fValue);
            CFloat::New(mLastValue, (IFloat**)&lValue);
            mEvaluator->Evaluate(fraction, fValue, lValue, (IInterface**)&obj);
            assert(obj != NULL);

            obj->GetValue(value);
            return NOERROR;
        }
    }

    if (fraction <= 0.0f) {
        AutoPtr<IFloatKeyframe> prevKeyframe = IFloatKeyframe::Probe((*mKeyframes)[0]);
        AutoPtr<IFloatKeyframe> nextKeyframe = IFloatKeyframe::Probe((*mKeyframes)[1]);
        Float prevValue;
        prevKeyframe->GetFloatValue(&prevValue);
        Float nextValue;
        nextKeyframe->GetFloatValue(&nextValue);
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
            return (prevValue + intervalFraction * (nextValue - prevValue));
        }

        AutoPtr<IFloat> obj, pValue, nValue;
        CFloat::New(prevValue, (IFloat**)&pValue);
        CFloat::New(nextValue, (IFloat**)&nValue);
        mEvaluator->Evaluate(intervalFraction, pValue, nValue, (IInterface**)&obj);
        assert(obj != NULL);
        obj->GetValue(value);

        return NOERROR;
    } else if (fraction >= 1.0f) {
        AutoPtr<IFloatKeyframe> prevKeyframe =
                (IFloatKeyframe*)((*mKeyframes)[mNumKeyframes - 2]->Probe(EIID_IFloatKeyframe));

        AutoPtr<IFloatKeyframe> nextKeyframe =
                (IFloatKeyframe*)((*mKeyframes)[mNumKeyframes - 1]->Probe(EIID_IFloatKeyframe));

        Float prevValue;
        prevKeyframe->GetFloatValue(&prevValue);
        Float nextValue;
        nextKeyframe->GetFloatValue(&nextValue);
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
            *value = prevValue + intervalFraction * (nextValue - prevValue);
            return NOERROR;
        }

        AutoPtr<IFloat> obj, pValue, nValue;
        CFloat::New(prevValue, (IFloat**)&pValue);
        CFloat::New(nextValue, (IFloat**)&nValue);
        mEvaluator->Evaluate(intervalFraction, pValue, nValue, (IInterface**)&obj);
        assert(obj != NULL);
        obj->GetValue(value);

        return NOERROR;
    }

    AutoPtr<IFloatKeyframe> prevKeyframe = (IFloatKeyframe*)((*mKeyframes)[0]->Probe(EIID_IFloatKeyframe));
    for (Int32 i = 1; i < mNumKeyframes; ++i) {
        AutoPtr<IFloatKeyframe> nextKeyframe = (IFloatKeyframe*)((*mKeyframes)[i]->Probe(EIID_IFloatKeyframe));
        Float nFraction;
        nextKeyframe->GetFraction(&nFraction);
        if (fraction < nFraction) {
            AutoPtr<ITimeInterpolator> interpolator;
            nextKeyframe->GetInterpolator((ITimeInterpolator**)&interpolator);
            if (interpolator != NULL) {
                interpolator->GetInterpolation(fraction, &fraction);
            }

            nextKeyframe->GetFraction(&nFraction);
            Float pFraction;
            prevKeyframe->GetFraction(&pFraction);
            Float intervalFraction = (fraction - pFraction) / (nFraction - pFraction);
            Float prevValue;
            prevKeyframe->GetFloatValue(&prevValue);
            Float nextValue;
            nextKeyframe->GetFloatValue(&nextValue);

            if (mEvaluator == NULL) {
                *value = prevValue + intervalFraction * (nextValue - prevValue);
            }

            AutoPtr<IFloat> obj, pValue, nValue;
            CFloat::New(prevValue, (IFloat**)&pValue);
            CFloat::New(nextValue, (IFloat**)&nValue);
            mEvaluator->Evaluate(intervalFraction, pValue, nValue, (IInterface**)&obj);

            assert(obj != NULL);
            obj->GetValue(value);

            return NOERROR;
        }

        prevKeyframe = nextKeyframe;
    }

    // shouldn't get here
    AutoPtr<IFloatKeyframe> frame =
            (IFloatKeyframe*)((*mKeyframes)[mNumKeyframes - 1]->Probe(EIID_IFloatKeyframe));
    AutoPtr<IFloat> obj;
    frame->GetValue((IInterface**)&obj);
    assert(obj != NULL);
    obj->GetValue(value);
    return NOERROR;
}

ECode FloatKeyframeSet::GetKeyframes(
    /* [out, callee] */ ArrayOf<IKeyframe*>** frames)
{
    VALIDATE_NOT_NULL(frames);
    *frames = mKeyframes;
    REFCOUNT_ADD(*frames);
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
