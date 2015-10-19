
#include "webkit/native/content/browser/TimeZoneMonitor.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Content::EIID_IBroadcastReceiver;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//               TimeZoneMonitor::InnerBroadcastReceiver
//=====================================================================
TimeZoneMonitor::InnerBroadcastReceiver::InnerBroadcastReceiver(
    /* [in] */ TimeZoneMonitor* owner)
    : mOwner(owner)
{
}

CAR_INTERFACE_IMPL(TimeZoneMonitor::InnerBroadcastReceiver, Object, IBroadcastReceiver);

ECode TimeZoneMonitor::InnerBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (!action.Equals(IIntent::ACTION_TIMEZONE_CHANGED)) {
        Slogger::E(TAG, "unexpected intent");
        return NOERROR;
    }

    return mOwner->NativeTimeZoneChangedFromJava(mOwner->mNativePtr);
}

//=====================================================================
//                           TimeZoneMonitor
//=====================================================================
const String TimeZoneMonitor::TAG("TimeZoneMonitor");

TimeZoneMonitor::TimeZoneMonitor(
    /* [in] */ IContext* context,
    /* [in] */ Int64 nativePtr)
    : mNativePtr(nativePtr)
{
    context->GetApplicationContext((IContext**)&mAppContext);
    AutoPtr<IIntent> stickyIntent;
    mAppContext->RegisterReceiver(mBroadcastReceiver, mFilter, (IIntent**)&stickyIntent);
}

AutoPtr<TimeZoneMonitor> TimeZoneMonitor::GetInstance(
    /* [in] */ IContext* context,
    /* [in] */ Int64 nativePtr)
{
    return new TimeZoneMonitor(context, nativePtr);
}

ECode TimeZoneMonitor::Stop()
{
    mAppContext->UnregisterReceiver(mBroadcastReceiver);
    mNativePtr = 0;

    return NOERROR;
}

ECode TimeZoneMonitor::NativeTimeZoneChangedFromJava(
    /* [in] */ Int64 nativeTimeZoneMonitorAndroid)
{
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
