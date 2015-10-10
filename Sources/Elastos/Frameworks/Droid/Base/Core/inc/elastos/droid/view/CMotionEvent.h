
#ifndef __ELASTOS_DROID_VIEW_CMOTIONEVENT_H__
#define __ELASTOS_DROID_VIEW_CMOTIONEVENT_H__

#include "_Elastos_Droid_View_CMotionEvent.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>
#include "view/InputEvent.h"
#include <androidfw/Input.h>


using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Graphics::IMatrix;

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CMotionEvent), public InputEvent
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
    static CARAPI Obtain(
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
        /* [out] */ CMotionEvent** event);

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
    static CARAPI Obtain(
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
        /* [out] */ CMotionEvent** event);

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
    static CARAPI Obtain(
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
        /* [out] */ CMotionEvent** event);

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
    static CARAPI Obtain(
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
        /* [out] */ CMotionEvent** event);

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
    static CARAPI Obtain(
        /* [in] */ Int64 downTime,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 action,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 metaState,
        /* [out] */ CMotionEvent** event);

    /**
     * Create a new MotionEvent, copying from an existing one.
     */
    static CARAPI Obtain(
        /* [in] */ CMotionEvent* o,
        /* [out] */ CMotionEvent** event);

    /**
     * Create a new MotionEvent, copying from an existing one, but not including
     * any historical point information.
     */
    static CARAPI ObtainNoHistory(
        /* [in] */ CMotionEvent* o,
        /* [out] */ CMotionEvent** event);

    /**
     * Returns a string that represents the symbolic name of the specified action
     * such as "ACTION_DOWN", "ACTION_POINTER_DOWN(3)" or an equivalent numeric constant
     * such as "35" if unknown.
     *
     * @param action The action.
     * @return The symbolic name of the specified action.
     * @hide
     */
    static CARAPI_(String) ActionToString(
        /* [in] */ Int32 action);

    /**
     * Returns a string that represents the symbolic name of the specified axis
     * such as "AXIS_X" or an equivalent numeric constant such as "42" if unknown.
     *
     * @param axis The axis
     * @return The symbolic name of the specified axis.
     */
    static CARAPI_(String) AxisToString(
        /* [in] */ Int32 axis);

    /**
     * Gets an axis by its symbolic name such as "AXIS_X" or an
     * equivalent numeric constant such as "42".
     *
     * @param symbolicName The symbolic name of the axis.
     * @return The axis or -1 if not found.
     * @see #keycodeToString
     */
    static CARAPI AxisFromString(
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
    static CARAPI_(String) ButtonStateToString(
        /* [in] */ Int32 buttonState);

    /**
     * Returns a string that represents the symbolic name of the specified tool type
     * such as "TOOL_TYPE_FINGER" or an equivalent numeric constant such as "42" if unknown.
     *
     * @param toolType The tool type.
     * @return The symbolic name of the specified tool type.
     * @hide
     */
    static CARAPI_(String) ToolTypeToString(
        /* [in] */ Int32 toolType);

    static CARAPI CreateFromParcelBody(
        /* [in] */ IParcel* in,
        /* [out] */ CMotionEvent** event);

private:
    static CARAPI_(void) EnsureSharedTempPointerCapacity(
        /* [in] */ Int32 desiredCapacity);

    static CARAPI Obtain(
        /* [out] */ CMotionEvent** event);

    static CARAPI_(Float) Clamp(
        /* [in] */ Float value,
        /* [in] */ Float low,
        /* [in] */ Float high);

public:
    CMotionEvent();

    ~CMotionEvent();

    CARAPI constructor();

    //@Override
    CARAPI Copy(
        /* [out] */ IInputEvent** event);

    //@Override
    CARAPI Recycle();

    //@Override
    CARAPI RecycleIfNeededAfterDispatch();

    CARAPI GetDeviceId(
        /* [out] */ Int32* deviceId);

    CARAPI GetDevice(
        /* [out] */ IInputDevice** device);

    CARAPI GetSource(
        /* [out] */ Int32* source);

    CARAPI SetSource(
        /* [in] */ Int32 source);

    CARAPI GetSequenceNumber(
        /* [out] */ Int32* seq);

    /**
     * Applies a scale factor to all points within this event.
     *
     * This method is used to adjust touch events to simulate different density
     * displays for compatibility mode.  The values returned by {@link #getRawX()},
     * {@link #getRawY()}, {@link #getXPrecision()} and {@link #getYPrecision()}
     * are also affected by the scale factor.
     *
     * @param scale The scale factor to apply.
     * @hide
     */
    CARAPI Scale(
        /* [in] */ Float scale);

    /**
     * Return the kind of action being performed -- one of either
     * {@link #ACTION_DOWN}, {@link #ACTION_MOVE}, {@link #ACTION_UP}, or
     * {@link #ACTION_CANCEL}.  Consider using {@link #getActionMasked}
     * and {@link #getActionIndex} to retrieve the separate masked action
     * and pointer index.
     */
    CARAPI GetAction(
        /* [out] */ Int32* action);

    /**
     * Return the masked action being performed, without pointer index
     * information.  May be any of the actions: {@link #ACTION_DOWN},
     * {@link #ACTION_MOVE}, {@link #ACTION_UP}, {@link #ACTION_CANCEL},
     * {@link #ACTION_POINTER_DOWN}, or {@link #ACTION_POINTER_UP}.
     * Use {@link #getActionIndex} to return the index associated with
     * pointer actions.
     */
    CARAPI GetActionMasked(
        /* [out] */ Int32* actionMasked);

    /**
     * For {@link #ACTION_POINTER_DOWN} or {@link #ACTION_POINTER_UP}
     * as returned by {@link #getActionMasked}, this returns the associated
     * pointer index.  The index may be used with {@link #getPointerId(Int32)},
     * {@link #getX(Int32)}, {@link #getY(Int32)}, {@link #getPressure(Int32)},
     * and {@link #getSize(Int32)} to get information about the pointer that has
     * gone down or up.
     */
    CARAPI GetActionIndex(
        /* [out] */ Int32* actionIndex);

    /**
     * Returns true if this motion event is a touch event.
     * <p>
     * Specifically excludes pointer events with action {@link #ACTION_HOVER_MOVE},
     * {@link #ACTION_HOVER_ENTER}, {@link #ACTION_HOVER_EXIT}, or {@link #ACTION_SCROLL}
     * because they are not actually touch events (the pointer is not down).
     * </p>
     * @return True if this motion event is a touch event.
     * @hide
     */
    CARAPI IsTouchEvent(
        /* [out] */ Boolean* isTouchEvent);

    /**
     * Gets the motion event flags.
     *
     * @see #FLAG_WINDOW_IS_OBSCURED
     */
    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    //@Override
    CARAPI IsTainted(
        /* [out] */ Boolean* isTainted);

    //@Override
    CARAPI SetTainted(
        /* [in] */ Boolean tainted);

    /**
     * Returns the time (in ms) when the user originally pressed down to start
     * a stream of position events.
     */
    CARAPI GetDownTime(
        /* [out] */ Int64* downTime);

    /**
     * Sets the time (in ms) when the user originally pressed down to start
     * a stream of position events.
     *
     * @hide
     */
    CARAPI SetDownTime(
        /* [in] */ Int64 downTime);

    //@Override
    CARAPI GetEventTime(
        /* [out] */ Int64* eventTime);

    //@Override
    CARAPI GetEventTimeNano(
        /* [out] */ Int64* eventTimeNano);

    /**
     * {@link #getX(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetX(
        /* [out] */ Float* x);

    /**
     * {@link #getY(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetY(
        /* [out] */ Float* y);

    /**
     * {@link #getPressure(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetPressure(
        /* [out] */ Float* pressure);

    /**
     * {@link #getSize(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetSize(
        /* [out] */ Float* size);

    /**
     * {@link #getTouchMajor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetTouchMajor(
        /* [out] */ Float* touchMajor);

    /**
     * {@link #getTouchMinor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetTouchMinor(
        /* [out] */ Float* touchMinor);

    /**
     * {@link #getToolMajor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetToolMajor(
        /* [out] */ Float* toolMajor);

    /**
     * {@link #getToolMinor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetToolMinor(
        /* [out] */ Float* toolMinor);

    /**
     * {@link #getOrientation(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetOrientation(
        /* [out] */ Float* orientation);

    /**
     * {@link #getAxisValue(int)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     *
     * @param axis The axis identifier for the axis value to retrieve.
     *
     * @see #AXIS_X
     * @see #AXIS_Y
     */
    CARAPI GetAxisValue(
        /* [in] */ Int32 axis,
        /* [out] */ Float* value);

    /**
     * The number of pointers of data contained in this event.  Always
     * >= 1.
     */
    CARAPI GetPointerCount(
        /* [out] */ Int32* count);

    /**
     * Return the pointer identifier associated with a particular pointer
     * data index is this event.  The identifier tells you the actual pointer
     * number associated with the data, accounting for individual pointers
     * going up and down since the start of the current gesture.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetPointerId(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Int32* pointerId);

    /**
     * Gets the tool type of a pointer for the given pointer index.
     * The tool type indicates the type of tool used to make contact such
     * as a finger or stylus, if known.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @return The tool type of the pointer.
     *
     * @see #TOOL_TYPE_UNKNOWN
     * @see #TOOL_TYPE_FINGER
     * @see #TOOL_TYPE_STYLUS
     * @see #TOOL_TYPE_MOUSE
     * @see #TOOL_TYPE_INDIRECT_FINGER
     * @see #TOOL_TYPE_INDIRECT_STYLUS
     */
    CARAPI GetToolType(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Int32* toolType);

    /**
     * Given a pointer identifier, find the index of its data in the event.
     *
     * @param pointerId The identifier of the pointer to be found.
     * @return Returns either the index of the pointer (for use with
     * {@link #getX(Int32)} et al.), or -1 if there is no data available for
     * that pointer identifier.
     */
    CARAPI FindPointerIndex(
        /* [in] */ Int32 pointerId,
        /* [out] */ Int32* pointerIndex);

    /**
     * Returns the X coordinate of this event for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * Whole numbers are pixels; the
     * value may have a fraction for input devices that are sub-pixel precise.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetX(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* x);

    /**
     * Returns the Y coordinate of this event for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * Whole numbers are pixels; the
     * value may have a fraction for input devices that are sub-pixel precise.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetY(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* y);

    /**
     * Returns the current pressure of this event for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * The pressure generally
     * ranges from 0 (no pressure at all) to 1 (normal pressure), however
     * values higher than 1 may be generated depending on the calibration of
     * the input device.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetPressure(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* pressure);

    /**
     * Returns a scaled value of the approximate size for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * This represents some approximation of the area of the screen being
     * pressed; the actual value in pixels corresponding to the
     * touch is normalized with the device specific range of values
     * and scaled to a value between 0 and 1. The value of size can be used to
     * determine fat touch events.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetSize(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* size);

    /**
     * Returns the length of the major axis of an ellipse that describes the touch
     * area at the point of contact for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetTouchMajor(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* touchMajor);

    /**
     * Returns the length of the minor axis of an ellipse that describes the touch
     * area at the point of contact for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetTouchMinor(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* touchMinor);

    /**
     * Returns the length of the major axis of an ellipse that describes the size of
     * the approaching tool for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * The tool area represents the estimated size of the finger or pen that is
     * touching the device independent of its actual touch area at the point of contact.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetToolMajor(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* toolMajor);

    /**
     * Returns the length of the minor axis of an ellipse that describes the size of
     * the approaching tool for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * The tool area represents the estimated size of the finger or pen that is
     * touching the device independent of its actual touch area at the point of contact.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetToolMinor(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* toolMinor);

    /**
     * Returns the orientation of the touch area and tool area in radians clockwise from vertical
     * for the given pointer <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * An angle of 0 degrees indicates that the major axis of contact is oriented
     * upwards, is perfectly circular or is of unknown orientation.  A positive angle
     * indicates that the major axis of contact is oriented to the right.  A negative angle
     * indicates that the major axis of contact is oriented to the left.
     * The full range is from -PI/2 radians (finger pointing fully left) to PI/2 radians
     * (finger pointing fully right).
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetOrientation(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* orientation);

    /**
     * Returns the value of the requested axis for the given pointer <em>index</em>
     * (use {@link #getPointerId(int)} to find the pointer identifier for this index).
     *
     * @param axis The axis identifier for the axis value to retrieve.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @return The value of the axis, or 0 if the axis is not available.
     *
     * @see #AXIS_X
     * @see #AXIS_Y
     */
    CARAPI GetAxisValue(
        /* [in] */ Int32 axis,
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* value);

    /**
     * Populates a {@link PointerCoords} object with pointer coordinate data for
     * the specified pointer index.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param outPointerCoords The pointer coordinate object to populate.
     */
    CARAPI GetPointerCoords(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ IPointerCoords* outPointerCoords);

    /**
     * Populates a {@link PointerProperties} object with pointer properties for
     * the specified pointer index.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param outPointerProperties The pointer properties object to populate.
     *
     * @see PointerProperties
     */
    CARAPI GetPointerProperties(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ IPointerProperties* outPointerProperties);

    /**
     * Returns the state of any meta / modifier keys that were in effect when
     * the event was generated.  This is the same values as those
     * returned by {@link KeyEvent#getMetaState() KeyEvent.getMetaState}.
     *
     * @return an integer in which each bit set to 1 represents a pressed
     *         meta key
     *
     * @see KeyEvent#getMetaState()
     */
    CARAPI GetMetaState(
        /* [out] */ Int32* metaState);

    /**
     * Gets the state of all buttons that are pressed such as a mouse or stylus button.
     *
     * @return The button state.
     *
     * @see #BUTTON_PRIMARY
     * @see #BUTTON_SECONDARY
     * @see #BUTTON_TERTIARY
     * @see #BUTTON_FORWARD
     * @see #BUTTON_BACK
     */
    CARAPI GetButtonState(
        /* [out] */ Int32* buttonState);

    /**
     * Returns the original raw X coordinate of this event.  For touch
     * events on the screen, this is the original location of the event
     * on the screen, before it had been adjusted for the containing window
     * and views.
     */
    CARAPI GetRawX(
        /* [out] */ Float* rawX);

    /**
     * Returns the original raw Y coordinate of this event.  For touch
     * events on the screen, this is the original location of the event
     * on the screen, before it had been adjusted for the containing window
     * and views.
     */
    CARAPI GetRawY(
        /* [out] */ Float* rawY);

    /**
     * Return the precision of the X coordinates being reported.  You can
     * multiple this number with {@link #getX} to find the actual hardware
     * value of the X coordinate.
     * @return Returns the precision of X coordinates being reported.
     */
    CARAPI GetXPrecision(
        /* [out] */ Float* xPrecision);
    /**
     * Return the precision of the Y coordinates being reported.  You can
     * multiple this number with {@link #getY} to find the actual hardware
     * value of the Y coordinate.
     * @return Returns the precision of Y coordinates being reported.
     */
    CARAPI GetYPrecision(
        /* [out] */ Float* yPrecision);

    /**
     * Returns the number of historical points in this event.  These are
     * movements that have occurred between this event and the previous event.
     * This only applies to ACTION_MOVE events -- all other actions will have
     * a size of 0.
     *
     * @return Returns the number of historical points in the event.
     */
    CARAPI GetHistorySize(
        /* [out] */ Int32* historySize);

    /**
     * Returns the time that a historical movement occurred between this event
     * and the previous event.  Only applies to ACTION_MOVE events.
     *
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getEventTime
     */
    CARAPI GetHistoricalEventTime(
        /* [in] */ Int32 pos,
        /* [out] */ Int64* hEventTime);

    /**
     * Returns the time that a historical movement occurred between this event
     * and the previous event, in the {@link android.os.SystemClock#uptimeMillis} time base
     * but with nanosecond (instead of millisecond) precision.
     * <p>
     * This only applies to ACTION_MOVE events.
     * </p><p>
     * The value is in nanosecond precision but it may not have nanosecond accuracy.
     * </p>
     *
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     * @return Returns the time that a historical movement occurred between this
     * event and the previous event,
     * in the {@link android.os.SystemClock#uptimeMillis} time base but with
     * nanosecond (instead of millisecond) precision.
     *
     * @see #getHistorySize
     * @see #getEventTime
     *
     * @hide
     */
    CARAPI GetHistoricalEventTimeNano(
        /* [in] */ Int32 pos,
        /* [out] */ Int64* eventTimeNano);

    /**
     * {@link #getHistoricalX(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalX(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hX);

    /**
     * {@link #getHistoricalY(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalY(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hY);

    /**
     * {@link #getHistoricalPressure(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalPressure(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hPressure);

    /**
     * {@link #getHistoricalSize(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalSize(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hSize);

    /**
     * {@link #getHistoricalTouchMajor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalTouchMajor(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hTouchMajor);

    /**
     * {@link #getHistoricalTouchMinor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalTouchMinor(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hTouchMinor);

    /**
     * {@link #getHistoricalToolMajor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalToolMajor(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hToolMajor);

    /**
     * {@link #getHistoricalToolMinor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalToolMinor(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hToolMinor);

    /**
     * {@link #getHistoricalOrientation(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalOrientation(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hOrientation);

    /**
     * {@link #getHistoricalAxisValue(int, int, int)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     *
     * @param axis The axis identifier for the axis value to retrieve.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getAxisValue(int)
     * @see #AXIS_X
     * @see #AXIS_Y
     */
    CARAPI GetHistoricalAxisValue(
        /* [in] */ Int32 axis,
        /* [in] */ Int32 pos,
        /* [out] */ Float* value);

    /**
     * Returns a historical X coordinate, as per {@link #getX(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getX
     */
    CARAPI GetHistoricalX(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hX);

    /**
     * Returns a historical Y coordinate, as per {@link #getY(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getY
     */
    CARAPI GetHistoricalY(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hY);

    /**
     * Returns a historical pressure coordinate, as per {@link #getPressure(Int32)},
     * that occurred between this event and the previous event for the given
     * pointer.  Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getPressure
     */
    CARAPI GetHistoricalPressure(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hPressure);

    /**
     * Returns a historical size coordinate, as per {@link #getSize(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getSize
     */
    CARAPI GetHistoricalSize(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hSize);

    /**
     * Returns a historical touch major axis coordinate, as per {@link #getTouchMajor(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getTouchMajor
     */
    CARAPI GetHistoricalTouchMajor(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hTouchMajor);

    /**
     * Returns a historical touch minor axis coordinate, as per {@link #getTouchMinor(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getTouchMinor
     */
    CARAPI GetHistoricalTouchMinor(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hTouchMinor);

    /**
     * Returns a historical tool major axis coordinate, as per {@link #getToolMajor(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getToolMajor
     */
    CARAPI GetHistoricalToolMajor(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hToolMajor);

    /**
     * Returns a historical tool minor axis coordinate, as per {@link #getToolMinor(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getToolMinor
     */
    CARAPI GetHistoricalToolMinor(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hToolMinorEx);

    /**
     * Returns a historical orientation coordinate, as per {@link #getOrientation(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getOrientation
     */
    CARAPI GetHistoricalOrientation(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hOrientation);

    /**
     * Returns the historical value of the requested axis, as per {@link #getAxisValue(int, int)},
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param axis The axis identifier for the axis value to retrieve.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     * @return The value of the axis, or 0 if the axis is not available.
     *
     * @see #AXIS_X
     * @see #AXIS_Y
     */
    CARAPI GetHistoricalAxisValue(
        /* [in] */ Int32 axis,
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* value);

    /**
     * Populates a {@link PointerCoords} object with historical pointer coordinate data,
     * as per {@link #getPointerCoords}, that occurred between this event and the previous
     * event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     * @param outPointerCoords The pointer coordinate object to populate.
     *
     * @see #getHistorySize
     * @see #getPointerCoords
     */
    CARAPI GetHistoricalPointerCoords(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [in] */ IPointerCoords* outPointerCoords);

    /**
     * Returns a bitfield indicating which edges, if any, were touched by this
     * MotionEvent. For touch events, clients can use this to determine if the
     * user's finger was touching the edge of the display.
     *
     * @see #EDGE_LEFT
     * @see #EDGE_TOP
     * @see #EDGE_RIGHT
     * @see #EDGE_BOTTOM
     */
    CARAPI GetEdgeFlags(
        /* [out] */ Int32* edgeFlags);

    /**
     * Sets the bitfield indicating which edges, if any, were touched by this
     * MotionEvent.
     *
     * @see #getEdgeFlags()
     */
    CARAPI SetEdgeFlags(
        /* [in] */ Int32 flags);

    /**
     * Sets this event's action.
     */
    CARAPI SetAction(
        /* [in] */ Int32 action);

    /**
     * Adjust this event's location.
     * @param deltaX Amount to add to the current X coordinate of the event.
     * @param deltaY Amount to add to the current Y coordinate of the event.
     */
    CARAPI OffsetLocation(
        /* [in] */ Float deltaX,
        /* [in] */ Float deltaY);

    /**
     * Set this event's location.  Applies {@link #offsetLocation} with a
     * delta from the current location to the given new location.
     *
     * @param x New absolute X location.
     * @param y New absolute Y location.
     */
    CARAPI SetLocation(
        /* [in] */ Float x,
        /* [in] */ Float y);

    /**
     * Applies a transformation matrix to all of the points in the event.
     *
     * @param matrix The transformation matrix to apply.
     */
    CARAPI Transform(
        /* [in] */ IMatrix* matrix);

    /**
     * Add a new movement to the batch of movements in this event.  The event's
     * current location, position and size is updated to the new values.
     * The current values in the event are added to a list of historical values.
     *
     * Only applies to {@link #ACTION_MOVE} events.
     *
     * @param eventTime The time stamp (in ms) for this data.
     * @param x The new X position.
     * @param y The new Y position.
     * @param pressure The new pressure.
     * @param size The new size.
     * @param metaState Meta key state.
     */
    CARAPI AddBatch(
        /* [in] */ Int64 eventTime,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float pressure,
        /* [in] */ Float size,
        /* [in] */ Int32 metaState);

    /**
     * Add a new movement to the batch of movements in this event.  The event's
     * current location, position and size is updated to the new values.
     * The current values in the event are added to a list of historical values.
     *
     * Only applies to {@link #ACTION_MOVE} events.
     *
     * @param eventTime The time stamp (in ms) for this data.
     * @param pointerCoords The new pointer coordinates.
     * @param metaState Meta key state.
     */
    CARAPI AddBatch(
        /* [in] */ Int64 eventTime,
        /* [in] */ ArrayOf<IPointerCoords*>* pointerCoords,
        /* [in] */ Int32 metaState);

    /**
     * Adds all of the movement samples of the specified event to this one if
     * it is compatible.  To be compatible, the event must have the same device id,
     * source, action, flags, pointer count, pointer properties.
     *
     * Only applies to {@link #ACTION_MOVE} or {@link #ACTION_HOVER_MOVE} events.
     *
     * @param event The event whose movements samples should be added to this one
     * if possible.
     * @return True if batching was performed or FALSE if batching was not possible.
     * @hide
     */
    CARAPI AddBatch(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* res);

    /**
     * Returns TRUE if all points in the motion event are completely within the specified bounds.
     * @hide
     */
    CARAPI IsWithinBoundsNoHistory(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [out] */ Boolean* res);

    /**
     * Returns a new motion events whose points have been clamped to the specified bounds.
     * @hide
     */
    CARAPI ClampNoHistory(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [out] */ IMotionEvent** event);

    /**
     * Gets an integer where each pointer id present in the event is marked as a bit.
     * @hide
     */
    CARAPI GetPointerIdBits(
        /* [out] */ Int32* idBits);

    /**
     * Splits a motion event such that it includes only a subset of pointer ids.
     * @hide
     */
    CARAPI Split(
        /* [in] */ Int32 idBits,
        /* [out] */ IMotionEvent** event);

    //@Override
    CARAPI_(String) ToString();

    //public static const Parcelable.Creator<MotionEvent> CREATOR
    //        = new Parcelable.Creator<MotionEvent>() {
    //    public MotionEvent createFromParcel(Parcel in) {
    //        in.readInt(); // skip token, we already know this is a MotionEvent
    //        return MotionEvent.createFromParcelBody(in);
    //    }

    //    public MotionEvent[] newArray(Int32 size) {
    //        return new MotionEvent[size];
    //    }
    //};

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI GetNative(
        /* [out] */ Handle32* native);

    CARAPI SetNative(
        /* [in] */ Handle32 native);

