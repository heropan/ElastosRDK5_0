
#include "am/IntentBindRecord.h"
#include "am/CServiceRecord.h"
#include "am/ProcessRecord.h"
#include <elastos/StringBuilder.h>
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Core::IStringBuilder;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

IntentBindRecord::IntentBindRecord(
    /* [in] */ CServiceRecord* service,
    /* [in] */ IIntentFilterComparison* intent)
    : mService(service)
    , mIntent(intent)
    , mRequested(FALSE)
    , mReceived(FALSE)
    , mHasBound(FALSE)
    , mDoRebind(FALSE)
{
}

IntentBindRecord::~IntentBindRecord()
{
    mApps.Clear();
}

String IntentBindRecord::ToString()
{
    if (!mStringName.IsNull()) {
        return mStringName;
    }

    StringBuilder sb(128);
    sb += ("IntentBindRecord{");
    sb += (StringUtils::Int32ToHexString(Int32(this)));
    sb += (' ');
    if ((CollectFlags()& IContext::BIND_AUTO_CREATE) != 0) {
        sb += ("CR ");
    }
    sb += (mService->mShortName);
    sb += (':');
    if (mIntent != NULL) {
        AutoPtr<IIntent> intent;
        mIntent->GetIntent((IIntent**)&intent);
        intent->ToShortStringEx((IStringBuilder*)&sb, FALSE, FALSE, FALSE, FALSE);
    }
    sb += ('}');
    return mStringName = sb.ToString();
}

Int32 IntentBindRecord::CollectFlags()
{
    Int32 flags = 0;
    if (mApps.IsEmpty() == FALSE) {
        HashSet< AutoPtr<ConnectionRecord> >::Iterator cit;
        HashMap< AutoPtr<ProcessRecord>, AutoPtr<AppBindRecord> >::Iterator it;
        for (it = mApps.Begin(); it != mApps.End(); ++it) {
            AutoPtr<AppBindRecord> app = it->mSecond;
            if (app->mConnections.IsEmpty() == FALSE) {
                for (cit = app->mConnections.Begin(); cit != app->mConnections.End(); ++cit) {
                    AutoPtr<ConnectionRecord> conn = *cit;
                    flags |= conn->mFlags;
                }
            }
        }
    }
    return flags;
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
