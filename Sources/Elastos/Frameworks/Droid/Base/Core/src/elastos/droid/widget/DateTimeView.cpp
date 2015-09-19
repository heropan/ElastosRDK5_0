#include "ext/frameworkext.h"
#include "widget/DateTimeView.h"
#include "text/format/DateFormat.h"
#include "text/format/CTime.h"
#include "content/CIntentFilter.h"
#include "content/CIntent.h"
#include "provider/Settings.h"
#include <elastos/utility/logging/Slogger.h>
#include <R.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Text::Format::DateFormat;
using Elastos::Droid::Text::Format::ITime;
using Elastos::Droid::Text::Format::CTime;
using Elastos::Text::ISimpleDateFormat;
using Elastos::Text::IDateFormatHelper;
using Elastos::Text::CDateFormatHelper;
using Elastos::Utility::IDate;
using Elastos::Utility::CDate;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Provider::Settings;
using Elastos::Droid::Provider::ISettingsSystem;
using Elastos::Droid::Widget::TextView;

namespace Elastos {
namespace Droid {
namespace Widget {

const Int64 DateTimeView::TWELVE_HOURS_IN_MINUTES = 12 * 60;
const Int64 DateTimeView::TWENTY_FOUR_HOURS_IN_MILLIS = 24 * 60 * 60 * 1000;

const Int32 DateTimeView::SHOW_TIME = 0;
const Int32 DateTimeView::SHOW_MONTH_DAY_YEAR = 1;

DateTimeView::DateTimeView()
    : mAttachedToWindow(FALSE)
    , mTimeMillis(0)
    , mLastDisplay(-1)
{
    mBroadcastReceiver = new DateTimeViewReceiver(this);
}

DateTimeView::DateTimeView(
    /* [in] */ IContext* context)
    : mAttachedToWindow(FALSE)
    , mTimeMillis(0)
    , mLastDisplay(-1)
{
    Init(context);
}

DateTimeView::DateTimeView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mAttachedToWindow(FALSE)
    , mTimeMillis(0)
    , mLastDisplay(-1)
{
    Init(context, attrs);
}

ECode DateTimeView::Init(
    /* [in] */ IContext* context)
{
    TextView::Init(context);
    return NOERROR;
}

ECode DateTimeView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    TextView::Init(context, attrs);
    return NOERROR;
}

ECode DateTimeView::OnAttachedToWindow()
{
    TextView::OnAttachedToWindow();
    RegisterReceivers();
    mAttachedToWindow = TRUE;
    return NOERROR;
}

ECode DateTimeView::OnDetachedFromWindow()
{
    TextView::OnDetachedFromWindow();
    UnRegisterReceivers();
    mAttachedToWindow = FALSE;
    return NOERROR;
}

ECode DateTimeView::SetTime(
    /* [in] */ Int64 time)
{
    AutoPtr<ITime> inTime;
    CTime::New((ITime**)&inTime);
    inTime->Set(time);
    inTime->SetSecond(0);
    inTime->ToMillis(FALSE, &mTimeMillis);
    Int32 year, month, monthDay, hour, minute;
    inTime->GetYear(&year);
    inTime->GetMonth(&month);
    inTime->GetMonthDay(&monthDay);
    inTime->GetHour(&hour);
    inTime->GetMinute(&minute);
    mTime = NULL;
    ASSERT_SUCCEEDED(CDate::New((year - 1900), month, monthDay, hour, minute, 0, (IDate**)&mTime));

    Update();
    return NOERROR;
}

ECode DateTimeView::Update()
{
    if (!mTime) {
        return NOERROR;
    }

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 start;
    system->GetCurrentTimeMillis(&start);

    Int32 display = 0;
    AutoPtr<IDate> date = mTime;
    AutoPtr<ITime> time;
    ASSERT_SUCCEEDED(CTime::New((ITime**)&time));
    time->Set(mTimeMillis);
    time->SetSecond(0);
    Int32 hour;
    time->GetHour(&hour);
    time->SetHour(hour-12);
    Int64 twelveHoursBefore = 0;
    time->ToMillis(FALSE, &twelveHoursBefore);
    Int32 hour1 = 0;
    time->GetHour(&hour1);
    time->SetHour(hour1+12);
    Int64 twelveHoursAfter = 0;
    time->ToMillis(FALSE, &twelveHoursAfter);
    time->SetHour(0);
    time->SetMinute(0);
    Int64 midnightBefore = 0;
    time->ToMillis(FALSE, &midnightBefore);
    Int32 monthDay;
    time->GetMonthDay(&monthDay);
    time->SetMonthDay(monthDay + 1);
    Int64 midnightAfter;
    time->ToMillis(FALSE, &midnightAfter);

    Int64 nowMillis;
    system->GetCurrentTimeMillis(&nowMillis);
    time->Set(nowMillis);
    time->SetSecond(0);
    time->Normalize(FALSE, &nowMillis);

    if((nowMillis >= midnightBefore && nowMillis < midnightAfter)
            || (nowMillis >= twelveHoursBefore && nowMillis < twelveHoursAfter)) {
        display = SHOW_TIME;
    }
    else {
        display = SHOW_MONTH_DAY_YEAR;
    }

    AutoPtr<IDateFormat> format;
    if(display == mLastDisplay && mLastFormat != NULL) {
        format = mLastFormat;
    }
    else {
        switch (display){
            case SHOW_TIME:
                format = GetTimeFormat();
                break;
            case SHOW_MONTH_DAY_YEAR:
                format = GetDateFormat();
                break;
            default:
                return E_RUNTIME_EXCEPTION;
                //throw new RuntimeException("unknown display value: " + display);
        }
       mLastFormat = format;
    }

    assert(format != NULL);
    String text;
    format->FormatDate(mTime, &text);

    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(text, (ICharSequence**)&seq));
    TextView::SetText(seq);

