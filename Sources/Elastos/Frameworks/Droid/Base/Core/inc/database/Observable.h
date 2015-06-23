#ifndef __OBSERVABLE_H__
#define __OBSERVABLE_H__

#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Core::Mutex;
using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Droid {
namespace Database {
/**
 * Provides methods for (un)registering arbitrary observers in an ArrayList.
 */
class Observable
{
public:
    /**
     * Adds an observer to the list. The observer cannot be null and it must not already
     * be registered.
     * @param observer the observer to register
     * @throws IllegalArgumentException the observer is null
     * @throws IllegalStateException the observer is already registered
     */
    virtual CARAPI RegisterObserver(
        /* [in] */ IInterface* observer);

    /**
     * Removes a previously registered observer. The observer must not be null and it
     * must already have been registered.
     * @param observer the observer to unregister
     * @throws IllegalArgumentException the observer is null
     * @throws IllegalStateException the observer is not yet registered
     */
    virtual CARAPI UnregisterObserver(
        /* [in] */ IInterface* observer);

    /**
     * Remove all registered observers.
     */
    virtual CARAPI UnregisterAll();

protected:
    /**
     * The list of observers.  An observer can be in the list at most
     * once and will never be NULL.
     */
    List< AutoPtr<IInterface> > mObservers;
    Mutex mObserversLock;
};

} //Database
} //Droid
} //Elastos

#endif //__OBSERVABLE_H__
