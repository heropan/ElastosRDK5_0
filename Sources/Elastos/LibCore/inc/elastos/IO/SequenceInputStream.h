
#ifndef __SEQUENCEINPUTSTREAM_H__
#define __SEQUENCEINPUTSTREAM_H__

#include "InputStream.h"

namespace Elastos {
namespace IO {

class SequenceInputStream : public InputStream
{
public:
    /**
     * Constructs a new {@code SequenceInputStream} using the two streams
     * {@code s1} and {@code s2} as the sequence of streams to read from.
     *
     * @param s1
     *            the first stream to get bytes from.
     * @param s2
     *            the second stream to get bytes from.
     * @throws NullPointerException
     *             if {@code s1} is {@code null}.
     */
    CARAPI Init(
        /* [in] */ IInputStream* s1,
        /* [in] */ IInputStream* s2);

    /**
     * Constructs a new SequenceInputStream using the elements returned from
     * Enumeration {@code e} as the stream sequence. The instances returned by
     * {@code e.nextElement()} must be of type {@link InputStream}.
     *
     * @param e
     *            the enumeration of {@code InputStreams} to get bytes from.
     * @throws NullPointerException
     *             if any of the elements in {@code e} is {@code null}.
     */
    CARAPI Init(
        /* [in] */ IObjectEnumerator* e);

    CARAPI Available(
        /* [out] */ Int32* number);

    /**
     * Closes all streams in this sequence of input stream.
     *
     * @throws IOException
     *             if an error occurs while closing any of the input streams.
     */
    CARAPI Close();

    /**
     * Reads a single byte from this sequence of input streams and returns it as
     * an integer in the range from 0 to 255. It tries to read from the current
     * stream first; if the end of this stream has been reached, it reads from
     * the next one. Blocks until one byte has been read, the end of the last
     * input stream in the sequence has been reached, or an exception is thrown.
     *
     * @return the byte read or -1 if either the end of the last stream in the
     *         sequence has been reached or this input stream sequence is
     *         closed.
     * @throws IOException
     *             if an error occurs while reading the current source input
     *             stream.
     */
    CARAPI Read(
        /* [out] */ Int32* value);

    /**
     * Reads at most {@code count} bytes from this sequence of input streams and
     * stores them in the byte array {@code buffer} starting at {@code offset}.
     * Blocks only until at least 1 byte has been read, the end of the stream
     * has been reached, or an exception is thrown.
     * <p>
     * This SequenceInputStream shows the same behavior as other InputStreams.
     * To do this it will read only as many bytes as a call to read on the
     * current substream returns. If that call does not return as many bytes as
     * requested by {@code count}, it will not retry to read more on its own
     * because subsequent reads might block. This would violate the rule that
     * it will only block until at least one byte has been read.
     * <p>
     * If a substream has already reached the end when this call is made, it
     * will close that substream and start with the next one. If there are no
     * more substreams it will return -1.
     *
     * @param buffer
     *            the array in which to store the bytes read.
     * @param offset
     *            the initial position in {@code buffer} to store the bytes read
     *            from this stream.
     * @param count
     *            the maximum number of bytes to store in {@code buffer}.
     * @return the number of bytes actually read; -1 if this sequence of streams
     *         is closed or if the end of the last stream in the sequence has
     *         been reached.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code count < 0}, or if {@code
     *             offset + count} is greater than the size of {@code buffer}.
     * @throws IOException
     *             if an I/O error occurs.
     * @throws NullPointerException
     *             if {@code buffer} is {@code null}.
     */
    CARAPI ReadBytesEx(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

private:
    /**
     * Sets up the next InputStream or leaves it alone if there are none left.
     *
     * @throws IOException
     */
    CARAPI NextStream();

private:
    AutoPtr<IObjectContainer> mContainer;

    /**
     * An enumeration which will return types of InputStream.
     */
    AutoPtr<IObjectEnumerator> mEnum;

    /**
     * The current input stream.
     */
    AutoPtr<IInputStream> mIn;
};

} // namespace IO
} // namespace Elastos

#endif //__SEQUENCEINPUTSTREAM_H__
