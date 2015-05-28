
#include "AtomicReference.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {
namespace Atomic {

const Int64 AtomicReference::mSerialVersionUID = -1848883965231344442L;

const Int64 AtomicReference::mValueOffset = 0; //unsafe.objectFieldOffset(AtomicReference.class.getDeclaredField("value"));

ECode AtomicReference::Init(
    /* [in] */ IInterface* initialValue)
{
    mValue = initialValue;
    return NOERROR;
}

ECode AtomicReference::Init()
{
    return NOERROR;
}

ECode AtomicReference::Get(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    *outface = mValue;
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

ECode AtomicReference::Set(
    /* [in] */ IInterface* newValue)
{
    mValue = newValue;
    return NOERROR;
}

ECode AtomicReference::LazySet(
    /* [in] */ IInterface* newValue)
{
    assert(0 && "TODO");
    // unsafe.putOrderedObject(this, valueOffset, newValue);
    return NOERROR;
}

ECode AtomicReference::CompareAndSet(
    /* [in] */ IInterface* expect,
    /* [in] */ IInterface* update,
    /* [out] */ Boolean* value)
{
    assert(0 && "TODO");
    // return unsafe.compareAndSwapObject(this, valueOffset, expect, update);
    return NOERROR;
}

ECode AtomicReference::WeakCompareAndSet(
    /* [in] */ IInterface* expect,
    /* [in] */ IInterface* update,
    /* [out] */ Boolean* value)
{
    assert(0 && "TODO");
    // return unsafe.compareAndSwapObject(this, valueOffset, expect, update);
    return NOERROR;
}

ECode AtomicReference::GetAndSet(
    /* [in] */ IInterface* newValue,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    while (TRUE) {
        AutoPtr<IInterface> x;
        Get((IInterface**)&x);
        Boolean isflag = FALSE;
        if (CompareAndSet(x, newValue, &isflag), isflag) {
            *outface = x;
            INTERFACE_ADDREF(*outface)
            return NOERROR;
        }
    }
    return NOERROR;
}

ECode AtomicReference::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    assert(0 && "TODO");
    // return String.valueOf(get());
    return NOERROR;
}

} // namespace Atomic
} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
