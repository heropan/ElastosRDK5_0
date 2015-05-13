#ifndef __ELASTOS_IO_CHARSET_CMODIFIEDUTF8_H__
#define __ELASTOS_IO_CHARSET_CMODIFIEDUTF8_H__

#include "_CModifiedUtf8.h"

namespace Elastos {
namespace IO {
namespace Charset {

CarClass(CModifiedUtf8)
{
public:

    /*
     * Decodes a byte array containing modified UTF-8 bytes into
     * a string.
     * Note that although this method decodes the (supposedly
     * impossible) zero byte to U+0000, that's what the RI does too
     */
    CARAPI Decode(
        /* [in] */ ArrayOf<Byte>*   bytes,
        /* [in] */ ArrayOf<Char8>*  chars,
        /* [in] */ Int32            offset,
        /* [in] */ Int32            utfSize,
        /* [out] */ String*         string);

    CARAPI CountBytes(
        /* [in] */  String*   s,
        /* [in] */  Boolean   shortLength,
        /* [out] */ Int64*    num);

    CARAPI Encode(
        /* [in] */          String*         s,
        /* [out, callee] */ ArrayOf<Byte>**  bytes);

    CARAPI EncodeEx(
        /* [in] */ ArrayOf<Byte>* dst,
        /* [in] */ Int32          offset,
        /* [out] */ String*        s);
};

} // namespace Charset
} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_CHARSET_CMODIFIEDUTF8_H__
