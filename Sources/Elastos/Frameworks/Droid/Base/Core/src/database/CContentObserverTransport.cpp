
#include "database/CContentObserverTransport.h"

namespace Elastos {
namespace Droid {
namespace Database {

ECode CContentObserverTransport::OnChange(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    AutoPtr<IContentObserver> contentObserver = mContentObserver;
    if (contentObserver) {
        contentObserver->DispatchChangeEx(selfChange, uri);
    }
    return NOERROR;
}

ECode CContentObserverTransport::ReleaseContentObserver()
{
    mContentObserver = NULL;
    return NOERROR;
}

ECode CContentObserverTransport::constructor(
    /* [in] */ IContentObserver* contentObserver)
{
    mContentObserver = contentObserver;
    return NOERROR;
}

} //Database
} //Droid
} //Elastos
