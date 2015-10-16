
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/provider/CSettings.h"
#include "elastos/droid/provider/Settings.h"

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CSettings::GetGTalkDeviceId(
    /* [in] */ Int64 androidId,
    /* [out] */ String* deviceId)
{
    VALIDATE_NOT_NULL(deviceId)
    *deviceId = Settings::GetGTalkDeviceId(androidId);
    return NOERROR;
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos