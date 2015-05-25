
#include "cmdef.h"
#include "CStructLinger.h"

namespace Libcore {
namespace IO {

ECode CStructLinger::GetOnoff(
    /* [out] */ Int32* onoff)
{
    VALIDATE_NOT_NULL(onoff);

    *onoff = mOnoff;
    return NOERROR;
}

ECode CStructLinger::GetLinger(
    /* [out] */ Int32* linger)
{
    VALIDATE_NOT_NULL(linger);

    *linger = mLinger;
    return NOERROR;
}

ECode CStructLinger::IsOn(
    /* [out] */ Boolean* isOn)
{
    VALIDATE_NOT_NULL(isOn);

    *isOn = mOnoff != 0;
    return NOERROR;
}

ECode CStructLinger::constructor(
    /* [in] */ Int32 onoff,
    /* [in] */ Int32 linger)
{
    mOnoff = onoff;
    mLinger = linger;
    return NOERROR;
}

} // namespace IO
} // namespace Libcore
