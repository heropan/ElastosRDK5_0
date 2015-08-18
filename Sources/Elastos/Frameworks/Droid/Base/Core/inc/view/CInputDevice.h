
#ifndef __ELASTOS_DROID_VIEW_CINPUTDEVICE_H__
#define __ELASTOS_DROID_VIEW_CINPUTDEVICE_H__

#include "_Elastos_Droid_View_CInputDevice.h"
#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Utility::Etl::List;
using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Droid::Os::IVibrator;

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CInputDevice)
{
public:
    /**
     * Provides information about the range of values for a particular {@link MotionEvent} axis.
     *
     * @see InputDevice#getMotionRange(int)
     */
    class MotionRange : public ElRefBase, public IMotionRange
    {
        friend class CInputDevice;
    private:
        MotionRange(
            /* [in] */ Int32 axis,
            /* [in] */ Int32 source,
            /* [in] */ Float min,
            /* [in] */ Float max,
            /* [in] */ Float flat,
            /* [in] */ Float fuzz);

    public:
        CAR_INTERFACE_DECL();

        /**
         * Gets the axis id.
         * @return The axis id.
         */
        CARAPI GetAxis(
            /* [out] */ Int32* axis);

        /**
         * Gets the source for which the axis is defined.
         * @return The source.
         */
        CARAPI GetSource(
            /* [out] */ Int32* source);

        /**
         * Gets the minimum value for the coordinate.
         * @return The minimum value.
         */
        CARAPI GetMin(
            /* [out] */ Float* minimum);

        /**
         * Gets the maximum value for the coordinate.
         * @return The minimum value.
         */
        CARAPI GetMax(
            /* [out] */ Float* maximum);

        /**
         * Gets the range of the coordinate (difference between maximum and minimum).
         * @return The range of values.
         */
        CARAPI GetRange(
            /* [out] */ Float* range);

        /**
         * Gets the extent of the center flat position with respect to this coordinate.
         * For example, a flat value of 8 means that the center position is between -8 and +8.
         * This value is mainly useful for calibrating self-centering devices.
         * @return The extent of the center flat position.
         */
        CARAPI GetFlat(
            /* [out] */ Float* flat);

        /**
         * Gets the error tolerance for input device measurements with respect to this coordinate.
         * For example, a value of 2 indicates that the measured value may be up to +/- 2 units
         * away from the actual value due to noise and device sensitivity limitations.
         * @return The error tolerance.
         */
        CARAPI GetFuzz(
            /* [out] */ Float* fuzz);

    private:
        Int32 mAxis;
        Int32 mSource;
        Float mMin;
        Float mMax;
        Float mFlat;
        Float mFuzz;
    };

public:
    /**
     * Gets information about the input device with the specified id.
     * @param id The device id.
     * @return The input device or NULL if not found.
     */
    static CARAPI GetDevice(
        /* [in] */ Int32 id,
        /* [out] */ IInputDevice** device);

    /**
     * Gets the ids of all input devices in the system.
     * @return The input device ids.
     */
    static CARAPI GetDeviceIds(
        /* [out, callee] */ ArrayOf<Int32>** deviceIds);

public:
    CInputDevice();

    ~CInputDevice();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 id,
        /* [in] */ Int32 generation,
        /* [in] */ const String& name,
        /* [in] */ const String& descriptor,
        /* [in] */ Boolean isExternal,
        /* [in] */ Int32 sources,
        /* [in] */ Int32 keyboardType,
        /* [in] */ IKeyCharacterMap* keyCharacterMap,
        /* [in] */ Boolean hasVibrator);

    // CARAPI constructor(
    //     /* [in] */ IParcel* in);

    /**
     * Gets the input device id.
     * <p>
     * Each input device receives a unique id when it is first configured
     * by the system.  The input device id may change when the system is restarted or if the
     * input device is disconnected, reconnected or reconfigured at any time.
     * If you require a stable identifier for a device that persists across
     * boots and reconfigurations, use {@link #getDescriptor()}.
     * </p>
     *
     * @return The input device id.
     */
    CARAPI GetId(
        /* [out] */ Int32* id);

    /**
     * Gets a generation number for this input device.
     * The generation number is incremented whenever the device is reconfigured and its
     * properties may have changed.
     *
     * @return The generation number.
     *
     * @hide
     */
    CARAPI GetGeneration(
        /* [out] */ Int32* generation);

    /**
     * Gets the input device descriptor, which is a stable identifier for an input device.
     * <p>
     * An input device descriptor uniquely identifies an input device.  Its value
     * is intended to be persistent across system restarts, and should not change even
     * if the input device is disconnected, reconnected or reconfigured at any time.
     * </p><p>
     * It is possible for there to be multiple {@link InputDevice} instances that have the
     * same input device descriptor.  This might happen in situations where a single
     * human input device registers multiple {@link InputDevice} instances (HID collections)
     * that describe separate features of the device, such as a keyboard that also
     * has a trackpad.  Alternately, it may be that the input devices are simply
     * indistinguishable, such as two keyboards made by the same manufacturer.
     * </p><p>
     * The input device descriptor returned by {@link #getDescriptor} should only be
     * used when an application needs to remember settings associated with a particular
     * input device.  For all other purposes when referring to a logical
     * {@link InputDevice} instance at runtime use the id returned by {@link #getId()}.
     * </p>
     *
     * @return The input device descriptor.
     */
    CARAPI GetDescriptor(
        /* [out] */ String* descriptor);

    /**
     * Returns true if the device is a virtual input device rather than a real one,
     * such as the virtual keyboard (see {@link KeyCharacterMap#VIRTUAL_KEYBOARD}).
     * <p>
     * Virtual input devices are provided to implement system-level functionality
     * and should not be seen or configured by users.
     * </p>
     *
     * @return True if the device is virtual.
     *
     * @see KeyCharacterMap#VIRTUAL_KEYBOARD
     */
    CARAPI IsVirtual(
        /* [out] */ Boolean* isVirtual);

    /**
     * Returns true if the device is external (connected to USB or Bluetooth or some other
     * peripheral bus), otherwise it is built-in.
     *
     * @return True if the device is external.
     *
     * @hide
     */
    CARAPI IsExternal(
        /* [out] */ Boolean* isExternal);

    /**
     * Returns true if the device is a full keyboard.
     *
     * @return True if the device is a full keyboard.
     *
     * @hide
     */
    CARAPI IsFullKeyboard(
        /* [out] */ Boolean* isFullKeyboard);

    /**
     * Gets the name of this input device.
     * @return The input device name.
     */
    CARAPI GetName(
        /* [out] */ String* name);

    /**
     * Gets the input sources supported by this input device as a combined bitfield.
     * @return The supported input sources.
     */
    CARAPI GetSources(
        /* [out] */ Int32* sources);

    /**
     * Gets the keyboard type.
     * @return The keyboard type.
     */
    CARAPI GetKeyboardType(
        /* [out] */ Int32* type);

    /**
     * Gets the key character map associated with this input device.
     * @return The key character map.
     */
    CARAPI GetKeyCharacterMap(
        /* [out] */ IKeyCharacterMap** keyCharacterMap);

    /**
     * Gets information about the range of values for a particular {@link MotionEvent} axis.
     * If the device supports multiple sources, the same axis may have different meanings
     * for each source.  Returns information about the first axis found for any source.
     * To obtain information about the axis for a specific source, use
     * {@link #getMotionRange(int, int)}.
     *
     * @param axis The axis constant.
     * @return The range of values, or null if the requested axis is not
     * supported by the device.
     *
     * @see MotionEvent#AXIS_X
     * @see MotionEvent#AXIS_Y
     * @see #getSupportedAxes()
     */
    CARAPI GetMotionRange(
        /* [in] */ Int32 axis,
        /* [out] */ IMotionRange** motionRange);

    /**
     * Gets information about the range of values for a particular {@link MotionEvent} axis
     * used by a particular source on the device.
     * If the device supports multiple sources, the same axis may have different meanings
     * for each source.
     *
     * @param axis The axis constant.
     * @param source The source for which to return information.
     * @return The range of values, or null if the requested axis is not
     * supported by the device.
     *
     * @see MotionEvent#AXIS_X
     * @see MotionEvent#AXIS_Y
     * @see #getSupportedAxes()
     */
    CARAPI GetMotionRange(
        /* [in] */ Int32 axis,
        /* [in] */ Int32 source,
        /* [out] */ IMotionRange** motionRange);

    /**
     * Gets the ranges for all axes supported by the device.
     * @return The motion ranges for the device.
     *
     * @see #getMotionRange(int, int)
     */
    CARAPI GetMotionRanges(
        /* [out] */ IObjectContainer** motionRanges);

    /**
     * Gets the vibrator service associated with the device, if there is one.
     * Even if the device does not have a vibrator, the result is never null.
     * Use {@link Vibrator#hasVibrator} to determine whether a vibrator is
     * present.
     *
     * Note that the vibrator associated with the device may be different from
     * the system vibrator.  To obtain an instance of the system vibrator instead, call
     * {@link Context#getSystemService} with {@link Context#VIBRATOR_SERVICE} as argument.
     *
     * @return The vibrator service associated with the device, never null.
     */
    CARAPI GetVibrator(
        /* [out] */ IVibrator** vibrator);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI AddMotionRange(
        /* [in] */ Int32 axis,
        /* [in] */ Int32 source,
        /* [in] */ Float min,
        /* [in] */ Float max,
        /* [in] */ Float flat,
        /* [in] */ Float fuzz);

private:
    CARAPI AppendSourceDescriptionIfApplicable(
        /* [in] */ StringBuilder& description,
        /* [in] */ Int32 source,
        /* [in] */ const String& sourceName);

private:
    Int32 mId;
    Int32 mGeneration;
    String mName;
    String mDescriptor;
    Boolean mIsExternal;
    Int32 mSources;
    Int32 mKeyboardType;
    AutoPtr<IKeyCharacterMap> mKeyCharacterMap;
    Boolean mHasVibrator;
    List<AutoPtr<MotionRange> > mMotionRanges;
    Object mMotionRangesLock;

    AutoPtr<IVibrator> mVibrator; // guarded by mMotionRanges during initialization
};

}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__ELASTOS_DROID_VIEW_CINPUTDEVICE_H__
