
#include "CSecurity.h"
#include "CProperties.h"
#include "CStringWrapper.h"
#include "CHashMap.h"
#include "CHashSet.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Security::IService;
using Elastos::Security::IProvider;
using Elastos::Utility::IEnumeration;
using Elastos::Utility::IMapEntry;
using Elastos::Utility::IIterator;
using Elastos::Utility::IList;
using Elastos::Utility::CProperties;
using Elastos::Utility::CHashMap;
using Elastos::Utility::CHashSet;

namespace Elastos {
namespace Security {

const AutoPtr<IProperties> CSecurity::mSecprops = CSecurity::InitStatics();

AutoPtr<IProperties> CSecurity::InitStatics()
{
    // static initialization
    // - load security properties files
    // - load statically registered providers
    // - if no provider description file found then load default providers
    /*
    Boolean loaded = FALSE;
    try {
        InputStream configStream = Security.class.getResourceAsStream("security.properties");
        InputStream input = new BufferedInputStream(configStream);
        secprops.load(input);
        loaded = true;
        configStream.close();
    } catch (Exception ex) {
        System.logE("Could not load 'security.properties'", ex);
    }
    if (!loaded) {
        registerDefaultProviders();
    }
    Engine.door = new SecurityDoor();
    */
    AutoPtr<CProperties> prop;
    CProperties::NewByFriend((CProperties**)&prop);
    return prop;
}

// Register default providers
ECode CSecurity::RegisterDefaultProviders()
{
    String temp;
    mSecprops->SetProperty(String("security.provider.1"), String("org.apache.harmony.xnet.provider.jsse."
        "OpenSSLProvider"), &temp);
    mSecprops->SetProperty(String("security.provider.2"), String("org.apache.harmony.security.provider"
        ".cert.DRLCertFactory"), &temp);
    mSecprops->SetProperty(String("security.provider.3"), String("org.bouncycastle.jce.provider"
        ".BouncyCastleProvider"), &temp);
    mSecprops->SetProperty(String("security.provider.4"), String("org.apache.harmony.security.provider."
        "crypto.CryptoProvider"), &temp);
    mSecprops->SetProperty(String("security.provider.5"), String("org.apache.harmony.xnet.provider"
        ".jsse.JSSEProvider"), &temp);
    return NOERROR;
}

/**
 * Returns value for the specified algorithm with the specified name.
 *
 * @param algName
 *            the name of the algorithm.
 * @param propName
 *            the name of the property.
 * @return value of the property.
 * @deprecated Use {@link AlgorithmParameters} and {@link KeyFactory}
 *             instead.
 */
ECode CSecurity::GetAlgorithmProperty(
    /* [in] */ const String& algName,
    /* [in] */ const String& propName,
    /* [out] */ String* algProp)
{
    VALIDATE_NOT_NULL(algProp)
    if (algName.IsNull() || propName.IsNull()) {
            *algProp = String();
            return NOERROR;
    }
    String prop("Alg.");
    prop += propName + "." + algName;
    AutoPtr<ArrayOf<IProvider*> > providers;
    GetProviders((ArrayOf<IProvider*>**)&providers);
    for (Int32 i = 0; i < providers->GetLength(); ++i) {
        AutoPtr<IInterface> inter;
        (*providers)[i]->PropertyNames((PInterface*)&inter);
        AutoPtr<IEnumeration> e = IEnumeration::Probe(inter);
        for (Boolean hasMore; (e->HasMoreElements(&hasMore), hasMore); ) {
            inter = NULL;
            String propertyName;
            AutoPtr<ICharSequence> cs;
            e->NextElement((PInterface*)&inter);
            ICharSequence::Probe(inter)->ToString(&propertyName);
            if (propertyName.EqualsIgnoreCase(prop)) {
                (*providers)[i]->GetProperty(propertyName, algProp);
                return NOERROR;
            }
        }
    }
    *algProp = String();
    return NOERROR;
}

/**
 * Insert the given {@code Provider} at the specified {@code position}. The
 * positions define the preference order in which providers are searched for
 * requested algorithms.
 *
 * @param provider
 *            the provider to insert.
 * @param position
 *            the position (starting from 1).
 * @return the actual position or {@code -1} if the given {@code provider}
 *         was already in the list. The actual position may be different
 *         from the desired position.
 */
ECode CSecurity::InsertProviderAt(
    /* [in] */ IProvider* provider,
    /* [in] */ Int32 position,
    /* [out] */ Int32* pos)
{
    // check that provider is not already
    // installed, else return -1; if (position <1) or (position > max
    // position) position = max position + 1; insert provider, shift up
    // one position for next providers; Note: The position is 1-based
    VALIDATE_NOT_NULL(pos)
    Autolock lock(mLock);
    String name;
    provider->GetName(&name);
    AutoPtr<IProvider> pro;
    GetProvider(name, (IProvider**)&pro);
    if (pro != NULL) {
        *pos = -1;
        return NOERROR;
    }
    Int32 result;// = Services.insertProviderAt(provider, position);
    RenumProviders();
    *pos = result;
    return NOERROR;
}

/**
 * Adds the given {@code provider} to the collection of providers at the
 * next available position.
 *
 * @param provider
 *            the provider to be added.
 * @return the actual position or {@code -1} if the given {@code provider}
 *         was already in the list.
 */
ECode CSecurity::AddProvider(
    /* [in] */ IProvider* provider,
    /* [out] */ Int32* pos)
{
    VALIDATE_NOT_NULL(pos)
    return InsertProviderAt(provider, 0, pos);
}

/**
 * Removes the {@code Provider} with the specified name form the collection
 * of providers. If the the {@code Provider} with the specified name is
 * removed, all provider at a greater position are shifted down one
 * position.
 *
 * <p>Returns silently if {@code name} is {@code null} or no provider with the
 * specified name is installed.
 *
 * @param name
 *            the name of the provider to remove.
 */
ECode CSecurity::RemoveProvider(
    /* [in] */ const String& name)
{
    // It is not clear from spec.:
    // 1. if name is null, should we checkSecurityAccess or not?
    //    throw SecurityException or not?
    // 2. as 1 but provider is not installed
    // 3. behavior if name is empty string?
    AutoPtr<IProvider> p;
    if (name.IsNull() || name.GetLength() == 0) {
        return NOERROR;
    }
    GetProvider(name, (IProvider**)&p);
    if (p == NULL) {
        return NOERROR;
    }
    /*
    Services.removeProvider(p.getProviderNumber());
    renumProviders();
    p.setProviderNumber(-1);
    */
    return NOERROR;
}

/**
 * Returns an array containing all installed providers. The providers are
 * ordered according their preference order.
 *
 * @return an array containing all installed providers.
 */
ECode CSecurity::GetProviders(
    /* [out, callee] */ ArrayOf<IProvider*>** providers)
{
    VALIDATE_NOT_NULL(providers)
    Autolock lock(mLock);
    //Todo ... after apache
    //return Services.getProviders();
    return NOERROR;
}

ECode CSecurity::GetProvider(
    /* [in] */ const String& name,
    /* [out] */ IProvider** provider)
{
    VALIDATE_NOT_NULL(provider)
    Autolock lock(mLock);
    //Todo ... after apache
    //return Services.getProvider(name);
    return NOERROR;
}

/**
 * Returns the array of providers which meet the user supplied string
 * filter. The specified filter must be supplied in one of two formats:
 * <nl>
 * <li> CRYPTO_SERVICE_NAME.ALGORITHM_OR_TYPE
 * <p>
 * (for example: "MessageDigest.SHA")
 * <li> CRYPTO_SERVICE_NAME.ALGORITHM_OR_TYPE
 * ATTR_NAME:ATTR_VALUE
 * <p>
 * (for example: "Signature.MD2withRSA KeySize:512")
 * </nl>
 *
 * @param filter
 *            case-insensitive filter.
 * @return the providers which meet the user supplied string filter {@code
 *         filter}. A {@code null} value signifies that none of the
 *         installed providers meets the filter specification.
 * @throws InvalidParameterException
 *             if an unusable filter is supplied.
 * @throws NullPointerException
 *             if {@code filter} is {@code null}.
 */
ECode CSecurity::GetProvidersEx(
    /* [in] */ const String& filter,
    /* [out, callee] */ ArrayOf<IProvider*>** providers)
{
    VALIDATE_NOT_NULL(providers)
    if (filter.IsNull()) {
        //Slogger::E(TAG, "filter == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }

    if (filter.GetLength() == 0) {
        return E_INVALID_PARAMETER_EXCEPTION;
    }
    AutoPtr<IMap> hm;
    CHashMap::New((IMap**)&hm);
    Int32 i = filter.IndexOf(':');
    if ((i == filter.GetLength() - 1) || (i == 0)) {
        return E_INVALID_PARAMETER_EXCEPTION;
    }
    AutoPtr<ICharSequence> key, value;
    AutoPtr<IInterface> rst;
    if (i < 1) {
        CStringWrapper::New(filter, (ICharSequence**)&key);
        CStringWrapper::New(String(""), (ICharSequence**)&value);
        hm->Put(key.Get(), value.Get(), (PInterface*)&rst);
    } else {
        AutoPtr<CStringWrapper> swKey, swValue;
        CStringWrapper::NewByFriend(filter, (CStringWrapper**)&swKey);
        swKey->SubSequence(0, i, (ICharSequence**)&key);
        CStringWrapper::NewByFriend(filter, (CStringWrapper**)&swValue);
        swValue->SubSequence(i + 1, filter.GetLength(), (ICharSequence**)&value);
        hm->Put(key.Get(), value.Get(), (PInterface*)&rst);
    }
    return GetProvidersEx2(hm, providers);
}

/**
 * Returns the array of providers which meet the user supplied set of
 * filters. The filter must be supplied in one of two formats:
 * <nl>
 * <li> CRYPTO_SERVICE_NAME.ALGORITHM_OR_TYPE
 * <p>
 * for example: "MessageDigest.SHA" The value associated with the key must
 * be an empty string. <li> CRYPTO_SERVICE_NAME.ALGORITHM_OR_TYPE
 * ATTR_NAME:ATTR_VALUE
 * <p>
 * for example: "Signature.MD2withRSA KeySize:512" where "KeySize:512" is
 * the value of the filter map entry.
 * </nl>
 *
 * @param filter
 *            case-insensitive filter.
 * @return the providers which meet the user supplied string filter {@code
 *         filter}. A {@code null} value signifies that none of the
 *         installed providers meets the filter specification.
 * @throws InvalidParameterException
 *             if an unusable filter is supplied.
 * @throws NullPointerException
 *             if {@code filter} is {@code null}.
 */
ECode CSecurity::GetProvidersEx2(
    /* [in] */ IMap* filter,
    /* [out, callee] */ ArrayOf<IProvider*>** providers)
{
    Autolock lock(mLock);
    VALIDATE_NOT_NULL(providers)
    if (filter == NULL) {
        //Slogger::E(TAG, "filter == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    Boolean empty;
    if ((filter->IsEmpty(&empty), empty)) {
        *providers = NULL;
        return NOERROR;
    }

    //uncomment codes related to apache...
    //java.util.List<Provider> result = Services.getProvidersList();
    AutoPtr<IList> result;
    AutoPtr<ISet> keys;
    filter->EntrySet((ISet**)&keys);
    Boolean isflag = FALSE;
    AutoPtr<IIterator> it;
    keys->GetIterator((IIterator**)&it);
    for (; (it->HasNext(&isflag), isflag);) {
        AutoPtr<IInterface> outface;
        it->GetNext((IInterface**)&outface);
        AutoPtr<IMapEntry> entry = IMapEntry::Probe(outface);
        AutoPtr<IInterface> itkey, itvalue;
        entry->GetKey((IInterface**)&itkey);
        String key, val;
        ICharSequence::Probe(itkey)->ToString(&key);
        entry->GetValue((IInterface**)&itvalue);
        ICharSequence::Probe(itvalue)->ToString(&val);
        String attribute;
        Int32 i = key.IndexOf(' ');
        Int32 j = key.IndexOf('.');
        if (j == -1) {
            return E_INVALID_PARAMETER_EXCEPTION;
        }
        if (i == -1) {
            // <crypto_service>.<algorithm_or_type>
            if (val.GetLength() != 0) {
                return E_INVALID_PARAMETER_EXCEPTION;
            }
        } else {
            // <crypto_service>.<algorithm_or_type> <attribute_name>
            if (val.GetLength() == 0) {
                return E_INVALID_PARAMETER_EXCEPTION;
            }
            attribute = key.Substring(i + 1);
            if (attribute.Trim().GetLength() == 0) {
                return E_INVALID_PARAMETER_EXCEPTION;
            }
            key = key.Substring(0, i);
        }
        String serv = key.Substring(0, j);
        String alg = key.Substring(j + 1);
        if (serv.GetLength() == 0 || alg.GetLength() == 0) {
            return E_INVALID_PARAMETER_EXCEPTION;
        }
        AutoPtr<IProvider> p;
        Int32 size;
        result->GetSize(&size);
        for (Int32 k = 0; k < size; ++k) {
            AutoPtr<IInterface> elm;
            FAIL_RETURN(result->Get(k, (PInterface*)&elm))
            p = IProvider::Probe(elm);
            /*if (!p.implementsAlg(serv, alg, attribute, val))*/ {
                Boolean ret;
                result->Remove(p.Get(), &ret);
                k--;
            }
        }
    }
    Int32 size;
    result->GetSize(&size);
    if (size > 0) {
        AutoPtr<ArrayOf<PInterface> > tmp;
        result->ToArray((ArrayOf<PInterface>**)&tmp);
        AutoPtr<ArrayOf<IProvider*> > props = ArrayOf<IProvider*>::Alloc(tmp->GetLength());
        for (Int32 i = 0; i < tmp->GetLength(); ++i) {
            props->Set(i, (*tmp)[i]);
        }
    }
    return NOERROR;
}

/**
 * Returns the value of the security property named by the argument.
 *
 * @param key
 *            the name of the requested security property.
 * @return the value of the security property.
 */
ECode CSecurity::GetProperty(
    /* [in] */ const String& key,
    /* [out] */ String* prop)
{
    VALIDATE_NOT_NULL(prop)
    if (key.IsNull()) {
        //Slogger::E(TAG, "key == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    String property;
    mSecprops->GetProperty(key, &property);
    if (!property.IsNull()) {
        property = property.Trim();
    }
    *prop = property;
    return NOERROR;
}

/**
 * Sets the value of the specified security property.
 */
ECode CSecurity::SetProperty(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    String temp;
    return mSecprops->SetProperty(key, value, &temp);
}

/**
 * Returns a {@code Set} of all registered algorithms for the specified
 * cryptographic service. {@code "Signature"}, {@code "Cipher"} and {@code
 * "KeyStore"} are examples for such kind of services.
 *
 * @param serviceName
 *            the case-insensitive name of the service.
 * @return a {@code Set} of all registered algorithms for the specified
 *         cryptographic service, or an empty {@code Set} if {@code
 *         serviceName} is {@code null} or if no registered provider
 *         provides the requested service.
 */
ECode CSecurity::GetAlgorithms(
    /* [in] */ const String& serviceName,
    /* [out] */ ISet** algs)
{
    VALIDATE_NOT_NULL(algs)
    AutoPtr<ISet> result;
    CHashSet::New((ISet**)&result);
    // compatibility with RI
    if (serviceName.IsNull()) {
        *algs = result;
        REFCOUNT_ADD(*algs)
        return NOERROR;
    }
    AutoPtr<ArrayOf<IProvider*> > providers;
    GetProviders((ArrayOf<IProvider*>**)&providers);
    for (Int32 i = 0; i < providers->GetLength(); ++i) {
        AutoPtr<ISet> set;
        (*providers)[i]->GetServices((ISet**)&set);
        AutoPtr<IIterator> it = IIterator::Probe(set.Get());
        Boolean next;
        while ((it->HasNext(&next), next)) {
            AutoPtr<IInterface> elm;
            it->GetNext((IInterface**)&elm);
            AutoPtr<IService> service = IService::Probe(elm);
            String type, algorithm;
            if ((service->GetType(&type), type).EqualsIgnoreCase(serviceName)) {
                service->GetAlgorithm(&algorithm);
                AutoPtr<ICharSequence> cs;
                CStringWrapper::New(algorithm, (ICharSequence**)&cs);
                Boolean ret;
                result->Add(cs.Get(), &ret);
            }
        }
    }
    *algs = result;
    REFCOUNT_ADD(*algs)
    return NOERROR;
}

/**
 *
 * Update sequence numbers of all providers.
 *
 */
ECode CSecurity::RenumProviders()
{
    //related to apache, todo later
/*
    Provider[] p = Services.getProviders();
    for (int i = 0; i < p.length; i++) {
        p[i].setProviderNumber(i + 1);
    }
*/
    return E_NOT_IMPLEMENTED;
}

}
}

