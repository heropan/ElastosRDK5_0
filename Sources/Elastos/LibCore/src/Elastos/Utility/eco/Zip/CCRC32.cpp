
#include "cmdef.h"
#include "CCRC32.h"
#include <zlib.h>

namespace Elastos {
namespace Utility {
namespace Zip {


CCRC32::CCRC32()
    : mTbytes(0ll)
    , mCrc(0ll)
{}

/**
 * Returns the CRC32 checksum for all input received.
 *
 * @return The checksum for this instance.
 */
ECode CCRC32::GetValue(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mCrc;
    return NOERROR;
}

/**
 * Resets the CRC32 checksum to it initial state.
 */
ECode CCRC32::Reset()
{
    mTbytes = mCrc = 0;
    return NOERROR;
}

/**
 * Updates this checksum with the byte value provided as integer.
 *
 * @param val
 *            represents the byte to update the checksum.
 */
ECode CCRC32::Update(
    /* [in] */ Int32 val)
{
    mCrc = UpdateByteImpl((Byte)val, mCrc);
    return NOERROR;
}

/**
 * Updates this checksum with the bytes contained in buffer {@code buf}.
 *
 * @param buf
 *            the buffer holding the data to update the checksum with.
 */
ECode CCRC32::UpdateEx(
    /* [in] */ const ArrayOf<Byte>& buf)
{
    return UpdateEx2(buf, 0, buf.GetLength());
}

/**
 * Update this {@code CRC32} checksum with the contents of {@code buf},
 * starting from {@code offset} and reading {@code byteCount} bytes of data.
 */
ECode CCRC32::UpdateEx2(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount)
{
    if ((offset | byteCount) < 0 || offset > buf.GetLength()
            || buf.GetLength() - offset < byteCount) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
//                count);
    }
    mTbytes += byteCount;
    mCrc = UpdateImpl(buf, offset, byteCount, mCrc);
    return NOERROR;
}

Int64 CCRC32::UpdateImpl(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int64 crc)
{
    return crc32(crc, reinterpret_cast<const Bytef*>(buf.GetPayload() + offset), byteCount);
}

Int64 CCRC32::UpdateByteImpl(
    /* [in] */ Byte val,
    /* [in] */ Int64 crc)
{
    return crc32(crc, reinterpret_cast<const Bytef*>(&val), 1);
}


} // namespace Zip
} // namespace Utility
} // namespace Elastos
