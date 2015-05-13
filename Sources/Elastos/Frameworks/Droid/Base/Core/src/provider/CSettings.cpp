
#include "ext/frameworkext.h"
#include "provider/CSettings.h"
#include "provider/Settings.h"

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