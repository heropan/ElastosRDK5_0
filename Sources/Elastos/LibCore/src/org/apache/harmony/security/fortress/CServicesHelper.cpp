
#include "CServicesHelper.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CString;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Fortress {

AutoPtr<IMap> CServicesHelper::sServices;// = new HashMap<String, Provider.Service>(600);

/**
* Save default SecureRandom service as well.
* Avoids similar provider/services iteration in SecureRandom constructor.
 */
AutoPtr<IProviderService> CServicesHelper::sCachedSecureRandomService;

/**
* Need refresh flag.
 */
Boolean CServicesHelper::sNseedRefresh;

/**
* The cacheVersion is changed on every update of service
* information. It is used by external callers to validate their
* own caches of Service information.
 */
//Int32 CServicesHelper::sCacheVersion = 1;

/**
* Registered providers.
 */
AutoPtr<IList> CServicesHelper::sProviders;// = new ArrayList<Provider>(20);

/**
* Hash for quick provider access by name.
 */
AutoPtr<IMap> CServicesHelper::sProvidersNames = InitStatic();

AutoPtr<IMap> CServicesHelper::InitStatic()
{
    {
        //Todo
        /*
        String providerClassName = null;
        int i = 1;
        ClassLoader cl = ClassLoader.getSystemClassLoader();

        while ((providerClassName = Security.getProperty("security.provider." + i++)) != null) {
            try {
                Class providerClass = Class.forName(providerClassName.trim(), true, cl);
                Provider p = (Provider) providerClass.newInstance();
                providers.add(p);
                providersNames.put(p.getName(), p);
                initServiceInfo(p);
            } catch (ClassNotFoundException ignored) {
            } catch (IllegalAccessException ignored) {
            } catch (InstantiationException ignored) {
            }
        }
        Engine.door.renumProviders();
        */
    }
    CHashMap::New(600, (IMap**)&sServices);
    CArrayList::New(20, (IList**)&sProviders);
    AutoPtr<IMap> ret;
    CHashMap::New(20, (IMap**)&ret);
    return ret;
}

ECode CServicesHelper::GetProviders(
    /* [out, callee] */ ArrayOf<IProvider *>** providers)
{
    VALIDATE_NOT_NULL(providers)
    AutoLock lock(mLock);
    //return providers.toArray(new Provider[providers.size()]);
    AutoPtr<ArrayOf<IInterface*> > tmp1, tmp2;
    Int32 size;
    sProviders->GetSize(&size);
    tmp1 = ArrayOf<IInterface *>::Alloc(size);
    AutoPtr<ArrayOf<IProvider *> > ret = ArrayOf<IProvider *>::Alloc(size);
    sProviders->ToArrayEx(tmp1, (ArrayOf<IInterface*>**)&tmp2);
    for (Int32 i = 0; i < ret->GetLength(); i++) {
        ret->Set(i, IProvider::Probe((*tmp2)[i]));
    }
    *providers = ret;
    REFCOUNT_ADD(*providers)
    return NOERROR;
}

ECode CServicesHelper::GetProvidersList(
    /* [out] */ IList** providersList)
{
    AutoLock lock(mLock);
    return CArrayList::New(sProviders.Get(), providersList);
}

ECode CServicesHelper::GetProvider(
    /* [in] */ const String& name,
    /* [out] */ IProvider** provider)
{
    AutoLock lock(mLock);
    VALIDATE_NOT_NULL(provider)
    if (name.IsNull()) {
        return NOERROR;
    }
    //return providersNames.get(name);
    AutoPtr<ICharSequence> cs;
    AutoPtr<IInterface> ret;
    CString::New(name, (ICharSequence**)&cs);
    sProvidersNames->Get(cs.Get(), (IInterface**)&ret);
    *provider = IProvider::Probe(ret);
    REFCOUNT_ADD(*provider)
    return NOERROR;
}

ECode CServicesHelper::InsertProviderAt(
    /* [in] */ IProvider* provider,
    /* [in] */ Int32 position,
    /* [out] */ Int32* pos)
{
    AutoLock lock(mLock);
    VALIDATE_NOT_NULL(pos)
    Int32 size;
    sProviders->GetSize(&size);
    if ((position < 1) || (position > size)) {
        position = size + 1;
    }
    sProviders->AddEx(position - 1, provider);
    String name;
    provider->GetName(&name);
    AutoPtr<ICharSequence> cs;
    CString::New(name, (ICharSequence**)&cs);
    AutoPtr<IInterface> ret;
    sProvidersNames->Put(cs.Get(), provider, (IInterface**)&ret);
    SetNeedRefresh();
    *pos = position;
    return NOERROR;
}

