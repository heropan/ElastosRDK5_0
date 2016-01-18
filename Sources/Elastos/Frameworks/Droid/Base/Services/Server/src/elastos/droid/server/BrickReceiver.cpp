
#include "elastos/droid/server/BrickReceiver.h"
#include "elastos/droid/os/SystemService.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Os::SystemService;

namespace Elastos {
namespace Droid {
namespace Server {

ECode BrickReceiver::OnReceive(
    /* [in] */ IContext *context,
    /* [in] */ IIntent *intent)
{
    Slogger::W("BrickReceiver", "!!! BRICKING DEVICE !!!");
    return SystemService::Start(String("brick"));
}

} // Server
} // Droid
} // Elastos