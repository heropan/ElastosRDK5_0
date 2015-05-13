
#ifndef __CHRONOMETER_H__
#define __CHRONOMETER_H__

#include "widget/TextView.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::Os::IHandler;

namespace Elastos {
namespace Droid {
namespace Widget {

/**
 * Class that implements a simple timer.
 * <p>
 * You can give it a start time in the {@link SystemClock#elapsedRealtime} timebase,
 * and it counts up from that, or if you don't give it a base time, it will use the
 * time at which you call {@link #start}.  By default it will display the current
 * timer value in the form "MM:SS" or "H:MM:SS", or you can use {@link #setFormat}
 * to format the timer value into an arbitrary string.
 *
 * @attr ref android.R.styleable#Chronometer_format
 */
class Chronometer : public TextView
{
private:
    class MyHandler : public HandlerBase
    {
    public:
        MyHandler(
            /* [in] */ Chronometer* host)
            : mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        Chronometer* mHost;
    };

public:
    Chronometer();
    ~Chronometer();

    /**
     * Initialize with standard view layout information and style.
     * Sets the base to the current time.
     */
    Chronometer(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0);

    /**
     * Set the time that the count-up timer is in reference to.
     *
     * @param base Use the {@link SystemClock#elapsedRealtime} time base.
     */
    virtual CARAPI SetBase(
        /* [in] */ Int64 base);

    /**
     * Return the base time as set through {@link #setBase}.
     */
    virtual CARAPI_(Int64) GetBase();

    /**
     * Sets the format string used for display.  The Chronometer will display
     * this string, with the first "%s" replaced by the current timer value in
     * "MM:SS" or "H:MM:SS" form.
     *
     * If the format string is null, or if you never call setFormat(), the
     * Chronometer will simply display the timer value in "MM:SS" or "H:MM:SS"
     * form.
     *
     * @param format the format string.
     */
    virtual CARAPI SetFormat(
        /* [in] */ const String& format);

    /**
     * Returns the current format string as set through {@link #setFormat}.
     */
    virtual CARAPI_(String) GetFormat();

    /**
     * Sets the listener to be called when the chronometer changes.
     *
     * @param listener The listener.
     */
    virtual CARAPI SetOnChronometerTickListener(
        /* [in] */ IOnChronometerTickListener* listener);

    /**
     * @return The listener (may be null) that is listening for chronometer change
     *         events.
     */
    virtual CARAPI_(AutoPtr<IOnChronometerTickListener>) GetOnChronometerTickListener();

    /**
     * Start counting up.  This does not affect the base as set from {@link #setBase}, just
     * the view display.
     *
     * Chronometer works by regularly scheduling messages to the handler, even when the
     * Widget is not visible.  To make sure resource leaks do not occur, the user should
     * make sure that each start() call has a reciprocal call to {@link #stop}.
     */
    virtual CARAPI Start();

    /**
     * Stop counting up.  This does not affect the base as set from {@link #setBase}, just
     * the view display.
     *
     * This stops the messages to the handler, effectively releasing resources that would
     * be held as the chronometer is running, via {@link #start}.
     */
    virtual CARAPI Stop();

    /**
     * The same as calling {@link #start} or {@link #stop}.
     * @hide pending API council approval
     */
    virtual CARAPI SetStarted(
        /* [in] */ Boolean started);

    virtual CARAPI_(void) DispatchChronometerTick();

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info) ;

protected:
    virtual CARAPI OnDetachedFromWindow();


    virtual CARAPI_(void) OnWindowVisibilityChanged(
        /* [in] */ Int32 visibility);

protected:

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:
    CARAPI_(void) Init();

    CARAPI_(void) UpdateText(Int64 now);

    CARAPI_(void) UpdateRunning();

    CARAPI InitInternal(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);
private:
    static const String TAG;

    Int64 mBase;
    Boolean mVisible;
    Boolean mStarted;
    Boolean mRunning;
    Boolean mLogged;
    String mFormat;
    //Formatter mFormatter;
    AutoPtr<ILocale> mFormatterLocale;
    AutoPtr<ArrayOf<IInterface*> > mFormatterArgs;// = new Object[1];
    AutoPtr<StringBuilder> mFormatBuilder;
    AutoPtr<IOnChronometerTickListener> mOnChronometerTickListener;
    StringBuilder mRecycle;// = new StringBuilder(8);

    static const Int32 TICK_WHAT = 2;

    AutoPtr<IHandler> mHandler;// = new Handler();
};

}// namespace Elastos
}// namespace Droid
}// namespace Widget

#endif
