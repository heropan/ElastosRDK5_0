
#ifndef __ELASTOS_DROID_JAVAPROXY_CCONTENTOBSERVERNATIVE_H__
#define __ELASTOS_DROID_JAVAPROXY_CCONTENTOBSERVERNATIVE_H__

#include "_Elastos_Droid_JavaProxy_CContentObserverNative.h"
#include <elastos/core/Object.h>
#include <jni.h>

using Elastos::Droid::Database::IIContentObserver;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IBinder;

namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CContentObserverNative)
    , public Object
    , public IBinder
{
public:
    ~CContentObserverNative();

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

    CARAPI GetContentObserver(
        /* [out] */ IIContentObserver** observer);

    CARAPI ReleaseContentObserver(
        /* [out] */ IIContentObserver** oldObserver);

    CARAPI DeliverSelfNotifications(
        /* [out] */ Boolean* result);

    CARAPI OnChange(
        /* [in] */ Boolean selfChange);

    CARAPI OnChangeEx(
        /* [in] */ Boolean selfChange,
        /* [in] */ IUri* uri);

    CARAPI DispatchChange(
        /* [in] */ Boolean selfChange);

    CARAPI DispatchChangeEx(
        /* [in] */ Boolean selfChange,
        /* [in] */ IUri* uri);

private:
    static const String TAG;

    JavaVM* mJVM;
    jobject mJInstance;
};

}
}
}

#endif // __ELASTOS_DROID_JAVAPROXY_CCONTENTOBSERVERNATIVE_H__
