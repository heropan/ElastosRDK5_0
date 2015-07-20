
#include "view/inputmethod/CInputMethodSubtype.h"
#include "text/TextUtils.h"
#include <elastos/utility/etl/List.h>

using Elastos::Core::CStringWrapper;
using Elastos::Utility::Etl::List;
using Libcore::ICU::CLocale;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

String CInputMethodSubtype::TAG("CInputMethodSubtype");
String CInputMethodSubtype::EXTRA_VALUE_PAIR_SEPARATOR(",");
String CInputMethodSubtype::EXTRA_VALUE_KEY_VALUE_SEPARATOR("=");
String CInputMethodSubtype::EXTRA_KEY_UNTRANSLATABLE_STRING_IN_SUBTYPE_NAME("UntranslatableReplacementStringInSubtypeName");


CInputMethodSubtype::CInputMethodSubtype()
    : mIsAuxiliary(FALSE)
    , mOverridesImplicitlyEnabledSubtype(FALSE)
    , mSubtypeHashCode(0)
    , mSubtypeIconResId(0)
    , mSubtypeNameResId(0)
    , mSubtypeId(0)
{
}

CInputMethodSubtype::~CInputMethodSubtype()
{
    mExtraValueHashMapCache = NULL;
}

ECode CInputMethodSubtype::constructor() {
    return NOERROR;
}

ECode CInputMethodSubtype::constructor(
    /* [in] */ Int32 nameId,
    /* [in] */ Int32 iconId,
    /* [in] */ const String& locale,
    /* [in] */ const String& mode,
    /* [in] */ const String& extraValue,
    /* [in] */ Boolean isAuxiliary)
{
    return Init(nameId, iconId, locale, mode, extraValue, isAuxiliary);
}

ECode CInputMethodSubtype::constructor(
    /* [in] */ Int32 nameId,
    /* [in] */ Int32 iconId,
    /* [in] */ const String& locale,
    /* [in] */ const String& mode,
    /* [in] */ const String& extraValue,
    /* [in] */ Boolean isAuxiliary,
    /* [in] */ Boolean overridesImplicitlyEnabledSubtype)
{
    return Init(nameId, iconId, locale, mode, extraValue, isAuxiliary,
            overridesImplicitlyEnabledSubtype);
}

ECode CInputMethodSubtype::constructor(
    /* [in] */ Int32 nameId,
    /* [in] */ Int32 iconId,
    /* [in] */ const String& locale,
    /* [in] */ const String& mode,
    /* [in] */ const String& extraValue,
    /* [in] */ Boolean isAuxiliary,
    /* [in] */ Boolean overridesImplicitlyEnabledSubtype,
    /* [in] */ Int32 id)
{
    return Init(nameId, iconId, locale, mode, extraValue, isAuxiliary, overridesImplicitlyEnabledSubtype, id);
}

ECode CInputMethodSubtype::Init(
    /* [in] */ Int32 nameId,
    /* [in] */ Int32 iconId,
    /* [in] */ const String& locale,
    /* [in] */ const String& mode,
    /* [in] */ const String& extraValue,
    /* [in] */ Boolean isAuxiliary,
    /* [in] */ Boolean overridesImplicitlyEnabledSubtype,
    /* [in] */ Int32 id)
{
    mSubtypeNameResId = nameId;
    mSubtypeIconResId = iconId;
    mSubtypeLocale = !locale.IsNull() ? locale : "";
    mSubtypeMode = !mode.IsNull() ? mode : "";
    mSubtypeExtraValue = !extraValue.IsNull() ? extraValue : "";
    mIsAuxiliary = isAuxiliary;
    mOverridesImplicitlyEnabledSubtype = overridesImplicitlyEnabledSubtype;
    // If hashCode() of this subtype is 0 and you want to specify it as an id of this subtype,
    // just specify 0 as this subtype's id. Then, this subtype's id is treated as 0.
    mSubtypeHashCode = id != 0 ? id : HashCodeInternal(mSubtypeLocale, mSubtypeMode,
            mSubtypeExtraValue, mIsAuxiliary, mOverridesImplicitlyEnabledSubtype);
    mSubtypeId = id;
    return NOERROR;
}

ECode CInputMethodSubtype::GetNameResId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mSubtypeNameResId;
    return NOERROR;
}

ECode CInputMethodSubtype::GetIconResId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mSubtypeIconResId;
    return NOERROR;
}

ECode CInputMethodSubtype::GetLocale(
    /* [out] */ String* locale)
{
    VALIDATE_NOT_NULL(locale);
    *locale = mSubtypeLocale;
    return NOERROR;
}

ECode CInputMethodSubtype::GetMode(
    /* [out] */ String* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = mSubtypeMode;
    return NOERROR;
}

ECode CInputMethodSubtype::GetExtraValue(
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mSubtypeExtraValue;
    return NOERROR;
}

ECode CInputMethodSubtype::IsAuxiliary(
    /* [out] */ Boolean* auxi)
{
    VALIDATE_NOT_NULL(auxi);
    *auxi = mIsAuxiliary;
    return NOERROR;
}

