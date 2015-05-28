
#ifndef __CADLER32_H__
#define __CADLER32_H__

#include "_CAdler32.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CarClass(CAdler32)
{
public:
    CAdler32();

    /**
     * Returns the {@code Adler32} checksum for all input received.
     *
     * @return The checksum for this instance.
     */
    CARAPI GetValue(
        /* [out] */ Int64* value);

    /**
     * Reset this instance to its initial checksum.
     */
    CARAPI Reset();

    /**
     * Update this {@code Adler32} checksum with the single byte provided as
     * argument.
     *
     * @param i
     *            the byte to update checksum with.
     */
    CARAPI Update(
        /* [in] */ Int32 i);

    /**
     * Update this {@code Adler32} checksum using the contents of {@code buf}.
     *
     * @param buf
     *            bytes to update checksum with.
     */
    CARAPI Update(
        /* [in] */ const ArrayOf<Byte>& buf);

    /**
     * Update this {@code Adler32} checksum with the contents of {@code buf},
     * starting from {@code offset} and reading {@code byteCount} bytes of data.
     */
    CARAPI Update(
        /* [in] */ const ArrayOf<Byte>& buf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount);

private:
    CARAPI_(Int64) UpdateImpl(
        /* [in] */ const ArrayOf<Byte>& buf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int64 adler);

    CARAPI_(Int64) UpdateByteImpl(
        /* [in] */ Int32 val,
        /* [in] */ Int64 adler);

private:
    Int64 mAdler;
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__CADLER32_H__