private:
    static CARAPI_(android::MotionEvent*) NativeInitialize(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 source,
        /* [in] */ Int32 action,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 edgeFlags,
        /* [in] */ Int32 metaState,
        /* [in] */ Int32 buttonState,
        /* [in] */ Float xOffset,
        /* [in] */ Float yOffset,
        /* [in] */ Float xPrecision,
        /* [in] */ Float yPrecision,
        /* [in] */ Int64 downTimeNanos,
        /* [in] */ Int64 eventTimeNanos,
        /* [in] */ Int32 pointerCount,
        /* [in] */ ArrayOf<IPointerProperties*>* pointerIds,
        /* [in] */ ArrayOf<IPointerCoords*>* pointerCoords);

    static CARAPI_(android::MotionEvent*) NativeCopy(
        /* [in] */ android::MotionEvent* destNativePtr,
        /* [in] */ android::MotionEvent* sourceNativePtr,
        /* [in] */ Boolean keepHistory);

    static CARAPI_(void) NativeAddBatch(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Int64 eventTimeNanos,
        /* [in] */ ArrayOf<IPointerCoords*>* pointerCoords,
        /* [in] */ Int32 metaState);

    static CARAPI_(Int32) NativeGetDeviceId(
        /* [in] */ android::MotionEvent* nativePtr);

    static CARAPI_(Int32) NativeGetSource(
        /* [in] */ android::MotionEvent* nativePtr);

    static CARAPI_(void) NativeSetSource(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Int32 source);

    static CARAPI_(Int32) NativeGetAction(
        /* [in] */ android::MotionEvent* nativePtr);

    static CARAPI_(void) NativeSetAction(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Int32 action);

    static CARAPI_(Boolean) NativeIsTouchEvent(
        /* [in] */ android::MotionEvent* nativePtr);

    static CARAPI_(Int32) NativeGetFlags(
        /* [in] */ android::MotionEvent* nativePtr);

    static CARAPI_(void) NativeSetFlags(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Int32 flags);

    static CARAPI_(Int32) NativeGetEdgeFlags(
        /* [in] */ android::MotionEvent* nativePtr);

    static CARAPI_(void) NativeSetEdgeFlags(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Int32 edgeFlags);

    static CARAPI_(Int32) NativeGetMetaState(
        /* [in] */ android::MotionEvent* nativePtr);

    static CARAPI_(Int32) NativeGetButtonState(
        /* [in] */ android::MotionEvent* nativePtr);

    static CARAPI_(void) NativeOffsetLocation(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Float deltaX,
        /* [in] */ Float deltaY);

    static CARAPI_(Float) NativeGetXOffset(
        /* [in] */ android::MotionEvent* nativePtr);

    static CARAPI_(Float) NativeGetYOffset(
        /* [in] */ android::MotionEvent* nativePtr);

    static CARAPI_(Float) NativeGetXPrecision(
        /* [in] */ android::MotionEvent* nativePtr);

    static CARAPI_(Float) NativeGetYPrecision(
        /* [in] */ android::MotionEvent* nativePtr);

    static CARAPI_(Int64) NativeGetDownTimeNanos(
        /* [in] */ android::MotionEvent* nativePtr);

    static CARAPI_(void) NativeSetDownTimeNanos(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Int64 downTime);

    static CARAPI_(Int32) NativeGetPointerCount(
        /* [in] */ android::MotionEvent* nativePtr);

    static CARAPI_(Int32) NativeGetPointerId(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Int32 pointerIndex);

    static CARAPI_(Int32) NativeGetToolType(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Int32 pointerIndex);

    static CARAPI_(Int32) NativeFindPointerIndex(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Int32 pointerId);

    static CARAPI_(Int32) NativeGetHistorySize(
        /* [in] */ android::MotionEvent* nativePtr);

    static CARAPI_(Int64) NativeGetEventTimeNanos(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Int32 historyPos);

    static CARAPI_(Float) NativeGetRawAxisValue(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Int32 axis,
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 historyPos);

    static CARAPI_(Float) NativeGetAxisValue(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Int32 axis,
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 historyPos);

    static CARAPI_(void) NativeGetPointerCoords(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 historyPos,
        /* [in, out] */ IPointerCoords* outPointerCoords);

    static CARAPI_(void) NativeGetPointerProperties(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Int32 pointerIndex,
        /* [in, out] */ IPointerProperties* outPointerProperties);

    static CARAPI_(void) NativeScale(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ Float scale);

    static CARAPI_(void) NativeTransform(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ IMatrix* matrix);

    static CARAPI_(android::MotionEvent*) NativeReadFromParcel(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ IParcel* parcel);

    static CARAPI_(void) NativeWriteToParcel(
        /* [in] */ android::MotionEvent* nativePtr,
        /* [in] */ IParcel* parcel);

