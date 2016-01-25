
#ifndef __ELASTOS_DROID_SERVER_ALARMMANAGERSERVICE_H__
#define __ELASTOS_DROID_SERVER_ALARMMANAGERSERVICE_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/content/BroadcastReceiver.h"
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/Pair.h>
#include "elastos/droid/os/Handler.h"

using Elastos::Droid::App::IActivity;
using Elastos::Droid::App::IActivityManager;
using Elastos::Droid::App::IActivityManagerNative;
using Elastos::Droid::App::IAlarmManager;
using Elastos::Droid::App::IAlarmClockInfo;
using Elastos::Droid::App::IIAlarmManager;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::IPowerManager;
using Elastos::Droid::Os::ISystemClock;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::IWorkSource;
using Elastos::Droid::Provider::Settings;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::format::IDateFormat;
using Elastos::Droid::Utility::IArrayMap;
using Elastos::Droid::Utility::ISparseArray;
using Elastos::Droid::Utility::ISparseBooleanArray;
using Elastos::Droid::Utility::ITimeUtils;

using Elastos::IO::IByteArrayOutputStream;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IPrintWriter;
using Elastos::Text::ISimpleDateFormat;
using Elastos::Utility::IArrayList;
using Elastos::Utility::IArrays;
using Elastos::Utility::ICalendar;
using Elastos::Utility::ICollections;
using Elastos::Utility::IComparator;
using Elastos::Utility::IDate;
using Elastos::Utility::IHashMap;
using Elastos::Utility::ILinkedList;
using Elastos::Utility::ILocale;
using Elastos::Utility::ITimeZone;

using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::Pair;

namespace Elastos {
namespace Droid {
namespace Server {


class AlarmManagerService
    : public SystemService
{
public:
    class BinderService
        : public Object
        , public IIAlarmManager
        , public IBinder
    {
    public:
        CAR_INTERFACE_DECL()

        //@Override
        CARAPI Set(
            /* [in] */ Int32 type,
            /* [in] */ Int64 triggerAtTime,
            /* [in] */ Int64 windowLength,
            /* [in] */ Int64 interval,
            /* [in] */ IPendingIntent* operation,
            /* [in] */ IWorkSource* workSource,
            /* [in] */ IAlarmClockInfo* alarmClock)
        {
            if (workSource != null) {
                GetContext()->EnforceCallingPermission(
                        android.Manifest.permission.UPDATE_DEVICE_STATS,
                        "AlarmManager.set");
            }

            setImpl(type, triggerAtTime, windowLength, interval, operation,
                    FALSE, workSource, alarmClock);
        }

        //@Override
        CARAPI SetTime(
            /* [in] */ Int64 millis,
            /* [out] */ Boolean* result)
        {
            GetContext()->EnforceCallingOrSelfPermission(
                    "android.permission.SET_TIME",
                    "setTime");

            if (mNativeData == 0) {
                Slogger::W(TAG, "Not setting time since no alarm driver is available.");
                return FALSE;
            }

            synchronized (mLock) {
                return setKernelTime(mNativeData, millis) == 0;
            }
        }

        //@Override
        CARAPI SetTimeZone(
            /* [in] */ String tz)
        {
            GetContext()->EnforceCallingOrSelfPermission(
                    "android.permission.SET_TIME_ZONE",
                    "SetTimeZone");

            Int64 oldId = Binder->ClearCallingIdentity();
            try {
                SetTimeZoneImpl(tz);
            } finally {
                Binder->RestoreCallingIdentity(oldId);
            }
        }

        //@Override
        CARAPI Remove(
            /* [in] */ IPendingIntent* operation)
        {
            removeImpl(operation);

        }

        //@Override
        CARAPI GetNextAlarmClock(
            /* [in] */ Int32 userId,
            /* [out] */IAlarmClockInfo** result)
        {
            userId = ActivityManager->HandleIncomingUser(Binder->GetCallingPid(),
                    Binder->GetCallingUid(), userId, FALSE /* allowAll */, FALSE /* requireFull */,
                    "getNextAlarmClock", null);

            return getNextAlarmClockImpl(userId);
        }

    protected:
        //@Override
        CARAPI Dump(
            /* [in] */ IFileDescriptor* fd,
            /* [in] */ IPrintWriter* pw,
            /* [in] */ ArrayOf<String>* args)
        {
            if (GetContext()->CheckCallingOrSelfPermission(android.Manifest.permission.DUMP)
                    != PackageManager.PERMISSION_GRANTED) {
                pw->Println("Permission Denial: can't dump AlarmManager from from pid="
                        + Binder->GetCallingPid()
                        + ", uid=" + Binder->GetCallingUid());
                return;
            }

            dumpImpl(pw);
        }
    };

private:
    class PriorityClass
        : public Object
    {
    public:
        Int32 mSeq;
        Int32 mPriority;

        PriorityClass()
        {
            mSeq = mCurrentSeq - 1;
            mPriority = PRIO_NORMAL;
        }
    };

    class WakeupEvent
        : public Object
    {
    public:
        Int64 mWhen;
        Int32 mUid;
        String mAction;

        WakeupEvent(
            /* [in] */ Int64 theTime,
            /* [in] */ Int32 theUid,
            /* [in] */ String theAction)
        {
            when = theTime;
            uid = theUid;
            action = theAction;
        }
    };

    class Batch
        : public Object
    {
    public:
        Int64 mStart;     // These endpoints are always in ELAPSED
        Int64 mEnd;
        Boolean mStandalone; // certain "batches" don't participate in coalescing

        AutoPtr<IArrayList> mAlarms; // new ArrayList<Alarm>();

        Batch()
        {
            start = 0;
            end = Long.MAX_VALUE;
        }

        Batch(
            /* [in] */ Alarm* seed)
        {
            start = seed.whenElapsed;
            end = seed.maxWhen;
            alarms->Add(seed);
        }

        Int32 Size()
        {
            return alarms->Size();
        }

        AutoPtr<Alarm> Get(
            /* [in] */ Int32 index)
        {
            return alarms->Get(index);
        }

        Boolean CanHold(
            /* [in] */ Int64 whenElapsed,
            /* [in] */ Int64 maxWhen)
        {
            return (end >= whenElapsed) && (start <= maxWhen);
        }

        Boolean Add(
            /* [in] */ Alarm* alarm)
        {
            Boolean newStart = FALSE;
            // narrows the batch if necessary; presumes that CanHold(alarm) is TRUE
            Int32 index = Collections->BinarySearch(alarms, alarm, sIncreasingTimeOrder);
            if (index < 0) {
                index = 0 - index - 1;
            }
            alarms->Add(index, alarm);
            if (DEBUG_BATCH) {
                Slog->V(TAG, "Adding " + alarm + " to " + this);
            }
            if (alarm.whenElapsed > start) {
                start = alarm.whenElapsed;
                newStart = TRUE;
            }
            if (alarm.maxWhen < end) {
                end = alarm.maxWhen;
            }

            if (DEBUG_BATCH) {
                Slog->V(TAG, "    => now " + this);
            }
            return newStart;
        }

        Boolean Remove(
            /* [in] */ IPendingIntent* operation)
        {
            Boolean didRemove = FALSE;
            Int64 newStart = 0;  // recalculate endpoints as we go
            Int64 newEnd = Long.MAX_VALUE;
            for (Int32 i = 0; i < alarms->Size(); ) {
                Alarm alarm = alarms->Get(i);
                if (alarm.operation->Equals(operation)) {
                    alarms->Remove(i);
                    didRemove = TRUE;
                    if (alarm.alarmClock != null) {
                        mNextAlarmClockMayChange = TRUE;
                    }
                } else {
                    if (alarm.whenElapsed > newStart) {
                        newStart = alarm.whenElapsed;
                    }
                    if (alarm.maxWhen < newEnd) {
                        newEnd = alarm.maxWhen;
                    }
                    i++;
                }
            }
            if (didRemove) {
                // commit the new batch bounds
                start = newStart;
                end = newEnd;
            }
            return didRemove;
        }

        Boolean Remove(
            /* [in] */ String packageName)
        {
            Boolean didRemove = FALSE;
            Int64 newStart = 0;  // recalculate endpoints as we go
            Int64 newEnd = Long.MAX_VALUE;
            for (Int32 i = 0; i < alarms->Size(); ) {
                Alarm alarm = alarms->Get(i);
                if (alarm.operation->GetTargetPackage()->Equals(packageName)) {
                    alarms->Remove(i);
                    didRemove = TRUE;
                    if (alarm.alarmClock != null) {
                        mNextAlarmClockMayChange = TRUE;
                    }
                } else {
                    if (alarm.whenElapsed > newStart) {
                        newStart = alarm.whenElapsed;
                    }
                    if (alarm.maxWhen < newEnd) {
                        newEnd = alarm.maxWhen;
                    }
                    i++;
                }
            }
            if (didRemove) {
                // commit the new batch bounds
                start = newStart;
                end = newEnd;
            }
            return didRemove;
        }

        Boolean Remove(
            /* [in] */ Int32 userHandle)
        {
            Boolean didRemove = FALSE;
            Int64 newStart = 0;  // recalculate endpoints as we go
            Int64 newEnd = Long.MAX_VALUE;
            for (Int32 i = 0; i < alarms->Size(); ) {
                Alarm alarm = alarms->Get(i);
                if (UserHandle->GetUserId(alarm.operation->GetCreatorUid()) == userHandle) {
                    alarms->Remove(i);
                    didRemove = TRUE;
                    if (alarm.alarmClock != null) {
                        mNextAlarmClockMayChange = TRUE;
                    }
                } else {
                    if (alarm.whenElapsed > newStart) {
                        newStart = alarm.whenElapsed;
                    }
                    if (alarm.maxWhen < newEnd) {
                        newEnd = alarm.maxWhen;
                    }
                    i++;
                }
            }
            if (didRemove) {
                // commit the new batch bounds
                start = newStart;
                end = newEnd;
            }
            return didRemove;
        }

        Boolean HasPackage(
            /* [in] */ String packageName)
        {
            Int32 N = alarms->Size();
            for (Int32 i = 0; i < N; i++) {
                Alarm a = alarms->Get(i);
                if (a.operation->GetTargetPackage()->Equals(packageName)) {
                    return TRUE;
                }
            }
            return FALSE;
        }

        Boolean HasWakeups()
        {
            Int32 N = alarms->Size();
            for (Int32 i = 0; i < N; i++) {
                Alarm a = alarms->Get(i);
                // non-wakeup alarms are types 1 and 3, i.e. have the low bit set
                if ((a.type & TYPE_NONWAKEUP_MASK) == 0) {
                    return TRUE;
                }
            }
            return FALSE;
        }

        //@Override
        CARAPI ToString(
            /* [out] */ String* result)
        {
            StringBuilder b = new StringBuilder(40);
            b->Append("Batch{"); b->Append(Integer->ToHexString(this->HashCode()));
            b->Append(" num="); b->Append(size());
            b->Append(" start="); b->Append(start);
            b->Append(" end="); b->Append(end);
            if (standalone) {
                b->Append(" STANDALONE");
            }
            b->Append('}');
            return b->ToString();
        }
    };

    class BatchTimeOrder
        : public Object
        , public IComparator  //<Batch>
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Compare(
            /* [in] */ IInterface* o1, //Batch b1,
            /* [in] */ IInterface* o2, //Batch b2
            /* [out] */ Int32* result)
        {
            Int64 when1 = b1.start;
            Int64 when2 = b2.start;
            if (when1 - when2 > 0) {
                return 1;
            }
            if (when1 - when2 < 0) {
                return -1;
            }
            return 0;
        }
    };

