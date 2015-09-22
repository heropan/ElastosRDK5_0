
#ifndef __ELASTOS_DROID_SERVER_NET_CNETWORKSTATSSERVICE_H__
#define __ELASTOS_DROID_SERVER_NET_CNETWORKSTATSSERVICE_H__

#include "ext/frameworkdef.h"
#include "_Elastos_Droid_Server_Net_CNetworkStatsService.h"
#include "content/BroadcastReceiver.h"
#include "net/NetworkIdentitySet.h"
#include "net/NetworkStatsRecorder.h"
#include "net/NetworkStatsCollection.h"

using Elastos::IO::IFileDescriptor;
using Elastos::Droid::Utility::ITrustedTime;
using Elastos::Droid::App::IIActivityManager;
using Elastos::Droid::App::IIAlarmManager;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Net::IIConnectivityManager;
using Elastos::Droid::Net::INetworkStatsSession;
using Elastos::Droid::Net::INetworkStats;
using Elastos::Droid::Net::INetworkStatsHistory;
using Elastos::Droid::Net::INetworkStatsService;
using Elastos::Droid::Net::INetworkTemplate;
using Elastos::Droid::Net::INetworkTemplateHelper;
using Elastos::Droid::Net::INetworkManagementEventObserver;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IHandlerCallback;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Os::IIPowerManager;
using Elastos::Droid::Os::IPowerManagerWakeLock;
using Elastos::Droid::Os::INetworkManagementService;
using Elastos::Droid::Telephony::ITelephonyManager;


namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

class CNetworkStatsSession;
class CNetworkStatsServiceAlertObserber;

CarClass(CNetworkStatsService)
{
public:
    class MyHandlerCallback
        : public ElRefBase
        , public IHandlerCallback
    {
    public:
        CAR_INTERFACE_DECL()

        MyHandlerCallback(
            /* [in] */ CNetworkStatsService* host)
            : mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg,
            /* [out] */ Boolean* result);

    private:
        CNetworkStatsService* mHost;
    };

    class NetworkStatsSettingsConfig : public ElRefBase
    {
    public:
        NetworkStatsSettingsConfig(
            /* [in] */ Int64 bucketDuration,
            /* [in] */ Int64 rotateAgeMillis,
            /* [in] */ Int64 deleteAgeMillis)
            : mBucketDuration(bucketDuration)
            , mRotateAgeMillis(rotateAgeMillis)
            , mDeleteAgeMillis(deleteAgeMillis)
        { }

    public:
        Int64 mBucketDuration;
        Int64 mRotateAgeMillis;
        Int64 mDeleteAgeMillis;
    };

    /**
     * Settings that can be changed externally.
     */
    class NetworkStatsSettings : public ElRefBase
    {
    public:
        virtual CARAPI_(Int64) GetPollInterval() = 0;

        virtual CARAPI_(Int64) GetTimeCacheMaxAge() = 0;

        virtual CARAPI_(Boolean) GetSampleEnabled() = 0;

        virtual CARAPI_(Boolean) GetReportXtOverDev() = 0;

        virtual CARAPI_(AutoPtr<NetworkStatsSettingsConfig>) GetDevConfig() = 0;

        virtual CARAPI_(AutoPtr<NetworkStatsSettingsConfig>) GetXtConfig() = 0;

        virtual CARAPI_(AutoPtr<NetworkStatsSettingsConfig>) GetUidConfig() = 0;

        virtual CARAPI_(AutoPtr<NetworkStatsSettingsConfig>) GetUidTagConfig() = 0;

        virtual CARAPI_(Int64) GetGlobalAlertBytes(
            /* [in] */ Int64 def) = 0;

        virtual CARAPI_(Int64) GetDevPersistBytes(
            /* [in] */ Int64 def) = 0;

        virtual CARAPI_(Int64) GetXtPersistBytes(
            /* [in] */ Int64 def) = 0;

        virtual CARAPI_(Int64) GetUidPersistBytes(
            /* [in] */ Int64 def) = 0;

        virtual CARAPI_(Int64) GetUidTagPersistBytes(
            /* [in] */ Int64 def) = 0;
    };

private:
    class ConnReceiver : public BroadcastReceiver
    {
    public:
        ConnReceiver(
            /* [in] */ CNetworkStatsService* owner);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CNetworkStatsService::ConnReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CNetworkStatsService* mOwner;
    };

    class TetherReceiver : public BroadcastReceiver
    {
    public:
        TetherReceiver(
            /* [in] */ CNetworkStatsService* owner);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CNetworkStatsService::TetherReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CNetworkStatsService* mOwner;
    };

    class PollReceiver : public BroadcastReceiver
    {
    public:
        PollReceiver(
            /* [in] */ CNetworkStatsService* owner);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CNetworkStatsService::PollReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CNetworkStatsService* mOwner;
    };

    class RemovedReceiver : public BroadcastReceiver
    {
    public:
        RemovedReceiver(
            /* [in] */ CNetworkStatsService* owner);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CNetworkStatsService::RemovedReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CNetworkStatsService* mOwner;
    };

    class UserReceiver : public BroadcastReceiver
    {
    public:
        UserReceiver(
            /* [in] */ CNetworkStatsService* owner);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CNetworkStatsService::UserReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CNetworkStatsService* mOwner;
    };

    class ShutdownReceiver : public BroadcastReceiver
    {
    public:
        ShutdownReceiver(
            /* [in] */ CNetworkStatsService* owner);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CNetworkStatsService::ShutdownReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CNetworkStatsService* mOwner;
    };

    /**
     * Receiver that watches for {@link TelephonyManager} changes, such as
     * transitioning between network types.
     */

    // class PhoneListener// : public PhoneStateListener
    // {
    // public:
    //     PhoneListener(
    //         /* [in] */ CNetworkStatsService* host);
    //
    //     CARAPI_(void) OnDataConnectionStateChanged(
    //         /* [in] */ Int32 state,
    //         /* [in] */ Int32 networkType);
    // private:
    //     CNetworkStatsService* mHost;
    // };

    class DropBoxNonMonotonicObserver
        : public ElRefBase
        , public INonMonotonicObserver
    {
    public:
        DropBoxNonMonotonicObserver(
            /* [in] */ CNetworkStatsService* owner);

        CAR_INTERFACE_DECL()

        CARAPI FoundNonMonotonic(
            /* [in] */ INetworkStats* left,
            /* [in] */ Int32 leftIndex,
            /* [in] */ INetworkStats* right,
            /* [in] */ Int32 rightIndex,
            /* [in] */ IInterface* cookie);

    private:
        CNetworkStatsService* mOwner;
    };

    /**
     * Default external settings that read from
     * {@link android.provider.Settings.Global}.
     */
    class DefaultNetworkStatsSettings : public NetworkStatsSettings
    {
    public:
        DefaultNetworkStatsSettings(
            /* [in] */ IContext* context);

        CARAPI_(Int64) GetPollInterval();

        CARAPI_(Int64) GetTimeCacheMaxAge();

        CARAPI_(Boolean) GetSampleEnabled();

        CARAPI_(Boolean) GetReportXtOverDev();

        CARAPI_(AutoPtr<NetworkStatsSettingsConfig>) GetDevConfig();

        CARAPI_(AutoPtr<NetworkStatsSettingsConfig>) GetXtConfig();

        CARAPI_(AutoPtr<NetworkStatsSettingsConfig>) GetUidConfig();

        CARAPI_(AutoPtr<NetworkStatsSettingsConfig>) GetUidTagConfig();

        CARAPI_(Int64) GetGlobalAlertBytes(
            /* [in] */ Int64 def);

        CARAPI_(Int64) GetDevPersistBytes(
            /* [in] */ Int64 def);

        CARAPI_(Int64) GetXtPersistBytes(
            /* [in] */ Int64 def);

        CARAPI_(Int64) GetUidPersistBytes(
            /* [in] */ Int64 def);

        CARAPI_(Int64) GetUidTagPersistBytes(
            /* [in] */ Int64 def);

    private:
        CARAPI_(Int64) GetGlobalInt64(
            /* [in] */ const String& name,
            /* [in] */ Int64 def);

        CARAPI_(Boolean) GetGlobalBoolean(
            /* [in] */ const String& name,
            /* [in] */ Boolean def);

    private:
        AutoPtr<IContentResolver> mResolver;
    };

