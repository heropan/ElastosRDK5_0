
#include "am/ReceiverList.h"
#include "am/ProcessRecord.h"
#include "am/CActivityManagerService.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;

#include <elastos/utility/logging/Slogger.h>
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

CAR_INTERFACE_IMPL_LIGHT(ReceiverList, IProxyDeathRecipient)

ReceiverList::ReceiverList(
    /* [in] */ CActivityManagerService* owner,
    /* [in] */ ProcessRecord* callerApp,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 userId,
    /* [in] */ IIntentReceiver* receiver)
    : mOwner(owner)
    , mReceiver(receiver)
    , mApp(callerApp)
    , mPid(pid)
    , mUid(uid)
    , mUserId(userId)
    , mLinkedToDeath(FALSE)
{
}

ReceiverList::~ReceiverList()
{
    for (Iterator it = Begin(); it !=  End(); ++it) {
        AutoPtr<BroadcastFilter> filter = *it;
        filter->mReceiverList = NULL;
    }
    Clear();
}

ECode ReceiverList::ProxyDied()
{
    Slogger::W("ReceiverList", " >> %p ProxyDied()", (IProxyDeathRecipient*)this);
    mLinkedToDeath = FALSE;
    return mOwner->UnregisterReceiver(mReceiver);
}

String ReceiverList::ToString()
{
    if (!mStringName.IsNull()) {
        return mStringName;
    }
    StringBuilder sb(128);
    sb += ("ReceiverList{");
    sb += (StringUtils::Int32ToHexString(Int32(this)));
    sb += (", pid=");
    sb += (mPid);
    sb += (", processName=");
    sb += ((mApp != NULL ? mApp->mProcessName : "(unknown name)"));
    sb += (", uid=");
    sb += (mUid);
    sb += (", userId=");
    sb += (mUserId);
    sb += "receiver=";
    sb += (StringUtils::Int32ToHexString((Int32)mReceiver.Get()));
    sb += ("}");
    return mStringName = sb.ToString();
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
