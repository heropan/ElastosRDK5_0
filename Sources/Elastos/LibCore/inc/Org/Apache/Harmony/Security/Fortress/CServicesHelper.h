
#ifndef __CSERVICESHELPER_H__
#define __CSERVICESHELPER_H__

#include "_CServicesHelper.h"

using Elastos::Security::IProvider;
using Elastos::Utility::IList;
using Elastos::Security::IService;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Fortress {

CarClass(CServicesHelper)
{
public:
    static AutoPtr<IMap> InitStatic();

    CARAPI GetProviders(
        /* [out, callee] */ ArrayOf<IProvider*>** providers);

    CARAPI GetProvidersList(
        /* [out] */ IList** providersList);

    CARAPI GetProvider(
        /* [in] */ const String& name,
        /* [out] */ IProvider** provider);

    CARAPI InsertProviderAt(
        /* [in] */ IProvider* provider,
        /* [in] */ Int32 position,
        /* [out] */ Int32* pos);

    CARAPI RemoveProvider(
        /* [in] */ Int32 providerNumber);

    CARAPI InitServiceInfo(
        /* [in] */ IProvider* pro);

    CARAPI IsEmpty(
        /* [out] */ Boolean* empty);

    CARAPI GetService(
        /* [in] */ const String& key,
        /* [out] */ IService** service);

    CARAPI GetSecureRandomService(
        /* [out] */ IService** service);

    CARAPI SetNeedRefresh();

    CARAPI GetCacheVersion(
        /* [out] */ Int32* cacheVersion);

private:
    /**
     * The HashMap that contains information about preferred implementations for
     * all serviceName.algName in the registered providers.
     * Set the initial size to 600 so we don't grow to 1024 by default because
     * initialization adds a few entries more than the growth threshold.
     */
    static AutoPtr<IMap> sServices;// = new HashMap<String, Provider.Service>(600);

    /**
     * Save default SecureRandom service as well.
     * Avoids similar provider/services iteration in SecureRandom constructor.
     */
    static AutoPtr<IService> sCachedSecureRandomService;

    /**
     * Need refresh flag.
     */
    static Boolean sNeedRefresh;

    /**
     * The cacheVersion is changed on every update of service
     * information. It is used by external callers to validate their
     * own caches of Service information.
     */
    static Int32 sCacheVersion = 1;

    /**
     * Registered providers.
     */
    static AutoPtr<IList> sProviders;// = new ArrayList<Provider>(20);

    /**
     * Hash for quick provider access by name.
     */
    static AutoPtr<IMap> sProvidersNames;// = new HashMap<String, Provider>(20);

    Object mLock;
    Object mLockForServices;
};

} // namespace Fortress
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CSERVICESHELPER_H__
