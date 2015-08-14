
#ifndef __ELASTOS_DROID_OS_CPARCELFILEDESCRIPTORAUTOCLOSEINPUTSTREAM_H__
#define __ELASTOS_DROID_OS_CPARCELFILEDESCRIPTORAUTOCLOSEINPUTSTREAM_H__

#include "_CParcelFileDescriptorAutoCloseInputStream.h"
#include "os/ParcelFileDescriptor.h"

using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * An InputStream you can create on a ParcelFileDescriptor, which will
 * take care of calling {@link ParcelFileDescriptor#close
 * ParcelFileDescriptor.close()} for you when the stream is closed.
 */
CarClass(CParcelFileDescriptorAutoCloseInputStream) , public ParcelFileDescriptor::AutoCloseInputStream
{
public:
    CParcelFileDescriptorAutoCloseInputStream();

    CARAPI constructor(
        /* [in] */ IParcelFileDescriptor* fd);

    CARAPI Close();

    CARAPI GetFD(
        /* [out] */ IFileDescriptor** fd);

    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 byteCount,
        /* [out] */ Int64* number);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_OS_CPARCELFILEDESCRIPTORAUTOCLOSEINPUTSTREAM_H__
