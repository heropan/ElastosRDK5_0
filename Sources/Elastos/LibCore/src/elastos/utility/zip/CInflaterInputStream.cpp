
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

} // namespace Zip
} // namespace Utility
} // namespace Elastos
