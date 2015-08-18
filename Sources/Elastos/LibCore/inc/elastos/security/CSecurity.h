
#ifndef __ELASTOS_SECURITY_CSECURITY_H__
#define __ELASTOS_SECURITY_CSECURITY_H__

#include "_Elastos_Security_CSecurity.h"
#include "elastos/Object.h"



using Elastos::Utility::IMap;
using Elastos::Utility::ISet;
using Elastos::Utility::IProperties;

namespace Elastos {
namespace Security {

/**
 * {@code Security} is the central class in the Java Security API. It manages
 * the list of security {@code Provider} that have been installed into this
 * runtime environment.
 */
CarClass(CSecurity)
{
public:
    static CARAPI_(AutoPtr<IProperties>) InitStatics();

    CARAPI GetAlgorithmProperty(
        /* [in] */ const String& algName,
        /* [in] */ const String& propName,
        /* [out] */ String* algProp);

    CARAPI InsertProviderAt(
        /* [in] */ IProvider * provider,
        /* [in] */ Int32 position,
        /* [out] */ Int32* pos);

    CARAPI AddProvider(
        /* [in] */ IProvider* provider,
        /* [out] */ Int32* pos);

    CARAPI RemoveProvider(
        /* [in] */ const String& name);

    CARAPI GetProviders(
        /* [out, callee] */ ArrayOf<IProvider*>** providers);

    CARAPI GetProvider(
        /* [in] */ const String& name,
        /* [out] */ IProvider** provider);

    CARAPI GetProvidersEx(
        /* [in] */ const String& filter,
        /* [out, callee] */ ArrayOf<IProvider*>** providers);

    CARAPI GetProvidersEx2(
        /* [in] */ IMap* filter,
        /* [out, callee] */ ArrayOf<IProvider*>** providers);

    CARAPI GetProperty(
        /* [in] */ const String& key,
        /* [out] */ String* prop);

    CARAPI SetProperty(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    CARAPI GetAlgorithms(
        /* [in] */ const String& serviceName,
        /* [out] */ ISet ** algs);

private:
    //Todo... after apache.
    /*
    private static class SecurityDoor implements SecurityAccess {
    // Access to Security.renumProviders()
    public void renumProviders() {
        Security.renumProviders();
    }

    //  Access to Security.getAliases()
    public List<String> getAliases(Provider.Service s) {
        return s.getAliases();
    }

    // Access to Provider.getService()
    public Provider.Service getService(Provider p, String type) {
        return p.getService(type);
    }
    }
    */
    static CARAPI RenumProviders();

private:
    static CARAPI RegisterDefaultProviders();
    Object mLock;

private:
    // Security properties
    static const AutoPtr<IProperties> mSecprops;
};

}
}

#endif // __ELASTOS_SECURITY_CSECURITY_H__
