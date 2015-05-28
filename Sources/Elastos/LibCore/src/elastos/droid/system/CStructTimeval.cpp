
#include "CStructTimeval.h"

namespace Elastos {
namespace Droid {
namespace System {
CAR_OBJECT_IMPL(CStructTimeval)

CAR_INTERFACE_IMPL(CStructTimeval, Object, IStructTimeval)

ECode CStructTimeval::GetSec(
    /* [out] */ Int64* sec)
{
    VALIDATE_NOT_NULL(sec);

    *sec = tv_sec;
    return NOERROR;
}

ECode CStructTimeval::GetUsec(
    /* [out] */ Int64* usec)
{
    VALIDATE_NOT_NULL(usec);

    *usec = tv_usec;
    return NOERROR;
}

ECode CStructTimeval::ToMillis(
    /* [out] */ Int64* millis)
{
    VALIDATE_NOT_NULL(millis);

    *millis = (tv_sec * 1000) + (tv_usec / 1000);
    return NOERROR;
}

ECode CStructTimeval::constructor(
    /* [in] */ Int64 sec,
    /* [in] */ Int64 usec)
{
    tv_sec = sec;
    tv_usec = usec;
    return NOERROR;
}

ECode CStructTimeval::FromMillis(
    /* [in] */ Int64 millis,
    /* [out] */ IStructTimeval** tv)
{
    VALIDATE_NOT_NULL(tv);

    Int64 tv_sec = millis / 1000;
    Int64 tv_usec = (millis - (tv_sec * 1000)) * 1000;
    return CStructTimeval::New(tv_sec, tv_usec, tv);
}

} // namespace System
} // namespace Droid
} // namespace Elastos
