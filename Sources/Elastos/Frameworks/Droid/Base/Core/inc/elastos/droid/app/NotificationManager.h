
#ifndef __ELASTOS_DROID_APP_NOTIFICATIONMANAGER_H__
#define __ELASTOS_DROID_APP_NOTIFICATIONMANAGER_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace App {

class NotificationManager
    : public ElRefBase
    , public INotificationManager
{
public:
    CAR_INTERFACE_DECL();

    /** @hide */
    static AutoPtr<IINotificationManager> GetService();

    /** @hide */
    static AutoPtr<INotificationManager> From(
        /* [in] */ IContext* context);

    /* package */ NotificationManager(
        /* [in] */ IContext* context,
        /* [in] */ IHandler* handler);

    /**
     * Post a notification to be shown in the status bar. If a notification with
     * the same id has already been posted by your application and has not yet been canceled, it
     * will be replaced by the updated information.
     *
     * @param id An identifier for this notification unique within your
     *        application.
     * @param notification A {@link Notification} object describing what to show the user. Must not
     *        be null.
     */
    CARAPI Notify(
        /* [in] */ Int32 id,
        /* [in] */ INotification* notification);

    /**
     * Post a notification to be shown in the status bar. If a notification with
     * the same tag and id has already been posted by your application and has not yet been
     * canceled, it will be replaced by the updated information.
     *
     * @param tag A string identifier for this notification.  May be {@code null}.
     * @param id An identifier for this notification.  The pair (tag, id) must be unique
     *        within your application.
     * @param notification A {@link Notification} object describing what to
     *        show the user. Must not be null.
     */
    CARAPI Notify(
        /* [in] */ const String& tag,
        /* [in] */ Int32 id,
        /* [in] */ INotification* notification);

    /**
     * @hide
     */
    CARAPI NotifyAsUser(
        /* [in] */ const String& tag,
        /* [in] */ Int32 id,
        /* [in] */ INotification* notification,
        /* [in] */ IUserHandle* user);

    /**
     * Cancel a previously shown notification.  If it's transient, the view
     * will be hidden.  If it's persistent, it will be removed from the status
     * bar.
     */
    CARAPI Cancel(
        /* [in] */ Int32 id);

    /**
     * Cancel a previously shown notification.  If it's transient, the view
     * will be hidden.  If it's persistent, it will be removed from the status
     * bar.
     */
    CARAPI Cancel(
        /* [in] */ const String& tag,
        /* [in] */ Int32 id);

    /**
     * @hide
     */
    CARAPI CancelAsUser(
        /* [in] */ const String& tag,
        /* [in] */ Int32 id,
        /* [in] */ IUserHandle* user);

    /**
     * Cancel all previously shown notifications. See {@link #cancel} for the
     * detailed behavior.
     */
    CARAPI CancelAll();

    /**
     * @hide
     */
    AutoPtr<IComponentName> GetEffectsSuppressor();

    /**
     * @hide
     */
    Boolean MatchesCallFilter(
        /* [in] */ IBundle* extras);

private:
    static const String TAG;
    static const Boolean DEBUG;
    static const Boolean localLOGV/* || android.util.Config.LOGV*/;

    static AutoPtr<IINotificationManager> sService;

    AutoPtr<IContext> mContext;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_NOTIFICATIONMANAGER_H__