public:
    CNetworkStatsService();

    CARAPI BindConnectivityManager(
        /* [in] */ IIConnectivityManager* connManager);

    CARAPI_(void) SystemReady();

    //@Override
    CARAPI OpenSession(
        /* [out] */ INetworkStatsSession** comple);

    //@Override
    CARAPI GetNetworkTotalBytes(
        /* [in] */ INetworkTemplate* templ,
        /* [in] */ Int64 start,
        /* [in] */ Int64 end,
        /* [out] */ Int64* totalBytes);

    //@Override
    CARAPI GetDataLayerSnapshotForUid(
        /* [in] */ Int32 uid,
        /* [out] */ INetworkStats** datalayerOut);

    //@Override
    CARAPI GetMobileIfaces(
        /* [out, callee] */ ArrayOf<String>** mobileIfaces);

    //@Override
    CARAPI IncrementOperationCount(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 tag,
        /* [in] */ Int32 operationCount);

    //@Override
    CARAPI SetUidForeground(
        /* [in] */ Int32 uid,
        /* [in] */ Boolean uidForeground);

    //@Override
    CARAPI ForceUpdate();

    //@Override
    CARAPI AdvisePersistThreshold(
        /* [in] */ Int64 thresholdBytes);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ INetworkManagementService* networkManager,
        /* [in] */ IIAlarmManager* alarmManager);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ INetworkManagementService* networkManager,
        /* [in] */ IIAlarmManager* alarmManager,
        /* [in] */ ITrustedTime* time,
        /* [in] */ IFile* systemDir,
        /* [in] */ Handle32 settings);

protected:
    CARAPI_(void) Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* writer,
        /* [in] */ const ArrayOf<String>& args);

private:
    static CARAPI_(AutoPtr<IFile>) GetDefaultSystemDir();

    CARAPI_(AutoPtr<NetworkStatsRecorder>) BuildRecorder(
        /* [in] */ const String& prefix,
        /* [in] */ NetworkStatsSettingsConfig* config,
        /* [in] */ Boolean includeTags);

    CARAPI_(void) ShutdownLocked();

    CARAPI_(void) MaybeUpgradeLegacyStatsLocked();

    /**
     * Clear any existing {@link #ACTION_NETWORK_STATS_POLL} alarms, and
     * reschedule based on current {@link NetworkStatsSettings#getPollInterval()}.
     */
    CARAPI_(void) RegisterPollAlarmLocked();

    /**
     * Register for a global alert that is delivered through
     * {@link INetworkManagementEventObserver} once a threshold amount of data
     * has been transferred.
     */
    CARAPI_(void) RegisterGlobalAlert();

    /**
     * Return network summary, splicing between {@link #mDevStatsCached}
     * and {@link #mXtStatsCached} when appropriate.
     */
    CARAPI_(AutoPtr<INetworkStats>) InternalGetSummaryForNetwork(
        /* [in] */ INetworkTemplate* templ,
        /* [in] */ Int64 start,
        /* [in] */ Int64 end);

    /**
     * Return network history, splicing between {@link #mDevStatsCached}
     * and {@link #mXtStatsCached} when appropriate.
     */
    CARAPI_(AutoPtr<INetworkStatsHistory>) InternalGetHistoryForNetwork(
        /* [in] */ INetworkTemplate* templ,
        /* [in] */ Int32 fields);

    /**
     * Update {@link NetworkStatsRecorder} and {@link #mGlobalAlertBytes} to
     * reflect current {@link #mPersistThreshold} value. Always defers to
     * {@link Global} values when defined.
     */
    CARAPI_(void) UpdatePersistThresholds();

    CARAPI_(void) UpdateIfaces();

    /**
    * Inspect all current {@link NetworkState} to derive mapping from {@code
    * iface} to {@link NetworkStatsHistory}. When multiple {@link NetworkInfo}
    * are active on a single {@code iface}, they are combined under a single
    * {@link NetworkIdentitySet}.
    */
    CARAPI_(void) UpdateIfacesLocked();

    /**
     * Bootstrap initial stats snapshot, usually during {@link #systemReady()}
     * so we have baseline values without double-counting.
     */
    CARAPI_(void) BootstrapStatsLocked();

    CARAPI_(void) PerformPoll(
        /* [in] */ Int32 flags);

    CARAPI_(void) PerformPollLocked(
        /* [in] */ Int32 flags);

    /**
     * Sample recent statistics summary into {@link EventLog}.
     */
    CARAPI_(void) PerformSampleLocked();

    /**
    * Clean up {@link #mUidRecorder} after UID is removed.
    */
    CARAPI_(void) RemoveUidsLocked(
        /* [in] */ ArrayOf<Int32>* uids);

    /**
     * Clean up {@link #mUidRecorder} after user is removed.
     */
    CARAPI_(void) RemoveUserLocked(
        /* [in] */ Int32 userId);

    /**
     * Return snapshot of current UID statistics, including any
     * {@link TrafficStats#UID_TETHERING} and {@link #mUidOperations} values.
     */
    CARAPI GetNetworkStatsUidDetail(
        /* [out] */ INetworkStats** stats);

    /**
     * Return snapshot of current tethering statistics. Will return empty
     * {@link NetworkStats} if any problems are encountered.
     */
    CARAPI GetNetworkStatsTethering(
        /* [out] */ INetworkStats** stats);


    CARAPI AssertBandwidthControlEnabled();

    CARAPI_(Boolean) IsBandwidthControlEnabled();

    CARAPI_(void) HandleMsgPerformPoll(
        /* [in] */ Int32 flags);

    CARAPI_(void) HandleMsgUpdateIfaces();

    CARAPI_(void) HandleMsgRegisterGlobalAlert();

public:
    // @VisibleForTesting
    static const String ACTION_NETWORK_STATS_POLL;
    static const String ACTION_NETWORK_STATS_UPDATED;

private:
    static const String TAG;
    static const Boolean LOGV = FALSE;

    static const Int32 MSG_PERFORM_POLL;// = 1;
    static const Int32 MSG_UPDATE_IFACES;// = 2;
    static const Int32 MSG_REGISTER_GLOBAL_ALERT;// = 3;

    /** Flags to control detail level of poll event. */
    static const Int32 FLAG_PERSIST_NETWORK = 0x1;
    static const Int32 FLAG_PERSIST_UID = 0x2;
    static const Int32 FLAG_PERSIST_ALL = FLAG_PERSIST_NETWORK | FLAG_PERSIST_UID;
    static const Int32 FLAG_PERSIST_FORCE = 0x100;

    static const String TAG_NETSTATS_ERROR;

    static const String PREFIX_DEV;
    static const String PREFIX_XT;
    static const String PREFIX_UID;
    static const String PREFIX_UID_TAG;

    AutoPtr<IContext> mContext;
    AutoPtr<INetworkManagementService> mNetworkManager;
    AutoPtr<IIAlarmManager> mAlarmManager;
    AutoPtr<ITrustedTime> mTime;
    AutoPtr<ITelephonyManager> mTeleManager;
    AutoPtr<NetworkStatsSettings> mSettings;
    AutoPtr<IFile> mSystemDir;
    AutoPtr<IFile> mBaseDir;
    AutoPtr<IPowerManagerWakeLock> mWakeLock;
    AutoPtr<IIConnectivityManager> mConnManager;
    AutoPtr<IPendingIntent> mPollIntent;

    /** Set of currently active ifaces. */
    HashMap<String, AutoPtr<NetworkIdentitySet> > mActiveIfaces;
    /** Current default active iface. */
    String mActiveIface;
    /** Set of any ifaces associated with mobile networks since boot. */
    AutoPtr< ArrayOf<String> > mMobileIfaces;

    AutoPtr<DropBoxNonMonotonicObserver> mNonMonotonicObserver;

    AutoPtr<NetworkStatsRecorder> mDevRecorder;
    AutoPtr<NetworkStatsRecorder> mXtRecorder;
    AutoPtr<NetworkStatsRecorder> mUidRecorder;
    AutoPtr<NetworkStatsRecorder> mUidTagRecorder;

    /** Cached {@link #mDevRecorder} stats. */
    AutoPtr<NetworkStatsCollection> mDevStatsCached;
    /** Cached {@link #mXtRecorder} stats. */
    AutoPtr<NetworkStatsCollection> mXtStatsCached;

    /** Current counter sets for each UID. */
    HashMap<Int32, Int32> mActiveUidCounterSet;

    /** Data layer operation counters for splicing into other structures. */
    AutoPtr<INetworkStats> mUidOperations;// = new NetworkStats(0L, 10);

    AutoPtr<IHandlerThread> mHandlerThread;
    AutoPtr<IHandler> mHandler;
    AutoPtr<IHandlerCallback> mHandlerCallback;

    Boolean mSystemReady;
    Int64 mPersistThreshold;
    Int64 mGlobalAlertBytes;

    AutoPtr<BroadcastReceiver> mConnReceiver;
    AutoPtr<BroadcastReceiver> mTetherReceiver;
    AutoPtr<BroadcastReceiver> mPollReceiver;
    AutoPtr<BroadcastReceiver> mRemovedReceiver;
    AutoPtr<BroadcastReceiver> mUserReceiver;
    AutoPtr<BroadcastReceiver> mShutdownReceiver;

    AutoPtr<INetworkManagementEventObserver> mAlertObserver;

    Int32 mLastPhoneState;
    Int32 mLastPhoneNetworkType;

    //AutoPtr<PhoneListener> mPhoneListener;

    friend class CNetworkStatsSession;
    friend class CNetworkStatsServiceAlertObserber;
};

} // namespace Net
} // namespace Server
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_NET_CNETWORKSTATSSERVICE_H__
