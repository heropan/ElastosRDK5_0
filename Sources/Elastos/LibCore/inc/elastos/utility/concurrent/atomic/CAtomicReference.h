
#ifndef __CATOMICREFERENCE_H__
#define __CATOMICREFERENCE_H__

#include "_CAtomicReference.h"
#include "AtomicReference.h"

using Elastos::Utility::Concurrent::Atomic::AtomicReference;

namespace Elastos {
namespace Utility {
namespace Concurrent {
namespace Atomic {

CarClass(CAtomicReference)
    , public AtomicReference
{
public:
    /**
     * Creates a new AtomicReference with the given initial value.
     *
     * @param initialValue the initial value
     */
    CARAPI constructor(
        /* [in] */ IInterface* initialValue);

    /**
     * Creates a new AtomicReference with null initial value.
     */
    CARAPI constructor();

    /**
     * Gets the current value.
     *
     * @return the current value
     */
    CARAPI Get(
        /* [out] */ IInterface** outface);

    /**
     * Sets to the given value.
     *
     * @param newValue the new value
     */
    CARAPI Set(
        /* [in] */ IInterface* newValue);

    /**
     * Eventually sets to the given value.
     *
     * @param newValue the new value
     * @since 1.6
     */
    CARAPI LazySet(
        /* [in] */ IInterface* newValue);

    /**
     * Atomically sets the value to the given updated value
     * if the current value {@code ==} the expected value.
     * @param expect the expected value
     * @param update the new value
     * @return true if successful. False return indicates that
     * the actual value was not equal to the expected value.
     */
    CARAPI CompareAndSet(
        /* [in] */ IInterface* expect,
        /* [in] */ IInterface* update,
        /* [out] */ Boolean* value);

    /**
     * Atomically sets the value to the given updated value
     * if the current value {@code ==} the expected value.
     *
     * <p>May <a href="package-summary.html#Spurious">fail spuriously</a>
     * and does not provide ordering guarantees, so is only rarely an
     * appropriate alternative to {@code compareAndSet}.
     *
     * @param expect the expected value
     * @param update the new value
     * @return true if successful.
     */
    CARAPI WeakCompareAndSet(
        /* [in] */ IInterface* expect,
        /* [in] */ IInterface* update,
        /* [out] */ Boolean* value);

    /**
     * Atomically sets to the given value and returns the old value.
     *
     * @param newValue the new value
     * @return the previous value
     */
    CARAPI GetAndSet(
        /* [in] */ IInterface* newValue,
        /* [out] */ IInterface** outface);

    /**
     * Returns the String representation of the current value.
     * @return the String representation of the current value.
     */
    CARAPI ToString(
        /* [out] */ String* str);
};

} // namespace Atomic
} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__CATOMICREFERENCE_H__
