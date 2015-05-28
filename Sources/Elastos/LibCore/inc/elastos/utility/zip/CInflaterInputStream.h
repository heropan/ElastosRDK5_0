
#ifndef __CINFLATERINPUTSTREAM_H__
#define __CINFLATERINPUTSTREAM_H__

#include "_CInflaterInputStream.h"
#include "InflaterInputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CarClass(CInflaterInputStream) , public InflaterInputStream
{
public:
    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

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

#endif //__CINFLATERINPUTSTREAM_H__
