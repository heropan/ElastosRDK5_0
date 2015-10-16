
#include "elastos/droid/widget/AnalogClock.h"
#include <elastos/core/Math.h>
#include "elastos/droid/content/CIntentFilter.h"
#include "elastos/droid/text/format/CDateUtils.h"

using namespace Elastos::Core;
using Elastos::Utility::ITimeZone;
using Elastos::Utility::ITimeZoneHelper;
using Elastos::Utility::CTimeZoneHelper;
using Elastos::Droid::Text::Format::IDateUtils;
using Elastos::Droid::Text::Format::CDateUtils;
using Elastos::Droid::Content::EIID_IBroadcastReceiver;
using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Widget {


AnalogClock::IntentReceiver::IntentReceiver(
    /* [in] */ AnalogClock* host) : mHost(host)
{}

ECode AnalogClock::IntentReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if(action == IIntent::ACTION_TIMEZONE_CHANGED)
    {
        String tz;
        intent->GetStringExtra(String("time-zone"), &tz);
        AutoPtr<ITimeZoneHelper> helper;
        AutoPtr<ITimeZone> itz;

        CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&helper);
        helper->GetTimeZone(tz, (ITimeZone**)&itz);
        String id;
        itz->GetID(&id);
        mHost->mCalendar = NULL;
        CTime::New(id, (ITime**)&(mHost->mCalendar));
    }
    return NOERROR;
}

ECode AnalogClock::IntentReceiver::GoAsync(
    /* [out] */ IPendingResult** pendingResult)
{
    return BroadcastReceiver::GoAsync(pendingResult);
}

ECode AnalogClock::IntentReceiver::PeekService(
    /* [in] */ IContext* myContext,
    /* [in] */ IIntent* service,
    /* [out] */ IBinder** binder)
{
    return BroadcastReceiver::PeekService(myContext, service, binder);
}

ECode AnalogClock::IntentReceiver::SetResultCode(
    /* [in] */ Int32 code)
{
    return BroadcastReceiver::SetResultCode(code);
}

ECode AnalogClock::IntentReceiver::GetResultCode(
    /* [out] */ Int32* code)
{
    return BroadcastReceiver::GetResultCode(code);
}

ECode AnalogClock::IntentReceiver::SetResultData(
    /* [in] */ const String& data)
{
    return BroadcastReceiver::SetResultData(data);
}

ECode AnalogClock::IntentReceiver::GetResultData(
    /* [out] */ String* data)
{
    return BroadcastReceiver::GetResultData(data);
}

ECode AnalogClock::IntentReceiver::SetResultExtras(
    /* [in] */ IBundle* extras)
{
    return BroadcastReceiver::SetResultExtras(extras);
}

ECode AnalogClock::IntentReceiver::GetResultExtras(
    /* [in] */ Boolean makeMap,
    /* [out] */ IBundle** extras)
{
    return BroadcastReceiver::GetResultExtras(makeMap, extras);
}

ECode AnalogClock::IntentReceiver::SetResult(
    /* [in] */ Int32 code,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras)
{
    return BroadcastReceiver::SetResult(code, data, extras);
}

ECode AnalogClock::IntentReceiver::GetAbortBroadcast(
    /* [out] */ Boolean* isAborted)
{
    return BroadcastReceiver::GetAbortBroadcast(isAborted);
}

ECode AnalogClock::IntentReceiver::AbortBroadcast()
{
    return BroadcastReceiver::AbortBroadcast();
}

ECode AnalogClock::IntentReceiver::ClearAbortBroadcast()
{
    return BroadcastReceiver::ClearAbortBroadcast();
}

ECode AnalogClock::IntentReceiver::IsOrderedBroadcast(
    /* [out] */ Boolean* isOrdered)
{
    return BroadcastReceiver::IsOrderedBroadcast(isOrdered);
}

ECode AnalogClock::IntentReceiver::IsInitialStickyBroadcast(
    /* [out] */ Boolean* isInitial)
{
    return BroadcastReceiver::IsInitialStickyBroadcast(isInitial);
}

ECode AnalogClock::IntentReceiver::SetOrderedHint(
    /* [in] */ Boolean isOrdered)
{
    return BroadcastReceiver::SetOrderedHint(isOrdered);
}

ECode AnalogClock::IntentReceiver::SetPendingResult(
    /* [in] */ IPendingResult* result)
{
    return BroadcastReceiver::SetPendingResult(result);
}

ECode AnalogClock::IntentReceiver::GetPendingResult(
    /* [out] */ IPendingResult** pendingResult)
{
    return BroadcastReceiver::GetPendingResult(pendingResult);
}

