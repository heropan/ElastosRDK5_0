
#ifndef __ELASTOS_DROID_APP_CALARMMANAGER_H__
#define __ELASTOS_DROID_APP_CALARMMANAGER_H__

#include "_CAlarmManager.h"

using Elastos::Droid::App::IIAlarmManager;
using Elastos::Droid::App::IPendingIntent;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CAlarmManager)
{
public:
    /**
     * package private on purpose
     */
    CARAPI constructor(
        /* [in] */ IIAlarmManager* service);

    /**
     * Schedule an alarm.  <b>Note: for timing operations (ticks, timeouts,
     * etc) it is easier and much more efficient to use
     * {@link android.os.Handler}.</b>  If there is already an alarm scheduled
     * for the same IntentSender, it will first be canceled.
     *
     * <p>If the time occurs in the past, the alarm will be triggered
     * immediately.  If there is already an alarm for this Intent
     * scheduled (with the equality of two intents being defined by
     * {@link Intent#filterEquals}), then it will be removed and replaced by
     * this one.
     *
     * <p>
     * The alarm is an intent broadcast that goes to a broadcast receiver that
     * you registered with {@link android.content.Context#registerReceiver}
     * or through the &lt;receiver&gt; tag in an AndroidManifest.xml file.
     *
     * <p>
     * Alarm intents are delivered with a data extra of type int called
     * {@link Intent#EXTRA_ALARM_COUNT Intent.EXTRA_ALARM_COUNT} that indicates
     * how many past alarm events have been accumulated into this intent
     * broadcast.  Recurring alarms that have gone undelivered because the
     * phone was asleep may have a count greater than one when delivered.
     *
     * @param type One of ELAPSED_REALTIME, ELAPSED_REALTIME_WAKEUP, RTC or
     *             RTC_WAKEUP.
     * @param triggerAtMillis time in milliseconds that the alarm should go
     * off, using the appropriate clock (depending on the alarm type).
     * @param operation Action to perform when the alarm goes off;
     * typically comes from {@link PendingIntent#getBroadcast
     * IntentSender.getBroadcast()}.
     *
     * @see android.os.Handler
     * @see #setRepeating
     * @see #cancel
     * @see android.content.Context#sendBroadcast
     * @see android.content.Context#registerReceiver
     * @see android.content.Intent#filterEquals
     * @see #ELAPSED_REALTIME
     * @see #ELAPSED_REALTIME_WAKEUP
     * @see #RTC
     * @see #RTC_WAKEUP
     */
    CARAPI Set(
        /* [in] */ Int32 type,
        /* [in] */ Int64 triggerAtMillis,
        /* [in] */ IPendingIntent* operation);

    /**
     * Schedule a repeating alarm.  <b>Note: for timing operations (ticks,
     * timeouts, etc) it is easier and much more efficient to use
     * {@link android.os.Handler}.</b>  If there is already an alarm scheduled
     * for the same IntentSender, it will first be canceled.
     *
     * <p>Like {@link #set}, except you can also
     * supply a rate at which the alarm will repeat.  This alarm continues
     * repeating until explicitly removed with {@link #cancel}.  If the time
     * occurs in the past, the alarm will be triggered immediately, with an
     * alarm count depending on how far in the past the trigger time is relative
     * to the repeat interval.
     *
     * <p>If an alarm is delayed (by system sleep, for example, for non
     * _WAKEUP alarm types), a skipped repeat will be delivered as soon as
     * possible.  After that, future alarms will be delivered according to the
     * original schedule; they do not drift over time.  For example, if you have
     * set a recurring alarm for the top of every hour but the phone was asleep
     * from 7:45 until 8:45, an alarm will be sent as soon as the phone awakens,
     * then the next alarm will be sent at 9:00.
     *
     * <p>If your application wants to allow the delivery times to drift in
     * order to guarantee that at least a certain time interval always elapses
     * between alarms, then the approach to take is to use one-time alarms,
     * scheduling the next one yourself when handling each alarm delivery.
     *
     * @param type One of ELAPSED_REALTIME, ELAPSED_REALTIME_WAKEUP}, RTC or
     *             RTC_WAKEUP.
     * @param triggerAtMillis time in milliseconds that the alarm should first
     * go off, using the appropriate clock (depending on the alarm type).
     * @param intervalMillis interval in milliseconds between subsequent repeats
     * of the alarm.
     * @param operation Action to perform when the alarm goes off;
     * typically comes from {@link PendingIntent#getBroadcast
     * IntentSender.getBroadcast()}.
     *
     * @see android.os.Handler
     * @see #set
     * @see #cancel
     * @see android.content.Context#sendBroadcast
     * @see android.content.Context#registerReceiver
     * @see android.content.Intent#filterEquals
     * @see #ELAPSED_REALTIME
     * @see #ELAPSED_REALTIME_WAKEUP
     * @see #RTC
     * @see #RTC_WAKEUP
     */
    CARAPI SetRepeating(
        /* [in] */ Int32 type,
        /* [in] */ Int64 triggerAtMillis,
        /* [in] */ Int64 intervalMillis,
        /* [in] */ IPendingIntent* operation);

    /**
     * Schedule a repeating alarm that has inexact trigger time requirements;
     * for example, an alarm that repeats every hour, but not necessarily at
     * the top of every hour.  These alarms are more power-efficient than
     * the strict recurrences supplied by {@link #setRepeating}, since the
     * system can adjust alarms' phase to cause them to fire simultaneously,
     * avoiding waking the device from sleep more than necessary.
     *
     * <p>Your alarm's first trigger will not be before the requested time,
     * but it might not occur for almost a full interval after that time.  In
     * addition, while the overall period of the repeating alarm will be as
     * requested, the time between any two successive firings of the alarm
     * may vary.  If your application demands very low jitter, use
     * {@link #setRepeating} instead.
     *
     * @param type One of ELAPSED_REALTIME, ELAPSED_REALTIME_WAKEUP}, RTC or
     *             RTC_WAKEUP.
     * @param triggerAtMillis time in milliseconds that the alarm should first
     * go off, using the appropriate clock (depending on the alarm type).  This
     * is inexact: the alarm will not fire before this time, but there may be a
     * delay of almost an entire alarm interval before the first invocation of
     * the alarm.
     * @param intervalMillis interval in milliseconds between subsequent repeats
     * of the alarm.  If this is one of INTERVAL_FIFTEEN_MINUTES,
     * INTERVAL_HALF_HOUR, INTERVAL_HOUR, INTERVAL_HALF_DAY, or INTERVAL_DAY
     * then the alarm will be phase-aligned with other alarms to reduce the
     * number of wakeups.  Otherwise, the alarm will be set as though the
     * application had called {@link #setRepeating}.
     * @param operation Action to perform when the alarm goes off;
     * typically comes from {@link PendingIntent#getBroadcast
     * IntentSender.getBroadcast()}.
     *
     * @see android.os.Handler
     * @see #set
     * @see #cancel
     * @see android.content.Context#sendBroadcast
     * @see android.content.Context#registerReceiver
     * @see android.content.Intent#filterEquals
     * @see #ELAPSED_REALTIME
     * @see #ELAPSED_REALTIME_WAKEUP
     * @see #RTC
     * @see #RTC_WAKEUP
     * @see #INTERVAL_FIFTEEN_MINUTES
     * @see #INTERVAL_HALF_HOUR
     * @see #INTERVAL_HOUR
     * @see #INTERVAL_HALF_DAY
     * @see #INTERVAL_DAY
     */
    CARAPI SetInexactRepeating(
        /* [in] */ Int32 type,
        /* [in] */ Int64 triggerAtMillis,
        /* [in] */ Int64 intervalMillis,
        /* [in] */ IPendingIntent* operation);

    /**
     * Remove any alarms with a matching {@link Intent}.
     * Any alarm, of any type, whose Intent matches this one (as defined by
     * {@link Intent#filterEquals}), will be canceled.
     *
     * @param operation IntentSender which matches a previously added
     * IntentSender.
     *
     * @see #set
     */
    CARAPI Cancel(
        /* [in] */ IPendingIntent* operation);

    /**
     * Set the system wall clock time.
     * Requires the permission android.permission.SET_TIME.
     *
     * @param millis time in milliseconds since the Epoch
     */
    CARAPI SetTime(
        /* [in] */ Int64 millis);

    /**
     * Set the system default time zone.
     * Requires the permission android.permission.SET_TIME_ZONE.
     *
     * @param timeZone in the format understood by {@link java.util.TimeZone}
     */
    CARAPI SetTimeZone(
        /* [in] */ const String& timeZone);

private:
    AutoPtr<IIAlarmManager> mService;
};

}
}
}

#endif //__ELASTOS_DROID_APP_CALARMMANAGER_H__
