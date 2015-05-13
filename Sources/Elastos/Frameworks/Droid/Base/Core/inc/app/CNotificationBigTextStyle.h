
#ifndef __CNOTIFICATIONINBOXSTYLE_H__
#define __CNOTIFICATIONINBOXSTYLE_H__

#include "_CNotificationBigTextStyle.h"
#include "app/NotificationStyle.h"

namespace Elastos {
namespace Droid {
namespace App {

    /**
     * Helper class for generating large-format notifications that include a lot of text.
     *
     * This class is a "rebuilder": It consumes a Builder object and modifies its behavior, like so:
     * <pre class="prettyprint">
     * Notification noti = new Notification.BigTextStyle(
     *      new Notification.Builder()
     *         .setContentTitle(&quot;New mail from &quot; + sender.toString())
     *         .setContentText(subject)
     *         .setSmallIcon(R.drawable.new_mail)
     *         .setLargeIcon(aBitmap))
     *      .bigText(aVeryLongString)
     *      .build();
     * </pre>
     *
     * @see Notification#bigContentView
     */
CarClass(CNotificationBigTextStyle), public NotificationStyle
{
public:
    CNotificationBigTextStyle();

    virtual ~CNotificationBigTextStyle();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ INotificationBuilder* builder);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetBuilder(
        /* [in] */ INotificationBuilder* builder);

    CARAPI Build(
        /* [out] */ INotification** notification);

    /**
     * Overrides ContentTitle in the big form of the template.
     * This defaults to the value passed to setContentTitle().
     */
    CARAPI SetBigContentTitle(
        /* [in] */ ICharSequence* title);

    /**
     * Set the first line of text after the detail section in the big form of the template.
     */
    CARAPI SetSummaryText(
        /* [in] */ ICharSequence* cs);

    /**
     * Provide the longer text to be displayed in the big form of the
     * template in place of the content text.
     */
    CARAPI BigText(
        /* [in] */ ICharSequence* cs);

private:
    AutoPtr<IRemoteViews> MakeBigContentView();

private:
    AutoPtr<ICharSequence> mBigText;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __CNOTIFICATIONINBOXSTYLE_H__
