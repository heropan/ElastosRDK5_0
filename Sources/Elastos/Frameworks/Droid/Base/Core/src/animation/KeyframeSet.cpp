
#include "animation/KeyframeSet.h"
#include "animation/Int32KeyframeSet.h"
#include "animation/FloatKeyframeSet.h"
#include <elastos/core/Math.h>
namespace Elastos {
namespace Droid {
namespace Animation {

CAR_INTERFACE_IMPL(KeyframeSet, IKeyframeSet)

KeyframeSet::KeyframeSet(
    /* [in] */ ArrayOf<IKeyframe*>* keyframes)
    : KeyframeSetBase(keyframes)
{}

AutoPtr<IKeyframeSet> KeyframeSet::OfInt32(
    /* [in] */ ArrayOf<Int32>* values)
{
    assert(values != NULL);
    Int32 numKeyframes = values->GetLength();
    AutoPtr<ArrayOf<IInt32Keyframe*> > keyframes =
            ArrayOf<IInt32Keyframe*>::Alloc(Elastos::Core::Math::Max(numKeyframes, 2));

    if (numKeyframes == 1) {
        AutoPtr<IInt32Keyframe> temp;
        Keyframe::OfInt32(0.0f, (IKeyframe**)&temp);
        keyframes->Set(0, temp); temp = NULL;
        Keyframe::OfInt32(1.0f, (*values)[0], (IKeyframe**)&temp);
        keyframes->Set(1, temp);
    } else {
        AutoPtr<IInt32Keyframe> temp;
        Keyframe::OfInt32(0.0f, (*values)[0], (IKeyframe**)&temp);
        keyframes->Set(0, temp);
        for (Int32 i = 1; i < numKeyframes; ++i) {
            temp = NULL;
            Keyframe::OfInt32((Float) i / (numKeyframes - 1), (*values)[i], (IKeyframe**)&temp);
            keyframes->Set(i, temp);
        }
    }

    return new Int32KeyframeSet(keyframes);
}

AutoPtr<IKeyframeSet> KeyframeSet::OfFloat(
    /* [in] */ ArrayOf<Float>* values)
{
    assert(values != NULL);

    Int32 numKeyframes = values->GetLength();
    AutoPtr<ArrayOf<IFloatKeyframe*> > keyframes =
            ArrayOf<IFloatKeyframe*>::Alloc(Elastos::Core::Math::Max(numKeyframes, 2));

    if (numKeyframes == 1) {
        AutoPtr<IFloatKeyframe> temp;
        Keyframe::OfFloat(0.0f, (IKeyframe**)&temp);
        keyframes->Set(0, temp); temp = NULL;
        Keyframe::OfFloat(1.0f, (*values)[0], (IKeyframe**)&temp);
        keyframes->Set(1, temp);
    } else {
        AutoPtr<IFloatKeyframe> temp;
        Keyframe::OfFloat(0.0f, (*values)[0], (IKeyframe**)&temp);
        keyframes->Set(0, temp);
        for (Int32 i = 1; i < numKeyframes; ++i) {
            temp = NULL;
            Keyframe::OfFloat((Float) i / (numKeyframes - 1), (*values)[i], (IKeyframe**)&temp);
            keyframes->Set(i, temp);
        }
    }

    return new FloatKeyframeSet(keyframes);
}

AutoPtr<IKeyframeSet> KeyframeSet::OfKeyframe(
    /* [in] */ ArrayOf<IKeyframe*>* keyframes)
{
    assert(keyframes != NULL);
    // if all keyframes of same primitive type, create the appropriate KeyframeSet
    Int32 numKeyframes = keyframes->GetLength();
    Boolean hasFloat = FALSE;
    Boolean hasInt = FALSE;
    Boolean hasOther = FALSE;
    for (Int32 i = 0; i < numKeyframes; ++i) {
        if (IFloatKeyframe::Probe((*keyframes)[i])) {
            hasFloat = TRUE;
        } else if (IInt32Keyframe::Probe((*keyframes)[i])) {
            hasInt = TRUE;
        } else {
            hasOther = TRUE;
        }
    }
    if (hasFloat && !hasInt && !hasOther) {
        AutoPtr<ArrayOf<IFloatKeyframe*> > floatKeyframes =
                ArrayOf<IFloatKeyframe*>::Alloc(numKeyframes);

        for (Int32 i = 0; i < numKeyframes; ++i) {
            AutoPtr<IFloatKeyframe> temp = IFloatKeyframe::Probe((*keyframes)[i]);
            floatKeyframes->Set(i, temp);
        }

        return new FloatKeyframeSet(floatKeyframes);
    } else if (hasInt && !hasFloat && !hasOther) {
        AutoPtr<ArrayOf<IInt32Keyframe*> > intKeyframes =
                ArrayOf<IInt32Keyframe*>::Alloc(numKeyframes);

        for (Int32 i = 0; i < numKeyframes; ++i) {
            AutoPtr<IInt32Keyframe> temp = IInt32Keyframe::Probe((*keyframes)[i]);
            intKeyframes->Set(i, temp);
        }

        return new Int32KeyframeSet(intKeyframes);
    } else {
        return new KeyframeSet(keyframes);
    }
}

AutoPtr<IKeyframeSet> KeyframeSet::OfObject(
    /* [in] */ ArrayOf<IInterface*>* values)
{
    assert(values != NULL);

    Int32 numKeyframes = values->GetLength();
    AutoPtr<ArrayOf<IObjectKeyframe*> > keyframes =
            ArrayOf<IObjectKeyframe*>::Alloc(Elastos::Core::Math::Max(numKeyframes, 2));

    if (numKeyframes == 1) {
        AutoPtr<IObjectKeyframe> temp;
        Keyframe::OfObject(0.0f, (IKeyframe**)&temp);
        keyframes->Set(0, temp); temp = NULL;
        Keyframe::OfObject(1.0f, (*values)[0], (IKeyframe**)&temp);
        keyframes->Set(1, temp);
    } else {
        AutoPtr<IObjectKeyframe> temp;
        Keyframe::OfObject(0.0f, (*values)[0], (IKeyframe**)&temp);
        keyframes->Set(0, temp);
        for (Int32 i = 1; i < numKeyframes; ++i) {
            temp = NULL;
            Keyframe::OfObject((Float) i / (numKeyframes - 1), (*values)[i], (IKeyframe**)&temp);
            keyframes->Set(i, temp);
        }
    }

    return new KeyframeSet((ArrayOf<IKeyframe*>*)(keyframes.Get()));
}

ECode KeyframeSet::SetEvaluator(
    /* [in] */ ITypeEvaluator* evaluator)
{
    return KeyframeSetBase::SetEvaluator(evaluator);
}

ECode KeyframeSet::GetValue(
    /* [in] */ Float fraction,
    /* [out] */ IInterface** value)
{
    AutoPtr<IInterface> temp = KeyframeSetBase::GetValue(fraction);
    *value = temp;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode KeyframeSet::Clone(
    /* [out] */ IKeyframeSet** object)
{
    Int32 numKeyframes = mKeyframes->GetLength();
    AutoPtr<ArrayOf<IKeyframe*> > newKeyframes = ArrayOf<IKeyframe*>::Alloc(numKeyframes);
    for (Int32 i = 0; i < numKeyframes; ++i) {
        AutoPtr<IKeyframe> temp;
        (*mKeyframes)[i]->Clone((IKeyframe**)&temp);
        newKeyframes->Set(i, temp);
    }
    AutoPtr<IKeyframeSet> newSet = new KeyframeSet(newKeyframes);
    *object = newSet;
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode KeyframeSet::GetKeyframes(
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
