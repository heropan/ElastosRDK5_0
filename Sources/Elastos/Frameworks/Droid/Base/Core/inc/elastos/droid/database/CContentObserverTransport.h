#ifndef __ELASTOS_DROID_DATABASE_CCONTENTOBSERVERTRANSPORT_H__
#define __ELASTOS_DROID_DATABASE_CCONTENTOBSERVERTRANSPORT_H__

#include "_Elastos_Droid_Database_CContentObserverTransport.h"
#include <elastos/core/Object.h>
#include <Elastos.Droid.Database.h>

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Database::IIContentObserver;

namespace Elastos {
namespace Droid {
namespace Database {

CarClass(CContentObserverTransport)
    , public Object
    , public IContentObserverTransport
    , public IIContentObserver
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI OnChange(
        /* [in] */ Boolean selfchange,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 userId);

    CARAPI ReleaseContentObserver();

    CARAPI constructor(
        /* [in] */ IContentObserver* contentObserver);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    AutoPtr<IContentObserver> mContentObserver;
};

} //Database
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_DATABASE_CCONTENTOBSERVERTRANSPORT_H__
