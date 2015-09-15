
#ifndef __ELASTOS_DROID_CONTENT_RES_CCONFIGURATION_H__
#define __ELASTOS_DROID_CONTENT_RES_CCONFIGURATION_H__

#include "_Elastos_Droid_Content_Res_CConfiguration.h"
#include <elastos/core/Object.h>

using Libcore::ICU::ILocale;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

/**
 * This class describes all device configuration information that can
 * impact the resources the application retrieves.  This includes both
 * user-specified configuration options (locale and scaling) as well
 * as device configurations (such as input modes, screen size and screen orientation).
 * <p>You can acquire this object from {@link Resources}, using {@link
 * Resources#getConfiguration}. Thus, from an activity, you can get it by chaining the request
 * with {@link android.app.Activity#getResources}:</p>
 * <pre>Configuration config = getResources().getConfiguration();</pre>
 */
CarClass(CConfiguration)
    , public Object
    , public IConfiguration
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CConfiguration();

    /** @hide */
    static CARAPI_(Int32) ResetScreenLayout(
        /* [in] */ Int32 curLayout);

    /** @hide */
    static CARAPI_(Int32) ReduceScreenLayout(
        /* [in] */ Int32 curLayout,
        /* [in] */ Int32 longSizeDp,
        /* [in] */ Int32 shortSizeDp);

    /**
     * Check if the Configuration's current {@link #screenLayout} is at
     * least the given size.
     *
     * @param size The desired size, either {@link #SCREENLAYOUT_SIZE_SMALL},
     * {@link #SCREENLAYOUT_SIZE_NORMAL}, {@link #SCREENLAYOUT_SIZE_LARGE}, or
     * {@link #SCREENLAYOUT_SIZE_XLARGE}.
     * @return Returns true if the current screen layout size is at least
     * the given size.
     */
    CARAPI IsLayoutSizeAtLeast(
        /* [in] */ Int32 size,
        /* [out] */ Boolean* value);

    CARAPI SetTo(
        /* [in] */ IConfiguration* o);

    /**
     * Set this object to the system defaults.
     */
    CARAPI SetToDefaults();

    CARAPI MakeDefault();

    /**
     * Copy the fields from delta into this Configuration object, keeping
     * track of which ones have changed.  Any undefined fields in
     * <var>delta</var> are ignored and not copied in to the current
     * Configuration.
     * @return Returns a bit mask of the changed fields, as per
     * {@link #diff}.
     */
    CARAPI UpdateFrom(
        /* [in] */ IConfiguration* delta,
        /* [out] */ Int32* changes);

    /**
     * Return a bit mask of the differences between this Configuration
     * object and the given one.  Does not change the values of either.  Any
     * undefined fields in <var>delta</var> are ignored.
     * @return Returns a bit mask indicating which configuration
     * values has changed, containing any combination of
     * {@link android.content.pm.ActivityInfo#CONFIG_FONT_SCALE
     * PackageManager.ActivityInfo.CONFIG_FONT_SCALE},
     * {@link android.content.pm.ActivityInfo#CONFIG_MCC
     * PackageManager.ActivityInfo.CONFIG_MCC},
     * {@link android.content.pm.ActivityInfo#CONFIG_MNC
     * PackageManager.ActivityInfo.CONFIG_MNC},
     * {@link android.content.pm.ActivityInfo#CONFIG_LOCALE
     * PackageManager.ActivityInfo.CONFIG_LOCALE},
     * {@link android.content.pm.ActivityInfo#CONFIG_TOUCHSCREEN
     * PackageManager.ActivityInfo.CONFIG_TOUCHSCREEN},
     * {@link android.content.pm.ActivityInfo#CONFIG_KEYBOARD
     * PackageManager.ActivityInfo.CONFIG_KEYBOARD},
     * {@link android.content.pm.ActivityInfo#CONFIG_NAVIGATION
     * PackageManager.ActivityInfo.CONFIG_NAVIGATION},
     * {@link android.content.pm.ActivityInfo#CONFIG_ORIENTATION
     * PackageManager.ActivityInfo.CONFIG_ORIENTATION},
     * {@link android.content.pm.ActivityInfo#CONFIG_SCREEN_LAYOUT
     * PackageManager.ActivityInfo.CONFIG_SCREEN_LAYOUT}, or
     * {@link android.content.pm.ActivityInfo#CONFIG_SCREEN_SIZE
     * PackageManager.ActivityInfo.CONFIG_SCREEN_SIZE}, or
     * {@link android.content.pm.ActivityInfo#CONFIG_SMALLEST_SCREEN_SIZE
     * PackageManager.ActivityInfo.CONFIG_SMALLEST_SCREEN_SIZE}.
     * {@link android.content.pm.ActivityInfo#CONFIG_LAYOUT_DIRECTION
     * PackageManager.ActivityInfo.CONFIG_LAYOUT_DIRECTION}.
     */
    CARAPI Diff(
        /* [in] */ IConfiguration* delta,
        /* [out] */ Int32* result);

    /**
     * Determine if a new resource needs to be loaded from the bit set of
     * configuration changes returned by {@link #updateFrom(Configuration)}.
     *
     * @param configChanges The mask of changes configurations as returned by
     * {@link #updateFrom(Configuration)}.
     * @param interestingChanges The configuration changes that the resource
     * can handled, as given in {@link android.util.TypedValue#changingConfigurations}.
     *
     * @return Return true if the resource needs to be loaded, else false.
     */
    static CARAPI_(Boolean) NeedNewResources(
        /* [in] */ Int32 configChanges,
        /* [in] */ Int32 interestingChanges);

    /**
     * @hide Return true if the sequence of 'other' is better than this.  Assumes
     * that 'this' is your current sequence and 'other' is a new one you have
     * received some how and want to compare with what you have.
     */
    CARAPI IsOtherSeqNewer(
        /* [in] */ IConfiguration* other,
        /* [out] */ Boolean* isBetter);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI CompareTo(
        /* [in] */ IInterface* that,
        /* [out] */ Int32*  result);

    CARAPI Equals(
        /* [in] */ IInterface* that,
        /* [out] */ Boolean* result);

    CARAPI Equals(
        /* [in] */ IConfiguration* that,
        /* [out] */ Boolean* result);

    CARAPI ToString(
        /* [out] */ String* result);

    /**
     * Set the locale. This is the preferred way for setting up the locale (instead of using the
     * direct accessor). This will also set the userLocale and layout direction according to
     * the locale.
     *
     * @param loc The locale. Can be null.
     */
    CARAPI SetLocale(
        /* [in] */ ILocale* locale);

    CARAPI GetFontScale(
        /* [out] */ Float* fontScale);

    /**
     * Return the layout direction. Will be either {@link View#LAYOUT_DIRECTION_LTR} or
     * {@link View#LAYOUT_DIRECTION_RTL}.
     *
     * @return the layout direction
     */
    CARAPI GetLayoutDirection(
        /* [out] */ Int32* dir);

    /**
     * Set the layout direction from the Locale.
     *
     * @param locale The Locale. If null will set the layout direction to
     * {@link View#LAYOUT_DIRECTION_LTR}. If not null will set it to the layout direction
     * corresponding to the Locale.
     *
     * @see {@link View#LAYOUT_DIRECTION_LTR} and {@link View#LAYOUT_DIRECTION_RTL}
     */
    CARAPI SetLayoutDirection(
        /* [in] */ ILocale* locale);

    CARAPI SetFontScale(
        /* [in] */ Float fontScale);

    CARAPI GetMcc(
        /* [out] */ Int32* mcc);

    CARAPI SetMcc(
        /* [in] */ Int32 mcc);

    CARAPI GetMnc(
        /* [out] */ Int32* mnc);

    CARAPI SetMnc(
        /* [in] */ Int32 mnc);

    CARAPI GetLocale(
        /* [out] */ ILocale** locale);

    CARAPI IsUserSetLocale(
        /* [out] */ Boolean* userSetLocale);

    CARAPI SetUserSetLocale(
        /* [in] */ Boolean userSetLocale);

    CARAPI GetScreenLayout(
        /* [out] */ Int32* layout);

    CARAPI SetScreenLayout(
        /* [in] */ Int32 layout);

    CARAPI GetTouchscreen(
        /* [out] */ Int32* touchscreen);

    CARAPI SetTouchscreen(
        /* [in] */ Int32 touchscreen);

    CARAPI GetKeyboard(
        /* [out] */ Int32* keyboard);

    CARAPI SetKeyboard(
        /* [in] */ Int32 keyboard);

    CARAPI GetKeyboardHidden(
        /* [out] */ Int32* keyboardHidden);

    CARAPI SetKeyboardHidden(
        /* [in] */ Int32 keyboardHidden);

    CARAPI GetHardKeyboardHidden(
        /* [out] */ Int32* hardKeyboardHidden);

    CARAPI SetHardKeyboardHidden(
        /* [in] */ Int32 hardKeyboardHidden);

    CARAPI GetNavigation(
        /* [out] */ Int32* navigation);

    CARAPI SetNavigation(
        /* [in] */ Int32 navigation);

    CARAPI GetNavigationHidden(
        /* [out] */ Int32* navigationHidden);

    CARAPI SetNavigationHidden(
        /* [in] */ Int32 navigationHidden);

    CARAPI GetOrientation(
        /* [out] */ Int32* orientation);

    CARAPI SetOrientation(
        /* [in] */ Int32 orientation);

    CARAPI GetUiMode(
        /* [out] */ Int32* uiMode);

    CARAPI SetUiMode(
        /* [in] */ Int32 uiMode);

    CARAPI GetScreenWidthDp(
        /* [out] */ Int32* screenWidthDp);

    CARAPI SetScreenWidthDp(
        /* [in] */ Int32 screenWidthDp);

    CARAPI GetScreenHeightDp(
        /* [out] */ Int32* screenHeightDp);

    CARAPI SetScreenHeightDp(
        /* [in] */ Int32 screenHeightDp);

    CARAPI GetSmallestScreenWidthDp(
        /* [out] */ Int32* smallestScreenWidthDp);

    CARAPI SetSmallestScreenWidthDp(
        /* [in] */ Int32 smallestScreenWidthDp);

    CARAPI GetDensityDpi(
        /* [out] */ Int32* densityDpi);

    CARAPI SetDensityDpi(
        /* [in] */ Int32 densityDpi);

    CARAPI GetSeq(
        /* [out] */ Int32* seq);

    CARAPI SetSeq(
        /* [in] */ Int32 seq);

    CARAPI GetCompatScreenWidthDp(
        /* [out] */ Int32* dp);

    CARAPI SetCompatScreenWidthDp(
        /* [in] */ Int32 dp);

    CARAPI GetCompatScreenHeightDp(
        /* [out] */ Int32* dp);

    CARAPI SetCompatScreenHeightDp(
        /* [in] */ Int32 dp);

    CARAPI GetCompatSmallestScreenWidthDp(
        /* [out] */ Int32* dp);

    CARAPI SetCompatSmallestScreenWidthDp(
        /* [in] */ Int32 dp);

    /**
     * Construct an invalid Configuration.  You must call {@link #setToDefaults}
     * for this object to be valid.  {@more}
     */
    CARAPI constructor();

    /**
     * Makes a deep copy suitable for modification.
     */
    CARAPI constructor(
        /* [in] */ IConfiguration* config);


private:
    static CARAPI_(Int32) GetScreenLayoutNoDirection(
        /* [in] */Int32 screenLayout);

public:
    /** @hide */
    static const AutoPtr<IConfiguration> EMPTY;

    /**
     * Current user preference for the scaling factor for fonts, relative
     * to the base density scaling.
     */
    Float mFontScale;

    /**
     * IMSI MCC (Mobile Country Code), corresponding to
     * <a href="{@docRoot}guide/topics/resources/providing-resources.html#MccQualifier">mcc</a>
     * resource qualifier.  0 if undefined.
     */
    Int32 mMcc;

    /**
     * IMSI MNC (Mobile Network Code), corresponding to
     * <a href="{@docRoot}guide/topics/resources/providing-resources.html#MccQualifier">mnc</a>
     * resource qualifier.  0 if undefined.
     */
    Int32 mMnc;

    /**
     * Current user preference for the locale, corresponding to
     * <a href="{@docRoot}guide/topics/resources/providing-resources.html#LocaleQualifier">locale</a>
     * resource qualifier.
     */
    AutoPtr<ILocale> mLocale;

    /**
     * Locale should persist on setting.  This is hidden because it is really
     * questionable whether this is the right way to expose the functionality.
     * @hide
     */
    Boolean mUserSetLocale;

    /**
     * Bit mask of overall layout of the screen.  Currently there are two
     * fields:
     * <p>The {@link #SCREENLAYOUT_SIZE_MASK} bits define the overall size
     * of the screen.  They may be one of
     * {@link #SCREENLAYOUT_SIZE_SMALL}, {@link #SCREENLAYOUT_SIZE_NORMAL},
     * {@link #SCREENLAYOUT_SIZE_LARGE}, or {@link #SCREENLAYOUT_SIZE_XLARGE}.
     *
     * <p>The {@link #SCREENLAYOUT_LONG_MASK} defines whether the screen
     * is wider/taller than normal.  They may be one of
     * {@link #SCREENLAYOUT_LONG_NO} or {@link #SCREENLAYOUT_LONG_YES}.
     *
     * <p>The {@link #SCREENLAYOUT_LAYOUTDIR_MASK} defines whether the screen layout
     * is either LTR or RTL.  They may be one of
     * {@link #SCREENLAYOUT_LAYOUTDIR_LTR} or {@link #SCREENLAYOUT_LAYOUTDIR_RTL}.
     *
     * <p>See <a href="{@docRoot}guide/practices/screens_support.html">Supporting
     * Multiple Screens</a> for more information.
     */
    Int32 mScreenLayout;

    /**
     * The kind of touch screen attached to the device.
     * One of: {@link #TOUCHSCREEN_NOTOUCH}, {@link #TOUCHSCREEN_FINGER}.
     */
    Int32 mTouchscreen;

    /**
     * The kind of keyboard attached to the device.
     * One of: {@link #KEYBOARD_NOKEYS}, {@link #KEYBOARD_QWERTY},
     * {@link #KEYBOARD_12KEY}.
     */
    Int32 mKeyboard;

    /**
     * A flag indicating whether any keyboard is available.  Unlike
     * {@link #hardKeyboardHidden}, this also takes into account a soft
     * keyboard, so if the hard keyboard is hidden but there is soft
     * keyboard available, it will be set to NO.  Value is one of:
     * {@link #KEYBOARDHIDDEN_NO}, {@link #KEYBOARDHIDDEN_YES}.
     */
    Int32 mKeyboardHidden;

    /**
     * A flag indicating whether the hard keyboard has been hidden.  This will
     * be set on a device with a mechanism to hide the keyboard from the
     * user, when that mechanism is closed.  One of:
     * {@link #HARDKEYBOARDHIDDEN_NO}, {@link #HARDKEYBOARDHIDDEN_YES}.
     */
    Int32 mHardKeyboardHidden;

    /**
     * The kind of navigation method available on the device.
     * One of: {@link #NAVIGATION_NONAV}, {@link #NAVIGATION_DPAD},
     * {@link #NAVIGATION_TRACKBALL}, {@link #NAVIGATION_WHEEL}.
     */
    Int32 mNavigation;

    /**
     * A flag indicating whether any 5-way or DPAD navigation available.
     * This will be set on a device with a mechanism to hide the navigation
     * controls from the user, when that mechanism is closed.  One of:
     * {@link #NAVIGATIONHIDDEN_NO}, {@link #NAVIGATIONHIDDEN_YES}.
     */
    Int32 mNavigationHidden;

    /**
     * Overall orientation of the screen.  May be one of
     * {@link #ORIENTATION_LANDSCAPE}, {@link #ORIENTATION_PORTRAIT}.
     */
    Int32 mOrientation;

    /**
     * Bit mask of the ui mode.  Currently there are two fields:
     * <p>The {@link #UI_MODE_TYPE_MASK} bits define the overall ui mode of the
     * device. They may be one of {@link #UI_MODE_TYPE_UNDEFINED},
     * {@link #UI_MODE_TYPE_NORMAL}, {@link #UI_MODE_TYPE_DESK},
     * {@link #UI_MODE_TYPE_CAR}, {@link #UI_MODE_TYPE_TELEVISION}, or
     * {@link #UI_MODE_TYPE_APPLIANCE}.
     *
     * <p>The {@link #UI_MODE_NIGHT_MASK} defines whether the screen
     * is in a special mode. They may be one of {@link #UI_MODE_NIGHT_UNDEFINED},
     * {@link #UI_MODE_NIGHT_NO} or {@link #UI_MODE_NIGHT_YES}.
     */
    Int32 mUiMode;

    /**
     * The current width of the available screen space, in dp units,
     * corresponding to
     * <a href="{@docRoot}guide/topics/resources/providing-resources.html#ScreenWidthQualifier">screen
     * width</a> resource qualifier.  Set to
     * {@link #SCREEN_WIDTH_DP_UNDEFINED} if no width is specified.
     */
    Int32 mScreenWidthDp;

    /**
     * The current height of the available screen space, in dp units,
     * corresponding to
     * <a href="{@docRoot}guide/topics/resources/providing-resources.html#ScreenHeightQualifier">screen
     * height</a> resource qualifier.  Set to
     * {@link #SCREEN_HEIGHT_DP_UNDEFINED} if no height is specified.
     */
    Int32 mScreenHeightDp;

    /**
     * The smallest screen size an application will see in normal operation,
     * corresponding to
     * <a href="{@docRoot}guide/topics/resources/providing-resources.html#SmallestScreenWidthQualifier">smallest
     * screen width</a> resource qualifier.
     * This is the smallest value of both screenWidthDp and screenHeightDp
     * in both portrait and landscape.  Set to
     * {@link #SMALLEST_SCREEN_WIDTH_DP_UNDEFINED} if no width is specified.
     */
    Int32 mSmallestScreenWidthDp;

    /**
     * The target screen density being rendered to,
     * corresponding to
     * <a href="{@docRoot}guide/topics/resources/providing-resources.html#DensityQualifier">density</a>
     * resource qualifier.  Set to
     * {@link #DENSITY_DPI_UNDEFINED} if no density is specified.
     */
    Int32 mDensityDpi;

    /** @hide Hack to get this information from WM to app running in compat mode. */
    Int32 mCompatScreenWidthDp;

    /** @hide Hack to get this information from WM to app running in compat mode. */
    Int32 mCompatScreenHeightDp;

    /** @hide Hack to get this information from WM to app running in compat mode. */
    Int32 mCompatSmallestScreenWidthDp;

    /**
     * @hide Internal book-keeping.
     */
    Int32 mSeq;
};

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_RES_CCONFIGURATION_H__
