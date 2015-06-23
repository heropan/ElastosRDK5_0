
#ifndef __CASSETFILEDESCRIPTORAUTOCLOSEOUTPUTSTREAM_H__
#define __CASSETFILEDESCRIPTORAUTOCLOSEOUTPUTSTREAM_H__

#include "_CAssetFileDescriptorAutoCloseOutputStream.h"
#include "os/ParcelFileDescriptor.h"

using Elastos::IO::IFileDescriptor;
using Elastos::Droid::Os::ParcelFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

/**
 * An OutputStream you can create on a ParcelFileDescriptor, which will
 * take care of calling {@link ParcelFileDescriptor#close
 * ParcelFileDescritor.close()} for you when the stream is closed.
 */
CarClass(CAssetFileDescriptorAutoCloseOutputStream) , public ParcelFileDescriptor::AutoCloseOutputStream
{
public:
    CAssetFileDescriptorAutoCloseOutputStream();

    CARAPI constructor(
        /* [in] */ IAssetFileDescriptor* fd);

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI Flush();

    CARAPI Close();

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI GetFD(
        /* [out] */ IFileDescriptor** fd);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    Int64 mRemaining;
};

}
}
}
}

#endif // __CASSETFILEDESCRIPTORAUTOCLOSEOUTPUTSTREAM_H__
