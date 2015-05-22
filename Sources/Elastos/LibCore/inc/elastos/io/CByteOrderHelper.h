#ifndef __CBYTEORDERHELPER_H__
#define __CBYTEORDERHELPER_H__

#include <elastos.h>
#include <Elastos.CoreLibrary_server.h>

#include "_CByteOrderHelper.h"

namespace Elastos {
namespace IO {

/*
 * Defines byte order constants
 */
CarClass(CByteOrderHelper)
{
public:
    /**
     * Returns the current platform byte order.
     *
     * @return the byte order object, which is either LITTLE_ENDIAN or
     *         BIG_ENDIAN.
     */
    CARAPI GetNativeOrder(
        /* [out] */ ByteOrder* nativeOrder);

    /**
     * This is the only thing that ByteOrder is really used for: to know whether we need to swap
     * bytes to get this order, given bytes in native order. (That is, this is the opposite of
     * the hypothetical "isNativeOrder".)
     * @hide - needed in libcore.io too.
     */
    CARAPI IsNeedsSwap(
        /* [in] */ ByteOrder order,
        /* [out] */ Boolean* isNeedsSwap);

private:
    static Boolean Init();
    static Boolean IsLittleEndian();

private:
    static ByteOrder    sNativeOrder;
    static ByteOrder    sBigEndian;
    static ByteOrder    sLittleEndian;
    static Boolean      sIsLittleEndian;
    static Boolean      sIsInited;
};

} // namespace IO
} // namespace Elastos

#endif //__CBYTEORDERHELPER_H__
