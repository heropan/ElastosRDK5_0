
#include "elastos/droid/hardware/camera2/marshal/impl/MarshalQueryableRggbChannelVector.h"
#include "elastos/droid/hardware/camera2/params/CRggbChannelVector.h"

using Elastos::Droid::Hardware::Camera2::Impl::ICameraMetadataNative;
using Elastos::Droid::Hardware::Camera2::Params::IRggbChannelVector;
using Elastos::Droid::Hardware::Camera2::Params::CRggbChannelVector;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Camera2 {
namespace Marshal {
namespace Impl {

MarshalQueryableRggbChannelVector::MarshalerRggbChannelVector::MarshalerRggbChannelVector(
    /* [in] */ ITypeReference* typeReference,
    /* [in] */ Int32 nativeType,
    /* [in] */ MarshalQueryableRggbChannelVector* host)
{
    Marshaler::constructor(host, typeReference, nativeType);
}

ECode MarshalQueryableRggbChannelVector::MarshalerRggbChannelVector::Marshal(
    /* [in] */ IInterface* value,
    /* [in] */ IByteBuffer* buffer)
{
    AutoPtr<IRggbChannelVector> _vector = IRggbChannelVector::Probe(value);

    for (Int32 i = 0; i < IRggbChannelVector::COUNT; ++i) {
        Float component;
        _vector->GetComponent(i, &component);
        buffer->PutFloat(component);
    }
    return NOERROR;
}

ECode MarshalQueryableRggbChannelVector::MarshalerRggbChannelVector::Unmarshal(
    /* [in] */ IByteBuffer* buffer,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    *outface = NULL;

    Float red;
    buffer->GetFloat(&red);
    Float gEven;
    buffer->GetFloat(&gEven);
    Float gOdd;
    buffer->GetFloat(&gOdd);
    Float blue;
    buffer->GetFloat(&blue);

    AutoPtr<IRggbChannelVector> _vector;
    CRggbChannelVector::New(red, gEven, gOdd, blue, (IRggbChannelVector**)&_vector);
    *outface = TO_IINTERFACE(_vector);
    REFCOUNT_ADD(*outface);
    return NOERROR;
}

ECode MarshalQueryableRggbChannelVector::MarshalerRggbChannelVector::GetNativeSize(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = SIZE;
    return NOERROR;
}

const Int32 MarshalQueryableRggbChannelVector::SIZE = IMarshalHelpers::SIZEOF_FLOAT * IRggbChannelVector::COUNT;

CAR_INTERFACE_IMPL_2(MarshalQueryableRggbChannelVector, Object,
        IMarshalQueryableRggbChannelVector, IMarshalQueryable)

ECode MarshalQueryableRggbChannelVector::CreateMarshaler(
    /* [in] */ ITypeReference* managedType,
    /* [in] */ Int32 nativeType,
    /* [out] */ IMarshaler** outmar)
{
    VALIDATE_NOT_NULL(outmar);

    AutoPtr<IMarshaler> _outmar = new MarshalerRggbChannelVector(managedType, nativeType, this);
    *outmar = _outmar;
    REFCOUNT_ADD(*outmar);
    return NOERROR;
}

ECode MarshalQueryableRggbChannelVector::IsTypeMappingSupported(
    /* [in] */ ITypeReference* managedType,
    /* [in] */ Int32 nativeType,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = FALSE;

    assert(0);
    //return nativeType == ICameraMetadataNative::TYPE_FLOAT && (RggbChannelVector.class.equals(managedType.getType()));
    return NOERROR;
}

} // namespace Impl
} // namespace Marshal
} // namespace Camera2
} // namespace Hardware
} // namespace Droid
} // namespace Elastos
