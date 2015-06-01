
#ifndef __ELASTOS_UTILITY_CINFLATEROUTPUTSTREAM_H__
#define __ELASTOS_UTILITY_CINFLATEROUTPUTSTREAM_H__

#include "_Elastos_Utility_Zip_CInflaterOutputStream.h"
#include "InflaterOutputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CarClass(CInflaterOutputStream)
    , public InflaterOutputStream
{
public:
    /**
     * Constructs an {@code InflaterOutputStream} with a new {@code Inflater} and an
     * implementation-defined default internal buffer size. {@code out} is a destination
     * for uncompressed data, and compressed data will be written to this stream.
     *
     * @param out the destination {@code OutputStream}
     */
    CARAPI constructor(
        /* [in] */ IOutputStream* out);

    /**
     * Constructs an {@code InflaterOutputStream} with the given {@code Inflater} and an
     * implementation-defined default internal buffer size. {@code out} is a destination
     * for uncompressed data, and compressed data will be written to this stream.
     *
     * @param out the destination {@code OutputStream}
     * @param inf the {@code Inflater} to be used for decompression
     */
    CARAPI constructor(
        /* [in] */ IOutputStream* out,
        /* [in] */ IInflater* inf);

    /**
     * Constructs an {@code InflaterOutputStream} with the given {@code Inflater} and
     * given internal buffer size. {@code out} is a destination
     * for uncompressed data, and compressed data will be written to this stream.
     *
     * @param out the destination {@code OutputStream}
     * @param inf the {@code Inflater} to be used for decompression
     * @param bufferSize the length in bytes of the internal buffer
     */
    CARAPI constructor(
        /* [in] */ IOutputStream* out,
        /* [in] */ IInflater* inf,
        /* [in] */ Int32 bufferSize);

    // CARAPI GetLock(
    //      [out]  IInterface** lockobj);
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CINFLATEROUTPUTSTREAM_H__