    class AlarmDispatchComparator
        : public Object
        , public IComparator
    {
        CAR_INTERFACE_DECL()

        //@Override
        CARAPI Compare(
            /* [in] */ IInterface* o1, //Alarm b1,
            /* [in] */ IInterface* o2, //Alarm b2
            /* [out] */ Int32* result)
        {
            // priority class trumps everything.  TICK < WAKEUP < NORMAL
            if (lhs.priorityClass.priority < rhs.priorityClass.priority) {
                return -1;
            } else if (lhs.priorityClass.priority > rhs.priorityClass.priority) {
                return 1;
            }

            // within each class, sort by nominal delivery time
            if (lhs.whenElapsed < rhs.whenElapsed) {
                return -1;
            } else if (lhs.whenElapsed > rhs.whenElapsed) {
                return 1;
            }

            // same priority class + same target delivery time
            return 0;
        }
    };


    class AlarmThread
        : public Thread
    {
    public:
        AlarmThread()
        {
            Thread::constructor("AlarmManager");
        }

        CARAPI Run()
        {
            ArrayList<Alarm> triggerList = new ArrayList<Alarm>();

            while (TRUE)
            {
                Int32 result = waitForAlarm(mNativeData);

                triggerList->Clear();

                if ((result & TIME_CHANGED_MASK) != 0) {
                    if (DEBUG_BATCH) {
                        Slog->V(TAG, "Time changed notification from kernel; rebatching");
                    }
                    removeImpl(mTimeTickSender);
                    rebatchAllAlarms();
                    mClockReceiver->ScheduleTimeTickEvent();
                    synchronized (mLock) {
                        mNumTimeChanged++;
                    }
                    Intent intent = new Intent(Intent.ACTION_TIME_CHANGED);
                    intent->AddFlags(Intent.FLAG_RECEIVER_REPLACE_PENDING
                            | Intent.FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
                    GetContext()->SendBroadcastAsUser(intent, UserHandle.ALL);
                }

                synchronized (mLock) {
                    Int64 nowRTC = System->CurrentTimeMillis();
                    Int64 nowELAPSED = SystemClock->ElapsedRealtime();
                    if (localLOGV) Slog->V(
                        TAG, "Checking for alarms... rtc=" + nowRTC
                        + ", elapsed=" + nowELAPSED);

                    if (WAKEUP_STATS) {
                        if ((result & IS_WAKEUP_MASK) != 0) {
                            Int64 newEarliest = nowRTC - RECENT_WAKEUP_PERIOD;
                            Int32 n = 0;
                            for (WakeupEvent event : mRecentWakeups) {
                                if (event.when > newEarliest) break;
                                n++; // number of now-stale entries at the list head
                            }
                            for (Int32 i = 0; i < n; i++) {
                                mRecentWakeups->Remove();
                            }

                            recordWakeupAlarms(mAlarmBatches, nowELAPSED, nowRTC);
                        }
                    }

                    Boolean hasWakeup = triggerAlarmsLocked(triggerList, nowELAPSED, nowRTC);
                    if (!hasWakeup && checkAllowNonWakeupDelayLocked(nowELAPSED)) {
                        // if there are no wakeup alarms and the screen is off, we can
                        // delay what we have so far until the future.
                        if (mPendingNonWakeupAlarms->Size() == 0) {
                            mStartCurrentDelayTime = nowELAPSED;
                            mNextNonWakeupDeliveryTime = nowELAPSED
                                    + ((currentNonWakeupFuzzLocked(nowELAPSED)*3)/2);
                        }
                        mPendingNonWakeupAlarms->AddAll(triggerList);
                        mNumDelayedAlarms += triggerList->Size();
                        rescheduleKernelAlarmsLocked();
                        updateNextAlarmClockLocked();
                    } else {
                        // now deliver the alarm intents; if there are pending non-wakeup
                        // alarms, we need to merge them in to the list.  note we don't
                        // just deliver them first because we generally want non-wakeup
                        // alarms delivered after wakeup alarms.
                        rescheduleKernelAlarmsLocked();
                        updateNextAlarmClockLocked();
                        if (mPendingNonWakeupAlarms->Size() > 0) {
                            CalculateDeliveryPriorities(mPendingNonWakeupAlarms);
                            triggerList->AddAll(mPendingNonWakeupAlarms);
                            Collections->Sort(triggerList, mAlarmDispatchComparator);
                            Int64 thisDelayTime = nowELAPSED - mStartCurrentDelayTime;
                            mTotalDelayTime += thisDelayTime;
                            if (mMaxDelayTime < thisDelayTime) {
                                mMaxDelayTime = thisDelayTime;
                            }
                            mPendingNonWakeupAlarms->Clear();
                        }
                        deliverAlarmsLocked(triggerList, nowELAPSED);
                    }
                }
            }
        }
    };

    class AlarmHandler
        : public Handler
    {
    public:
        AlarmHandler()
        {
        }

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg)
        {
            if (msg.what == ALARM_EVENT) {
                ArrayList<Alarm> triggerList = new ArrayList<Alarm>();
                synchronized (mLock) {
                    Int64 nowRTC = System->CurrentTimeMillis();
                    Int64 nowELAPSED = SystemClock->ElapsedRealtime();
                    triggerAlarmsLocked(triggerList, nowELAPSED, nowRTC);
                    updateNextAlarmClockLocked();
                }

                // now trigger the alarms without the lock held
                for (Int32 i=0; i<triggerList->Size(); i++) {
                    Alarm alarm = triggerList->Get(i);
                    try {
                        alarm.operation->Send();
                    } catch (PendingIntent.CanceledException e) {
                        if (alarm.repeatInterval > 0) {
                            // This IntentSender is no longer valid, but this
                            // is a repeating alarm, so toss the hoser.
                            removeImpl(alarm.operation);
                        }
                    }
                }
            } else if (msg.what == SEND_NEXT_ALARM_CLOCK_CHANGED) {
                sendNextAlarmClockChanged();
            }
        }

    public:
        static const Int32 ALARM_EVENT = 1;
        static const Int32 MINUTE_CHANGE_EVENT = 2;
        static const Int32 DATE_CHANGE_EVENT = 3;
        static const Int32 SEND_NEXT_ALARM_CLOCK_CHANGED = 4;

    };

    class ClockReceiver
        : public BroadcastReceiver
    {
        ClockReceiver()
        {
            IntentFilter filter = new IntentFilter();
            filter->AddAction(Intent.ACTION_TIME_TICK);
            filter->AddAction(Intent.ACTION_DATE_CHANGED);
            GetContext()->RegisterReceiver(this, filter);
        }

        //@Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent)
        {
            if (intent->GetAction()->Equals(Intent.ACTION_TIME_TICK)) {
                if (DEBUG_BATCH) {
                    Slog->V(TAG, "Received TIME_TICK alarm; rescheduling");
                }
                ScheduleTimeTickEvent();
            } else if (intent->GetAction()->Equals(Intent.ACTION_DATE_CHANGED)) {
                // Since the kernel does not keep track of DST, we need to
                // reset the TZ information at the beginning of each day
                // based off of the current Zone gmt offset + userspace tracked
                // daylight savings information.
                TimeZone zone = TimeZone->GetTimeZone(SystemProperties->Get(TIMEZONE_PROPERTY));
                Int32 gmtOffset = zone->GetOffset(System->CurrentTimeMillis());
                setKernelTimezone(mNativeData, -(gmtOffset / 60000));
                ScheduleDateChangedEvent();
            }
        }

        CARAPI ScheduleTimeTickEvent()
        {
            Int64 currentTime = System->CurrentTimeMillis();
            Int64 nextTime = 60000 * ((currentTime / 60000) + 1);

            // Schedule this event for the amount of time that it would take to get to
            // the top of the next minute.
            Int64 tickEventDelay = nextTime - currentTime;

            WorkSource workSource = null; // Let system take blame for time tick events.
            setImpl(ELAPSED_REALTIME, SystemClock->ElapsedRealtime() + tickEventDelay, 0,
                    0, mTimeTickSender, TRUE, workSource, null);
        }

        CARAPI ScheduleDateChangedEvent()
        {
            Calendar calendar = Calendar->GetInstance();
            calendar->SetTimeInMillis(System->CurrentTimeMillis());
            calendar->Set(Calendar.HOUR, 0);
            calendar->Set(Calendar.MINUTE, 0);
            calendar->Set(Calendar.SECOND, 0);
            calendar->Set(Calendar.MILLISECOND, 0);
            calendar->Add(Calendar.DAY_OF_MONTH, 1);

            WorkSource workSource = null; // Let system take blame for date change events.
            setImpl(RTC, calendar->GetTimeInMillis(), 0, 0, mDateChangeSender, TRUE, workSource,
                    null);
        }
    };

    class InteractiveStateReceiver
        : public BroadcastReceiver
    {
        InteractiveStateReceiver()
        {
            IntentFilter filter = new IntentFilter();
            filter->AddAction(Intent.ACTION_SCREEN_OFF);
            filter->AddAction(Intent.ACTION_SCREEN_ON);
            filter->SetPriority(IntentFilter.SYSTEM_HIGH_PRIORITY);
            GetContext()->RegisterReceiver(this, filter);
        }

        //@Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent)
        {
            synchronized (mLock) {
                interactiveStateChangedLocked(Intent.ACTION_SCREEN_ON->Equals(intent->GetAction()));
            }
        }
    };

    class UninstallReceiver
        : public BroadcastReceiver
    {
        UninstallReceiver() {
            IntentFilter filter = new IntentFilter();
            filter->AddAction(Intent.ACTION_PACKAGE_REMOVED);
            filter->AddAction(Intent.ACTION_PACKAGE_RESTARTED);
            filter->AddAction(Intent.ACTION_QUERY_PACKAGE_RESTART);
            filter->AddDataScheme("package");
            GetContext()->RegisterReceiver(this, filter);
             // Register for events related to sdcard installation.
            IntentFilter sdFilter = new IntentFilter();
            sdFilter->AddAction(Intent.ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE);
            sdFilter->AddAction(Intent.ACTION_USER_STOPPED);
            GetContext()->RegisterReceiver(this, sdFilter);
        }

        //@Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent)
        {
            synchronized (mLock) {
                String action = intent->GetAction();
                String pkgList[] = null;
                if (Intent.ACTION_QUERY_PACKAGE_RESTART->Equals(action)) {
                    pkgList = intent->GetStringArrayExtra(Intent.EXTRA_PACKAGES);
                    for (String packageName : pkgList) {
                        if (lookForPackageLocked(packageName)) {
                            setResultCode(Activity.RESULT_OK);
                            return;
                        }
                    }
                    return;
                } else if (Intent.ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE->Equals(action)) {
                    pkgList = intent->GetStringArrayExtra(Intent.EXTRA_CHANGED_PACKAGE_LIST);
                } else if (Intent.ACTION_USER_STOPPED->Equals(action)) {
                    Int32 userHandle = intent->GetIntExtra(Intent.EXTRA_USER_HANDLE, -1);
                    if (userHandle >= 0) {
                        removeUserLocked(userHandle);
                    }
                } else {
                    if (Intent.ACTION_PACKAGE_REMOVED->Equals(action)
                            && intent->GetBooleanExtra(Intent.EXTRA_REPLACING, FALSE)) {
                        // This package is being updated; don't kill its alarms.
                        return;
                    }
                    Uri data = intent->GetData();
                    if (data != null) {
                        String pkg = data->GetSchemeSpecificPart();
                        if (pkg != null) {
                            pkgList = new String[]{pkg};
                        }
                    }
                }
                if (pkgList != null && (pkgList.length > 0)) {
                    for (String pkg : pkgList) {
                        removeLocked(pkg);
                        mPriorities->Remove(pkg);
                        for (Int32 i=mBroadcastStats->Size()-1; i>=0; i--) {
                            ArrayMap<String, BroadcastStats> uidStats = mBroadcastStats->ValueAt(i);
                            if (uidStats->Remove(pkg) != null) {
                                if (uidStats->Size() <= 0) {
                                    mBroadcastStats->RemoveAt(i);
                                }
                            }
                        }
                    }
                }
            }
        }
    };

    class ResultReceiver
        : public Object
        , public IPendingIntentOnFinished
    {

        CAR_INTERFACE_DECL()

        CARAPI OnSendFinished(
            /* [in] */ IPendingIntent* pi,
            /* [in] */ IIntent* intent,
            /* [in] */ Int32 resultCode,
            /* [in] */ String resultData,
            /* [in] */ IBundle* resultExtras)
        {
            synchronized (mLock) {
                InFlight inflight = null;
                for (Int32 i=0; i<mInFlight->Size(); i++) {
                    if (mInFlight->Get(i).mPendingIntent == pi) {
                        inflight = mInFlight->Remove(i);
                        break;
                    }
                }
                if (inflight != null) {
                    Int64 nowELAPSED = SystemClock->ElapsedRealtime();
                    BroadcastStats bs = inflight.mBroadcastStats;
                    bs.nesting--;
                    if (bs.nesting <= 0) {
                        bs.nesting = 0;
                        bs.aggregateTime += nowELAPSED - bs.startTime;
                    }
                    FilterStats fs = inflight.mFilterStats;
                    fs.nesting--;
                    if (fs.nesting <= 0) {
                        fs.nesting = 0;
                        fs.aggregateTime += nowELAPSED - fs.startTime;
                    }
                } else {
                    mLog->W("No in-flight alarm for " + pi + " " + intent);
                }
                mBroadcastRefCount--;
                if (mBroadcastRefCount == 0) {
                    mWakeLock->Release();
                    if (mInFlight->Size() > 0) {
                        mLog->W("Finished all broadcasts with " + mInFlight->Size()
                                + " remaining inflights");
                        for (Int32 i=0; i<mInFlight->Size(); i++) {
                            mLog->W("  Remaining #" + i + ": " + mInFlight->Get(i));
                        }
                        mInFlight->Clear();
                    }
                } else {
                    // the next of our alarms is now in flight.  reattribute the wakelock.
                    if (mInFlight->Size() > 0) {
                        InFlight inFlight = mInFlight->Get(0);
                        setWakelockWorkSource(inFlight.mPendingIntent, inFlight.mWorkSource,
                                inFlight.mAlarmType, inFlight.mTag, FALSE);
                    } else {
                        // should never happen
                        mLog->W("Alarm wakelock still held but sent queue empty");
                        mWakeLock->SetWorkSource(null);
                    }
                }
            }
        }
    };

    /**
     * This Comparator sorts Alarms into increasing time order.
     */
    class IncreasingTimeOrder
        : public Object
        , public IComparator
    {
        CAR_INTERFACE_DECL()

        CARAPI Compare(
            /* [in] */ IInterface* o1,
            /* [in] */ IInterface* o2,
            /* [out] */ Int32* result)
        {
            Int64 when1 = a1.when;
            Int64 when2 = a2.when;
            if (when1 - when2 > 0) {
                return 1;
            }
            if (when1 - when2 < 0) {
                return -1;
            }
            return 0;
        }
    };

    class Alarm
        : public Object
    {
    public:
        Int32 mType;
        Boolean mWakeup;
        AutoPtr<IPendingIntent> mOperation;
        String mTag;
        AutoPtr<IWorkSource> mWorkSource;
        Int32 mCount;
        Int64 mWhen;
        Int64 mWindowLength;
        Int64 mWhenElapsed;    // 'when' in the elapsed time base
        Int64 mMaxWhen;        // also in the elapsed time base
        Int64 mRepeatInterval;
        AutoPtr<IAlarmClockInfo> mAlarmClock;
        Int32 mUserId;
        AutoPtr<PriorityClass> priorityClass;

    public:
        Alarm(
            /* [in] */ Int32 _type,
            /* [in] */ Int64 _when,
            /* [in] */ Int64 _whenElapsed,
            /* [in] */ Int64 _windowLength,
            /* [in] */ Int64 _maxWhen,
            /* [in] */ Int64 _interval,
            /* [in] */ IPendingIntent* _op,
            /* [in] */ IWorkSource* _ws,
            /* [in] */ IAlarmClockInfo* _info,
            /* [in] */ Int32 _userId)
        {
            type = _type;
            wakeup = _type == AlarmManager.ELAPSED_REALTIME_WAKEUP
                    || _type == AlarmManager.RTC_WAKEUP;
            when = _when;
            whenElapsed = _whenElapsed;
            windowLength = _windowLength;
            maxWhen = _maxWhen;
            repeatInterval = _interval;
            operation = _op;
            tag = makeTag(_op, _type);
            workSource = _ws;
            alarmClock = _info;
            userId = _userId;
        }

        static String MakeTag(
            /* [in] */ IPendingIntent* pi,
            /* [in] */ Int32 type)
        {
            return pi->GetTag(type == ELAPSED_REALTIME_WAKEUP || type == RTC_WAKEUP
                    ? "*walarm*:" : "*alarm*:");
        }

        //@Override
        CARAPI ToString(
            /* [out] */ String* str)
        {
            StringBuilder sb = new StringBuilder(128);
            sb->Append("Alarm{");
            sb->Append(Integer->ToHexString(System->IdentityHashCode(this)));
            sb->Append(" type ");
            sb->Append(type);
            sb->Append(" when ");
            sb->Append(when);
            sb->Append(" ");
            sb->Append(operation->GetTargetPackage());
            sb->Append('}');
            return sb->ToString();
        }

        CARAPI Dump(
            /* [in] */ IPrintWriter* pw,
            /* [in] */ String prefix,
            /* [in] */ Int64 nowRTC,
            /* [in] */ Int64 nowELAPSED,
            /* [in] */ ISimpleDateFormat* sdf)
        {
            Boolean isRtc = (type == RTC || type == RTC_WAKEUP);
            pw->Print(prefix); pw->Print("tag="); pw->Println(tag);
            pw->Print(prefix); pw->Print("type="); pw->Print(type);
                    pw->Print(" whenElapsed="); TimeUtils->FormatDuration(whenElapsed,
                            nowELAPSED, pw);
                    if (isRtc) {
                        pw->Print(" when="); pw->Print(sdf->Format(new Date(when)));
                    } else {
                        pw->Print(" when="); TimeUtils->FormatDuration(when, nowELAPSED, pw);
                    }
                    pw->Println();
            pw->Print(prefix); pw->Print("window="); pw->Print(windowLength);
                    pw->Print(" repeatInterval="); pw->Print(repeatInterval);
                    pw->Print(" count="); pw->Println(count);
            pw->Print(prefix); pw->Print("operation="); pw->Println(operation);
        }
    };

    friend class FilterStats;
    friend class BroadcastStats;

    class InFlight
        : public Intent
    {
    public:
        AutoPtr<IPendingIntent> mPendingIntent;
        AutoPtr<IWorkSource> mWorkSource;
        String mTag;
        AutoPtr<BroadcastStats> mBroadcastStats;
        AutoPtr<FilterStats> mFilterStats;
        Int32 mAlarmType;

        InFlight(
            /* [in] */ AlarmManagerService* service,
            /* [in] */ IPendingIntent* pendingIntent,
            /* [in] */ IWorkSource* workSource,
            /* [in] */ Int32 alarmType,
            /* [in] */ String tag)
        {
            mPendingIntent = pendingIntent;
            mWorkSource = workSource;
            mTag = tag;
            mBroadcastStats = service->GetStatsLocked(pendingIntent);
            FilterStats fs = mBroadcastStats.filterStats->Get(mTag);
            if (fs == null) {
                fs = new FilterStats(mBroadcastStats, mTag);
                mBroadcastStats.filterStats->Put(mTag, fs);
            }
            mFilterStats = fs;
            mAlarmType = alarmType;
        }
    };

