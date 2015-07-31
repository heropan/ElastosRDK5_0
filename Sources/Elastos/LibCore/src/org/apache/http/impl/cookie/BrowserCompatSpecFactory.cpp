
#include "BrowserCompatSpecFactory.h"
#include "BestMatchSpec.h"

using Org::Apache::Http::Cookie::Params::ICookieSpecPNames;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Cookie {

CAR_INTERFACE_IMPL(BrowserCompatSpecFactory, Object, ICookieSpecFactory)

ECode BrowserCompatSpecFactory::NewInstance(
    /* [in] */ IHttpParams* params,
    /* [out] */ ICookieSpec** spec);
{
    VALIDATE_NOT_NULL(spec)
    if (params != NULL) {
        AutoPtr<IInterface> param;
        params->GetParameter(ICookieSpecPNames::DATE_PATTERNS, (IInterface**)&value);
        AutoPtr<ICollection> collection = ICollection::Probe(param);
        AutoPtr< ArrayOf<IInterface*> > paramArray;
        collection->ToArray((ArrayOf<IInterface*>**)&paramArray);
        AutoPtr< ArrayOf<String> > strs = ArrayOf<String>::Alloc(paramArray->GetLength());
        for (Int32 i = 0; i < paramArray->GetLength(); i++) {
            AutoPtr<ICharSequence> cs = ICharSequence::Probe((*paramArray)[i]);
            cs->ToString(&(*strs)[i]);
        }
        *spec = (ICookieSpec*)new BrowserCompatSpec(strs);
    }
    else {
        *spec = (ICookieSpec*)new BrowserCompatSpec();
    }
    REFCOUNT_ADD(*spec)
    return NOERROR;
}

} // namespace Cookie
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org