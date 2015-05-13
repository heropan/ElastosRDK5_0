
#ifndef __CMTPSTORAGEINFO_H__
#define __CMTPSTORAGEINFO_H__
#include "ext/frameworkext.h"
#include "_CMtpStorageInfo.h"

namespace Elastos {
namespace Droid {
namespace Mtp {

CarClass(CMtpStorageInfo)
{
public:
    CMtpStorageInfo();

    CARAPI GetStorageId(
        /* [out] */ Int32* result);

    CARAPI GetMaxCapacity(
        /* [out] */ Int64* result);

    CARAPI GetFreeSpace(
        /* [out] */ Int64* result);

    CARAPI GetDescription(
        /* [out] */ String* result);

    CARAPI GetVolumeIdentifier(
        /* [out] */ String* result);

    CARAPI SetStorageId(
        /* [in] */ Int32 result);

    CARAPI SetMaxCapacity(
        /* [in] */ Int64 result);

    CARAPI SetFreeSpace(
        /* [in] */ Int64 result);

    CARAPI SetDescription(
        /* [in] */ const String& result);

    CARAPI SetVolumeIdentifier(
        /* [in] */ const String& result);

private:
    Int32 mStorageId;
    Int64 mMaxCapacity;
    Int64 mFreeSpace;
    String mDescription;
    String mVolumeIdentifier;
};

} // namespace Mtp
} // namepsace Droid
} // namespace Elastos

#endif // __CMTPSTORAGEINFO_H__
