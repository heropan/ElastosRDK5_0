
#include "am/AppBindRecord.h"
#include "am/CServiceRecord.h"
#include "am/IntentBindRecord.h"
#include "am/ProcessRecord.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

AppBindRecord::AppBindRecord(
    /* [in] */ CServiceRecord* service,
    /* [in] */ IntentBindRecord* intent,
    /* [in] */ ProcessRecord* client)
    : mService(service)
    , mIntent(intent)
    , mClient(client)
{
}

String AppBindRecord::ToString()
{
    StringBuilder sb(128);
    sb += "AppBindRecord{";
    sb += (StringUtils::Int32ToHexString(Int32(this)));
    sb += " ";
    sb += mService->mShortName;
    sb += ":";
    sb += mClient->mProcessName;
    sb += "}";
    return sb.ToString();
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
