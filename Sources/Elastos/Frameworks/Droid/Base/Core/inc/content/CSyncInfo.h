
#ifndef __ELASTOS_DROID_CONTENT_CSYNCINFO_H__
#define __ELASTOS_DROID_CONTENT_CSYNCINFO_H__

#include "_Elastos_Droid_Content_CSyncInfo.h"

using Elastos::Droid::Accounts::IAccount;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CSyncInfo)
{
public:
    CSyncInfo();

    ~CSyncInfo();

    /** @hide */
    CARAPI GetAuthorityId(
        /* [out] */ Int32* authorityId);

    /**
     * The {@link Account} that is currently being synced.
     */
    CARAPI GetAccount(
        /* [out] */ IAccount** account);

    /**
     * The authority of the provider that is currently being synced.
     */
    CARAPI GetAuthority(
        /* [out] */ String* authority);

    /**
     * The start time of the current sync operation in milliseconds since boot.
     * This is represented in elapsed real time.
     * See {@link android.os.SystemClock#elapsedRealtime()}.
     */
    CARAPI GetStartTime(
        /* [out] */ Int64* startTime);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    /** @hide */
    CARAPI constructor(
        /* [in] */ Int32 authorityId,
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [in] */ Int64 startTime);

private:
    /** @hide */
    Int32 mAuthorityId;

    /**
     * The {@link Account} that is currently being synced.
     */
    AutoPtr<IAccount> mAccount;

    /**
     * The authority of the provider that is currently being synced.
     */
    String mAuthority;

    /**
     * The start time of the current sync operation in milliseconds since boot.
     * This is represented in elapsed real time.
     * See {@link android.os.SystemClock#elapsedRealtime()}.
     */
    Int64 mStartTime;

};

}
}
}

#endif // __ELASTOS_DROID_CONTENT_CSYNCINFO_H__
