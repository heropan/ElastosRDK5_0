
#ifndef __CMOUNTSERVICEBINDERLISTENER_H__
#define __CMOUNTSERVICEBINDERLISTENER_H__

#include "_CMountServiceBinderListener.h"
#include "os/storage/CStorageManager.h"

namespace Elastos {
namespace Droid {
namespace Os {
namespace Storage {

CarClass(CMountServiceBinderListener)
{
public:
    CARAPI constructor(
        /* [in] */ IStorageManager* host);

    CARAPI OnUsbMassStorageConnectionChanged(
        /* [in] */ Boolean connected);

    CARAPI OnStorageStateChanged(
        /* [in] */ const String& path,
        /* [in] */ const String& oldState,
        /* [in] */ const String& newState);

private:
    CStorageManager* mHost;
};

} // namespace Storage
} // namespace Os
} // namepsace Droid
} // namespace Elastos

#endif // __CMOUNTSERVICEBINDERLISTENER_H__
