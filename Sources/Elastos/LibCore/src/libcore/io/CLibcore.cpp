
#include "cmdef.h"
#include "CLibcore.h"
#include "CPosix.h"

namespace Libcore {
namespace IO {

static AutoPtr<IOs> InitsOs()
{
    AutoPtr<CPosix> posix;
    CPosix::NewByFriend((CPosix**)&posix);
    assert(posix != NULL);
    return (IOs*)posix.Get();
}

AutoPtr<IOs> CLibcore::sOs = InitsOs();

ECode CLibcore::GetOs(
    /* [out] */ IOs ** os)
{
    VALIDATE_NOT_NULL(os);

    *os = sOs;
    INTERFACE_ADDREF(*os);

    return NOERROR;
}

} // namespace IO
} // namespace Libcore
