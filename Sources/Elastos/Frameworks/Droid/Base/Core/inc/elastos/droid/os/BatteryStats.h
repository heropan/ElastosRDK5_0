
#ifndef __ELASTOS_DROID_OS_BATTERYSTATS_H__
#define __ELASTOS_DROID_OS_BATTERYSTATS_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::IInteger32;
using Elastos::IO::IPrintWriter;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::List;
using Elastos::Utility::IFormatter;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Utility::IPrinter;

namespace Elastos {
namespace Droid {
namespace Os {

class BatteryStats
{
public:
    /**
     * State for keeping track of counting information.
     */
    class BatteryStatsCounter : public ElRefBase
    {
    public:
        /**
         * Returns the count associated with this Counter for the
         * selected type of statistics.
         *
         * @param which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT
         */
        virtual CARAPI_(Int32) GetCountLocked(
            /* [in] */ Int32 which) = 0;

        /**
         * Temporary for debugging.
         */
        virtual CARAPI_(void) LogState(
            /* [in] */ IPrinter* pw,
            /* [in] */ const String& prefix) = 0;
    };

    /**
     * State for keeping track of timing information.
     */
    class BatteryStatsTimer : public ElRefBase
    {
    public:
        friend class BatteryStats;

        /**
         * Returns the count associated with this Timer for the
         * selected type of statistics.
         *
         * @param which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT
         */
        virtual CARAPI_(Int32) GetCountLocked(
            /* [in] */ Int32 which) = 0;

        /**
         * Returns the total time in microseconds associated with this Timer for the
         * selected type of statistics.
         *
         * @param batteryRealtime system realtime on  battery in microseconds
         * @param which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT
         * @return a time in microseconds
         */
        virtual CARAPI_(Int64) GetTotalTimeLocked(
            /* [in] */ Int64 batteryRealtime,
            /* [in] */ Int32 which) = 0;

        /**
         * Temporary for debugging.
         */
        virtual CARAPI_(void) LogState(
            /* [in] */ IPrinter* pw,
            /* [in] */ const String& prefix) = 0;
    };

    /**
     * The statistics associated with a particular uid.
     */
    class BatteryStatsUid : public ElRefBase
    {
    public:
        /**
         * The statistics associated with a particular wake lock.
         */
        class BatteryStatsWakelock : public ElRefBase
        {
        public:
            virtual CARAPI GetWakeTime(
                /* [in] */ Int32 type,
                /* [out] */ BatteryStatsTimer** timer) = 0;
        };

        class BatteryStatsSensor : public ElRefBase
        {
        public:
            virtual CARAPI_(Int32) GetHandle() = 0;

            virtual CARAPI_(AutoPtr<BatteryStatsTimer>) GetSensorTime() = 0;

        public:
            // Magic sensor number for the GPS.
            static const Int32 GPS = -10000;
        };

        class Pid : public ElRefBase
        {
        public:
            Int64 mWakeSum;
            Int64 mWakeStart;
        };

        /**
         * The statistics associated with a particular process.
         */
        class BatteryStatsProc : public ElRefBase
        {
        public:
            class ExcessivePower : public ElRefBase
            {
            public:
                ExcessivePower()
                    : mType(0)
                    , mOverTime(0)
                    , mUsedTime(0)
                {}

            public:
                static const Int32 TYPE_WAKE = 1;
                static const Int32 TYPE_CPU = 2;

                Int32 mType;
                Int64 mOverTime;
                Int64 mUsedTime;
            };

            /**
             * Returns the total time (in 1/100 sec) spent executing in user code.
             *
             * @param which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT.
             */
            virtual CARAPI_(Int64) GetUserTime(
                /* [in] */ Int32 which) = 0;

            /**
             * Returns the total time (in 1/100 sec) spent executing in system code.
             *
             * @param which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT.
             */
            virtual CARAPI_(Int64) GetSystemTime(
                /* [in] */ Int32 which) = 0;

            /**
             * Returns the number of times the process has been started.
             *
             * @param which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT.
             */
            virtual CARAPI_(Int32) getStarts(
                /* [in] */ Int32 which) = 0;

            /**
             * Returns the cpu time spent in microseconds while the process was in the foreground.
             * @param which one of STATS_TOTAL, STATS_LAST, STATS_CURRENT or STATS_UNPLUGGED
             * @return foreground cpu time in microseconds
             */
            virtual CARAPI_(Int64) GetForegroundTime(
                /* [in] */ Int32 which) = 0;

