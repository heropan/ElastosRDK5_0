// wuweizuo automatic build .cpp file from .java file.

#include "TimeZoneMonitor.h"

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
    // ==================before translated======================
    // mOwner = owner;
}

ECode TimeZoneMonitor::InnerBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(intent);
    // ==================before translated======================
    // if (!intent.getAction().equals(Intent.ACTION_TIMEZONE_CHANGED)) {
    //     Log.e(TAG, "unexpected intent");
    //     return;
    // }
    //
    // nativeTimeZoneChangedFromJava(mNativePtr);
    assert(0);
    return NOERROR;
}

//=====================================================================
//                           TimeZoneMonitor
//=====================================================================
const String TimeZoneMonitor::TAG("TimeZoneMonitor");

AutoPtr<TimeZoneMonitor> TimeZoneMonitor::GetInstance(
    /* [in] */ IContext* context,
    /* [in] */ Int64 nativePtr)
{
    // ==================before translated======================
    // return new TimeZoneMonitor(context, nativePtr);
    assert(0);
    AutoPtr<TimeZoneMonitor> empty;
    return empty;
}

ECode TimeZoneMonitor::Stop()
{
    // ==================before translated======================
    // mAppContext.unregisterReceiver(mBroadcastReceiver);
    // mNativePtr = 0;
    assert(0);
    return NOERROR;
}

TimeZoneMonitor::TimeZoneMonitor(
    /* [in] */ IContext* context,
    /* [in] */ Int64 nativePtr)
{
    // ==================before translated======================
    // mAppContext = context.getApplicationContext();
    // mNativePtr = nativePtr;
    // mAppContext.registerReceiver(mBroadcastReceiver, mFilter);
}

ECode TimeZoneMonitor::NativeTimeZoneChangedFromJava(
    /* [in] */ Int64 nativeTimeZoneMonitorAndroid)
{
    assert(0);
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