ECode AnalogClock::IntentReceiver::GetSendingUserId(
    /* [out] */ Int32* userId)
{
    return BroadcastReceiver::GetSendingUserId(userId);
}

ECode AnalogClock::IntentReceiver::SetDebugUnregister(
    /* [in] */ Boolean debug)
{
    return BroadcastReceiver::SetDebugUnregister(debug);
}

ECode AnalogClock::IntentReceiver::GetDebugUnregister(
    /* [out] */ Boolean* debugUnregister)
{
    return BroadcastReceiver::GetDebugUnregister(debugUnregister);
}

AnalogClock::AnalogClock()
    : mDialWidth(0)
    , mDialHeight(0)
    , mAttached(FALSE)
    , mMinutes(0.0f)
    , mHour(0.0f)
    , mChanged(FALSE)
{
    mIntentReceiver = new IntentReceiver(this);
}

AnalogClock::AnalogClock(
    /* [in] */ IContext* context)
    : View(context, NULL)
    , mDialWidth(0)
    , mDialHeight(0)
    , mAttached(FALSE)
    , mMinutes(0.0f)
    , mHour(0.0f)
    , mChanged(FALSE)
{
    Init(context, NULL, 0);
}

AnalogClock::AnalogClock(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : View(context, attrs, 0)
    , mDialWidth(0)
    , mDialHeight(0)
    , mAttached(FALSE)
    , mMinutes(0.0f)
    , mHour(0.0f)
    , mChanged(FALSE)
{
    mIntentReceiver = new IntentReceiver(this);
    Init(context, attrs, 0);
}

AnalogClock::AnalogClock(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : View(context, attrs, defStyle)
    , mDialWidth(0)
    , mDialHeight(0)
    , mAttached(FALSE)
    , mMinutes(0.0f)
    , mHour(0.0f)
    , mChanged(FALSE)
{
    mIntentReceiver = new IntentReceiver(this);
    Init(context, attrs, defStyle);
}


ECode AnalogClock::Init(
    /* [in] */ IContext* context)
{
    return Init(context, NULL);
}

ECode AnalogClock::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs, 0);
}

ECode AnalogClock::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    View::Init(context, attrs, defStyle);
    return InitImpl(context, attrs, defStyle);
}

ECode AnalogClock::InitImpl(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<IResources> r;
    mContext->GetResources((IResources**)&r);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::AnalogClock),
            ARRAY_SIZE(R::styleable::AnalogClock));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(
        attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

    a->GetDrawable(R::styleable::AnalogClock_dial, (IDrawable**)&mDial);
    if (mDial == NULL) {
        r->GetDrawable(R::drawable::clock_dial, (IDrawable**)&mDial);
    }

    a->GetDrawable(R::styleable::AnalogClock_hand_hour, (IDrawable**)&mHourHand);
    if (mHourHand == NULL) {
        r->GetDrawable(R::drawable::clock_hand_hour, (IDrawable**)&mHourHand);
    }

    a->GetDrawable(R::styleable::AnalogClock_hand_minute, (IDrawable**)&mMinuteHand);
    if (mMinuteHand == NULL) {
        r->GetDrawable(R::drawable::clock_hand_minute, (IDrawable**)&mMinuteHand);
    }

    //mCalendar = new Time();
    CTime::New((ITime**)&mCalendar);
    mDial->GetIntrinsicWidth(&mDialWidth);
    mDial->GetIntrinsicHeight(&mDialHeight);
    return NOERROR;
}

ECode AnalogClock::OnAttachedToWindow()
{
    View::OnAttachedToWindow();

    if (!mAttached) {
        mAttached = TRUE;
        AutoPtr<IIntentFilter> filter;
        CIntentFilter::New((IIntentFilter**)&filter);

        filter->AddAction(IIntent::ACTION_TIME_TICK);
        filter->AddAction(IIntent::ACTION_TIME_CHANGED);
        filter->AddAction(IIntent::ACTION_TIMEZONE_CHANGED);
        AutoPtr<IIntent> rst;
        GetContext()->RegisterReceiver(mIntentReceiver, filter, String(NULL), mHandler, (IIntent**)&rst);
    }

    // NOTE: It's safe to do these after registering the receiver since the receiver always runs
    // in the main thread, therefore the receiver can't run before this method returns.

    // The time zone may have changed while the receiver wasn't registered, so update the Time
    if(mCalendar)
        mCalendar = NULL;
    CTime::New((ITime**)&mCalendar);
    // Make sure we update to the current time
    OnTimeChanged();
    return NOERROR;
}

ECode AnalogClock::OnDetachedFromWindow()
{
    View::OnDetachedFromWindow();
    if (mAttached && mIntentReceiver) {
        GetContext()->UnregisterReceiver(mIntentReceiver);
        delete mIntentReceiver;
        mAttached = FALSE;
    }
    return NOERROR;
}

