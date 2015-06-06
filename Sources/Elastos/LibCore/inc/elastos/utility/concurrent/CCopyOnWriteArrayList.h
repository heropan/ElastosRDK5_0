
#ifndef __ELASTOS_UTILITY_CCOPYONWRITEARRAYLIST_H__
#define __ELASTOS_UTILITY_CCOPYONWRITEARRAYLIST_H__

#include "_Elastos_Utility_Concurrent_CCopyOnWriteArrayList.h"
#include "CopyOnWriteArrayList.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CCopyOnWriteArrayList)
    , public CopyOnWriteArrayList
{
public:
    CAR_OBJECT_DECL()
    /**
     * Creates an empty instance.
     */
    CARAPI constructor();

    /**
     * Creates a new instance containing the elements of {@code collection}.
     */
    CARAPI constructor(
        /* [in] */ ICollection* collection);

    /**
     * Creates a new instance containing the elements of {@code array}.
     */
    CARAPI constructor(
        /* [in] */ ArrayOf<IInterface*>* array);

    CARAPI Add(
        /* [in] */ IInterface* object);

    CARAPI AddAll(
        /* [in] */ ICollection* collection);

    CARAPI Remove(
        /* [in] */ IInterface* object);

    CARAPI RemoveAll(
        /* [in] */ ICollection* collection);
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CCOPYONWRITEARRAYLIST_H__
