
#include "cmdef.h"
#include "CBufferedOutputStream.h"

namespace Elastos {
namespace IO {

CAR_OBJECT_IMPL(CBufferedOutputStream)　

ECode CBufferedOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = BufferedOutputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
