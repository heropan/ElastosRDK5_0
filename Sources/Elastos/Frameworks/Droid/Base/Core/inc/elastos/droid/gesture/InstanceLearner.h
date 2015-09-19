#ifndef __ELASTOS_DROID_GESTURE_INSTANCELEARNER_H__
#define __ELASTOS_DROID_GESTURE_INSTANCELEARNER_H__

#include "ext/frameworkext.h"
#include "gesture/Learner.h"

using Elastos::Core::IComparator;

namespace Elastos {
namespace Droid {
namespace Gesture {

class InstanceLearner
    : public Learner
{
private:
    class PredictionComparator
        : public ElRefBase
        , public IComparator
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Compare(
            /* [in] */ IInterface* object1,
            /* [in] */ IInterface* object2,
            /* [out] */ Int32* result);
    };

public:
    InstanceLearner();

    //@Override
    CARAPI_(AutoPtr<IArrayList>) Classify(
        /* [in] */ Int32 sequenceType,
        /* [in] */ Int32 orientationType,
        /* [in] */ ArrayOf<Float> *vec);

private:
    AutoPtr<IComparator> mComparator;

};

}//namespace Gesture
}//namespace Droid
}//namespace Elastos

#endif //__ELASTOS_DROID_GESTURE_INSTANCELEARNER_H__