ECode CServicesHelper::RemoveProvider(
    /* [in] */ Int32 providerNumber)
{
    AutoLock lock(mLock);
    AutoPtr<IInterface> rm;
    sProviders->RemoveEx(providerNumber - 1, (IInterface**)&rm);
    AutoPtr<IProvider> p = IProvider::Probe(rm);
    String name;
    p->GetName(&name);
    AutoPtr<ICharSequence> cs;
    CString::New(name, (ICharSequence**)&cs);
    rm = NULL;
    sProvidersNames->Remove(cs.Get(), (IInterface**)&rm);
    return SetNeedRefresh();
}

ECode CServicesHelper::InitServiceInfo(
    /* [in] */ IProvider* p)
{
    AutoLock lock(mLock);
    AutoPtr<ISet> set;
    p->GetServices((ISet**)&set);
    AutoPtr<IIterator> it;
    set->GetIterator((IIterator**)&it);
    Boolean hasNext;
    it->HasNext(&hasNext);
    while(hasNext) {
        AutoPtr<IInterface> next;
        it->GetNext((IInterface**)&next);
        AutoPtr<IProviderService> service = IProviderService::Probe(next);
        String type;
        service->GetType(&type);
        if (sCachedSecureRandomService == NULL && type->Equals("SecureRandom")) {
            sCachedSecureRandomService = service;
        }
        String algo;
        service->GetAlgorithm(&algo);
        String key = type + "." + algo.ToUpperCase();
        AutoPtr<ICharSequence> cs;
        CString::New(key, (ICharSequence**)&cs);
        Boolean contains;
        if (sServices->ContainsKey(cs.Get(), &contains), !contains) {
            AutoPtr<IInterface> old;
            sServices->Put(cs.Get(), service.Get(), (IInterface**)&old);
        }
        AutoPtr<IList> lst;
        CEngine::mDoor->GetAliases(service, (IList**)&lst);
        AutoPtr<IIterator> it_alias;
        lst->GetIterator((IIterator**)&it_alias);
        Boolean hasNextAlias;
        it_alias->HasNext(&hasNextAlias);
        while(hasNextAlias) {
            String alias;
            AutoPtr<IInterface> nextAlias;
            it_alias->Next((IInterface**)&nextAlias);
            ICharSequence::Probe(nextAlias)->ToString(&alias);
            key = type + "." + alias.ToUpperCase();
            AutoPtr<ICharSequence> cs;
            CString::New(key, (ICharSequence**)&cs);
            Boolean contains;
            if (sServices->ContainsKey(cs.Get(), &contains), !contains) {
                AutoPtr<IInterface> old;
                sServices->Put(cs.Get(), service.Get(), (IInterface**)&old);
            }
            it_alias->HasNext(&hasNextAlias);
        }
        it->HasNext(&hasNext);
    }
    return NOERROR;
}

ECode CServicesHelper::IsEmpty(
    /* [out] */ Boolean* empty)
{
    AutoLock lock(mLock);
    return sServices->IsEmpty(empty);
}

ECode CServicesHelper::GetService(
    /* [in] */ const String& key,
    /* [out] */ IProviderService** service)
{
    AutoLock lock(mLock);
    VALIDATE_NOT_NULL(service)
    AutoPtr<ICharSequence> cs;
    CString::New(key, (ICharSequence**)&cs);
    AutoPtr<IInterface> ret;
    sServices->Get(cs.Get(), (IInterface**)&ret);
    *service = IProviderService::Probe(ret);
    REFCOUNT_ADD(*service)
    return NOERROR;
}

ECode CServicesHelper::GetSecureRandomService(
    /* [out] */ IProviderService** service)
{
    AutoLock lock(mLock);
    VALIDATE_NOT_NULL(service)
    GetCacheVersion(); // used for side effect of updating cache if needed
    *service = sCachedSecureRandomService;
    REFCOUNT_ADD(*service);
    return NOERROR;
}

ECode CServicesHelper::SetNeedRefresh()
{
    AutoLock lock(mLock);
    sNeedRefresh = TRUE;
    return NOERROR;
}

ECode CServicesHelper::GetCacheVersion(
    /* [out] */ Int32* cacheVersion)
{
    AutoLock lock(mLock);
    VALIDATE_NOT_NULL(cacheVersion)
    if (sNeedRefresh) {
        sCacheVersion++;
        //synchronized (services)
        {
            AutoLock lock(mLockForServices);
            sServices->Clear();
        }
        sCachedSecureRandomService = NULL;
        AutoPtr<IIterator> it;
        sProviders->GetIterator((IIterator**)&it);
        Boolean hasNext;
        it->HasNext(&hasNext);
        while (hasNext) {
            AutoPtr<IInterface> next;
            it->GetNext((IInterface**)&next);
            InitServiceInfo(IProvider::Probe(next));
            it->HasNext(&hasNext);
        }
        sNeedRefresh = FALSE;
    }
    *cacheVersion = sCacheVersion;
    return NOERROR;
}

} // namespace Fortress
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

