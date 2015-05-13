
#ifndef __DISPLAYEVENTRECEIVER_H__
#define __DISPLAYEVENTRECEIVER_H__

#include "view/NativeDisplayEventReceiver.h"

using Elastos::Droid::Os::IMessageQueue;
using Elastos::Droid::Os::ILooper;

namespace Elastos {
namespace Droid {
namespace View {

/**
 * Provides a low-level mechanism for an application to receive display events
 * such as vertical sync.
 *
 * The display event receive is NOT thread safe.  Moreover, its methods must only
 * be called on the Looper thread to which it is attached.
 *
 * @hide
 */
class DisplayEventReceiver : public ElRefBase
{
    friend class NativeDisplayEventReceiver;
public:
    /**
     * Creates a display event receiver.
     *
     * @param looper The looper to use when invoking callbacks.
     */
    DisplayEventReceiver(
        /* [in] */ ILooper* looper);


    virtual ~DisplayEventReceiver();

    /**
     * Disposes the receiver.
     */
    virtual CARAPI_(void) Dispose();

    /**
     * Called when a vertical sync pulse is received.
     * The recipient should render a frame and then call {@link #scheduleVsync}
     * to schedule the next vertical sync pulse.
     *
     * @param timestampNanos The timestamp of the pulse, in the {@link System#nanoTime()}
     * timebase.
     * @param builtInDisplayId The surface flinger built-in display id such as
     * {@link Surface#BUILT_IN_DISPLAY_ID_MAIN}.
     * @param frame The frame number.  Increases by one for each vertical sync interval.
     */
    virtual CARAPI_(void) OnVsync(
        /* [in] */ Int64 timestampNanos,
        /* [in] */ Int32 builtInDisplayId,
        /* [in] */ Int32 frame);

    /**
     * Called when a display hotplug event is received.
     *
     * @param timestampNanos The timestamp of the event, in the {@link System#nanoTime()}
     * timebase.
     * @param builtInDisplayId The surface flinger built-in display id such as
     * {@link Surface#BUILT_IN_DISPLAY_ID_HDMI}.
     * @param connected True if the display is connected, false if it disconnected.
     */
    virtual CARAPI_(void) OnHotplug(
        /* [in] */ Int64 timestampNanos,
        /* [in] */ Int32 builtInDisplayId,
        /* [in] */ Boolean connected);

    /**
     * Schedules a single vertical sync pulse to be delivered when the next
     * display frame begins.
     */
    virtual CARAPI ScheduleVsync();

private:
    CARAPI_(void) NativeInit();

    CARAPI_(void) Dispose(
        /* [in] */ Boolean finalized);

    // Called from native code.
    //@SuppressWarnings("unused")
    CARAPI_(void) DispatchVsync(
        /* [in] */ Int64 timestampNanos,
        /* [in] */ Int32 builtInDisplayId,
        /* [in] */ Int32 frame);

    // Called from native code.
    //@SuppressWarnings("unused")
    CARAPI_(void) DispatchHotplug(
        /* [in] */ Int64 timestampNanos,
        /* [in] */ Int32 builtInDisplayId,
        /* [in] */ Boolean connected);

private:
    static const char* TAG;

    //final CloseGuard mCloseGuard = CloseGuard.get();

    android::sp<NativeDisplayEventReceiver> mNativeReceiver;

    // We keep a reference message queue object here so that it is not
    // GC'd while the native peer of the receiver is using them.
    AutoPtr<IMessageQueue> mMessageQueue;
};

}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__DISPLAYEVENTRECEIVER_H__
