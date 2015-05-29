
#ifndef __OUTPUTSTREAM_H__
#define __OUTPUTSTREAM_H__

#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Elastos {
namespace IO {

class OutputStream
    : public Object
    , public ICloseable
    , public IFlushable
    , public IOutputStream
{
public:
    CAR_INTERFACE_DECL()

    OutputStream();

    virtual ~OutputStream();

    /**
     * Closes this stream. Implementations of this method should free any
     * resources used by the stream. This implementation does nothing.
     *
     * @throws IOException
     *             if an error occurs while closing this stream.
     */
    CARAPI Close();

    /**
     * Flushes this stream. Implementations of this method should ensure that
     * any buffered data is written out. This implementation does nothing.
     *
     * @throws IOException
     *             if an error occurs while flushing this stream.
     */
    CARAPI Flush();

    /**
     * Writes the entire contents of the byte array {@code buffer} to this
     * stream.
     *
     * @param buffer
     *            the buffer to be written.
     * @throws IOException
     *             if an error occurs while writing to this stream.
     */
    CARAPI Write(
        /* [in] */ const ArrayOf<Byte>& buffer);

    /**
     * Writes {@code count} bytes from the byte array {@code buffer} starting at
     * position {@code offset} to this stream.
     *
     * @param buffer
     *            the buffer to be written.
     * @param offset
     *            the start position in {@code buffer} from where to get bytes.
     * @param count
     *            the number of bytes from {@code buffer} to write to this
     *            stream.
     * @throws IOException
     *             if an error occurs while writing to this stream.
     * @throws IndexOutOfBoundsException
     *             if {@code offset < 0} or {@code count < 0}, or if
     *             {@code offset + count} is bigger than the length of
     *             {@code buffer}.
     */
    CARAPI Write(
        /* [in] */ const ArrayOf<Byte> & buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    /**
     * Writes a single byte to this stream. Only the least significant byte of
     * the integer oneByte is written to the stream.
     *
     * @param oneByte
     *            the byte to be written.
     * @return ECode = NOERROR(0) if success, else as follows:
     * Value | Meaning |
     * :-|:------------|
     * IOException | if an error occurs while writing to this stream.
     */
    CARAPI Write(
        /* [in] */ Int32 oneByte)=0;


    /**
     * Returns true if this writer has encountered and suppressed an error. Used
     * by PrintStreams as an alternative to checked exceptions.
     */
    CARAPI CheckError(
        /* [out] */ Boolean* hasError);
};

} // namespace IO
} // namespace Elastos

#endif //__OUTPUTSTREAM_H__
