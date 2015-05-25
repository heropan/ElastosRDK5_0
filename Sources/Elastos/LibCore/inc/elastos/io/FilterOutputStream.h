
#ifndef __FILTEROUTPUTSTREAM_H__
#define __FILTEROUTPUTSTREAM_H__

#include "OutputStream.h"

namespace Elastos {
namespace IO {

extern "C" const InterfaceID EIID_FilterOutputStream;

class FilterOutputStream
    : public OutputStream
    , public IFilterOutputStream
{
protected:
    FilterOutputStream();

    virtual ~FilterOutputStream();

    CARAPI constructor(
        /* [in] */ IOutputStream* outs);

public:
    CAR_INTERFACE_DECL()

    /**
     * Closes this stream. This implementation closes the target stream.
     *
     * @throws IOException
     *             if an error occurs attempting to close this stream.
     */
    CARAPI Close();

    /**
     * Ensures that all pending data is sent out to the target stream. This
     * implementation flushes the target stream.
     *
     * @throws IOException
     *             if an error occurs attempting to flush this stream.
     */
    CARAPI Flush();

    /**
     * Writes one byte to the target stream. Only the low order byte of the
     * integer {@code oneByte} is written.
     *
     * @param oneByte
     *            the byte to be written.
     * @throws IOException
     *             if an I/O error occurs while writing to this stream.
     */
    CARAPI Write(
        /* [in] */ Int32 oneByte);

    /**
     * Writes {@code count} bytes from the byte array {@code buffer} starting at
     * {@code offset} to the target stream.
     *
     * @param buffer
     *            the buffer to write.
     * @param offset
     *            the index of the first byte in {@code buffer} to write.
     * @param length
     *            the number of bytes in {@code buffer} to write.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code count < 0}, or if
     *             {@code offset + count} is bigger than the length of
     *             {@code buffer}.
     * @throws IOException
     *             if an I/O error occurs while writing to this stream.
     */
    CARAPI Write(
        /* [in] */ const ArrayOf<Byte> & buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

public:

    CARAPI ToString(
        /* [out] */ String* s);

    CARAPI GetClassID(
        /* [out] */ ClassID *pCLSID);

protected:
    /**
     * The target output stream for this filter stream.
     */
    AutoPtr<IOutputStream> mOut;
};

} // namespace IO
} // namespace Elastos

#endif // __FILTEROUTPUTSTREAM_H__
