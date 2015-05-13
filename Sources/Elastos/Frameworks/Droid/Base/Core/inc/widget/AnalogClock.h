
#ifndef __ANALOGCLOCK_H__
#define __ANALOGCLOCK_H__

#include <ext/frameworkext.h>
#include "content/BroadcastReceiver.h"
#include "content/CIntentFilter.h"
#include "view/View.h"
#include "text/format/CTime.h"
#include "R.h"

using Elastos::Droid::Text::Format::ITime;
using Elastos::Droid::Text::Format::CTime;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IBroadcastReceiverPendingResult;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::View::View;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Widget {

/**
 * This widget display an analogic clock with two hands for hours and
 * minutes.
 */
class AnalogClock : public Elastos::Droid::View::View
{
private:
    class IntentReceiver
            : public BroadcastReceiver
    {
    public:

        IntentReceiver(
            /* [in] */ AnalogClock* host);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("AnalogClock::IntentReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }

        CARAPI GoAsync(
            /* [out] */ IBroadcastReceiverPendingResult** pendingResult);

        CARAPI PeekService(
            /* [in] */ IContext* myContext,
            /* [in] */ IIntent* service,
            /* [out] */ IBinder** binder);

        CARAPI SetResultCode(
            /* [in] */ Int32 code);

        CARAPI GetResultCode(
            /* [out] */ Int32* code);

        CARAPI SetResultData(
            /* [in] */ const String& data);

        CARAPI GetResultData(
            /* [out] */ String* data);

        CARAPI SetResultExtras(
            /* [in] */ IBundle* extras);

        CARAPI GetResultExtras(
            /* [in] */ Boolean makeMap,
            /* [out] */ IBundle** extras);

        CARAPI SetResult(
            /* [in] */ Int32 code,
            /* [in] */ const String& data,
            /* [in] */ IBundle* extras);

        CARAPI GetAbortBroadcast(
            /* [out] */ Boolean* isAborted);

        CARAPI AbortBroadcast();

        CARAPI ClearAbortBroadcast();

        CARAPI IsOrderedBroadcast(
            /* [out] */ Boolean* isOrdered);

        CARAPI IsInitialStickyBroadcast(
            /* [out] */ Boolean* isInitial);

        CARAPI SetOrderedHint(
            /* [in] */ Boolean isOrdered);

        CARAPI SetPendingResult(
            /* [in] */ IBroadcastReceiverPendingResult* result);

        CARAPI GetPendingResult(
            /* [out] */ IBroadcastReceiverPendingResult** pendingResult);

        CARAPI GetSendingUserId(
            /* [out] */ Int32* userId);

        CARAPI SetDebugUnregister(
            /* [in] */ Boolean debug);

        CARAPI GetDebugUnregister(
            /* [out] */ Boolean* debugUnregister);
    private:
        AnalogClock* mHost;
    };
public:
    AnalogClock();

    AnalogClock(
        /* [in] */ IContext* context);

    AnalogClock(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    AnalogClock(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

protected:

    virtual CARAPI OnAttachedToWindow();

    virtual CARAPI OnDetachedFromWindow();

    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    virtual CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

private:
    CARAPI_(void) OnTimeChanged();

    CARAPI_(void) UpdateContentDescription(
        /* [in] */ ITime* time);

    CARAPI InitImpl(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);
private:
    AutoPtr<IBroadcastReceiver> mIntentReceiver;// = new BroadcastReceiver();

    AutoPtr<ITime> mCalendar;

    AutoPtr<IDrawable> mHourHand;
    AutoPtr<IDrawable> mMinuteHand;
    AutoPtr<IDrawable> mDial;

    Int32 mDialWidth;
    Int32 mDialHeight;

    Boolean mAttached;

    AutoPtr<IHandler> mHandler;// = new Handler();
    Float mMinutes;
    Float mHour;
    Boolean mChanged;

};

}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
