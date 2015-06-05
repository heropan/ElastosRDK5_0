
#include "CCopyOnWriteArrayList.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

CAR_OBJECT_IMPL(CCopyOnWriteArrayList)

ECode CCopyOnWriteArrayList::constructor()
{
    return CopyOnWriteArrayList::Init();
}

ECode CCopyOnWriteArrayList::constructor(
    /* [in] */ ICollection* collection)
{
    return CopyOnWriteArrayList::Init(collection);
}

ECode CCopyOnWriteArrayList::constructor(
    /* [in] */ ArrayOf<IInterface*>* array)
{
    return CopyOnWriteArrayList::Init(array);
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
