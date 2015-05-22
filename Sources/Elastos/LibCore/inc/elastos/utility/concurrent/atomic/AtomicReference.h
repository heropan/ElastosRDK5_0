
#ifndef __ATOMICREFERENCE_H__
#define __ATOMICREFERENCE_H__

#ifdef ELASTOS_CORE
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

namespace Elastos {
namespace Utility {
namespace Concurrent {
namespace Atomic {

class AtomicReference
{
public:
    /**
     * Creates a new AtomicReference with the given initial value.
     *
     * @param initialValue the initial value
     */
    CARAPI Init(
        /* [in] */ IInterface* initialValue);

    /**
     * Creates a new AtomicReference with null initial value.
     */
    CARAPI Init();

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

private:
    static const Int64 mSerialVersionUID;// = -1848883965231344442L;

    // private static final Unsafe unsafe = Unsafe.getUnsafe();
    static const Int64 mValueOffset;

    // static {
    //     try {
    //         valueOffset = unsafe.objectFieldOffset
    //             (AtomicReference.class.getDeclaredField("value"));
    //     } catch (Exception ex) { throw new Error(ex); }
    // }

    /* volatile */ AutoPtr<IInterface> mValue;
};

} // namespace Atomic
} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__ATOMICREFERENCE_H__
