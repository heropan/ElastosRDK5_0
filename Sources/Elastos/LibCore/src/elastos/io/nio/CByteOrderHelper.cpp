
#include "CByteOrderHelper.h"

namespace Elastos {
namespace IO {

ByteOrder CByteOrderHelper::sNativeOrder;
ByteOrder CByteOrderHelper::sBigEndian;
ByteOrder CByteOrderHelper::sLittleEndian;
Boolean CByteOrderHelper::sIsLittleEndian = FALSE;
Boolean CByteOrderHelper::sIsInited = FALSE;

CAR_INTERFACE_IMPL(CByteOrderHelper, Singleton, IByteOrderHelper)

CAR_SINGLETON_IMPL(CByteOrderHelper)

Boolean CByteOrderHelper::IsLittleEndian()
{
    int i = 1;
    return *reinterpret_cast<Byte*>(&i) == 1;
}

void CByteOrderHelper::Init()
{
    if (sIsInited == FALSE) {
        sIsInited = TRUE;

        sIsLittleEndian = IsLittleEndian();
        sBigEndian      = ByteOrder_BIG_ENDIAN;
        sLittleEndian   = ByteOrder_LITTLE_ENDIAN;
        sNativeOrder    = sIsLittleEndian ? sLittleEndian : sBigEndian;
    }
}

ECode CByteOrderHelper::GetNativeOrder(
        /* [out] */ByteOrder* nativeOrder)
{
    VALIDATE_NOT_NULL(nativeOrder);

    Init();

    *nativeOrder = sNativeOrder;

    return NOERROR;
}

ECode CByteOrderHelper::IsNeedsSwap(
    /* [in] */ ByteOrder order,
    /* [out] */ Boolean* isNeedsSwap)
{
    VALIDATE_NOT_NULL(isNeedsSwap)

    Init();

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
