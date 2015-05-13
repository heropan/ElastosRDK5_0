
#include "os/CParcelFileDescriptorHelper.h"
#include "os/CParcelFileDescriptor.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CParcelFileDescriptorHelper::Open(
    /* [in] */ IFile* file,
    /* [in] */ Int32 mode,
    /* [out] */ IParcelFileDescriptor** descriptor)
{
    return CParcelFileDescriptor::Open(file, mode, descriptor);
}

ECode CParcelFileDescriptorHelper::AdoptFd(
    /* [in] */ Int32 fd,
    /* [out] */ IParcelFileDescriptor** descriptor)
{
    return CParcelFileDescriptor::AdoptFd(fd, descriptor);
}

ECode CParcelFileDescriptorHelper::CreatePipe(
    /* [out, callee] */ ArrayOf<IParcelFileDescriptor*>** pfds)
{
    return CParcelFileDescriptor::CreatePipe(pfds);
}

ECode CParcelFileDescriptorHelper::Dup(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ IParcelFileDescriptor** descriptor)
{
    return CParcelFileDescriptor::Dup(fd, descriptor);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos

