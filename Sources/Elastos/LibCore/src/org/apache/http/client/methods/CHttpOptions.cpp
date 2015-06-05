
#include "CHttpOptions.h"
#include <elastos/Logger.h>

using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;
using Elastos::Utility::ISet;
using Elastos::Utility::CHashSet;
using Elastos::Utility::Logging::Logger;
using Elastos::Net::IURIHelper;
using Elastos::Net::CURIHelper;
using Org::Apache::Http::IHeaderIterator;
using Org::Apache::Http::IHeader;
using Org::Apache::Http::IHeaderElement;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Methods {

CAR_INTERFACE_IMPL(CHttpOptions, HttpRequestBase, IHttpOptions)

CAR_OBJECT_IMPL(CHttpOptions)

ECode CHttpOptions::GetMethod(
    /* [out] */ String* method)
{
    VALIDATE_NOT_NULL(result)
    *method = METHOD_NAME;
    return NOERROR;
}

ECode CHttpOptions::GetAllowedMethods(
    /* [in] */ IHttpResponse* response,
    /* [out] */ ISet** methods)
{
    VALIDATE_NOT_NULL(methods)
    *methods = NULL;

    if (response == NULL) {
        Logger::E("CHttpOptions", "HTTP response may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IHeaderIterator> it;
    response->HeaderIterator(String("Allow"), (IHeaderIterator**)&it);
    AutoPtr<ISet> _methods;
    CHashSet::New((ISet**)&_methods);
    AutoPtr<ICollection> col = ICollection::Probe(_methods);
    Boolean hasNext;
    while (it->HasNext(&hasNext), hasNext) {
        AutoPtr<IHeader> header;
        it->NextHeader((IHeader**)&header);
        AutoPtr< ArrayOf<IHeaderElement*> > elements;
        header->GetElements((ArrayOf<IHeaderElement*>**)&elements);
        for (Int32 i = 0; i < elements->GetLength(); ++i) {
            AutoPtr<IHeaderElement> element = (*elements)[i];
            String name;
            element->GetName(&name);
            AutoPtr<ICharSequence> cs;
            CStringWrapper::New(name, (ICharSequence**)&cs);
            col->Add(cs);
        }
    }
    *methods = _methods;
    REFCOUNT_ADD(*methods)
    return NOERROR;
}

ECode CHttpOptions::constructor();
{
    return NOERROR;
}

ECode CHttpOptions::constructor(
    /* [in] */ IURI* uri)
{
    return SetURI(uri);
}

ECode CHttpOptions::constructor(
    /* [in] */ const String& uri)
{
    AutoPtr<IURIHelper> helper;
    CURIHelper::AcquireSingleton((IURIHelper**)&helper);
    AutoPtr<IURI> iuri;
    helper->Create(uri, (IURI**)&iuri);
    return SetURI(iuri);
}

} // namespace Methods
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org