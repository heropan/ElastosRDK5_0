
#include "CAtomicBoolean.h"
#include <cutils/atomic.h>
#include <cutils/atomic-inline.h>

using Elastos::IO::EIID_ISerializable;

namespace Elastos {
namespace Utility {
namespace Concurrent {
namespace Atomic {

CAR_INTERFACE_IMPL_2(CAtomicBoolean, Object, IAtomicBoolean, ISerializable)

CAR_OBJECT_IMPL(CAtomicBoolean)

/**
 * Creates a new AtomicInteger with the given initial value.
 *
 * @param initialValue the initial value
 */
ECode CAtomicBoolean::constructor(
    /* [in] */ Boolean initialValue)
{
    mValue = initialValue ? 1 : 0;
    return NOERROR;
}

/**
 * Creates a new AtomicInteger with initial value {@code 0}.
 */
ECode CAtomicBoolean::constructor()
{
    mValue = 0;
    return NOERROR;
}

/**
 * Gets the current value.
 *
 * @return the current value
 */
ECode CAtomicBoolean::Get(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = (mValue != 0);
    return NOERROR;
}

/**
 * Sets to the given value.
 *
 * @param newValue the new value
 */
ECode CAtomicBoolean::Set(
    /* [in] */ Boolean newValue)
{
    mValue = newValue ? 1 : 0;
    return NOERROR;
}

/**
 * Eventually sets to the given value.
 *
 * @param newValue the new value
 * @since 1.6
 */
ECode CAtomicBoolean::LazySet(
    /* [in] */ Boolean newValue)
{
    volatile Int32* address = &mValue;

    ANDROID_MEMBAR_STORE();
    *address = newValue ? 1 : 0;
    return NOERROR;
}

/**
 * Atomically sets to the given value and returns the old value.
 *
 * @param newValue the new value
 * @return the previous value
 */
ECode CAtomicBoolean::GetAndSet(
    /* [in] */ Boolean newValue,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    for (;;) {
        Boolean current;
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
ECode CAtomicBoolean::CompareAndSet(
    /* [in] */ Boolean expect,
    /* [in] */ Boolean update,
    /* [out] */ Boolean* result)
{
    volatile int32_t* address = (volatile int32_t*)&mValue;
    int e = expect ? 1 : 0;
    int u = update ? 1 : 0;

    // Note: android_atomic_release_cas() returns 0 on success, not failure.
    int ret = android_atomic_release_cas(e, u, address);

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
ECode CAtomicBoolean::WeakCompareAndSet(
    /* [in] */ Boolean expect,
    /* [in] */ Boolean update,
    /* [out] */ Boolean* result)
{
    volatile int32_t* address = (volatile int32_t*)&mValue;
    int e = expect ? 1 : 0;
    int u = update ? 1 : 0;

    // Note: android_atomic_release_cas() returns 0 on success, not failure.
    int ret = android_atomic_release_cas(e, u, address);

    *result = (ret == 0);
    return NOERROR;
}

} // namespace Atomic
} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
