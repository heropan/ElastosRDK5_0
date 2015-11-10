
#include "elastos/droid/app/IAppOpsManager::h"

using Elastos::Droid::App::Usage::UsageStatsManager;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Media::AudioAttributes.AttributeUsage;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IIBinder;
using Elastos::Droid::Os::IParcel;
using Elastos::Droid::Os::IParcelable;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::IRemoteException;
using Elastos::Droid::Os::IUserManager;
using Elastos::Droid::Utility::CArrayMap;

using Elastos::Droid::Internal::App::IAppOpsCallback;
using Elastos::Droid::Internal::App::IAppOpsService;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

namespace Elastos {
namespace Droid {
namespace App {

/**
 * Class holding all of the operation information associated with an app.
 * @hide
 */
class PackageOps
    : public Object
    , public IAppOpsManagerPackageOps
    , public IParcelable
{
public:
    PackageOps();

    CARAPI constructor(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [in] */ IList* entries)
    {
        mPackageName = packageName;
        mUid = uid;
        mEntries = entries;
    }

    CARAPI GetPackageName(
        /* [out] */ String* str)
    {
        return mPackageName;
    }

    CARAPI GetUid(
        /* [out] */ Int32* uid)
    {
        return mUid;
    }

    CARAPI GetOps(
        /* [out] */ IList** ops)
    {
        return mEntries;
    }

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest)
    {
        dest.writeString(mPackageName);
        dest.writeInt(mUid);
        dest.writeInt(mEntries.size());
        for (Int32 i=0; i<mEntries.size(); i++) {
            mEntries.get(i).writeToParcel(dest, flags);
        }
    }

    CARAPI ReadFromParcel(
        /* [out] */ IParcel* source)
    {
        mPackageName = source.readString();
        mUid = source.readInt();
        mEntries = new ArrayList<OpEntry>();
        final Int32 N = source.readInt();
        for (Int32 i=0; i<N; i++) {
            mEntries.add(OpEntry.CREATOR.createFromParcel(source));
        }
    }

private:
    String mPackageName;
    Int32 mUid;
    AutoPtr<IList> mEntries;//List<OpEntry>
};

/**
 * Class holding the information about one unique operation of an application.
 * @hide
 */
class OpEntry
    : public Object
    , public IAppOpsManagerOpEntry
    , public IParcelable
{
public:
    OpEntry();

    CARAPI constructor(
        /* [in] */ Int32 op,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 time,
        /* [in] */ Int32 rejectTime,
        /* [in] */ Int32 duration)
    {
        mOp = op;
        mMode = mode;
        mTime = time;
        mRejectTime = rejectTime;
        mDuration = duration;
    }

    CARAPI GetOp(
        /* [out] */ Int32* op)
    {
        return mOp;
    }

    CARAPI GetMode(
        /* [out] */ Int32* mode)
    {
        return mMode;
    }

    CARAPI GetTime(
        /* [out] */ Int64* time)
    {
        return mTime;
    }

    CARAPI GetRejectTime(
        /* [out] */ Int64* time)
    {
        return mRejectTime;
    }

    CARAPI IsRunning(
        /* [out] */ Boolean* running)
    {
        return mDuration == -1;
    }

    CARAPI GetDuration(
        /* [out] */ Int32* duration)
    {
        return mDuration == -1 ? (Int32)(System.currentTimeMillis()-mTime) : mDuration;
    }

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest)
    {
        dest.writeInt(mOp);
        dest.writeInt(mMode);
        dest.writeLong(mTime);
        dest.writeLong(mRejectTime);
        dest.writeInt(mDuration);
    }

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source)
    {
        mOp = source.readInt();
        mMode = source.readInt();
        mTime = source.readLong();
        mRejectTime = source.readLong();
        mDuration = source.readInt();
    }

private:
    Int32 mOp;
    Int32 mMode;
    Int64 mTime;
    Int64 mRejectTime;
    Int32 mDuration;
};

/**
 * Callback for notification of changes to operation state.
 * This allows you to see the raw op codes instead of strings.
 * @hide
 */
class OnOpChangedInternalListener
    : public Object
    , public IOnOpChangedListener
{
public:
    ECode OnOpChanged(
        /* [in] */ const String& op,
        /* [in] */String packageName) { }

    ECode OnOpChanged(
        /* [in] */ Int32 op,
        /* [in] */ const String& packageName) { }
};

//========================================================================
//
//========================================================================

static AutoPtr<IBinder> AppOpsManager::sToken;

/**
 * This maps each operation to the operation that serves as the
 * switch to determine whether it is allowed.  Generally this is
 * a 1:1 mapping, but for some things (like location) that have
 * multiple low-level operations being tracked that should be
 * presented to the user as one switch then this can be used to
 * make them all controlled by the same single operation.
 */
Int32 AppOpsManager::sOpToSwitch[] = new Int32[] {
        IAppOpsManager::OP_COARSE_LOCATION,
        IAppOpsManager::OP_COARSE_LOCATION,
        IAppOpsManager::OP_COARSE_LOCATION,
        IAppOpsManager::OP_VIBRATE,
        IAppOpsManager::OP_READ_CONTACTS,
        IAppOpsManager::OP_WRITE_CONTACTS,
        IAppOpsManager::OP_READ_CALL_LOG,
        IAppOpsManager::OP_WRITE_CALL_LOG,
        IAppOpsManager::OP_READ_CALENDAR,
        IAppOpsManager::OP_WRITE_CALENDAR,
        IAppOpsManager::OP_COARSE_LOCATION,
        IAppOpsManager::OP_POST_NOTIFICATION,
        IAppOpsManager::OP_COARSE_LOCATION,
        IAppOpsManager::OP_CALL_PHONE,
        IAppOpsManager::OP_READ_SMS,
        IAppOpsManager::OP_WRITE_SMS,
        IAppOpsManager::OP_RECEIVE_SMS,
        IAppOpsManager::OP_RECEIVE_SMS,
        IAppOpsManager::OP_RECEIVE_SMS,
        IAppOpsManager::OP_RECEIVE_SMS,
        IAppOpsManager::OP_SEND_SMS,
        IAppOpsManager::OP_READ_SMS,
        IAppOpsManager::OP_WRITE_SMS,
        IAppOpsManager::OP_WRITE_SETTINGS,
        IAppOpsManager::OP_SYSTEM_ALERT_WINDOW,
        IAppOpsManager::OP_ACCESS_NOTIFICATIONS,
        IAppOpsManager::OP_CAMERA,
        IAppOpsManager::OP_RECORD_AUDIO,
        IAppOpsManager::OP_PLAY_AUDIO,
        IAppOpsManager::OP_READ_CLIPBOARD,
        IAppOpsManager::OP_WRITE_CLIPBOARD,
        IAppOpsManager::OP_TAKE_MEDIA_BUTTONS,
        IAppOpsManager::OP_TAKE_AUDIO_FOCUS,
        IAppOpsManager::OP_AUDIO_MASTER_VOLUME,
        IAppOpsManager::OP_AUDIO_VOICE_VOLUME,
        IAppOpsManager::OP_AUDIO_RING_VOLUME,
        IAppOpsManager::OP_AUDIO_MEDIA_VOLUME,
        IAppOpsManager::OP_AUDIO_ALARM_VOLUME,
        IAppOpsManager::OP_AUDIO_NOTIFICATION_VOLUME,
        IAppOpsManager::OP_AUDIO_BLUETOOTH_VOLUME,
        IAppOpsManager::OP_WAKE_LOCK,
        IAppOpsManager::OP_COARSE_LOCATION,
        IAppOpsManager::OP_COARSE_LOCATION,
        IAppOpsManager::OP_GET_USAGE_STATS,
        IAppOpsManager::OP_MUTE_MICROPHONE,
        IAppOpsManager::OP_TOAST_WINDOW,
        IAppOpsManager::OP_PROJECT_MEDIA,
        IAppOpsManager::OP_ACTIVATE_VPN,
};

String AppOpsManager::sOpToString[] = new String[] {
        IAppOpsManager::OPSTR_COARSE_LOCATION,
        IAppOpsManager::OPSTR_FINE_LOCATION,
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        String(NULL),
        IAppOpsManager::OPSTR_MONITOR_LOCATION,
        IAppOpsManager::OPSTR_MONITOR_HIGH_POWER_LOCATION,
        IAppOpsManager::OPSTR_GET_USAGE_STATS,
        String(NULL),
        String(NULL),
        String(NULL),
        IAppOpsManager::OPSTR_ACTIVATE_VPN,
};

/**
 * This provides a simple name for each operation to be used
 * in debug output.
 */
String AppOpsManager::sOpNames[] = {
        String("COARSE_LOCATION"),
        String("FINE_LOCATION"),
        String("GPS"),
        String("VIBRATE"),
        String("READ_CONTACTS"),
        String("WRITE_CONTACTS"),
        String("READ_CALL_LOG"),
        String("WRITE_CALL_LOG"),
        String("READ_CALENDAR"),
        String("WRITE_CALENDAR"),
        String("WIFI_SCAN"),
        String("POST_NOTIFICATION"),
        String("NEIGHBORING_CELLS"),
        String("CALL_PHONE"),
        String("READ_SMS"),
        String("WRITE_SMS"),
        String("RECEIVE_SMS"),
        String("RECEIVE_EMERGECY_SMS"),
        String("RECEIVE_MMS"),
        String("RECEIVE_WAP_PUSH"),
        String("SEND_SMS"),
        String("READ_ICC_SMS"),
        String("WRITE_ICC_SMS"),
        String("WRITE_SETTINGS"),
        String("SYSTEM_ALERT_WINDOW"),
        String("ACCESS_NOTIFICATIONS"),
        String("CAMERA"),
        String("RECORD_AUDIO"),
        String("PLAY_AUDIO"),
        String("READ_CLIPBOARD"),
        String("WRITE_CLIPBOARD"),
        String("TAKE_MEDIA_BUTTONS"),
        String("TAKE_AUDIO_FOCUS"),
        String("AUDIO_MASTER_VOLUME"),
        String("AUDIO_VOICE_VOLUME"),
        String("AUDIO_RING_VOLUME"),
        String("AUDIO_MEDIA_VOLUME"),
        String("AUDIO_ALARM_VOLUME"),
        String("AUDIO_NOTIFICATION_VOLUME"),
        String("AUDIO_BLUETOOTH_VOLUME"),
        String("WAKE_LOCK"),
        String("MONITOR_LOCATION"),
        String("MONITOR_HIGH_POWER_LOCATION"),
        String("GET_USAGE_STATS"),
        String("MUTE_MICROPHONE"),
        String("TOAST_WINDOW"),
        String("PROJECT_MEDIA"),
        String("ACTIVATE_VPN"),
};

String AppOpsManager::sOpPerms[] =  {
        Manifest::permission::ACCESS_COARSE_LOCATION,
        Manifest::permission::ACCESS_FINE_LOCATION,
        String(NULL),
        Manifest::permission::VIBRATE,
        Manifest::permission::READ_CONTACTS,
        Manifest::permission::WRITE_CONTACTS,
        Manifest::permission::READ_CALL_LOG,
        Manifest::permission::WRITE_CALL_LOG,
        Manifest::permission::READ_CALENDAR,
        Manifest::permission::WRITE_CALENDAR,
        Manifest::permission::ACCESS_WIFI_STATE,
        String(NULL), // no permission required for notifications
        String(NULL), // neighboring cells shares the coarse location perm
        Manifest::permission::CALL_PHONE,
        Manifest::permission::READ_SMS,
        Manifest::permission::WRITE_SMS,
        Manifest::permission::RECEIVE_SMS,
        Manifest::permission::RECEIVE_EMERGENCY_BROADCAST,
        Manifest::permission::RECEIVE_MMS,
        Manifest::permission::RECEIVE_WAP_PUSH,
        Manifest::permission::SEND_SMS,
        Manifest::permission::READ_SMS,
        Manifest::permission::WRITE_SMS,
        Manifest::permission::WRITE_SETTINGS,
        Manifest::permission::SYSTEM_ALERT_WINDOW,
        Manifest::permission::ACCESS_NOTIFICATIONS,
        Manifest::permission::CAMERA,
        Manifest::permission::RECORD_AUDIO,
        String(NULL), // no permission for playing audio
        String(NULL), // no permission for reading clipboard
        String(NULL), // no permission for writing clipboard
        String(NULL), // no permission for taking media buttons
        String(NULL), // no permission for taking audio focus
        String(NULL), // no permission for changing master volume
        String(NULL), // no permission for changing voice volume
        String(NULL), // no permission for changing ring volume
        String(NULL), // no permission for changing media volume
        String(NULL), // no permission for changing alarm volume
        String(NULL), // no permission for changing notification volume
        String(NULL), // no permission for changing bluetooth volume
        Manifest::permission::WAKE_LOCK,
        String(NULL), // no permission for generic location monitoring
        String(NULL), // no permission for high power location monitoring
        Manifest::permission::PACKAGE_USAGE_STATS,
        String(NULL), // no permission for muting/unmuting microphone
        String(NULL), // no permission for displaying toasts
        String(NULL), // no permission for projecting media
        String(NULL), // no permission for activating vpn
};

