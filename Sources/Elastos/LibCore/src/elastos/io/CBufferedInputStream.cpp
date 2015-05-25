
#include "cmdef.h"
#include "CBufferedInputStream.h"

namespace Elastos {
namespace IO {

CAR_OBJECT_IMPL(CBufferedInputStream)　

ECode CBufferedInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = BufferedInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
