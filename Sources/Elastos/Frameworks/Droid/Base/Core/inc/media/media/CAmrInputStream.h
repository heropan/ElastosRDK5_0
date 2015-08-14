
#ifndef __ELASTOS_DROID_MEDIA_CAMRINPUTSTREAM_H__
#define __ELASTOS_DROID_MEDIA_CAMRINPUTSTREAM_H__

#include "_CAmrInputStream.h"
#include "AmrInputStream.h"

using Elastos::IO::IInputStream;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CAmrInputStream), public AmrInputStream
{
public:
    /**
     * Create a new AmrInputStream, which converts 16 bit PCM to AMR
     * @param inputStream InputStream containing 16 bit PCM.
     */
    CARAPI constructor(
        /* [in] */ IInputStream* inputStream);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Close();

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
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 byteCount,
        /* [out] */ Int64* number);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CAMRINPUTSTREAM_H__
