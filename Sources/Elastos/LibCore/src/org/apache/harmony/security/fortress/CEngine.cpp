
#include "CEngine.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Fortress {

CEngine::ServiceCacheEntry::ServiceCacheEntry(
    /* [in] */ String algorithm,
    /* [in] */ Int32 cacheVersion,
    /* [in] */ IService* service)
    : mCacheVersion(0)
{
    mAlgorithm = algorithm;
    mCacheVersion = cacheVersion;
    mService = service;
}

ECode CEngine::GetInstance(
    /* [in] */ const String& algorithm,
    /* [in] */ IInterface* param,
    /* [out] */ ISpiAndProvider** instance)
{
    VALIDATE_NOT_NULL(instance)
    if (algorithm.IsNull()) {
        //throw new NoSuchAlgorithmException("Null algorithm name");
        return E_NO_SUCH_ALGORITHM_EXCEPTION;
    }
    AutoPtr<IServicesHelper> helper;
    CServicesHelper::AcquireSingleton((IServicesHelper**)&helper);
    Int32 newCacheVersion;
    helper->GetCacheVersion(&newCacheVersion);
    AutoPtr<IService> service;
    AutoPtr<ServiceCacheEntry> cacheEntry = mServiceCache;
    if (cacheEntry != NULL
            && cacheEntry->mAlgorithm.EqualsIgnoreCase(algorithm)
            && newCacheVersion == cacheEntry.mCacheVersion) {
        service = cacheEntry.mService;
    } else {
        Boolean isEmpty;
        if (helper->IsEmpty(&isEmpty), isEmpty) {
            //throw notFound(serviceName, algorithm);
            return E_NO_SUCH_ALGORITHM_EXCEPTION;
        }
        String name = mServiceName + "." + algorithm.ToUpperCase();
        helper->GetService(name, (IService**)&service);
        if (service == NULL) {
            //throw notFound(serviceName, algorithm);
            return E_NO_SUCH_ALGORITHM_EXCEPTION;
        }
        mServiceCache = new ServiceCacheEntry(algorithm, newCacheVersion, service);
    }
    AutoPtr<IInterface> spi;
    service->NewInstance(param, (IInterface**)&spi);
    AutoPtr<IProvider> pro;
    service->GetProvider((IProvider**)&pro);
    *instance = new SpiAndProvider(spi, pro);
    INTERFACE_ADDREF(*instance)
    return NOERROR;
}

ECode CEngine::GetInstanceEx(
    /* [in] */ const String& algorithm,
    /* [in] */ IProvider* provider,
    /* [in] */ IInterface* param,
    /* [out] */ IInterface** instance)
{
    if (algorithm.IsNull()) {
        //throw new NoSuchAlgorithmException("algorithm == null");
        return E_NO_SUCH_ALGORITHM_EXCEPTION;
    }
    AutoPtr<IService> service;
    provider->GetService(mServiceName, algorithm, (IService**)&service);
    if (service == NULL) {
        //throw notFound(serviceName, algorithm);
        return E_NO_SUCH_ALGORITHM_EXCEPTION;
    }

    return service->NewInstance(param, instance);
}

ECode CEngine::constructor(
    /* [in] */ const String& service)
{
    mServiceName = service;
    return NOERROR;
}

} // namespace Fortress
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