            /**
             * Returns the approximate cpu time spent in microseconds, at a certain CPU speed.
             * @param speedStep the index of the CPU speed. This is not the actual speed of the
             * CPU.
             * @param which one of STATS_TOTAL, STATS_LAST, STATS_CURRENT or STATS_UNPLUGGED
             * @see BatteryStats#getCpuSpeedSteps()
             */
            virtual CARAPI_(Int64) GetTimeAtCpuSpeedStep(
                /* [in] */ Int32 speedStep,
                /* [in] */ Int32 which) = 0;

            virtual CARAPI_(Int32) CountExcessivePowers() = 0;

            virtual CARAPI_(AutoPtr<ExcessivePower>) GetExcessivePower(
                /* [in] */ Int32 i) = 0;
        };

        /**
         * The statistics associated with a particular package.
         */
        class BatteryStatsPkg : public ElRefBase
        {
        public:
            /**
             * The statistics associated with a particular service.
             */
            class BatteryStatsServ : public ElRefBase
            {
            public:
                /**
                 * Returns the amount of time spent started.
                 *
                 * @param batteryUptime elapsed uptime on battery in microseconds.
                 * @param which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT.
                 * @return
                 */
                virtual CARAPI_(Int64) GetStartTime(
                    /* [in] */ Int64 batteryUptime,
                    /* [in] */ Int32 which) = 0;

                /**
                 * Returns the total number of times startService() has been called.
                 *
                 * @param which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT.
                 */
                virtual CARAPI_(Int32) GetStarts(
                    /* [in] */ Int32 which) = 0;

                /**
                 * Returns the total number times the service has been launched.
                 *
                 * @param which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT.
                 */
                virtual CARAPI_(Int32) GetLaunches(
                    /* [in] */ Int32 which) = 0;
            };

        public:
            /**
             * Returns the number of times this package has done something that could wake up the
             * device from sleep.
             *
             * @param which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT.
             */
            virtual CARAPI_(Int32) GetWakeups(
                /* [in] */ Int32 which) = 0;

            /**
             * Returns a mapping containing service statistics.
             */
            virtual AutoPtr< HashMap<String, AutoPtr<BatteryStatsServ> > > GetServiceStats() = 0;
        };

    public:
        /**
         * Returns a mapping containing wakelock statistics.
         *
         * @return a Map from Strings to Uid.Wakelock objects.
         */
        virtual AutoPtr< HashMap<String, AutoPtr<BatteryStatsWakelock> > > GetWakelockStats() = 0;

        /**
         * Returns a mapping containing sensor statistics.
         *
         * @return a Map from Integer sensor ids to Uid.Sensor objects.
         */
        virtual AutoPtr< HashMap<Int32, AutoPtr<BatteryStatsSensor> > > GetSensorStats() = 0;

        /**
         * Returns a mapping containing active process data.
         */
        virtual HashMap<Int32, AutoPtr<Pid> >& GetPidStats() = 0;

        /**
         * Returns a mapping containing process statistics.
         *
         * @return a Map from Strings to Uid.Proc objects.
         */
        virtual AutoPtr< HashMap<String, AutoPtr<BatteryStatsProc> > > GetProcessStats() = 0;

        /**
         * Returns a mapping containing package statistics.
         *
         * @return a Map from Strings to Uid.Pkg objects.
         */
        virtual AutoPtr< HashMap<String, AutoPtr<BatteryStatsPkg> > > GetPackageStats() = 0;

        /**
         * {@hide}
         */
        virtual CARAPI_(Int32) GetUid() = 0;

        /**
         * {@hide}
         */
        virtual CARAPI_(Int64) GetTcpBytesReceived(
            /* [in] */ Int32 which) = 0;

        /**
         * {@hide}
         */
        virtual CARAPI_(Int64) GetTcpBytesSent(
            /* [in] */ Int32 which) = 0;

        virtual CARAPI_(void) NoteWifiRunningLocked() = 0;

        virtual CARAPI_(void) NoteWifiStoppedLocked() = 0;

        virtual CARAPI_(void) NoteFullWifiLockAcquiredLocked() = 0;

        virtual CARAPI_(void) NoteFullWifiLockReleasedLocked() = 0;

        virtual CARAPI_(void) NoteWifiScanStartedLocked() = 0;

        virtual CARAPI_(void) NoteWifiScanStoppedLocked() = 0;

        virtual CARAPI_(void) NoteWifiMulticastEnabledLocked() = 0;

        virtual CARAPI_(void) NoteWifiMulticastDisabledLocked() = 0;

        virtual CARAPI_(void) NoteAudioTurnedOnLocked() = 0;

        virtual CARAPI_(void) NoteAudioTurnedOffLocked() = 0;