    class FilterStats
        : public Object
    {
    public:
        AutoPtr<BroadcastStats> mBroadcastStats;
        String mTag;

        Int64 aggregateTime;
        Int32 count;
        Int32 numWakeup;
        Int64 startTime;
        Int32 nesting;

        FilterStats(BroadcastStats broadcastStats, String tag) {
            mBroadcastStats = broadcastStats;
            mTag = tag;
        }
    };

    class BroadcastStats
        : public Object
    {
    public:
        Int32 mUid;
        String mPackageName;

        Int64 mAggregateTime;
        Int32 mCount;
        Int32 mNumWakeup;
        Int64 mStartTime;
        Int32 mNesting;
        AutoPtr<IArrayMap> mFilterStats;// new ArrayMap<String, FilterStats>();

        BroadcastStats(
            /* [in] */ Int32 uid,
            /* [in] */ String packageName)
        {
            mUid = uid;
            mPackageName = packageName;
        }
    };

public:

    CARAPI CalculateDeliveryPriorities(
        /* [in] */ IArrayList* alarms)
    {
        Int32 N = alarms->Size();
        for (Int32 i = 0; i < N; i++) {
            Alarm a = alarms->Get(i);

            Int32 alarmPrio;
            if (Intent.ACTION_TIME_TICK->Equals(a.operation->GetIntent()->GetAction())) {
                alarmPrio = PRIO_TICK;
            } else if (a.wakeup) {
                alarmPrio = PRIO_WAKEUP;
            } else {
                alarmPrio = PRIO_NORMAL;
            }

            PriorityClass packagePrio = a.priorityClass;
            if (packagePrio == null) packagePrio = mPriorities->Get(a.operation->GetCreatorPackage());
            if (packagePrio == null) {
                packagePrio = a.priorityClass = new PriorityClass(); // lowest prio & stale sequence
                mPriorities->Put(a.operation->GetCreatorPackage(), packagePrio);
            }
            a.priorityClass = packagePrio;

            if (packagePrio.seq != mCurrentSeq) {
                // first alarm we've seen in the current delivery generation from this package
                packagePrio.priority = alarmPrio;
                packagePrio.seq = mCurrentSeq;
            } else {
                // Multiple alarms from this package being delivered in this generation;
                // bump the package's delivery class if it's warranted.
                // TICK < WAKEUP < NORMAL
                if (alarmPrio < packagePrio.priority) {
                    packagePrio.priority = alarmPrio;
                }
            }
        }
    }

    CAR_INTERFACE_DECL()

    AlarmManagerService();

    CARAPI constructor(
        /* [in] */ IContext* context)
    {
        super(context);
    }

    static Int64 ConvertToElapsed(
        /* [in] */ Int64 when,
        /* [in] */ Int32 type)
    {
        Boolean isRtc = (type == RTC || type == RTC_WAKEUP);
        if (isRtc) {
            when -= System->CurrentTimeMillis() - SystemClock->ElapsedRealtime();
        }
        return when;
    }

    // Apply a heuristic to { recurrence interval, futurity of the trigger time } to
    // calculate the end of our nominal delivery window for the alarm.
    static Int64 maxTriggerTime(
        /* [in] */ Int64 now,
        /* [in] */ Int64 triggerAtTime,
        /* [in] */ Int64 interval)
    {
        // Current heuristic: batchable window is 75% of either the recurrence interval
        // [for a periodic alarm] or of the time from now to the desired delivery time,
        // with a minimum delay/interval of 10 seconds, under which we will simply not
        // defer the alarm.
        Int64 futurity = (interval == 0)
                ? (triggerAtTime - now)
                : interval;
        if (futurity < MIN_FUZZABLE_INTERVAL) {
            futurity = 0;
        }
        return triggerAtTime + (Int64)(.75 * futurity);
    }

    // returns TRUE if the batch was added at the head
    static Boolean addBatchLocked(
        /* [in] */ ArrayList<Batch> list,
        /* [in] */ Batch* newBatch)
    {
        Int32 index = Collections->BinarySearch(list, newBatch, sBatchOrder);
        if (index < 0) {
            index = 0 - index - 1;
        }
        list->Add(index, newBatch);
        return (index == 0);
    }

    // Return the index of the matching batch, or -1 if none found.
    Int32 attemptCoalesceLocked(
        /* [in] */ Int64 whenElapsed,
        /* [in] */ Int64 maxWhen)
    {
        Int32 N = mAlarmBatches->Size();
        for (Int32 i = 0; i < N; i++) {
            Batch b = mAlarmBatches->Get(i);
            if (!b.standalone && b->CanHold(whenElapsed, maxWhen)) {
                return i;
            }
        }
        return -1;
    }

    // The RTC clock has moved arbitrarily, so we need to recalculate all the batching
    void rebatchAllAlarms()
    {
        synchronized (mLock) {
            rebatchAllAlarmsLocked(TRUE);
        }
    }

    void rebatchAllAlarmsLocked(
        /* [in] */ Boolean doValidate)
    {
        ArrayList<Batch> oldSet = (ArrayList<Batch>) mAlarmBatches->Clone();
        mAlarmBatches->Clear();
        Int64 nowElapsed = SystemClock->ElapsedRealtime();
        Int32 oldBatches = oldSet->Size();
        for (Int32 batchNum = 0; batchNum < oldBatches; batchNum++) {
            Batch batch = oldSet->Get(batchNum);
            Int32 N = batch->Size();
            for (Int32 i = 0; i < N; i++) {
                Alarm a = batch->Get(i);
                Int64 whenElapsed = ConvertToElapsed(a.when, a.type);
                Int64 maxElapsed;
                if (a.whenElapsed == a.maxWhen) {
                    // Exact
                    maxElapsed = whenElapsed;
                } else {
                    // Not exact.  Preserve any explicit window, otherwise recalculate
                    // the window based on the alarm's new futurity.  Note that this
                    // reflects a policy of preferring timely to deferred delivery.
                    maxElapsed = (a.windowLength > 0)
                            ? (whenElapsed + a.windowLength)
                            : maxTriggerTime(nowElapsed, whenElapsed, a.repeatInterval);
                }
                setImplLocked(a.type, a.when, whenElapsed, a.windowLength, maxElapsed,
                        a.repeatInterval, a.operation, batch.standalone, doValidate, a.workSource,
                        a.alarmClock, a.userId);
            }
        }
    }

    //@Override
    CARAPI onStart() {
        mNativeData = init();
        mNextWakeup = mNextNonWakeup = 0;

        // We have to set current TimeZone info to kernel
        // because kernel doesn't keep this after reboot
        SetTimeZoneImpl(SystemProperties->Get(TIMEZONE_PROPERTY));

        PowerManager pm = (PowerManager) GetContext()->GetSystemService(Context.POWER_SERVICE);
        mWakeLock = pm->NewWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "*alarm*");

        mTimeTickSender = PendingIntent->GetBroadcastAsUser(GetContext(), 0,
                new Intent(Intent.ACTION_TIME_TICK).addFlags(
                        Intent.FLAG_RECEIVER_REGISTERED_ONLY
                        | Intent.FLAG_RECEIVER_FOREGROUND), 0,
                        UserHandle.ALL);
        Intent intent = new Intent(Intent.ACTION_DATE_CHANGED);
        intent->AddFlags(Intent.FLAG_RECEIVER_REPLACE_PENDING);
        mDateChangeSender = PendingIntent->GetBroadcastAsUser(GetContext(), 0, intent,
                Intent.FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT, UserHandle.ALL);

        // now that we have initied the driver schedule the alarm
        mClockReceiver = new ClockReceiver();
        mClockReceiver->ScheduleTimeTickEvent();
        mClockReceiver->ScheduleDateChangedEvent();
        mInteractiveStateReceiver = new InteractiveStateReceiver();
        mUninstallReceiver = new UninstallReceiver();

        if (mNativeData != 0) {
            AlarmThread waitThread = new AlarmThread();
            waitThread->Start();
        } else {
            Slogger::W(TAG, "Failed to open alarm driver. Falling back to a handler.");
        }

        publishBinderService(Context.ALARM_SERVICE, mService);
    }

    //@Override
    protected void finalize() throws Throwable {
        try {
            close(mNativeData);
        } finally {
            super->Finalize();
        }
    }

    void SetTimeZoneImpl(String tz) {
        if (TextUtils->IsEmpty(tz)) {
            return;
        }

        TimeZone zone = TimeZone->GetTimeZone(tz);
        // Prevent reentrant calls from stepping on each other when writing
        // the time zone property
        Boolean timeZoneWasChanged = FALSE;
        synchronized (this) {
            String current = SystemProperties->Get(TIMEZONE_PROPERTY);
            if (current == null || !current->Equals(zone->GetID())) {
                if (localLOGV) {
                    Slog->V(TAG, "timezone changed: " + current + ", new=" + zone->GetID());
                }
                timeZoneWasChanged = TRUE;
                SystemProperties->Set(TIMEZONE_PROPERTY, zone->GetID());
            }

            // Update the kernel timezone information
            // Kernel tracks time offsets as 'minutes west of GMT'
            Int32 gmtOffset = zone->GetOffset(System->CurrentTimeMillis());
            setKernelTimezone(mNativeData, -(gmtOffset / 60000));
        }

        TimeZone->SetDefault(null);

        if (timeZoneWasChanged) {
            Intent intent = new Intent(Intent.ACTION_TIMEZONE_CHANGED);
            intent->AddFlags(Intent.FLAG_RECEIVER_REPLACE_PENDING);
            intent->PutExtra("time-zone", zone->GetID());
            GetContext()->SendBroadcastAsUser(intent, UserHandle.ALL);
        }
    }

    void removeImpl(PendingIntent operation) {
        if (operation == null) {
            return;
        }
        synchronized (mLock) {
            removeLocked(operation);
        }
    }

    void setImpl(Int32 type, Int64 triggerAtTime, Int64 windowLength, Int64 interval,
            PendingIntent operation, Boolean isStandalone, WorkSource workSource,
            AlarmManager.AlarmClockInfo alarmClock) {
        if (operation == null) {
            Slogger::W(TAG, "set/setRepeating ignored because there is no intent");
            return;
        }

        // Sanity check the window length.  This will catch people mistakenly
        // trying to pass an end-of-window timestamp rather than a duration.
        if (windowLength > AlarmManager.INTERVAL_HALF_DAY) {
            Slogger::W(TAG, "Window length " + windowLength
                    + "ms suspiciously Int64; limiting to 1 hour");
            windowLength = AlarmManager.INTERVAL_HOUR;
        }

        if (type < RTC_WAKEUP || type > ELAPSED_REALTIME) {
            throw new IllegalArgumentException("Invalid alarm type " + type);
        }

        if (triggerAtTime < 0) {
            Int64 who = Binder->GetCallingUid();
            Int64 what = Binder->GetCallingPid();
            Slogger::W(TAG, "Invalid alarm trigger time! " + triggerAtTime + " from uid=" + who
                    + " pid=" + what);
            triggerAtTime = 0;
        }

        Int64 nowElapsed = SystemClock->ElapsedRealtime();
        Int64 triggerElapsed = ConvertToElapsed(triggerAtTime, type);
        Int64 maxElapsed;
        if (windowLength == AlarmManager.WINDOW_EXACT) {
            maxElapsed = triggerElapsed;
        } else if (windowLength < 0) {
            maxElapsed = maxTriggerTime(nowElapsed, triggerElapsed, interval);
        } else {
            maxElapsed = triggerElapsed + windowLength;
        }

        Int32 userId = UserHandle->GetCallingUserId();

        synchronized (mLock) {
            if (DEBUG_BATCH) {
                Slog->V(TAG, "set(" + operation + ") : type=" + type
                        + " triggerAtTime=" + triggerAtTime + " win=" + windowLength
                        + " tElapsed=" + triggerElapsed + " maxElapsed=" + maxElapsed
                        + " interval=" + interval + " standalone=" + isStandalone);
            }
            setImplLocked(type, triggerAtTime, triggerElapsed, windowLength, maxElapsed,
                    interval, operation, isStandalone, TRUE, workSource, alarmClock, userId);
        }
    }

    private void setImplLocked(Int32 type, Int64 when, Int64 whenElapsed, Int64 windowLength,
            Int64 maxWhen, Int64 interval, PendingIntent operation, Boolean isStandalone,
            Boolean doValidate, WorkSource workSource, AlarmManager.AlarmClockInfo alarmClock,
            Int32 userId) {
        Alarm a = new Alarm(type, when, whenElapsed, windowLength, maxWhen, interval,
                operation, workSource, alarmClock, userId);
        removeLocked(operation);

        Int32 whichBatch = (isStandalone) ? -1 : attemptCoalesceLocked(whenElapsed, maxWhen);
        if (whichBatch < 0) {
            Batch batch = new Batch(a);
            batch.standalone = isStandalone;
            addBatchLocked(mAlarmBatches, batch);
        } else {
            Batch batch = mAlarmBatches->Get(whichBatch);
            if (batch->Add(a)) {
                // The start time of this batch advanced, so batch ordering may
                // have just been broken.  Move it to where it now belongs.
                mAlarmBatches->Remove(whichBatch);
                addBatchLocked(mAlarmBatches, batch);
            }
        }

        if (alarmClock != null) {
            mNextAlarmClockMayChange = TRUE;
            updateNextAlarmClockLocked();
        }

        if (DEBUG_VALIDATE) {
            if (doValidate && !validateConsistencyLocked()) {
                Slog->V(TAG, "Tipping-point operation: type=" + type + " when=" + when
                        + " when(hex)=" + Long->ToHexString(when)
                        + " whenElapsed=" + whenElapsed + " maxWhen=" + maxWhen
                        + " interval=" + interval + " op=" + operation
                        + " standalone=" + isStandalone);
                rebatchAllAlarmsLocked(FALSE);
            }
        }

        rescheduleKernelAlarmsLocked();
    }

    void dumpImpl(PrintWriter pw) {
        synchronized (mLock) {
            pw->Println("Current Alarm Manager state:");
            Int64 nowRTC = System->CurrentTimeMillis();
            Int64 nowELAPSED = SystemClock->ElapsedRealtime();
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

            pw->Print("nowRTC="); pw->Print(nowRTC);
            pw->Print("="); pw->Print(sdf->Format(new Date(nowRTC)));
            pw->Print(" nowELAPSED="); TimeUtils->FormatDuration(nowELAPSED, pw);
            pw->Println();
            if (!mInteractive) {
                pw->Print("Time since non-interactive: ");
                TimeUtils->FormatDuration(nowELAPSED - mNonInteractiveStartTime, pw);
                pw->Println();
                pw->Print("Max wakeup delay: ");
                TimeUtils->FormatDuration(currentNonWakeupFuzzLocked(nowELAPSED), pw);
                pw->Println();
                pw->Print("Time since last dispatch: ");
                TimeUtils->FormatDuration(nowELAPSED - mLastAlarmDeliveryTime, pw);
                pw->Println();
                pw->Print("Next non-wakeup delivery time: ");
                TimeUtils->FormatDuration(nowELAPSED - mNextNonWakeupDeliveryTime, pw);
                pw->Println();
            }

            Int64 nextWakeupRTC = mNextWakeup + (nowRTC - nowELAPSED);
            Int64 nextNonWakeupRTC = mNextNonWakeup + (nowRTC - nowELAPSED);
            pw->Print("Next non-wakeup alarm: ");
                    TimeUtils->FormatDuration(mNextNonWakeup, nowELAPSED, pw);
                    pw->Print(" = "); pw->Println(sdf->Format(new Date(nextNonWakeupRTC)));
            pw->Print("Next wakeup: "); TimeUtils->FormatDuration(mNextWakeup, nowELAPSED, pw);
                    pw->Print(" = "); pw->Println(sdf->Format(new Date(nextWakeupRTC)));
            pw->Print("Num time change events: "); pw->Println(mNumTimeChanged);

            if (mAlarmBatches->Size() > 0) {
                pw->Println();
                pw->Print("Pending alarm batches: ");
                pw->Println(mAlarmBatches->Size());
                for (Batch b : mAlarmBatches) {
                    pw->Print(b); pw->Println(':');
                    dumpAlarmList(pw, b.alarms, "  ", nowELAPSED, nowRTC, sdf);
                }
            }

            pw->Println();
            pw->Print("Past-due non-wakeup alarms: ");
            if (mPendingNonWakeupAlarms->Size() > 0) {
                pw->Println(mPendingNonWakeupAlarms->Size());
                dumpAlarmList(pw, mPendingNonWakeupAlarms, "  ", nowELAPSED, nowRTC, sdf);
            } else {
                pw->Println("(none)");
            }
            pw->Print("  Number of delayed alarms: "); pw->Print(mNumDelayedAlarms);
            pw->Print(", total delay time: "); TimeUtils->FormatDuration(mTotalDelayTime, pw);
            pw->Println();
            pw->Print("  Max delay time: "); TimeUtils->FormatDuration(mMaxDelayTime, pw);
            pw->Print(", max non-interactive time: ");
            TimeUtils->FormatDuration(mNonInteractiveTime, pw);
            pw->Println();

            pw->Println();
            pw->Print("  Broadcast ref count: "); pw->Println(mBroadcastRefCount);
            pw->Println();

            if (mLog->Dump(pw, "  Recent problems", "    ")) {
                pw->Println();
            }

            FilterStats[] topFilters = new FilterStats[10];
            Comparator<FilterStats> comparator = new Comparator<FilterStats>() {
                //@Override
                public Int32 compare(FilterStats lhs, FilterStats rhs) {
                    if (lhs.aggregateTime < rhs.aggregateTime) {
                        return 1;
                    } else if (lhs.aggregateTime > rhs.aggregateTime) {
                        return -1;
                    }
                    return 0;
                }
            };
            Int32 len = 0;
            for (Int32 iu=0; iu<mBroadcastStats->Size(); iu++) {
                ArrayMap<String, BroadcastStats> uidStats = mBroadcastStats->ValueAt(iu);
                for (Int32 ip=0; ip<uidStats->Size(); ip++) {
                    BroadcastStats bs = uidStats->ValueAt(ip);
                    for (Int32 is=0; is<bs.filterStats->Size(); is++) {
                        FilterStats fs = bs.filterStats->ValueAt(is);
                        Int32 pos = len > 0
                                ? Arrays->BinarySearch(topFilters, 0, len, fs, comparator) : 0;
                        if (pos < 0) {
                            pos = -pos - 1;
                        }
                        if (pos < topFilters.length) {
                            Int32 copylen = topFilters.length - pos - 1;
                            if (copylen > 0) {
                                System->Arraycopy(topFilters, pos, topFilters, pos+1, copylen);
                            }
                            topFilters[pos] = fs;
                            if (len < topFilters.length) {
                                len++;
                            }
                        }
                    }
                }
            }
            if (len > 0) {
                pw->Println("  Top Alarms:");
                for (Int32 i=0; i<len; i++) {
                    FilterStats fs = topFilters[i];
                    pw->Print("    ");
                    if (fs.nesting > 0) pw->Print("*ACTIVE* ");
                    TimeUtils->FormatDuration(fs.aggregateTime, pw);
                    pw->Print(" running, "); pw->Print(fs.numWakeup);
                    pw->Print(" wakeups, "); pw->Print(fs.count);
                    pw->Print(" alarms: "); UserHandle->FormatUid(pw, fs.mBroadcastStats.mUid);
                    pw->Print(":"); pw->Print(fs.mBroadcastStats.mPackageName);
                    pw->Println();
                    pw->Print("      "); pw->Print(fs.mTag);
                    pw->Println();
                }
            }

            pw->Println(" ");
            pw->Println("  Alarm Stats:");
            ArrayList<FilterStats> tmpFilters = new ArrayList<FilterStats>();
            for (Int32 iu=0; iu<mBroadcastStats->Size(); iu++) {
                ArrayMap<String, BroadcastStats> uidStats = mBroadcastStats->ValueAt(iu);
                for (Int32 ip=0; ip<uidStats->Size(); ip++) {
                    BroadcastStats bs = uidStats->ValueAt(ip);
                    pw->Print("  ");
                    if (bs.nesting > 0) pw->Print("*ACTIVE* ");
                    UserHandle->FormatUid(pw, bs.mUid);
                    pw->Print(":");
                    pw->Print(bs.mPackageName);
                    pw->Print(" "); TimeUtils->FormatDuration(bs.aggregateTime, pw);
                            pw->Print(" running, "); pw->Print(bs.numWakeup);
                            pw->Println(" wakeups:");
                    tmpFilters->Clear();
                    for (Int32 is=0; is<bs.filterStats->Size(); is++) {
                        tmpFilters->Add(bs.filterStats->ValueAt(is));
                    }
                    Collections->Sort(tmpFilters, comparator);
                    for (Int32 i=0; i<tmpFilters->Size(); i++) {
                        FilterStats fs = tmpFilters->Get(i);
                        pw->Print("    ");
                                if (fs.nesting > 0) pw->Print("*ACTIVE* ");
                                TimeUtils->FormatDuration(fs.aggregateTime, pw);
                                pw->Print(" "); pw->Print(fs.numWakeup);
                                pw->Print(" wakes " ); pw->Print(fs.count);
                                pw->Print(" alarms: ");
                                pw->Print(fs.mTag);
                                pw->Println();
                    }
                }
            }

            if (WAKEUP_STATS) {
                pw->Println();
                pw->Println("  Recent Wakeup History:");
                Int64 last = -1;
                for (WakeupEvent event : mRecentWakeups) {
                    pw->Print("    "); pw->Print(sdf->Format(new Date(event.when)));
                    pw->Print('|');
                    if (last < 0) {
                        pw->Print('0');
                    } else {
                        pw->Print(event.when - last);
                    }
                    last = event.when;
                    pw->Print('|'); pw->Print(event.uid);
                    pw->Print('|'); pw->Print(event.action);
                    pw->Println();
                }
                pw->Println();
            }
        }
    }

    private void logBatchesLocked(SimpleDateFormat sdf) {
        ByteArrayOutputStream bs = new ByteArrayOutputStream(2048);
        PrintWriter pw = new PrintWriter(bs);
        Int64 nowRTC = System->CurrentTimeMillis();
        Int64 nowELAPSED = SystemClock->ElapsedRealtime();
        Int32 NZ = mAlarmBatches->Size();
        for (Int32 iz = 0; iz < NZ; iz++) {
            Batch bz = mAlarmBatches->Get(iz);
            pw->Append("Batch "); pw->Print(iz); pw->Append(": "); pw->Println(bz);
            dumpAlarmList(pw, bz.alarms, "  ", nowELAPSED, nowRTC, sdf);
            pw->Flush();
            Slog->V(TAG, bs->ToString());
            bs->Reset();
        }
    }

    private Boolean validateConsistencyLocked() {
        if (DEBUG_VALIDATE) {
            Int64 lastTime = Long.MIN_VALUE;
            Int32 N = mAlarmBatches->Size();
            for (Int32 i = 0; i < N; i++) {
                Batch b = mAlarmBatches->Get(i);
                if (b.start >= lastTime) {
                    // duplicate start times are okay because of standalone batches
                    lastTime = b.start;
                } else {
                    Slogger::E(TAG, "CONSISTENCY FAILURE: Batch " + i + " is out of order");
                    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                    logBatchesLocked(sdf);
                    return FALSE;
                }
            }
        }
        return TRUE;
    }

    private Batch findFirstWakeupBatchLocked() {
        Int32 N = mAlarmBatches->Size();
        for (Int32 i = 0; i < N; i++) {
            Batch b = mAlarmBatches->Get(i);
            if (b->HasWakeups()) {
                return b;
            }
        }
        return null;
    }

    private AlarmManager.AlarmClockInfo getNextAlarmClockImpl(Int32 userId) {
        synchronized (mLock) {
            return mNextAlarmClockForUser->Get(userId);
        }
    }

    /**
     * Recomputes the next alarm clock for all users.
     */
    private void updateNextAlarmClockLocked() {
        if (!mNextAlarmClockMayChange) {
            return;
        }
        mNextAlarmClockMayChange = FALSE;

        SparseArray<AlarmManager.AlarmClockInfo> nextForUser = mTmpSparseAlarmClockArray;
        nextForUser->Clear();

        Int32 N = mAlarmBatches->Size();
        for (Int32 i = 0; i < N; i++) {
            ArrayList<Alarm> alarms = mAlarmBatches->Get(i).alarms;
            Int32 M = alarms->Size();

            for (Int32 j = 0; j < M; j++) {
                Alarm a = alarms->Get(j);
                if (a.alarmClock != null) {
                    Int32 userId = a.userId;

                    if (DEBUG_ALARM_CLOCK) {
                        Log->V(TAG, "Found AlarmClockInfo at " +
                                formatNextAlarm(GetContext(), a.alarmClock) +
                                " for user " + userId);
                    }

                    // Alarms and batches are sorted by time, no need to compare times here.
                    if (nextForUser->Get(userId) == null) {
                        nextForUser->Put(userId, a.alarmClock);
                    }
                }
            }
        }

        // Update mNextAlarmForUser with new values.
        Int32 NN = nextForUser->Size();
        for (Int32 i = 0; i < NN; i++) {
            AlarmManager.AlarmClockInfo newAlarm = nextForUser->ValueAt(i);
            Int32 userId = nextForUser->KeyAt(i);
            AlarmManager.AlarmClockInfo currentAlarm = mNextAlarmClockForUser->Get(userId);
            if (!newAlarm->Equals(currentAlarm)) {
                updateNextAlarmInfoForUserLocked(userId, newAlarm);
            }
        }

        // Remove users without any alarm clocks scheduled.
        Int32 NNN = mNextAlarmClockForUser->Size();
        for (Int32 i = NNN - 1; i >= 0; i--) {
            Int32 userId = mNextAlarmClockForUser->KeyAt(i);
            if (nextForUser->Get(userId) == null) {
                updateNextAlarmInfoForUserLocked(userId, null);
            }
        }
    }

    private void updateNextAlarmInfoForUserLocked(Int32 userId,
            AlarmManager.AlarmClockInfo alarmClock) {
        if (alarmClock != null) {
            if (DEBUG_ALARM_CLOCK) {
                Log->V(TAG, "Next AlarmClockInfoForUser(" + userId + "): " +
                        formatNextAlarm(GetContext(), alarmClock));
            }
            mNextAlarmClockForUser->Put(userId, alarmClock);
        } else {
            if (DEBUG_ALARM_CLOCK) {
                Log->V(TAG, "Next AlarmClockInfoForUser(" + userId + "): None");
            }
            mNextAlarmClockForUser->Remove(userId);
        }

        mPendingSendNextAlarmClockChangedForUser->Put(userId, TRUE);
        mHandler->RemoveMessages(AlarmHandler.SEND_NEXT_ALARM_CLOCK_CHANGED);
        mHandler->SendEmptyMessage(AlarmHandler.SEND_NEXT_ALARM_CLOCK_CHANGED);
    }

    /**
     * Updates NEXT_ALARM_FORMATTED and sends NEXT_ALARM_CLOCK_CHANGED_INTENT for all users
     * for which alarm clocks have changed since the last call to this.
     *
     * Do not call with a lock held. Only call from mHandler's thread.
     *
     * @see AlarmHandler#SEND_NEXT_ALARM_CLOCK_CHANGED
     */
    private void sendNextAlarmClockChanged() {
        SparseArray<AlarmManager.AlarmClockInfo> pendingUsers = mHandlerSparseAlarmClockArray;
        pendingUsers->Clear();

        synchronized (mLock) {
            Int32 N  = mPendingSendNextAlarmClockChangedForUser->Size();
            for (Int32 i = 0; i < N; i++) {
                Int32 userId = mPendingSendNextAlarmClockChangedForUser->KeyAt(i);
                pendingUsers->Append(userId, mNextAlarmClockForUser->Get(userId));
            }
            mPendingSendNextAlarmClockChangedForUser->Clear();
        }

        Int32 N = pendingUsers->Size();
        for (Int32 i = 0; i < N; i++) {
            Int32 userId = pendingUsers->KeyAt(i);
            AlarmManager.AlarmClockInfo alarmClock = pendingUsers->ValueAt(i);
            Settings.System->PutStringForUser(GetContext()->GetContentResolver(),
                    Settings.System.NEXT_ALARM_FORMATTED,
                    formatNextAlarm(GetContext(), alarmClock),
                    userId);

            GetContext()->SendBroadcastAsUser(NEXT_ALARM_CLOCK_CHANGED_INTENT,
                    new UserHandle(userId));
        }
    }

    /**
     * Formats an alarm like platform/packages/apps/DeskClock used to.
     */
    private static String formatNextAlarm(Context context, AlarmManager.AlarmClockInfo info) {
        String skeleton = DateFormat->Is24HourFormat(context) ? "EHm" : "Ehma";
        String pattern = DateFormat->GetBestDateTimePattern(Locale->GetDefault(), skeleton);
        return (info == null) ? "" :
                DateFormat->Format(pattern, info->GetTriggerTime()).toString();
    }

    void rescheduleKernelAlarmsLocked() {
        // Schedule the next upcoming wakeup alarm.  If there is a deliverable batch
        // prior to that which contains no wakeups, we schedule that as well.
        Int64 nextNonWakeup = 0;
        if (mAlarmBatches->Size() > 0) {
            Batch firstWakeup = findFirstWakeupBatchLocked();
            Batch firstBatch = mAlarmBatches->Get(0);
            if (firstWakeup != null && mNextWakeup != firstWakeup.start) {
                mNextWakeup = firstWakeup.start;
                setLocked(ELAPSED_REALTIME_WAKEUP, firstWakeup.start);
            }
            if (firstBatch != firstWakeup) {
                nextNonWakeup = firstBatch.start;
            }
        }
        if (mPendingNonWakeupAlarms->Size() > 0) {
            if (nextNonWakeup == 0 || mNextNonWakeupDeliveryTime < nextNonWakeup) {
                nextNonWakeup = mNextNonWakeupDeliveryTime;
            }
        }
        if (nextNonWakeup != 0 && mNextNonWakeup != nextNonWakeup) {
            mNextNonWakeup = nextNonWakeup;
            setLocked(ELAPSED_REALTIME, nextNonWakeup);
        }
    }

