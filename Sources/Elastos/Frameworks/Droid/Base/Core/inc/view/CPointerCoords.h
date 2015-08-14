
#ifndef __ELASTOS_DROID_VIEW_CPOINTERCOORDS_H__
#define __ELASTOS_DROID_VIEW_CPOINTERCOORDS_H__

#include "_CPointerCoords.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CPointerCoords)
{
public:
    CPointerCoords();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IPointerCoords* other);

    /**
     * The X component of the pointer movement.
     *
     * @see MotionEvent#AXIS_X
     */
    CARAPI SetX(
        /* [in] */ Float value);

    CARAPI GetX(
        /* [out] */ Float* value);

    /**
     * The Y component of the pointer movement.
     *
     * @see MotionEvent#AXIS_Y
     */
    CARAPI SetY(
        /* [in] */ Float value);

    CARAPI GetY(
        /* [out] */ Float* value);

    /**
     * A normalized value that describes the pressure applied to the device
     * by a finger or other tool.
     * The pressure generally ranges from 0 (no pressure at all) to 1 (normal pressure),
     * although values higher than 1 may be generated depending on the calibration of
     * the input device.
     *
     * @see MotionEvent#AXIS_PRESSURE
     */
    CARAPI SetPressure(
        /* [in] */ Float value);

    CARAPI GetPressure(
        /* [out] */ Float* value);

    /**
     * A normalized value that describes the approximate size of the pointer touch area
     * in relation to the maximum detectable size of the device.
     * It represents some approximation of the area of the screen being
     * pressed; the actual value in pixels corresponding to the
     * touch is normalized with the device specific range of values
     * and scaled to a value between 0 and 1. The value of size can be used to
     * determine fat touch events.
     *
     * @see MotionEvent#AXIS_SIZE
     */
    CARAPI SetSize(
        /* [in] */ Float value);

    CARAPI GetSize(
        /* [out] */ Float* value);

    /**
     * The length of the major axis of an ellipse that describes the touch area at
     * the point of contact.
     * If the device is a touch screen, the length is reported in pixels, otherwise it is
     * reported in device-specific units.
     *
     * @see MotionEvent#AXIS_TOUCH_MAJOR
     */
    CARAPI SetTouchMajor(
        /* [in] */ Float value);

    CARAPI GetTouchMajor(
        /* [out] */ Float* value);

    /**
     * The length of the minor axis of an ellipse that describes the touch area at
     * the point of contact.
     * If the device is a touch screen, the length is reported in pixels, otherwise it is
     * reported in device-specific units.
     *
     * @see MotionEvent#AXIS_TOUCH_MINOR
     */
    CARAPI SetTouchMinor(
        /* [in] */ Float value);

    CARAPI GetTouchMinor(
        /* [out] */ Float* value);

    /**
     * The length of the major axis of an ellipse that describes the size of
     * the approaching tool.
     * The tool area represents the estimated size of the finger or pen that is
     * touching the device independent of its actual touch area at the point of contact.
     * If the device is a touch screen, the length is reported in pixels, otherwise it is
     * reported in device-specific units.
     *
     * @see MotionEvent#AXIS_TOOL_MAJOR
     */
    CARAPI SetToolMajor(
        /* [in] */ Float value);

    CARAPI GetToolMajor(
        /* [out] */ Float* value);

    /**
     * The length of the minor axis of an ellipse that describes the size of
     * the approaching tool.
     * The tool area represents the estimated size of the finger or pen that is
     * touching the device independent of its actual touch area at the point of contact.
     * If the device is a touch screen, the length is reported in pixels, otherwise it is
     * reported in device-specific units.
     *
     * @see MotionEvent#AXIS_TOOL_MINOR
     */
    CARAPI SetToolMinor(
        /* [in] */ Float value);

    CARAPI GetToolMinor(
        /* [out] */ Float* value);

    /**
     * The orientation of the touch area and tool area in radians clockwise from vertical.
     * An angle of 0 radians indicates that the major axis of contact is oriented
     * upwards, is perfectly circular or is of unknown orientation.  A positive angle
     * indicates that the major axis of contact is oriented to the right.  A negative angle
     * indicates that the major axis of contact is oriented to the left.
     * The full range is from -PI/2 radians (finger pointing fully left) to PI/2 radians
     * (finger pointing fully right).
     *
     * @see MotionEvent#AXIS_ORIENTATION
     */
    CARAPI SetOrientation(
        /* [in] */ Float value);

    CARAPI GetOrientation(
        /* [out] */ Float* value);

    /**
     * Clears the contents of this object.
     * Resets all axes to zero.
     */
    CARAPI Clear();

    /**
     * Copies the contents of another pointer coords object.
     *
     * @param other The pointer coords object to copy.
     */
    CARAPI CopyFrom(
        /* [in] */ IPointerCoords* other);

    /**
     * Gets the value associated with the specified axis.
     *
     * @param axis The axis identifier for the axis value to retrieve.
     * @return The value associated with the axis, or 0 if none.
     *
     * @see MotionEvent#AXIS_X
     * @see MotionEvent#AXIS_Y
     */
    CARAPI GetAxisValue(
        /* [in] */ Int32 axis,
        /* [out] */ Float* value);

    /**
     * Sets the value associated with the specified axis.
     *
     * @param axis The axis identifier for the axis value to assign.
     * @param value The value to set.
     *
     * @see MotionEvent#AXIS_X
     * @see MotionEvent#AXIS_Y
     */
    CARAPI SetAxisValue(
        /* [in] */ Int32 axis,
        /* [in] */ Float value);

