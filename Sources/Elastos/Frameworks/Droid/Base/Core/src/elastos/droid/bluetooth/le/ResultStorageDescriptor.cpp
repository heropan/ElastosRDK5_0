
#include "elastos/droid/bluetooth/le/ResultStorageDescriptor.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {
namespace LE {

//=====================================================================
//                       ResultStorageDescriptor
//=====================================================================
CAR_INTERFACE_IMPL_2(ResultStorageDescriptor, Object, IResultStorageDescriptor, IParcelable);

ResultStorageDescriptor::ResultStorageDescriptor()
{
}

ResultStorageDescriptor::ResultStorageDescriptor(
    /* [in] */ Int32 type,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    // ==================before translated======================
    // mType = type;
    // mOffset = offset;
    // mLength = length;
}

ECode ResultStorageDescriptor::GetType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mType;
    assert(0);
    return NOERROR;
}

ECode ResultStorageDescriptor::GetOffset(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mOffset;
    assert(0);
    return NOERROR;
}

ECode ResultStorageDescriptor::GetLength(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mLength;
    assert(0);
    return NOERROR;
}

ECode ResultStorageDescriptor::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    VALIDATE_NOT_NULL(dest);
    // ==================before translated======================
    // dest.writeInt(mType);
    // dest.writeInt(mOffset);
    // dest.writeInt(mLength);
    assert(0);
    return NOERROR;
}

ResultStorageDescriptor::ResultStorageDescriptor(
    /* [in] */ IParcel* in)
{
    // ==================before translated======================
    // ReadFromParcel(in);
}

ECode ResultStorageDescriptor::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    // ==================before translated======================
    // mType = in.readInt();
    // mOffset = in.readInt();
    // mLength = in.readInt();
    assert(0);
    return NOERROR;
}

} // namespace LE
} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos


