
#ifndef __BUFFEREDREADER_H__
#define __BUFFEREDREADER_H__

#include "Reader.h"

namespace Elastos {
namespace IO {

/**
 * Wraps an existing {@link Reader} and <em>buffers</em> the input. Expensive
 * interaction with the underlying reader is minimized, since most (smaller)
 * requests can be satisfied by accessing the buffer alone. The drawback is that
 * some extra space is required to hold the buffer and that copying takes place
 * when filling that buffer, but this is usually outweighed by the performance
 * benefits.
 *
 * <p/>A typical application pattern for the class looks like this:<p/>
 *
 * <pre>
 * BufferedReader buf = new BufferedReader(new FileReader(&quot;file.java&quot;));
 * </pre>
 *
 * @see BufferedWriter
 * @since 1.1
 */
class BufferedReader
    : public Reader
{
protected:
    BufferedReader();

    ~BufferedReader();

    /**
     * Constructs a new {@code BufferedReader}, providing {@code in} with a buffer
     * of 8192 characters.
     *
     * @param in the {@code Reader} the buffer reads from.
     */
    CARAPI Init(
        /* [in] */ IReader* rin);

    /**
     * Constructs a new {@code BufferedReader}, providing {@code in} with {@code size} characters
     * of buffer.
     *
     * @param in the {@code InputStream} the buffer reads from.
     * @param size the size of buffer in characters.
     * @throws IllegalArgumentException if {@code size <= 0}.
     */
    CARAPI Init(
        /* [in] */ IReader* rin,
        /* [in] */ Int32 size);

public:
    /**
     * Closes this reader. This implementation closes the buffered source reader
     * and releases the buffer. Nothing is done if this reader has already been
     * closed.
     *
     * @throws IOException
     *             if an error occurs while closing this reader.
     */
    //@Override
    CARAPI Close();

    /**
     * Sets a mark position in this reader. The parameter {@code markLimit}
     * indicates how many characters can be read before the mark is invalidated.
     * Calling {@code reset()} will reposition the reader back to the marked
     * position if {@code markLimit} has not been surpassed.
     *
     * @param markLimit
     *            the number of characters that can be read before the mark is
     *            invalidated.
     * @throws IllegalArgumentException
     *             if {@code markLimit < 0}.
     * @throws IOException
     *             if an error occurs while setting a mark in this reader.
     * @see #markSupported()
     * @see #reset()
     */
    CARAPI Mark(
        /* [in] */ Int32 markLimit);

    /**
     * Indicates whether this reader supports the {@code mark()} and
     * {@code reset()} methods. This implementation returns {@code true}.
     *
     * @return {@code true} for {@code BufferedReader}.
     * @see #mark(int)
     * @see #reset()
     */
    //@Override
    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    /**
     * Reads a single character from this reader and returns it with the two
     * higher-order bytes set to 0. If possible, BufferedReader returns a
     * character from the buffer. If there are no characters available in the
     * buffer, it fills the buffer and then returns a character. It returns -1
     * if there are no more characters in the source reader.
     *
     * @return the character read or -1 if the end of the source reader has been
     *         reached.
     * @throws IOException
     *             if this reader is closed or some other I/O error occurs.
     */
    //@Override
    CARAPI Read(
        /* [out] */ Int32* value);

    /**
     * Reads at most {@code length} characters from this reader and stores them
     * at {@code offset} in the character array {@code buffer}. Returns the
     * number of characters actually read or -1 if the end of the source reader
     * has been reached. If all the buffered characters have been used, a mark
     * has not been set and the requested number of characters is larger than
     * this readers buffer size, BufferedReader bypasses the buffer and simply
     * places the results directly into {@code buffer}.
     *
     * @param buffer
     *            the character array to store the characters read.
     * @param offset
     *            the initial position in {@code buffer} to store the bytes read
     *            from this reader.
     * @param length
     *            the maximum number of characters to read, must be
     *            non-negative.
     * @return number of characters read or -1 if the end of the source reader
     *         has been reached.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code length < 0}, or if
     *             {@code offset + length} is greater than the size of
     *             {@code buffer}.
     * @throws IOException
     *             if this reader is closed or some other I/O error occurs.
     */
    //@Override
    CARAPI ReadCharsEx(
        /* [out] */ ArrayOf<Char32>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    /**
     * Returns the next line of text available from this reader. A line is
     * represented by zero or more characters followed by {@code '\n'},
     * {@code '\r'}, {@code "\r\n"} or the end of the reader. The string does
     * not include the newline sequence.
     *
     * @return the contents of the line or {@code null} if no characters were
     *         read before the end of the reader has been reached.
     * @throws IOException
     *             if this reader is closed or some other I/O error occurs.
     */
    virtual CARAPI ReadLine(
         /* [out] */ String* contents);

    /**
     * Indicates whether this reader is ready to be read without blocking.
     *
     * @return {@code true} if this reader will not block when {@code read} is
     *         called, {@code false} if unknown or blocking will occur.
     * @throws IOException
     *             if this reader is closed or some other I/O error occurs.
     * @see #read()
     * @see #read(char[], int, int)
     * @see #readLine()
     */
    //@Override
    CARAPI IsReady(
        /* [out] */ Boolean* ready);

    /**
     * Resets this reader's position to the last {@code mark()} location.
     * Invocations of {@code read()} and {@code skip()} will occur from this new
     * location.
     *
     * @throws IOException
     *             if this reader is closed or no mark has been set.
     * @see #mark(int)
     * @see #markSupported()
     */
    //@Override
    CARAPI Reset();

    /**
     * Skips {@code amount} characters in this reader. Subsequent
     * {@code read()}s will not return these characters unless {@code reset()}
     * is used. Skipping characters may invalidate a mark if {@code markLimit}
     * is surpassed.
     *
     * @param amount
     *            the maximum number of characters to skip.
     * @return the number of characters actually skipped.
     * @throws IllegalArgumentException
     *             if {@code amount < 0}.
     * @throws IOException
     *             if this reader is closed or some other I/O error occurs.
     * @see #mark(int)
     * @see #markSupported()
     * @see #reset()
     */
    //@Override
    CARAPI Skip(
        /* [in] */ Int64 amount,
        /* [out] */ Int64* number);

protected:
    /**
     * Peeks at the next input character, refilling the buffer if necessary. If
     * this character is a newline character ("\n"), it is discarded.
     */
    CARAPI ChompNewline();

private:
    /**
     * Populates the buffer with data. It is an error to call this method when
     * the buffer still contains data; ie. if {@code pos < end}.
     *
     * @return the number of bytes read into the buffer, or -1 if the end of the
     *      source stream has been reached.
     */
    CARAPI FillBuf(
        /* [out] */ Int32* number);

    /**
     * Indicates whether or not this reader is closed.
     *
     * @return {@code true} if this reader is closed, {@code false}
     *         otherwise.
     */
    CARAPI_(Boolean) IsClosed();

    CARAPI CheckNotClosed();

private:
    AutoPtr<IReader> mIn;

    /**
     * The characters that can be read and refilled in bulk. We maintain three
     * indices into this buffer:<pre>
     *     { X X X X X X X X X X X X - - }
     *           ^     ^             ^
     *           |     |             |
     *         mark   pos           end</pre>
     * Pos points to the next readable character. End is one greater than the
     * last readable character. When {@code pos == end}, the buffer is empty and
     * must be {@link #fillBuf() filled} before characters can be read.
     *
     * <p>Mark is the value pos will be set to on calls to {@link #reset}. Its
     * value is in the range {@code [0...pos]}. If the mark is {@code -1}, the
     * buffer cannot be reset.
     *
     * <p>MarkLimit limits the distance between the mark and the pos. When this
     * limit is exceeded, {@link #reset} is permitted (but not required) to
     * throw an exception. For shorter distances, {@link #reset} shall not throw
     * (unless the reader is closed).
     */
    AutoPtr< ArrayOf<Char32> > mBuf;

    Int32 mPos;

    Int32 mEnd;

    Int32 mMark;

    Int32 mMarkLimit;
};

} // namespace IO
} // namespace Elastos

#endif //__BUFFEREDREADER_H__
