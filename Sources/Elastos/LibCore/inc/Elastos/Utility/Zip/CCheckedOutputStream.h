
#ifndef __CCHECKEDOUTPUTSTREAM_H__
#define __CCHECKEDOUTPUTSTREAM_H__


#include "_CCheckedOutputStream.h"
#include <FilterOutputStream.h>

using Elastos::IO::IOutputStream;
using Elastos::IO::FilterOutputStream;

namespace Elastos {
namespace Utility {
namespace Zip {

class CheckedOutputStream : public FilterOutputStream
{
public:

    CARAPI Init(
        /* [in] */ IOutputStream* os,
        /* [in] */ IChecksum* cs);

    /**
     * Returns the checksum calculated on the stream read so far.
     *
     * @return the updated checksum.
     */
    CARAPI GetChecksum(
        /* [out] */ IChecksum** checksum);

    /**
     * Writes the specified byte to the underlying stream. The checksum is
     * updated with {@code val}.
     *
     * @param val
     *            the data value to written to the output stream.
     * @throws IOException
     *             if an IO error has occurred.
     */
    //@Override
    CARAPI Write(
        /* [in] */ Int32 val);

    /**
     * Writes n bytes of data from {@code buf} starting at offset {@code off} to
     * the underlying stream. The checksum is updated with the bytes written.
     *
     * @param buf
     *            data written to the output stream.
     * @param off
     *            the offset to start reading the data from {@code buf} written
     *            to the output stream.
     * @param nbytes
     *            number of bytes to write to the output stream.
     * @throws IOException
     *             if an IO error has occurred.
     */
    //@Override
    CARAPI WriteBytesEx(
        /* [in] */ const ArrayOf<Byte>& buf,
        /* [in] */ Int32 off,
        /* [in] */ Int32 nbytes);

    CARAPI Close();

    CARAPI Flush();

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

private:
    AutoPtr<IChecksum> mCheck;
};

CarClass(CCheckedOutputStream), public CheckedOutputStream
{
public:
    /**
     * Returns the checksum calculated on the stream read so far.
     *
     * @return the updated checksum.
     */
    CARAPI GetChecksum(
        /* [out] */ IChecksum** checksum);

    /**
     * Writes the specified byte to the underlying stream. The checksum is
     * updated with {@code val}.
     *
     * @param val
     *            the data value to written to the output stream.
     * @throws IOException
     *             if an IO error has occurred.
     */
    //@Override
    CARAPI Write(
        /* [in] */ Int32 val);

    /**
     * Writes n bytes of data from {@code buf} starting at offset {@code off} to
     * the underlying stream. The checksum is updated with the bytes written.
     *
     * @param buf
     *            data written to the output stream.
     * @param off
     *            the offset to start reading the data from {@code buf} written
     *            to the output stream.
     * @param nbytes
     *            number of bytes to write to the output stream.
     * @throws IOException
     *             if an IO error has occurred.
     */
    //@Override
    CARAPI WriteBytesEx(
        /* [in] */ const ArrayOf<Byte>& buf,
        /* [in] */ Int32 off,
        /* [in] */ Int32 nbytes);

    CARAPI Close();

    CARAPI Flush();

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    /**
     * Constructs a new {@code CheckedOutputStream} on {@code OutputStream}
     * {@code os}. The checksum is calculated using the algorithm implemented
     * by {@code csum}.
     *
     * @param os
     *            the output stream to calculate checksum for.
     * @param cs
     *            an entity implementing the checksum algorithm.
     */
    CARAPI constructor(
        /* [in] */ IOutputStream* os,
        /* [in] */ IChecksum* cs);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};


} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__CCHECKEDOUTPUTSTREAM_H__