    private void removeLocked(PendingIntent operation) {
        Boolean didRemove = FALSE;
        for (Int32 i = mAlarmBatches->Size() - 1; i >= 0; i--) {
            Batch b = mAlarmBatches->Get(i);
            didRemove |= b->Remove(operation);
            if (b->Size() == 0) {
                mAlarmBatches->Remove(i);
            }
        }

        if (didRemove) {
            if (DEBUG_BATCH) {
                Slog->V(TAG, "Remove(operation) changed bounds; rebatching");
            }
            rebatchAllAlarmsLocked(TRUE);
            rescheduleKernelAlarmsLocked();
            updateNextAlarmClockLocked();
        }
    }

    void removeLocked(String packageName) {
        Boolean didRemove = FALSE;
        for (Int32 i = mAlarmBatches->Size() - 1; i >= 0; i--) {
            Batch b = mAlarmBatches->Get(i);
            didRemove |= b->Remove(packageName);
            if (b->Size() == 0) {
                mAlarmBatches->Remove(i);
            }
        }

        if (didRemove) {
            if (DEBUG_BATCH) {
                Slog->V(TAG, "Remove(package) changed bounds; rebatching");
            }
            rebatchAllAlarmsLocked(TRUE);
            rescheduleKernelAlarmsLocked();
            updateNextAlarmClockLocked();
        }
    }

    void removeUserLocked(Int32 userHandle) {
        Boolean didRemove = FALSE;
        for (Int32 i = mAlarmBatches->Size() - 1; i >= 0; i--) {
            Batch b = mAlarmBatches->Get(i);
            didRemove |= b->Remove(userHandle);
            if (b->Size() == 0) {
                mAlarmBatches->Remove(i);
            }
        }

        if (didRemove) {
            if (DEBUG_BATCH) {
                Slog->V(TAG, "Remove(user) changed bounds; rebatching");
            }
            rebatchAllAlarmsLocked(TRUE);
            rescheduleKernelAlarmsLocked();
            updateNextAlarmClockLocked();
        }
    }

    void interactiveStateChangedLocked(Boolean interactive) {
        if (mInteractive != interactive) {
            mInteractive = interactive;
            Int64 nowELAPSED = SystemClock->ElapsedRealtime();
            if (interactive) {
                if (mPendingNonWakeupAlarms->Size() > 0) {
                    Int64 thisDelayTime = nowELAPSED - mStartCurrentDelayTime;
                    mTotalDelayTime += thisDelayTime;
                    if (mMaxDelayTime < thisDelayTime) {
                        mMaxDelayTime = thisDelayTime;
                    }
                    deliverAlarmsLocked(mPendingNonWakeupAlarms, nowELAPSED);
                    mPendingNonWakeupAlarms->Clear();
                }
                if (mNonInteractiveStartTime > 0) {
                    Int64 dur = nowELAPSED - mNonInteractiveStartTime;
                    if (dur > mNonInteractiveTime) {
                        mNonInteractiveTime = dur;
                    }
                }
            } else {
                mNonInteractiveStartTime = nowELAPSED;
            }
        }
    }

    Boolean lookForPackageLocked(String packageName) {
        for (Int32 i = 0; i < mAlarmBatches->Size(); i++) {
            Batch b = mAlarmBatches->Get(i);
            if (b->HasPackage(packageName)) {
                return TRUE;
            }
        }
        return FALSE;
    }

    private void setLocked(Int32 type, Int64 when) {
        if (mNativeData != 0) {
            // The kernel never triggers alarms with negative wakeup times
            // so we ensure they are positive.
            Int64 alarmSeconds, alarmNanoseconds;
            if (when < 0) {
                alarmSeconds = 0;
                alarmNanoseconds = 0;
            } else {
                alarmSeconds = when / 1000;
                alarmNanoseconds = (when % 1000) * 1000 * 1000;
            }

            set(mNativeData, type, alarmSeconds, alarmNanoseconds);
        } else {
            Message msg = Message->Obtain();
            msg.what = ALARM_EVENT;

            mHandler->RemoveMessages(ALARM_EVENT);
            mHandler->SendMessageAtTime(msg, when);
        }
    }

    static const void dumpAlarmList(PrintWriter pw, ArrayList<Alarm> list,
            String prefix, String label, Int64 nowRTC, Int64 nowELAPSED, SimpleDateFormat sdf) {
        for (Int32 i=list->Size()-1; i>=0; i--) {
            Alarm a = list->Get(i);
            pw->Print(prefix); pw->Print(label); pw->Print(" #"); pw->Print(i);
                    pw->Print(": "); pw->Println(a);
            a->Dump(pw, prefix + "  ", nowRTC, nowELAPSED, sdf);
        }
    }

    static const String labelForType(Int32 type) {
        switch (type) {
        case RTC: return "RTC";
        case RTC_WAKEUP : return "RTC_WAKEUP";
        case ELAPSED_REALTIME : return "ELAPSED";
        case ELAPSED_REALTIME_WAKEUP: return "ELAPSED_WAKEUP";
        default:
            break;
        }
        return "--unknown--";
    }

    static const void dumpAlarmList(PrintWriter pw, ArrayList<Alarm> list,
            String prefix, Int64 nowELAPSED, Int64 nowRTC, SimpleDateFormat sdf) {
        for (Int32 i=list->Size()-1; i>=0; i--) {
            Alarm a = list->Get(i);
            String label = labelForType(a.type);
            pw->Print(prefix); pw->Print(label); pw->Print(" #"); pw->Print(i);
                    pw->Print(": "); pw->Println(a);
            a->Dump(pw, prefix + "  ", nowRTC, nowELAPSED, sdf);
        }
    }

    private native Int64 init();
    private native void close(Int64 nativeData);
    private native void set(Int64 nativeData, Int32 type, Int64 seconds, Int64 nanoseconds);
    private native Int32 waitForAlarm(Int64 nativeData);
    private native Int32 setKernelTime(Int64 nativeData, Int64 millis);
    private native Int32 setKernelTimezone(Int64 nativeData, Int32 minuteswest);

    Boolean triggerAlarmsLocked(ArrayList<Alarm> triggerList, Int64 nowELAPSED,
            Int64 nowRTC) {
        Boolean hasWakeup = FALSE;
        // batches are temporally sorted, so we need only pull from the
        // start of the list until we either empty it or hit a batch
        // that is not yet deliverable
        while (mAlarmBatches->Size() > 0) {
            Batch batch = mAlarmBatches->Get(0);
            if (batch.start > nowELAPSED) {
                // Everything else is scheduled for the future
                break;
            }

            // We will (re)schedule some alarms now; don't let that interfere
            // with delivery of this current batch
            mAlarmBatches->Remove(0);

            Int32 N = batch->Size();
            for (Int32 i = 0; i < N; i++) {
                Alarm alarm = batch->Get(i);
                alarm.count = 1;
                triggerList->Add(alarm);

                // Recurring alarms may have passed several alarm intervals while the
                // phone was asleep or off, so pass a trigger count when sending them.
                if (alarm.repeatInterval > 0) {
                    // this adjustment will be zero if we're late by
                    // less than one full repeat interval
                    alarm.count += (nowELAPSED - alarm.whenElapsed) / alarm.repeatInterval;

                    // Also schedule its next recurrence
                    Int64 delta = alarm.count * alarm.repeatInterval;
                    Int64 nextElapsed = alarm.whenElapsed + delta;
                    setImplLocked(alarm.type, alarm.when + delta, nextElapsed, alarm.windowLength,
                            maxTriggerTime(nowELAPSED, nextElapsed, alarm.repeatInterval),
                            alarm.repeatInterval, alarm.operation, batch.standalone, TRUE,
                            alarm.workSource, alarm.alarmClock, alarm.userId);

                    // For now we count this as a wakeup alarm, meaning it needs to be
                    // delivered immediately.  In the future we should change this, but
                    // that required delaying when we reschedule the repeat...!
                    hasWakeup = FALSE;
                } else if (alarm.wakeup) {
                    hasWakeup = TRUE;
                }

                // We removed an alarm clock. Let the caller recompute the next alarm clock.
                if (alarm.alarmClock != null) {
                    mNextAlarmClockMayChange = TRUE;
                }
            }
        }

        // This is a new alarm delivery set; bump the sequence number to indicate that
        // all apps' alarm delivery classes should be recalculated.
        mCurrentSeq++;
        CalculateDeliveryPriorities(triggerList);
        Collections->Sort(triggerList, mAlarmDispatchComparator);

        if (localLOGV) {
            for (Int32 i=0; i<triggerList->Size(); i++) {
                Slog->V(TAG, "Triggering alarm #" + i + ": " + triggerList->Get(i));
            }
        }

        return hasWakeup;
    }

    void recordWakeupAlarms(ArrayList<Batch> batches, Int64 nowELAPSED, Int64 nowRTC) {
        Int32 numBatches = batches->Size();
        for (Int32 nextBatch = 0; nextBatch < numBatches; nextBatch++) {
            Batch b = batches->Get(nextBatch);
            if (b.start > nowELAPSED) {
                break;
            }

            Int32 numAlarms = b.alarms->Size();
            for (Int32 nextAlarm = 0; nextAlarm < numAlarms; nextAlarm++) {
                Alarm a = b.alarms->Get(nextAlarm);
                WakeupEvent e = new WakeupEvent(nowRTC,
                        a.operation->GetCreatorUid(),
                        a.operation->GetIntent()->GetAction());
                mRecentWakeups->Add(e);
            }
        }
    }

    Int64 currentNonWakeupFuzzLocked(Int64 nowELAPSED) {
        Int64 timeSinceOn = nowELAPSED - mNonInteractiveStartTime;
        if (timeSinceOn < 5*60*1000) {
            // If the screen has been off for 5 minutes, only delay by at most two minutes.
            return 2*60*1000;
        } else if (timeSinceOn < 30*60*1000) {
            // If the screen has been off for 30 minutes, only delay by at most 15 minutes.
            return 15*60*1000;
        } else {
            // Otherwise, we will delay by at most an hour.
            return 60*60*1000;
        }
    }

