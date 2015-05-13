
#include "media/CRemoteDisplayHelper.h"
#include "media/CRemoteDisplay.h"

namespace Elastos {
namespace Droid {
namespace Media {

ECode CRemoteDisplayHelper::Listen(
    /* [in] */ const String& iface,
    /* [in] */ IRemoteDisplayListener* listener,
    /* [in] */ IHandler* handler,
    /* [out] */ IRemoteDisplay** result)
{
    return CRemoteDisplay::Listen(iface, listener, handler, result);
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
