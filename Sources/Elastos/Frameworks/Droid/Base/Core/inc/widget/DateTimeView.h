
#ifndef __DATETIMEVIEW_H__
#define __DATETIMEVIEW_H__

#include <Elastos.CoreLibrary.h>
#include "ext/frameworkext.h"
#include "widget/TextView.h"
#include "content/BroadcastReceiver.h"
#include "database/ContentObserver.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Text::IDateFormat;
using Elastos::Utility::IDate;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Widget::TextView;
using Elastos::Text::CSimpleDateFormat;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Database::ContentObserver;


namespace Elastos {
namespace Droid {
namespace Widget {

class DateTimeView : public Elastos::Droid::Widget::TextView
{
private:
    class DateTimeViewReceiver : public BroadcastReceiver
    {
    public:
        DateTimeViewReceiver(
            /* [in] */ DateTimeView* host);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("DateTimeView::DateTimeViewReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        DateTimeView* mHost;
    };

    class DateTimeViewObserver : public ContentObserver
    {
    public:
        DateTimeViewObserver(
            /* [in] */ DateTimeView* host);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange);
    private:
        DateTimeView* mHost;
    };

public:

    DateTimeView(
        /* [in] */ IContext* context);

    DateTimeView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetTime(
        /* [in] */ Int64 time);

    CARAPI Update();

protected:
    DateTimeView();

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);


    CARAPI OnAttachedToWindow();

    CARAPI OnDetachedFromWindow();

private:
    CARAPI_(AutoPtr<IDateFormat>) GetTimeFormat();

    CARAPI_(AutoPtr<IDateFormat>) GetDateFormat();

    CARAPI RegisterReceivers();

    CARAPI UnRegisterReceivers();

private:
    const static Int64 TWELVE_HOURS_IN_MINUTES;
    const static Int64 TWENTY_FOUR_HOURS_IN_MILLIS;

    const static Int32 SHOW_TIME;
    const static Int32 SHOW_MONTH_DAY_YEAR;

    Boolean mAttachedToWindow;
    Int64 mUpdateTimeMillis;

    AutoPtr<IDate> mTime;
    Int64 mTimeMillis;

    Int32 mLastDisplay;
    AutoPtr<IDateFormat> mLastFormat;

    AutoPtr<BroadcastReceiver> mBroadcastReceiver;
    AutoPtr<ContentObserver> mContentObserver;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif
