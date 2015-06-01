
#include "CDeflaterInputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CAR_OBJECT_IMPL(CDeflaterInputStream)

ECode CDeflaterInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj;// = DeflaterInputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

ECode CDeflaterInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return DeflaterInputStream::Init(is);
}

ECode CDeflaterInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IDeflater* deflater)
{
    return DeflaterInputStream::Init(is, deflater);
}

ECode CDeflaterInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IDeflater* deflater,
    /* [in] */ Int32 bufferSize)
{
    return DeflaterInputStream::Init(is, deflater, bufferSize);
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
