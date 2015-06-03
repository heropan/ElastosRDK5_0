
#ifndef __ELASTOS_UTILITY_CGZIPOUTPUTSTREAM_H__
#define __ELASTOS_UTILITY_CGZIPOUTPUTSTREAM_H__

#include "_Elastos_Utility_Zip_CGZIPOutputStream.h"
#include "DeflaterOutputStream.h"
#include "CCRC32.h"

namespace Elastos {
namespace Utility {
namespace Zip {

/**
 * The {@code GZIPOutputStream} class is used to write data to a stream in the
 * GZIP storage format.
 */
CarClass(CGZIPOutputStream)
    , public DeflaterOutputStream
    , public IGZIPOutputStream
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CGZIPOutputStream();

    /**
     * Indicates to the stream that all data has been written out, and any GZIP
     * terminal data can now be written.
     *
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    //@Override
    CARAPI Finish();

    /**
     * Write up to nbytes of data from the given buffer, starting at offset off,
     * to the underlying stream in GZIP format.
     */
    //@Override
    CARAPI Write(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 off,
        /* [in] */ Int32 nbytes);

    /**
     * Construct a new {@code GZIPOutputStream} to write data in GZIP format to
     * the underlying stream.
     *
     * @param os
     *            the {@code OutputStream} to write data to.
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    CARAPI constructor(
        /* [in] */ IOutputStream* os);

    /**
     * Construct a new {@code GZIPOutputStream} to write data in GZIP format to
     * the underlying stream. Set the internal compression buffer to size
     * {@code size}.
     *
     * @param os
     *            the {@code OutputStream} to write to.
     * @param size
     *            the internal buffer size.
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    CARAPI constructor(
        /* [in] */ IOutputStream* os,
        /* [in] */ Int32 size);

private:
    CARAPI_(Int64) WriteInt64(
        /* [in] */ Int64 i);

    CARAPI_(Int32) WriteInt32(
        /* [in] */ Int32 i);

protected:
    /**
     * The checksum algorithm used when treating uncompressed data.
     */
    AutoPtr<CCRC32> mCrc;
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CGZIPOUTPUTSTREAM_H__
