
#include "DateFormatSymbols.h"
#include "CDateFormatSymbols.h"
#include "CStringWrapper.h"
#include "CObjectContainer.h"
#include "CLocaleHelper.h"
#include "CLocaleDataHelper.h"
#include "CICUHelper.h"
#include "TimeZones.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Core::CObjectContainer;
using Elastos::Utility::ILocaleHelper;
using Elastos::Utility::CLocaleHelper;
using Libcore::ICU::ILocaleDataHelper;
using Libcore::ICU::CLocaleDataHelper;
using Libcore::ICU::ILocaleData;
using Libcore::ICU::IICUHelper;
// using Libcore::ICU::CICUHelper;
using Libcore::ICU::TimeZones;

namespace Elastos {
namespace Text {

DateFormatSymbols::DateFormatSymbols()
    : mCustomZoneStrings(FALSE)
{
}

DateFormatSymbols::~DateFormatSymbols()
{
}

AutoPtr<ArrayOf<IObjectContainer*> > DateFormatSymbols::InternalZoneStrings()
{
    Mutex::Autolock lock(mLock);
    if (mZoneStrings == NULL) {
        AutoPtr< ArrayOf<IArrayOf*> > result;
        TimeZones::GetZoneStrings(mLocale, (ArrayOf<IArrayOf*>**)&result);
        Int32 length = result->GetLength();
        mZoneStrings = ArrayOf<IObjectContainer*>::Alloc(length);
        for (Int32 i = 0; i < length; ++i) {
            AutoPtr<IArrayOf> arrstr = (*result)[i];
            AutoPtr<IObjectContainer> newBc;
            CObjectContainer::New((IObjectContainer**)&newBc);
            Int32 arrlen = 0;
            arrstr->GetLength(&arrlen);
            for (Int32 j = 0; j < arrlen; ++j) {
                AutoPtr<IInterface> newSeq;
                arrstr->Get(j, (IInterface**)&newSeq);
                newBc->Add(newSeq);
            }
            mZoneStrings->Set(i, newBc);
        }
    }

    return mZoneStrings;
}

ECode DateFormatSymbols::Init()
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return Init(locale);
}

ECode DateFormatSymbols::Init(
    /* [in] */ ILocale* locale)
{
    mLocale = locale;
    mLocalPatternChars = ISimpleDateFormat_PATTERN_CHARS;
    AutoPtr<ILocaleDataHelper> localeDataHelper;
    FAIL_RETURN(CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&localeDataHelper));
    AutoPtr<ILocaleData> localeData;
    FAIL_RETURN(localeDataHelper->Get(locale, (ILocaleData**)&localeData));

    localeData->GetAmPm((ArrayOf<String>**)&mAmpms);
    localeData->GetEras((ArrayOf<String>**)&mEras);
    localeData->GetLongMonthNames((ArrayOf<String>**)&mMonths);
    localeData->GetShortMonthNames((ArrayOf<String>**)&mShortMonths);
    localeData->GetLongWeekdayNames((ArrayOf<String>**)&mWeekdays);
    localeData->GetShortWeekdayNames((ArrayOf<String>**)&mShortWeekdays);

    // ICU/Android extensions.
    localeData->GetLongStandAloneMonthNames((ArrayOf<String>**)&mLongStandAloneMonths);
    localeData->GetShortStandAloneMonthNames((ArrayOf<String>**)&mShortStandAloneMonths);
    localeData->GetLongStandAloneWeekdayNames((ArrayOf<String>**)&mLongStandAloneWeekdays);
    localeData->GetShortStandAloneWeekdayNames((ArrayOf<String>**)&mShortStandAloneWeekdays);

    return NOERROR;
}

ECode DateFormatSymbols::GetInstance(
    /* [out] */ IDateFormatSymbols** instance)
{
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> pLocale;
    pLocaleHelper->GetDefault((ILocale**)&pLocale);
    return GetInstance((ILocale*)pLocale, instance);
}

ECode DateFormatSymbols::GetInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormatSymbols** instance)
{
    VALIDATE_NOT_NULL(locale);
    VALIDATE_NOT_NULL(instance)

    return CDateFormatSymbols::New(locale, instance);
}

