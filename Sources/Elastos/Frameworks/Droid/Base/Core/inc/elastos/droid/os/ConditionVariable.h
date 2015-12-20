
#ifndef __ELASTOS_DROID_OS_CONDITION_VARIABLE_H__
#define __ELASTOS_DROID_OS_CONDITION_VARIABLE_H__

#include "Elastos.Droid.Os.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * Class that implements the condition variable locking paradigm.
 *
 * <p>
 * This differs from the built-in java.lang.Object wait() and notify()
 * in that this class contains the condition to wait on itself.  That means
 * open(), close() and block() are sticky.  If open() is called before block(),
 * block() will not block, and instead return immediately.
 *
 * <p>
 * This class uses itself as the object to wait on, so if you wait()
 * or notify() on a ConditionVariable, the results are undefined.
 */
class ConditionVariable
    : public Object
    , public IConditionVariable
{
public:
    CAR_INTERFACE_DECL()

    /**
     * Create the ConditionVariable in the default closed state.
     */
    ConditionVariable();

    /**
     * Create the ConditionVariable with the given state.
     *
     * <p>
     * Pass true for opened and false for closed.
     */
    ConditionVariable(
        /* [in] */ Boolean state);

    /**
     * Open the condition, and release all threads that are blocked.
     *
     * <p>
     * Any threads that later approach block() will not block unless close()
     * is called.
     */
    CARAPI Open();

    /**
     * Reset the condition to the closed state.
     *
     * <p>
     * Any threads that call block() will block until someone calls open.
     */
    CARAPI Close();

    /**
     * Block the current thread until the condition is opened.
     *
     * <p>
     * If the condition is already opened, return immediately.
     */
    CARAPI Block();

    /**
     * Block the current thread until the condition is opened or until
     * timeout milliseconds have passed.
     *
     * <p>
     * If the condition is already opened, return immediately.
     *
     * @param timeout the minimum time to wait in milliseconds.
     *
     * @return true if the condition was opened, false if the call returns
     * because of the timeout.
     */
    CARAPI Block(
        /* [in] */ Int64 timeout,
        /* [out] */ Boolean* result);

private:
    volatile Boolean mCondition;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_CONDITION_VARIABLE_H__
