#include "CInsatnceLearner.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

ECode CInsatnceLearner::AddInstance(
    /* [in] */ IInstance *instance)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInsatnceLearner::GetInstance(
    /* [out] */ IObjectContainer **instances)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInsatnceLearner::RemoveInstance(
    /* [in] */ Int64 id)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInsatnceLearner::RemoveInstances(
    /* [in] */ const String& name)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInsatnceLearner::Classify(
    /* [in] */ Int32 sequenceType,
    /* [in] */ Int32 orientationType,
    /* [in] */ ArrayOf<Float> *vec,
    /* [out] */ IObjectContainer **predictions)
{
    return InstanceLearner::Classify(sequenceType, orientationType, vec, predictions);
}


}
}
}