    Boolean checkAllowNonWakeupDelayLocked(Int64 nowELAPSED) {
        if (mInteractive) {
            return FALSE;
        }
        if (mLastAlarmDeliveryTime <= 0) {
            return FALSE;
        }
        if (mPendingNonWakeupAlarms->Size() > 0 && mNextNonWakeupDeliveryTime > nowELAPSED) {
            // This is just a little paranoia, if somehow we have pending non-wakeup alarms
            // and the next delivery time is in the past, then just deliver them all.  This
            // avoids bugs where we get stuck in a loop trying to poll for alarms.
            return FALSE;
        }
        Int64 timeSinceLast = nowELAPSED - mLastAlarmDeliveryTime;
        return timeSinceLast <= currentNonWakeupFuzzLocked(nowELAPSED);
    }

    void deliverAlarmsLocked(ArrayList<Alarm> triggerList, Int64 nowELAPSED) {
        mLastAlarmDeliveryTime = nowELAPSED;
        for (Int32 i=0; i<triggerList->Size(); i++) {
            Alarm alarm = triggerList->Get(i);
            try {
                if (localLOGV) {
                    Slog->V(TAG, "sending alarm " + alarm);
                }
                alarm.operation->Send(GetContext(), 0,
                        mBackgroundIntent->PutExtra(
                                Intent.EXTRA_ALARM_COUNT, alarm.count),
                        mResultReceiver, mHandler);

                // we have an active broadcast so stay awake.
                if (mBroadcastRefCount == 0) {
                    setWakelockWorkSource(alarm.operation, alarm.workSource,
                            alarm.type, alarm.tag, TRUE);
                    mWakeLock->Acquire();
                }
                InFlight inflight = new InFlight(AlarmManagerService.this,
                        alarm.operation, alarm.workSource, alarm.type, alarm.tag);
                mInFlight->Add(inflight);
                mBroadcastRefCount++;

                BroadcastStats bs = inflight.mBroadcastStats;
                bs.count++;
                if (bs.nesting == 0) {
                    bs.nesting = 1;
                    bs.startTime = nowELAPSED;
                } else {
                    bs.nesting++;
                }
                FilterStats fs = inflight.mFilterStats;
                fs.count++;
                if (fs.nesting == 0) {
                    fs.nesting = 1;
                    fs.startTime = nowELAPSED;
                } else {
                    fs.nesting++;
                }
                if (alarm.type == ELAPSED_REALTIME_WAKEUP
                        || alarm.type == RTC_WAKEUP) {
                    bs.numWakeup++;
                    fs.numWakeup++;
                    if (alarm.workSource != null && alarm.workSource->Size() > 0) {
                        for (Int32 wi=0; wi<alarm.workSource->Size(); wi++) {
                            ActivityManagerNative->NoteWakeupAlarm(
                                    alarm.operation, alarm.workSource->Get(wi),
                                    alarm.workSource->GetName(wi));
                        }
                    } else {
                        ActivityManagerNative->NoteWakeupAlarm(
                                alarm.operation, -1, null);
                    }
                }
            } catch (PendingIntent.CanceledException e) {
                if (alarm.repeatInterval > 0) {
                    // This IntentSender is no longer valid, but this
                    // is a repeating alarm, so toss the hoser.
                    removeImpl(alarm.operation);
                }
            } catch (RuntimeException e) {
                Slogger::W(TAG, "Failure sending alarm.", e);
            }
        }
    }

    /**
     * Attribute blame for a WakeLock.
     * @param pi PendingIntent to attribute blame to if ws is null.
     * @param ws WorkSource to attribute blame.
     */
    void setWakelockWorkSource(PendingIntent pi, WorkSource ws, Int32 type, String tag,
            Boolean first) {
        try {
            Boolean unimportant = pi == mTimeTickSender;
            mWakeLock->SetUnimportantForLogging(unimportant);
            if (first || mLastWakeLockUnimportantForLogging) {
                mWakeLock->SetHistoryTag(tag);
            } else {
                mWakeLock->SetHistoryTag(null);
            }
            mLastWakeLockUnimportantForLogging = unimportant;
            if (ws != null) {
                mWakeLock->SetWorkSource(ws);
                return;
            }

            Int32 uid = ActivityManagerNative->GetDefault()
                    .getUidForIntentSender(pi->GetTarget());
            if (uid >= 0) {
                mWakeLock->SetWorkSource(new WorkSource(uid));
                return;
            }
        } catch (Exception e) {
        }

        // Something went wrong; fall back to attributing the lock to the OS
        mWakeLock->SetWorkSource(null);
    }

    private BroadcastStats getStatsLocked(PendingIntent pi) {
        String pkg = pi->GetCreatorPackage();
        Int32 uid = pi->GetCreatorUid();
        ArrayMap<String, BroadcastStats> uidStats = mBroadcastStats->Get(uid);
        if (uidStats == null) {
            uidStats = new ArrayMap<String, BroadcastStats>();
            mBroadcastStats->Put(uid, uidStats);
        }
        BroadcastStats bs = uidStats->Get(pkg);
        if (bs == null) {
            bs = new BroadcastStats(uid, pkg);
            uidStats->Put(pkg, bs);
        }
        return bs;
    };


private:
    // The threshold for how Int64 an alarm can be late before we print a
    // warning message.  The time duration is in milliseconds.
    static const Int64 LATE_ALARM_THRESHOLD = 10 * 1000;

    static const Int32 RTC_WAKEUP_MASK = 1 << RTC_WAKEUP;
    static const Int32 RTC_MASK = 1 << RTC;
    static const Int32 ELAPSED_REALTIME_WAKEUP_MASK = 1 << ELAPSED_REALTIME_WAKEUP;
    static const Int32 ELAPSED_REALTIME_MASK = 1 << ELAPSED_REALTIME;
    static const Int32 TIME_CHANGED_MASK = 1 << 16;
    static const Int32 IS_WAKEUP_MASK = RTC_WAKEUP_MASK|ELAPSED_REALTIME_WAKEUP_MASK;

    // Mask for testing whether a given alarm type is wakeup vs non-wakeup
    static const Int32 TYPE_NONWAKEUP_MASK = 0x1; // low bit => non-wakeup

    static const String TAG = "AlarmManager";
    static const String ClockReceiver_TAG = "ClockReceiver";
    static const Boolean localLOGV = FALSE;
    static const Boolean DEBUG_BATCH = localLOGV || FALSE;
    static const Boolean DEBUG_VALIDATE = localLOGV || FALSE;
    static const Boolean DEBUG_ALARM_CLOCK = localLOGV || FALSE;
    static const Int32 ALARM_EVENT = 1;
    static const String TIMEZONE_PROPERTY = "persist.sys.timezone";

    static const AutoPtr<IIntent> mBackgroundIntent
            = new Intent()->AddFlags(Intent.FLAG_FROM_BACKGROUND);
    static const AutoPtr<IncreasingTimeOrder> sIncreasingTimeOrder = new IncreasingTimeOrder();

    static const Boolean WAKEUP_STATS = FALSE;

    static const AutoPtr<IIntent> NEXT_ALARM_CLOCK_CHANGED_INTENT = new Intent(
            AlarmManager.ACTION_NEXT_ALARM_CLOCK_CHANGED);

    AutoPtr<ILocalLog> mLog = new LocalLog(TAG);

    Object mLock = new Object();

    Int64 mNativeData;
    Int64 mNextWakeup;
    Int64 mNextNonWakeup;
    Int32 mBroadcastRefCount = 0;
    AutoPtr<IPowerManagerWakeLock> mWakeLock;
    Boolean mLastWakeLockUnimportantForLogging;

    ArrayList<Alarm> mPendingNonWakeupAlarms = new ArrayList<Alarm>();
    ArrayList<InFlight> mInFlight = new ArrayList<InFlight>();

    AutoPtr<AlarmHandler> mHandler = new AlarmHandler();
    AutoPtr<ClockReceiver> mClockReceiver;
    AutoPtr<InteractiveStateReceiver> mInteractiveStateReceiver;
    AutoPtr<UninstallReceiver> mUninstallReceiver;
    AutoPtr<ResultReceiver> mResultReceiver = new ResultReceiver();
    AutoPtr<IPendingIntent> mTimeTickSender;
    AutoPtr<IPendingIntent> mDateChangeSender;
    Boolean mInteractive = TRUE;
    Int64 mNonInteractiveStartTime;
    Int64 mNonInteractiveTime;
    Int64 mLastAlarmDeliveryTime;
    Int64 mStartCurrentDelayTime;
    Int64 mNextNonWakeupDeliveryTime;
    Int32 mNumTimeChanged;

    private SparseArray<AlarmManager.AlarmClockInfo> mNextAlarmClockForUser =
            new SparseArray<>();
    private SparseArray<AlarmManager.AlarmClockInfo> mTmpSparseAlarmClockArray =
            new SparseArray<>();
    private SparseBooleanArray mPendingSendNextAlarmClockChangedForUser =
            new SparseBooleanArray();
    private Boolean mNextAlarmClockMayChange;

    // May only use on mHandler's thread, locking not required.
    private SparseArray<AlarmManager.AlarmClockInfo> mHandlerSparseAlarmClockArray =
            new SparseArray<>();

    // Alarm delivery ordering bookkeeping
    static const Int32 PRIO_TICK = 0;
    static const Int32 PRIO_WAKEUP = 1;
    static const Int32 PRIO_NORMAL = 2;


    HashMap<String, PriorityClass> mPriorities =
            new HashMap<String, PriorityClass>();
    Int32 mCurrentSeq = 0;


    LinkedList<WakeupEvent> mRecentWakeups = new LinkedList<WakeupEvent>();
    Int64 RECENT_WAKEUP_PERIOD = 1000L * 60 * 60 * 24; // one day

    Comparator<Alarm> mAlarmDispatchComparator;

    // minimum recurrence period or alarm futurity for us to be able to fuzz it
    static const Int64 MIN_FUZZABLE_INTERVAL = 10000;
    static const BatchTimeOrder sBatchOrder = new BatchTimeOrder();
    ArrayList<Batch> mAlarmBatches = new ArrayList<Batch>();


    SparseArray<ArrayMap<String, BroadcastStats>> mBroadcastStats
            = new SparseArray<ArrayMap<String, BroadcastStats>>();

    Int32 mNumDelayedAlarms = 0;
    Int64 mTotalDelayTime = 0;
    Int64 mMaxDelayTime = 0;

    IBinder mService;
};


} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_CALARMMANAGERSERVICE_H__