ECode DateFormatSymbols::Clone(
    /* [out] */ IDateFormatSymbols** instance)
{
    VALIDATE_NOT_NULL(instance)

    AutoPtr<IDateFormatSymbols> outfs;
    if (mLocale) {
        FAIL_RETURN(CDateFormatSymbols::New(mLocale, (IDateFormatSymbols **)&outfs));
    } else {
        FAIL_RETURN(CDateFormatSymbols::New((IDateFormatSymbols **)&outfs));
    }
    outfs->SetAmPmStrings(*mAmpms);
    outfs->SetEras(*mEras);
    outfs->SetLocalPatternChars(mLocalPatternChars);
    outfs->SetMonths(*mMonths);
    outfs->SetShortMonths(*mShortMonths);
    outfs->SetShortWeekdays(*mShortWeekdays);
    outfs->SetWeekdays(*mWeekdays);
    // outfs->SetZoneStrings(mZoneStrings);
    *instance = outfs;
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode DateFormatSymbols::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if (this->Probe(EIID_IDateFormatSymbols) == IDateFormatSymbols::Probe(object)) {
        *res = TRUE;
        return NOERROR;
    }
    if (object->Probe(EIID_IDateFormatSymbols) == NULL) {
        *res = FALSE;
        return NOERROR;
    }

    AutoPtr<IDateFormatSymbols> rhs = (IDateFormatSymbols*) object;
    String lpc;
    rhs->GetLocalPatternChars(&lpc);
    AutoPtr<ArrayOf<String> > ampms;
    rhs->GetAmPmStrings((ArrayOf<String>**)&ampms);
    AutoPtr<ArrayOf<String> > eras;
    rhs->GetEras((ArrayOf<String>**)&eras);
    AutoPtr<ArrayOf<String> > months;
    rhs->GetMonths((ArrayOf<String>**)&months);
    AutoPtr<ArrayOf<String> > shortMonths;
    rhs->GetShortMonths((ArrayOf<String>**)&shortMonths);
    AutoPtr<ArrayOf<String> > shortWeekdays;
    rhs->GetShortWeekdays((ArrayOf<String>**)&shortWeekdays);
    AutoPtr<ArrayOf<String> > weekdays;
    rhs->GetWeekdays((ArrayOf<String>**)&weekdays);

    *res = mLocalPatternChars.Equals(lpc) &&
                mAmpms->Equals(ampms) &&
                mEras->Equals(eras) &&
                mMonths->Equals(months) &&
                mShortMonths->Equals(shortMonths) &&
                mShortWeekdays->Equals(shortWeekdays) &&
                mWeekdays->Equals(weekdays) &&
                TimeZoneStringsEqual(this->Probe(EIID_IDateFormatSymbols), rhs);
    return NOERROR;
}

ECode DateFormatSymbols::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** arrayOfLocales)
{
    VALIDATE_NOT_NULL(arrayOfLocales)
    AutoPtr<IICUHelper> pICUHelper;
    FAIL_RETURN(CICUHelper::AcquireSingleton((IICUHelper**)&pICUHelper))
    return pICUHelper->GetAvailableDateFormatSymbolsLocales(arrayOfLocales);
}

Boolean DateFormatSymbols::TimeZoneStringsEqual(
    /* [in] */ IDateFormatSymbols *lhs,
    /* [in] */ IDateFormatSymbols *rhs)
{
    // Quick check that may keep us from having to load the zone strings.
    // Note that different locales may have the same strings, so the opposite check isn't valid.
    Boolean loflag;
    ((CDateFormatSymbols *)lhs)->mLocale->Equals(((CDateFormatSymbols *)rhs)->mLocale, &loflag);
    if (loflag && ((CDateFormatSymbols *)lhs)->mZoneStrings == NULL &&
        ((CDateFormatSymbols *)rhs)->mZoneStrings == NULL) {
        return TRUE;
    }

    // Make sure zone strings are loaded, then check.
    AutoPtr<ArrayOf<IObjectContainer*> > arrays1 = ((CDateFormatSymbols *)lhs)->InternalZoneStrings();
    AutoPtr<ArrayOf<IObjectContainer*> > arrays2 = ((CDateFormatSymbols *)rhs)->InternalZoneStrings();
    if (arrays1 != NULL || arrays2 != NULL || arrays1->GetLength() != arrays2->GetLength()) {
        return FALSE;
    }

    Int32 n1, n2;
    String str1, str2;
    Boolean hasNext1, hasNext2;
    for (Int32 i = 0; i < arrays1->GetLength(); ++i) {
        AutoPtr<IObjectContainer> arr1 = (*arrays1)[i];
        AutoPtr<IObjectContainer> arr2 = (*arrays2)[i];

        if (arr1.Get() == arr2.Get())
            continue;

        if (arr1 == NULL || arr2 == NULL) return FALSE;

        arr1->GetObjectCount(&n1);
        arr2->GetObjectCount(&n2);
        if (n1 != n2) return FALSE;

        AutoPtr<IObjectEnumerator> e1, e2;
        arr1->GetObjectEnumerator((IObjectEnumerator**)&e1);
        arr2->GetObjectEnumerator((IObjectEnumerator**)&e2);

        for (Int32 i = 0; i < n1; ++i) {
            e1->MoveNext(&hasNext1);
            e2->MoveNext(&hasNext2);
            if (hasNext1 != hasNext2) return FALSE;
            if (!hasNext1) break;

            AutoPtr<ICharSequence> seq1, seq2;
            e1->Current((IInterface**)&seq1);
            e2->Current((IInterface**)&seq2);
            if (seq1 == seq2) continue;
            if (seq1 == NULL) continue;

            seq1->ToString(&str1);
            seq2->ToString(&str2);
            if (!str1.Equals(str2)) return FALSE;
        }
    }

    return TRUE;
}

ECode DateFormatSymbols::GetLongStandAloneMonths(
    /* [out, callee] */ ArrayOf<String>** longStandAloneMonths)
{
    VALIDATE_NOT_NULL(longStandAloneMonths)

    *longStandAloneMonths = mLongStandAloneMonths->Clone();
    REFCOUNT_ADD(*longStandAloneMonths)
    return NOERROR;
}

ECode DateFormatSymbols::GetShortStandAloneMonths(
    /* [out, callee] */ ArrayOf<String>** shortStandAloneMonths)
{
    VALIDATE_NOT_NULL(shortStandAloneMonths)

    *shortStandAloneMonths = mShortStandAloneMonths->Clone();
    REFCOUNT_ADD(*shortStandAloneMonths)
    return NOERROR;
}

ECode DateFormatSymbols::GetLongStandAloneWeekdays(
    /* [out, callee] */ ArrayOf<String>** longStandAloneWeekdays)
{
    VALIDATE_NOT_NULL(longStandAloneWeekdays)

    *longStandAloneWeekdays = mLongStandAloneWeekdays->Clone();
    REFCOUNT_ADD(*longStandAloneWeekdays)
    return NOERROR;
}

ECode DateFormatSymbols::GetShortStandAloneWeekdays(
    /* [out, callee] */ ArrayOf<String>** shortStandAloneWeekdays)
{
    VALIDATE_NOT_NULL(shortStandAloneWeekdays)

    *shortStandAloneWeekdays = mShortStandAloneWeekdays->Clone();
    REFCOUNT_ADD(*shortStandAloneWeekdays)
    return NOERROR;
}

ECode DateFormatSymbols::GetCustomZoneStrings(
    /* [out] */ Boolean* customZoneStrings)
{
    VALIDATE_NOT_NULL(customZoneStrings);

    *customZoneStrings = mCustomZoneStrings;
    return NOERROR;
}

ECode DateFormatSymbols::GetLocale(
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale)

    *locale = (ILocale*)mLocale;
    REFCOUNT_ADD(*locale)
    return NOERROR;
}

ECode DateFormatSymbols::GetAmPmStrings(
    /* [out, callee] */ ArrayOf<String>** arrayOfStrings)
{
    VALIDATE_NOT_NULL(arrayOfStrings)

    AutoPtr<ArrayOf<String> > ans = mAmpms->Clone();
    *arrayOfStrings = ans;
    REFCOUNT_ADD(*arrayOfStrings)
    return NOERROR;
}

ECode DateFormatSymbols::GetEras(
    /* [out, callee] */ ArrayOf<String>** arrayOfStrings)
{
    VALIDATE_NOT_NULL(arrayOfStrings)

    *arrayOfStrings = mEras->Clone();
    REFCOUNT_ADD(*arrayOfStrings)
    return NOERROR;
}

ECode DateFormatSymbols::GetLocalPatternChars(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string);

    *string = mLocalPatternChars;
    return NOERROR;
}

ECode DateFormatSymbols::GetMonths(
    /* [out, callee] */ ArrayOf<String>** arrayOfStrings)
{
    VALIDATE_NOT_NULL(arrayOfStrings)

    *arrayOfStrings = mMonths->Clone();
    REFCOUNT_ADD(*arrayOfStrings)
    return NOERROR;
}

