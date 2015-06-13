
#include "CSchemeRegistry.h"
#include <elastos/Logger.h>
#include <elastos/StringBuilder.h>
#include <elastos/StringUtils.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Utility::CLinkedHashMap;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Conn {
namespace Scheme {

CAR_INTERFACE_IMPL(CSchemeRegistry, Object, ISchemeRegistry)

CAR_OBJECT_IMPL(CSchemeRegistry)

ECode CSchemeRegistry::GetScheme(
    /* [in] */ const String& name,
    /* [out] */ IScheme** scheme)
{
    synchronized(this)

    VALIDATE_NOT_NULL(scheme)
    *scheme = NULL;

    AutoPtr<IScheme> found;
    Get(name, (IScheme**)&found);
    if (found == NULL) {
        Logger::E("CSchemeRegistry", "Scheme '%s' not registered.", name.string());
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    *scheme = found;
    REFCOUNT_ADD(*scheme)
    return NOERROR;
}

ECode CSchemeRegistry::GetScheme(
    /* [in] */ IHttpHost* host,
    /* [out] */ IScheme** scheme);
{
    synchronized(this)

    VALIDATE_NOT_NULL(scheme)
    *scheme = NULL;
    if (host == NULL) {
        Logger::E("CSchemeRegistry", "Host must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String name;
    host->GetSchemeName(&name);
    return GetScheme(name, scheme);
}

ECode CSchemeRegistry::Get(
    /* [in] */ const String& name,
    /* [out] */ IScheme** scheme)
{
    synchronized(this)

    VALIDATE_NOT_NULL(scheme)
    *scheme = NULL;
    if (name.IsNull()) {
        Logger::E("CSchemeRegistry", "Name must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // leave it to the caller to use the correct name - all lowercase
    //name = name.toLowerCase();
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(name, (ICharSequence**)&cs);
    AutoPtr<IInterface> found;
    mRegisteredSchemes->Get(cs, (IInterface**)&found);
    *scheme = IScheme::Probe(found);
    REFCOUNT_ADD(*scheme)
    return NOERROR;
}

ECode CSchemeRegistry::Register(
    /* [in] */ IScheme* sch,
    /* [out] */ IScheme** scheme)
{
    synchronized(this)

    VALIDATE_NOT_NULL(scheme)
    *scheme = NULL;
    if (sch == NULL) {
        Logger::E("CSchemeRegistry", "Scheme must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    String name;
    sch->GetName(&name);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(name, (ICharSequence**)&cs);
    AutoPtr<IInterface> old;
    mRegisteredSchemes->(cs, (IInterface**)&old);
    mRegisteredSchemes->Put(cs, sch);
    *scheme = IScheme::Probe(old);
    REFCOUNT_ADD(*scheme)
    return NOERROR;
}

ECode CSchemeRegistry::Unregister(
    /* [in] */ const String& name,
    /* [out] */ IScheme** scheme)
{
    synchronized(this)

    VALIDATE_NOT_NULL(scheme)
    *scheme = NULL;
    if (name.IsNull()) {
        Logger::E("CSchemeRegistry", "Name must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // leave it to the caller to use the correct name - all lowercase
    //name = name.toLowerCase();
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(name, (ICharSequence**)&cs);
    AutoPtr<IInterface> gone;
    mRegisteredSchemes->Remove(cs, (IInterface**)&gone);
    *scheme = IScheme::Probe(gone);
    REFCOUNT_ADD(*scheme)
    return NOERROR;
}

ECode CSchemeRegistry::GetSchemeNames(
    /* [out] */ IList** names)
{
    synchronized(this)

    VALIDATE_NOT_NULL(names)
    AutoPtr<ISet> keyset;
    mRegisteredSchemes->GetKeySet((ISet**)&keyset);
    AutoPtr<ICollection> col = ICollection::Probe(keyset);
    AutoPtr<IArrayList> _names;
    CArrayList::New(col, (IArrayList**)&_names);
    *names = IList::Probe(_names);
    REFCOUNT_ADD(*names)
    return NOERROR;
}

ECode CSchemeRegistry::SetItems(
    /* [in] */ IMap* map)
{
    synchronized(this)

    if (map == NULL) {
        return NOERROR;
    }
    mRegisteredSchemes->Clear();
    return mRegisteredSchemes->PutAll(map);
}

ECode CSchemeRegistry::constructor()
{
    CLinkedHashMap::New((IMap**)&mRegisteredSchemes);
    return NOERROR;
}

} // namespace Scheme
} // namespace Conn
} // namespace Http
} // namespace Apache
} // namespace Org
