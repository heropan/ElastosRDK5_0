
#ifndef __ELASTOS_DROID_OS_CPARCELFILEDESCRIPTORAUTOCLOSEOUTPUTSTREAM_H__
#define __ELASTOS_DROID_OS_CPARCELFILEDESCRIPTORAUTOCLOSEOUTPUTSTREAM_H__

#include "_CParcelFileDescriptorAutoCloseOutputStream.h"
#include "os/ParcelFileDescriptor.h"

using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * An OutputStream you can create on a ParcelFileDescriptor, which will
 * take care of calling {@link ParcelFileDescriptor#close
 * ParcelFileDescriptor.close()} for you when the stream is closed.
 */
CarClass(CParcelFileDescriptorAutoCloseOutputStream) , public ParcelFileDescriptor::AutoCloseOutputStream
{
public:
    CParcelFileDescriptorAutoCloseOutputStream();

    CARAPI Close();

    CARAPI Flush();

    CARAPI constructor(
        /* [in] */ IParcelFileDescriptor* pfd);

    CARAPI GetFD(
        /* [out] */ IFileDescriptor** fd);

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_OS_CPARCELFILEDESCRIPTORAUTOCLOSEOUTPUTSTREAM_H__
