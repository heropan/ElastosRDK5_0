
#include "elastos/droid/gesture/Instance.h"
#include "elastos/droid/gesture/GestureUtils.h"
#include <elastos/core/Math.h>

using Elastos::Utility::IArrayList;

namespace Elastos {
namespace Droid {
namespace Gesture {

static AutoPtr<ArrayOf<Float> > InitORIENTATIONS()
{
    AutoPtr<ArrayOf<Float> > orientations = ArrayOf<Float>::Alloc(10);
    (*orientations)[0] = 0.0;
    (*orientations)[1] = (Float)(Elastos::Core::Math::PI / 4.0);
    (*orientations)[2] = (Float)(Elastos::Core::Math::PI / 2.0);
    (*orientations)[3] = (Float)(Elastos::Core::Math::PI * 3.0 / 4.0);
    (*orientations)[4] = (Float)(Elastos::Core::Math::PI);
    (*orientations)[5] = -0.0;
    (*orientations)[6] = (Float)(-Elastos::Core::Math::PI / 4.0);
    (*orientations)[7] = (Float)(-Elastos::Core::Math::PI / 2.0);
    (*orientations)[8] = (Float)(-Elastos::Core::Math::PI * 3.0 / 4.0);
    (*orientations)[9] = (Float)(-Elastos::Core::Math::PI);
    return orientations;
}

const AutoPtr<ArrayOf<Float> > Instance::ORIENTATIONS = InitORIENTATIONS();

CAR_INTERFACE_IMPL(Instance, Object, IInstance);

Instance::Instance(
    /* [in] */ Int64 id,
    /* [in] */ ArrayOf<Float> *sample,
    /* [in] */ const String& sampleName)
    : mVector(sample)
    , mLabel(sampleName)
    , mId(id)
{}

Instance::Instance()
{}

Instance::~Instance()
{}

ECode Instance::constructor()
{
    return NOERROR;
}

ECode Instance::Normalize()
{
    AutoPtr<ArrayOf<Float> > sample = mVector;
    Float sum = 0;

    Int32 size = sample->GetLength();
    for (Int32 i = 0; i < size; i++) {
        sum += (*sample)[i] * (*sample)[i];
    }

    Float magnitude = (Float)Elastos::Core::Math::Sqrt(sum);
    for (Int32 i = 0; i < size; i++) {
        (*sample)[i] /= magnitude;
    }

    return NOERROR;
}

AutoPtr<Instance> Instance::CreateInstance(
    /* [in] */ Int32 sequenceType,
    /* [in] */ Int32 orientationType,
    /* [in] */ IGesture *gesture,
    /* [in] */ const String& label)
{
    AutoPtr<ArrayOf<Float> > pts;
    AutoPtr<Instance> instance;
    Int64 id;

    if (sequenceType == IGestureStore::SEQUENCE_SENSITIVE) {
        pts = TemporalSampler(orientationType, gesture);
        gesture->GetID(&id);
        instance = new Instance(id, pts, label);
        instance->Normalize();
    }
    else {
        SpatialSampler(gesture, (ArrayOf<Float> **)&pts);
        gesture->GetID(&id);
        instance = new Instance(id, pts, label);
    }

    REFCOUNT_ADD(instance);

    return instance;
}

ECode Instance::CreateInstance(
    /* [in] */ Int32 sequenceType,
    /* [in] */ Int32 orientationType,
    /* [in] */ IGesture *gesture,
    /* [in] */ const String& label,
    /* [out] */ IInstance** obj)
{
    *obj = CreateInstance(sequenceType, orientationType, gesture, label);

    return NOERROR;
}

ECode Instance::SpatialSampler(
    /* [in] */ IGesture *gesture,
    /* [out, callee] */ ArrayOf<Float> **sampler)
{
    VALIDATE_NOT_NULL(sampler);
    AutoPtr<ArrayOf<Float> > temp = GestureUtils::SpatialSampling(gesture, PATCH_SAMPLE_SIZE, FALSE);
    *sampler = temp;
    REFCOUNT_ADD(*sampler);
    return NOERROR;
}

AutoPtr<ArrayOf<Float> > Instance::TemporalSampler(
    /* [in] */ Int32 orientationType,
    /* [in] */ IGesture *gesture)
{
    AutoPtr<IArrayList> strokes;
    AutoPtr<IGestureStroke> stroke;

    gesture->GetStrokes((IArrayList**)&strokes);
    strokes->Get(0, (IInterface**)&stroke);

    AutoPtr<ArrayOf<Float> > pts = GestureUtils::TemporalSampling((IGestureStroke *)stroke,
            SEQUENCE_SAMPLE_SIZE);
    AutoPtr<ArrayOf<Float> > center = GestureUtils::ComputeCentroid(pts);
    Float orientation = (Float)Elastos::Core::Math::Atan2((*pts)[1] - (*center)[1], (*pts)[0] - (*center)[0]);

    Float adjustment = -orientation;
    if (orientationType != IGestureStore::ORIENTATION_INVARIANT) {
        Int32 count = ORIENTATIONS->GetLength();
        for (Int32 i = 0; i < count; i++) {
            Float delta = (*ORIENTATIONS)[i] - orientation;
            if (Elastos::Core::Math::Abs(delta) < Elastos::Core::Math::Abs(adjustment)) {
                adjustment = delta;
            }
        }
    }

    GestureUtils::Translate(pts, -(*center)[0], -(*center)[1]);
    GestureUtils::Rotate(pts, adjustment);

    return pts;
}

} // namespace Gesture
} // namespace Droid
} // namespace Elastos
