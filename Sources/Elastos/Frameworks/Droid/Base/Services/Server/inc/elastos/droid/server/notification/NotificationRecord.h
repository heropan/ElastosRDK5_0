
#ifndef __ELASTOS_DROID_SERVER_NOTIFICATION_NOTIFICATIONRECORD_H__
#define __ELASTOS_DROID_SERVER_NOTIFICATION_NOTIFICATIONRECORD_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/database/ContentObserver.h"
// #include "elastos/droid/os/Handler.h"

using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Database::ContentObserver;
// using Elastos::Droid::Os::Handler;
using Elastos::Droid::Utility::IArrayMap;
using Elastos::Droid::Utility::IArraySet;
using Elastos::Core::ICharSequence;
using Elastos::Utility::IArrayList;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Notification {

/**
 * Holds data about notifications that should not be shared with the
 * {@link android.service.notification.NotificationListenerService}s.
 *
 * <p>These objects should not be mutated unless the code is synchronized
 * on {@link NotificationManagerService#mNotificationList}, and any
 * modification should be followed by a sorting of that list.</p>
 *
 * <p>Is sortable by {@link NotificationComparator}.</p>
 *
 * {@hide}
 */
class NotificationRecord
    : public Object
    , public INotificationRecord
{
public:
    CAR_INTERFACE_DECL();

    // @VisibleForTesting
    NotificationRecord(
        /* [in] */ IStatusBarNotification* sbn,
        /* [in] */ Int32 score);

    ~NotificationRecord();

    // copy any notes that the ranking system may have made before the update
    CARAPI(void) CopyRankingInformation(
        /* [in] */ NotificationRecord* previous);

    CARAPI_(AutoPtr<INotification>) GetNotification();

    CARAPI_(Int32) GetFlags();

    CARAPI_(AutoPtr<IUserHandle>) GetUser();

    CARAPI_(String) GetKey();

    /** @deprecated Use {@link #getUser()} instead. */
    CARAPI_(Int32) GetUserId();

    // @Override
    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI_(void) SetContactAffinity(
        /* [in] */ Float contactAffinity);

    CARAPI_(Float) GetContactAffinity();

    CARAPI_(void) SetRecentlyIntusive(
        /* [in] */ Boolean recentlyIntrusive);

    CARAPI_(Boolean) IsRecentlyIntrusive();

    CARAPI_(void) SetPackagePriority(
        /* [in] */ Int32 packagePriority);

    CARAPI_(Int32) GetPackagePriority();

    CARAPI_(void) SetPackageVisibilityOverride(
        /* [in] */ Int32 packageVisibility);

    CARAPI_(Int32) GetPackageVisibilityOverride();

    CARAPI_(Boolean) SetIntercepted(
        /* [in] */ Boolean intercept);

    CARAPI_(Boolean) IsIntercepted();

    CARAPI_(Boolean) IsCategory(
        /* [in] */ const String& category);

    CARAPI_(Boolean) IsAudioStream(
        /* [in] */ Int32 stream);

    CARAPI_(Boolean) IsAudioAttributesUsage(
        /* [in] */ Int32 usage);

    /**
     * Returns the timestamp to use for time-based sorting in the ranker.
     */
    CARAPI_(Int64) GetRankingTimeMs();

    CARAPI_(void) SetGlobalSortKey(
        /* [in] */ const String& globalSortKey);

    CARAPI_(String) GetGlobalSortKey();

    CARAPI_(void) SetAuthoritativeRank(
        /* [in] */ Int32 authoritativeRank);

    CARAPI_(Int32) GetAuthoritativeRank();

    CARAPI_(String) GetGroupKey();

protected:
    CARAPI_(void) Dump(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& prefix,
        /* [in] */ IContext* baseContext);

    static CARAPI IdDebugString(
        /* [in] */ IContext* baseContext,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 id
        /* [out] */ String* str);

private:
    /**
     * @param previousRankingTimeMs for updated notifications, {@link #getRankingTimeMs()}
     *     of the previous notification record, 0 otherwise
     */
    CARAPI_(Int64) CalculateRankingTimeMs(
        /* [in] */ Int64 previousRankingTimeMs);

public:
    // Is this record an update of an old record?
    Boolean mIsUpdate;

protected:
    AutoPtr<IStatusBarNotification> mSbn;
    Int32 mOriginalFlags;

    AutoPtr<NotificationUsageStats::SingleNotificationStats> mStats;
    Boolean IsCanceled;
    Int32 mScore;

private:
    // These members are used by NotificationSignalExtractors
    // to communicate with the ranking module.
    Float mContactAffinity;
    Boolean mRecentlyIntrusive;

    // is this notification currently being intercepted by Zen Mode?
    Boolean mIntercept;

    // The timestamp used for ranking.
    Int64 mRankingTimeMs;

    Int32 mPackagePriority;

    Int32 mAuthoritativeRank;
    String mGlobalSortKey;
    Int32 mPackageVisibility;
};

} // Notification
} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_NOTIFICATION_NOTIFICATIONRECORD_H__