        virtual CARAPI_(void) NoteVideoTurnedOnLocked() = 0;

        virtual CARAPI_(void) NoteVideoTurnedOffLocked() = 0;

        virtual CARAPI_(Int64) GetWifiRunningTime(
            /* [in] */ Int64 batteryRealtime,
            /* [in] */ Int32 which) = 0;

        virtual CARAPI_(Int64) GetFullWifiLockTime(
            /* [in] */ Int64 batteryRealtime,
            /* [in] */ Int32 which) = 0;

        virtual CARAPI_(Int64) GetWifiScanTime(
            /* [in] */ Int64 batteryRealtime,
            /* [in] */ Int32 which) = 0;

        virtual CARAPI_(Int64) GetWifiMulticastTime(
            /* [in] */ Int64 batteryRealtime,
            /* [in] */ Int32 which) = 0;

        virtual CARAPI_(Int64) GetAudioTurnedOnTime(
            /* [in] */ Int64 batteryRealtime,
            /* [in] */ Int32 which) = 0;

        virtual CARAPI_(Int64) GetVideoTurnedOnTime(
            /* [in] */ Int64 batteryRealtime,
            /* [in] */ Int32 which) = 0;

        virtual CARAPI_(void) NoteUserActivityLocked(
            /* [in] */ Int32 type) = 0;

        virtual CARAPI_(Boolean) HasUserActivity() = 0;

        virtual CARAPI_(Int32) GetUserActivityCount(
            /* [in] */ Int32 type,
            /* [in] */ Int32 which) = 0;

    public:
        /**
         * Note that these must match the constants in android.os.PowerManager.
         * Also, if the user activity types change, the BatteryStatsImpl.VERSION must
         * also be bumped.
         */
        static const AutoPtr<ArrayOf<String> > USER_ACTIVITY_TYPES;

        static const Int32 NUM_USER_ACTIVITY_TYPES = 3;
    };

    class HistoryItem
        : public ElRefBase
        , public IParcelable
    {
    public:
        HistoryItem()
            : mTime(0)
            , mCmd(CMD_NULL)
            , mBatteryLevel(0)
            , mBatteryStatus(0)
            , mBatteryHealth(0)
            , mBatteryPlugType(0)
            , mStates(0)
        {}

        HistoryItem(
            /* [in] */ Int64 time,
            /* [in] */ IParcel* src);

        CAR_INTERFACE_DECL()

        CARAPI_(Int32) DescribeContents();

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

        CARAPI_(void) WriteDelta(
            /* [in] */ IParcel* dest,
            /* [in] */ HistoryItem* last);

        CARAPI_(void) ReadDelta(
            /* [in] */ IParcel* src);

        CARAPI_(void) Clear();

        CARAPI_(void) SetTo(
            /* [in] */ HistoryItem* o);

        CARAPI_(void) SetTo(
            /* [in] */ Int64 time,
            /* [in] */ Byte cmd,
            /* [in] */ HistoryItem* o);

        CARAPI_(Boolean) Same(
            /* [in] */ HistoryItem* o);

    private:
        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        CARAPI_(Int32) BuildBatteryLevelInt();

        CARAPI_(Int32) BuildStateInt();

    public:
        static const String TAG;
        static const Boolean DEBUG = FALSE;

        static const Byte CMD_NULL = 0;
        static const Byte CMD_UPDATE = 1;
        static const Byte CMD_START = 2;
        static const Byte CMD_OVERFLOW = 3;

        // Constants from SCREEN_BRIGHTNESS_*
        static const Int32 STATE_BRIGHTNESS_MASK = 0x0000000f;
        static const Int32 STATE_BRIGHTNESS_SHIFT = 0;
        // Constants from SIGNAL_STRENGTH_*
        static const Int32 STATE_SIGNAL_STRENGTH_MASK = 0x000000f0;
        static const Int32 STATE_SIGNAL_STRENGTH_SHIFT = 4;
        // Constants from ServiceState.STATE_*
        static const Int32 STATE_PHONE_STATE_MASK = 0x00000f00;
        static const Int32 STATE_PHONE_STATE_SHIFT = 8;
        // Constants from DATA_CONNECTION_*
        static const Int32 STATE_DATA_CONNECTION_MASK = 0x0000f000;
        static const Int32 STATE_DATA_CONNECTION_SHIFT = 12;

        // These states always appear directly in the first int token
        // of a delta change; they should be ones that change relatively
        // frequently.
        static const Int32 STATE_WAKE_LOCK_FLAG = 1<<30;
        static const Int32 STATE_SENSOR_ON_FLAG = 1<<29;
        static const Int32 STATE_GPS_ON_FLAG = 1<<28;
        static const Int32 STATE_PHONE_SCANNING_FLAG = 1<<27;
        static const Int32 STATE_WIFI_RUNNING_FLAG = 1<<26;
        static const Int32 STATE_WIFI_FULL_LOCK_FLAG = 1<<25;
        static const Int32 STATE_WIFI_SCAN_FLAG = 1<<24;
        static const Int32 STATE_WIFI_MULTICAST_ON_FLAG = 1<<23;
        // These are on the lower bits used for the command; if they change
        // we need to write another int of data.
        static const Int32 STATE_AUDIO_ON_FLAG = 1<<22;
        static const Int32 STATE_VIDEO_ON_FLAG = 1<<21;
        static const Int32 STATE_SCREEN_ON_FLAG = 1<<20;
        static const Int32 STATE_BATTERY_PLUGGED_FLAG = 1<<19;
        static const Int32 STATE_PHONE_IN_CALL_FLAG = 1<<18;
        static const Int32 STATE_WIFI_ON_FLAG = 1<<17;
        static const Int32 STATE_BLUETOOTH_ON_FLAG = 1<<16;

        static const Int32 MOST_INTERESTING_STATES =
                STATE_BATTERY_PLUGGED_FLAG | STATE_SCREEN_ON_FLAG
                | STATE_GPS_ON_FLAG | STATE_PHONE_IN_CALL_FLAG;

        // Part of initial delta int that specifies the time delta.
        static const Int32 DELTA_TIME_MASK = 0x3ffff;
        static const Int32 DELTA_TIME_ABS = 0x3fffd;    // Following is an entire abs update.
        static const Int32 DELTA_TIME_INT = 0x3fffe;    // The delta is a following int
        static const Int32 DELTA_TIME_LONG = 0x3ffff;   // The delta is a following long
        // Part of initial delta int holding the command code.
        static const Int32 DELTA_CMD_MASK = 0x3;
        static const Int32 DELTA_CMD_SHIFT = 18;
        // Flag in delta int: a new battery level int follows.
        static const Int32 DELTA_BATTERY_LEVEL_FLAG = 1<<20;
        // Flag in delta int: a new full state and battery status int follows.
        static const Int32 DELTA_STATE_FLAG = 1<<21;
        static const Int32 DELTA_STATE_MASK = 0xffc00000;

    public:
        AutoPtr<HistoryItem> mNext;

        Int64 mTime;

        Byte mCmd;

        Byte mBatteryLevel;
        Byte mBatteryStatus;
        Byte mBatteryHealth;
        Byte mBatteryPlugType;

        Char32 mBatteryTemperature;
        Char32 mBatteryVoltage;

        Int32 mStates;
    };

    class BitDescription : public ElRefBase
    {
    public:
        BitDescription(
            /* [in] */ Int32 mask,
            /* [in] */ const String& name)
            : mMask(mask)
            , mShift(-1)
            , mName(name)
        {}

        BitDescription(
            /* [in] */ Int32 mask,
            /* [in] */ Int32 shift,
            /* [in] */ const String& name,
            /* [in] */ ArrayOf<String>* values)
            : mMask(mask)
            , mShift(shift)
            , mName(name)
            , mValues(values)
        {}

    public:
        Int32 mMask;
        Int32 mShift;
        String mName;
        AutoPtr< ArrayOf<String> > mValues;
    };

    class TimerEntry : public ElRefBase
    {
    public:
        TimerEntry(
            /* [in] */ const String& name,
            /* [in] */ Int32 id,
            /* [in] */ BatteryStatsTimer* timer,
            /* [in] */ Int64 time)
            : mName(name)
            , mId(id)
            , mTimer(timer)
            , mTime(time)
        {}

    public:
        String mName;
        Int32 mId;
        AutoPtr<BatteryStatsTimer> mTimer;
        Int64 mTime;
    };

    class HistoryPrinter : public ElRefBase
    {
    public:
        HistoryPrinter()
            : mOldState(0)
            , mOldStatus(-1)
            , mOldHealth(-1)
            , mOldPlug(-1)
            , mOldTemp(-1)
            , mOldVolt(-1)
        {}

        CARAPI_(void) PrintNextItem(
            /* [in] */ IPrintWriter* pw,
            /* [in] */ HistoryItem* rec,
            /* [in] */ Int64 now);

    public:
        Int32 mOldState;
        Int32 mOldStatus;
        Int32 mOldHealth;
        Int32 mOldPlug;
        Int32 mOldTemp;
        Int32 mOldVolt;
    };

public:
    BatteryStats();

    virtual CARAPI_(Boolean) StartIteratingHistoryLocked() = 0;

    virtual CARAPI_(Boolean) GetNextHistoryLocked(
        /* [in] */ HistoryItem* out) = 0;

    virtual CARAPI_(void) FinishIteratingHistoryLocked() = 0;

    virtual CARAPI_(Boolean) StartIteratingOldHistoryLocked() = 0;

    virtual CARAPI_(Boolean) GetNextOldHistoryLocked(
        /* [in] */ HistoryItem* out) = 0;

    virtual CARAPI_(void) FinishIteratingOldHistoryLocked() = 0;

    /**
     * Return the base time offset for the battery history.
     */
    virtual CARAPI_(Int64) GetHistoryBaseTime() = 0;

    /**
     * Returns the number of times the device has been started.
     */
    virtual CARAPI_(Int32) GetStartCount() = 0;

    /**
     * Returns the time in microseconds that the screen has been on while the device was
     * running on battery.
     *
     * {@hide}
     */
    virtual CARAPI_(Int64) GetScreenOnTime(
        /* [in] */ Int64 batteryRealtime,
        /* [in] */ Int32 which) = 0;

    /**
     * Returns the time in microseconds that the screen has been on with
     * the given brightness
     *
     * {@hide}
     */
    virtual CARAPI_(Int64) GetScreenBrightnessTime(
        /* [in] */ Int32 brightnessBin,
        /* [in] */ Int64 batteryRealtime,
        /* [in] */ Int32 which) = 0;

    virtual CARAPI_(Int32) GetInputEventCount(
        /* [in] */ Int32 which) = 0;

    /**
     * Returns the time in microseconds that the phone has been on while the device was
     * running on battery.
     *
     * {@hide}
     */
    virtual CARAPI_(Int64) GetPhoneOnTime(
        /* [in] */ Int64 batteryRealtime,
        /* [in] */ Int32 which) = 0;

    /**
     * Returns the time in microseconds that the phone has been running with
     * the given signal strength.
     *
     * {@hide}
     */
    virtual CARAPI_(Int64) GetPhoneSignalStrengthTime(
        /* [in] */ Int32 strengthBin,
        /* [in] */ Int64 batteryRealtime,
        /* [in] */ Int32 which) = 0;

    /**
     * Returns the time in microseconds that the phone has been trying to
     * acquire a signal.
     *
     * {@hide}
     */
    virtual CARAPI_(Int64) GetPhoneSignalScanningTime(
        /* [in] */ Int64 batteryRealtime,
        /* [in] */ Int32 which) = 0;

    /**
     * Returns the number of times the phone has entered the given signal strength.
     *
     * {@hide}
     */
    virtual CARAPI_(Int32) GetPhoneSignalStrengthCount(
        /* [in] */ Int32 strengthBin,
        /* [in] */ Int32 which) = 0;

    /**
     * Returns the time in microseconds that the phone has been running with
     * the given data connection.
     *
     * {@hide}
     */
    virtual CARAPI_(Int64) GetPhoneDataConnectionTime(
        /* [in] */ Int32 dataType,
        /* [in] */ Int64 batteryRealtime,
        /* [in] */ Int32 which) = 0;

    /**
     * Returns the number of times the phone has entered the given data
     * connection type.
     *
     * {@hide}
     */
    virtual CARAPI_(Int32) GetPhoneDataConnectionCount(
        /* [in] */ Int32 dataType,
        /* [in] */ Int32 which) = 0;

    /**
     * Returns the time in microseconds that wifi has been on while the device was
     * running on battery.
     *
     * {@hide}
     */
    virtual CARAPI_(Int64) GetWifiOnTime(
        /* [in] */ Int64 batteryRealtime,
        /* [in] */ Int32 which) = 0;

    /**
     * Returns the time in microseconds that wifi has been on and the driver has
     * been in the running state while the device was running on battery.
     *
     * {@hide}
     */
    virtual CARAPI_(Int64) GetGlobalWifiRunningTime(
        /* [in] */ Int64 batteryRealtime,
        /* [in] */ Int32 which) = 0;

    /**
     * Returns the time in microseconds that bluetooth has been on while the device was
     * running on battery.
     *
     * {@hide}
     */
    virtual CARAPI_(Int64) GetBluetoothOnTime(
        /* [in] */ Int64 batteryRealtime,
        /* [in] */ Int32 which) = 0;

    /**
     * Return whether we are currently running on battery.
     */
    virtual CARAPI_(Boolean) GetIsOnBattery() = 0;

    /**
     * Returns a SparseArray containing the statistics for each uid.
     */
    virtual AutoPtr< HashMap<Int32, AutoPtr<BatteryStatsUid> > > GetUidStats() = 0;

    /**
     * Returns the current battery uptime in microseconds.
     *
     * @param curTime the amount of elapsed realtime in microseconds.
     */
    virtual CARAPI_(Int64) GetBatteryUptime(
        /* [in] */ Int64 curTime) = 0;

    /**
     * @deprecated use getRadioDataUptime
     */
    CARAPI_(Int64) GetRadioDataUptimeMs();

    /**
     * Returns the time that the radio was on for data transfers.
     * @return the uptime in microseconds while unplugged
     */
    virtual CARAPI_(Int64) GetRadioDataUptime() = 0;

    /**
     * Returns the current battery realtime in microseconds.
     *
     * @param curTime the amount of elapsed realtime in microseconds.
     */
    virtual CARAPI_(Int64) GetBatteryRealtime(
        /* [in] */ Int64 curTime) = 0;

    /**
     * Returns the battery percentage level at the last time the device was unplugged from power, or
     * the last time it booted on battery power.
     */
    virtual CARAPI_(Int32) GetDischargeStartLevel() = 0;

    /**
     * Returns the current battery percentage level if we are in a discharge cycle, otherwise
     * returns the level at the last plug event.
     */
    virtual CARAPI_(Int32) GetDischargeCurrentLevel() = 0;

    /**
     * Get the amount the battery has discharged since the stats were
     * last reset after charging, as a lower-end approximation.
     */
    virtual CARAPI_(Int32) GetLowDischargeAmountSinceCharge() = 0;

    /**
     * Get the amount the battery has discharged since the stats were
     * last reset after charging, as an upper-end approximation.
     */
    virtual CARAPI_(Int32) GetHighDischargeAmountSinceCharge() = 0;

    /**
     * Get the amount the battery has discharged while the screen was on,
     * since the last time power was unplugged.
     */
    virtual CARAPI_(Int32) GetDischargeAmountScreenOn() = 0;

    /**
     * Get the amount the battery has discharged while the screen was on,
     * since the last time the device was charged.
     */
    virtual CARAPI_(Int32) GetDischargeAmountScreenOnSinceCharge() = 0;

    /**
     * Get the amount the battery has discharged while the screen was off,
     * since the last time power was unplugged.
     */
    virtual CARAPI_(Int32) GetDischargeAmountScreenOff() = 0;

    /**
     * Get the amount the battery has discharged while the screen was off,
     * since the last time the device was charged.
     */
    virtual CARAPI_(Int32) GetDischargeAmountScreenOffSinceCharge() = 0;

    /**
     * Returns the total, last, or current battery uptime in microseconds.
     *
     * @param curTime the elapsed realtime in microseconds.
     * @param which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT.
     */
    virtual CARAPI_(Int64) ComputeBatteryUptime(
        /* [in] */ Int64 curTime,
        /* [in] */ Int32 which) = 0;

    /**
     * Returns the total, last, or current battery realtime in microseconds.
     *
     * @param curTime the current elapsed realtime in microseconds.
     * @param which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT.
     */
    virtual CARAPI_(Int64) ComputeBatteryRealtime(
        /* [in] */ Int64 curTime,
        /* [in] */ Int32 which) = 0;

    /**
     * Returns the total, last, or current uptime in microseconds.
     *
     * @param curTime the current elapsed realtime in microseconds.
     * @param which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT.
     */
    virtual CARAPI_(Int64) ComputeUptime(
        /* [in] */ Int64 curTime,
        /* [in] */ Int32 which) = 0;

    /**
     * Returns the total, last, or current realtime in microseconds.
     * *
     * @param curTime the current elapsed realtime in microseconds.
     * @param which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT.
     */
    virtual CARAPI_(Int64) ComputeRealtime(
        /* [in] */ Int64 curTime,
        /* [in] */ Int32 which) = 0;

    virtual AutoPtr< HashMap<String, AutoPtr<BatteryStatsTimer> > > GetKernelWakelockStats() = 0;

    /** Returns the number of different speeds that the CPU can run at */
    virtual CARAPI_(Int32) GetCpuSpeedSteps() = 0;

    /**
     * Checkin server version of dump to produce more compact, computer-readable log.
     *
     * NOTE: all times are expressed in 'ms'.
     */
    CARAPI_(void) DumpCheckinLocked(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int32 which,
        /* [in] */ Int32 reqUid);

    CARAPI_(void) DumpLocked(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ Int32 which,
        /* [in] */ Int32 reqUid);

    static CARAPI_(void) PrintBitDescriptions(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int32 oldval,
        /* [in] */ Int32 newval,
        /* [in] */ ArrayOf<BitDescription*>* descriptions);

    CARAPI_(void) PrepareForDumpLocked();

    /**
     * Dumps a human-readable summary of the battery statistics to the given PrintWriter.
     *
     * @param pw a Printer to receive the dump output.
     */
    // @SuppressWarnings("unused")
    CARAPI_(void) DumpLocked(
        /* [in] */ IPrintWriter* pw);

    //@SuppressWarnings("unused")
    CARAPI_(void) DumpCheckinLocked(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ List< AutoPtr<IApplicationInfo> >& apps);

private:
    static CARAPI_(AutoPtr< ArrayOf<BatteryStats::BitDescription*> >) InitHistoryStateDescriptions();

    static CARAPI_(void) FormatTimeRaw(
        /* [in] */ StringBuilder& out,
        /* [in] */ Int64 seconds);

    static CARAPI_(void) FormatTime(
        /* [in] */ StringBuilder& sb,
        /* [in] */ Int64 time);

    static CARAPI_(void) FormatTimeMs(
        /* [in] */ StringBuilder& sb,
        /* [in] */ Int64 time);

    CARAPI_(String) FormatRatioLocked(
        /* [in] */ Int64 num,
        /* [in] */ Int64 den);

    CARAPI_(String) FormatBytesLocked(
        /* [in] */ Int64 bytes);

    static CARAPI_(Int64) ComputeWakeLock(
        /* [in] */ BatteryStatsTimer* timer,
        /* [in] */ Int64 batteryRealtime,
        /* [in] */ Int32 which);

    /**
     *
     * @param sb a StringBuilder object.
     * @param timer a Timer object contining the wakelock times.
     * @param batteryRealtime the current on-battery time in microseconds.
     * @param name the name of the wakelock.
     * @param which which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT.
     * @param linePrefix a String to be prepended to each line of output.
     * @return the line prefix
     */
    static CARAPI_(String) PrintWakeLock(
        /* [in] */ StringBuilder& sb,
        /* [in] */ BatteryStatsTimer* timer,
        /* [in] */ Int64 batteryRealtime,
        /* [in] */ const String& name,
        /* [in] */ Int32 which,
        /* [in] */ const String& linePrefix);

    /**
     * Checkin version of wakelock printer. Prints simple comma-separated list.
     *
     * @param sb a StringBuilder object.
     * @param timer a Timer object contining the wakelock times.
     * @param now the current time in microseconds.
     * @param name the name of the wakelock.
     * @param which which one of STATS_TOTAL, STATS_LAST, or STATS_CURRENT.
     * @param linePrefix a String to be prepended to each line of output.
     * @return the line prefix
     */
    static CARAPI_(String) PrintWakeLockCheckin(
        /* [in] */ StringBuilder& sb,
        /* [in] */ BatteryStatsTimer* timer,
        /* [in] */ Int64 now,
        /* [in] */ const String& name,
        /* [in] */ Int32 which,
        /* [in] */ const String& linePrefix);

    /**
     * Dump a comma-separated line of values for terse checkin mode.
     *
     * @param pw the PageWriter to dump log to
     * @param category category of data (e.g. "total", "last", "unplugged", "current" )
     * @param type type of data (e.g. "wakelock", "sensor", "process", "apk" ,  "process", "network")
     * @param args type-dependent data arguments
     */
    static CARAPI_(void) DumpLine(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ Int32 uid,
        /* [in] */ const String& category,
        /* [in] */ const String& type
        /* Object... args */);

public:
    /**
     * A constant indicating a partial wake lock timer.
     */
    static const Int32 WAKE_TYPE_PARTIAL = 0;

    /**
     * A constant indicating a full wake lock timer.
     */
    static const Int32 WAKE_TYPE_FULL = 1;

    /**
     * A constant indicating a window wake lock timer.
     */
    static const Int32 WAKE_TYPE_WINDOW = 2;

    /**
     * A constant indicating a sensor timer.
     */
    static const Int32 SENSOR = 3;

    /**
     * A constant indicating a a wifi running timer
     */
    static const Int32 WIFI_RUNNING = 4;

    /**
     * A constant indicating a full wifi lock timer
     */
    static const Int32 FULL_WIFI_LOCK = 5;

    /**
     * A constant indicating a wifi scan
     */
    static const Int32 WIFI_SCAN = 6;

     /**
      * A constant indicating a wifi multicast timer
      */
     static const Int32 WIFI_MULTICAST_ENABLED = 7;

