#include "Collator.h"
#include "CRuleBasedCollator.h"
#include "CLocaleHelper.h"
// #include "CICUUtil.h"
// #include "CRuleBasedCollatorICU.h"

using Elastos::Utility::ILocaleHelper;
using Elastos::Utility::CLocaleHelper;
using Libcore::ICU::IICUUtil;
// using Libcore::ICU::CICUUtil;
// using Libcore::ICU::CRuleBasedCollatorICU;
using Elastos::Core::ICharSequence;
using Elastos::Core::EIID_ICharSequence;

namespace Elastos {
namespace Text {

ECode Collator::constructor()
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    assert(0 && "TODO");
    // CRuleBasedCollatorICU::New(locale, (IRuleBasedCollatorICU**)&mICUColl);
    return NOERROR;
}

ECode Collator::constructor(
    /* [in] */ IRuleBasedCollatorICU* icuColl)
{
    mICUColl = icuColl;
    return NOERROR;
}

ECode Collator::Compare(
    /* [in] */ IInterface* object1,
    /* [in] */ IInterface* object2,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ICharSequence> cs;
    String s1(NULL), s2(NULL);
    if (object1 != NULL && object1->Probe(EIID_ICharSequence) != NULL) {
        cs = (ICharSequence*)object1->Probe(EIID_ICharSequence);
        cs->ToString(&s1);
    }
    if (object2 != NULL && object2->Probe(EIID_ICharSequence) != NULL) {
        cs = (ICharSequence*)object2->Probe(EIID_ICharSequence);
        cs->ToString(&s2);
    }
    return Compare(s1, s2, result);
}

ECode Collator::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (ICollator::Probe(object) == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    AutoPtr<ICollator> collator = ICollator::Probe(object);
    AutoPtr<IRuleBasedCollatorICU> icuCollator;
    assert(0 && "TODO");
    // collator->GetICUCollator((IRuleBasedCollatorICU**)&icuCollator);
    if (mICUColl == NULL) {
        *result = icuCollator == NULL;
        return NOERROR;
    }
    else {
        return IObject::Probe(mICUColl)->Equals(icuCollator->Probe(EIID_IInterface), result);
    }
}

ECode Collator::Equals(
    /* [in] */ const String& string1,
    /* [in] */ const String& string2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 i;
    Compare(string1, string2, &i);
    *result = i == 0;
    return  NOERROR;
}

ECode Collator::GetAvailableLocales(
    /* [out] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);
    AutoPtr<IICUUtil> icuHelper;
    assert(0 && "TODO");
    // CICUUtil::AcquireSingleton((IICUUtil**)&icuHelper);
    return icuHelper->GetAvailableCollatorLocales(locales);
}

ECode Collator::GetDecomposition(
    /* [out] */ Int32* decomposition)
{
    VALIDATE_NOT_NULL(decomposition);
    Int32 i;
    mICUColl->GetDecomposition(&i);
    return DecompositionMode_ICU_Java(i, decomposition);
}

ECode Collator::GetInstance(
    /* [out] */ ICollator** instance)
{
    VALIDATE_NOT_NULL(instance);
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return GetInstance(locale, instance);
}

ECode Collator::GetInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ ICollator** instance)
{
    VALIDATE_NOT_NULL(instance);
    if (locale == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<IRuleBasedCollator> rbc;
    AutoPtr<IRuleBasedCollatorICU> icuCollator;
    assert(0 && "TODO");
    // CRuleBasedCollatorICU::New(locale, (IRuleBasedCollatorICU**)&icuCollator);
    FAIL_RETURN(CRuleBasedCollator::New(icuCollator, (IRuleBasedCollator**)&rbc));
    *instance = (ICollator*)rbc->Probe(EIID_ICollator);
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode Collator::GetStrength(
    /* [out] */ Int32* strength)
{
    VALIDATE_NOT_NULL(strength);
    Int32 i;
    mICUColl->GetStrength(&i);
    return Strength_ICU_Java(i, strength);
}

ECode Collator::SetDecomposition(
    /* [in] */ Int32 value)
{
    Int32 i;
    FAIL_RETURN(DecompositionMode_Java_ICU(value, &i));
    return mICUColl->SetDecomposition(i);
}

ECode Collator::SetStrength(
    /* [in] */ Int32 value)
{
    Int32 i;
    FAIL_RETURN(Strength_Java_ICU(value, &i));
    return mICUColl->SetStrength(i);
}

ECode Collator::DecompositionMode_Java_ICU(
    /* [in] */ Int32 mode,
    /* [out] */ Int32* value)
{
    switch (mode) {
        case ICollator::CANONICAL_DECOMPOSITION:
            *value = IRuleBasedCollatorICU::VALUE_ON;
            return NOERROR;
        case ICollator::NO_DECOMPOSITION:
            *value = IRuleBasedCollatorICU::VALUE_OFF;
            return NOERROR;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode Collator::DecompositionMode_ICU_Java(
    /* [in] */ Int32 mode,
    /* [out] */ Int32* value)
{
    Int32 javaMode = mode;
    switch (mode) {
        case IRuleBasedCollatorICU::VALUE_OFF:
            javaMode = ICollator::NO_DECOMPOSITION;
            break;
        case IRuleBasedCollatorICU::VALUE_ON:
            javaMode = ICollator::CANONICAL_DECOMPOSITION;
            break;
    }
    *value = javaMode;
    return NOERROR;
}

ECode Collator::Strength_Java_ICU(
    /* [in] */ Int32 value,
    /* [out] */ Int32* result)
{
    switch (value) {
        case ICollator::PRIMARY:
            *result = IRuleBasedCollatorICU::VALUE_PRIMARY;
            return NOERROR;
        case ICollator::SECONDARY:
            *result = IRuleBasedCollatorICU::VALUE_SECONDARY;
            return NOERROR;
        case ICollator::TERTIARY:
            *result = IRuleBasedCollatorICU::VALUE_TERTIARY;
            return NOERROR;
        case ICollator::IDENTICAL:
            *result = IRuleBasedCollatorICU::VALUE_IDENTICAL;
            return NOERROR;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode Collator::Strength_ICU_Java(
    /* [in] */ Int32 value,
    /* [out] */ Int32* result)
{
    switch (value) {
        case IRuleBasedCollatorICU::VALUE_PRIMARY:
            *result = ICollator::PRIMARY;
            break;
        case IRuleBasedCollatorICU::VALUE_SECONDARY:
            *result = ICollator::SECONDARY;
            break;
        case IRuleBasedCollatorICU::VALUE_TERTIARY:
            *result = ICollator::TERTIARY;
            break;
        case IRuleBasedCollatorICU::VALUE_IDENTICAL:
            *result = ICollator::IDENTICAL;
            break;
    }
    return NOERROR;
}

ECode Collator::GetICUCollator(
    /* [out] */ IRuleBasedCollatorICU** icuCollator)
{
    VALIDATE_NOT_NULL(icuCollator);
    *icuCollator = mICUColl;
    REFCOUNT_ADD(*icuCollator);
    return NOERROR;
}

} // namespace Text
} // namespace Elastos
