
#include "coredef.h"
#include "CChar16.h"
#include "IntegralToString.h"

using Elastos::Core::IntegralToString;
using Elastos::IO::EIID_ISerializable;

namespace Elastos {
namespace Core {

CAR_INTERFACE_IMPL_3(CChar16, Object, IChar16, INumber, ISerializable)

CAR_OBJECT_IMPL(CChar16)

ECode CChar16::constructor(
    /* [in] */ Char16 value)
{
    mValue = value;
    return NOERROR;
}

ECode CChar16::GetValue(
    /* [out] */ Char16* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mValue;
    return NOERROR;
}

ECode CChar16::ByteValue(
    /* [out] */ Byte* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (Byte)mValue;

    return NOERROR;
}

ECode CChar16::Int16Value(
    /* [out] */ Int16* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (Int16)mValue;

    return NOERROR;
}

ECode CChar16::Int32Value(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (Int32)mValue;

    return NOERROR;
}

ECode CChar16::Int64Value(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (Int64)mValue;

    return NOERROR;
}

ECode CChar16::FloatValue(
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (Float)mValue;

    return NOERROR;
}

ECode CChar16::DoubleValue(
    /* [out] */ Double* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (Double)mValue;

    return NOERROR;
}

ECode CChar16::CompareTo(
    /* [in] */ IInterface* _other,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = -1;
    VALIDATE_NOT_NULL(_other);

    IChar16* other = (IChar16*)_other->Probe(EIID_IChar16);
    if (other == NULL) {
        return NOERROR;
    }

    Char16 otherValue;
    other->GetValue(&otherValue);
    *result = (mValue > otherValue ? 1 : (mValue < otherValue ? -1 : 0));
    return NOERROR;
}

ECode CChar16::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    if (IChar16::Probe(other) == NULL) return NOERROR;

    Char16 otherValue;
    IChar16::Probe(other)->GetValue(&otherValue);
    *result = (mValue == otherValue);
    return NOERROR;
}

ECode CChar16::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = mValue;
    return NOERROR;
}

ECode CChar16::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    //*result = IntegralToString::Int32ToString((Int32)mValue);

    return NOERROR;
}

} // namespace Core
} // namespace Elastos
