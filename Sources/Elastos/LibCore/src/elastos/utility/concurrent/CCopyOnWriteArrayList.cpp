
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

ECode CCopyOnWriteArrayList::Add(
    /* [in] */ IInterface* object)
{
    return E_NO_SUCH_METHOD_EXCEPTION;
}

ECode CCopyOnWriteArrayList::AddAll(
    /* [in] */ ICollection* collection)
{
    return E_NO_SUCH_METHOD_EXCEPTION;
}

ECode CCopyOnWriteArrayList::Remove(
    /* [in] */ IInterface* object)
{
    return E_NO_SUCH_METHOD_EXCEPTION;
}

ECode CCopyOnWriteArrayList::RemoveAll(
    /* [in] */ ICollection* collection)
{
    return E_NO_SUCH_METHOD_EXCEPTION;
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
