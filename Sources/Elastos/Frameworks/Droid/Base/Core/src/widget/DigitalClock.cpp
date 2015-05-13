#include "widget/DigitalClock.h"
#include "text/format/DateFormat.h"
#include "os/CHandler.h"
#include "net/CUriHelper.h"
#include "os/SystemClock.h"

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Text::Format::DateFormat;
using Elastos::Utility::ICalendarHelper;
using Elastos::Utility::CCalendarHelper;

namespace Elastos{
namespace Droid{
namespace Widget{

const String DigitalClock::m12 = String("h:mm:ss aa");
const String DigitalClock::m24 =  String("k:mm:ss");

DigitalClock::DigitalClock()
    : mTickerStopped(FALSE)
{

}

DigitalClock::DigitalClock(
    /* [in] */ IContext* context)
    : mTickerStopped(FALSE)
{
    Init(context);
}

DigitalClock::DigitalClock(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mTickerStopped(FALSE)
{
    Init(context, attrs);
}

ECode DigitalClock::Init(
        /* [in] */ IContext* context)
{
    TextView::Init(context);
    InitClock();
    return NOERROR;
}

ECode DigitalClock::Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs)
{
    TextView::Init(context, attrs);
    InitClock();
    return NOERROR;
}

ECode DigitalClock::OnAttachedToWindow()
{
    mTickerStopped = FALSE;
    TextView::OnAttachedToWindow();
    CHandler::New((IHandler**)&mHandler);
    mTicker = new DigitalClockRunnable(this);
    mTicker->Run();
    return NOERROR;
}

ECode DigitalClock::OnDetachedFromWindow()
{
    TextView::OnDetachedFromWindow();
    mTickerStopped = TRUE;
    return NOERROR;
}

ECode DigitalClock::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    TextView::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(String("DigitalClock"), (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

ECode DigitalClock::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    TextView::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(String("DigitalClock"), (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}

void DigitalClock::InitClock()
{
    if (mCalendar == NULL) {
        AutoPtr<ICalendarHelper> helper;
        CCalendarHelper::AcquireSingleton((ICalendarHelper**)&helper);
        helper->GetInstance((ICalendar**)&mCalendar);
    }

    mFormatChangeObserver = new FormatChangeObserver(this);
    AutoPtr<IContentResolver> resolver;
    GetContext()->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<IUriHelper> uriHelper;
    CUriHelper::AcquireSingleton((IUriHelper**)&uriHelper);
    AutoPtr<IUri> uri;
    uriHelper->Parse(String("content://settings/system"), (IUri**)&uri);
    resolver->RegisterContentObserver(uri, TRUE, mFormatChangeObserver);
    SetFormat();
}

Boolean DigitalClock::Get24HourMode()
{
    return DateFormat::Is24HourFormat(GetContext());
}

void DigitalClock::SetFormat()
{
    if (Get24HourMode()) {
        mFormat = m24;
    } else {
        mFormat = m12;
    }
}

///////////////////////////////////////////////////////////////////////////////
//                     DigitalClock::DigitalClockRunnable
///////////////////////////////////////////////////////////////////////////////

DigitalClock::DigitalClockRunnable::DigitalClockRunnable(
    /* [in] */ DigitalClock* host)
    :mHost(host)
{
}

ECode DigitalClock::DigitalClockRunnable::Run()
{
    if (mHost->mTickerStopped) return NOERROR;

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 now;
    system->GetCurrentTimeMillis(&now);
    mHost->mCalendar->SetTimeInMillis(now);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(mHost->mFormat, (ICharSequence**)&csq);
    mHost->SetText(DateFormat::Format(csq, mHost->mCalendar));
    mHost->Invalidate();
    now = SystemClock::GetUptimeMillis();
    Int64 next = now + (1000 - now % 1000);
    Boolean res;
    mHost->mHandler->PostAtTime(mHost->mTicker, next, &res);
    return NOERROR;
}

///////////////////////////////////////////////////////////////////////////////
//                     DigitalClock::FormatChangeObserver
///////////////////////////////////////////////////////////////////////////////
DigitalClock::FormatChangeObserver::FormatChangeObserver(
    /* [in] */ DigitalClock* host)
    : mHost(host)
{
    AutoPtr<IHandler> handler;
    CHandler::New((IHandler**)&handler);
    ContentObserver::Init(handler);
}

ECode DigitalClock::FormatChangeObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    mHost->SetFormat();
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
