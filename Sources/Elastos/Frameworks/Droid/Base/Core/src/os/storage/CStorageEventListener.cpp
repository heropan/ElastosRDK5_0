
#include "os/storage/CStorageEventListener.h"

namespace Elastos {
namespace Droid {
namespace Os {
namespace Storage {

ECode CStorageEventListener::OnUsbMassStorageConnectionChanged(
    /* [in] */ Boolean connected)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStorageEventListener::OnStorageStateChanged(
    /* [in] */ CString path,
    /* [in] */ CString oldState,
    /* [in] */ CString newState)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

} // namespace Storage
} // namespace Os
} // namepsace Droid
} // namespace Elastos