    if (display == SHOW_TIME) {
        mUpdateTimeMillis = twelveHoursAfter > midnightAfter ? twelveHoursAfter: midnightAfter;
    }
    else {
        if (mTimeMillis < nowMillis) {
            mUpdateTimeMillis = 0;
        } else {
            mUpdateTimeMillis = twelveHoursBefore < midnightBefore ? twelveHoursBefore : midnightBefore;
        }
    }
    // Int64 finish;
    // system->GetNanoTime(&finish);
    return NOERROR;
}

AutoPtr<IDateFormat> DateTimeView::GetTimeFormat()
{
    Int32 res;
    AutoPtr<IContext> context = GetContext();
    if (DateFormat::Is24HourFormat(context)) {
        res = R::string::twenty_four_hour_time_format;
    }
    else {
        res = R::string::twelve_hour_time_format;
    }

    String format;
    context->GetString(res, &format);

    AutoPtr<ISimpleDateFormat> sf;
    CSimpleDateFormat::New(format, (ISimpleDateFormat**)&sf);
    AutoPtr<IDateFormat> timeFormat = IDateFormat::Probe(sf.Get());
    return timeFormat;
}

AutoPtr<IDateFormat> DateTimeView::GetDateFormat()
{
    AutoPtr<IContentResolver> resolver;
    AutoPtr<IContext> context = GetContext();
    context->GetContentResolver((IContentResolver**)&resolver);
    String format;
    Settings::System::GetString(resolver, ISettingsSystem::DATE_FORMAT, &format);

    AutoPtr<IDateFormat> df;
    if (format.IsNullOrEmpty()) {
        AutoPtr<IDateFormatHelper> helper;
        CDateFormatHelper::AcquireSingleton((IDateFormatHelper**)&helper);
        helper->GetDateInstance(IDateFormat::SHORT, (IDateFormat**)&df);
    }
    else {
        //try {
            AutoPtr<ISimpleDateFormat> sf;
            CSimpleDateFormat::New(format, (ISimpleDateFormat**)&sf);
            df = IDateFormat::Probe(sf.Get());
       /* } catch (IllegalArgumentException e) {
            return DateFormat.getDateInstance(DateFormat.SHORT);
        }*/
    }
    return df;
}

ECode DateTimeView::RegisterReceivers()
{
    AutoPtr<IContext> context;
    context = GetContext();
    AutoPtr<IIntentFilter> filter;
    ASSERT_SUCCEEDED(CIntentFilter::New((IIntentFilter**)&filter));
    AutoPtr<IIntent> intent;
    ASSERT_SUCCEEDED(CIntent::New((IIntent**)&intent));
    filter->AddAction(intent->ACTION_TIME_TICK);
    filter->AddAction(intent->ACTION_TIME_CHANGED);
    filter->AddAction(intent->ACTION_CONFIGURATION_CHANGED);
    filter->AddAction(intent->ACTION_TIMEZONE_CHANGED);

    AutoPtr<IIntent> registerIntent;
    context->RegisterReceiver(mBroadcastReceiver, filter, (IIntent**)&registerIntent);

    AutoPtr<IUri> uri;
    Settings::System::GetUriFor(ISettingsSystem::DATE_FORMAT, (IUri**)&uri);

    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    resolver->RegisterContentObserver(uri, TRUE, mContentObserver);
    return NOERROR;
}

ECode DateTimeView::UnRegisterReceivers()
{
    AutoPtr<IContext> context;
    context = GetContext();
    context->UnregisterReceiver(mBroadcastReceiver);

    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    resolver->UnregisterContentObserver(mContentObserver);
    return NOERROR;
}

//=====================================================================
//              DateTimeView::DateTimeViewReceiver
//=====================================================================
DateTimeView::DateTimeViewReceiver::DateTimeViewReceiver(
    /* [in] */ DateTimeView* host) :
    mHost(host)
{
}

ECode DateTimeView::DateTimeViewReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (action.Equals(IIntent::ACTION_TIME_TICK)) {
        AutoPtr<ISystem> system;
        Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
        Int64 now;
        system->GetCurrentTimeMillis(&now);
        if (now < mHost->mUpdateTimeMillis) {
            return NOERROR;
        }
    }
    mHost->mLastFormat = NULL;
    mHost->Update();
    return NOERROR;
}

//=====================================================================
//              DateTimeView::DateTimeViewObserver
//=====================================================================
DateTimeView::DateTimeViewObserver::DateTimeViewObserver(
    /* [in] */ DateTimeView* host) :
    mHost(host)
{
}

ECode DateTimeView::DateTimeViewObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    mHost->mLastFormat = NULL;
    mHost->Update();
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
