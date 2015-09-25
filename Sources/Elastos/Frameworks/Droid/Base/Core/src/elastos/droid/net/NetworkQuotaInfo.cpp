
#include "elastos/droid/net/NetworkQuotaInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(NetworkQuotaInfo, Object, IParcelable, INetworkQuotaInfo)

ECode NetworkQuotaInfo::constructor(
    /* [in] */ Int64 estimatedBytes,
    /* [in] */ Int64 softLimitBytes,
    /* [in] */ Int64 hardLimitBytes)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    mEstimatedBytes = estimatedBytes;
    mSoftLimitBytes = softLimitBytes;
    mHardLimitBytes = hardLimitBytes;
    return NOERROR;
#endif
}

ECode NetworkQuotaInfo::constructor(
    /* [in] */ IParcel* in)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mEstimatedBytes = in.readLong();
        mSoftLimitBytes = in.readLong();
        mHardLimitBytes = in.readLong();

#endif
}

ECode NetworkQuotaInfo::GetEstimatedBytes(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mEstimatedBytes;
    return NOERROR;
#endif
}

ECode NetworkQuotaInfo::GetSoftLimitBytes(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(result);
    *result = mSoftLimitBytes;
    return NOERROR;
#endif
}

ECode NetworkQuotaInfo::GetHardLimitBytes(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(result);
    *result = mHardLimitBytes;
    return NOERROR;
#endif
}

ECode NetworkQuotaInfo::DescribeContents(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return 0;

#endif
}

ECode NetworkQuotaInfo::WriteToParcel(
    /* [in] */ IParcel* out,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        out.writeLong(mEstimatedBytes);
        out.writeLong(mSoftLimitBytes);
        out.writeLong(mHardLimitBytes);

#endif
}

ECode NetworkQuotaInfo::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
     VALIDATE_NOT_NULL(source);
     source->ReadInt64(&mEstimatedBytes);
     source->ReadInt64(&mSoftLimitBytes);
     source->ReadInt64(&mHardLimitBytes);
     return NOERROR;
#endif
}

ECode NetworkQuotaInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(dest);
    dest->WriteInt64(mEstimatedBytes);
    dest->WriteInt64(mSoftLimitBytes);
    dest->WriteInt64(mHardLimitBytes);
    return NOERROR;
#endif
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
