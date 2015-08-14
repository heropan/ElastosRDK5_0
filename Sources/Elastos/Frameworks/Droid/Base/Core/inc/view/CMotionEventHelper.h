
#ifndef __ELASTOS_DROID_VIEW_CMOTIONEVENTHELPER_H__
#define __ELASTOS_DROID_VIEW_CMOTIONEVENTHELPER_H__

#include "_CMotionEventHelper.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CMotionEventHelper)
{
public:
    /**
     * Create a new MotionEvent, filling in all of the basic values that
     * define the motion.
     *
     * @param downTime The time (in ms) when the user originally pressed down to start
     * a stream of position events.  This must be obtained from {@link SystemClock#uptimeMillis()}.
     * @param eventTime The the time (in ms) when this specific event was generated.  This
     * must be obtained from {@link SystemClock#uptimeMillis()}.
     * @param action The kind of action being performed, such as {@link #ACTION_DOWN}.
     * @param pointerCount The number of pointers that will be in this event.
     * @param pointerProperties An array of <em>pointerCount</em> values providing
     * a {@link PointerProperties} property object for each pointer, which must
     * include the pointer identifier.
     * @param pointerCoords An array of <em>pointerCount</em> values providing
     * a {@link PointerCoords} coordinate object for each pointer.
     * @param metaState The state of any meta / modifier keys that were in effect when
     * the event was generated.
     * @param buttonState The state of buttons that are pressed.
     * @param xPrecision The precision of the X coordinate being reported.
     * @param yPrecision The precision of the Y coordinate being reported.
     * @param deviceId The id for the device that this event came from.  An id of
     * zero indicates that the event didn't come from a physical device; other
     * numbers are arbitrary and you shouldn't depend on the values.
     * @param edgeFlags A bitfield indicating which edges, if any, were touched by this
     * MotionEvent.
     * @param source The source of this event.
     * @param flags The motion event flags.
     */
    CARAPI Obtain(
        /* [in] */ Int64 downTime,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 action,
        /* [in] */ Int32 pointerCount,
        /* [in] */ ArrayOf<IPointerProperties*>* pointerProperties,
        /* [in] */ ArrayOf<IPointerCoords*>* pointerCoords,
        /* [in] */ Int32 metaState,
        /* [in] */ Int32 buttonState,
        /* [in] */ Float xPrecision,
        /* [in] */ Float yPrecision,
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 edgeFlags,
        /* [in] */ Int32 source,
        /* [in] */ Int32 flags,
        /* [out] */ IMotionEvent** event);

    /**
     * Create a new MotionEvent, filling in all of the basic values that
     * define the motion.
     *
     * @param downTime The time (in ms) when the user originally pressed down to start
     * a stream of position events.  This must be obtained from {@link SystemClock#uptimeMillis()}.
     * @param eventTime The the time (in ms) when this specific event was generated.  This
     * must be obtained from {@link SystemClock#uptimeMillis()}.
     * @param action The kind of action being performed, such as {@link #ACTION_DOWN}.
     * @param pointerCount The number of pointers that will be in this event.
     * @param pointerIds An array of <em>pointerCount</em> values providing
     * an identifier for each pointer.
     * @param pointerCoords An array of <em>pointerCount</em> values providing
     * a {@link PointerCoords} coordinate object for each pointer.
     * @param metaState The state of any meta / modifier keys that were in effect when
     * the event was generated.
     * @param xPrecision The precision of the X coordinate being reported.
     * @param yPrecision The precision of the Y coordinate being reported.
     * @param deviceId The id for the device that this event came from.  An id of
     * zero indicates that the event didn't come from a physical device; other
     * numbers are arbitrary and you shouldn't depend on the values.
     * @param edgeFlags A bitfield indicating which edges, if any, were touched by this
     * MotionEvent.
     * @param source The source of this event.
     * @param flags The motion event flags.
     *
     * @deprecated Use {@link #obtain(long, long, int, int, PointerProperties[], PointerCoords[], int, int, float, float, int, int, int, int)}
     * instead.
     */
    //@Deprecated
    CARAPI Obtain(
        /* [in] */ Int64 downTime,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 action,
        /* [in] */ Int32 pointerCount,
        /* [in] */ ArrayOf<Int32>* pointerIds,
        /* [in] */ ArrayOf<IPointerCoords*>* pointerCoords,
        /* [in] */ Int32 metaState,
        /* [in] */ Float xPrecision,
        /* [in] */ Float yPrecision,
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 edgeFlags,
        /* [in] */ Int32 source,
        /* [in] */ Int32 flags,
        /* [out] */ IMotionEvent** event);

    /**
     * Create a new MotionEvent, filling in all of the basic values that
     * define the motion.
     *
     * @param downTime The time (in ms) when the user originally pressed down to start
     * a stream of position events.  This must be obtained from {@link SystemClock#uptimeMillis()}.
     * @param eventTime  The the time (in ms) when this specific event was generated.  This
     * must be obtained from {@link SystemClock#uptimeMillis()}.
     * @param action The kind of action being performed, such as {@link #ACTION_DOWN}.
     * @param x The X coordinate of this event.
     * @param y The Y coordinate of this event.
     * @param pressure The current pressure of this event.  The pressure generally
     * ranges from 0 (no pressure at all) to 1 (normal pressure), however
     * values higher than 1 may be generated depending on the calibration of
     * the input device.
     * @param size A scaled value of the approximate size of the area being pressed when
     * touched with the finger. The actual value in pixels corresponding to the finger
     * touch is normalized with a device specific range of values
     * and scaled to a value between 0 and 1.
     * @param metaState The state of any meta / modifier keys that were in effect when
     * the event was generated.
     * @param xPrecision The precision of the X coordinate being reported.
     * @param yPrecision The precision of the Y coordinate being reported.
     * @param deviceId The id for the device that this event came from.  An id of
     * zero indicates that the event didn't come from a physical device; other
     * numbers are arbitrary and you shouldn't depend on the values.
     * @param edgeFlags A bitfield indicating which edges, if any, were touched by this
     * MotionEvent.
     */
    CARAPI Obtain(
        /* [in] */ Int64 downTime,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 action,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float pressure,
        /* [in] */ Float size,
        /* [in] */ Int32 metaState,
        /* [in] */ Float xPrecision,
        /* [in] */ Float yPrecision,
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 edgeFlags,
        /* [out] */ IMotionEvent** event);

    /**
     * Create a new MotionEvent, filling in all of the basic values that
     * define the motion.
     *
     * @param downTime The time (in ms) when the user originally pressed down to start
     * a stream of position events.  This must be obtained from {@link SystemClock#uptimeMillis()}.
     * @param eventTime  The the time (in ms) when this specific event was generated.  This
     * must be obtained from {@link SystemClock#uptimeMillis()}.
     * @param action The kind of action being performed, such as {@link #ACTION_DOWN}.
     * @param pointerCount The number of pointers that are active in this event.
     * @param x The X coordinate of this event.
     * @param y The Y coordinate of this event.
     * @param pressure The current pressure of this event.  The pressure generally
     * ranges from 0 (no pressure at all) to 1 (normal pressure), however
     * values higher than 1 may be generated depending on the calibration of
     * the input device.
     * @param size A scaled value of the approximate size of the area being pressed when
     * touched with the finger. The actual value in pixels corresponding to the finger
     * touch is normalized with a device specific range of values
     * and scaled to a value between 0 and 1.
     * @param metaState The state of any meta / modifier keys that were in effect when
     * the event was generated.
     * @param xPrecision The precision of the X coordinate being reported.
     * @param yPrecision The precision of the Y coordinate being reported.
     * @param deviceId The id for the device that this event came from.  An id of
     * zero indicates that the event didn't come from a physical device; other
     * numbers are arbitrary and you shouldn't depend on the values.
     * @param edgeFlags A bitfield indicating which edges, if any, were touched by this
     * MotionEvent.
     *
     * @deprecated Use {@link #obtain(long, long, int, float, float, float, float, int, float, float, int, int)}
     * instead.
     */
    //@Deprecated
    CARAPI Obtain(
        /* [in] */ Int64 downTime,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 action,
        /* [in] */ Int32 pointerCount,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float pressure,
        /* [in] */ Float size,
        /* [in] */ Int32 metaState,
        /* [in] */ Float xPrecision,
        /* [in] */ Float yPrecision,
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 edgeFlags,
        /* [out] */ IMotionEvent** event);

    /**
     * Create a new MotionEvent, filling in a subset of the basic motion
     * values.  Those not specified here are: device id (always 0), pressure
     * and size (always 1), x and y precision (always 1), and edgeFlags (always 0).
     *
     * @param downTime The time (in ms) when the user originally pressed down to start
     * a stream of position events.  This must be obtained from {@link SystemClock#uptimeMillis()}.
     * @param eventTime  The the time (in ms) when this specific event was generated.  This
     * must be obtained from {@link SystemClock#uptimeMillis()}.
     * @param action The kind of action being performed, such as {@link #ACTION_DOWN}.
     * @param x The X coordinate of this event.
     * @param y The Y coordinate of this event.
     * @param metaState The state of any meta / modifier keys that were in effect when
     * the event was generated.
     */
    CARAPI Obtain(
        /* [in] */ Int64 downTime,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 action,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 metaState,
        /* [out] */ IMotionEvent** event);

    /**
     * Create a new MotionEvent, copying from an existing one.
     */
    CARAPI Obtain(
        /* [in] */ IMotionEvent* o,
        /* [out] */ IMotionEvent** event);

    /**
     * Create a new MotionEvent, copying from an existing one, but not including
     * any historical point information.
     */
    CARAPI ObtainNoHistory(
        /* [in] */ IMotionEvent* o,
        /* [out] */ IMotionEvent** event);

    /**
     * Returns a string that represents the symbolic name of the specified action
     * such as "ACTION_DOWN", "ACTION_POINTER_DOWN(3)" or an equivalent numeric constant
     * such as "35" if unknown.
     *
     * @param action The action.
     * @return The symbolic name of the specified action.
     * @hide
     */
    CARAPI ActionToString(
        /* [in] */ Int32 action,
        /* [out] */ String* str);

    /**
     * Returns a string that represents the symbolic name of the specified axis
     * such as "AXIS_X" or an equivalent numeric constant such as "42" if unknown.
     *
     * @param axis The axis
     * @return The symbolic name of the specified axis.
     */
    CARAPI AxisToString(
        /* [in] */ Int32 axis,
        /* [out] */ String* str);

    /**
     * Gets an axis by its symbolic name such as "AXIS_X" or an
     * equivalent numeric constant such as "42".
     *
     * @param symbolicName The symbolic name of the axis.
     * @return The axis or -1 if not found.
     * @see #keycodeToString
     */
    CARAPI AxisFromString(
        /* [in] */ const String& symbolicName,
        /* [out] */ Int32* axis);

    /**
     * Returns a string that represents the symbolic name of the specified combined
     * button state flags such as "0", "BUTTON_PRIMARY",
     * "BUTTON_PRIMARY|BUTTON_SECONDARY" or an equivalent numeric constant such as "0x10000000"
     * if unknown.
     *
     * @param buttonState The button state.
     * @return The symbolic name of the specified combined button state flags.
     * @hide
     */
    CARAPI ButtonStateToString(
        /* [in] */ Int32 buttonState,
        /* [out] */ String* str);

    /**
     * Returns a string that represents the symbolic name of the specified tool type
     * such as "TOOL_TYPE_FINGER" or an equivalent numeric constant such as "42" if unknown.
     *
     * @param toolType The tool type.
     * @return The symbolic name of the specified tool type.
     * @hide
     */
    CARAPI ToolTypeToString(
        /* [in] */ Int32 toolType,
        /* [out] */ String* str);

    CARAPI CreateFromParcelBody(
        /* [in] */ IParcel* in,
        /* [out] */ IMotionEvent** event);
};

}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__ELASTOS_DROID_VIEW_CMOTIONEVENTHELPER_H__
