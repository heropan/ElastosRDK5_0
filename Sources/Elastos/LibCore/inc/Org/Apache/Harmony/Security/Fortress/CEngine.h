
#ifndef __CENGINE_H__
#define __CENGINE_H__

#include "_CEngine.h"

using Elastos::Security::IService;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Fortress {

CarClass(CEngine)
{
private:
    class ServiceCacheEntry
    : public ElLightRefBase {
    public:
        ServiceCacheEntry(
            /* [in] */ String algorithm,
            /* [in] */ Int32 cacheVersion,
            /* [in] */ IService* service);
    public:
        /** used to test for cache hit */
        String mAlgorithm;
        /** used to test for cache validity */
        Int32 mCacheVersion;
        /** cached result */
        AutoPtr<IService> mService;
    };
public:
    CARAPI GetInstance(
        /* [in] */ const String& algorithm,
        /* [in] */ IInterface * pParam,
        /* [out] */ Org::Apache::Harmony::Security::Fortress::ISpiAndProvider ** ppInstance);

    CARAPI GetInstanceEx(
        /* [in] */ const String& algorithm,
        /* [in] */ Elastos::Security::IProvider * pProvider,
        /* [in] */ IInterface * pParam,
        /* [out] */ IInterface ** ppInstance);

    CARAPI constructor(
        /* [in] */ const String& service);

public:
    /**
     * Access to package visible api in java.security
     */
    static AutoPtr<ISecurityAccess> mDoor;

private:
    /**
     * Service name such as Cipher or SSLContext
     */
    String mServiceName;

    /**
     * Previous result for getInstance(String, Object) optimization.
     * Only this non-Provider version of getInstance is optimized
     * since the the Provider version does not require an expensive
     * Services.getService call.
     */
    AutoPtr<ServiceCacheEntry> mServiceCache;
};

} // namespace Fortress
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CENGINE_H__
