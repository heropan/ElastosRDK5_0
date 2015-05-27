
#include "coredef.h"
#include "CByteOrderHelper.h"
#include <stdio.h>

namespace Elastos {
namespace IO {

ByteOrder CByteOrderHelper::sNativeOrder;
ByteOrder CByteOrderHelper::sBigEndian;
ByteOrder CByteOrderHelper::sLittleEndian;
Boolean CByteOrderHelper::sIsLittleEndian;
Boolean CByteOrderHelper::sIsInited = CByteOrderHelper::Init();

ECode CByteOrderHelper::GetNativeOrder(
        /* [out] */ByteOrder* nativeOrder)
{
    VALIDATE_NOT_NULL(nativeOrder);
    *nativeOrder = sNativeOrder;

    return NOERROR;
}

Boolean CByteOrderHelper::Init()
{
    sIsLittleEndian = IsLittleEndian();
    sBigEndian      = ByteOrder_BIG_ENDIAN;
    sLittleEndian   = ByteOrder_LITTLE_ENDIAN;
    sNativeOrder    = sIsLittleEndian ? sLittleEndian : sBigEndian;
    return TRUE;
}

Boolean CByteOrderHelper::IsLittleEndian()
{
    int i = 1;
    return *reinterpret_cast<Byte*>(&i) == 1;
}

ECode CByteOrderHelper::IsNeedsSwap(
    /* [in] */ ByteOrder order,
    /* [out] */ Boolean* isNeedsSwap)
{
    VALIDATE_NOT_NULL(isNeedsSwap)
    if (order == ByteOrder_BIG_ENDIAN) {
        *isNeedsSwap = sIsLittleEndian;
        return NOERROR;
    }
    else if (order == ByteOrder_LITTLE_ENDIAN) {
        *isNeedsSwap = !sIsLittleEndian;
        return NOERROR;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

} // namespace IO
} // namespace Elastos
