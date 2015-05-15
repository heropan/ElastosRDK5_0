#ifndef __BYTEARRAYINPUTSTREAM_H__
#define __BYTEARRAYINPUTSTREAM_H__

#include "InputStream.h"

namespace Elastos {
namespace IO {

class ByteArrayInputStream
    : public InputStream
{
protected:
    ByteArrayInputStream();

    ~ByteArrayInputStream();

    CARAPI Init(
        /* [in] */ ArrayOf<Byte>* buffer);

    CARAPI Init(
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

public:
    /**
     * Returns the number of remaining bytes.
     *
     * @return {@code count - pos}
     */
    // synchronized
    CARAPI Available(
        /* [out] */ Int32* number);

    /**
     * Closes this stream and frees resources associated with this stream.
     *
     * @throws IOException
     *             if an I/O error occurs while closing this stream.
     */
    CARAPI Close();

    /**
     * Sets a mark position in this ByteArrayInputStream. The parameter
     * {@code readlimit} is ignored. Sending {@code reset()} will reposition the
     * stream back to the marked position.
     *
     * @param readlimit
     *            ignored.
     * @see #markSupported()
     * @see #reset()
     */
    // synchronized
    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    /**
     * Indicates whether this stream supports the {@code mark()} and
     * {@code reset()} methods. Returns {@code true} since this class supports
     * these methods.
     *
     * @return always {@code true}.
     * @see #mark(int)
     * @see #reset()
     */
    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    /**
     * Reads a single byte from the source byte array and returns it as an
     * integer in the range from 0 to 255. Returns -1 if the end of the source
     * array has been reached.
     *
     * @return the byte read or -1 if the end of this stream has been reached.
     */
    // synchronized
    CARAPI Read(
        /* [out] */ Int32* value);

    /**
     * Reads at most {@code len} bytes from this stream and stores
     * them in byte array {@code b} starting at {@code offset}. This
     * implementation reads bytes from the source byte array.
     *
     * @param buffer
     *            the byte array in which to store the bytes read.
     * @param offset
     *            the initial position in {@code b} to store the bytes read from
     *            this stream.
     * @param length
     *            the maximum number of bytes to store in {@code b}.
     * @return the number of bytes actually read or -1 if no bytes were read and
     *         the end of the stream was encountered.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code length < 0}, or if
     *             {@code offset + length} is greater than the size of
     *             {@code b}.
     * @throws NullPointerException
     *             if {@code b} is {@code null}.
     */
    // synchronized
    CARAPI ReadBytesEx(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    /**
     * Resets this stream to the last marked location. This implementation
     * resets the position to either the marked position, the start position
     * supplied in the constructor or 0 if neither has been provided.
     *
     * @see #mark(int)
     */
    // synchronized
    CARAPI Reset();

    /**
     * Skips {@code count} number of bytes in this InputStream. Subsequent
     * {@code read()}s will not return these bytes unless {@code reset()} is
     * used. This implementation skips {@code count} number of bytes in the
     * target stream. It does nothing and returns 0 if {@code n} is negative.
     *
     * @param n
     *            the number of bytes to skip.
     * @return the number of bytes actually skipped.
     */
    // synchronized
    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

protected:
    /**
     * The {@code byte} array containing the bytes to stream over.
     */
    AutoPtr<ArrayOf<Byte> > mBuf;

    /**
     * The current position within the byte array.
     */
    Int32 mPos;

    /**
     * The current mark position. Initially set to 0 or the <code>offset</code>
     * parameter within the constructor.
     */
    Int32 mMark;

    /**
     * The total number of bytes initially available in the byte array
     * {@code buf}.
     */
    Int32 mCount;
};

} // namespace IO
} // namespace Elastos

#endif // __BYTEARRAYINPUTSTREAM_H__
