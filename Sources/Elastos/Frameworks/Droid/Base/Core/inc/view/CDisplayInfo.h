
#ifndef __CDISPLAYINFO_H__
#define __CDISPLAYINFO_H__

#include "_CDisplayInfo.h"

using Elastos::Droid::Utility::IDisplayMetrics;

namespace Elastos {
namespace Droid {
namespace View {

/**
 * Describes the characteristics of a particular logical display.
 * @hide
 */
CarClass(CDisplayInfo)
{
public:
    CDisplayInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IDisplayInfo* other);

    CARAPI GetLayerStack(
        /* [out] */ Int32* layerStack);

    CARAPI SetLayerStack(
        /* [in] */ Int32 layerStack);

    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI GetType(
        /* [out] */ Int32* type);

    CARAPI SetType(
        /* [in] */ Int32 type);

    CARAPI GetAddress(
        /* [out] */ String* address);

    CARAPI SetAddress(
        /* [in] */ const String& address);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI SetName(
        /* [in] */ const String& name);

    CARAPI GetAppWidth(
        /* [out] */ Int32* appWidth);

    CARAPI SetAppWidth(
        /* [in] */ Int32 appWidth);

    CARAPI GetAppHeight(
        /* [out] */ Int32* appHeight);

    CARAPI SetAppHeight(
        /* [in] */ Int32 appHeight);

    CARAPI GetSmallestNominalAppWidth(
        /* [out] */ Int32* smallestNominalAppWidth);

    CARAPI SetSmallestNominalAppWidth(
        /* [in] */ Int32 smallestNominalAppWidth);

    CARAPI GetSmallestNominalAppHeight(
        /* [out] */ Int32* smallestNominalAppHeight);

    CARAPI SetSmallestNominalAppHeight(
        /* [in] */ Int32 smallestNominalAppHeight);

    CARAPI GetLargestNominalAppWidth(
        /* [out] */ Int32* largestNominalAppWidth);

    CARAPI SetLargestNominalAppWidth(
        /* [in] */ Int32 largestNominalAppWidth);

    CARAPI GetLargestNominalAppHeight(
        /* [out] */ Int32* largestNominalAppHeight);

    CARAPI SetLargestNominalAppHeight(
        /* [in] */ Int32 largestNominalAppHeight);

    CARAPI GetLogicalWidth(
        /* [out] */ Int32* logicalWidth);

    CARAPI SetLogicalWidth(
        /* [in] */ Int32 logicalWidth);

    CARAPI GetLogicalHeight(
        /* [out] */ Int32* logicalHeight);

    CARAPI SetLogicalHeight(
        /* [in] */ Int32 logicalHeight);

    CARAPI GetRotation(
        /* [out] */ Int32* rotation);

    CARAPI SetRotation(
        /* [in] */ Int32 rotation);

    CARAPI GetRefreshRate(
        /* [out] */ Float* refreshRate);

    CARAPI SetRefreshRate(
        /* [in] */ Float refreshRate);

    CARAPI GetLogicalDensityDpi(
        /* [out] */ Int32* logicalDensityDpi);

    CARAPI SetLogicalDensityDpi(
        /* [in] */ Int32 logicalDensityDpi);

    CARAPI GetPhysicalXDpi(
        /* [out] */ Float* physicalXDpi);

    CARAPI SetPhysicalXDpi(
        /* [in] */ Float physicalXDpi);

    CARAPI GetPhysicalYDpi(
        /* [out] */ Float* physicalYDpi);

    CARAPI SetPhysicalYDpi(
        /* [in] */ Float physicalYDpi);

    CARAPI Equals(
        /* [in] */ IDisplayInfo* otherObj,
        /* [out] */ Boolean* isEquals);

    CARAPI Equals(
        /* [in] */ IInterface* otherObj,
        /* [out] */ Boolean* isEquals);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);

    CARAPI CopyFrom(
        /* [in] */ IDisplayInfo* otherObj);

    CARAPI GetAppMetrics(
        /* [in] */ IDisplayMetrics* outMetrics,
        /* [in] */ ICompatibilityInfoHolder* cih);

    CARAPI GetLogicalMetrics(
        /* [in] */ IDisplayMetrics* outMetrics,
        /* [in] */ ICompatibilityInfoHolder* cih);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetNaturalWidth(
        /* [out] */ Int32* width);

    CARAPI GetNaturalHeight(
        /* [out] */ Int32* height);

private:
    CARAPI GetMetricsWithSize(
        /* [in] */ IDisplayMetrics* outMetricsObj,
        /* [in] */ ICompatibilityInfoHolder* cih,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

public:
    /**
     * The surface flinger layer stack associated with this logical display.
     */
    Int32 mLayerStack;

    /**
     * Display flags.
     */
    Int32 mFlags;

    /**
     * Display type.
     */
    Int32 mType;

    /**
     * Display address, or NULL if none.
     * Interpretation varies by display type.
     */
    String mAddress;

    /**
     * The human-readable name of the display.
     */
    String mName;

    /**
     * The width of the portion of the display that is available to applications, in pixels.
     * Represents the size of the display minus any system decorations.
     */
    Int32 mAppWidth;

    /**
     * The height of the portion of the display that is available to applications, in pixels.
     * Represents the size of the display minus any system decorations.
     */
    Int32 mAppHeight;

    /**
     * The smallest value of {@link #appWidth} that an application is likely to encounter,
     * in pixels, excepting cases where the width may be even smaller due to the presence
     * of a soft keyboard, for example.
     */
    Int32 mSmallestNominalAppWidth;

    /**
     * The smallest value of {@link #appHeight} that an application is likely to encounter,
     * in pixels, excepting cases where the height may be even smaller due to the presence
     * of a soft keyboard, for example.
     */
    Int32 mSmallestNominalAppHeight;

    /**
     * The largest value of {@link #appWidth} that an application is likely to encounter,
     * in pixels, excepting cases where the width may be even larger due to system decorations
     * such as the status bar being hidden, for example.
     */
    Int32 mLargestNominalAppWidth;

    /**
     * The largest value of {@link #appHeight} that an application is likely to encounter,
     * in pixels, excepting cases where the height may be even larger due to system decorations
     * such as the status bar being hidden, for example.
     */
    Int32 mLargestNominalAppHeight;

    /**
     * The logical width of the display, in pixels.
     * Represents the usable size of the display which may be smaller than the
     * physical size when the system is emulating a smaller display.
     */
    Int32 mLogicalWidth;

    /**
     * The logical height of the display, in pixels.
     * Represents the usable size of the display which may be smaller than the
     * physical size when the system is emulating a smaller display.
     */
    Int32 mLogicalHeight;

    /**
     * The rotation of the display relative to its natural orientation.
     * May be one of {@link android.view.Surface#ROTATION_0},
     * {@link android.view.Surface#ROTATION_90}, {@link android.view.Surface#ROTATION_180},
     * {@link android.view.Surface#ROTATION_270}.
     * <p>
     * The value of this field is indeterminate if the logical display is presented on
     * more than one physical display.
     * </p>
     */
    Int32 mRotation;

    /**
     * The refresh rate of this display in frames per second.
     * <p>
     * The value of this field is indeterminate if the logical display is presented on
     * more than one physical display.
     * </p>
     */
    Float mRefreshRate;

    /**
     * The logical display density which is the basis for density-independent
     * pixels.
     */
    Int32 mLogicalDensityDpi;

    /**
     * The exact physical pixels per inch of the screen in the X dimension.
     * <p>
     * The value of this field is indeterminate if the logical display is presented on
     * more than one physical display.
     * </p>
     */
    Float mPhysicalXDpi;

    /**
     * The exact physical pixels per inch of the screen in the Y dimension.
     * <p>
     * The value of this field is indeterminate if the logical display is presented on
     * more than one physical display.
     * </p>
     */
    Float mPhysicalYDpi;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__CDISPLAYINFO_H__
