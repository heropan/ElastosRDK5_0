#ifndef __ELASTOS_DROID_DATABASE_CCONTENTOBSERVERTRANSPORT_H__
#define __ELASTOS_DROID_DATABASE_CCONTENTOBSERVERTRANSPORT_H__

#include "_CContentObserverTransport.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Database {

CarClass(CContentObserverTransport)
{
public:
    CARAPI OnChange(
        /* [in] */ Boolean selfchange,
        /* [in] */ IUri* uri);

    CARAPI ReleaseContentObserver();

    CARAPI constructor(
        /* [in] */ IContentObserver* contentObserver);

private:
    AutoPtr<IContentObserver> mContentObserver;
};

} //Database
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_DATABASE_CCONTENTOBSERVERTRANSPORT_H__
