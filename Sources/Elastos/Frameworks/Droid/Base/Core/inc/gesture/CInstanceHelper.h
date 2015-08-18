#ifndef __ELASTOS_DROID_GESTURE_CINSTANCEHELPER_H__
#define __ELASTOS_DROID_GESTURE_CINSTANCEHELPER_H__

#include "_Elastos_Droid_Gesture_CInstanceHelper.h"
#include "Instance.h"

namespace Elastos {
namespace Droid {
namespace Gesture {
CarClass(CInstanceHelper)
{
public:
    CARAPI CreateInstance(
        /* [in] */ Int32 sequenceType,
        /* [in] */ Int32 orientationType,
        /* [in] */ IGesture *gesture,
        /* [in] */ const String& label,
        /* [out] */ IInstance **instance);

    CARAPI SpatialSampler(
        /* [in] */ IGesture *gesture,
        /* [out, callee] */ ArrayOf<Float> **sampler);

    CARAPI TemporalSampler(
        /* [in] */ Int32 orientationType,
        /* [in] */ IGesture *gesture,
        /* [out, callee] */ ArrayOf<Float> **sampler);
};
}
}
}

#endif
