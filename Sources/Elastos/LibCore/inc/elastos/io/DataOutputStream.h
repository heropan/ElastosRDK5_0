
#ifndef __DATAOUTPUTSTREAM_H__
#define __DATAOUTPUTSTREAM_H__

#include "FilterOutputStream.h"

namespace Elastos {
namespace IO {

class DataOutputStream
    : public FilterOutputStream
{
protected:
    DataOutputStream();

    ~DataOutputStream();

    CARAPI Init(
        /* [in] */ IOutputStream* out);

public:
    /**
     * Flushes this stream to ensure all pending data is sent out to the target
     * stream. This implementation then also flushes the target stream.
     *
     * @throws IOException
     *             if an error occurs attempting to flush this stream.
     */
    CARAPI Flush();

    /**
     * Returns the total number of bytes written to the target stream so far.
     *
     * @return the number of bytes written to the target stream.
     */
    CARAPI GetSize(
    /* [out] */ Int32* value);

    /**
     * Writes a byte to the target stream. Only the least significant byte of
     * the integer {@code oneByte} is written.
     *
     * @param oneByte
     *            the byte to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @see DataInputStream#readByte()
     */
    CARAPI Write(
        /* [in] */ Int32 oneByte);

    /**
     * Writes {@code count} bytes from the byte array {@code buffer} starting at
     * {@code offset} to the target stream.
     *
     * @param buffer
     *            the buffer to write to the target stream.
     * @param offset
     *            the index of the first byte in {@code buffer} to write.
     * @param count
     *            the number of bytes from the {@code buffer} to write.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @throws NullPointerException
     *             if {@code buffer} is {@code null}.
     * @see DataInputStream#readFully(byte[])
     * @see DataInputStream#readFully(byte[], int, int)
     */
    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte> & buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    /**
     * Writes a boolean to the target stream.
     *
     * @param val
     *            the boolean value to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @see DataInputStream#readBoolean()
     */
    CARAPI WriteBoolean(
        /* [in] */ Boolean val);

    /**
     * Writes an 8-bit byte to the target stream. Only the least significant
     * byte of the integer {@code val} is written.
     *
     * @param val
     *            the byte value to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @see DataInputStream#readByte()
     * @see DataInputStream#readUnsignedByte()
     */
    CARAPI WriteByte(
        /* [in] */ Int32 val);

    /**
     * Writes the low order bytes from a string to the target stream.
     *
     * @param str
     *            the string containing the bytes to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @see DataInputStream#readFully(byte[])
     * @see DataInputStream#readFully(byte[],int,int)
     */
    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    /**
     * Writes the low order 8-bit bytes from the specified string.
     *
     * @param str
     *            the string containing the bytes to write.
     * @throws IOException
     *             if an I/O error occurs while writing.
     */
    CARAPI WriteBytesFromString(
        /* [in] */ const String& str);

    /**
     * Writes a 32-bit character to the target stream. Only the two lower bytes
     * of the integer {@code val} are written, with the higher one written
     * first. This corresponds to the Unicode value of {@code val}.
     *
     * @param val
     *            the character to write to the target stream
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @see DataInputStream#readChar()
     */
    CARAPI WriteChar(
        /* [in] */ Int32 val);

    /**
     * Writes the 16-bit characters contained in {@code str} in big-endian order.
     *
     * @param str
     *            the string that contains the characters to write.
     * @throws IOException
     *             if an I/O error occurs while writing.
     * @see DataInput#readChar()
     */
    CARAPI WriteChars(
        /* [in] */ const String& str);

    /**
     * Writes a 64-bit double to the target stream. The resulting output is the
     * eight bytes resulting from calling Double.doubleToLongBits().
     *
     * @param val
     *            the double to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @see DataInputStream#readDouble()
     */
    CARAPI WriteDouble(
        /* [in] */ Double val);

    /**
     * Writes a 32-bit float to the target stream. The resulting output is the
     * four bytes resulting from calling Float.floatToIntBits().
     *
     * @param val
     *            the float to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @see DataInputStream#readFloat()
     */
    CARAPI WriteFloat(
        /* [in] */ Float val);

    /**
     * Writes a 32-bit int to the target stream. The resulting output is the
     * four bytes, highest order first, of {@code val}.
     *
     * @param val
     *            the int to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @see DataInputStream#readInt()
     */
    CARAPI WriteInt32(
        /* [in] */ Int32 val);

    /**
     * Writes a 64-bit long to the target stream. The resulting output is the
     * eight bytes, highest order first, of {@code val}.
     *
     * @param val
     *            the long to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @see DataInputStream#readLong()
     */
    CARAPI WriteInt64(
        /* [in] */ Int64 val);

    /**
     * Writes the specified 16-bit short to the target stream. Only the lower
     * two bytes of the integer {@code val} are written, with the higher one
     * written first.
     *
     * @param val
     *            the short to write to the target stream.
     * @throws IOException
     *             if an error occurs while writing to the target stream.
     * @see DataInputStream#readShort()
     * @see DataInputStream#readUnsignedShort()
     */
    CARAPI WriteInt16(
        /* [in] */ Int32 val);

    /**
     * Writes the specified string encoded in {@link DataInput modified UTF-8}.
     *
     * @param str
     *            the string to write encoded in {@link DataInput modified UTF-8}.
     * @throws IOException
     *             if an I/O error occurs while writing.
     * @see DataInput#readUTF()
         */
    CARAPI WriteUTF(
        /* [in] */ const String& str);

protected:
    CARAPI_(Int32) CountUTFBytes(
        /* [in] */ Char32 srcChar);

    CARAPI_(void) WriteUTFBytesToBuffer(
        /* [in] */ Byte* dstP,
        /* [in] */ Char32 srcChar,
        /* [in] */ Int32 bytes);

protected:
    /**
     * The number of bytes written out so far.
     */
    Int32 mWritten;

    AutoPtr<ArrayOf<Byte> > mBuff;
};

} // namespace IO
} // namespace Elastos

#endif
