
#include "CZipOutputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CAR_OBJECT_IMPL(CZipOutputStream)

ECode CZipOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj;// = ZipOutputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