    /**
     * A constant indicating an audio turn on timer
     */
    static const Int32 AUDIO_TURNED_ON = 7;

    /**
     * A constant indicating a video turn on timer
     */
    static const Int32 VIDEO_TURNED_ON = 8;

    /**
     * Include all of the data in the stats, including previously saved data.
     */
    static const Int32 STATS_SINCE_CHARGED = 0;

    /**
     * Include only the last run in the stats.
     */
    static const Int32 STATS_LAST = 1;

    /**
     * Include only the current run in the stats.
     */
    static const Int32 STATS_CURRENT = 2;

    /**
     * Include only the run since the last time the device was unplugged in the stats.
     */
    static const Int32 STATS_SINCE_UNPLUGGED = 3;

    static const Int32 SCREEN_BRIGHTNESS_DARK = 0;
    static const Int32 SCREEN_BRIGHTNESS_DIM = 1;
    static const Int32 SCREEN_BRIGHTNESS_MEDIUM = 2;
    static const Int32 SCREEN_BRIGHTNESS_LIGHT = 3;
    static const Int32 SCREEN_BRIGHTNESS_BRIGHT = 4;

    static const Int32 NUM_SCREEN_BRIGHTNESS_BINS = 5;

    static const Int32 DATA_CONNECTION_NONE = 0;
    static const Int32 DATA_CONNECTION_GPRS = 1;
    static const Int32 DATA_CONNECTION_EDGE = 2;
    static const Int32 DATA_CONNECTION_UMTS = 3;
    static const Int32 DATA_CONNECTION_CDMA = 4;
    static const Int32 DATA_CONNECTION_EVDO_0 = 5;
    static const Int32 DATA_CONNECTION_EVDO_A = 6;
    static const Int32 DATA_CONNECTION_1xRTT = 7;
    static const Int32 DATA_CONNECTION_HSDPA = 8;
    static const Int32 DATA_CONNECTION_HSUPA = 9;
    static const Int32 DATA_CONNECTION_HSPA = 10;
    static const Int32 DATA_CONNECTION_IDEN = 11;
    static const Int32 DATA_CONNECTION_EVDO_B = 12;
    static const Int32 DATA_CONNECTION_LTE = 13;
    static const Int32 DATA_CONNECTION_EHRPD = 14;
    static const Int32 DATA_CONNECTION_OTHER = 15;

    static const Int32 NUM_DATA_CONNECTION_TYPES = DATA_CONNECTION_OTHER + 1;

private:
    static const Boolean LOCAL_LOGV = FALSE;

    // NOTE: Update this list if you add/change any stats above.
    // These characters are supposed to represent "total", "last", "current",
    // and "unplugged". They were shortened for efficiency sake.
    static const AutoPtr< ArrayOf<String> > STAT_NAMES;

    /**
     * Bump the version on this if the checkin format changes.
     */
    static const Int32 BATTERY_STATS_CHECKIN_VERSION = 5;

    static const Int64 BYTES_PER_KB = 1024;
    static const Int64 BYTES_PER_MB = 1048576; // 1024^2
    static const Int64 BYTES_PER_GB = 1073741824; //1024^3

    static const String UID_DATA;
    static const String APK_DATA;
    static const String PROCESS_DATA;
    static const String SENSOR_DATA;
    static const String WAKELOCK_DATA;
    static const String KERNEL_WAKELOCK_DATA;
    static const String NETWORK_DATA;
    static const String USER_ACTIVITY_DATA;
    static const String BATTERY_DATA;
    static const String BATTERY_DISCHARGE_DATA;
    static const String BATTERY_LEVEL_DATA;
    static const String WIFI_DATA;
    static const String MISC_DATA;
    static const String SCREEN_BRIGHTNESS_DATA;
    static const String SIGNAL_STRENGTH_TIME_DATA;
    static const String SIGNAL_SCANNING_TIME_DATA;
    static const String SIGNAL_STRENGTH_COUNT_DATA;
    static const String DATA_CONNECTION_TIME_DATA;
    static const String DATA_CONNECTION_COUNT_DATA;

    static const AutoPtr< ArrayOf<String> > SCREEN_BRIGHTNESS_NAMES;

    static const AutoPtr< ArrayOf<String> > DATA_CONNECTION_NAMES;

    static const AutoPtr< ArrayOf<BitDescription*> > HISTORY_STATE_DESCRIPTIONS;

    AutoPtr<StringBuilder> mFormatBuilder;
    AutoPtr<IFormatter> mFormatter;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_BATTERYSTATS_H__
