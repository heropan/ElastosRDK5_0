
#ifndef __ELASTOS_DROID_OS_STORAGE_STORAGEEVENTLISTENER_H__
#define __ELASTOS_DROID_OS_STORAGE_STORAGEEVENTLISTENER_H__

#include <elastos/core/Object.h>

namespace Elastos {
namespace Droid {
namespace Os {
namespace Storage {

class StorageEventListener
    : public Object
    , public IStorageEventListener
{
public:
    CAR_INTERFACE_DECL()

    CARAPI OnUsbMassStorageConnectionChanged(
        /* [in] */ Boolean connected);

    CARAPI OnStorageStateChanged(
        /* [in] */ const String& path,
        /* [in] */ const String& oldState,
        /* [in] */ const String& newState);

private:
};

} // namespace Storage
} // namespace Os
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_OS_STORAGE_STORAGEEVENTLISTENER_H__
