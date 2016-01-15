
#include "elastos/droid/hardware/camera2/marshal/Marshaler.h"
#include "elastos/droid/hardware/camera2/marshal/MarshalHelpers.h"
#include "elastos/droid/internal/utility/Preconditions.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Hardware::Camera2::Marshal::MarshalHelpers;
using Elastos::Droid::Internal::Utility::Preconditions;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Camera2 {
namespace Marshal {

CAR_INTERFACE_IMPL(Marshaler, Object, IMarshaler)

Marshaler::Marshaler()
    : mNativeType(0)
{
}

ECode Marshaler::constructor()
{
    return NOERROR;
}

ECode Marshaler::constructor(
    /* [in] */ IMarshalQueryable* query,
    /* [in] */ ITypeReference* typeReference,
    /* [in] */ Int32 nativeType)
{
    FAIL_RETURN(Preconditions::CheckNotNull(typeReference, String("typeReference must not be null")))
    FAIL_RETURN(MarshalHelpers::CheckNativeType(nativeType, &mNativeType))
    mTypeReference = typeReference;

    Boolean result;
    query->IsTypeMappingSupported(typeReference, nativeType, &result);
    if (!result) {
        // throw new UnsupportedOperationException(
        //         "Unsupported type marshaling for managed type "
        //                 + typeReference + " and native type "
        //                 + MarshalHelpers.toStringNativeType(nativeType));
        String str;
        IObject::Probe(typeReference)->ToString(&str);
        String type;
        MarshalHelpers::ToStringNativeType(nativeType, &type);
        Slogger::E("Marshaler", "Unsupported type marshaling for managed "
                "type %s and native type %s", str.string(), type.string());
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }
    return NOERROR;
}

ECode Marshaler::CalculateMarshalSize(
    /* [in] */ IInterface* value,
    /* [out] */ Int32* outvalue)
{
    VALIDATE_NOT_NULL(outvalue);
    *outvalue = 0;

    Int32 nativeSize;
    GetNativeSize(&nativeSize);

    if (nativeSize == IMarshaler::NATIVE_SIZE_DYNAMIC) {
        //throw new AssertionError("Override this function for dynamically-sized objects");
        Slogger::E("Marshaler", "Override this function for dynamically-sized objects");
        return E_ASSERTION_ERROR;
    }

    *outvalue = nativeSize;
    return NOERROR;
}

ECode Marshaler::GetTypeReference(
    /* [out] */ ITypeReference** outtr)
{
    VALIDATE_NOT_NULL(outtr);

    *outtr = mTypeReference;
    REFCOUNT_ADD(*outtr);
    return NOERROR;
}

ECode Marshaler::GetNativeType(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mNativeType;
    return NOERROR;
}

} // namespace Marshal
} // namespace Camera2
} // namespace Hardware
} // namespace Droid
} // namespace Elastos