void AnalogClock::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 widthMode = View::MeasureSpec::GetMode(widthMeasureSpec);
    Int32 widthSize =  View::MeasureSpec::GetSize(widthMeasureSpec);
    Int32 heightMode = View::MeasureSpec::GetMode(heightMeasureSpec);
    Int32 heightSize =  View::MeasureSpec::GetSize(heightMeasureSpec);

    Float hScale = 1.0f;
    Float vScale = 1.0f;

    if (widthMode != View::MeasureSpec::UNSPECIFIED && widthSize < mDialWidth) {
        hScale = (Float) widthSize / (Float) mDialWidth;
    }

    if (heightMode != View::MeasureSpec::UNSPECIFIED && heightSize < mDialHeight) {
        vScale = (Float )heightSize / (Float) mDialHeight;
    }

    Float scale = Elastos::Core::Math::Min(hScale, vScale);

    SetMeasuredDimension(ResolveSizeAndState((Int32) (mDialWidth * scale), widthMeasureSpec, 0),
        ResolveSizeAndState((Int32) (mDialHeight * scale), heightMeasureSpec, 0));
}

void AnalogClock::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    View::OnSizeChanged(w, h, oldw, oldh);
    mChanged = TRUE;
}

void AnalogClock::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    View::OnDraw(canvas);

    Boolean changed = mChanged;
    if (changed) {
        mChanged = FALSE;
    }

    Int32 availableWidth = mRight - mLeft;
    Int32 availableHeight = mBottom - mTop;

    Int32 x = availableWidth / 2;
    Int32 y = availableHeight / 2;

    AutoPtr<IDrawable> dial = mDial;
    Int32 w;
    dial->GetIntrinsicWidth(&w);
    Int32 h;
    dial->GetIntrinsicHeight(&h);

    Boolean scaled = FALSE;

    Int32 res;
    if (availableWidth < w || availableHeight < h) {
        scaled = TRUE;
        Float scale = Elastos::Core::Math::Min((Float) availableWidth / (Float) w,
            (Float) availableHeight / (Float) h);
        canvas->Save(&res);
        canvas->Scale(scale, scale, x, y);
    }

    if (changed) {
        dial->SetBounds(x - (w / 2), y - (h / 2), x + (w / 2), y + (h / 2));
    }
    dial->Draw(canvas);

    canvas->Save(&res);
    canvas->Rotate(mHour / 12.0f * 360.0f, x, y);
    AutoPtr<IDrawable> hourHand = mHourHand;
    if (changed) {
        hourHand->GetIntrinsicWidth(&w);
        hourHand->GetIntrinsicHeight(&h);
        hourHand->SetBounds(x - (w / 2), y - (h / 2), x + (w / 2), y + (h / 2));
    }
    hourHand->Draw(canvas);
    canvas->Restore();

    canvas->Save(&res);
    canvas->Rotate(mMinutes / 60.0f * 360.0f, x, y);

    AutoPtr<IDrawable> minuteHand = mMinuteHand;
    if (changed) {
        minuteHand->GetIntrinsicWidth(&w);
        minuteHand->GetIntrinsicHeight(&h);
        minuteHand->SetBounds(x - (w / 2), y - (h / 2), x + (w / 2), y + (h / 2));
    }
    minuteHand->Draw(canvas);
    canvas->Restore();

    if (scaled) {
        canvas->Restore();
    }
}

void AnalogClock::OnTimeChanged()
{
    mCalendar->SetToNow();

    Int32 hour = ((CTime*)mCalendar.Get())->mHour;
    Int32 minute = ((CTime*)mCalendar.Get())->mMinute;
    Int32 second = ((CTime*)mCalendar.Get())->mSecond;

    mMinutes = minute + second / 60.0f;
    mHour = hour + mMinutes / 60.0f;
    mChanged = TRUE;

    UpdateContentDescription(mCalendar);
}

void AnalogClock::UpdateContentDescription(
    /* [in] */ ITime* time)
{
    AutoPtr<IDateUtils> utils;
    CDateUtils::AcquireSingleton((IDateUtils**)&utils);
    Int32 flags = IDateUtils::FORMAT_SHOW_TIME | IDateUtils::FORMAT_24HOUR;
    String contentDescription;
    Int64 millis;
    time->ToMillis(FALSE, &millis);
    utils->FormatDateTime(mContext,
            millis, flags, &contentDescription);
    AutoPtr<ICharSequence> text;
    CStringWrapper::New(contentDescription, (ICharSequence**)&text);
    SetContentDescription(text);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
