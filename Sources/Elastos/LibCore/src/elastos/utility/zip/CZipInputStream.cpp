
#include "CZipInputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CAR_OBJECT_IMPL(CZipInputStream)

ECode CZipInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj;// = ZipInputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
