
#ifndef __CINPUTSTREAMREADER_H__
#define __CINPUTSTREAMREADER_H__

#include "_CInputStreamReader.h"
#include "InputStreamReader.h"

namespace Elastos {
namespace IO {

CarClass(CInputStreamReader)
    , public InputStreamReader
{
public:
    /**
     * Constructs a new {@code InputStreamReader} on the {@link InputStream}
     * {@code in}. This constructor sets the character converter to the encoding
     * specified in the "file.encoding" property and falls back to ISO 8859_1
     * (ISO-Latin-1) if the property doesn't exist.
     *
     * @param in
     *            the input stream from which to read characters.
     */
    CARAPI constructor(
        /* [in] */ IInputStream* in);

    /**
     * Constructs a new InputStreamReader on the InputStream {@code in}. The
     * character converter that is used to decode bytes into characters is
     * identified by name by {@code enc}. If the encoding cannot be found, an
     * UnsupportedEncodingException error is thrown.
     *
     * @param in
     *            the InputStream from which to read characters.
     * @param enc
     *            identifies the character converter to use.
     * @throws NullPointerException
     *             if {@code enc} is {@code null}.
     * @throws UnsupportedEncodingException
     *             if the encoding specified by {@code enc} cannot be found.
     */
    CARAPI constructor(
        /* [in] */ IInputStream* in,
        /* [in] */ const String& enc);

    /**
     * Constructs a new InputStreamReader on the InputStream {@code in} and
     * Charset {@code charset}.
     *
     * @param in
     *            the source InputStream from which to read characters.
     * @param charset
     *            the Charset that defines the character converter
     */
    // CARAPI constructor(
    //     /* [in] */ IInputStream* in,
    //     /* [in] */ ICharset* charset);


    /**
     * Closes this reader. This implementation closes the source InputStream and
     * releases all local storage.
     *
     * @throws IOException
     *             if an error occurs attempting to close this reader.
     */
    CARAPI Close();

    /**
     * Returns the name of the encoding used to convert bytes into characters.
     * The value {@code null} is returned if this reader has been closed.
     *
     * @return the name of the character converter or {@code null} if this
     *         reader is closed.
     */
    CARAPI GetEncoding(
        /* [out] */ String* encoding);

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    /**
     * Reads a single character from this reader and returns it as an integer
     * with the two higher-order bytes set to 0. Returns -1 if the end of the
     * reader has been reached. The byte value is either obtained from
     * converting bytes in this reader's buffer or by first filling the buffer
     * from the source InputStream and then reading from the buffer.
     *
     * @return the character read or -1 if the end of the reader has been
     *         reached.
     * @throws IOException
     *             if this reader is closed or some other I/O error occurs.
     */
    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadChars(
        /* [out] */ ArrayOf<Char32>* buffer,
        /* [out] */ Int32* number);

    /**
     * Reads at most {@code length} characters from this reader and stores them
     * at position {@code offset} in the character array {@code buf}. Returns
     * the number of characters actually read or -1 if the end of the reader has
     * been reached. The bytes are either obtained from converting bytes in this
     * reader's buffer or by first filling the buffer from the source
     * InputStream and then reading from the buffer.
     *
     * @param buffer
     *            the array to store the characters read.
     * @param offset
     *            the initial position in {@code buf} to store the characters
     *            read from this reader.
     * @param length
     *            the maximum number of characters to read.
     * @return the number of characters read or -1 if the end of the reader has
     *         been reached.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code length < 0}, or if
     *             {@code offset + length} is greater than the length of
     *             {@code buf}.
     * @throws IOException
     *             if this reader is closed or some other I/O error occurs.
     */
    CARAPI ReadChars(
        /* [out] */ ArrayOf<Char32>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [out] */ Int32* number);

    /**
     * Indicates whether this reader is ready to be read without blocking. If
     * the result is {@code true}, the next {@code read()} will not block. If
     * the result is {@code false} then this reader may or may not block when
     * {@code read()} is called. This implementation returns {@code true} if
     * there are bytes available in the buffer or the source stream has bytes
     * available.
     *
     * @return {@code true} if the receiver will not block when {@code read()}
     *         is called, {@code false} if unknown or blocking will occur.
     * @throws IOException
     *             if this reader is closed or some other I/O error occurs.
     */
    CARAPI IsReady(
        /* [out] */ Boolean* ready);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI ReadCharBuffer(
        /* [in] */ ICharBuffer* target,
        /* [out] */ Int32* number);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace IO
} // namespace Elastos

#endif //__CINPUTSTREAMREADER_H__
