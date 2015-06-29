
#include "BreakIterator.h"
#include "CRuleBasedBreakIterator.h"
#include "CLocaleHelper.h"
// #include "CLocaleDataHelper.h"
// #include "ICUUtil.h"
// #include "CNativeBreakIteratorHelper.h"

using Elastos::Utility::ILocaleHelper;
using Elastos::Utility::CLocaleHelper;
using Libcore::ICU::ILocaleDataHelper;
// using Libcore::ICU::CLocaleDataHelper;
using Libcore::ICU::ILocaleData;
//using Libcore::ICU::ICUUtil;
using Libcore::ICU::INativeBreakIteratorHelper;
// using Libcore::ICU::CNativeBreakIteratorHelper;

namespace Elastos {
namespace Text {

BreakIterator::BreakIterator()
{ }

BreakIterator::BreakIterator(
    /* [in] */ INativeBreakIterator* iterator)
    :mWrapped(iterator)
{
}

ECode BreakIterator::GetAvailableLocales(
    /* [out] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);
    // return ICUUtil::GetAvailableBreakIteratorLocales(locales);
}

ECode BreakIterator::GetCharacterInstance(
    /* [out] */ IBreakIterator** instance)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return GetCharacterInstance(locale, instance);
}

ECode BreakIterator::GetCharacterInstance(
    /* [in] */ ILocale* where,
    /* [out] */ IBreakIterator** instance)
{
    VALIDATE_NOT_NULL(where);
    VALIDATE_NOT_NULL(instance);

    AutoPtr<INativeBreakIterator> iter;
    AutoPtr<INativeBreakIteratorHelper> iterhelper;
    assert(0 && "TODO");
    // CNativeBreakIteratorHelper::AcquireSingleton((INativeBreakIteratorHelper **)&iterhelper);
    iterhelper->GetCharacterInstance(where,(INativeBreakIterator **)&iter);
    AutoPtr<IBreakIterator> rbbi;
    CRuleBasedBreakIterator::New(iter,(IBreakIterator **)&rbbi);
    *instance = rbbi;
    REFCOUNT_ADD(*instance);

    return NOERROR;
}

ECode BreakIterator::GetLineInstance(
    /* [out] */ IBreakIterator** instance)
{
    VALIDATE_NOT_NULL(instance);
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return GetLineInstance(locale, instance);
}

ECode BreakIterator::GetLineInstance(
    /* [in] */ ILocale* where,
    /* [out] */ IBreakIterator** instance)
{
    VALIDATE_NOT_NULL(where);
    VALIDATE_NOT_NULL(instance);

    AutoPtr<INativeBreakIterator> iter;
    AutoPtr<INativeBreakIteratorHelper> iterhelper;
    assert(0 && "TODO");
    // CNativeBreakIteratorHelper::AcquireSingleton((INativeBreakIteratorHelper **)&iterhelper);
    iterhelper->GetLineInstance(where,(INativeBreakIterator **)&iter);
    AutoPtr<IBreakIterator> rbbi;
    CRuleBasedBreakIterator::New(iter,(IBreakIterator **)&rbbi);
    *instance = rbbi;
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode BreakIterator::GetSentenceInstance(
    /* [out] */ IBreakIterator** instance)
{
    VALIDATE_NOT_NULL(instance);
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return GetSentenceInstance(locale, instance);
}

ECode BreakIterator::GetSentenceInstance(
    /* [in] */ ILocale* where,
    /* [out] */ IBreakIterator** instance)
{
    VALIDATE_NOT_NULL(where);
    VALIDATE_NOT_NULL(instance);

    AutoPtr<INativeBreakIterator> iter;
    AutoPtr<INativeBreakIteratorHelper> iterhelper;
    assert(0 && "TODO");
    // CNativeBreakIteratorHelper::AcquireSingleton((INativeBreakIteratorHelper **)&iterhelper);
    iterhelper->GetSentenceInstance(where,(INativeBreakIterator **)&iter);
    AutoPtr<IBreakIterator> rbbi;
    CRuleBasedBreakIterator::New(iter,(IBreakIterator **)&rbbi);
    *instance = rbbi;
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode BreakIterator::GetWordInstance(
    /* [out] */ IBreakIterator** instance)
{
    VALIDATE_NOT_NULL(instance);
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return GetWordInstance(locale, instance);
}

ECode BreakIterator::GetWordInstance(
    /* [in] */ ILocale* where,
    /* [out] */ IBreakIterator** instance)
{
    VALIDATE_NOT_NULL(where);
    VALIDATE_NOT_NULL(instance);

    AutoPtr<INativeBreakIterator> iter;
    AutoPtr<INativeBreakIteratorHelper> iterhelper;
    assert(0 && "TODO");
    // CNativeBreakIteratorHelper::AcquireSingleton((INativeBreakIteratorHelper **)&iterhelper);
    iterhelper->GetWordInstance(where,(INativeBreakIterator **)&iter);
    AutoPtr<IBreakIterator> rbbi;
    FAIL_RETURN(CRuleBasedBreakIterator::New(iter,(IBreakIterator **)&rbbi));
    *instance = rbbi;
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode BreakIterator::IsBoundary(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* isBoundary)
{
    VALIDATE_NOT_NULL(isBoundary);
    return mWrapped->IsBoundary(offset,isBoundary);
}

ECode BreakIterator::Preceding(
    /* [in] */ Int32 offset,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    return mWrapped->Preceding(offset,position);
}

ECode BreakIterator::SetText(
    /* [in] */ const String& newText)
{
    return mWrapped->SetText(newText);
}

} // namespace Text
} // namespace Elastos
