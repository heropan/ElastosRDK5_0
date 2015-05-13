
#ifndef __CSPELLCHECKERSUBTYPE_H__
#define __CSPELLCHECKERSUBTYPE_H__

namespace Elastos {
namespace Droid {
namespace View {
namespace Textservice {

/**
 * This class is used to specify meta information of a subtype contained in a spell checker.
 * Subtype can describe locale (e.g. en_US, fr_FR...) used for settings.
 */
CarClass(CSpellCheckerSubtype)
{
public:
    CSpellCheckerSubtype();
    ~CSpellCheckerSubtype();

    /**
     * Constructor
     * @param nameId The name of the subtype
     * @param locale The locale supported by the subtype
     * @param extraValue The extra value of the subtype
     */
    CARAPI constructor(
        /* [in] */ Int32 nameId,
        /* [in] */ const String& locale,
        /* [in] */ const String& extraValue);

//    SpellCheckerSubtype(Parcel source);

    /**
     * @return the name of the subtype
     */
    CARAPI GetNameResId(
        /* [out] */ Int32* id);

    /**
     * @return the locale of the subtype
     */
    CARAPI GetLocale(
        /* [out] */ String* locale);

    /**
     * @return the extra value of the subtype
     */
    CARAPI GetExtraValue(
        /* [out] */ String* value);

    /**
     * The string of ExtraValue in subtype should be defined as follows:
     * example: key0,key1=value1,key2,key3,key4=value4
     * @param key the key of extra value
     * @return the subtype contains specified the extra value
     */
    CARAPI ContainsExtraValueKey(
        /* [in] */ const String& key,
        /* [out] */ Boolean* bFlag);

    /**
     * The string of ExtraValue in subtype should be defined as follows:
     * example: key0,key1=value1,key2,key3,key4=value4
     * @param key the key of extra value
     * @return the value of the specified key
     */
    CARAPI GetExtraValueOf(
        /* [in] */ const String& key,
        /* [out] */ String* str);

    /**
     * @hide
     */
    static CARAPI_(AutoPtr<ILocale>) ConstructLocaleFromString(
        /* [in] */ const String& localeStr);

    /**
     * @param context Context will be used for getting Locale and PackageManager.
     * @param packageName The package name of the spell checker
     * @param appInfo The application info of the spell checker
     * @return a display name for this subtype. The string resource of the label (mSubtypeNameResId)
     * can have only one %s in it. If there is, the %s part will be replaced with the locale's
     * display name by the formatter. If there is not, this method simply returns the string
     * specified by mSubtypeNameResId. If mSubtypeNameResId is not specified (== 0), it's up to the
     * framework to generate an appropriate display name.
     */
    CARAPI GetDisplayName(
        /* [in] */ IContext* context,
        /* [in] */ const String& packageName,
        /* [in] */ IApplicationInfo* appInfo,
        /* [out] */ ICharSequence* name);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    //@Override
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    /**
     * Sort the list of subtypes
     * @param context Context will be used for getting localized strings
     * @param flags Flags for the sort order
     * @param sci SpellCheckerInfo of which subtypes are subject to be sorted
     * @param subtypeList List which will be sorted
     * @return Sorted list of subtypes
     * @hide
     */
    static CARAPI_(AutoPtr<List<SpellCheckerSubtype> >) Sort(
        /* [in] */ IContext* context,
        /* [in] */ Int32 flags,
        /* [in] */ SpellCheckerInfo sci,
        /* [in] */ List<SpellCheckerSubtype>* subtypeList);

private:

    CARAPI_(AutoPtr<HashMap<String, String> >) GetExtraValueHashMap();

    static CARAPI_(Int32) HashCodeInternal(
        /* [in] */ const String& locale,
        /* [in] */ const String& extraValue);

private:
    static const String TAG;// = SpellCheckerSubtype.class.getSimpleName();
    static const String EXTRA_VALUE_PAIR_SEPARATOR;// = ",";
    static const String EXTRA_VALUE_KEY_VALUE_SEPARATOR;// = "=";

    Int32 mSubtypeHashCode;
    Int32 mSubtypeNameResId;
    String mSubtypeLocale;
    String mSubtypeExtraValue;
    HashMap<String, String> mExtraValueHashMapCache;
};

}   //namespace Textservice
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__CSPELLCHECKERSUBTYPE_H__
