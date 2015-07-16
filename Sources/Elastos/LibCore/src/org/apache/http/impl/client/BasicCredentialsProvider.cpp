
#include "BasicCredentialsProvider.h"
#include <elastos/Logger.h>

using Elastos::Utility::CHashMap;
using Elastos::Utility::ISet;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Client {

BasicCredentialsProvider::BasicCredentialsProvider()
{
    CHashMap::New((IHashMap**)&mCredMap);
}

CAR_INTERFACE_DECL(BasicCredentialsProvider, Object, ICredentialsProvider)

ECode BasicCredentialsProvider::SetCredentials(
    /* [in] */ IAuthScope* authscope,
    /* [in] */ ICredentials* credentials)
{
    synchronized(this) {
        if (authscope == NULL) {
            Logger::E("BasicCredentialsProvider", "Authentication scope may not be null");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        mCredMap->Put(authscope, credentials);
    }
    return NOERROR;
}

AutoPtr<ICredentials> BasicCredentialsProvider::MatchCredentials(
    /* [in] */ IHashMap* map,
    /* [in] */ IAuthScope* authscope)
{
    // see if we get a direct hit
    AutoPtr<IInterface> value;
    map->Get(authscope, (IInterface**)&value);
    AutoPtr<ICredentials> creds = ICredentials::Probe(value);
    if (creds == NULL) {
        // Nope.
        // Do a full scan
        Int32 bestMatchFactor  = -1;
        AutoPtr<IAuthScope> bestMatch;
        AutoPtr<ISet> keySet;
        map->GetKeySet((ISet**)&keySet);
        Boolean hasNext;
        while (keySet->HasNext(&hasNext), hasNext) {
            AutoPtr<IAuthScope> current;
            keySet->Get((IInterface**)&current);
            Int32 factor;
            authscope->Match(current, &factor);
            if (factor > bestMatchFactor) {
                bestMatchFactor = factor;
                bestMatch = current;
            }
        }
        if (bestMatch != NULL) {
            AutoPtr<IInterface> v;
            map->Get(bestMatch, (IInterface**)&v);
            creds = ICredentials::Probe(v);
        }
    }
    return creds;
}

ECode BasicCredentialsProvider::GetCredentials(
    /* [in] */ IAuthScope* authscope,
    /* [out] */ ICredentials** credentials)
{
    VALIDATE_NOT_NULL(cookies)
    *credentials = NULL;
    synchronized(this) {
        if (authscope == NULL) {
            Logger::E("BasicCredentialsProvider", "Authentication scope may not be null");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        *credentials = matchCredentials(mCredMap, authscope);
        REFCOUNT_ADD(*credentials)
    }
    return NOERROR;
}

ECode BasicCredentialsProvider::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    return IObject::Probe(mCredMap)->ToString(str);
}

ECode BasicCredentialsProvider::Clear()
{
    synchronized(this) {
        mCredMap->Clear();
    }
    return NOERROR;
}

} // namespace Client
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org