ECode DateFormatSymbols::GetShortMonths(
    /* [out, callee] */ ArrayOf<String>** arrayOfStrings)
{
    VALIDATE_NOT_NULL(arrayOfStrings)

    *arrayOfStrings = mShortMonths->Clone();
    REFCOUNT_ADD(*arrayOfStrings)
    return NOERROR;
}

ECode DateFormatSymbols::GetShortWeekdays(
    /* [out, callee] */ ArrayOf<String>** arrayOfStrings)
{
    VALIDATE_NOT_NULL(arrayOfStrings)

    *arrayOfStrings = mShortWeekdays->Clone();
    REFCOUNT_ADD(*arrayOfStrings)
    return NOERROR;
}

ECode DateFormatSymbols::GetWeekdays(
    /* [out, callee] */ ArrayOf<String>** arrayOfStrings)
{
    VALIDATE_NOT_NULL(arrayOfStrings)

    *arrayOfStrings = mWeekdays->Clone();
    REFCOUNT_ADD(*arrayOfStrings)
    return NOERROR;
}

ECode DateFormatSymbols::GetZoneStrings(
    /* [out, callee] */ ArrayOf<IObjectContainer*>** zoneStrings)
{
    VALIDATE_NOT_NULL(zoneStrings)
    AutoPtr<ArrayOf<IObjectContainer*> > strings = InternalZoneStrings();
    return Clone2dStringArray(strings, zoneStrings);
}

ECode DateFormatSymbols::Clone2dStringArray(
    /* [in] */ ArrayOf<IObjectContainer*>* inArray,
    /* [out, callee] */ ArrayOf<IObjectContainer*>** outArray)
{
    VALIDATE_NOT_NULL(inArray)
    VALIDATE_NOT_NULL(outArray)

    String string;
    AutoPtr<ArrayOf<IObjectContainer*> > result = ArrayOf<IObjectContainer*>::Alloc(inArray->GetLength());
    for (Int32 i = 0; i < inArray->GetLength(); ++i) {
        AutoPtr<IObjectContainer> bc = (*inArray)[i];
        if (bc == NULL) {
            result->Set(i, NULL);
            continue;
        }

        AutoPtr<IObjectContainer> newBc;
        CObjectContainer::New((IObjectContainer**)&newBc);

        AutoPtr<IObjectEnumerator> enumerator;
        bc->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
        Boolean hasNext = FALSE;
        while (enumerator->MoveNext(&hasNext), hasNext) {
            AutoPtr<ICharSequence> seq;
            enumerator->Current((IInterface**)&seq);
            seq->ToString(&string);

            AutoPtr<ICharSequence> newSeq;
            CStringWrapper::New(string, (ICharSequence**)&newSeq);
            newBc->Add(newSeq->Probe(EIID_IInterface));
        };

        result->Set(i, newBc);
    }

    *outArray = result;
    REFCOUNT_ADD(*outArray)
    return NOERROR;
}

ECode DateFormatSymbols::SetAmPmStrings(
    /* [in] */ const ArrayOf<String>& data)
{
    mAmpms = data.Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetEras(
    /* [in] */ const ArrayOf<String>& data)
{
    mEras = data.Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetLocalPatternChars(
    /* [in] */ const String& data)
{
    if (data.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mLocalPatternChars = data;
    return NOERROR;
}

ECode DateFormatSymbols::SetMonths(
    /* [in] */ const ArrayOf<String>& data)
{
    mMonths = data.Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetShortMonths(
    /* [in] */ const ArrayOf<String>& data)
{
    mShortMonths = data.Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetShortWeekdays(
    /* [in] */ const ArrayOf<String>& data)
{
    mShortWeekdays = data.Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetWeekdays(
    /* [in] */ const ArrayOf<String>& data)
{
    mWeekdays = data.Clone();
    return NOERROR;
}

ECode DateFormatSymbols::SetZoneStrings(
    /* [in] */ ArrayOf<IObjectContainer*>* zoneStrings)
{
    if (zoneStrings == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 length;
    for (Int32 i = 0; i < zoneStrings->GetLength(); i++) {
        AutoPtr<IObjectContainer> bc = (*zoneStrings)[i];
        if (bc == NULL) return E_ILLEGAL_ARGUMENT_EXCEPTION;

        bc->GetObjectCount(&length);
        if (length < 5) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    mZoneStrings = NULL;
    Clone2dStringArray(zoneStrings, (ArrayOf<IObjectContainer*>**)&mZoneStrings);
    mCustomZoneStrings = TRUE;
    return NOERROR;
}

} // namespace Text
} // namespace Elastos
