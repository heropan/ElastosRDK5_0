
#ifndef __ELASTOS_UTILITY_CGZIPINPUTSTREAM_H__
#define __ELASTOS_UTILITY_CGZIPINPUTSTREAM_H__

#include "_Elastos_Utility_Zip_CGZIPInputStream.h"
#include "InflaterInputStream.h"
#include "CCRC32.h"

namespace Elastos {
namespace Utility {
namespace Zip {

/**
 * The {@code GZIPInputStream} class is used to read data stored in the GZIP
 * format, reading and decompressing GZIP data from the underlying stream into
 * its buffer.
 */
CarClass(CGZIPInputStream)
    , public InflaterInputStream
    , public IGZIPInputStream
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CGZIPInputStream();

    /**
     * Closes this stream and any underlying streams.
     */
    //@Override
    CARAPI Close();

    /**
     * Reads and decompresses GZIP data from the underlying stream into the
     * given buffer.
     */
    CARAPI Read(
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* number);

    /**
     * Construct a {@code GZIPInputStream} to read from GZIP data from the
     * underlying stream.
     *
     * @param is
     *            the {@code InputStream} to read data from.
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    CARAPI constructor(
        /* [in] */ IInputStream* is);

    /**
     * Construct a {@code GZIPInputStream} to read from GZIP data from the
     * underlying stream. Set the internal buffer size to {@code size}.
     *
     * @param is
     *            the {@code InputStream} to read data from.
     * @param size
     *            the internal read buffer size.
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    CARAPI constructor(
        /* [in] */ IInputStream* is,
        /* [in] */ Int32 size);

private:

    CARAPI VerifyCrc();

    CARAPI ReadFully(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer);

    CARAPI ReadZeroTerminated(
        /* [in] */ Boolean hcrc);

protected:
    /**
     * The checksum algorithm used when handling uncompressed data.
     */
    AutoPtr<CCRC32> mCrc;

    /**
     * Indicates the end of the input stream.
     */
    Boolean mEos;

private:
    static const Int32 FCOMMENT = 16;

    static const Int32 FEXTRA = 4;

    static const Int32 FHCRC = 2;

    static const Int32 FNAME = 8;
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CGZIPINPUTSTREAM_H__