static String sOpRestrictions[] = {
        IUserManager::DISALLOW_SHARE_LOCATION, //COARSE_LOCATION
        IUserManager::DISALLOW_SHARE_LOCATION, //FINE_LOCATION
        IUserManager::DISALLOW_SHARE_LOCATION, //GPS
        String(NULL), //VIBRATE
        String(NULL), //READ_CONTACTS
        String(NULL), //WRITE_CONTACTS
        IUserManager::DISALLOW_OUTGOING_CALLS, //READ_CALL_LOG
        IUserManager::DISALLOW_OUTGOING_CALLS, //WRITE_CALL_LOG
        String(NULL), //READ_CALENDAR
        String(NULL), //WRITE_CALENDAR
        IUserManager::DISALLOW_SHARE_LOCATION, //WIFI_SCAN
        String(NULL), //POST_NOTIFICATION
        String(NULL), //NEIGHBORING_CELLS
        String(NULL), //CALL_PHONE
        IUserManager::DISALLOW_SMS, //READ_SMS
        IUserManager::DISALLOW_SMS, //WRITE_SMS
        IUserManager::DISALLOW_SMS, //RECEIVE_SMS
        String(NULL), //RECEIVE_EMERGENCY_SMS
        IUserManager::DISALLOW_SMS, //RECEIVE_MMS
        String(NULL), //RECEIVE_WAP_PUSH
        IUserManager::DISALLOW_SMS, //SEND_SMS
        IUserManager::DISALLOW_SMS, //READ_ICC_SMS
        IUserManager::DISALLOW_SMS, //WRITE_ICC_SMS
        String(NULL), //WRITE_SETTINGS
        IUserManager::DISALLOW_CREATE_WINDOWS, //SYSTEM_ALERT_WINDOW
        String(NULL), //ACCESS_NOTIFICATIONS
        String(NULL), //CAMERA
        String(NULL), //RECORD_AUDIO
        String(NULL), //PLAY_AUDIO
        String(NULL), //READ_CLIPBOARD
        String(NULL), //WRITE_CLIPBOARD
        String(NULL), //TAKE_MEDIA_BUTTONS
        String(NULL), //TAKE_AUDIO_FOCUS
        IUserManager::DISALLOW_ADJUST_VOLUME, //AUDIO_MASTER_VOLUME
        IUserManager::DISALLOW_ADJUST_VOLUME, //AUDIO_VOICE_VOLUME
        IUserManager::DISALLOW_ADJUST_VOLUME, //AUDIO_RING_VOLUME
        IUserManager::DISALLOW_ADJUST_VOLUME, //AUDIO_MEDIA_VOLUME
        IUserManager::DISALLOW_ADJUST_VOLUME, //AUDIO_ALARM_VOLUME
        IUserManager::DISALLOW_ADJUST_VOLUME, //AUDIO_NOTIFICATION_VOLUME
        IUserManager::DISALLOW_ADJUST_VOLUME, //AUDIO_BLUETOOTH_VOLUME
        String(NULL), //WAKE_LOCK
        IUserManager::DISALLOW_SHARE_LOCATION, //MONITOR_LOCATION
        IUserManager::DISALLOW_SHARE_LOCATION, //MONITOR_HIGH_POWER_LOCATION
        String(NULL), //GET_USAGE_STATS
        IUserManager::DISALLOW_UNMUTE_MICROPHONE, // MUTE_MICROPHONE
        IUserManager::DISALLOW_CREATE_WINDOWS, // TOAST_WINDOW
        String(NULL), //PROJECT_MEDIA
        IUserManager::DISALLOW_CONFIG_VPN, // ACTIVATE_VPN
};

Boolean AppOpsManager::sOpAllowSystemRestrictionBypass[] = {
        FALSE, //COARSE_LOCATION
        FALSE, //FINE_LOCATION
        FALSE, //GPS
        FALSE, //VIBRATE
        FALSE, //READ_CONTACTS
        FALSE, //WRITE_CONTACTS
        FALSE, //READ_CALL_LOG
        FALSE, //WRITE_CALL_LOG
        FALSE, //READ_CALENDAR
        FALSE, //WRITE_CALENDAR
        TRUE, //WIFI_SCAN
        FALSE, //POST_NOTIFICATION
        FALSE, //NEIGHBORING_CELLS
        FALSE, //CALL_PHONE
        FALSE, //READ_SMS
        FALSE, //WRITE_SMS
        FALSE, //RECEIVE_SMS
        FALSE, //RECEIVE_EMERGECY_SMS
        FALSE, //RECEIVE_MMS
        FALSE, //RECEIVE_WAP_PUSH
        FALSE, //SEND_SMS
        FALSE, //READ_ICC_SMS
        FALSE, //WRITE_ICC_SMS
        FALSE, //WRITE_SETTINGS
        TRUE, //SYSTEM_ALERT_WINDOW
        FALSE, //ACCESS_NOTIFICATIONS
        FALSE, //CAMERA
        FALSE, //RECORD_AUDIO
        FALSE, //PLAY_AUDIO
        FALSE, //READ_CLIPBOARD
        FALSE, //WRITE_CLIPBOARD
        FALSE, //TAKE_MEDIA_BUTTONS
        FALSE, //TAKE_AUDIO_FOCUS
        FALSE, //AUDIO_MASTER_VOLUME
        FALSE, //AUDIO_VOICE_VOLUME
        FALSE, //AUDIO_RING_VOLUME
        FALSE, //AUDIO_MEDIA_VOLUME
        FALSE, //AUDIO_ALARM_VOLUME
        FALSE, //AUDIO_NOTIFICATION_VOLUME
        FALSE, //AUDIO_BLUETOOTH_VOLUME
        FALSE, //WAKE_LOCK
        FALSE, //MONITOR_LOCATION
        FALSE, //MONITOR_HIGH_POWER_LOCATION
        FALSE, //GET_USAGE_STATS
        FALSE, //MUTE_MICROPHONE
        TRUE, //TOAST_WINDOW
        FALSE, //PROJECT_MEDIA
        FALSE, //ACTIVATE_VPN
};