public:
    /**
     * The X component of the pointer movement.
     *
     * @see MotionEvent#AXIS_X
     */
    Float mX;

    /**
     * The Y component of the pointer movement.
     *
     * @see MotionEvent#AXIS_Y
     */
    Float mY;

    /**
     * A normalized value that describes the pressure applied to the device
     * by a finger or other tool.
     * The pressure generally ranges from 0 (no pressure at all) to 1 (normal pressure),
     * although values higher than 1 may be generated depending on the calibration of
     * the input device.
     *
     * @see MotionEvent#AXIS_PRESSURE
     */
    Float mPressure;

    /**
     * A normalized value that describes the approximate size of the pointer touch area
     * in relation to the maximum detectable size of the device.
     * It represents some approximation of the area of the screen being
     * pressed; the actual value in pixels corresponding to the
     * touch is normalized with the device specific range of values
     * and scaled to a value between 0 and 1. The value of size can be used to
     * determine fat touch events.
     *
     * @see MotionEvent#AXIS_SIZE
     */
    Float mSize;

    /**
     * The length of the major axis of an ellipse that describes the touch area at
     * the point of contact.
     * If the device is a touch screen, the length is reported in pixels, otherwise it is
     * reported in device-specific units.
     *
     * @see MotionEvent#AXIS_TOUCH_MAJOR
     */
    Float mTouchMajor;

    /**
     * The length of the minor axis of an ellipse that describes the touch area at
     * the point of contact.
     * If the device is a touch screen, the length is reported in pixels, otherwise it is
     * reported in device-specific units.
     *
     * @see MotionEvent#AXIS_TOUCH_MINOR
     */
    Float mTouchMinor;

    /**
     * The length of the major axis of an ellipse that describes the size of
     * the approaching tool.
     * The tool area represents the estimated size of the finger or pen that is
     * touching the device independent of its actual touch area at the point of contact.
     * If the device is a touch screen, the length is reported in pixels, otherwise it is
     * reported in device-specific units.
     *
     * @see MotionEvent#AXIS_TOOL_MAJOR
     */
    Float mToolMajor;

    /**
     * The length of the minor axis of an ellipse that describes the size of
     * the approaching tool.
     * The tool area represents the estimated size of the finger or pen that is
     * touching the device independent of its actual touch area at the point of contact.
     * If the device is a touch screen, the length is reported in pixels, otherwise it is
     * reported in device-specific units.
     *
     * @see MotionEvent#AXIS_TOOL_MINOR
     */
    Float mToolMinor;

    /**
     * The orientation of the touch area and tool area in radians clockwise from vertical.
     * An angle of 0 radians indicates that the major axis of contact is oriented
     * upwards, is perfectly circular or is of unknown orientation.  A positive angle
     * indicates that the major axis of contact is oriented to the right.  A negative angle
     * indicates that the major axis of contact is oriented to the left.
     * The full range is from -PI/2 radians (finger pointing fully left) to PI/2 radians
     * (finger pointing fully right).
     *
     * @see MotionEvent#AXIS_ORIENTATION
     */
    Float mOrientation;

    static const Int32 INITIAL_PACKED_AXIS_VALUES = 8;
    Int64 mPackedAxisBits;
    AutoPtr<ArrayOf<Float> > mPackedAxisValues;
};

}
}
}

#endif // __ELASTOS_DROID_VIEW_CPOINTERCOORDS_H__
