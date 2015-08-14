#ifndef __ELASTOS_DROID_GESTURE_CINSTANCELEARNER_H__
#define __ELASTOS_DROID_GESTURE_CINSTANCELEARNER_H__

#include "_CInstanceLearner.h"
#include "InsatnceLearner.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

CarClass(CInstanceLearner), public InstanceLearner
{
public:
    CARAPI AddInstance(
        /* [in] */ IInstance *instance);

    CARAPI GetInstance(
        /* [out] */ IObjectContainer **instances);

    CARAPI RemoveInstance(
        /* [in] */ Int64 id);

    CARAPI RemoveInstances(
        /* [in] */ const String& name);

    CARAPI Classify(
        /* [in] */ Int32 sequenceType,
        /* [in] */ Int32 orientationType,
        /* [in] */ ArrayOf<Float> *vec,
        /* [out] */ IObjectContainer **predictions);
};

}
}
}

#endif
