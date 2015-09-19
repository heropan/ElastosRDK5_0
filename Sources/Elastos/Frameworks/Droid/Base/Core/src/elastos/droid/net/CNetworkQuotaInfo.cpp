
#include "ext/frameworkext.h"
#include "net/CNetworkQuotaInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CNetworkQuotaInfo::constructor()
{
    return NOERROR;
}

/** {@hide} */
ECode CNetworkQuotaInfo::constructor(
    /* [in] */ Int64 estimatedBytes,
    /* [in] */ Int64 softLimitBytes,
    /* [in] */ Int64 hardLimitBytes)
{
    mEstimatedBytes = estimatedBytes;
    mSoftLimitBytes = softLimitBytes;
    mHardLimitBytes = hardLimitBytes;
    return NOERROR;
}

ECode CNetworkQuotaInfo::GetEstimatedBytes(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mEstimatedBytes;
    return NOERROR;
}

ECode CNetworkQuotaInfo::GetSoftLimitBytes(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mSoftLimitBytes;
    return NOERROR;
}

ECode CNetworkQuotaInfo::GetHardLimitBytes(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHardLimitBytes;
    return NOERROR;
}

//@Override
ECode CNetworkQuotaInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);
    dest->WriteInt64(mEstimatedBytes);
    dest->WriteInt64(mSoftLimitBytes);
    dest->WriteInt64(mHardLimitBytes);
    return NOERROR;
}

ECode CNetworkQuotaInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
     VALIDATE_NOT_NULL(source);
     source->ReadInt64(&mEstimatedBytes);
     source->ReadInt64(&mSoftLimitBytes);
     source->ReadInt64(&mHardLimitBytes);
     return NOERROR;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
