
#include "CAtomicInteger32.h"
#include <cutils/atomic.h>
#include <cutils/atomic-inline.h>

namespace Elastos {
namespace Utility {
namespace Concurrent {
namespace Atomic {

CAR_INTERFACE_IMPL(CAtomicInteger32, Object, IAtomicInteger32)

CAR_OBJECT_IMPL(CAtomicInteger32)
/**
 * Creates a new AtomicInteger with the given initial value.
 *
 * @param initialValue the initial value
 */
ECode CAtomicInteger32::constructor(
    /* [in] */ Int32 initialValue)
{
    mValue = initialValue;
    return NOERROR;
}

/**
 * Creates a new AtomicInteger with initial value {@code 0}.
 */
ECode CAtomicInteger32::constructor()
{
    mValue = 0;
    return NOERROR;
}

/**
 * Gets the current value.
 *
 * @return the current value
 */
ECode CAtomicInteger32::Get(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    return NOERROR;
}

/**
 * Sets to the given value.
 *
 * @param newValue the new value
 */
ECode CAtomicInteger32::Set(
    /* [in] */ Int32 newValue)
{
    mValue = newValue;
    return NOERROR;
}

/**
 * Eventually sets to the given value.
 *
 * @param newValue the new value
 * @since 1.6
 */
ECode CAtomicInteger32::LazySet(
    /* [in] */ Int32 newValue)
{
    volatile Int32* address = &mValue;

    ANDROID_MEMBAR_STORE();
    *address = newValue;
    return NOERROR;
}

/**
 * Atomically sets to the given value and returns the old value.
 *
 * @param newValue the new value
 * @return the previous value
 */
ECode CAtomicInteger32::GetAndSet(
    /* [in] */ Int32 newValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    for (;;) {
        Int32 current;
        Get(&current);
        Boolean result;
        if (CompareAndSet(current, newValue, &result), result) {
            *value = current;
            return NOERROR;
        }
    }
}

/**
 * Atomically sets the value to the given updated value
 * if the current value {@code ==} the expected value.
 *
 * @param expect the expected value
 * @param update the new value
 * @return true if successful. False return indicates that
 * the actual value was not equal to the expected value.
 */
ECode CAtomicInteger32::CompareAndSet(
    /* [in] */ Int32 expect,
    /* [in] */ Int32 update,
    /* [out] */ Boolean* result)
{
    volatile int32_t* address = (volatile int32_t*)&mValue;

    // Note: android_atomic_release_cas() returns 0 on success, not failure.
    int ret = android_atomic_release_cas(expect, update, address);

    *result = (ret == 0);
    return NOERROR;
}

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
ECode CAtomicInteger32::WeakCompareAndSet(
    /* [in] */ Int32 expect,
    /* [in] */ Int32 update,
    /* [out] */ Boolean* result)
{
    volatile int32_t* address = (volatile int32_t*)&mValue;

    // Note: android_atomic_release_cas() returns 0 on success, not failure.
    int ret = android_atomic_release_cas(expect, update, address);

    *result = (ret == 0);
    return NOERROR;
}

/**
 * Atomically increments by one the current value.
 *
 * @return the previous value
 */
ECode CAtomicInteger32::GetAndIncrement(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    for (;;) {
        Int32 current;
        Get(&current);
        Int32 next = current + 1;
        Boolean result;
        if (CompareAndSet(current, next, &result), result) {
            *value = current;
            return NOERROR;
        }
    }
}

/**
 * Atomically decrements by one the current value.
 *
 * @return the previous value
 */
ECode CAtomicInteger32::GetAndDecrement(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    for (;;) {
        Int32 current;
        Get(&current);
        Int32 next = current - 1;
        Boolean result;
        if (CompareAndSet(current, next, &result), result) {
            *value = current;
            return NOERROR;
        }
    }
}

/**
 * Atomically adds the given value to the current value.
 *
 * @param delta the value to add
 * @return the previous value
 */
ECode CAtomicInteger32::GetAndAdd(
    /* [in] */ Int32 delta,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    for (;;) {
        Int32 current;
        Get(&current);
        Int32 next = current + delta;
        Boolean result;
        if (CompareAndSet(current, next, &result), result) {
            *value = current;
            return NOERROR;
        }
    }
}

/**
 * Atomically increments by one the current value.
 *
 * @return the updated value
 */
ECode CAtomicInteger32::IncrementAndGet(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    for (;;) {
        Int32 current;
        Get(&current);
        Int32 next = current + 1;
        Boolean result;
        if (CompareAndSet(current, next, &result), result) {
            *value = next;
            return NOERROR;
        }
    }
}

/**
 * Atomically decrements by one the current value.
 *
 * @return the updated value
 */
ECode CAtomicInteger32::DecrementAndGet(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    for (;;) {
        Int32 current;
        Get(&current);
        Int32 next = current - 1;
        Boolean result;
        if (CompareAndSet(current, next, &result), result) {
            *value = next;
            return NOERROR;
        }
    }
}

/**
 * Atomically adds the given value to the current value.
 *
 * @param delta the value to add
 * @return the updated value
 */
ECode CAtomicInteger32::AddAndGet(
    /* [in] */ Int32 delta,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    for (;;) {
        Int32 current;
        Get(&current);
        Int32 next = current + delta;
        Boolean result;
        if (CompareAndSet(current, next, &result), result) {
            *value = next;
            return NOERROR;
        }
    }
}

/**
 * Returns the value of this {@code AtomicInteger} as an {@code int}.
 */
ECode CAtomicInteger32::Int32Value(
    /* [out] */ Int32* value)
{
    return Get(value);
}

/**
 * Returns the value of this {@code AtomicInteger} as a {@code long}
 * after a widening primitive conversion.
 */
ECode CAtomicInteger32::Int64Value(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 current;
    Get(&current);
    *value = (Int64)current;
    return NOERROR;
}

/**
 * Returns the value of this {@code AtomicInteger} as a {@code float}
 * after a widening primitive conversion.
 */
ECode CAtomicInteger32::FloatValue(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 current;
    Get(&current);
    *value = (Float)current;
    return NOERROR;
}

/**
 * Returns the value of this {@code AtomicInteger} as a {@code double}
 * after a widening primitive conversion.
 */
ECode CAtomicInteger32::DoubleValue(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 current;
    Get(&current);
    *value = (Double)current;
    return NOERROR;
}

} // namespace Atomic
} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
