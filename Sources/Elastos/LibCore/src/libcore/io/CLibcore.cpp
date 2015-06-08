
#include "CLibcore.h"
// #include "CPosix.h"

namespace Libcore {
namespace IO {

static AutoPtr<IOs> InitsOs()
{
    // AutoPtr<CPosix> posix;
    // CPosix::NewByFriend((CPosix**)&posix);
    // assert(posix != NULL);
    // return posix.Get();
    return NULL; // wait CPosix... TODO::
}

AutoPtr<IOs> CLibcore::sOs = InitsOs();

CAR_SINGLETON_IMPL(CLibcore)

CAR_INTERFACE_IMPL(CLibcore, Singleton, ILibcore)

ECode CLibcore::GetOs(
    /* [out] */ IOs ** os)
{
    VALIDATE_NOT_NULL(os);

    *os = sOs;
    REFCOUNT_ADD(*os);

    return NOERROR;
}

} // namespace IO
} // namespace Libcore
