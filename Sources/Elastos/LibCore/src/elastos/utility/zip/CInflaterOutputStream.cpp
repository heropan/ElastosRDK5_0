
#include "CInflaterOutputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

ECode CInflaterOutputStream::constructor(
    /* [in] */ IOutputStream* out)
{
    return InflaterOutputStream::Init(out);
}

ECode CInflaterOutputStream::constructor(
    /* [in] */ IOutputStream* out,
    /* [in] */ IInflater* inf)
{
    return InflaterOutputStream::Init(out, inf);
}

ECode CInflaterOutputStream::constructor(
    /* [in] */ IOutputStream* out,
    /* [in] */ IInflater* inf,
    /* [in] */ Int32 bufferSize)
{
    return InflaterOutputStream::Init(out, inf, bufferSize);
}

// ECode CInflaterOutputStream::GetLock(
//     /* [out] */ IInterface** lockobj)
// {
//     VALIDATE_NOT_NULL(lockobj);

//     AutoPtr<IInterface> obj = InflaterOutputStream::GetLock();
//     *lockobj = obj;
//     REFCOUNT_ADD(*lockobj);
//     return NOERROR;
// }

} // namespace Zip
} // namespace Utility
} // namespace Elastos
