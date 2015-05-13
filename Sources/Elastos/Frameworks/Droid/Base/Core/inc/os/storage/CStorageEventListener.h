
#ifndef __CSTORAGEEVENTLISTENER_H__
#define __CSTORAGEEVENTLISTENER_H__

#include "_CStorageEventListener.h"

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

#endif // __CSTORAGEEVENTLISTENER_H__
