
namespace Elastos {
namespace Droid {
namespace View {
namespace Textservice {

const String CSpellCheckerSubtype::TAG;// = SpellCheckerSubtype.class.getSimpleName();
const String CSpellCheckerSubtype::EXTRA_VALUE_PAIR_SEPARATOR = ",";
const String CSpellCheckerSubtype::EXTRA_VALUE_KEY_VALUE_SEPARATOR = "=";

CSpellCheckerSubtype::CSpellCheckerSubtype()
    : mSubtypeHashCode(0)
    , mSubtypeNameResId(0)
{

}
CSpellCheckerSubtype~CSpellCheckerSubtype()
{

}

/**
 * Constructor
 * @param nameId The name of the subtype
 * @param locale The locale supported by the subtype
 * @param extraValue The extra value of the subtype
 */
ECode CSpellCheckerSubtype::constructor(
    /* [in] */ Int32 nameId,
    /* [in] */ const String& locale,
    /* [in] */ const String& extraValue)
{
    mSubtypeNameResId = nameId;
    mSubtypeLocale = locale != NULL ? locale : "";
    mSubtypeExtraValue = extraValue != NULL ? extraValue : "";
    mSubtypeHashCode = HashCodeInternal(mSubtypeLocale, mSubtypeExtraValue);

    return NOERROR;
}

/**
 * @return the name of the subtype
 */
ECode CSpellCheckerSubtype::GetNameResId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mSubtypeNameResId;
    return NOERROR;
}

/**
 * @return the locale of the subtype
 */
ECode CSpellCheckerSubtype::GetLocale(
    /* [out] */ String* locale)
{
    VALIDATE_NOT_NULL(locale);
    *locale = mSubtypeLocale;
    return NOERROR;
}

/**
 * @return the extra value of the subtype
 */
ECode CSpellCheckerSubtype::GetExtraValue(
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mSubtypeExtraValue;
    return NOERROR;
}

/**
 * The string of ExtraValue in subtype should be defined as follows:
 * example: key0,key1=value1,key2,key3,key4=value4
 * @param key the key of extra value
 * @return the subtype contains specified the extra value
 */
ECode CSpellCheckerSubtype::ContainsExtraValueKey(
    /* [in] */ const String& key,
    /* [out] */ Boolean* bFlag)
{
    VALIDATE_NOT_NULL(bFlag);
    AutoPtr<HashMap<String, String> > hashMap = GetExtraValueHashMap();
    HashMap<String, String>::Iterator iter = Find(key);
    *bFlag = iter != hashMap->End();
    return NOERROR;
}

/**
 * The string of ExtraValue in subtype should be defined as follows:
 * example: key0,key1=value1,key2,key3,key4=value4
 * @param key the key of extra value
 * @return the value of the specified key
 */
ECode CSpellCheckerSubtype::GetExtraValueOf(
    /* [in] */ const String& key,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    AutoPtr<HashMap<String, String> > hashMap = GetExtraValueHashMap();
    *str = (*hashMap)[key];
    return NOERROR;
}

/**
 * @hide
 */
AutoPtr<ILocale> CSpellCheckerSubtype::ConstructLocaleFromString(
    /* [in] */ const String& localeStr)
{
    if (TextUtils::IsEmpty(localeStr))
        return NULL;

//    String[] localeParams = localeStr.split("_", 3);
    // The length of localeStr is guaranteed to always return a 1 <= value <= 3
    // because localeStr is not empty.
    AutoPtr<ILocale> locale;
    if (localeParams.GetLength() == 1) {
        CLocale::New(localeParams[0], (ILocale**)&locale);
        return locale;
    } else if (localeParams.GetLength() == 2) {
        CLocale::New(localeParams[0], localeParams[1], (ILocale**)&locale);
        return locale;
    } else if (localeParams.GetLength() == 3) {
        CLocale::New(localeParams[0], localeParams[1], localeParams[2], (ILocale**)&locale);
        return locale;
    }

    return NULL;
}

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
ECode CSpellCheckerSubtype::GetDisplayName(
    /* [in] */ IContext* context,
    /* [in] */ const String& packageName,
    /* [in] */ IApplicationInfo* appInfo,
    /* [out] */ ICharSequence* name)
{
    VALIDATE_NOT_NULL(name);

    AutoPtr<ILocale> locale = ConstructLocaleFromString(mSubtypeLocale);
    String localeStr;
    if (locale != NULL) {
        locale->GetDisplayName(&localeStr);
    } else {
        localeStr = mSubtypeLocale;
     }
/*
    if (mSubtypeNameResId == 0) {
        return localeStr;
    }

    final CharSequence subtypeName = context.getPackageManager().getText(
            packageName, mSubtypeNameResId, appInfo);

    if (!TextUtils::IsEmpty(subtypeName)) {
        return String.format(subtypeName.toString(), localeStr);
    } else {
        return localeStr;
    }*/
}

ECode CSpellCheckerSubtype::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    String s;
    source->ReadInt32(&mSubtypeNameResId);
    source->ReadString(&mSubtypeLocale);
//    mSubtypeLocale = s != null ? s : "";
    source->ReadString(&mSubtypeExtraValue);
//    mSubtypeExtraValue = s != null ? s : "";
//    mSubtypeHashCode = hashCodeInternal(mSubtypeLocale, mSubtypeExtraValue);
}

//@Override
ECode CSpellCheckerSubtype::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mSubtypeNameResId);
    dest->WriteString(mSubtypeLocale);
    dest->WriteString(mSubtypeExtraValue);

    return NOERROR;
}

/**
 * Sort the list of subtypes
 * @param context Context will be used for getting localized strings
 * @param flags Flags for the sort order
 * @param sci SpellCheckerInfo of which subtypes are subject to be sorted
 * @param subtypeList List which will be sorted
 * @return Sorted list of subtypes
 * @hide
 */
AutoPtr< List<SpellCheckerSubtype> > CSpellCheckerSubtype::Sort(
    /* [in] */ IContext* context,
    /* [in] */ Int32 flags,
    /* [in] */ SpellCheckerInfo sci,
    /* [in] */ List<SpellCheckerSubtype>* subtypeList)
{/*
    if (sci == null) return subtypeList;
    final HashSet<SpellCheckerSubtype> subtypesSet = new HashSet<SpellCheckerSubtype>(
            subtypeList);
    final ArrayList<SpellCheckerSubtype> sortedList = new ArrayList<SpellCheckerSubtype>();
    int N = sci.getSubtypeCount();
    for (int i = 0; i < N; ++i) {
        SpellCheckerSubtype subtype = sci.getSubtypeAt(i);
        if (subtypesSet.contains(subtype)) {
            sortedList.add(subtype);
            subtypesSet.remove(subtype);
        }
    }
    // If subtypes in subtypesSet remain, that means these subtypes are not
    // contained in sci, so the remaining subtypes will be appended.
    for (SpellCheckerSubtype subtype: subtypesSet) {
        sortedList.add(subtype);
    }
    return sortedList;*/
}

AutoPtr<HashMap<String, String> > CSpellCheckerSubtype::GetExtraValueHashMap()
{/*
    if (mExtraValueHashMapCache == null) {
        mExtraValueHashMapCache = new HashMap<String, String>();
        final String[] pairs = mSubtypeExtraValue.split(EXTRA_VALUE_PAIR_SEPARATOR);
        final int N = pairs.length;
        for (int i = 0; i < N; ++i) {
            final String[] pair = pairs[i].split(EXTRA_VALUE_KEY_VALUE_SEPARATOR);
            if (pair.length == 1) {
                mExtraValueHashMapCache.put(pair[0], null);
            } else if (pair.length > 1) {
                if (pair.length > 2) {
                    Slog.w(TAG, "ExtraValue has two or more '='s");
                }
                mExtraValueHashMapCache.put(pair[0], pair[1]);
            }
        }
    }

    return mExtraValueHashMapCache;*/
}

Int32 CSpellCheckerSubtype::HashCodeInternal(
    /* [in] */ const String& locale,
    /* [in] */ const String& extraValue)
{
//    return Arrays.hashCode(new Object[] {locale, extraValue});
}

}   //namespace Textservice
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
