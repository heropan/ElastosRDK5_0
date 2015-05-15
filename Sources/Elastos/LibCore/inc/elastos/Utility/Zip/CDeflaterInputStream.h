
#ifndef __CDEFLATERINPUTSTREAM_H__
#define __CDEFLATERINPUTSTREAM_H__


#include "_CDeflaterInputStream.h"
#include "DeflaterInputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CarClass(CDeflaterInputStream) , public DeflaterInputStream
{
public:
    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Close();

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBytesEx(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI Reset();

    /**
     * Constructs a {@code DeflaterInputStream} with a new {@code Deflater} and an
     * implementation-defined default internal buffer size. {@code in} is a source of
     * uncompressed data, and this stream will be a source of compressed data.
     *
     * @param in the source {@code InputStream}
     */
    CARAPI constructor(
        /* [in] */ IInputStream* is);

    /**
     * Constructs a {@code DeflaterInputStream} with the given {@code Deflater} and an
     * implementation-defined default internal buffer size. {@code in} is a source of
     * uncompressed data, and this stream will be a source of compressed data.
     *
     * @param in the source {@code InputStream}
     * @param deflater the {@code Deflater} to be used for compression
     */
    CARAPI constructor(
        /* [in] */ IInputStream* is,
        /* [in] */ IDeflater* deflater);

    /**
     * Constructs a {@code DeflaterInputStream} with the given {@code Deflater} and
     * given internal buffer size. {@code in} is a source of
     * uncompressed data, and this stream will be a source of compressed data.
     *
     * @param in the source {@code InputStream}
     * @param deflater the {@code Deflater} to be used for compression
     * @param bufferSize the length in bytes of the internal buffer
     */
    CARAPI constructor(
        /* [in] */ IInputStream* is,
        /* [in] */ IDeflater* deflater,
        /* [in] */ Int32 bufferSize);
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__CDEFLATERINPUTSTREAM_H__
