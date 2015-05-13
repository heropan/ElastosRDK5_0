#include "CInstanceHelper.h"

namespace Elastos {
namespace Droid {
namespace Gesture {
ECode CInstanceHelper::CreateInstance(
    /* [in] */ Int32 sequenceType,
    /* [in] */ Int32 orientationType,
    /* [in] */ IGesture *gesture,
    /* [in] */ const String& label,
    /* [out] */ IInstance **instance)
{
    return Instance::CreateInstance(sequenceType, orientationType, gesture, label, instance);
}

ECode CInstanceHelper::SpatialSampler(
    /* [in] */ IGesture *gesture,
    /* [out, callee] */ ArrayOf<Float> **sampler)
{
    return Instance::SpatialSampler(gesture, sampler);
}

ECode CInstanceHelper::TemporalSampler(
    /* [in] */ Int32 orientationType,
    /* [in] */ IGesture *gesture,
    /* [out, callee] */ ArrayOf<Float> **sampler)
{
    return Instance::TemporalSampler(orientationType, gesture, sampler);
}
}
}
}