ECode CInputMethodSubtype::OverridesImplicitlyEnabledSubtype(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = mOverridesImplicitlyEnabledSubtype;
    return NOERROR;
}

ECode CInputMethodSubtype::GetDisplayName(
    /* [in] */ IContext* context,
    /* [in] */ const String& packageName,
    /* [in] */ IApplicationInfo* appInfo,
    /* [out] */ ICharSequence** name)
{
    VALIDATE_NOT_NULL(name);

    AutoPtr<ILocale> locale = ConstructLocaleFromString(mSubtypeLocale);
    String localeStr = mSubtypeLocale;
    if (locale != NULL) {
        locale->GetDisplayName(&localeStr);
    }
    if (mSubtypeNameResId == 0) {
        return CStringWrapper::New(localeStr, name);
    }
    AutoPtr<ICharSequence> subtypeName;
    AutoPtr<IPackageManager> pm;
    context->GetPackageManager((IPackageManager**)&pm);
    pm->GetText(packageName, mSubtypeNameResId, appInfo, (ICharSequence**)&subtypeName);
    if (!TextUtils::IsEmpty(subtypeName)) {
        Boolean result = FALSE;
        ContainsExtraValueKey(EXTRA_KEY_UNTRANSLATABLE_STRING_IN_SUBTYPE_NAME, &result);
        String replacementString = localeStr;
        if (result) {
            GetExtraValueOf(EXTRA_KEY_UNTRANSLATABLE_STRING_IN_SUBTYPE_NAME, &replacementString);
        }
        assert(0);
        // try {
        //     return String.format(
        //             subtypeName.toString(), replacementString != null ? replacementString : "");
        // } catch (IllegalFormatException e) {
        //     Slog.w(TAG, "Found illegal format in subtype name("+ subtypeName + "): " + e);
        //     return "";
        // }
    }
    return CStringWrapper::New(localeStr, name);
}

AutoPtr< HashMap<String, String> > CInputMethodSubtype::GetExtraValueHashMap()
{
    if (mExtraValueHashMapCache == NULL) {
        AutoLock lock(mLock);

        if (mExtraValueHashMapCache == NULL) {
            mExtraValueHashMapCache = new HashMap<String, String>(10);
            assert(0);
            //TODO
            // final String[] pairs = mSubtypeExtraValue.split(EXTRA_VALUE_PAIR_SEPARATOR);
            AutoPtr< ArrayOf<String> > pairs;
            const Int32 N = pairs->GetLength();
            for (Int32 i = 0; i < N; ++i) {
                assert(0);
                //TODO
                //final String[] pair = pairs[i].split(EXTRA_VALUE_KEY_VALUE_SEPARATOR);
                AutoPtr< ArrayOf<String> > pair;
                Int32 len = pair->GetLength();
                if (len == 1) {
                    (*mExtraValueHashMapCache)[(*pair)[0]] = NULL;
                }
                else if (len > 1) {
                    if (len > 2) {
                        // Slog.w(TAG, "ExtraValue has two or more '='s");
                    }
                    (*mExtraValueHashMapCache)[(*pair)[0]] = (*pair)[1];
                }
            }
        }
    }
    return mExtraValueHashMapCache;
}

ECode CInputMethodSubtype::ContainsExtraValueKey(
    /* [in] */ const String& key,
    /* [out] */ Boolean* contains)
{
    VALIDATE_NOT_NULL(contains);

    AutoPtr< HashMap<String, String> > values = GetExtraValueHashMap();
    *contains = values->Find(key) != values->End();
    return NOERROR;
}

ECode CInputMethodSubtype::GetExtraValueOf(
    /* [in] */ const String& key,
    /* [out] */ String* extraValue)
{
    VALIDATE_NOT_NULL(extraValue);

    *extraValue = NULL;
    AutoPtr< HashMap<String, String> > values = GetExtraValueHashMap();
    HashMap<String, String>::Iterator it = values->Find(key);
    if (it != values->End()) {
        *extraValue = it->mSecond;
    }
    return NOERROR;
}

ECode CInputMethodSubtype::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = mSubtypeHashCode;
    return NOERROR;
}

ECode CInputMethodSubtype::Equals(
    /* [in] */ IInputMethodSubtype* o,
    /* [out] */ Boolean* equals)
{
    VALIDATE_NOT_NULL(equals);
    CInputMethodSubtype* subtype = (CInputMethodSubtype*)o;
    if (subtype->mSubtypeId != 0 || mSubtypeId != 0) {
        *equals = subtype->mSubtypeHashCode == mSubtypeHashCode;
        return NOERROR;
    }
    *equals = (subtype->mSubtypeHashCode == mSubtypeHashCode)
            && (subtype->mSubtypeNameResId == mSubtypeNameResId)
            && (subtype->mSubtypeMode.Equals(mSubtypeMode))
            && (subtype->mSubtypeIconResId == mSubtypeIconResId)
            && (subtype->mSubtypeLocale.Equals(mSubtypeLocale))
            && (subtype->mSubtypeExtraValue.Equals(mSubtypeExtraValue))
            && (subtype->mIsAuxiliary == mIsAuxiliary);
    return NOERROR;
}

