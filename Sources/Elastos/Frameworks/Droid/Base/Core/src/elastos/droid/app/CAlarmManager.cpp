
#include "elastos/droid/app/CAlarmManager.h"
#include "elastos/droid/os/Build.h"

using Elastos::Droid::Os::Build;

namespace Elastos {
namespace Droid {
namespace App {

const String CAlarmManager::TAG("CAlarmManager");

CAR_INTERFACE_IMPL(CAlarmManager, Object, IAlarmManager)

CAR_OBJECT_IMPL(CAlarmManager)

CAlarmManager::CAlarmManager()
    : mAlwaysExact(FALSE)
{
}

ECode CAlarmManager::constructor(
    /* [in] */ IIAlarmManager* service,
    /* [in] */ IContext* ctx)
{
    mService = service;

    Int32 sdkVersion = ctx->GetApplicationInfo().targetSdkVersion;
    mAlwaysExact = (sdkVersion < Build::VERSION_CODES::KITKAT);
    return NOERROR;
}

Int64 CAlarmManager::LegacyExactLength()
{
    return (mAlwaysExact ? WINDOW_EXACT : WINDOW_HEURISTIC);
}

ECode CAlarmManager::Set(
    /* [in] */ Int32 type,
    /* [in] */ Int64 triggerAtMillis,
    /* [in] */ IPendingIntent* operation)
{
    return SetImpl(type, triggerAtMillis, LegacyExactLength(), 0, operation, NULL, NULL);
}

ECode CAlarmManager::SetRepeating(
    /* [in] */ Int32 type,
    /* [in] */ Int64 triggerAtMillis,
    /* [in] */ Int64 intervalMillis,
    /* [in] */ IPendingIntent* operation)
{
    return SetImpl(type, triggerAtMillis, LegacyExactLength(), intervalMillis, operation, NULL, NULL);
}

ECode CAlarmManager::SetWindow(
    /* [in] */ Int32 type,
    /* [in] */ Int64 windowStartMillis,
    /* [in] */ Int64 windowLengthMillis,
    /* [in] */ IPendingIntent* operation);
{
    return SetImpl(type, windowStartMillis, windowLengthMillis, 0, operation, NULL, NULL);
}


    /**
     * Schedule an alarm to be delivered precisely at the stated time.
     *
     * <p>
     * This method is like {@link #set(int, long, PendingIntent)}, but does not permit
     * the OS to adjust the delivery time.  The alarm will be delivered as nearly as
     * possible to the requested trigger time.
     *
     * <p>
     * <b>Note:</b> only alarms for which there is a strong demand for exact-time
     * delivery (such as an alarm clock ringing at the requested time) should be
     * scheduled as exact.  Applications are strongly discouraged from using exact
     * alarms unnecessarily as they reduce the OS's ability to minimize battery use.
     *
     * @param type One of {@link #ELAPSED_REALTIME}, {@link #ELAPSED_REALTIME_WAKEUP},
     *        {@link #RTC}, or {@link #RTC_WAKEUP}.
     * @param triggerAtMillis time in milliseconds that the alarm should go
     *        off, using the appropriate clock (depending on the alarm type).
     * @param operation Action to perform when the alarm goes off;
     *        typically comes from {@link PendingIntent#getBroadcast
     *        IntentSender.getBroadcast()}.
     *
     * @see #set
     * @see #setRepeating
     * @see #setWindow
     * @see #cancel
     * @see android.content.Context#sendBroadcast
     * @see android.content.Context#registerReceiver
     * @see android.content.Intent#filterEquals
     * @see #ELAPSED_REALTIME
     * @see #ELAPSED_REALTIME_WAKEUP
     * @see #RTC
     * @see #RTC_WAKEUP
     */
    ECode CAlarmManager::SetExact(
        /* [in] */ Int32 type,
        /* [in] */ Int64 triggerAtMillis,
        /* [in] */ IPendingIntent* operation)
    {
        setImpl(type, triggerAtMillis, WINDOW_EXACT, 0, operation, null, null);
    }

    /**
     * Schedule an alarm that represents an alarm clock.
     *
     * The system may choose to display information about this alarm to the user.
     *
     * <p>
     * This method is like {@link #setExact(int, long, PendingIntent)}, but implies
     * {@link #RTC_WAKEUP}.
     *
     * @param info
     * @param operation Action to perform when the alarm goes off;
     *        typically comes from {@link PendingIntent#getBroadcast
     *        IntentSender.getBroadcast()}.
     *
     * @see #set
     * @see #setRepeating
     * @see #setWindow
     * @see #setExact
     * @see #cancel
     * @see #getNextAlarmClock()
     * @see android.content.Context#sendBroadcast
     * @see android.content.Context#registerReceiver
     * @see android.content.Intent#filterEquals
     */
    ECode CAlarmManager::SetAlarmClock(
        /* [in] */ IAlarmClockInfo* info,
        /* [in] */ IPendingIntent* operation)
    {
        return SetImpl(RTC_WAKEUP, info.getTriggerTime(), WINDOW_EXACT, 0, operation, null, info);
    }

/** @hide */
ECode CAlarmManager::Set(
    /* [in] */ Int32 type,
    /* [in] */ Int64 triggerAtMillis,
    /* [in] */ Int64 windowMillis,
    /* [in] */ Int64 intervalMillis,
    /* [in] */ IPendingIntent* operation,
    /* [in] */ IWorkSource* workSource)
{
    return SetImpl(type, triggerAtMillis, windowMillis, intervalMillis, operation, workSource, NULL);
}

ECode CAlarmManager::SetImpl(
    /* [in] */ Int32 type,
    /* [in] */ Int64 triggerAtMillis,
    /* [in] */ Int64 windowMillis,
    /* [in] */ Int64 intervalMillis,
    /* [in] */ IPendingIntent* operation,
    /* [in] */ IWorkSource* workSource,
    /* [in] */ IAlarmClockInfo* alarmClock);
{
    if (triggerAtMillis < 0) {
        /* NOTYET
        if (mAlwaysExact) {
            // Fatal error for KLP+ apps to use negative trigger times
            throw new IllegalArgumentException("Invalid alarm trigger time "
                    + triggerAtMillis);
        }
        */
        triggerAtMillis = 0;
    }

    try {
        mService.set(type, triggerAtMillis, windowMillis, intervalMillis, operation,
                workSource, alarmClock);
    } catch (RemoteException ex) {
    }
}

ECode CAlarmManager::SetInexactRepeating(
    /* [in] */ Int32 type,
    /* [in] */ Int64 triggerAtMillis,
    /* [in] */ Int64 intervalMillis,
    /* [in] */ IPendingIntent* operation)
{
    return SetImpl(type, triggerAtMillis, WINDOW_HEURISTIC, intervalMillis, operation, null, null);
}

ECode CAlarmManager::Cancel(
    /* [in] */ IPendingIntent* operation)
{
    //try {
    return mService->Remove(operation);
    //} catch (RemoteException ex) {
    //}
}

ECode CAlarmManager::SetTime(
    /* [in] */ Int64 millis)
{
    //try {
    return mService->SetTime(millis);
    //} catch (RemoteException ex) {
    //}
}

/**
 * Set the system default time zone.
 * Requires the permission android.permission.SET_TIME_ZONE.
 *
 * @param timeZone in the format understood by {@link java.util.TimeZone}
 */
ECode CAlarmManager::SetTimeZone(
    /* [in] */ const String& timeZone)
{
    //try {
    return mService->SetTimeZone(timeZone);
    //} catch (RemoteException ex) {
    //}
}

ECode CAlarmManager::GetNextAlarmClock(
    /* [out] */ IAlarmClockInfo** info)
{
    return GetNextAlarmClock(UserHandle.myUserId());
}

ECode CAlarmManager::GetNextAlarmClock(
    /* [in] */ Int32 userId
    /* [out] */ IAlarmClockInfo** info)
{
    try {
        return mService.getNextAlarmClock(userId);
    } catch (RemoteException ex) {
        return null;
    }
}


}
}
}