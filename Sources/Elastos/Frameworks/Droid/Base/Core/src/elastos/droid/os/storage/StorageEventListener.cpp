
#include "elastos/droid/os/storage/StorageEventListener.h"

namespace Elastos {
namespace Droid {
namespace Os {
namespace Storage {

ECode StorageEventListener::OnUsbMassStorageConnectionChanged(
    /* [in] */ Boolean connected)
{
    return NOERROR;
}

ECode StorageEventListener::OnStorageStateChanged(
    /* [in] */ const String& path,
    /* [in] */ const String& oldState,
    /* [in] */ const String& newState)
{
    return NOERROR;
}

} // namespace Storage
} // namespace Os
} // namepsace Droid
} // namespace Elastos

