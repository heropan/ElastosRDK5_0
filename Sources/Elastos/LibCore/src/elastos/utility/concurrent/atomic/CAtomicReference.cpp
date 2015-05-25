
#include "cmdef.h"
#include "CAtomicReference.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {
namespace Atomic {

ECode CAtomicReference::constructor(
    /* [in] */ IInterface* initialValue)
{
    return AtomicReference::Init(initialValue);
}

ECode CAtomicReference::constructor()
{
    return AtomicReference::Init();
}

ECode CAtomicReference::Get(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    return AtomicReference::Get(outface);
}

ECode CAtomicReference::Set(
    /* [in] */ IInterface* newValue)
{
    return AtomicReference::Set(newValue);
}

ECode CAtomicReference::LazySet(
    /* [in] */ IInterface* newValue)
{
    return AtomicReference::LazySet(newValue);
}

ECode CAtomicReference::CompareAndSet(
    /* [in] */ IInterface* expect,
    /* [in] */ IInterface* update,
    /* [out] */ Boolean* value)
{
    return AtomicReference::CompareAndSet(expect, update, value);
}

ECode CAtomicReference::WeakCompareAndSet(
    /* [in] */ IInterface* expect,
    /* [in] */ IInterface* update,
    /* [out] */ Boolean* value)
{
    return AtomicReference::WeakCompareAndSet(expect, update, value);
}

ECode CAtomicReference::GetAndSet(
    /* [in] */ IInterface* newValue,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    return AtomicReference::GetAndSet(newValue, outface);
}

ECode CAtomicReference::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    return AtomicReference::ToString(str);
}

} // namespace Atomic
} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
