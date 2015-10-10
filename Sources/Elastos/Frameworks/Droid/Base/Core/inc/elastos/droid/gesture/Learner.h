#ifndef __ELASTOS_DROID_GESTURE_LEARNER_H__
#define __ELASTOS_DROID_GESTURE_LEARNER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "gesture/Instance.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Utility::IArrayList;

namespace Elastos {
namespace Droid {
namespace Gesture {
/**
 * The abstract class of a gesture learner
 */
class Learner
    : public ElRefBase
{
public:
    /**
     * Add an instance to the learner
     *
     * @param instance
     */
    virtual CARAPI_(void) AddInstance(
        /* [in] */ Instance *instance);

    /**
     * Retrieve all the instances
     *
     * @return instances
     */
    virtual CARAPI_(AutoPtr<List<AutoPtr<Instance> > >) GetInstances();

    /**
     * Remove an instance based on its id
     *
     * @param id
     */
    virtual CARAPI_(void) RemoveInstance(
        /* [in] */ Int64 id);

    /**
     * Remove all the instances of a category
     *
     * @param name the category name
     */
    virtual CARAPI_(void) RemoveInstances(
        /* [in] */ const String& name);

    virtual CARAPI_(AutoPtr<IArrayList>) Classify(
        /* [in] */ Int32 sequenceType,
        /* [in] */ Int32 orientationType,
        /* [in] */ ArrayOf<Float> *vec) = 0;

private:
    AutoPtr< List< AutoPtr<Instance> > > mInstances;
};

}//namespace Gesture
}//namespace Droid
}//namespace Elastos
#endif //__ELASTOS_DROID_GESTURE_LEARNER_H__
