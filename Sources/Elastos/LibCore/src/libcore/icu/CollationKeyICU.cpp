
#include "CollationKeyICU.h"
#include <Math.h>

using Elastos::Text::EIID_ICollationKey;

extern "C" const InterfaceID EIID_CollationKeyICU =
    { 0x278e167c, 0xaae5, 0x4e71, { 0x9f, 0x92, 0x21, 0x83, 0x7a, 0x76, 0x0, 0x86 } };

namespace Libcore {
namespace ICU {

#if 0 // for compiling
CAR_INTERFACE_IMPL(CollationKeyICU, CollationKey, ICollationKeyICU)
#else
CAR_INTERFACE_IMPL(CollationKeyICU, Object, ICollationKeyICU)
#endif // #if 0

ECode CollationKeyICU::constructor(
    /* [in] */ const String& source,
    /* [in] */ ArrayOf<Byte>* bytes)
{
#if 0 // for compiling
    CollationKey::constructor(source);
#endif // #if 0
    mBytes = bytes->Clone();
    mHashCode = 0;
    return NOERROR;
}

ECode CollationKeyICU::CompareTo(
    /* [in] */ IInterface* _other,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(_other);
    VALIDATE_NOT_NULL(result);

#if 0 // for compiling
    AutoPtr<ICollationKey> other = ICollationKey::Probe(_other);
    if (other == NULL) return E_ILLEGAL_ARGUMENT_EXCEPTION;

    // Get the bytes from the other collation key.
    AutoPtr<ArrayOf<Byte> > rhsBytes;
    if (other->Probe(EIID_CollationKeyICU) != NULL) {
        CollationKeyICU* c = reinterpret_cast<CollationKeyICU*>(other->Probe(EIID_CollationKeyICU));
        rhsBytes = c->mBytes;
    }
    else {
        other->ToByteArray((ArrayOf<Byte>**)&rhsBytes);
    }

    if (mBytes == NULL || mBytes->GetLength() == 0) {
        if (rhsBytes == NULL || rhsBytes->GetLength() == 0) {
            *result = 0;
            return NOERROR;
        }
        *result = -1;
        return NOERROR;
    }
    else {
        if (rhsBytes == NULL || rhsBytes->GetLength() == 0) {
            *result = 1;
            return NOERROR;
        }
    }

    Int32 count = Elastos::Core::Math::Min(mBytes->GetLength(), rhsBytes->GetLength());
    for (Int32 i = 0; i < count; ++i) {
        Int32 s = (*mBytes)[i] & 0xFF;
        Int32 t = (*rhsBytes)[i] & 0xFF;
        if (s < t) {
            *result = -1;
            return NOERROR;
        }
        if (s > t) {
            *result = 1;
            return NOERROR;
        }
    }
    if (mBytes->GetLength() < rhsBytes->GetLength()) {
        *result = -1;
        return NOERROR;
    }
    if (mBytes->GetLength() > rhsBytes->GetLength()) {
        *result = 1;
        return NOERROR;
    }
#endif // #if 0
    *result = 0;
    return NOERROR;
}

ECode CollationKeyICU::Equals(
    /* [in] */ IInterface * object,
    /* [out] */ Boolean * value)
{
    if (object == this->Probe(EIID_IInterface)) {
        *value = TRUE;
        return NOERROR;
    }
#if 0 // for compiling
    AutoPtr<ICollationKey> res = (ICollationKey *) object->Probe(EIID_ICollationKey);
    if (!res) {
        *value = FALSE;
        return NOERROR;
    }
    Int32 out(0);
    CompareTo(res,&out);
    *value = out == 0;
#endif // #if 0
    return NOERROR;
}

ECode CollationKeyICU::GetHashCode(
    /* [out] */ Int32 * value)
{
    if (mHashCode == 0) {
        if (mBytes != NULL && mBytes->GetLength() != 0) {
            Int32 len = mBytes->GetLength();
            Int32 inc = ((len - 32) / 32) + 1;
            for (Int32 i = 0; i < len;) {
                mHashCode = (mHashCode * 37) + (*mBytes)[i];
                i += inc;
            }
        }
        if (mHashCode == 0) {
            mHashCode = 1;
        }
    }
    *value = mHashCode;
    return NOERROR;
}

ECode CollationKeyICU::ToByteArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    VALIDATE_NOT_NULL(array);
    if (mBytes != NULL && mBytes->GetLength() == 0) {
        *array = NULL;
        return NOERROR;
    }
    *array = mBytes->Clone();
    REFCOUNT_ADD(*array);
    return NOERROR;
}

} // namespace ICU
} // namespace Libcore