Int32 AppOpsManager::sOpDefaultMode[] = {
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_IGNORED, // OP_WRITE_SMS
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_DEFAULT, // OP_GET_USAGE_STATS
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_ALLOWED,
        IAppOpsManager::MODE_IGNORED, // OP_PROJECT_MEDIA
        IAppOpsManager::MODE_IGNORED, // OP_ACTIVATE_VPN
};

Boolean AppOpsManager::sOpDisableReset[] = {
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        TRUE,      // OP_WRITE_SMS
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
};

static AutoPtr<HashMap<String, Int32> > InitOpStrToOp()
{
    AutoPtr<HashMap<String, Int32> > map = new HashMap<String, Int32>();
    for (Int32 i = 0; i < IAppOpsManager::_NUM_OP; i++) {
        if (!AppOpsManager::sOpToString[i].IsNull()) {
            (*map)[AppOpsManager::sOpToString[i]] = i;
        }
    }
    return map;
}

AutoPtr<HashMap<String, Int32> > AppOpsManager::sOpStrToOp = InitOpStrToOp();

CAR_INTERFACE_IMPL(AppOpsManager, Object, IAppOpsManager)

AppOpsManager();

virtual ~AppOpsManager();

/**
 * Retrieve the op switch that controls the given operation.
 * @hide
 */
Int32 OpToSwitch(
    /* [in] */ Int32 op)
{
    return sOpToSwitch[op];
}

/**
 * Retrieve a non-localized name for the operation, for debugging output.
 * @hide
 */
String OpToName(
    /* [in] */ Int32 op)
{
    if (op == OP_NONE) return "NONE";
    return op < sOpNames.length ? sOpNames[op] : ("Unknown(" + op + ")");
}

/**
 * Retrieve the permission associated with an operation, or null if there is not one.
 * @hide
 */
String OpToPermission(
    /* [in] */ Int32 op)
{
    return sOpPerms[op];
}

/**
 * Retrieve the user restriction associated with an operation, or null if there is not one.
 * @hide
 */
String OpToRestriction(
    /* [in] */ Int32 op)
{
    return sOpRestrictions[op];
}

/**
 * Retrieve whether the op allows the system (and system ui) to
 * bypass the user restriction.
 * @hide
 */
Boolean OpAllowSystemBypassRestriction(
    /* [in] */ Int32 op)
{
    return sOpAllowSystemRestrictionBypass[op];
}

/**
 * Retrieve the default mode for the operation.
 * @hide
 */
Int32 OpToDefaultMode(
    /* [in] */ Int32 op)
{
    return sOpDefaultMode[op];
}

/**
 * Retrieve whether the op allows itself to be reset.
 * @hide
 */
Boolean OpAllowsReset(
    /* [in] */ Int32 op)
{
    return !sOpDisableReset[op];
}

CARAPI constructor(
    /* [in] */ IContext* context,
    /* [in] */ IIAppOpsService* service)
{
    mContext = context;
    mService = service;
}

/**
 * Retrieve current operation state for all applications.
 *
 * @param ops The set of operations you are interested in, or null if you want all of them.
 * @hide
 */
ECode GetPackagesForOps(
    /* [in] */ ArrayOf<Int32>* ops,
    /* [out] */ IList** ops) //List<IAppOpsManager::PackageOps>
{
    try {
        return mService.getPackagesForOps(ops);
    } catch (RemoteException e) {
    }
    return null;
}

/**
 * Retrieve current operation state for one application.
 *
 * @param uid The uid of the application of interest.
 * @param packageName The name of the application of interest.
 * @param ops The set of operations you are interested in, or null if you want all of them.
 * @hide
 */
ECode GetOpsForPackage(
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [in] */ ArrayOf<Int32>* ops,
    /* [out] */ IList** list) //List<IAppOpsManager::PackageOps>
{
    try {
        return mService.getOpsForPackage(uid, packageName, ops);
    } catch (RemoteException e) {
    }
    return null;
}

/** @hide */
ECode SetMode(
    /* [in] */ Int32 code,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [in] */ Int32 mode)
{
    try {
        mService.setMode(code, uid, packageName, mode);
    } catch (RemoteException e) {
    }
}

/**
 * Set a non-persisted restriction on an audio operation at a stream-level.
 * Restrictions are temporary additional constraints imposed on top of the persisted rules
 * defined by {@link #setMode}.
 *
 * @param code The operation to restrict.
 * @param usage The {@link android.media.AudioAttributes} usage value.
 * @param mode The restriction mode (MODE_IGNORED,MODE_ERRORED) or MODE_ALLOWED to unrestrict.
 * @param exceptionPackages Optional list of packages to exclude from the restriction.
 * @hide
 */
ECode SetRestriction(
    /* [in] */ Int32 code,
    /* [in] */ Int32 usage,
    /* [in] */ Int32 mode,
    /* [in] */ ArrayOf<String>* exceptionPackages)
{
    try {
        final Int32 uid = Binder.getCallingUid();
        mService.setAudioRestriction(code, usage, uid, mode, exceptionPackages);
    } catch (RemoteException e) {
    }
}

/** @hide */
ECode ResetAllModes()
{
    try {
        mService.resetAllModes();
    } catch (RemoteException e) {
    }
}

/**
 * Monitor for changes to the operating mode for the given op in the given app package.
 * @param op The operation to monitor, one of OPSTR_*.
 * @param packageName The name of the application to monitor.
 * @param callback Where to report changes.
 */
ECode StartWatchingMode(
    /* [in] */ const String& op,
    /* [in] */ const String& packageName,
    /* [in] */ IAppOpsManagerOnOpChangedListener* callback)
{
    startWatchingMode(strOpToOp(op), packageName, callback);
}

/**
 * Monitor for changes to the operating mode for the given op in the given app package.
 * @param op The operation to monitor, one of OP_*.
 * @param packageName The name of the application to monitor.
 * @param callback Where to report changes.
 * @hide
 */
ECode StartWatchingMode(
    /* [in] */ Int32 op,
    /* [in] */ const String& packageName,
    /* [in] */ IAppOpsManagerOnOpChangedListener* callback)
{
    synchronized (mModeWatchers) {
        IAppOpsCallback cb = mModeWatchers.get(callback);
        if (cb == null) {
            cb = new IAppOpsCallback.Stub() {
                ECode opChanged(Int32 op, String packageName) {
                    if (callback instanceof OnOpChangedInternalListener) {
                        ((OnOpChangedInternalListener)callback).onOpChanged(op, packageName);
                    }
                    if (sOpToString[op] != null) {
                        callback.onOpChanged(sOpToString[op], packageName);
                    }
                }
            };
            mModeWatchers.put(callback, cb);
        }
        try {
            mService.startWatchingMode(op, packageName, cb);
        } catch (RemoteException e) {
        }
    }
}

/**
 * Stop monitoring that was previously started with {@link #startWatchingMode}.  All
 * monitoring associated with this callback will be removed.
 */
ECode StopWatchingMode(
    /* [in] */ IAppOpsManagerOnOpChangedListener* callback)
{
    synchronized (mModeWatchers) {
        IAppOpsCallback cb = mModeWatchers.get(callback);
        if (cb != null) {
            try {
                mService.stopWatchingMode(cb);
            } catch (RemoteException e) {
            }
        }
    }
}

private String BuildSecurityExceptionMsg(
    /* [in] */ Int32 op,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName)
{
    return packageName + " from uid " + uid + " not allowed to perform " + sOpNames[op];
}

/**
 * {@hide}
 */
CARAPI StrOpToOp(
    /* [in] */ const String& op,
    /* [out] */ Int32* result)
{
    Integer val = sOpStrToOp.get(op);
    if (val == null) {
        throw new IllegalArgumentException("Unknown operation string: " + op);
    }
    return val;
}

ECode CheckOp(
    /* [in] */ const String& op,
    /* [in] */ Int32 uid, .
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    return checkOp(strOpToOp(op), uid, packageName);
}

ECode CheckOpNoThrow(
    /* [in] */ const String& op,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    return checkOpNoThrow(strOpToOp(op), uid, packageName);
}

ECode NoteOp(
    /* [in] */ const String& op,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    return noteOp(strOpToOp(op), uid, packageName);
}

ECode NoteOpNoThrow(
    /* [in] */ const String& op,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    return noteOpNoThrow(strOpToOp(op), uid, packageName);
}

/**
 * Report that an application has started executing a long-running operation.  Note that you
 * must pass in both the uid and name of the application to be checked; this function will
 * verify that these two match, and if not, return {@link #MODE_IGNORED}.  If this call
 * succeeds, the last execution time of the operation for this app will be updated to
 * the current time and the operation will be marked as "running".  In this case you must
 * later call {@link #FinishOp(String, Int32, String)} to report when the application is no
 * longer performing the operation.
 * @param op The operation to start.  One of the OPSTR_* constants.
 * @param uid The user id of the application attempting to perform the operation.
 * @param packageName The name of the application attempting to perform the operation.
 * @return Returns {@link #MODE_ALLOWED} if the operation is allowed, or
 * {@link #MODE_IGNORED} if it is not allowed and should be silently ignored (without
 * causing the app to crash).
 * @throws SecurityException If the app has been configured to crash on this op.
 */
ECode StartOp(
    /* [in] */ const String& op,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    return startOp(strOpToOp(op), uid, packageName);
}

/**
 * Like {@link #startOp} but instead of throwing a {@link SecurityException} it
 * returns {@link #MODE_ERRORED}.
 */
ECode StartOpNoThrow(
    /* [in] */ const String& op,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    return startOpNoThrow(strOpToOp(op), uid, packageName);
}

/**
 * Report that an application is no longer performing an operation that had previously
 * been started with {@link #startOp(String, Int32, String)}.  There is no validation of input
 * or result; the parameters supplied here must be the exact same ones previously passed
 * in when starting the operation.
 */
ECode FinishOp(
    /* [in] */ const String& op,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName)
{
    FinishOp(strOpToOp(op), uid, packageName);
}

/**
 * Do a quick check for whether an application might be able to perform an operation.
 * This is <em>not</em> a security check; you must use {@link #noteOp(Int32, Int32, String)}
 * or {@link #startOp(Int32, Int32, String)} for your actual security checks, which also
 * ensure that the given uid and package name are consistent.  This function can just be
 * used for a quick check to see if an operation has been disabled for the application,
 * as an early reject of some work.  This does not modify the time stamp or other data
 * about the operation.
 * @param op The operation to check.  One of the OP_* constants.
 * @param uid The user id of the application attempting to perform the operation.
 * @param packageName The name of the application attempting to perform the operation.
 * @return Returns {@link #MODE_ALLOWED} if the operation is allowed, or
 * {@link #MODE_IGNORED} if it is not allowed and should be silently ignored (without
 * causing the app to crash).
 * @throws SecurityException If the app has been configured to crash on this op.
 * @hide
 */
ECode CheckOp(
    /* [in] */ Int32 op,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    try {
        Int32 mode = mService.checkOperation(op, uid, packageName);
        if (mode == MODE_ERRORED) {
            throw new SecurityException(buildSecurityExceptionMsg(op, uid, packageName));
        }
        return mode;
    } catch (RemoteException e) {
    }
    return MODE_IGNORED;
}

/**
 * Like {@link #checkOp} but instead of throwing a {@link SecurityException} it
 * returns {@link #MODE_ERRORED}.
 * @hide
 */
ECode CheckOpNoThrow(
    /* [in] */ Int32 op,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    try {
        return mService.checkOperation(op, uid, packageName);
    } catch (RemoteException e) {
    }
    return MODE_IGNORED;
}

/**
 * Do a quick check to validate if a package name belongs to a UID.
 *
 * @throws SecurityException if the package name doesn't belong to the given
 *             UID, or if ownership cannot be verified.
 */
ECode CheckPackage(
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName)
{
    try {
        if (mService.checkPackage(uid, packageName) != MODE_ALLOWED) {
            throw new SecurityException(
                    "Package " + packageName + " does not belong to " + uid);
        }
    } catch (RemoteException e) {
        throw new SecurityException("Unable to verify package ownership", e);
    }
}

/**
 * Like {@link #checkOp} but at a stream-level for audio operations.
 * @hide
 */
ECode CheckAudioOp(
    /* [in] */ Int32 op,
    /* [in] */ Int32 stream,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    try {
        final Int32 mode = mService.checkAudioOperation(op, stream, uid, packageName);
        if (mode == MODE_ERRORED) {
            throw new SecurityException(buildSecurityExceptionMsg(op, uid, packageName));
        }
        return mode;
    } catch (RemoteException e) {
    }
    return MODE_IGNORED;
}

/**
 * Like {@link #checkAudioOp} but instead of throwing a {@link SecurityException} it
 * returns {@link #MODE_ERRORED}.
 * @hide
 */
ECode CheckAudioOpNoThrow(
    /* [in] */ Int32 op,
    /* [in] */ Int32 stream,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    try {
        return mService.checkAudioOperation(op, stream, uid, packageName);
    } catch (RemoteException e) {
    }
    return MODE_IGNORED;
}

/**
 * Make note of an application performing an operation.  Note that you must pass
 * in both the uid and name of the application to be checked; this function will verify
 * that these two match, and if not, return {@link #MODE_IGNORED}.  If this call
 * succeeds, the last execution time of the operation for this app will be updated to
 * the current time.
 * @param op The operation to note.  One of the OP_* constants.
 * @param uid The user id of the application attempting to perform the operation.
 * @param packageName The name of the application attempting to perform the operation.
 * @return Returns {@link #MODE_ALLOWED} if the operation is allowed, or
 * {@link #MODE_IGNORED} if it is not allowed and should be silently ignored (without
 * causing the app to crash).
 * @throws SecurityException If the app has been configured to crash on this op.
 * @hide
 */
ECode NoteOp(
    /* [in] */ Int32 op,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    try {
        Int32 mode = mService.noteOperation(op, uid, packageName);
        if (mode == MODE_ERRORED) {
            throw new SecurityException(buildSecurityExceptionMsg(op, uid, packageName));
        }
        return mode;
    } catch (RemoteException e) {
    }
    return MODE_IGNORED;
}

/**
 * Like {@link #noteOp} but instead of throwing a {@link SecurityException} it
 * returns {@link #MODE_ERRORED}.
 * @hide
 */
ECode NoteOpNoThrow(
    /* [in] */ Int32 op,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    try {
        return mService.noteOperation(op, uid, packageName);
    } catch (RemoteException e) {
    }
    return MODE_IGNORED;
}

/** @hide */
ECode NoteOp(
    /* [in] */ Int32 op,
    /* [out] */ Int32* result)
{
    return noteOp(op, Process.myUid(), mContext.getOpPackageName());
}

/** @hide */
CARAPI GetToken(
    /* [in] */ IIAppOpsService* service,
    /* [out] */ IBinder** binder)
{
    synchronized (IAppOpsManager::class) {
        if (sToken != null) {
            return sToken;
        }
        try {
            sToken = service.getToken(new Binder());
        } catch (RemoteException e) {
            // System is dead, whatevs.
        }
        return sToken;
    }
}

/**
 * Report that an application has started executing a long-running operation.  Note that you
 * must pass in both the uid and name of the application to be checked; this function will
 * verify that these two match, and if not, return {@link #MODE_IGNORED}.  If this call
 * succeeds, the last execution time of the operation for this app will be updated to
 * the current time and the operation will be marked as "running".  In this case you must
 * later call {@link #FinishOp(Int32, Int32, String)} to report when the application is no
 * longer performing the operation.
 * @param op The operation to start.  One of the OP_* constants.
 * @param uid The user id of the application attempting to perform the operation.
 * @param packageName The name of the application attempting to perform the operation.
 * @return Returns {@link #MODE_ALLOWED} if the operation is allowed, or
 * {@link #MODE_IGNORED} if it is not allowed and should be silently ignored (without
 * causing the app to crash).
 * @throws SecurityException If the app has been configured to crash on this op.
 * @hide
 */
ECode StartOp(
    /* [in] */ Int32 op,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    try {
        Int32 mode = mService.startOperation(getToken(mService), op, uid, packageName);
        if (mode == MODE_ERRORED) {
            throw new SecurityException(buildSecurityExceptionMsg(op, uid, packageName));
        }
        return mode;
    } catch (RemoteException e) {
    }
    return MODE_IGNORED;
}

/**
 * Like {@link #startOp} but instead of throwing a {@link SecurityException} it
 * returns {@link #MODE_ERRORED}.
 * @hide
 */
ECode StartOpNoThrow(
    /* [in] */ Int32 op,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    try {
        return mService.startOperation(getToken(mService), op, uid, packageName);
    } catch (RemoteException e) {
    }
    return MODE_IGNORED;
}

/** @hide */
ECode StartOp(
    /* [in] */ Int32 op,
    /* [out] */ Int32* result)
{
    return startOp(op, Process.myUid(), mContext.getOpPackageName());
}

/**
 * Report that an application is no longer performing an operation that had previously
 * been started with {@link #startOp(Int32, Int32, String)}.  There is no validation of input
 * or result; the parameters supplied here must be the exact same ones previously passed
 * in when starting the operation.
 * @hide
 */
ECode FinishOp(
    /* [in] */ Int32 op,
    /* [in] */ Int32 uid,
    /* [in] */ const String& packageName)
{
    try {
        mService.FinishOperation(getToken(mService), op, uid, packageName);
    } catch (RemoteException e) {
    }
}

/** @hide */
ECode FinishOp(
    /* [in] */ Int32 op)
{
    FinishOp(op, Process.myUid(), mContext.getOpPackageName());
}


} // namespace App
} // namespace Droid
} // namespace Elastos
