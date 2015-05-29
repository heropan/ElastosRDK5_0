
#include "CChar8.h"
#include "IntegralToString.h"

using Elastos::Core::IntegralToString;
using Elastos::IO::EIID_ISerializable;

namespace Elastos {
namespace Core {
CAR_INTERFACE_IMPL_3(CChar8, Object, IChar8, INumber, ISerializable)

CAR_OBJECT_IMPL(CChar8)

ECode CChar8::constructor(
    /* [in] */ Char8 value)
{
    mValue = value;
    return NOERROR;
}

ECode CChar8::GetValue(
    /* [out] */ Char8* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mValue;
    return NOERROR;
}

ECode CChar8::ByteValue(
    /* [out] */ Byte* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (Byte)mValue;

    return NOERROR;
}

ECode CChar8::Int16Value(
    /* [out] */ Int16* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (Int16)mValue;

    return NOERROR;
}

ECode CChar8::Int32Value(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (Int32)mValue;

    return NOERROR;
}

ECode CChar8::Int64Value(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (Int64)mValue;

    return NOERROR;
}

ECode CChar8::FloatValue(
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (Float)mValue;

    return NOERROR;
}

ECode CChar8::DoubleValue(
    /* [out] */ Double* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (Double)mValue;

    return NOERROR;
}

ECode CChar8::CompareTo(
    /* [in] */ IInterface* _other,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = -1;
    VALIDATE_NOT_NULL(_other);

    IChar8* other = (IChar8*)_other->Probe(EIID_IChar8);
    if (other == NULL) {
        return NOERROR;
    }

    Char8 otherValue;
    other->GetValue(&otherValue);
    *result = (mValue > otherValue ? 1 : (mValue < otherValue ? -1 : 0));
    return NOERROR;
}

ECode CChar8::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    if (IChar8::Probe(other) == NULL) return NOERROR;

    Char8 otherValue;
    IChar8::Probe(other)->GetValue(&otherValue);
    *result = (mValue == otherValue);
    return NOERROR;
}

ECode CChar8::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = mValue;
    return NOERROR;
}

ECode CChar8::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    //*result = IntegralToString::ToString((Int32)mValue);

    return NOERROR;
}

} // namespace Core
} // namespace Elastos
