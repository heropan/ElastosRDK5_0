
#ifndef __STRINGBUFFERINPUTSTREAM_H__
#define __STRINGBUFFERINPUTSTREAM_H__

#include "InputStream.h"

namespace Elastos {
namespace IO {

class StringBufferInputStream : public InputStream
{
protected:
    StringBufferInputStream();

    ~StringBufferInputStream();

    CARAPI Init(
        /* [in] */ const String& str);

public:
    CARAPI Available(
        /* [out] */ Int32* number);

    /**
     * Reads a single byte from the source string and returns it as an integer
     * in the range from 0 to 255. Returns -1 if the end of the source string
     * has been reached.
     *
     * @return the byte read or -1 if the end of the source string has been
     *         reached.
     */
    CARAPI Read(
        /* [out] */ Int32* value);

    /**
     * Reads at most {@code length} bytes from the source string and stores them
     * in the byte array {@code b} starting at {@code offset}.
     *
     * @param buffer
     *            the byte array in which to store the bytes read.
     * @param offset
     *            the initial position in {@code b} to store the bytes read from
     *            this stream.
     * @param length
     *            the maximum number of bytes to store in {@code b}.
     * @return the number of bytes actually read or -1 if the end of the source
     *         string has been reached.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code length < 0}, or if
     *             {@code offset + length} is greater than the length of
     *             {@code b}.
     * @throws NullPointerException
     *             if {@code b} is {@code null}.
     */
    CARAPI ReadBytesEx(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    /**
     * Resets this stream to the beginning of the source string.
     */
    CARAPI Reset();

    /**
     * Skips {@code n} characters in the source string. It does nothing and
     * returns 0 if {@code n} is negative. Less than {@code n} characters are
     * skipped if the end of the source string is reached before the operation
     * completes.
     *
     * @param n
     *            the number of characters to skip.
     * @return the number of characters actually skipped.
     */
    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

protected:
    /**
     * The source string containing the data to read.
     */
    String mBuffer;

    /**
     * The total number of characters in the source string.
     */
    Int32 mCount;

    /**
     * The current position within the source string.
     */
    Int32 mPos;
};

} // namespace IO
} // namespace Elastos

#endif //__STRINGBUFFERINPUTSTREAM_H__
