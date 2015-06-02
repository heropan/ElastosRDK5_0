
#include "CInflaterInputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CAR_OBJECT_IMPL(CInflaterInputStream)

ECode CInflaterInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj;// = InflaterInputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

ECode CInflaterInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return InflaterInputStream::Init(is);
}

ECode CInflaterInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IInflater* inflater)
{
    return InflaterInputStream::Init(is, inflater);
}

ECode CInflaterInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IInflater* inflater,
    /* [in] */ Int32 bsize)
{
    return InflaterInputStream::Init(is, inflater, bsize);
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
