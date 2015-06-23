#ifndef __ELASTOS_DROID_UTILITY_NTPTRUSTEDTIME_H__
#define __ELASTOS_DROID_UTILITY_NTPTRUSTEDTIME_H__

#include "ext/frameworkext.h"
#include "util/TrustedTime.h"

using Elastos::Core::Mutex;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::INtpTrustedTime;

namespace Elastos {
namespace Droid {
namespace Utility {

/**
 * Interface that provides trusted time information, possibly coming from an NTP
 * server. Implementations may cache answers until {@link #forceRefresh()}.
 *
 * @hide
 */
class NtpTrustedTime
    : public ElRefBase
    , public INtpTrustedTime
{
private:
    NtpTrustedTime(
        /* [in] */ const String& server,
        /* [in] */ Int64 timeout);

public:
    CAR_INTERFACE_DECL();

    static CARAPI GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ INtpTrustedTime** instance);

    CARAPI ForceRefresh(
        /* [out] */  Boolean* isRefreshed);

    CARAPI HasCache(
        /* [out] */  Boolean* hasCache);

    CARAPI GetCacheAge(
        /* [out] */  Int64* cacheAge);

    CARAPI GetCacheCertainty(
        /* [out] */  Int64* cacheCertainty);

    CARAPI GetCurrentTimeMillis(
        /* [out] */  Int64* currentTimeMillis);

    virtual CARAPI GetCachedNtpTime(
        /* [out] */ Int64* cacheNtpTime);

    virtual CARAPI GetCachedNtpTimeReference(
        /* [out] */ Int64* cachedNtpTimeReference);

private:
    static const String TAG;
    static const Boolean LOGD;

    static AutoPtr<NtpTrustedTime> sSingleton;

    const String mServer;
    const Int64 mTimeout;

    Boolean mHasCache;
    Int64 mCachedNtpTime;
    Int64 mCachedNtpElapsedRealtime;
    Int64 mCachedNtpCertainty;
    static Mutex mLock;
};

} // namespace Utility
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_UTILITY_NTPTRUSTEDTIME_H__
