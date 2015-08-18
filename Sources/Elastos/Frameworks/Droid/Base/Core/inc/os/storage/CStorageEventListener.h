
#ifndef __ELASTOS_DROID_OS_STORAGE_CSTORAGEEVENTLISTENER_H__
#define __ELASTOS_DROID_OS_STORAGE_CSTORAGEEVENTLISTENER_H__

#include "_Elastos_Droid_Os_Storage_CStorageEventListener.h"

namespace Elastos {
namespace Droid {
namespace Os {
namespace Storage {

CarClass(CStorageEventListener)
{
public:
    CARAPI OnUsbMassStorageConnectionChanged(
        /* [in] */ Boolean connected);

    CARAPI OnStorageStateChanged(
        /* [in] */ CString path,
        /* [in] */ CString oldState,
        /* [in] */ CString newState);

private:
    // TODO: Add your private member variables here.
};

} // namespace Storage
} // namespace Os
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_OS_STORAGE_CSTORAGEEVENTLISTENER_H__