ECode CInputMethodSubtype::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    return Equals(IInputMethodSubtype::Probe(other), result);
}

ECode CInputMethodSubtype::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mSubtypeNameResId);
    dest->WriteInt32(mSubtypeIconResId);
    dest->WriteString(mSubtypeLocale);
    dest->WriteString(mSubtypeMode);
    dest->WriteString(mSubtypeExtraValue);
    dest->WriteBoolean(mIsAuxiliary);
    dest->WriteBoolean(mOverridesImplicitlyEnabledSubtype);
    dest->WriteInt32(mSubtypeHashCode);
    dest->WriteInt32(mSubtypeId);
    return NOERROR;
}

ECode CInputMethodSubtype::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    String s;
    source->ReadInt32(&mSubtypeNameResId);
    source->ReadInt32(&mSubtypeIconResId);
    source->ReadString(&s);
    mSubtypeLocale = !s.IsNull() ? s : "";
    source->ReadString(&s);
    mSubtypeMode = !s.IsNull() ? s : "";
    source->ReadString(&s);
    mSubtypeExtraValue = !s.IsNull() ? s : "";
    source->ReadBoolean(&mIsAuxiliary);
    source->ReadBoolean(&mOverridesImplicitlyEnabledSubtype);
    source->ReadInt32(&mSubtypeHashCode);
    source->ReadInt32(&mSubtypeId);
    return NOERROR;
}

AutoPtr<ILocale> CInputMethodSubtype::ConstructLocaleFromString(
    /* [in] */ const String& localeStr)
{
    if (localeStr.IsNullOrEmpty()) return NULL;

    assert(0);
    //TODO
    //String[] localeParams = localeStr.Split("_", 3);
    AutoPtr<ArrayOf<String> > localeParams;

    AutoPtr<ILocale> locale;
    // The length of localeStr is guaranteed to always return a 1 <= value <= 3
    // because localeStr is not empty.
    Int32 len = localeParams->GetLength();
    if (len == 1) {
        CLocale::New((*localeParams)[0], (ILocale**)&locale);
        return locale;
    }
    else if (len == 2) {
        CLocale::New((*localeParams)[0], (*localeParams)[1], (ILocale**)&locale);
        return locale;
    }
    else if (len == 3) {
        CLocale::New((*localeParams)[0], (*localeParams)[1], (*localeParams)[2], (ILocale**)&locale);
        return locale;
    }
    return NULL;
}

Int32 CInputMethodSubtype::HashCodeInternal(
    /* [in] */ const String& locale,
    /* [in] */ const String& mode,
    /* [in] */ const String& extraValue,
    /* [in] */ Boolean isAuxiliary,
    /* [in] */ Boolean overridesImplicitlyEnabledSubtype)
{
    //TODO
    // return Arrays.hashCode(new Object[] {locale, mode, extraValue, isAuxiliary,
    //         overridesImplicitlyEnabledSubtype});
    return 0;
}

AutoPtr<IObjectContainer> CInputMethodSubtype::Sort(
    /* [in] */ IContext* context,
    /* [in] */ Int32 flags,
    /* [in] */ IInputMethodInfo* imi,
    /* [in] */ IObjectContainer* subtypeList)
{
    assert(0);
//    if (imi == NULL) return _subtypeList;
//
//    AutoPtr<ArrayOf<IInputMethodSubtype*> > subtypeList = _subtypeList->Clone();
//
//    List<AutoPtr<IInputMethodSubtype> > sortedList;
//    Int32 N = 0;
//    imi->GetSubtypeCount(&N);
//    for (Int32 i = 0; i < N; ++i) {
//        AutoPtr<IInputMethodSubtype> subtype;
//        imi->GetSubtypeAt(i, (IInputMethodSubtype**)&subtype);
//        assert(subtype != NULL);
//
//        for (Int32 j = 0; j < subtypeList->GetLength(); j++) {
//            if ((*subtypeList)[j] != NULL && (*subtypeList)[j] == subtype) {
//                subtypeList->Set(j, NULL);
//                sortedList.PushBack(subtype);
//                break;
//            }
//        }
//    }
//
//    // If subtypes in subtypeList remain, that means these subtypes are not
//    // contained in imi, so the remaining subtypes will be appended.
//    for (Int32 i = 0; i < subtypeList->GetLength(); i++) {
//        AutoPtr<IInputMethodSubtype> subtype = (*subtypeList)[i];
//
//        if (subtype != NULL) {
//            sortedList.PushBack(subtype);
//        }
//    }
//
//    Int32 size = sortedList.GetSize();
//    AutoPtr<ArrayOf<IInputMethodSubtype*> > retList = ArrayOf<IInputMethodSubtype*>::Alloc(size);
//    for (Int32 i = 0; i < size; i++) {
//        (*retList)[i] = sortedList[i];
//    }
//
//    return retList;
    return NULL;
}

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos
