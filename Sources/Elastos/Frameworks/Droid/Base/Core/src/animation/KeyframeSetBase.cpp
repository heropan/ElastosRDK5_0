#include "animation/KeyframeSetBase.h"

namespace Elastos {
namespace Droid {
namespace Animation {

KeyframeSetBase::KeyframeSetBase(
    /* [in] */ ArrayOf<IKeyframe*>* keyframes)
{
    assert(keyframes != NULL);
    mNumKeyframes = keyframes->GetLength();
    mKeyframes = keyframes->Clone();

    mFirstKeyframe = (*mKeyframes)[0];
    mLastKeyframe = (*mKeyframes)[mNumKeyframes - 1];
    mLastKeyframe->GetInterpolator((ITimeInterpolator**)&mInterpolator);
}

ECode KeyframeSetBase::SetEvaluator(
    /* [in] */ ITypeEvaluator* evaluator)
{
    mEvaluator = evaluator;
    return NOERROR;
}

AutoPtr<IInterface> KeyframeSetBase::GetValue(
    /* [in] */ Float fraction)
{
    // Special-case optimization for the common case of only two keyframes
    AutoPtr<IInterface> result;
    Float fractionTemp = fraction;
    if (mNumKeyframes == 2) {
        if (mInterpolator != NULL) {
            mInterpolator->GetInterpolation(fractionTemp, &fractionTemp);
        }
        AutoPtr<IInterface> fvTemp;
        mFirstKeyframe->GetValue((IInterface**)&fvTemp);
        AutoPtr<IInterface> lvTemp;
        mLastKeyframe->GetValue((IInterface**)&lvTemp);
        mEvaluator->Evaluate(fractionTemp, fvTemp, lvTemp, (IInterface**)&result);
        return result;
    }

    if (fractionTemp <= 0.0f) {
        AutoPtr<IKeyframe> nextKeyframe = (*mKeyframes)[1];
        AutoPtr<ITimeInterpolator> interpolator;
        nextKeyframe->GetInterpolator((ITimeInterpolator**)&interpolator);
        if (interpolator != NULL) {
            interpolator->GetInterpolation(fractionTemp, &fractionTemp);
        }

        Float prevFraction;
        mFirstKeyframe->GetFraction(&prevFraction);
        Float nextFraction;
        nextKeyframe->GetFraction(&nextFraction);
        Float intervalFraction = (fractionTemp - prevFraction) /
            (nextFraction - prevFraction);

        AutoPtr<IInterface> vTemp;
        mFirstKeyframe->GetValue((IInterface**)&vTemp);
        AutoPtr<IInterface> nvTemp;
        nextKeyframe->GetValue((IInterface**)&nvTemp);
        mEvaluator->Evaluate(intervalFraction, vTemp, nvTemp, (IInterface**)&result);
        return result;
    }
    else if (fractionTemp >= 1.0f) {
        AutoPtr<IKeyframe> prevKeyframe = (*mKeyframes)[mNumKeyframes - 2];
        AutoPtr<ITimeInterpolator> interpolator;
        mLastKeyframe->GetInterpolator((ITimeInterpolator**)&interpolator);
        if (interpolator != NULL) {
            interpolator->GetInterpolation(fractionTemp, &fractionTemp);
        }

        Float prevFraction;
        prevKeyframe->GetFraction(&prevFraction);
        Float lastFraction;
        mLastKeyframe->GetFraction(&lastFraction);
        Float intervalFraction = (fractionTemp - prevFraction) /
            (lastFraction - prevFraction);
        AutoPtr<IInterface> vTemp;
        prevKeyframe->GetValue((IInterface**)&vTemp);
        AutoPtr<IInterface> lvTemp;
        mLastKeyframe->GetValue((IInterface**)&lvTemp);
        mEvaluator->Evaluate(intervalFraction, vTemp, lvTemp, (IInterface**)&result);
        return result;
    }

    AutoPtr<IKeyframe> prevKeyframe = mFirstKeyframe;
    for (Int32 i = 1; i < mNumKeyframes; ++i) {
        AutoPtr<IKeyframe> nextKeyframe = (*mKeyframes)[i];
        Float nextFraction;
        nextKeyframe->GetFraction(&nextFraction);
        if (fractionTemp < nextFraction) {
            AutoPtr<ITimeInterpolator> interpolator;
            nextKeyframe->GetInterpolator((ITimeInterpolator**)&interpolator);
            if (interpolator != NULL) {
                interpolator->GetInterpolation(fractionTemp, &fractionTemp);
            }

            Float prevFraction;
            prevKeyframe->GetFraction(&prevFraction);
            Float nFraction;
            prevKeyframe->GetFraction(&nFraction);
            Float intervalFraction = (fractionTemp - prevFraction) /
                (nFraction - prevFraction);
            AutoPtr<IInterface> vTemp;
            prevKeyframe->GetValue((IInterface**)&vTemp);
            AutoPtr<IInterface> nvTemp;
            nextKeyframe->GetValue((IInterface**)&nvTemp);
            mEvaluator->Evaluate(intervalFraction, vTemp, nvTemp, (IInterface**)&result);
            return result;
        }
        prevKeyframe = nextKeyframe;
    }
    // shouldn't reach here
    mLastKeyframe->GetValue((IInterface**)&result);
    return result;
}

}// namespace Animation
}// namespace Droid
}// namespace Elastos
