
#ifndef __CNOTIFICATIONINBOXSTYLE_H__
#define __CNOTIFICATIONINBOXSTYLE_H__

#include "_CNotificationInboxStyle.h"
#include "app/NotificationStyle.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Droid {
namespace App {

/**
 * Helper class for generating large-format notifications that include a list of (up to 5) strings.
 *
 * This class is a "rebuilder": It consumes a Builder object and modifies its behavior, like so:
 * <pre class="prettyprint">
 * Notification noti = new Notification.InboxStyle(
 *      new Notification.Builder()
 *         .setContentTitle(&quot;5 New mails from &quot; + sender.toString())
 *         .setContentText(subject)
 *         .setSmallIcon(R.drawable.new_mail)
 *         .setLargeIcon(aBitmap))
 *      .addLine(str1)
 *      .addLine(str2)
 *      .setContentTitle("")
 *      .setSummaryText(&quot;+3 more&quot;)
 *      .build();
 * </pre>
 *
 * @see Notification#bigContentView
 */
CarClass(CNotificationInboxStyle), public NotificationStyle
{
public:
    CNotificationInboxStyle();

    virtual ~CNotificationInboxStyle();

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
     * Append a line to the digest section of the Inbox notification.
     */
    CARAPI AddLine(
        /* [in] */ ICharSequence* cs);

private:
    AutoPtr<IRemoteViews> MakeBigContentView();

private:
    List<AutoPtr<ICharSequence> > mTexts;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __CNOTIFICATIONINBOXSTYLE_H__
