
#include "elastos/droid/widget/TextClock.h"

using Elastos::Utility::ICalendarHelper;
using Elastos::Utility::CCalendarHelper;

namespace Elastos{
namespace Droid{
namespace Widget{

static AutoPtr<ICharSequence> InitDEFAULT_FORMAT_12_HOUR()
{
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(String("h:mm aa"), (ICharSequence**)&csq);
    return csq;
}

static AutoPtr<ICharSequence> InitDEFAULT_FORMAT_24_HOUR()
{
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(String("k:mm"), (ICharSequence**)&csq);
    return csq;
}

AutoPtr<ICharSequence> TextClock::DEFAULT_FORMAT_12_HOUR = InitDEFAULT_FORMAT_12_HOUR();
AutoPtr<ICharSequence> TextClock::DEFAULT_FORMAT_24_HOUR = InitDEFAULT_FORMAT_24_HOUR();

TextClock::TextClock()
    : mHasSeconds(FALSE)
    , mAttached(FALSE)
{
    mFormat12 = DEFAULT_FORMAT_12_HOUR;
    mFormat24 = DEFAULT_FORMAT_24_HOUR;
}

/**
 * Creates a new clock using the default patterns
 * {@link #DEFAULT_FORMAT_24_HOUR} and {@link #DEFAULT_FORMAT_12_HOUR}
 * respectively for the 24-hour and 12-hour modes.
 *
 * @param context The Context the view is running in, through which it can
 *        access the current theme, resources, etc.
 */
TextClock::TextClock(
    /* [in] */ IContext* context)
    : TextView(context)
    , mHasSeconds(FALSE)
    , mAttached(FALSE)
{
    mFormat12 = DEFAULT_FORMAT_12_HOUR;
    mFormat24 = DEFAULT_FORMAT_24_HOUR;
    PrivateInit();
}

TextClock::TextClock(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : TextView(context, attrs)
    , mHasSeconds(FALSE)
    , mAttached(FALSE)
{
    mFormat12 = DEFAULT_FORMAT_12_HOUR;
    mFormat24 = DEFAULT_FORMAT_24_HOUR;
    InitFromAttributes(context, attrs, 0);
}

TextClock::TextClock(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    mFormat12 = DEFAULT_FORMAT_12_HOUR;
    mFormat24 = DEFAULT_FORMAT_24_HOUR;
    InitFromAttributes(context, attrs, defStyle);
}

ECode TextClock::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(TextView::InitFromAttributes(context, attrs, defStyle));

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::TextClock),
            ARRAY_SIZE(R::styleable::TextClock));

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, defStyle, 0, (ITypedArray**)&a);
//    try {
    AutoPtr<ICharSequence> format;

    a->GetText(R::styleable::TextClock_format12Hour, (ICharSequence**)&format);
    mFormat12 = format == NULL ? DEFAULT_FORMAT_12_HOUR : format;

    format = NULL;
    a->GetText(R::styleable::TextClock_format24Hour, (ICharSequence**)&format);
    mFormat24 = format == NULL ? DEFAULT_FORMAT_24_HOUR : format;

    a->GetString(R::styleable::TextClock_timeZone, &mTimeZone);
//    } finally {
    a->Recycle();
//    }

    PrivateInit();
    return NOERROR;
}

ECode TextClock::Init(
    /* [in] */ IContext* context)
{
    TextView::Init(context);
    PrivateInit();
    return NOERROR;
}

ECode TextClock::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return InitFromAttributes(context, attrs, 0);
}

ECode TextClock::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return InitFromAttributes(context, attrs, defStyle);
}

void TextClock::PrivateInit()
{
    CreateTime(mTimeZone);
    // Wait until onAttachedToWindow() to handle the ticker
    ChooseFormat(FALSE);
}

void TextClock::CreateTime(
    /* [in] */ const String& timeZone)
{
    mTime = NULL;
    AutoPtr<ICalendarHelper> helper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&helper);
    if (!timeZone.IsNull()) {
        AutoPtr<ITimeZoneHelper> timeZoneHelper;
        CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&timeZoneHelper);

        AutoPtr<ITimeZone> tz;
        timeZoneHelper->GetTimeZone(timeZone, (ITimeZone**)&tz);
        helper->GetInstance(tz, (ICalendar**)&mTime);
    } else {
        helper->GetInstance((ICalendar**)&mTime);
    }
}

AutoPtr<ICharSequence> TextClock::GetFormat12Hour()
{
    return mFormat12;
}

ECode TextClock::SetFormat12Hour(
    /* [in] */ ICharSequence* format)
{
    mFormat12 = format;

    ChooseFormat();
    OnTimeChanged();
}

AutoPtr<ICharSequence> TextClock::GetFormat24Hour()
{
    return mFormat24;
}

ECode TextClock::SetFormat24Hour(
    /* [in] */ ICharSequence* format)
{
    mFormat24 = format;

    ChooseFormat();
    OnTimeChanged();
}

Boolean TextClock::Is24HourModeEnabled()
{
    return DateFormat.Is24HourFormat(getContext());
}

String TextClock::GetTimeZone()
{
    return mTimeZone;
}

ECode TextClock::SetTimeZone(
    /* [in] */ const String& timeZone)
{
    mTimeZone = timeZone;

    CreateTime(timeZone);
    OnTimeChanged();
}

void TextClock::ChooseFormat()
{
    ChooseFormat(TRUE);
}

void TextClock::ChooseFormat(
    /* [in] */ Boolean handleTicker)
{
    Boolean format24Requested = Is24HourModeEnabled();

    if (format24Requested) {
        mFormat = Abc(mFormat24, mFormat12, DEFAULT_FORMAT_24_HOUR);
    } else {
        mFormat = Abc(mFormat12, mFormat24, DEFAULT_FORMAT_12_HOUR);
    }

    Boolean hadSeconds = mHasSeconds;
    mHasSeconds = DateFormat::HasSeconds(mFormat);

    if (handleTicker && mAttached && hadSeconds != mHasSeconds) {
        if (hadSeconds) getHandler().removeCallbacks(mTicker);
        else mTicker.run();
    }
}

/**
 * Returns a if not null, else return b if not null, else return c.
 */
AutoPtr<ICharSequence> TextClock::Abc(
    /* [in] */ ICharSequence* a,
    /* [in] */ ICharSequence* b,
    /* [in] */ ICharSequence* c)
{
    return a == NULL ? (b == NULL ? c : b) : a;
}

ECode TextClock::OnAttachedToWindow()
{
    super.onAttachedToWindow();

    if (!mAttached) {
        mAttached = TRUE;

        RegisterReceiver();
        RegisterObserver();

        CreateTime(mTimeZone);

        if (mHasSeconds) {
            mTicker.run();
        } else {
            OnTimeChanged();
        }
    }
    return NOERROR;
}

ECode TextClock::OnDetachedFromWindow()
{
    super.onDetachedFromWindow();

    if (mAttached) {
        UnregisterReceiver();
        UnregisterObserver();

        getHandler().removeCallbacks(mTicker);

        mAttached = FALSE;
    }
    return NOERROR;
}

void TextClock::RegisterReceiver()
{
    final IntentFilter filter = new IntentFilter();

    filter.addAction(Intent.ACTION_TIME_TICK);
    filter.addAction(Intent.ACTION_TIME_CHANGED);
    filter.addAction(Intent.ACTION_TIMEZONE_CHANGED);

    getContext().registerReceiver(mIntentReceiver, filter, null, getHandler());
}

void TextClock::RegisterObserver()
{
    final ContentResolver resolver = getContext().getContentResolver();
    resolver.registerContentObserver(Settings.System.CONTENT_URI, TRUE, mFormatChangeObserver);
}

void TextClock::UnregisterReceiver()
{
    getContext().unregisterReceiver(mIntentReceiver);
}

void TextClock::UnregisterObserver()
{
    final ContentResolver resolver = getContext().getContentResolver();
    resolver.unregisterContentObserver(mFormatChangeObserver);
}

void TextClock::OnTimeChanged()
{
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 now;
    system->GetCurrentTimeMillis(&now);
    mTime->SetTimeInMillis(now);
    SetText(DateFormat.format(mFormat, mTime));
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos
