
#include "cmdef.h"
#include "CBigDecimalHelper.h"
#include "CBigDecimal.h"

namespace Elastos {
namespace Math {

ECode CBigDecimalHelper::ValueOf(
    /* [in] */ Int64 unscaledVal,
    /* [in] */ Int32 scale,
    /* [out] */ IBigDecimal** result)
{
    VALIDATE_NOT_NULL(result);
    return CBigDecimal::ValueOf(unscaledVal, scale, result);
}

ECode CBigDecimalHelper::ValueOfEx(
    /* [in] */ Int64 unscaledVal,
    /* [out] */ IBigDecimal** result)
{
    VALIDATE_NOT_NULL(result);
    return CBigDecimal::ValueOf(unscaledVal, result);
}

ECode CBigDecimalHelper::ValueOfEx2(
    /* [in] */ Double val,
    /* [out] */ IBigDecimal** result)
{
    VALIDATE_NOT_NULL(result);
    return CBigDecimal::ValueOf(val, result);
}

ECode CBigDecimalHelper::GetZERO(
    /* [out] */ IBigDecimal** result)
{
    VALIDATE_NOT_NULL(result);
    *result = CBigDecimal::ZERO;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CBigDecimalHelper::GetONE(
    /* [out] */ IBigDecimal** result)
{
    VALIDATE_NOT_NULL(result);
    *result = CBigDecimal::ONE;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CBigDecimalHelper::GetTEN(
    /* [out] */ IBigDecimal** result)
{
    VALIDATE_NOT_NULL(result);
    *result = CBigDecimal::TEN;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

} // namespace Math
} // namespace Elastos
