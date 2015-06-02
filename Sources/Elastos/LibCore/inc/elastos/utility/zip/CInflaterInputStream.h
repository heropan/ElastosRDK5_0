
#ifndef __ELASTOS_UTILITY_CINFLATERINPUTSTREAM_H__
#define __ELASTOS_UTILITY_CINFLATERINPUTSTREAM_H__

#include "_Elastos_Utility_Zip_CInflaterInputStream.h"
#include "InflaterInputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CarClass(CInflaterInputStream)
    , public InflaterInputStream
{
public:
    CAR_OBJECT_DECL()

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    /**
     * This is the most basic constructor. You only need to pass the {@code
     * InputStream} from which the compressed data is to be read from. Default
     * settings for the {@code Inflater} and internal buffer are be used. In
     * particular the Inflater expects a ZLIB header from the input stream.
     *
     * @param is
     *            the {@code InputStream} to read data from.
     */
    CARAPI constructor(
        /* [in] */ IInputStream* is);

    /**
     * This constructor lets you pass a specifically initialized Inflater,
     * for example one that expects no ZLIB header.
     *
     * @param is
     *            the {@code InputStream} to read data from.
     * @param inflater
     *            the specific {@code Inflater} for uncompressing data.
     */
    CARAPI constructor(
        /* [in] */ IInputStream* is,
        /* [in] */ IInflater* inflater);

    /**
     * This constructor lets you specify both the {@code Inflater} as well as
     * the internal buffer size to be used.
     *
     * @param is
     *            the {@code InputStream} to read data from.
     * @param inflater
     *            the specific {@code Inflater} for uncompressing data.
     * @param bsize
     *            the size to be used for the internal buffer.
     */
    CARAPI constructor(
        /* [in] */ IInputStream* is,
        /* [in] */ IInflater* inflater,
        /* [in] */ Int32 bsize);
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CINFLATERINPUTSTREAM_H__
