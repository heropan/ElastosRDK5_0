
#ifndef __ELASTOS_UTILITY_CDEFLATERINPUTSTREAM_H__
#define __ELASTOS_UTILITY_CDEFLATERINPUTSTREAM_H__

#include "_Elastos_Utility_Zip_CDeflaterInputStream.h"
#include "DeflaterInputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CarClass(CDeflaterInputStream) , public DeflaterInputStream
{
public:
    CAR_OBJECT_DECL()

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

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

#endif //__ELASTOS_UTILITY_CDEFLATERINPUTSTREAM_H__
