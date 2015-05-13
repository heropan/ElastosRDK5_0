
#ifndef __CCRC32_H__
#define __CCRC32_H__

#include "_CCRC32.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CarClass(CCRC32)
{
public:
    CCRC32();

    /**
     * Returns the CRC32 checksum for all input received.
     *
     * @return The checksum for this instance.
     */
    CARAPI GetValue(
        /* [out] */ Int64* value);

    /**
     * Resets the CRC32 checksum to it initial state.
     */
    CARAPI Reset();

    /**
     * Updates this checksum with the byte value provided as integer.
     *
     * @param val
     *            represents the byte to update the checksum.
     */
    CARAPI Update(
        /* [in] */ Int32 val);

    /**
     * Updates this checksum with the bytes contained in buffer {@code buf}.
     *
     * @param buf
     *            the buffer holding the data to update the checksum with.
     */
    CARAPI UpdateEx(
        /* [in] */ const ArrayOf<Byte>& buf);

    /**
     * Update this {@code CRC32} checksum with the contents of {@code buf},
     * starting from {@code offset} and reading {@code byteCount} bytes of data.
     */
    CARAPI UpdateEx2(
        /* [in] */ const ArrayOf<Byte>& buf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount);

private:
    CARAPI_(Int64) UpdateImpl(
        /* [in] */ const ArrayOf<Byte>& buf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int64 crc);

    CARAPI_(Int64) UpdateByteImpl(
        /* [in] */ Byte val,
        /* [in] */ Int64 crc);

public:
    Int64 mTbytes;

private:
    Int64 mCrc;
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__CCRC32_H__
