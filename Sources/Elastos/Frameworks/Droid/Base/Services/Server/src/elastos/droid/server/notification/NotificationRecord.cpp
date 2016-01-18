
#include "elastos/droid/server/notification/NotificationRecord.h"
#include "elastos/droid/app/ActivityManagerNative.h"
#include "elastos/droid/os/Binder.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/R.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

// using Elastos::Droid::App::IActivityManagerHelper;
// using Elastos::Droid::App::CActivityManagerHelper;
// using Elastos::Droid::Content::IContentResolver;
// using Elastos::Droid::Content::IIntent;
// using Elastos::Droid::Content::CIntent;
// using Elastos::Droid::Content::Pm::IPackageManager;
// using Elastos::Droid::Content::Pm::IIPackageManager;
// using Elastos::Droid::Content::Pm::IApplicationInfo;
// using Elastos::Droid::Provider::ISettingsSystem;
// using Elastos::Droid::Provider::CSettingsSystem;
// using Elastos::Droid::Os::Binder;
// using Elastos::Droid::Os::IUserHandle;
// using Elastos::Droid::Text::TextUtils;
// using Elastos::Core::StringBuilder;
// using Elastos::Core::StringUtils;
// using Elastos::Utility::CArrayList;
// using Elastos::Utility::ICollection;
// using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Notification {

CAR_INTERFACE_IMPL(NotificationRecord, Object, INotificationRecord);

NotificationRecord::NotificationRecord(
    /* [in] */ IStatusBarNotification* sbn,
    /* [in] */ Int32 score)
{
    mSbn = sbn;
    mScore = score;
    AutoPtr<INotification> notification;
    sbn->GetNotification((INotification**)&notification);
    notification->GetFlags(&mOriginalFlags);
    mRankingTimeMs = CalculateRankingTimeMs(0);
}

NotificationRecord::~NotificationRecord()
{}

void NotificationRecord::CopyRankingInformation(
    /* [in] */ NotificationRecord* previous)
{
    mContactAffinity = previous->mContactAffinity;
    mRecentlyIntrusive = previous->mRecentlyIntrusive;
    mPackagePriority = previous->mPackagePriority;
    mPackageVisibility = previous->mPackageVisibility;
    mIntercept = previous->mIntercept;
    mRankingTimeMs = CalculateRankingTimeMs(previous->GetRankingTimeMs());
    // Don't copy mGlobalSortKey, recompute it.
}

AutoPtr<INotification> NotificationRecord::GetNotification()
{
    AutoPtr<INotification> notification;
    mSbn->GetNotification((INotification**)&notification);
    return notification;
}

Int32 NotificationRecord::GetFlags()
{
    AutoPtr<INotification> notification;
    mSbn->GetNotification((INotification**)&notification);
    Int32 flags;
    notification->GetFlags(&flags);
    return flags;
}

AutoPtr<IUserHandle> NotificationRecord::GetUser()
{
    AutoPtr<IUserHandle> user;
    mSbn->GetUser((IUserHandle**)&user);
    return user;
}

String NotificationRecord::GetKey()
{
    String key;
    mSbn->GetKey(&key);
    return key;
}

Int32 NotificationRecord::GetUserId()
{
    Int32 id;
    mSbn->GetUserId(&id);
    return id;
}

void NotificationRecord::Dump(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& prefix,
    /* [in] */ IContext* baseContext)
{
    AutoPtr<INotification> notification;
    mSbn->GetNotification((INotification**)&notification);

    StringBuilder buider;
    buider += prefix;
    buider += this;
    pw->Println(buider.ToString());

    Int32 uid;
    mSbn->GetUid(&uid);
    Int32 userId;
    mSbn->GetUserId(&userId);
    buider.Reset();
    buider += prefix;
    buider += "  uid=";
    buider += uid;
    buider += " userId=";
    buider += userId;
    pw->Println(buider.ToString());

    Int32 icon;
    notification->GetIcon(&icon);
    String packageName;
    mSbn->GetPackageName(&packageName);
    String str;
    IdDebugString(baseContext, packageName, icon, &str);
    buider.Reset();
    buider += prefix;
    buider += "  icon=0x";
    buider += StringUtils::Int32ToString(icon);
    buider += " / ";
    buider += str;
    pw->Println(buider.ToString());

    Int32 priority;
    notification->GetPriority(&priority);
    Int32 score;
    mSbn->GetScore(&score);
    buider.Reset();
    buider += prefix;
    buider += "  pri=";
    buider += priority;
    buider += " score=";
    buider += score;
    pw->Println(buider.ToString());

    String key;
    mSbn->GetKey(&key);
    buider.Reset();
    buider += prefix;
    buider += "  key=";
    buider += key;
    pw->Println(buider.ToString());

    buider.Reset();
    buider += prefix;
    buider += "  groupKey=";
    buider += GetGroupKey();
    pw->Println(buider.ToString());

    AutoPtr<IPendingIntent> intent;
    notification->GetContentIntent((IPendingIntent**)&intent);
    buider.Reset();
    buider += prefix;
    buider += "  contentIntent=";
    buider += intent;
    pw->Println(buider.ToString());

    intent = NULL;
    notification->GetDeleteIntent((IPendingIntent**)&intent);
    buider.Reset();
    buider += prefix;
    buider += "  deleteIntent=";
    buider += intent;
    pw->Println(buider.ToString());

    AutoPtr<ICharSequence> tickerText;
    notification->GetTickerText((ICharSequence**)&tickerText);
    buider.Reset();
    buider += prefix;
    buider += "  tickerText=";
    buider += tickerText;
    pw->Println(buider.ToString());

    AutoPtr<IRemoteViews> contentView;
    notification->GetContentView((IRemoteViews**)&contentView);
    buider.Reset();
    buider += prefix;
    buider += "  contentView=";
    buider += contentView;
    pw->Println(buider.ToString());

    Int32 defaults;
    notification->GetDefaults(&defaults);
    Int32 flags;
    notification->GetFlags(&flags);
    str = "";
    str.AppendFormat("  defaults=0x%08x flags=0x%08x", defaults, flags);
    pw->Println(prefix + str);

    AutoPtr<IUri> sound;
    notification->GetSound((IUri**)&sound);
    buider.Reset();
    buider += prefix;
    buider += "  sound=";
    buider += sound;
    pw->Println(buider.ToString());

    Int32 audioStreamType;
    notification->GetAudioStreamType





    pw->Println(prefix + "  audioStreamType=" + notification.audioStreamType);
    pw->Println(prefix + "  audioAttributes=" + notification.audioAttributes);
    pw->Println(prefix + String.format("  color=0x%08x", notification.color));
    pw->Println(prefix + "  vibrate=" + Arrays.toString(notification.vibrate));
    pw->Println(prefix + String.format("  led=0x%08x onMs=%d offMs=%d",
            notification.ledARGB, notification.ledOnMS, notification.ledOffMS));
    if (notification.actions != null && notification.actions.length > 0) {
        pw->Println(prefix + "  actions={");
        final int N = notification.actions.length;
        for (int i=0; i<N; i++) {
            final Notification.Action action = notification.actions[i];
            pw->Println(String.format("%s    [%d] \"%s\" -> %s",
                    prefix,
                    i,
                    action.title,
                    action.actionIntent.toString()
                    ));
        }
        pw->Println(prefix + "  }");
    }
    if (notification.extras != null && notification.extras.size() > 0) {
        pw->Println(prefix + "  extras={");
        for (String key : notification.extras.keySet()) {
            pw->Print(prefix + "    " + key + "=");
            Object val = notification.extras.get(key);
            if (val == null) {
                pw->Println("null");
            } else {
                pw->Print(val.getClass().getSimpleName());
                if (val instanceof CharSequence || val instanceof String) {
                    // redact contents from bugreports
                } else if (val instanceof Bitmap) {
                    pw->Print(String.format(" (%dx%d)",
                            ((Bitmap) val).getWidth(),
                            ((Bitmap) val).getHeight()));
                } else if (val.getClass().isArray()) {
                    final int N = Array.getLength(val);
                    pw->Println(" (" + N + ")");
                } else {
                    pw->Print(" (" + String.valueOf(val) + ")");
                }
                pw->Println();
            }
        }
        pw->Println(prefix + "  }");
    }
    pw->Println(prefix + "  stats=" + stats.toString());
    pw->Println(prefix + "  mContactAffinity=" + mContactAffinity);
    pw->Println(prefix + "  mRecentlyIntrusive=" + mRecentlyIntrusive);
    pw->Println(prefix + "  mPackagePriority=" + mPackagePriority);
    pw->Println(prefix + "  mPackageVisibility=" + mPackageVisibility);
    pw->Println(prefix + "  mIntercept=" + mIntercept);
    pw->Println(prefix + "  mGlobalSortKey=" + mGlobalSortKey);
    pw->Println(prefix + "  mRankingTimeMs=" + mRankingTimeMs);
}

ECode NotificationRecord::IdDebugString(
    /* [in] */ IContext* baseContext,
    /* [in] */ const String& packageName,
    /* [in] */ Int32 id
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    AutoPtr<IContext> c;
    ECode ec;

    if (!packageName.IsNull()) {
        // try {
        ec = baseContext->CreatePackageContext(packageName, 0, (IContext**)&c);
        if (FAILED(ec)) {
            c = baseContext;
        }
        // } catch (NameNotFoundException e) {
        //     c = baseContext;
        // }
    }
    else {
        c = baseContext;
    }

    AutoPtr<IResources> r;
    c->GetResources((IResources**)&r);
    // try {
    String name;
    ec = r->GetResourceName(id, &name);
    if (FAILED(ec)) {
        *str = "<name unknown>";
        return NOERROR;
    }
    // } catch (Resources.NotFoundException e) {
    //     return "<name unknown>";
    // }
    *str = name;
    return NOERROR;
}

ECode NotificationRecord::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    AutoPtr<ISystem> sys;
    CSystem::AcquireSingleton((ISystem**)&sys);
    Int32 HashCode;
    sys->IdentityHashCode((INotificationRecord*)this, &HashCode);

    String packageName;
    mSbn->GetPackageName(&packageName);
    AutoPtr<IUserHandle> user;
    mSbn->GetUser((IUserHandle**)&user);
    String result1;
    user->ToString(&result1);
    Int32 id;
    mSbn->GetId(&id);
    String tag;
    mSbn->GetTag(&tag);
    Int32 score;
    mSbn->GetScore(&score);
    String key;
    mSbn->GetKey(&key);

    AutoPtr<INotification> notification;
    mSbn->GetNotification((INotification**)&notification);
    String result2;
    notification->ToString(&result2);

    str.AppendFormat("NotificationRecord(0x%08x: pkg=%s user=%s id=%d tag=%s score=%d key=%s: %s)",
            HashCode, packageName, result1.string(), id, tag, score, key,
            result2.string());
    return NOERROR;
}

void NotificationRecord::SetContactAffinity(
    /* [in] */ Float contactAffinity)
{
    mContactAffinity = contactAffinity;
}

Float NotificationRecord::GetContactAffinity()
{
    return mContactAffinity;
}

void NotificationRecord::SetRecentlyIntusive(
    /* [in] */ Boolean recentlyIntrusive)
{
    mRecentlyIntrusive = recentlyIntrusive;
}

Boolean NotificationRecord::IsRecentlyIntrusive()
{
    return mRecentlyIntrusive;
}

void NotificationRecord::SetPackagePriority(
    /* [in] */ Int32 packagePriority)
{
    mPackagePriority = packagePriority;
}

Int32 NotificationRecord::GetPackagePriority()
{
    return mPackagePriority;
}

void NotificationRecord::SetPackageVisibilityOverride(
    /* [in] */ Int32 packageVisibility)
{
    mPackageVisibility = packageVisibility;
}

Int32 NotificationRecord::GetPackageVisibilityOverride()
{
    return mPackageVisibility;
}

Boolean NotificationRecord::SetIntercepted(
    /* [in] */ Boolean intercept)
{
    mIntercept = intercept;
    return mIntercept;
}

Boolean NotificationRecord::IsIntercepted()
{
    return mIntercept;
}

Boolean NotificationRecord::IsCategory(
    /* [in] */ const String& category)
{
    String cat;
    GetNotification()->GetCategory(&cat);
    return Objects::Equals(CoreUtils::Convert(cat), CoreUtils::Convert(category));
}

Boolean NotificationRecord::IsAudioStream(
    /* [in] */ Int32 stream)
{
    Int32 type;
    GetNotification()->GetAudioStreamType(&type);
    return type == stream;
}

Boolean NotificationRecord::IsAudioAttributesUsage(
    /* [in] */ Int32 usage)
{
    AutoPtr<IAudioAttributes> attributes;
    GetNotification()->GetAudioAttributes((IAudioAttributes**)&attributes);
    Int32 result;
    return attributes != NULL && (attributes->GetUsage(&result), result) == usage;
}

Int64 NotificationRecord::GetRankingTimeMs()
{
    return mRankingTimeMs;
}

Int64 NotificationRecord::CalculateRankingTimeMs(
    /* [in] */ Int64 previousRankingTimeMs)
{
    AutoPtr<INotification> n = GetNotification();
    // Take developer provided 'when', unless it's in the future.
    Int64 postTime;
    mSbn->GetPostTime(&postTime);
    Int64 when;
    n->GetWhen(&when);
    if (when != 0 && when <= postTime) {
        return when;
    }
    // If we've ranked a previous instance with a timestamp, inherit it. This case is
    // important in order to have ranking stability for updating notifications.
    if (previousRankingTimeMs > 0) {
        return previousRankingTimeMs;
    }
    return postTime;
}

void NotificationRecord::SetGlobalSortKey(
    /* [in] */ const String& globalSortKey)
{
    mGlobalSortKey = globalSortKey;
}

String NotificationRecord::GetGlobalSortKey()
{
    return mGlobalSortKey;
}

void NotificationRecord::SetAuthoritativeRank(
    /* [in] */ Int32 authoritativeRank)
{
    mAuthoritativeRank = authoritativeRank;
}

Int32 NotificationRecord::GetAuthoritativeRank()
{
    return mAuthoritativeRank;
}

String NotificationRecord::GetGroupKey()
{
    String groupKey;
    mSbn->GetGroupKey(&groupKey);
    return groupKey;
}

} // Notification
} // Server
} // Droid
} // Elastos
