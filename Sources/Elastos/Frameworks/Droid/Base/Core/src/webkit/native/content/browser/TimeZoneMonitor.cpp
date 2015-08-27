// wuweizuo automatic build .cpp file from .java file.

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
    /*
    mOwner = owner;
    */
}

ECode TimeZoneMonitor::InnerBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(intent);
    /*
    if (!intent.getAction().equals(Intent.ACTION_TIMEZONE_CHANGED)) {
        Log.e(TAG, "unexpected intent");
        return;
    }


    nativeTimeZoneChangedFromJava(mNativePtr);
    */
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
    /*
    return new TimeZoneMonitor(context, nativePtr);
    */
}

ECode TimeZoneMonitor::Stop()
{
    /*
    mAppContext.unregisterReceiver(mBroadcastReceiver);
    mNativePtr = 0;
    */
    return NOERROR;
}

TimeZoneMonitor::TimeZoneMonitor(
    /* [in] */ IContext* context,
    /* [in] */ Int64 nativePtr)
{
    /*
    mAppContext = context.getApplicationContext();
    mNativePtr = nativePtr;
    mAppContext.registerReceiver(mBroadcastReceiver, mFilter);
    */
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


