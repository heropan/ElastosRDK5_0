
#ifndef __ELASTOS_DROID_CONTENT_CLOADERFORCELOADCONTENTOBSERVER_H__
#define __ELASTOS_DROID_CONTENT_CLOADERFORCELOADCONTENTOBSERVER_H__

#include "_CLoaderForceLoadContentObserver.h"
#include <ext/frameworkext.h>

using namespace Elastos::Droid::Database;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CLoaderForceLoadContentObserver)
{
public:
    CARAPI GetContentObserver(
        /* [out] */ IIContentObserver** co);

    CARAPI ReleaseContentObserver(
        /* [out] */ IIContentObserver** co);

    CARAPI DeliverSelfNotifications(
        /* [out] */ Boolean* result);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __ELASTOS_DROID_CONTENT_CLOADERFORCELOADCONTENTOBSERVER_H__
