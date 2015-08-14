
#ifndef __ELASTOS_DROID_VIEW_INPUTMETHOD_CINPUTMETHODSUBTYPE_H__
#define  __ELASTOS_DROID_VIEW_INPUTMETHOD_CINPUTMETHODSUBTYPE_H__

#include "ext/frameworkdef.h"
#include "_CInputMethodSubtype.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Core::ICharSequence;
using Libcore::ICU::ILocale;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::IApplicationInfo;

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {


/**
 * This class is used to specify meta information of a subtype contained in an input method editor
 * (IME). Subtype can describe locale (e.g. en_US, fr_FR...) and mode (e.g. voice, keyboard...),
 * and is used for IME switch and settings. The input method subtype allows the system to bring up
 * the specified subtype of the designated IME directly.
 *
 * <p>It should be defined in an XML resource file of the input method with the
 * <code>&lt;subtype&gt;</code> element. For more information, see the guide to
 * <a href="{@docRoot}resources/articles/creating-input-method.html">
 * Creating an Input Method</a>.</p>
 */
CarClass(CInputMethodSubtype)
{
public:
    CInputMethodSubtype();

    ~CInputMethodSubtype();

    CARAPI constructor();

    /**
     * Constructor with no subtype ID specified, overridesImplicitlyEnabledSubtype not specified.
     * @param nameId Resource ID of the subtype name string. The string resource may have exactly
     * one %s in it. If there is, the %s part will be replaced with the locale's display name by
     * the formatter. Please refer to {@link #getDisplayName} for details.
     * @param iconId Resource ID of the subtype icon drawable.
     * @param locale The locale supported by the subtype
     * @param mode The mode supported by the subtype
     * @param extraValue The extra value of the subtype. This string is free-form, but the API
     * supplies tools to deal with a key-value comma-separated list; see
     * {@link #containsExtraValueKey} and {@link #getExtraValueOf}.
     * @param isAuxiliary true when this subtype is auxiliary, false otherwise. An auxiliary
     * subtype will not be shown in the list of enabled IMEs for choosing the current IME in
     * the Settings even when this subtype is enabled. Please note that this subtype will still
     * be shown in the list of IMEs in the IME switcher to allow the user to tentatively switch
     * to this subtype while an IME is shown. The framework will never switch the current IME to
     * this subtype by {@link android.view.inputmethod.InputMethodManager#switchToLastInputMethod}.
     * The intent of having this flag is to allow for IMEs that are invoked in a one-shot way as
     * auxiliary input mode, and return to the previous IME once it is finished (e.g. voice input).
     * @hide
     */
    CARAPI constructor(
        /* [in] */ Int32 nameId,
        /* [in] */ Int32 iconId,
        /* [in] */ const String& locale,
        /* [in] */ const String& mode,
        /* [in] */ const String& extraValue,
        /* [in] */ Boolean isAuxiliary);

    /**
     * Constructor with no subtype ID specified.
     * @param nameId Resource ID of the subtype name string. The string resource may have exactly
     * one %s in it. If there is, the %s part will be replaced with the locale's display name by
     * the formatter. Please refer to {@link #getDisplayName} for details.
     * @param iconId Resource ID of the subtype icon drawable.
     * @param locale The locale supported by the subtype
     * @param mode The mode supported by the subtype
     * @param extraValue The extra value of the subtype. This string is free-form, but the API
     * supplies tools to deal with a key-value comma-separated list; see
     * {@link #containsExtraValueKey} and {@link #getExtraValueOf}.
     * @param isAuxiliary true when this subtype is auxiliary, false otherwise. An auxiliary
     * subtype will not be shown in the list of enabled IMEs for choosing the current IME in
     * the Settings even when this subtype is enabled. Please note that this subtype will still
     * be shown in the list of IMEs in the IME switcher to allow the user to tentatively switch
     * to this subtype while an IME is shown. The framework will never switch the current IME to
     * this subtype by {@link android.view.inputmethod.InputMethodManager#switchToLastInputMethod}.
     * The intent of having this flag is to allow for IMEs that are invoked in a one-shot way as
     * auxiliary input mode, and return to the previous IME once it is finished (e.g. voice input).
     * @param overridesImplicitlyEnabledSubtype true when this subtype should be enabled by default
     * if no other subtypes in the IME are enabled explicitly. Note that a subtype with this
     * parameter being true will not be shown in the list of subtypes in each IME's subtype enabler.
     * Having an "automatic" subtype is an example use of this flag.
     */
    CARAPI constructor(
        /* [in] */ Int32 nameId,
        /* [in] */ Int32 iconId,
        /* [in] */ const String& locale,
        /* [in] */ const String& mode,
        /* [in] */ const String& extraValue,
        /* [in] */ Boolean isAuxiliary,
        /* [in] */ Boolean overridesImplicitlyEnabledSubtype);

    /**
     * Constructor.
     * @param nameId Resource ID of the subtype name string. The string resource may have exactly
     * one %s in it. If there is, the %s part will be replaced with the locale's display name by
     * the formatter. Please refer to {@link #getDisplayName} for details.
     * @param iconId Resource ID of the subtype icon drawable.
     * @param locale The locale supported by the subtype
     * @param mode The mode supported by the subtype
     * @param extraValue The extra value of the subtype. This string is free-form, but the API
     * supplies tools to deal with a key-value comma-separated list; see
     * {@link #containsExtraValueKey} and {@link #getExtraValueOf}.
     * @param isAuxiliary true when this subtype is auxiliary, false otherwise. An auxiliary
     * subtype will not be shown in the list of enabled IMEs for choosing the current IME in
     * the Settings even when this subtype is enabled. Please note that this subtype will still
     * be shown in the list of IMEs in the IME switcher to allow the user to tentatively switch
     * to this subtype while an IME is shown. The framework will never switch the current IME to
     * this subtype by {@link android.view.inputmethod.InputMethodManager#switchToLastInputMethod}.
     * The intent of having this flag is to allow for IMEs that are invoked in a one-shot way as
     * auxiliary input mode, and return to the previous IME once it is finished (e.g. voice input).
     * @param overridesImplicitlyEnabledSubtype true when this subtype should be enabled by default
     * if no other subtypes in the IME are enabled explicitly. Note that a subtype with this
     * parameter being true will not be shown in the list of subtypes in each IME's subtype enabler.
     * Having an "automatic" subtype is an example use of this flag.
     * @param id The unique ID for the subtype. The input method framework keeps track of enabled
     * subtypes by ID. When the IME package gets upgraded, enabled IDs will stay enabled even if
     * other attributes are different. If the ID is unspecified or 0,
     * Arrays.hashCode(new Object[] {locale, mode, extraValue,
     * isAuxiliary, overridesImplicitlyEnabledSubtype}) will be used instead.
     */
    CARAPI constructor(
        /* [in] */ Int32 nameId,
        /* [in] */ Int32 iconId,
        /* [in] */ const String& locale,
        /* [in] */ const String& mode,
        /* [in] */ const String& extraValue,
        /* [in] */ Boolean isAuxiliary,
        /* [in] */ Boolean overridesImplicitlyEnabledSubtype,
        /* [in] */ Int32 id);

    /**
     * @return Resource ID of the subtype name string.
     */
    CARAPI GetNameResId(
        /* [out] */ Int32* id);

    /**
     * @return Resource ID of the subtype icon drawable.
     */
    CARAPI GetIconResId(
        /* [out] */ Int32* id);

    /**
     * @return The locale of the subtype. This method returns the "locale" string parameter passed
     * to the constructor.
     */
    CARAPI GetLocale(
        /* [out] */ String* locale);

    /**
     * @return The mode of the subtype.
     */
    CARAPI GetMode(
        /* [out] */ String* mode);

    /**
     * @return The extra value of the subtype.
     */
    CARAPI GetExtraValue(
        /* [out] */ String* value);

    /**
     * @return true if this subtype is auxiliary, false otherwise. An auxiliary subtype will not be
     * shown in the list of enabled IMEs for choosing the current IME in the Settings even when this
     * subtype is enabled. Please note that this subtype will still be shown in the list of IMEs in
     * the IME switcher to allow the user to tentatively switch to this subtype while an IME is
     * shown. The framework will never switch the current IME to this subtype by
     * {@link android.view.inputmethod.InputMethodManager#switchToLastInputMethod}.
     * The intent of having this flag is to allow for IMEs that are invoked in a one-shot way as
     * auxiliary input mode, and return to the previous IME once it is finished (e.g. voice input).
     */
    CARAPI IsAuxiliary(
        /* [out] */ Boolean* auxi);

    /**
     * @return true when this subtype will be enabled by default if no other subtypes in the IME
     * are enabled explicitly, false otherwise. Note that a subtype with this method returning true
     * will not be shown in the list of subtypes in each IME's subtype enabler. Having an
     * "automatic" subtype is an example use of this flag.
     */
    CARAPI OverridesImplicitlyEnabledSubtype(
        /* [out] */ Boolean* enabled);

    /**
     * @param context Context will be used for getting Locale and PackageManager.
     * @param packageName The package name of the IME
     * @param appInfo The application info of the IME
     * @return a display name for this subtype. The string resource of the label (mSubtypeNameResId)
     * may have exactly one %s in it. If there is, the %s part will be replaced with the locale's
     * display name by the formatter. If there is not, this method returns the string specified by
     * mSubtypeNameResId. If mSubtypeNameResId is not specified (== 0), it's up to the framework to
     * generate an appropriate display name.
     */
    CARAPI GetDisplayName(
        /* [in] */ IContext* context,
        /* [in] */ const String& packageName,
        /* [in] */ IApplicationInfo* appInfo,
        /* [out] */ ICharSequence** name);

    /**
     * The string of ExtraValue in subtype should be defined as follows:
     * example: key0,key1=value1,key2,key3,key4=value4
     * @param key The key of extra value
     * @return The subtype contains specified the extra value
     */
    CARAPI ContainsExtraValueKey(
        /* [in] */ const String& key,
        /* [out] */ Boolean* contains);

    /**
     * The string of ExtraValue in subtype should be defined as follows:
     * example: key0,key1=value1,key2,key3,key4=value4
     * @param key The key of extra value
     * @return The value of the specified key
     */
    CARAPI GetExtraValueOf(
        /* [in] */ const String& key,
        /* [out] */ String* extraValue);

    //@Override
    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    //@Override
    CARAPI Equals(
        /* [in] */ IInputMethodSubtype* o,
        /* [out] */ Boolean* equals);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean * result);

    //@Override
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

private:
    CARAPI Init(
        /* [in] */ Int32 nameId,
        /* [in] */ Int32 iconId,
        /* [in] */ const String& locale,
        /* [in] */ const String& mode,
        /* [in] */ const String& extraValue,
        /* [in] */ Boolean isAuxiliary,
        /* [in] */ Boolean overridesImplicitlyEnabledSubtype = FALSE,
        /* [in] */ Int32 id = 0);

    AutoPtr< HashMap<String, String> > GetExtraValueHashMap();

    static CARAPI_(AutoPtr<ILocale>) ConstructLocaleFromString(
        /* [in] */ const String& localeStr);

    static CARAPI_(Int32) HashCodeInternal(
        /* [in] */ const String& locale,
        /* [in] */ const String& mode,
        /* [in] */ const String& extraValue,
        /* [in] */ Boolean isAuxiliary,
        /* [in] */ Boolean overridesImplicitlyEnabledSubtype);

    /**
     * Sort the list of InputMethodSubtype
     * @param context Context will be used for getting localized strings from IME
     * @param flags Flags for the sort order
     * @param imi InputMethodInfo of which subtypes are subject to be sorted
     * @param subtypeList List of InputMethodSubtype which will be sorted
     * @return Sorted list of subtypes
     * @hide
     */
    static CARAPI_(AutoPtr<IObjectContainer>) Sort(
        /* [in] */ IContext* context,
        /* [in] */ Int32 flags,
        /* [in] */ IInputMethodInfo* imi,
        /* [in] */ IObjectContainer* subtypeList);

private:
    static String TAG;
    static String EXTRA_VALUE_PAIR_SEPARATOR;
    static String EXTRA_VALUE_KEY_VALUE_SEPARATOR;
    // TODO: remove this
    static String EXTRA_KEY_UNTRANSLATABLE_STRING_IN_SUBTYPE_NAME;

    Boolean mIsAuxiliary;
    Boolean mOverridesImplicitlyEnabledSubtype;
    Int32 mSubtypeHashCode;
    Int32 mSubtypeIconResId;
    Int32 mSubtypeNameResId;
    Int32 mSubtypeId;
    String mSubtypeLocale;
    String mSubtypeMode;
    String mSubtypeExtraValue;
    /*volatile*/ AutoPtr< HashMap<String, String> > mExtraValueHashMapCache;
    Object mLock;
};

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  // __ELASTOS_DROID_VIEW_INPUTMETHOD_CINPUTMETHODSUBTYPE_H__
