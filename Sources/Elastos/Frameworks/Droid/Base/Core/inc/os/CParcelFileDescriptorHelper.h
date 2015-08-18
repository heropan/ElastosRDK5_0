
#ifndef __ELASTOS_DROID_OS_CPARCELFILEDESCRIPTORHELPER_H__
#define __ELASTOS_DROID_OS_CPARCELFILEDESCRIPTORHELPER_H__

#include "_Elastos_Droid_Os_CParcelFileDescriptorHelper.h"

using Elastos::IO::IFile;
using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CParcelFileDescriptorHelper)
{
public:
    CARAPI Open(
        /* [in] */ IFile* file,
        /* [in] */ Int32 mode,
        /* [out] */ IParcelFileDescriptor** descriptor);

    CARAPI AdoptFd(
        /* [in] */ Int32 fd,
        /* [out] */ IParcelFileDescriptor** descriptor);

    /**
     * Create two ParcelFileDescriptors structured as a data pipe.  The first
     * ParcelFileDescriptor in the returned array is the read side; the second
     * is the write side.
     */
    CARAPI CreatePipe(
        /* [out, callee] */ ArrayOf<IParcelFileDescriptor*>** pfds);

    /**
    * Create a new ParcelFileDescriptor that is a dup of an existing
    * FileDescriptor.  This obeys standard POSIX semantics, where the
    * new file descriptor shared state such as file position with the
    * original file descriptor.
    */
    CARAPI Dup(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ IParcelFileDescriptor** descriptor);

};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_CPARCELFILEDESCRIPTORHELPER_H__
