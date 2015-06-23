
#include "provider/Settings.h"
#include "util/Config.h"
#include "os/SystemProperties.h"
#include "os/Process.h"
#include "os/ServiceManager.h"
#include "os/UserHandle.h"
#include "net/Uri.h"
#include "privacy/CPrivacySettingsManager.h"
#include "content/Intent.h"
#include "content/CContentValues.h"
#include "os/CBundle.h"
#include "text/TextUtils.h"
#include <os/SystemProperties.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Core::CStringWrapper;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Content::CContentValues;
using Elastos::Droid::Content::Intent;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Droid::Provider::ISettingsGlobal;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Privacy::IIPrivacySettingsManager;
using Elastos::Droid::Privacy::CPrivacySettingsManager;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace Provider {

const String Settings::JID_RESOURCE_PREFIX = String("android");
const String Settings::TAG("Settings");
const Boolean Settings::LOCAL_LOGV = FALSE;


//================================================================================
//              Settings::NameValueCache
//================================================================================
static AutoPtr< ArrayOf<String> > InitSelectValue()
{
    AutoPtr< ArrayOf<String> > array = ArrayOf<String>::Alloc(1);
    (*array)[0] = ISettingsNameValueTable::VALUE;
    return array;
}
const AutoPtr< ArrayOf<String> > Settings::NameValueCache::SELECT_VALUE = InitSelectValue();
const String Settings::NameValueCache::NAME_EQ_PLACEHOLDER("name=?");

Boolean Settings::NameValueTable::PutString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IUri* uri,
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    // The database will take care of replacing duplicates.
    //try {
    AutoPtr<IContentValues> values;
    CContentValues::New((IContentValues**)&values);
    assert(values != NULL);

    AutoPtr<ICharSequence> tmp;
    CStringWrapper::New(name, (ICharSequence**)&tmp);
    values->PutString(String("name"), tmp);
    tmp = NULL;
    CStringWrapper::New(value, (ICharSequence**)&tmp);
    values->PutString(String("value"), tmp);
    AutoPtr<IUri> outUri;
    resolver->Insert(uri, values,(IUri**)&outUri);
    return TRUE;
    //} catch (/*SQLException e*/...) {
    //    Slogger::W(CSettings::TAG, StringBuffer("Can't set key ") + name + StringBuffer(" in ") + uri);
    //    return FALSE;
    //  }
}

ECode Settings::NameValueTable::GetUriFor(
    /* [in] */ IUri* uri,
    /* [in] */ const String& name,
    /* [out] */ IUri** value)
{
    VALIDATE_NOT_NULL(value)
    return Uri::WithAppendedPath(uri, name, value);
}

Settings::NameValueCache::NameValueCache(
    /* [in] */ const String& versionSystemProperty,
    /* [in] */ IUri* uri,
    /* [in] */ const String& getCommand,
    /* [in] */ const String& setCommand)
    : mVersionSystemProperty(versionSystemProperty)
    , mUri(uri)
    , mValues(10)
    , mValuesVersion(0)
    , mContentProvider(NULL)
    , mCallGetCommand(getCommand)
    , mCallSetCommand(setCommand)
{}

ECode Settings::NameValueCache::LazyGetProvider(
    /* [in] */ IContentResolver* cr,
    /* [out] */ IIContentProvider** provider)
{
    VALIDATE_NOT_NULL(provider)
    ECode ec = NOERROR;
    {
        Mutex::Autolock lock(mLock);
        if (mContentProvider == NULL) {
            String authority;
            mUri->GetAuthority(&authority);
            ec = cr->AcquireProvider(authority, (IIContentProvider**)&mContentProvider);
        }
    }
    *provider = mContentProvider;
    REFCOUNT_ADD(*provider)
    return ec;
}

ECode Settings::NameValueCache::PutStringForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    // try {
    AutoPtr<IBundle> arg, tmp;
    CBundle::New((IBundle**)&arg);
    arg->PutString(String("value"), value);
    arg->PutInt32(ISettings::CALL_METHOD_USER_KEY, userHandle);
    AutoPtr<IIContentProvider> cp;
    ECode ec = LazyGetProvider(cr, (IIContentProvider**)&cp);
    if (FAILED(ec)) {
        Slogger::W(TAG, "Can't set key %s in %p", name.string(), mUri.Get());
        *result = FALSE;
        return ec == (ECode)E_REMOTE_EXCEPTION ? NOERROR : ec;
    }
    ec = cp->Call(mCallSetCommand, name, arg, (IBundle**)&tmp);
    if (FAILED(ec)) {
        Slogger::W(TAG, "Can't set key %s in %p", name.string(), mUri.Get());
        *result = FALSE;
        return ec == (ECode)E_REMOTE_EXCEPTION ? NOERROR : ec;
    }
    // } catch (RemoteException e) {
    //     Log.w(TAG, "Can't set key " + name + " in " + mUri, e);
    //     return false;
    // }
    *result = TRUE;
    return NOERROR;
}

ECode Settings::NameValueCache::GetStringForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 userHandle,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)

    Boolean isSelf = (userHandle == UserHandle::GetMyUserId());
    if (isSelf) {
        Int64 newValuesVersion = SystemProperties::GetInt64(mVersionSystemProperty, 0);

        // Our own user's settings data uses a client-side cache
        Mutex::Autolock lock(mLock);
        if (mValuesVersion != newValuesVersion) {
            if (LOCAL_LOGV || FALSE) {
                String segment;
                mUri->GetLastPathSegment(&segment);
                Slogger::V(TAG, "invalidate [%s]: current %d != cached %d", segment.string(), newValuesVersion, mValuesVersion);
            }

            mValues.Clear();
            mValuesVersion = newValuesVersion;
        }

        HashMap<String, String>::Iterator ator = mValues.Find(name);
        if (ator != mValues.End()) {
            *value = ator->mSecond; // Could be null, that's OK -- negative caching
            return NOERROR;
        }
    }
    else {
        if (LOCAL_LOGV)
            Slogger::V(TAG, "get setting for user %d by user %d so skipping cache", userHandle, UserHandle::GetMyUserId());
    }

    AutoPtr<IIContentProvider> cp;
    if (FAILED(LazyGetProvider(cr, (IIContentProvider**)&cp)) || cp == NULL) {
        *value = String(NULL);
        return NOERROR;
    }

    // Try the fast path first, not using query().  If this
    // fails (alternate Settings provider that doesn't support
    // this interface?) then we fall back to the query/table
    // interface.
    if (mCallGetCommand != NULL) {
        // try {
        AutoPtr<IBundle> args;
        if (!isSelf) {
            CBundle::New((IBundle**)&args);
            args->PutInt32(ISettings::CALL_METHOD_USER_KEY, userHandle);
        }
        AutoPtr<IBundle> b;
        if (SUCCEEDED(cp->Call(mCallGetCommand, name, args, (IBundle**)&b)) && b != NULL) {
            String _value;
            b->GetPairValue(&_value);
            // Don't update our cache for reads of other users' data
            if (isSelf) {
                Mutex::Autolock lock(mLock);
                mValues[name] = _value;
            }
            else {
                if (LOCAL_LOGV)
                    Slogger::I(TAG, "call-query of user %d by %d so not updating cache", userHandle, UserHandle::GetMyUserId());
            }
            *value = _value;
            return NOERROR;
        }
        // If the response Bundle is null, we fall through
        // to the query interface below.
        // } catch (RemoteException e) {
        //     // Not supported by the remote side?  Fall through
        //     // to query().
        // }
    }

    AutoPtr<ICursor> c;
    // try {
    AutoPtr< ArrayOf<String> > selectionArgs = ArrayOf<String>::Alloc(1);
    (*selectionArgs)[0] = name;
    if (FAILED(cp->Query(mUri, SELECT_VALUE, NAME_EQ_PLACEHOLDER, selectionArgs, String(NULL), NULL, (ICursor**)&c))) {
        Slogger::W(TAG, "Can't get key %s from %p", name.string(), mUri.Get());
        if (c != NULL) {
            c->Close();
        }
        *value = String(NULL);  // Return null, but don't cache it.
        return NOERROR;
    }
    if (c == NULL) {
        Slogger::W(TAG, "Can't get key %s from %p", name.string(), mUri.Get());
        *value = String(NULL);
        return NOERROR;
    }

    String _value;
    Boolean succeeded = FALSE;
    c->MoveToNext(&succeeded);
    if (succeeded) {
        c->GetString(0, &_value);
    }
    {
        Mutex::Autolock lock(mLock);
        mValues[name] = _value;
    }
    if (LOCAL_LOGV) {
        String segment;
        mUri->GetLastPathSegment(&segment);
        Slogger::V(TAG, "cache miss [%s]: %s = %s", segment.string(), name.string(), (_value.IsNull()? "(null)" : _value.string()));
    }
    if (c != NULL) {
        c->Close();
    }
    *value = _value;
    return NOERROR;
    // } catch (RemoteException e) {
    //     Log.w(TAG, "Can't get key " + name + " from " + mUri, e);
    //     return null;  // Return null, but don't cache it.
    // } finally {
    //     if (c != null) c.close();
    // }
}

//================================================================================
//              Settings::System
//===============================================================================

static AutoPtr< ArrayOf<String> > InitVOLUMESETTINGS()
{
    AutoPtr< ArrayOf<String> > array = ArrayOf<String>::Alloc(7);
    (*array)[0] = ISettingsSystem::VOLUME_VOICE;
    (*array)[1] = ISettingsSystem::VOLUME_SYSTEM;
    (*array)[2] = ISettingsSystem::VOLUME_RING;
    (*array)[3] = ISettingsSystem::VOLUME_MUSIC;
    (*array)[4] = ISettingsSystem::VOLUME_ALARM;
    (*array)[5] = ISettingsSystem::VOLUME_NOTIFICATION;
    (*array)[6] = ISettingsSystem::VOLUME_BLUETOOTH_SCO;
    return array;
}
const AutoPtr< ArrayOf<String> > Settings::System::VOLUME_SETTINGS = InitVOLUMESETTINGS();

static AutoPtr<IUri> InitSystemCONTENTURI()
{
    AutoPtr<IUri> uri;
    Uri::Parse(String("content://") + ISettings::AUTHORITY + String("/system"), (IUri**)&uri);
    return uri;
}
const AutoPtr<IUri> Settings::System::CONTENT_URI = InitSystemCONTENTURI();

const AutoPtr<Settings::NameValueCache> Settings::System::sNameValueCache = new Settings::NameValueCache(ISettingsSystem::SYS_PROP_SETTING_VERSION,
        CONTENT_URI, ISettings::CALL_METHOD_GET_SYSTEM, ISettings::CALL_METHOD_PUT_SYSTEM);

HashSet<String> Settings::System::MOVED_TO_SECURE(30);

Boolean Settings::System::InitMOVEDTOSECURE()
{
    MOVED_TO_SECURE.Insert(ISettingsSecure::ANDROID_ID);
    MOVED_TO_SECURE.Insert(ISettingsSecure::HTTP_PROXY);
    MOVED_TO_SECURE.Insert(ISettingsSecure::LOCATION_PROVIDERS_ALLOWED);
    MOVED_TO_SECURE.Insert(ISettingsSecure::LOCK_BIOMETRIC_WEAK_FLAGS);
    MOVED_TO_SECURE.Insert(ISettingsSecure::LOCK_PATTERN_ENABLED);
    MOVED_TO_SECURE.Insert(ISettingsSecure::LOCK_PATTERN_VISIBLE);
    MOVED_TO_SECURE.Insert(ISettingsSecure::LOCK_PATTERN_TACTILE_FEEDBACK_ENABLED);
    MOVED_TO_SECURE.Insert(ISettingsSecure::LOGGING_ID);
    MOVED_TO_SECURE.Insert(ISettingsSecure::PARENTAL_CONTROL_ENABLED);
    MOVED_TO_SECURE.Insert(ISettingsSecure::PARENTAL_CONTROL_LAST_UPDATE);
    MOVED_TO_SECURE.Insert(ISettingsSecure::PARENTAL_CONTROL_REDIRECT_URL);
    MOVED_TO_SECURE.Insert(ISettingsSecure::SETTINGS_CLASSNAME);
    MOVED_TO_SECURE.Insert(ISettingsSecure::USE_GOOGLE_MAIL);
    MOVED_TO_SECURE.Insert(ISettingsSecure::WIFI_NETWORKS_AVAILABLE_NOTIFICATION_ON);
    MOVED_TO_SECURE.Insert(ISettingsSecure::WIFI_NETWORKS_AVAILABLE_REPEAT_DELAY);
    MOVED_TO_SECURE.Insert(ISettingsSecure::WIFI_NUM_OPEN_NETWORKS_KEPT);
    MOVED_TO_SECURE.Insert(ISettingsSecure::WIFI_ON);
    MOVED_TO_SECURE.Insert(ISettingsSecure::WIFI_WATCHDOG_ACCEPTABLE_PACKET_LOSS_PERCENTAGE);
    MOVED_TO_SECURE.Insert(ISettingsSecure::WIFI_WATCHDOG_AP_COUNT);
    MOVED_TO_SECURE.Insert(ISettingsSecure::WIFI_WATCHDOG_BACKGROUND_CHECK_DELAY_MS);
    MOVED_TO_SECURE.Insert(ISettingsSecure::WIFI_WATCHDOG_BACKGROUND_CHECK_ENABLED);
    MOVED_TO_SECURE.Insert(ISettingsSecure::WIFI_WATCHDOG_BACKGROUND_CHECK_TIMEOUT_MS);
    MOVED_TO_SECURE.Insert(ISettingsSecure::WIFI_WATCHDOG_INITIAL_IGNORED_PING_COUNT);
    MOVED_TO_SECURE.Insert(ISettingsSecure::WIFI_WATCHDOG_MAX_AP_CHECKS);
    MOVED_TO_SECURE.Insert(ISettingsSecure::WIFI_WATCHDOG_ON);
    MOVED_TO_SECURE.Insert(ISettingsSecure::WIFI_WATCHDOG_PING_COUNT);
    MOVED_TO_SECURE.Insert(ISettingsSecure::WIFI_WATCHDOG_PING_DELAY_MS);
    MOVED_TO_SECURE.Insert(ISettingsSecure::WIFI_WATCHDOG_PING_TIMEOUT_MS);
    return TRUE;
}
const Boolean Settings::System::HASINITMOVEDTOSECURE = InitMOVEDTOSECURE();

HashSet<String> Settings::System::MOVED_TO_GLOBAL;
HashSet<String> Settings::System::MOVED_TO_SECURE_THEN_GLOBAL;

Boolean Settings::System::InitMOVEDTOGLOBALANDSECURE()
{
    // these were originally in system but migrated to secure in the past,
    // so are duplicated in the Secure.* namespace
    MOVED_TO_SECURE_THEN_GLOBAL.Insert(ISettingsGlobal::ADB_ENABLED);
    MOVED_TO_SECURE_THEN_GLOBAL.Insert(ISettingsGlobal::BLUETOOTH_ON);
    MOVED_TO_SECURE_THEN_GLOBAL.Insert(ISettingsGlobal::DATA_ROAMING);
    MOVED_TO_SECURE_THEN_GLOBAL.Insert(ISettingsGlobal::DEVICE_PROVISIONED);
    MOVED_TO_SECURE_THEN_GLOBAL.Insert(ISettingsGlobal::INSTALL_NON_MARKET_APPS);
    MOVED_TO_SECURE_THEN_GLOBAL.Insert(ISettingsGlobal::USB_MASS_STORAGE_ENABLED);
    MOVED_TO_SECURE_THEN_GLOBAL.Insert(ISettingsGlobal::HTTP_PROXY);

    // these are moving directly from system to global
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::AIRPLANE_MODE_ON);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::AIRPLANE_MODE_RADIOS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::AIRPLANE_MODE_TOGGLEABLE_RADIOS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::AUTO_TIME);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::AUTO_TIME_ZONE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::CAR_DOCK_SOUND);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::CAR_UNDOCK_SOUND);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DESK_DOCK_SOUND);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DESK_UNDOCK_SOUND);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DOCK_SOUNDS_ENABLED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::LOCK_SOUND);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::UNLOCK_SOUND);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::LOW_BATTERY_SOUND);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::POWER_SOUNDS_ENABLED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::STAY_ON_WHILE_PLUGGED_IN);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_SLEEP_POLICY);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::MODE_RINGER);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WINDOW_ANIMATION_SCALE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::TRANSITION_ANIMATION_SCALE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::ANIMATOR_DURATION_SCALE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::FANCY_IME_ANIMATIONS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::COMPATIBILITY_MODE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::EMERGENCY_TONE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::CALL_AUTO_RETRY);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DEBUG_APP);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WAIT_FOR_DEBUGGER);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::SHOW_PROCESSES);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::ALWAYS_FINISH_ACTIVITIES);
    return TRUE;
}
const Boolean Settings::System::HASINITMOVEDTOGLOBALANDSECURE = InitMOVEDTOGLOBALANDSECURE();

AutoPtr< ArrayOf<Settings::System::KeyShortcutInfo*> > Settings::System::InitShortCutInfoArray()
{
    AutoPtr< ArrayOf<KeyShortcutInfo*> > array = ArrayOf<KeyShortcutInfo*>::Alloc(4);
    AutoPtr<KeyShortcutInfo> info1 = new KeyShortcutInfo(IKeyEvent::KEYCODE_PROG_RED, ISettingsSystem::SHORTCUT_KEY_0);
    array->Set(0, info1);
    AutoPtr<KeyShortcutInfo> info2 = new KeyShortcutInfo(IKeyEvent::KEYCODE_PROG_GREEN, ISettingsSystem::SHORTCUT_KEY_1);
    array->Set(1, info2);
    AutoPtr<KeyShortcutInfo> info3 = new KeyShortcutInfo(IKeyEvent::KEYCODE_PROG_YELLOW, ISettingsSystem::SHORTCUT_KEY_2);
    array->Set(2, info3);
    AutoPtr<KeyShortcutInfo> info4 = new KeyShortcutInfo(IKeyEvent::KEYCODE_PROG_BLUE, ISettingsSystem::SHORTCUT_KEY_3);
    array->Set(3, info4);
    return array;
}
AutoPtr< ArrayOf<Settings::System::KeyShortcutInfo*> > Settings::System::sKeyShortcutInfoArray = InitShortCutInfoArray();

static AutoPtr<IUri> InitSystemDefaultUri(
    /* [in] */ const String& type)
{
    AutoPtr<IUri> uri;
    Settings::System::GetUriFor(type, (IUri**)&uri);
    return uri;
}

const AutoPtr<IUri> Settings::System::DEFAULT_RINGTONE_URI = InitSystemDefaultUri(ISettingsSystem::RINGTONE);
const AutoPtr<IUri> Settings::System::DEFAULT_NOTIFICATION_URI = InitSystemDefaultUri(ISettingsSystem::NOTIFICATION_SOUND);
const AutoPtr<IUri> Settings::System::DEFAULT_ALARM_ALERT_URI = InitSystemDefaultUri(ISettingsSystem::ALARM_ALERT);

static AutoPtr< ArrayOf<String> > InitSystemSettingsToBackup()
{
    AutoPtr< ArrayOf<String> > array = ArrayOf<String>::Alloc(80);
    (*array)[0] = ISettingsSystem::STAY_ON_WHILE_PLUGGED_IN;
    (*array)[1] = ISettingsSystem::WIFI_USE_STATIC_IP;
    (*array)[2] = ISettingsSystem::WIFI_STATIC_IP;
    (*array)[3] = ISettingsSystem::WIFI_STATIC_GATEWAY;
    (*array)[4] = ISettingsSystem::WIFI_STATIC_NETMASK;
    (*array)[5] = ISettingsSystem::WIFI_STATIC_DNS1;
    (*array)[6] = ISettingsSystem::WIFI_STATIC_DNS2;
    (*array)[7] = ISettingsSystem::BLUETOOTH_DISCOVERABILITY;
    (*array)[8] = ISettingsSystem::BLUETOOTH_DISCOVERABILITY_TIMEOUT;
    (*array)[9] = ISettingsSystem::DIM_SCREEN;
    (*array)[10] = ISettingsSystem::SCREEN_OFF_TIMEOUT;
    (*array)[11] = ISettingsSystem::SCREEN_BRIGHTNESS;
    (*array)[12] = ISettingsSystem::SCREEN_BRIGHTNESS_MODE;
    (*array)[13] = ISettingsSystem::SCREEN_AUTO_BRIGHTNESS_ADJ;
    (*array)[14] = ISettingsSystem::VIBRATE_INPUT_DEVICES;
    (*array)[15] = ISettingsSystem::MODE_RINGER;                // moved to globa;
    (*array)[16] = ISettingsSystem::MODE_RINGER_STREAMS_AFFECTED;
    (*array)[17] = ISettingsSystem::MUTE_STREAMS_AFFECTED;
    (*array)[18] = ISettingsSystem::VOLUME_VOICE;
    (*array)[19] = ISettingsSystem::VOLUME_SYSTEM;
    (*array)[20] = ISettingsSystem::VOLUME_RING;
    (*array)[21] = ISettingsSystem::VOLUME_MUSIC;
    (*array)[22] = ISettingsSystem::VOLUME_ALARM;
    (*array)[23] = ISettingsSystem::VOLUME_NOTIFICATION;
    (*array)[24] = ISettingsSystem::VOLUME_BLUETOOTH_SCO;
    (*array)[25] = ISettingsSystem::VOLUME_VOICE + ISettingsSystem::APPEND_FOR_LAST_AUDIBLE;
    (*array)[26] = ISettingsSystem::VOLUME_SYSTEM + ISettingsSystem::APPEND_FOR_LAST_AUDIBLE;
    (*array)[27] = ISettingsSystem::VOLUME_RING + ISettingsSystem::APPEND_FOR_LAST_AUDIBLE;
    (*array)[28] = ISettingsSystem::VOLUME_MUSIC + ISettingsSystem::APPEND_FOR_LAST_AUDIBLE;
    (*array)[29] = ISettingsSystem::VOLUME_ALARM + ISettingsSystem::APPEND_FOR_LAST_AUDIBLE;
    (*array)[30] = ISettingsSystem::VOLUME_NOTIFICATION + ISettingsSystem::APPEND_FOR_LAST_AUDIBLE;
    (*array)[31] = ISettingsSystem::VOLUME_BLUETOOTH_SCO + ISettingsSystem::APPEND_FOR_LAST_AUDIBLE;
    (*array)[32] = ISettingsSystem::TEXT_AUTO_REPLACE;
    (*array)[33] = ISettingsSystem::TEXT_AUTO_CAPS;
    (*array)[34] = ISettingsSystem::TEXT_AUTO_PUNCTUATE;
    (*array)[35] = ISettingsSystem::TEXT_SHOW_PASSWORD;
    (*array)[36] = ISettingsSystem::AUTO_TIME;                  // moved to globa;
    (*array)[37] = ISettingsSystem::AUTO_TIME_ZONE;             // moved to globa;
    (*array)[38] = ISettingsSystem::TIME_12_24;
    (*array)[39] = ISettingsSystem::DATE_FORMAT;
    (*array)[40] = ISettingsSystem::DTMF_TONE_WHEN_DIALING;
    (*array)[41] = ISettingsSystem::DTMF_TONE_TYPE_WHEN_DIALING;
    (*array)[42] = ISettingsSystem::HEARING_AID;
    (*array)[43] = ISettingsSystem::TTY_MODE;
    (*array)[44] = ISettingsSystem::SOUND_EFFECTS_ENABLED;
    (*array)[45] = ISettingsSystem::HAPTIC_FEEDBACK_ENABLED;
    (*array)[46] = ISettingsSystem::POWER_SOUNDS_ENABLED;       // moved to globa;
    (*array)[47] = ISettingsSystem::DOCK_SOUNDS_ENABLED;        // moved to globa;
    (*array)[48] = ISettingsSystem::LOCKSCREEN_SOUNDS_ENABLED;
    (*array)[49] = ISettingsSystem::SHOW_WEB_SUGGESTIONS;
    (*array)[50] = ISettingsSystem::NOTIFICATION_LIGHT_PULSE;
    (*array)[51] = ISettingsSystem::SIP_CALL_OPTIONS;
    (*array)[52] = ISettingsSystem::SIP_RECEIVE_CALLS;
    (*array)[53] = ISettingsSystem::POINTER_SPEED;
    (*array)[54] = ISettingsSystem::VIBRATE_WHEN_RINGING;
    /* AW Code Begin */
    (*array)[55] = ISettingsSystem::BRIGHT_SYSTEM_MODE;
    (*array)[56] = ISettingsSystem::BRIGHTNESS_LIGHT_MODE;
    (*array)[57] = ISettingsSystem::IS_SCAN_TF_CARD;
    (*array)[58] = ISettingsSystem::DISPLAY_ADAPTION_MODE;
    (*array)[59] = ISettingsSystem::DISPLAY_ADAPTION_ENABLE;
    (*array)[60] = ISettingsSystem::SMART_BRIGHTNESS_ENABLE;
    /* add by Gary. start {{----------------------------------- */
    /* 2011-11-1 */
    /* support shortcut keys with color keys for some specific websites and apps */
    (*array)[61] = ISettingsSystem::SHORTCUT_KEY_0;
    (*array)[62] = ISettingsSystem::SHORTCUT_KEY_1;
    (*array)[63] = ISettingsSystem::SHORTCUT_KEY_2;
    (*array)[64] = ISettingsSystem::SHORTCUT_KEY_3;
    /* add by Gary. end   -----------------------------------}} */
    (*array)[65] = ISettingsSystem::DISPLAY_ADAPTION_ENABLE;
    /* add by Gary. start {{----------------------------------- */
    /* 2011-12-6 */
    /* adjust the display area */
    (*array)[66] = ISettingsSystem::DISPLAY_AREA_RATIO;
    /* add by Gary. end   -----------------------------------}} */

    /* add by Gary. start {{----------------------------------- */
    /* 2011-12-6 */
    /* set gate for scaning usb host */
    (*array)[67] = ISettingsSystem::IS_SCAN_USB_HOST;
    /* add by Gary. end   -----------------------------------}} */
    /* add by Gary. start {{----------------------------------- */
    /* 2011-12-10 */
    /* record the display format and the advance of the mouse in mouse mode */
    (*array)[68] = ISettingsSystem::DISPLY_OUTPUT_FORMAT;
    (*array)[69] = ISettingsSystem::MOUSE_ADVANCE;
    /* add by Gary. end   -----------------------------------}} */

    /* add by Gary. start {{----------------------------------- */
    /* 2011-12-11 */
    /* record the color parameters : brightness, contrast,and saturation */
    (*array)[70] = ISettingsSystem::COLOR_BRIGHTNESS;
    (*array)[71] = ISettingsSystem::COLOR_CONTRAST;
    (*array)[72] = ISettingsSystem::COLOR_SATURATION;
    /* add by Gary. end   -----------------------------------}} */

    /* add by Gary. start {{----------------------------------- */
    /* 2011-12-11 */
    /* record the audio output type */
    (*array)[73] = ISettingsSystem::AUDIO_OUTPUT_TYPE;
    /* add by Gary. end   -----------------------------------}} */

    /* add by Gary. start {{----------------------------------- */
    /* 2012-2-27 */
    /* record the audio output channel */
    (*array)[74] = ISettingsSystem::AUDIO_OUTPUT_CHANNEL;
    /* add by Gary. end   -----------------------------------}} */

    /* add by Gary. start {{----------------------------------- */
    /* 2011-12-17 */
    /* directly power off when long press on the power key */
    (*array)[75] = ISettingsSystem::DIRECTLY_POWER_OFF;
    /* add by Gary. end   -----------------------------------}} */

    /* add by Gary. start {{----------------------------------- */
    /* 2012-4-27 */
    /* add a switch to control BD folder play mode */
    (*array)[76] = ISettingsSystem::BD_FOLDER_PLAY_MODE;
    /* add by Gary. end   -----------------------------------}} */
    (*array)[77] = ISettingsSystem::HDMI_OUTPUT_MODE;
    /* AW Code Begin */
    (*array)[78] = ISettingsSystem::BOOT_FAST_ENABLE;
    (*array)[79] = ISettingsSystem::ENABLE_PASS_THROUGH;

    return array;
}

const AutoPtr< ArrayOf<String> > Settings::System::SETTINGS_TO_BACKUP = InitSystemSettingsToBackup();

void Settings::System::GetMovedKeys(
    /* [in] */ IObjectContainer* outKeySet)
{
    HashSet<String>::Iterator it = MOVED_TO_GLOBAL.Begin();
    for (; it != MOVED_TO_GLOBAL.End(); ++it) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(*it, (ICharSequence**)&cs);
        outKeySet->Add(cs);
    }
    for (it = MOVED_TO_SECURE_THEN_GLOBAL.Begin(); it != MOVED_TO_SECURE_THEN_GLOBAL.End(); ++it) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(*it, (ICharSequence**)&cs);
        outKeySet->Add(cs);
    }
}

void Settings::System::GetNonLegacyMovedKeys(
    /* [in] */ IObjectContainer* outKeySet)
{
    HashSet<String>::Iterator it = MOVED_TO_GLOBAL.Begin();
    for (; it != MOVED_TO_GLOBAL.End(); ++it) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(*it, (ICharSequence**)&cs);
        outKeySet->Add(cs);
    }
}

ECode Settings::System::GetString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    return GetStringForUser(resolver, name, UserHandle::GetMyUserId(), value);
}

ECode Settings::System::GetStringForUser(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ Int32 userHandle,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    if (MOVED_TO_SECURE.Find(name) != MOVED_TO_SECURE.End()) {
        Slogger::W(TAG, "Setting %s has moved from android.provider.Settings.System to android.provider.Settings.Secure, returning read-only value.", name.string());
        return Settings::Secure::GetStringForUser(resolver, name, userHandle, value);
    }
    if (MOVED_TO_GLOBAL.Find(name) != MOVED_TO_GLOBAL.End() || MOVED_TO_SECURE_THEN_GLOBAL.Find(name) != MOVED_TO_SECURE_THEN_GLOBAL.End()) {
        Slogger::W(TAG, "Setting %s has moved from android.provider.Settings.System to android.provider.Settings.Global, returning read-only value.", name.string());
        String str;
        FAIL_RETURN(Settings::Global::GetStringForUser(resolver, name, userHandle, &str))
        Slogger::D(TAG, "now get:%s", str.string());
        *value = str;
        return NOERROR;
    }
    return sNameValueCache->GetStringForUser(resolver, name, userHandle, value);
}

ECode Settings::System::PutString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutStringForUser(resolver, name, value, UserHandle::GetMyUserId(), result);
}

ECode Settings::System::PutStringForUser(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (MOVED_TO_SECURE.Find(name) != MOVED_TO_SECURE.End()) {
        Slogger::W(TAG, "Setting %s has moved from android.provider.Settings.System to android.provider.Settings.Secure, value is unchanged.", name.string());
        *result = FALSE;
        return NOERROR;
    }
    if (MOVED_TO_GLOBAL.Find(name) != MOVED_TO_GLOBAL.End() || MOVED_TO_SECURE_THEN_GLOBAL.Find(name) != MOVED_TO_SECURE_THEN_GLOBAL.End()) {
        Slogger::W(TAG, "Setting %s has moved from android.provider.Settings.System to android.provider.Settings.Global, value is unchanged.", name.string());
        *result = FALSE;
        return NOERROR;
    }
    return sNameValueCache->PutStringForUser(resolver, name, value, userHandle, result);
}

ECode Settings::System::GetUriFor(
    /* [in] */ const String& name,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    if (MOVED_TO_SECURE.Find(name) != MOVED_TO_SECURE.End()) {
        Slogger::W(TAG, "Setting %s has moved from android.provider.Settings.System to android.provider.Settings.Secure, returning Secure URI.", name.string());
        return Settings::Secure::GetUriFor(Settings::Secure::CONTENT_URI, name, uri);
    }
    if (MOVED_TO_GLOBAL.Find(name) != MOVED_TO_GLOBAL.End() || MOVED_TO_SECURE_THEN_GLOBAL.Find(name) != MOVED_TO_SECURE_THEN_GLOBAL.End()) {
        Slogger::W(TAG, "Setting %s has moved from android.provider.Settings.System to android.provider.Settings.Global, returning read-only global URI.", name.string());
        return Settings::Global::GetUriFor(Settings::Global::CONTENT_URI, name, uri);
    }
    return GetUriFor(CONTENT_URI, name, uri);
}

ECode Settings::System::GetInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 def,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    return GetInt32ForUser(cr, name, def, UserHandle::GetMyUserId(), value);
}

ECode Settings::System::GetInt32ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 def,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    String v;
    FAIL_RETURN(GetStringForUser(cr, name, userHandle, &v))
    // try {
    if (v.IsNull()) {
        *value = def;
        return NOERROR;
    }
    else {
        Int32 _value;
        if (FAILED(StringUtils::ParseInt32(v, &_value))) {
            *value = def;
            return NOERROR;
        }
        *value = _value;
        return NOERROR;
    }
    // } catch (NumberFormatException e) {
    //     return def;
    // }
}

ECode Settings::System::GetInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int32* value)
{
    return GetInt32ForUser(cr, name, UserHandle::GetMyUserId(), value);
}

ECode Settings::System::GetInt32ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    String v;
    FAIL_RETURN(GetStringForUser(cr, name, userHandle, &v))
    // try {
    if (FAILED(StringUtils::ParseInt32(v, value))){
        *value = 0;
        return E_SETTING_NOT_FOUND_EXCEPTION;
    }
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     throw new SettingNotFoundException(name);
    // }
}

ECode Settings::System::PutInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutInt32ForUser(cr, name, value, UserHandle::GetMyUserId(), result);
}

ECode Settings::System::PutInt32ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 value,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutStringForUser(cr, name, StringUtils::Int32ToString(value), userHandle, result);
}

ECode Settings::System::GetInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 def,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    return GetInt64ForUser(cr, name, def, UserHandle::GetMyUserId(), value);
}

ECode Settings::System::GetInt64ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 def,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    String valString;
    FAIL_RETURN(GetStringForUser(cr, name, userHandle, &valString))
    Int64 _value;
    // try {
    if (valString.IsNull()) {
        _value = def;
    }
    else {
        if (FAILED(StringUtils::ParseInt64(valString, &_value))) {
            _value = def;
        }
    }
    *value = _value;
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     value = def;
    // }
}

ECode Settings::System::GetInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int64* value)
{
    return GetInt64ForUser(cr, name, UserHandle::GetMyUserId(), value);
}

ECode Settings::System::GetInt64ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0;
    String valString;
    FAIL_RETURN(GetStringForUser(cr, name, userHandle, &valString))
    // try {
    if (FAILED(StringUtils::ParseInt64(valString, value))) {
        return E_SETTING_NOT_FOUND_EXCEPTION;
    }
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     throw new SettingNotFoundException(name);
    // }
}

ECode Settings::System::PutInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutInt64ForUser(cr, name, value, UserHandle::GetMyUserId(), result);
}

ECode Settings::System::PutInt64ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 value,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutStringForUser(cr, name, StringUtils::Int64ToString(value), userHandle, result);
}

ECode Settings::System::GetFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float def,
    /* [out] */ Float* value)
{
    return GetFloatForUser(cr, name, def, UserHandle::GetMyUserId(), value);
}

ECode Settings::System::GetFloatForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float def,
    /* [in] */ Int32 userHandle,
    /* [out] */ Float* value)
{
    String v;
    FAIL_RETURN(GetStringForUser(cr, name, userHandle, &v))
    // try {
    if (v.IsNull()) {
        *value = def;
        return NOERROR;
    }
    else {
        Float _value;
        if (FAILED(StringUtils::ParseFloat(v, &_value))) {
            *value = def;
            return NOERROR;
        }
        *value = _value;
        return NOERROR;
    }
    // } catch (NumberFormatException e) {
    //     return def;
    // }
}

ECode Settings::System::GetFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Float* value)
{
    return GetFloatForUser(cr, name, UserHandle::GetMyUserId(), value);
}

ECode Settings::System::GetFloatForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 userHandle,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0;
    String v;
    FAIL_RETURN(GetStringForUser(cr, name, userHandle, &v))
    if (v.IsNull()) {
        return E_SETTING_NOT_FOUND_EXCEPTION;
    }
    // try {
    if (FAILED(StringUtils::ParseFloat(v, value))) {
        return E_SETTING_NOT_FOUND_EXCEPTION;
    }
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     throw new SettingNotFoundException(name);
    // }
}

ECode Settings::System::PutFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutFloatForUser(cr, name, value, UserHandle::GetMyUserId(), result);
}

ECode Settings::System::PutFloatForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float value,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutStringForUser(cr, name, StringUtils::FloatToString(value), userHandle, result);
}

ECode Settings::System::GetConfiguration(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IConfiguration* outConfig)
{
    return GetConfigurationForUser(cr, outConfig, UserHandle::GetMyUserId());
}

ECode Settings::System::GetConfigurationForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IConfiguration* outConfig,
    /* [in] */ Int32 userHandle)
{
    Float fontScale;
    outConfig->GetFontScale(&fontScale);
    Float value;
    FAIL_RETURN(GetFloatForUser(cr, ISettingsSystem::FONT_SCALE, fontScale, userHandle, &value))
    outConfig->SetFontScale(value);
    if (outConfig->GetFontScale(&fontScale), fontScale < 0) {
        outConfig->SetFontScale(1);
    }
    return NOERROR;
}

void Settings::System::ClearConfiguration(
    /* [in] */ IConfiguration* inoutConfig)
{
    inoutConfig->SetFontScale(0);
}

ECode Settings::System::PutConfiguration(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IConfiguration* config,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutConfigurationForUser(cr, config, UserHandle::GetMyUserId(), result);
}

ECode Settings::System::PutConfigurationForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IConfiguration* config,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Float fontScale;
    config->GetFontScale(&fontScale);
    return Settings::System::PutFloatForUser(cr, ISettingsSystem::FONT_SCALE, fontScale, userHandle, result);
}

Boolean Settings::System::HasInterestingConfigurationChanges(
    /* [in] */ Int32 changes)
{
     return (changes & IActivityInfo::CONFIG_FONT_SCALE) != 0;
}

ECode Settings::System::GetShowGTalkServiceStatus(
    /* [in] */ IContentResolver* cr,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
     return GetShowGTalkServiceStatusForUser(cr, UserHandle::GetMyUserId(), result);
}

ECode Settings::System::GetShowGTalkServiceStatusForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Int32 value;
    FAIL_RETURN(GetInt32ForUser(cr, ISettingsSystem::SHOW_GTALK_SERVICE_STATUS, 0, userHandle, &value))
    *result = value != 0;
    return NOERROR;
}

ECode Settings::System::SetShowGTalkServiceStatus(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Boolean flag)
{
    return SetShowGTalkServiceStatusForUser(cr, flag, UserHandle::GetMyUserId());
}

ECode Settings::System::SetShowGTalkServiceStatusForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Boolean flag,
    /* [in] */ Int32 userHandle)
{
    Boolean result;
    return PutInt32ForUser(cr, ISettingsSystem::SHOW_GTALK_SERVICE_STATUS, flag ? 1 : 0, userHandle, &result);
}

String Settings::System::FindNameByKey(
    /* [in] */ Int32 key)
{
    for (Int32 i = 0; i < sKeyShortcutInfoArray->GetLength(); i++) {
        if ((*sKeyShortcutInfoArray)[i]->mKey == key)
            return (*sKeyShortcutInfoArray)[i]->mName;
    }

    return String(NULL);
}

//================================================================================
//              Settings::Secure
//================================================================================
//
static AutoPtr<IUri> InitSecureCONTENTURI()
{
    AutoPtr<IUri> uri;
    Uri::Parse(String("content://") + ISettings::AUTHORITY + String("/secure"), (IUri**)&uri);
    return uri;
}
const AutoPtr<IUri> Settings::Secure::CONTENT_URI = InitSecureCONTENTURI();

static AutoPtr< ArrayOf<String> > InitSecureSettingsToBackup()
{
    AutoPtr< ArrayOf<String> > array = ArrayOf<String>::Alloc(33);
    (*array)[0] = ISettingsSecure::BUGREPORT_IN_POWER_MENU;
    (*array)[1] = ISettingsSecure::ALLOW_MOCK_LOCATION;
    (*array)[2] = ISettingsSecure::PARENTAL_CONTROL_ENABLED;
    (*array)[3] = ISettingsSecure::PARENTAL_CONTROL_REDIRECT_URL;
    (*array)[4] = ISettingsSecure::USB_MASS_STORAGE_ENABLED;
    (*array)[5] = ISettingsSecure::ACCESSIBILITY_DISPLAY_MAGNIFICATION_ENABLED;
    (*array)[6] = ISettingsSecure::ACCESSIBILITY_DISPLAY_MAGNIFICATION_SCALE;
    (*array)[7] = ISettingsSecure::ACCESSIBILITY_DISPLAY_MAGNIFICATION_AUTO_UPDATE;
    (*array)[8] = ISettingsSecure::ACCESSIBILITY_SCRIPT_INJECTION;
    (*array)[9] = ISettingsSecure::BACKUP_AUTO_RESTORE;
    (*array)[10] = ISettingsSecure::ENABLED_ACCESSIBILITY_SERVICES;
    (*array)[11] = ISettingsSecure::TOUCH_EXPLORATION_GRANTED_ACCESSIBILITY_SERVICES;
    (*array)[12] = ISettingsSecure::TOUCH_EXPLORATION_ENABLED;
    (*array)[13] = ISettingsSecure::ACCESSIBILITY_ENABLED;
    (*array)[14] = ISettingsSecure::ACCESSIBILITY_SPEAK_PASSWORD;
    (*array)[15] = ISettingsSecure::TTS_USE_DEFAULTS;
    (*array)[16] = ISettingsSecure::TTS_DEFAULT_RATE;
    (*array)[17] = ISettingsSecure::TTS_DEFAULT_PITCH;
    (*array)[18] = ISettingsSecure::TTS_DEFAULT_SYNTH;
    (*array)[19] = ISettingsSecure::TTS_DEFAULT_LANG;
    (*array)[20] = ISettingsSecure::TTS_DEFAULT_COUNTRY;
    (*array)[21] = ISettingsSecure::TTS_ENABLED_PLUGINS;
    (*array)[22] = ISettingsSecure::TTS_DEFAULT_LOCALE;
    (*array)[23] = ISettingsSecure::WIFI_NETWORKS_AVAILABLE_NOTIFICATION_ON;
    (*array)[24] = ISettingsSecure::WIFI_NETWORKS_AVAILABLE_REPEAT_DELAY;
    (*array)[25] = ISettingsSecure::WIFI_NUM_OPEN_NETWORKS_KEPT;
    (*array)[26] = ISettingsSecure::MOUNT_PLAY_NOTIFICATION_SND;
    (*array)[27] = ISettingsSecure::MOUNT_UMS_AUTOSTART;
    (*array)[28] = ISettingsSecure::MOUNT_UMS_PROMPT;
    (*array)[29] = ISettingsSecure::MOUNT_UMS_NOTIFY_ENABLED;
    (*array)[30] = ISettingsSecure::UI_NIGHT_MODE;
    (*array)[31] = ISettingsSecure::LOCK_SCREEN_OWNER_INFO;
    (*array)[32] = ISettingsSecure::LOCK_SCREEN_OWNER_INFO_ENABLED;
    return array;
}
const AutoPtr< ArrayOf<String> > Settings::Secure::SETTINGS_TO_BACKUP = InitSecureSettingsToBackup();

const AutoPtr<Settings::NameValueCache> Settings::Secure::sNameValueCache = new Settings::NameValueCache(
        ISettingsSecure::SYS_PROP_SETTING_VERSION, CONTENT_URI, ISettings::CALL_METHOD_GET_SECURE, ISettings::CALL_METHOD_PUT_SECURE);

AutoPtr<ILockSettings> Settings::Secure::sLockSettings;
Boolean Settings::Secure::sIsSystemProcess = FALSE;

HashSet<String> Settings::Secure::MOVED_TO_LOCK_SETTINGS(3);
HashSet<String> Settings::Secure::MOVED_TO_GLOBAL;

Boolean Settings::Secure::InitHashSet()
{
    MOVED_TO_LOCK_SETTINGS.Insert(ISettingsSecure::LOCK_PATTERN_ENABLED);
    MOVED_TO_LOCK_SETTINGS.Insert(ISettingsSecure::LOCK_PATTERN_VISIBLE);
    MOVED_TO_LOCK_SETTINGS.Insert(ISettingsSecure::LOCK_PATTERN_TACTILE_FEEDBACK_ENABLED);

    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::ADB_ENABLED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::ASSISTED_GPS_ENABLED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::BLUETOOTH_ON);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::CDMA_CELL_BROADCAST_SMS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::CDMA_ROAMING_MODE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::CDMA_SUBSCRIPTION_MODE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DATA_ACTIVITY_TIMEOUT_MOBILE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DATA_ACTIVITY_TIMEOUT_WIFI);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DATA_ROAMING);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DEVELOPMENT_SETTINGS_ENABLED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DEVICE_PROVISIONED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DISPLAY_DENSITY_FORCED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DISPLAY_SIZE_FORCED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DOWNLOAD_MAX_BYTES_OVER_MOBILE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DOWNLOAD_RECOMMENDED_MAX_BYTES_OVER_MOBILE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::INSTALL_NON_MARKET_APPS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::MOBILE_DATA);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_DEV_BUCKET_DURATION);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_DEV_DELETE_AGE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_DEV_PERSIST_BYTES);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_DEV_ROTATE_AGE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_ENABLED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_GLOBAL_ALERT_BYTES);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_POLL_INTERVAL);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_REPORT_XT_OVER_DEV);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_SAMPLE_ENABLED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_TIME_CACHE_MAX_AGE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_UID_BUCKET_DURATION);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_UID_DELETE_AGE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_UID_PERSIST_BYTES);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_UID_ROTATE_AGE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_UID_TAG_BUCKET_DURATION);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_UID_TAG_DELETE_AGE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_UID_TAG_PERSIST_BYTES);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETSTATS_UID_TAG_ROTATE_AGE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NETWORK_PREFERENCE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NITZ_UPDATE_DIFF);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NITZ_UPDATE_SPACING);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NTP_SERVER);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NTP_TIMEOUT);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::PDP_WATCHDOG_ERROR_POLL_COUNT);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::PDP_WATCHDOG_LONG_POLL_INTERVAL_MS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::PDP_WATCHDOG_MAX_PDP_RESET_FAIL_COUNT);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::PDP_WATCHDOG_POLL_INTERVAL_MS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::PDP_WATCHDOG_TRIGGER_PACKET_COUNT);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::SAMPLING_PROFILER_MS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::SETUP_PREPAID_DATA_SERVICE_URL);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::SETUP_PREPAID_DETECTION_REDIR_HOST);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::SETUP_PREPAID_DETECTION_TARGET_URL);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::TETHER_DUN_APN);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::TETHER_DUN_REQUIRED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::TETHER_SUPPORTED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::THROTTLE_HELP_URI);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::THROTTLE_MAX_NTP_CACHE_AGE_SEC);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::THROTTLE_NOTIFICATION_TYPE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::THROTTLE_POLLING_SEC);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::THROTTLE_RESET_DAY);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::THROTTLE_THRESHOLD_BYTES);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::THROTTLE_VALUE_KBITSPS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::USB_MASS_STORAGE_ENABLED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::USE_GOOGLE_MAIL);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WEB_AUTOFILL_QUERY_URL);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_COUNTRY_CODE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_FRAMEWORK_SCAN_INTERVAL_MS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_FREQUENCY_BAND);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_IDLE_MS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_MAX_DHCP_RETRY_COUNT);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_MOBILE_DATA_TRANSITION_WAKELOCK_TIMEOUT_MS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_NETWORKS_AVAILABLE_NOTIFICATION_ON);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_NETWORKS_AVAILABLE_REPEAT_DELAY);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_NUM_OPEN_NETWORKS_KEPT);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_ON);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_P2P_DEVICE_NAME);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_SAVED_STATE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_SUPPLICANT_SCAN_INTERVAL_MS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_SUSPEND_OPTIMIZATIONS_ENABLED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_WATCHDOG_ON);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIFI_WATCHDOG_POOR_NETWORK_TEST_ENABLED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WIMAX_NETWORKS_AVAILABLE_NOTIFICATION_ON);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::PACKAGE_VERIFIER_ENABLE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::PACKAGE_VERIFIER_TIMEOUT);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::PACKAGE_VERIFIER_DEFAULT_RESPONSE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DATA_STALL_ALARM_NON_AGGRESSIVE_DELAY_IN_MS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DATA_STALL_ALARM_AGGRESSIVE_DELAY_IN_MS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::GPRS_REGISTER_CHECK_PERIOD_MS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::WTF_IS_FATAL);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::BATTERY_DISCHARGE_DURATION_THRESHOLD);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::BATTERY_DISCHARGE_THRESHOLD);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::SEND_ACTION_APP_ERROR);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DROPBOX_AGE_SECONDS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DROPBOX_MAX_FILES);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DROPBOX_QUOTA_KB);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DROPBOX_QUOTA_PERCENT);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DROPBOX_RESERVE_PERCENT);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DROPBOX_TAG_PREFIX);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::ERROR_LOGCAT_PREFIX);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::SYS_FREE_STORAGE_LOG_INTERVAL);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DISK_FREE_CHANGE_REPORTING_THRESHOLD);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::SYS_STORAGE_THRESHOLD_PERCENTAGE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::SYS_STORAGE_THRESHOLD_MAX_BYTES);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::SYS_STORAGE_FULL_THRESHOLD_BYTES);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::SYNC_MAX_RETRY_DELAY_IN_SECONDS);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::CONNECTIVITY_CHANGE_DELAY);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::CAPTIVE_PORTAL_DETECTION_ENABLED);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::CAPTIVE_PORTAL_SERVER);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::NSD_ON);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::SET_INSTALL_LOCATION);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DEFAULT_INSTALL_LOCATION);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::INET_CONDITION_DEBOUNCE_UP_DELAY);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::INET_CONDITION_DEBOUNCE_DOWN_DELAY);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::READ_EXTERNAL_STORAGE_ENFORCED_DEFAULT);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::HTTP_PROXY);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::GLOBAL_HTTP_PROXY_HOST);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::GLOBAL_HTTP_PROXY_PORT);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::GLOBAL_HTTP_PROXY_EXCLUSION_LIST);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::SET_GLOBAL_HTTP_PROXY);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::DEFAULT_DNS_SERVER);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::PREFERRED_NETWORK_MODE);
    MOVED_TO_GLOBAL.Insert(ISettingsGlobal::PREFERRED_CDMA_SUBSCRIPTION);
    return TRUE;
}
const Boolean Settings::Secure::sInited = Settings::Secure::InitHashSet();
Mutex Settings::Secure::sSecureLock;

const String Settings::Secure::PRIVACY_TAG("PM,SecureSettings");
AutoPtr<IContext> Settings::Secure::sContext;
AutoPtr<IPrivacySettingsManager> Settings::Secure::sPSetMan;
Boolean Settings::Secure::sPrivacyMode = FALSE;
AutoPtr<IIPackageManager> Settings::Secure::sPm;

void Settings::Secure::GetMovedKeys(
    /* [in] */ IObjectContainer* outKeySet)
{
    assert(outKeySet != NULL);
    HashSet<String>::Iterator ator = MOVED_TO_GLOBAL.Begin();
    for (; ator != MOVED_TO_GLOBAL.End(); ++ator) {
        AutoPtr<ICharSequence> keyObj;
        CStringWrapper::New(*ator, (ICharSequence**)&keyObj);
        outKeySet->Add(keyObj);
    }
}

ECode Settings::Secure::GetString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    return GetStringForUser(resolver, name, UserHandle::GetMyUserId(), value);
}

ECode Settings::Secure::GetStringForUser(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ Int32 userHandle,
    /* [out] */ String* value)
{
    if (MOVED_TO_GLOBAL.Find(name) != MOVED_TO_GLOBAL.End()) {
        Slogger::W(TAG, "Setting %s has moved from android.provider.Settings.Secure to android.provider.Settings.Global.", name.string());
        return Settings::Global::GetStringForUser(resolver, name, userHandle, value);
    }

    if (MOVED_TO_LOCK_SETTINGS.Find(name) != MOVED_TO_LOCK_SETTINGS.End()) {
        {
            Mutex::Autolock lock(sSecureLock);
            if (sLockSettings == NULL) {
                sLockSettings = (ILockSettings*)ServiceManager::GetService(String("lock_settings")).Get();
                sIsSystemProcess = Process::MyUid() == IProcess::SYSTEM_UID;
            }
        }
        if (sLockSettings != NULL && !sIsSystemProcess) {
            // try {
            return sLockSettings->GetString(name, String("0"), userHandle, value);
            // } catch (RemoteException re) {
            //     // Fall through
            // }
        }
    }

    return sNameValueCache->GetStringForUser(resolver, name, userHandle, value);
}

ECode Settings::Secure::PutString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutStringForUser(resolver, name, value, UserHandle::GetMyUserId(), result);
}

ECode Settings::Secure::PutStringForUser(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (MOVED_TO_GLOBAL.Find(name) != MOVED_TO_GLOBAL.End()) {
        Slogger::W(TAG, "Setting %s has moved from android.provider.Settings.System to android.provider.Settings.Global", name.string());
        return Settings::Global::PutStringForUser(resolver, name, value, userHandle, result);
    }
    return sNameValueCache->PutStringForUser(resolver, name, value, userHandle, result);
}

ECode Settings::Secure::GetUriFor(
    /* [in] */ const String& name,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    if (MOVED_TO_GLOBAL.Find(name) != MOVED_TO_GLOBAL.End()) {
        Slogger::W(TAG, "Setting %s has moved from android.provider.Settings.Secure to android.provider.Settings.Global, returning global URI.", name.string());
        return Global::GetUriFor(Settings::Global::CONTENT_URI, name, uri);
    }

    return GetUriFor(CONTENT_URI, name, uri);
}

ECode Settings::Secure::GetInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 def,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    return GetInt32ForUser(cr, name, def, UserHandle::GetMyUserId(), value);
}

ECode Settings::Secure::GetInt32ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 def,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    String v;
    FAIL_RETURN(GetStringForUser(cr, name, userHandle, &v))
    // try {
    Int32 _value;
    if (v.IsNull()) {
        _value = def;
    }
    else {
        if (FAILED(StringUtils::ParseInt32(v, &_value))) {
            _value = def;
        }
    }
    *value = _value;
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     return def;
    // }
}

ECode Settings::Secure::GetInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int32* value)
{
    return GetInt32ForUser(cr, name, UserHandle::GetMyUserId(), value);
}

ECode Settings::Secure::GetInt32ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0;
    String v;
    FAIL_RETURN(GetStringForUser(cr, name, userHandle, &v))
    // try {
    if (FAILED(StringUtils::ParseInt32(v, value))) {
        return E_SETTING_NOT_FOUND_EXCEPTION;
    }
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     throw new SettingNotFoundException(name);
    // }
}

ECode Settings::Secure::PutInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutInt32ForUser(cr, name, value, UserHandle::GetMyUserId(), result);
}

ECode Settings::Secure::PutInt32ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 value,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutStringForUser(cr, name, StringUtils::Int32ToString(value), userHandle, result);
}

ECode Settings::Secure::GetInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 def,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    return GetInt64ForUser(cr, name, def, UserHandle::GetMyUserId(), value);
}

ECode Settings::Secure::GetInt64ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 def,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    String valString;
    FAIL_RETURN(GetStringForUser(cr, name, userHandle, &valString))
    Int64 _value;
    // try {
    if (valString.IsNull()) {
        _value = def;
    }
    else {
        if (FAILED(StringUtils::ParseInt64(valString, &_value))) {
            _value = def;
        }
    }
    *value = _value;
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     value = def;
    // }
}

ECode Settings::Secure::GetInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int64* value)
{
    return GetInt64ForUser(cr, name, UserHandle::GetMyUserId(), value);
}

ECode Settings::Secure::GetInt64ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0;
    String valString;
    FAIL_RETURN(GetStringForUser(cr, name, userHandle, &valString))
    // try {
    if (FAILED(StringUtils::ParseInt64(valString, value))) {
        return E_SETTING_NOT_FOUND_EXCEPTION;
    }
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     throw new SettingNotFoundException(name);
    // }
}

ECode Settings::Secure::PutInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutInt64ForUser(cr, name, value, UserHandle::GetMyUserId(), result);
}

ECode Settings::Secure::PutInt64ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 value,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutStringForUser(cr, name, StringUtils::Int64ToString(value), userHandle, result);
}

ECode Settings::Secure::GetFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float def,
    /* [out] */ Float* value)
{
    return GetFloatForUser(cr, name, def, UserHandle::GetMyUserId(), value);
}

ECode Settings::Secure::GetFloatForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float def,
    /* [in] */ Int32 userHandle,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    String v;
    FAIL_RETURN(GetStringForUser(cr, name, userHandle, &v))
    // try {
    Float _value;
    if (v.IsNull()) {
        _value = def;
    }
    else {
        if (FAILED(StringUtils::ParseFloat(v, &_value))) {
            _value = def;
        }
    }
    *value = _value;
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     return def;
    // }
}

ECode Settings::Secure::GetFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    return GetFloatForUser(cr, name, UserHandle::GetMyUserId(), value);
}

ECode Settings::Secure::GetFloatForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 userHandle,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0;
    String v;
    FAIL_RETURN(GetStringForUser(cr, name, userHandle, &v))
    if (v.IsNull()) {
        return E_SETTING_NOT_FOUND_EXCEPTION;
        // throw new SettingNotFoundException(name);
    }
    // try {
    if (FAILED(StringUtils::ParseFloat(v, value))) {
        return E_SETTING_NOT_FOUND_EXCEPTION;
    }
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     throw new SettingNotFoundException(name);
    // }
}

ECode Settings::Secure::PutFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutFloatForUser(cr, name, value, UserHandle::GetMyUserId(), result);
}

ECode Settings::Secure::PutFloatForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float value,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutStringForUser(cr, name, StringUtils::FloatToString(value), userHandle, result);
}

AutoPtr< ArrayOf<String> > Settings::Secure::GetPackageName()
{
    // try{
    AutoPtr< ArrayOf<String> > package_names;
    if(sPm != NULL){
        Int32 uid = Process::MyUid();
        if (FAILED(sPm->GetPackagesForUid(uid, (ArrayOf<String>**)&package_names))) {
            Slogger::E(PRIVACY_TAG, "something went wrong with getting package name");
            return NULL;
        }
    }
    else{
        //todo: for java compatible
        // mPm = IPackageManager.Stub.asInterface(ServiceManager.getService("package"));
        sPm = (IIPackageManager*)ServiceManager::GetService(String("package")).Get();
        Int32 uid = Process::MyUid();
        if (FAILED(sPm->GetPackagesForUid(uid, (ArrayOf<String>**)&package_names))) {
            Slogger::E(PRIVACY_TAG, "something went wrong with getting package name");
            return NULL;
        }
    }
    return package_names;
    // }
    // catch(Exception e){
    //     e.printStackTrace();
    //     Log.e(PRIVACY_TAG,"something went wrong with getting package name");
    //     return null;
    // }
}

void Settings::Secure::Initiate()
{
    // try{
    sContext = NULL;
    // pSetMan = new PrivacySettingsManager(context, IPrivacySettingsManager.Stub.asInterface(ServiceManager.getService("privacy")));
    //todo: for java compatible
    AutoPtr<IIPrivacySettingsManager> service = (IIPrivacySettingsManager*)ServiceManager::GetService(String("privacy")).Get();
    if (FAILED(CPrivacySettingsManager::New(sContext, service, (IPrivacySettingsManager**)&sPSetMan))) {
        Slogger::E(PRIVACY_TAG, "Something went wrong with initalize variables");
        sPrivacyMode = FALSE;
        return;
    }
    // mPm = IPackageManager.Stub.asInterface(ServiceManager.getService("package"));
    sPm = (IIPackageManager*)ServiceManager::GetService(String("package")).Get();
    sPrivacyMode = TRUE;
    // }
    // catch(Exception e){
    //     e.printStackTrace();
    //     Log.e(PRIVACY_TAG, "Something went wrong with initalize variables");
    //     privacyMode = false;
    // }
}

ECode Settings::Secure::IsLocationProviderEnabled(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& provider,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return IsLocationProviderEnabledForUser(cr, provider, UserHandle::GetMyUserId(), result);
}

ECode Settings::Secure::IsLocationProviderEnabledForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& provider,
    /* [in] */ Int32 userId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    String allowedProviders;
    FAIL_RETURN(GetStringForUser(cr, ISettingsSecure::LOCATION_PROVIDERS_ALLOWED, userId, &allowedProviders))
    *result = TextUtils::DelimitedStringContains(allowedProviders, ',', provider);
    return NOERROR;
}

ECode Settings::Secure::SetLocationProviderEnabled(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& provider,
    /* [in] */ Boolean enabled)
{
    return SetLocationProviderEnabledForUser(cr, provider, enabled, UserHandle::GetMyUserId());
}

ECode Settings::Secure::SetLocationProviderEnabledForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& _provider,
    /* [in] */ Boolean enabled,
    /* [in] */ Int32 userId)
{
    // to ensure thread safety, we write the provider name with a '+' or '-'
    // and let the SettingsProvider handle it rather than reading and modifying
    // the list of enabled providers.
    String provider;
    if (enabled) {
        provider = String("+") + _provider;
    }
    else {
        provider = String("-") + _provider;
    }
    Boolean result;
    return PutStringForUser(cr, ISettingsSecure::LOCATION_PROVIDERS_ALLOWED, provider, userId, &result);
}

//================================================================================
//              Settings::Global
//================================================================================
static AutoPtr<IUri> InitGlobalCONTENTURI()
{
    StringBuilder sb("content://");
    sb += ISettings::AUTHORITY;
    sb += "/global";
    AutoPtr<IUri> uri;
    Uri::Parse(sb.ToString(), (IUri**)&uri);
    return uri;
}
const AutoPtr<IUri> Settings::Global::CONTENT_URI = InitGlobalCONTENTURI();

static AutoPtr< ArrayOf<String> > InitGlobalSettingsToBackup()
{
    AutoPtr< ArrayOf<String> > array = ArrayOf<String>::Alloc(15);
    (*array)[0] = ISettingsGlobal::STAY_ON_WHILE_PLUGGED_IN;
    (*array)[1] = ISettingsGlobal::MODE_RINGER;
    (*array)[2] = ISettingsGlobal::AUTO_TIME;
    (*array)[3] = ISettingsGlobal::AUTO_TIME_ZONE;
    (*array)[4] = ISettingsGlobal::POWER_SOUNDS_ENABLED;
    (*array)[5] = ISettingsGlobal::DOCK_SOUNDS_ENABLED;
    (*array)[6] = ISettingsGlobal::USB_MASS_STORAGE_ENABLED;
    (*array)[7] = ISettingsGlobal::ENABLE_ACCESSIBILITY_GLOBAL_GESTURE_ENABLED;
    (*array)[8] = ISettingsGlobal::WIFI_NETWORKS_AVAILABLE_NOTIFICATION_ON;
    (*array)[9] = ISettingsGlobal::WIFI_NETWORKS_AVAILABLE_REPEAT_DELAY;
    (*array)[10] = ISettingsGlobal::WIFI_WATCHDOG_POOR_NETWORK_TEST_ENABLED;
    (*array)[11] = ISettingsGlobal::WIFI_NUM_OPEN_NETWORKS_KEPT;
    (*array)[12] = ISettingsGlobal::EMERGENCY_TONE;
    (*array)[13] = ISettingsGlobal::CALL_AUTO_RETRY;
    (*array)[14] = ISettingsGlobal::DOCK_AUDIO_MEDIA_ENABLED;
    return array;
}
const AutoPtr< ArrayOf<String> > Settings::Global::SETTINGS_TO_BACKUP = InitGlobalSettingsToBackup();

const AutoPtr<Settings::NameValueCache> Settings::Global::sNameValueCache = new Settings::NameValueCache(
        ISettingsGlobal::SYS_PROP_SETTING_VERSION, CONTENT_URI,
        ISettings::CALL_METHOD_GET_GLOBAL, ISettings::CALL_METHOD_PUT_GLOBAL);

String Settings::Global::GetBluetoothHeadsetPriorityKey(
    /* [in] */ const String& address)
{
    return ISettingsGlobal::BLUETOOTH_HEADSET_PRIORITY_PREFIX + address.ToUpperCase();
}

String Settings::Global::GetBluetoothA2dpSinkPriorityKey(
    /* [in] */ const String& address)
{
    return ISettingsGlobal::BLUETOOTH_A2DP_SINK_PRIORITY_PREFIX + address.ToUpperCase();
}

String Settings::Global::GetBluetoothInputDevicePriorityKey(
    /* [in] */ const String& address)
{
    return ISettingsGlobal::BLUETOOTH_INPUT_DEVICE_PRIORITY_PREFIX + address.ToUpperCase();
}

ECode Settings::Global::GetString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    return GetStringForUser(resolver, name, UserHandle::GetMyUserId(), value);
}

ECode Settings::Global::GetStringForUser(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ Int32 userHandle,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    return sNameValueCache->GetStringForUser(resolver, name, userHandle, value);
}

ECode Settings::Global::PutString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutStringForUser(resolver, name, value, UserHandle::GetMyUserId(), result);
}

ECode Settings::Global::PutStringForUser(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (LOCAL_LOGV) {
        Slogger::V(TAG, "Global.putString(name=%s, value=%s for %d", name.string(), value.string(), userHandle);
    }
    return sNameValueCache->PutStringForUser(resolver, name, value, userHandle, result);
}

ECode Settings::Global::GetUriFor(
    /* [in] */ const String& name,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    return GetUriFor(CONTENT_URI, name, uri);
}

ECode Settings::Global::GetInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 def,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    String v;
    FAIL_RETURN(GetString(cr, name, &v))
    // try {
    Int32 _value;
    if (v.IsNull()) {
        _value = def;
    }
    else {
        if (FAILED(StringUtils::ParseInt32(v, &_value))) {
            _value = def;
        }
    }
    *value = _value;
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     return def;
    // }
}

ECode Settings::Global::GetInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0;
    String v;
    FAIL_RETURN(GetString(cr, name, &v))
    // try {
    if (FAILED(StringUtils::ParseInt32(v, value))) {
        return E_SETTING_NOT_FOUND_EXCEPTION;
    }
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     throw new SettingNotFoundException(name);
    // }
}

ECode Settings::Global::PutInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutString(cr, name, StringUtils::Int32ToString(value), result);
}

ECode Settings::Global::GetInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 def,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    String valString;
    FAIL_RETURN(GetString(cr, name, &valString))
    // try {
    Int64 _value;
    if (valString.IsNull()) {
        _value = def;
    }
    else {
        if (FAILED(StringUtils::ParseInt64(valString, &_value))) {
            _value = def;
        }
    }
    *value = _value;
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     value = def;
    // }
}

ECode Settings::Global::GetInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0;
    String valString;
    FAIL_RETURN(GetString(cr, name, &valString))
    // try {
    if (FAILED(StringUtils::ParseInt64(valString, value))) {
        return E_SETTING_NOT_FOUND_EXCEPTION;
    }
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     throw new SettingNotFoundException(name);
    // }
}

ECode Settings::Global::PutInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutString(cr, name, StringUtils::Int64ToString(value), result);
}

ECode Settings::Global::GetFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float def,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    String v;
    FAIL_RETURN(GetString(cr, name, &v))
    // try {
    Float _value;
    if (v.IsNull()) {
        _value = def;
    }
    else {
        if (FAILED(StringUtils::ParseFloat(v, &_value))) {
            _value = def;
        }
    }
    *value = _value;
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     value = def;
    // }
}

ECode Settings::Global::GetFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0;
    String v;
    FAIL_RETURN(GetString(cr, name, &v))
    if (v.IsNull()) {
        return E_SETTING_NOT_FOUND_EXCEPTION;
        // throw new SettingNotFoundException(name);
    }
    // try {
    if (FAILED(StringUtils::ParseFloat(v, value))) {
        return E_SETTING_NOT_FOUND_EXCEPTION;
    }
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     throw new SettingNotFoundException(name);
    // }
}

ECode Settings::Global::PutFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return PutString(cr, name, StringUtils::FloatToString(value), result);
}

//================================================================================
//              Settings::Bookmarks
//================================================================================

static AutoPtr<IUri> InitBookmarksContentUri()
{
    StringBuilder sb("content://");
    sb += ISettings::AUTHORITY;
    sb += "/bookmarks";
    AutoPtr<IUri> uri;
    Uri::Parse(sb.ToString(), (IUri**)&uri);
    return uri;
}
AutoPtr<IUri> Settings::Bookmarks::CONTENT_URI = InitBookmarksContentUri();

const String Settings::Bookmarks::TAG("Bookmarks");

static AutoPtr< ArrayOf<String> > IntitIntentProjection()
{
    AutoPtr< ArrayOf<String> > array = ArrayOf<String>::Alloc(1);
    (*array)[0] = ISettingsBookmarks::INTENT;
    return array;
}
const AutoPtr< ArrayOf<String> > Settings::Bookmarks::sIntentProjection = IntitIntentProjection();

static AutoPtr< ArrayOf<String> > IntitShortcutProjection()
{
    AutoPtr< ArrayOf<String> > array = ArrayOf<String>::Alloc(2);
    (*array)[0] = ISettingsBookmarks::ID;
    (*array)[1] = ISettingsBookmarks::SHORTCUT;
    return array;
}
const AutoPtr< ArrayOf<String> > Settings::Bookmarks::sShortcutProjection = IntitShortcutProjection();
const String Settings::Bookmarks::sShortcutSelection = ISettingsBookmarks::SHORTCUT + String("=?");

ECode Settings::Bookmarks::GetIntentForShortcut(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Char32 shortcut,
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent)
    AutoPtr<IIntent> _intent;

    AutoPtr<ICursor> c;
    AutoPtr< ArrayOf<String> > selectionArgs = ArrayOf<String>::Alloc(1);
    (*selectionArgs)[0] = StringUtils::Int32ToString((Int32)shortcut);
    FAIL_RETURN(cr->Query(CONTENT_URI, sIntentProjection, sShortcutSelection,
            selectionArgs, ISettingsBookmarks::ORDERING, (ICursor**)&c))
    assert(c != NULL);
    // Keep trying until we find a valid shortcut
    // try {
    Boolean hasNext = FALSE;
    while (_intent == NULL && (c->MoveToNext(&hasNext), hasNext)) {
        // try {
        Int32 colIndex;
        if (FAILED(c->GetColumnIndexOrThrow(ISettingsBookmarks::INTENT, &colIndex))) {
            Slogger::W(TAG, "Intent column not found");
            continue;
        }
        String intentURI;
        if (FAILED(c->GetString(colIndex, &intentURI))) {
            continue;
        }
        Intent::ParseUri(intentURI, 0, (IIntent**)&_intent);
        // } catch (java.net.URISyntaxException e) {
        //     // The stored URL is bad...  ignore it.
        // } catch (IllegalArgumentException e) {
        //     // Column not found
        //     Log.w(TAG, "Intent column not found", e);
        // }
    }
    // } finally {
    //     if (c != null) c.close();
    // }
    if (c != NULL) {
        c->Close();
    }
    *intent = _intent;
    REFCOUNT_ADD(*intent)
    return NOERROR;
}

ECode Settings::Bookmarks::Add(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& title,
    /* [in] */ const String& folder,
    /* [in] */ Char32 shortcut,
    /* [in] */ Int32 ordering,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    // If a shortcut is supplied, and it is already defined for
    // another bookmark, then remove the old definition.
    if (shortcut != 0) {
        AutoPtr< ArrayOf<String> > selectionArgs = ArrayOf<String>::Alloc(1);
        (*selectionArgs)[0] = StringUtils::Int32ToString((Int32)shortcut);
        Int32 rowsAffected;
        FAIL_RETURN(cr->Delete(CONTENT_URI, sShortcutSelection, selectionArgs, &rowsAffected))
    }

    AutoPtr<IContentValues> values;
    CContentValues::New((IContentValues**)&values);
    if (!title.IsNull()) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(title, (ICharSequence**)&cs);
        values->PutString(ISettingsBookmarks::TITLE, cs);
    }
    if (!folder.IsNull()) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(folder, (ICharSequence**)&cs);
        values->PutString(ISettingsBookmarks::FOLDER, cs);
    }

    String str;
    intent->ToUri(0, &str);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(str, (ICharSequence**)&cs);
    values->PutString(ISettingsBookmarks::INTENT, cs);

    if (shortcut != 0) {
        AutoPtr<IInteger32> integer;
        CInteger32::New((Int32)shortcut, (IInteger32**)&integer);
        values->PutInt32(ISettingsBookmarks::SHORTCUT, integer);
    }

    AutoPtr<IInteger32> integer;
    CInteger32::New(ordering, (IInteger32**)&integer);
    values->PutInt32(ISettingsBookmarks::ORDERING, integer);
    return cr->Insert(CONTENT_URI, values, uri);
}

AutoPtr<ICharSequence> Settings::Bookmarks::GetLabelForFolder(
    /* [in] */ IResources* r,
    /* [in] */ const String& folder)
{
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(folder, (ICharSequence**)&cs);
    return cs;
}

ECode Settings::Bookmarks::GetTitle(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title)
    *title = NULL;

    Int32 titleColumn;
    cursor->GetColumnIndex(ISettingsBookmarks::TITLE, &titleColumn);
    Int32 intentColumn;
    cursor->GetColumnIndex(ISettingsBookmarks::INTENT, &intentColumn);
    if (titleColumn == -1 || intentColumn == -1) {
        Slogger::E(TAG, "The cursor must contain the TITLE and INTENT columns.");
        return E_ILLEGAL_SQLITE_EXCEPTION;
        // throw new IllegalArgumentException(
        //         "The cursor must contain the TITLE and INTENT columns.");
    }

    String str;
    FAIL_RETURN(cursor->GetString(titleColumn, &str))
    if (!TextUtils::IsEmpty(str)) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(str, (ICharSequence**)&cs);
        *title = cs;
        REFCOUNT_ADD(*title)
        return NOERROR;
    }

    String intentUri;
    FAIL_RETURN(cursor->GetString(intentColumn, &intentUri))
    if (TextUtils::IsEmpty(intentUri)) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(String(""), (ICharSequence**)&cs);
        *title = cs;
        REFCOUNT_ADD(*title)
        return NOERROR;
    }

    AutoPtr<IIntent> intent;
    // try {
    if (FAILED(Intent::ParseUri(intentUri, 0, (IIntent**)&intent))) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(String(""), (ICharSequence**)&cs);
        *title = cs;
        REFCOUNT_ADD(*title)
        return NOERROR;
    }
    // } catch (URISyntaxException e) {
    //     return "";
    // }

    AutoPtr<IPackageManager> packageManager;
    context->GetPackageManager((IPackageManager**)&packageManager);
    AutoPtr<IResolveInfo> info;
    packageManager->ResolveActivity(intent, 0, (IResolveInfo**)&info);
    if (info) {
        return info->LoadLabel(packageManager, title);
    }
    else {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(String(""), (ICharSequence**)&cs);
        *title = cs;
        REFCOUNT_ADD(*title)
        return NOERROR;
    }
}

String Settings::GetGTalkDeviceId(
    /* [in] */ Int64 androidId)
{
    return String("android-") + StringUtils::Int64ToHexString(androidId);
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos
