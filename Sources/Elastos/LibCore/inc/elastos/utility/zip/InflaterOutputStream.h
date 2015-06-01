
#ifndef __ELASTOS_UTILITY_INFLATEROUTPUTSTREAM_H__
#define __ELASTOS_UTILITY_INFLATEROUTPUTSTREAM_H__

#include <FilterOutputStream.h>
#include "CInflater.h"

using Elastos::IO::IOutputStream;
using Elastos::IO::FilterOutputStream;

namespace Elastos {
namespace Utility {
namespace Zip {

/**
 * An {@code OutputStream} filter to decompress data. Callers write
 * compressed data in the "deflate" format, and uncompressed data is
 * written to the underlying stream.
 * @since 1.6
 */
class InflaterOutputStream
    : public FilterOutputStream
    , public IInflaterOutputStream
{
public:
    CAR_INTERFACE_DECL()

    InflaterOutputStream();

    virtual ~InflaterOutputStream();

    /**
     * Writes remaining data into the output stream and closes the underlying
     * output stream.
     */
    //@Override
    virtual CARAPI Close();

    //@Override
    virtual CARAPI Flush();

    /**
     * Finishes writing current uncompressed data into the InflaterOutputStream
     * without closing it.
     *
     * @throws IOException if an I/O error occurs, or the stream has been closed
     */
    virtual CARAPI Finish();

    /**
     * Writes a byte to the decompressing output stream. {@code b} should be a byte of
     * compressed input. The corresponding uncompressed data will be written to the underlying
     * stream.
     *
     * @param b the byte
     * @throws IOException if an I/O error occurs, or the stream has been closed
     * @throws ZipException if a zip exception occurs.
     */
    //@Override
    virtual CARAPI Write(
        /* [in] */ Int32 b);

    /**
     * Writes bytes to the decompressing output stream. {@code b} should be an array of
     * compressed input. The corresponding uncompressed data will be written to the underlying
     * stream.
     *
     * @throws IOException if an I/O error occurs, or the stream has been closed
     * @throws ZipException if a zip exception occurs.
     * @throws NullPointerException if {@code b == null}.
     * @throws IndexOutOfBoundsException if {@code off < 0 || len < 0 || off + len > b.length}
     */
    //@Override
    virtual CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount);

    //inherit frome super class
    virtual CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    virtual CARAPI CheckError(
        /* [out] */ Boolean* hasError);

protected:
    /**
     * Constructs an {@code InflaterOutputStream} with a new {@code Inflater} and an
     * implementation-defined default internal buffer size. {@code out} is a destination
     * for uncompressed data, and compressed data will be written to this stream.
     *
     * @param out the destination {@code OutputStream}
     */
    CARAPI Init(
        /* [in] */ IOutputStream* out);

    /**
     * Constructs an {@code InflaterOutputStream} with the given {@code Inflater} and an
     * implementation-defined default internal buffer size. {@code out} is a destination
     * for uncompressed data, and compressed data will be written to this stream.
     *
     * @param out the destination {@code OutputStream}
     * @param inf the {@code Inflater} to be used for decompression
     */
    CARAPI Init(
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
    CARAPI Init(
        /* [in] */ IOutputStream* out,
        /* [in] */ IInflater* inf,
        /* [in] */ Int32 bufferSize);

private:
    CARAPI Write();

    CARAPI CheckClosed();

protected:
    AutoPtr<IOutputStream> mOut;
    AutoPtr<CInflater> mInf;
    AutoPtr<ArrayOf<Byte> > mBuf;

private:
    static const Int32 DEFAULT_BUFFER_SIZE = 1024;

    Boolean mClosed;
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_INFLATEROUTPUTSTREAM_H__
