#ifndef __ELASTOS_DROID_VIEW_INPUTEVENT_H__
#define __ELASTOS_DROID_VIEW_INPUTEVENT_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace View {

class InputEvent
{
protected:
    InputEvent();

public:
    /**
     * Gets the id for the device that this event came from.  An id of
     * zero indicates that the event didn't come from a physical device
     * and maps to the default keymap.  The other numbers are arbitrary and
     * you shouldn't depend on the values.
     *
     * @return The device id.
     * @see InputDevice#getDevice
     */
    virtual CARAPI GetDeviceId(
        /* [out] */ Int32* deviceId) = 0;

    /**
     * Gets the device that this event came from.
     *
     * @return The device, or null if unknown.
     */
    CARAPI GetDevice(
        /* [out] */ IInputDevice** device);

    /**
     * Gets the source of the event.
     *
     * @return The event source or {@link InputDevice#SOURCE_UNKNOWN} if unknown.
     * @see InputDevice#getSourceInfo
     */
    virtual CARAPI GetSource(
        /* [out] */ Int32* source) = 0;

    /**
     * Modifies the source of the event.
     * @param source The source.
     *
     * @hide
     */
    virtual CARAPI SetSource(
        /* [in] */ Int32 source) = 0;


    /**
     * Copies the event.
     *
     * @return A deep copy of the event.
     * @hide
     */
    virtual CARAPI Copy(
        /* [out] */ IInputEvent** event) = 0;

    /**
     * Recycles the event.
     * This method should only be used by the system since applications do not
     * expect {@link KeyEvent} objects to be recycled, although {@link MotionEvent}
     * objects are fine.  See {@link KeyEvent#recycle()} for details.
     * @hide
     */
    virtual CARAPI Recycle();

    /**
     * Conditionally recycled the event if it is appropriate to do so after
     * dispatching the event to an application.
     *
     * If the event is a {@link MotionEvent} then it is recycled.
     *
     * If the event is a {@link KeyEvent} then it is NOT recycled, because applications
     * expect key events to be immutable so once the event has been dispatched to
     * the application we can no longer recycle it.
     * @hide
     */
    virtual CARAPI RecycleIfNeededAfterDispatch();

    /**
     * Gets a private flag that indicates when the system has detected that this input event
     * may be inconsistent with respect to the sequence of previously delivered input events,
     * such as when a key up event is sent but the key was not down or when a pointer
     * move event is sent but the pointer is not down.
     *
     * @return True if this event is tainted.
     * @hide
     */
    virtual CARAPI IsTainted(
        /* [out] */ Boolean* isTainted) = 0;

    /**
     * Sets a private flag that indicates when the system has detected that this input event
     * may be inconsistent with respect to the sequence of previously delivered input events,
     * such as when a key up event is sent but the key was not down or when a pointer
     * move event is sent but the pointer is not down.
     *
     * @param tainted True if this event is tainted.
     * @hide
     */
    virtual CARAPI SetTainted(
        /* [in] */ Boolean tainted) = 0;

    /**
     * Retrieve the time this event occurred,
     * in the {@link android.os.SystemClock#uptimeMillis} time base.
     *
     * @return Returns the time this event occurred,
     * in the {@link android.os.SystemClock#uptimeMillis} time base.
     */
    virtual CARAPI GetEventTime(
        /* [out] */ Int64* eventTime) = 0;

    /**
     * Retrieve the time this event occurred,
     * in the {@link android.os.SystemClock#uptimeMillis} time base but with
     * nanosecond (instead of millisecond) precision.
     * <p>
     * The value is in nanosecond precision but it may not have nanosecond accuracy.
     * </p>
     *
     * @return Returns the time this event occurred,
     * in the {@link android.os.SystemClock#uptimeMillis} time base but with
     * nanosecond (instead of millisecond) precision.
     *
     * @hide
     */
    virtual CARAPI GetEventTimeNano(
        /* [out] */ Int64* eventTimeNano) = 0;

    /**
     * Gets the unique sequence number of this event.
     * Every input event that is created or received by a process has a
     * unique sequence number.  Moreover, a new sequence number is obtained
     * each time an event object is recycled.
     *
     * Sequence numbers are only guaranteed to be locally unique within a process.
     * Sequence numbers are not preserved when events are parceled.
     *
     * @return The unique sequence number of this event.
     * @hide
     */
    virtual CARAPI GetSequenceNumber(
        /* [out] */ Int32* seq);

protected:
    virtual CARAPI_(void) PrepareForReuse();

    //public static final Parcelable.Creator<InputEvent> CREATOR
    //    = new Parcelable.Creator<InputEvent>() {
    //        public InputEvent createFromParcel(Parcel in) {
    //            Int32 token = in.readInt();
    //            if (token == PARCEL_TOKEN_KEY_EVENT) {
    //                return KeyEvent.createFromParcelBody(in);
    //            } else if (token == PARCEL_TOKEN_MOTION_EVENT) {
    //                return MotionEvent.createFromParcelBody(in);
    //            } else {
    //                throw new IllegalStateException("Unexpected input event type token in parcel.");
    //            }
    //        }

    //        public InputEvent[] newArray(Int32 size) {
    //            return new InputEvent[size];
    //        }
    //};

protected:
    /** @hide */
    static const Int32 PARCEL_TOKEN_MOTION_EVENT = 1;
    /** @hide */
    static const Int32 PARCEL_TOKEN_KEY_EVENT = 2;

    Int32 mSeq;

    Boolean mRecycled;

private:
    static Int32 sNextSeq;
    static Object sNextSeqLock;
    static const Boolean TRACK_RECYCLED_LOCATION = FALSE;
};

}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__ELASTOS_DROID_VIEW_INPUTEVENT_H__
