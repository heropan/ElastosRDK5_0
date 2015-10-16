#ifndef __ELASTOS_DROID_WIDGET_DIGITALCLOCK_H__
#define __ELASTOS_DROID_WIDGET_DIGITALCLOCK_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/TextView.h"
#include "elastos/droid/database/ContentObserver.h"
#include "elastos/droid/os/Runnable.h"

using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Database::ContentObserver;
using Elastos::Utility::ICalendar;

namespace Elastos{
namespace Droid{
namespace Widget{

class DigitalClock : public TextView
{
private:
    class DigitalClockRunnable
        : public Runnable
    {
    public:
        DigitalClockRunnable(
            /* [in] */ DigitalClock* host);

        CARAPI Run();

    private:
        DigitalClock* mHost;
    };

    class FormatChangeObserver
        : public ContentObserver
    {
    public:
        FormatChangeObserver(
            /* [in] */ DigitalClock* host);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        DigitalClock* mHost;
    };

public:
    DigitalClock(
        /* [in] */ IContext* context);

    DigitalClock(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

protected:
    DigitalClock();

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI OnAttachedToWindow();

    CARAPI OnDetachedFromWindow();

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

private:
    CARAPI_(void) InitClock();

    CARAPI_(Boolean) Get24HourMode();

    CARAPI_(void) SetFormat();

public:
    AutoPtr<ICalendar> mCalendar;
    String mFormat;

private:
    static const String m12;
    static const String m24;

    AutoPtr<FormatChangeObserver> mFormatChangeObserver;
    AutoPtr<DigitalClockRunnable> mTicker;
    AutoPtr<IHandler> mHandler;

    Boolean mTickerStopped;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_DIGITALCLOCK_H__
