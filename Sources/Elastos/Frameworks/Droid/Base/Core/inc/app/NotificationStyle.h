#ifndef __ELASTOS_DROID_APP_NOTIFICATIONSTYLE_H__
#define __ELASTOS_DROID_APP_NOTIFICATIONSTYLE_H__

#include "ext/frameworkext.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Widget::IRemoteViews;

namespace Elastos{
namespace Droid{
namespace App{

class NotificationStyle
{
public:
    NotificationStyle();

    virtual ~NotificationStyle();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    CARAPI SetBuilder(
        /* [in] */ INotificationBuilder * builder);

    virtual CARAPI Build(
        /* [out] */ INotification **notification) = 0;

public:
    AutoPtr<ICharSequence> mBigContentTitle;
    AutoPtr<ICharSequence> mSummaryText;
    Boolean mSummaryTextSet;
    AutoPtr<INotificationBuilder> mBuilder;

protected:
    /**
     * Overrides ContentTitle in the big form of the template.
     * This defaults to the value passed to setContentTitle().
     */
    CARAPI_(void) InternalSetBigContentTitle(
        /* [in] */ ICharSequence * title);

    /**
     * Set the first line of text after the detail section in the big form of the template.
     */
    CARAPI_(void) InternalSetSummaryText(
        /* [in] */ ICharSequence * cs);

    CARAPI CheckBuilder();

    CARAPI_(AutoPtr<IRemoteViews>) GetStandardView(
        /* [in] */ Int32 layoutId);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_NOTIFICATIONSTYLE_H__
