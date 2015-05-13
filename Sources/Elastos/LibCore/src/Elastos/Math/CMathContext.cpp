
#include "cmdef.h"
#include "CMathContext.h"

namespace Elastos {
namespace Math {

static AutoPtr<IMathContext> CreateMathContext(Int32 p, RoundingMode mode)
{
    AutoPtr<CMathContext> obj;
    CMathContext::NewByFriend(p, mode, (CMathContext**)&obj);
    return (IMathContext*)obj.Get();
}

const AutoPtr<IMathContext> CMathContext::DECIMAL128 = CreateMathContext(34, RoundingMode_HALF_EVEN);
const AutoPtr<IMathContext> CMathContext::DECIMAL32 = CreateMathContext(7, RoundingMode_HALF_EVEN);
const AutoPtr<IMathContext> CMathContext::DECIMAL64 = CreateMathContext(16, RoundingMode_HALF_EVEN);
const AutoPtr<IMathContext> CMathContext::UNLIMITED = CreateMathContext(0, RoundingMode_HALF_UP);

ECode CMathContext::constructor(
    /* [in] */ Int32 precision)
{
    if (precision < 0) {
        // throw new IllegalArgumentException("Negative precision: " + precision);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mPrecision = precision;
    mRoundingMode  = RoundingMode_HALF_UP;
    return NOERROR;
}

ECode CMathContext::constructor(
    /* [in] */ Int32 precision,
    /* [in] */ RoundingMode roundingMode)
{
    if (precision < 0) {
        // throw new IllegalArgumentException("Negative precision: " + precision);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mPrecision = precision;
    mRoundingMode  = roundingMode;
    return NOERROR;
}

ECode CMathContext::GetPrecision(
    /* [out] */ Int32* precision)
{
    VALIDATE_NOT_NULL(precision);
    *precision = mPrecision;
    return NOERROR;
}

ECode CMathContext::GetRoundingMode(
    /* [out] */ RoundingMode* roundingMode)
{
    VALIDATE_NOT_NULL(roundingMode);
    *roundingMode = mRoundingMode;
    return NOERROR;
}

} // namespace Math
} // namespace Elastos
