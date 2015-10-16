
#include "elastos/droid/gesture/Learner.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

void Learner::AddInstance(
    /* [in] */ Instance *instance)
{
    mInstances->PushBack(instance);
}

AutoPtr<List<AutoPtr<Instance> > > Learner::GetInstances()
{
    return mInstances;
}

void Learner::RemoveInstance(
    /* [in] */ Int64 id)
{
    AutoPtr< List< AutoPtr<Instance> > > instances = mInstances;
    List< AutoPtr<Instance> >::Iterator it;
    for (it = instances->Begin(); it != instances->End(); ++it) {
        AutoPtr<Instance> instance = *it;
        if (id == instance->mId) {
            it = instances->Erase(it);
            return;
        }
    }
}

void Learner::RemoveInstances(
    /* [in] */ const String& name)
{
    AutoPtr<List<AutoPtr<Instance> > > toDelete = new List<AutoPtr<Instance> >();
    AutoPtr<List<AutoPtr<Instance> > > instances = mInstances;

    List<AutoPtr<Instance> >::Iterator it;
    for (it = instances->Begin(); it != instances->End(); ++it) {
    // for (int i = 0; i < count; i++) {
        AutoPtr<Instance> instance = *it;
        // the label can be null, as specified in Instance
        if ((instance->mLabel.IsNull() && name.IsNull())
                || (!instance->mLabel.IsNull() && instance->mLabel.Equals(name))) {
            toDelete->PushBack(instance);
            it = instances->Erase(it);
        }
    }
}

}//namespace Gesture
}//namespace Droid
}//namespace Elastos
