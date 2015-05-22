
#include "cmdef.h"
#include "CBigIntegerHelper.h"
#include "CBigInteger.h"

namespace Elastos {
namespace Math {

CAR_SINGLETON_IMPL(CBigIntegerHelper, Singleton, IBigIntegerHelper)

ECode CBigIntegerHelper::ValueOf(
    /* [in] */ Int64 value,
    /* [out] */ IBigInteger** result)
{
    VALIDATE_NOT_NULL(result);
    return CBigInteger::ValueOf(value, result);
}

ECode CBigIntegerHelper::GetMINUSOne(
    /* [out] */ IBigInteger** result)
{
    VALIDATE_NOT_NULL(result);
    *result = CBigInteger::MINUS_ONE;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CBigIntegerHelper::GetZERO(
    /* [out] */ IBigInteger** result)
{
    VALIDATE_NOT_NULL(result);
    *result = CBigInteger::ZERO;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CBigIntegerHelper::GetONE(
    /* [out] */ IBigInteger** result)
{
    VALIDATE_NOT_NULL(result);
    *result = CBigInteger::ONE;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CBigIntegerHelper::GetTEN(
    /* [out] */ IBigInteger** result)
{
    VALIDATE_NOT_NULL(result);
    *result = CBigInteger::TEN;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

} // namespace Math
} // namespace Elastos

