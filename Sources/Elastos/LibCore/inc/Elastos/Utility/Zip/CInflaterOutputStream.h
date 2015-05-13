
#ifndef __CINFLATEROUTPUTSTREAM_H__
#define __CINFLATEROUTPUTSTREAM_H__


#include "_CInflaterOutputStream.h"
#include "InflaterOutputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CarClass(CInflaterOutputStream) , public InflaterOutputStream
{
public:
    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBytesEx(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI Finish();

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

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

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__CINFLATEROUTPUTSTREAM_H__
