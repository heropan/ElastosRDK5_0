
#ifndef __ELASTOS_DROID_CONTENT_RES_CASSETFILEDESCRIPTORAUTOCLOSEINPUTSTREAM_H__
#define __ELASTOS_DROID_CONTENT_RES_CASSETFILEDESCRIPTORAUTOCLOSEINPUTSTREAM_H__

#include "_CAssetFileDescriptorAutoCloseInputStream.h"
#include "os/ParcelFileDescriptor.h"

using Elastos::IO::IFileDescriptor;
using Elastos::Droid::Os::ParcelFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

/**
 * An InputStream you can create on a ParcelFileDescriptor, which will
 * take care of calling {@link ParcelFileDescriptor#close
 * ParcelFileDescritor.close()} for you when the stream is closed.
 */
CarClass(CAssetFileDescriptorAutoCloseInputStream)  , public ParcelFileDescriptor::AutoCloseInputStream
{
public:
    CAssetFileDescriptorAutoCloseInputStream();

    CARAPI constructor(
        /* [in] */ IAssetFileDescriptor* fd);

    CARAPI Available(
        /* [out] */ Int32* able);

    CARAPI Read(
        /* [out] */ Int32* result);

    CARAPI ReadBytes(
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [out] */ Int32* result);

    CARAPI ReadBytes(
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* result);

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* result);

    CARAPI Mark(
        /* [in] */ Int32 readlimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* result);

    CARAPI Reset();

    CARAPI Close();

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

#endif // __ELASTOS_DROID_CONTENT_RES_CASSETFILEDESCRIPTORAUTOCLOSEINPUTSTREAM_H__