private:
    static const Int64 NS_PER_MS = 1000000;

    // Symbolic names of all axes.
    static HashMap<Int32, String> AXIS_SYMBOLIC_NAMES;

    // Symbolic names of all button states in bit order from least significant
    // to most significant.
    static const String BUTTON_SYMBOLIC_NAMES[];

    // Symbolic names of all tool types.
    static HashMap<Int32, String> TOOL_TYPE_SYMBOLIC_NAMES;

    // Private value for history pos that obtains the current sample.
    static const Int32 HISTORY_CURRENT = -0x80000000;

    static const Int32 MAX_RECYCLED = 10;

    static Object gRecyclerLock;
    static Int32 gRecyclerUsed;
    static AutoPtr<CMotionEvent> gRecyclerTop;

    // Shared temporary objects used when translating coordinates supplied by
    // the caller into single element PointerCoords and pointer id arrays.
    static Object gSharedTempLock;
    static AutoPtr<ArrayOf<IPointerCoords*> > gSharedTempPointerCoords;
    static AutoPtr<ArrayOf<IPointerProperties*> > gSharedTempPointerProperties;
    static AutoPtr<ArrayOf<Int32> > gSharedTempPointerIndexMap;

    static const Boolean mIsStaticInited;

    static Boolean InitStatic();

private:
    android::MotionEvent* mNative;

    AutoPtr<CMotionEvent> mNext;
};

}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__ELASTOS_DROID_VIEW_CMOTIONEVENT_H__
