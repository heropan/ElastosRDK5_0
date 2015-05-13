
#include "CRuleBasedCollator.h"

namespace Elastos {
namespace Text {

ECode CRuleBasedCollator::constructor(
    /* [in] */ IRuleBasedCollatorICU* wrapper)
{
    return RuleBasedCollator::Init(wrapper);
}

ECode CRuleBasedCollator::constructor(
    /* [in] */ const String& rules)
{
    return RuleBasedCollator::Init(rules);
}

ECode CRuleBasedCollator::Compare(
    /* [in] */ IInterface* object1,
    /* [in] */ IInterface* object2,
    /* [out] */ Int32* result)
{
    return Collator::Compare(object1, object2, result);
}

ECode CRuleBasedCollator::CompareEx(
    /* [in] */ const String& string1,
    /* [in] */ const String& string2,
    /* [out] */ Int32* value)
{
    return RuleBasedCollator::CompareEx(string1, string2, value);
}

ECode CRuleBasedCollator::Equals(
    /* [in] */ IInterface* object,
    /*  [out] */   Boolean* result)
{
    return RuleBasedCollator::Equals(object, result);
}

ECode CRuleBasedCollator::EqualsEx(
    /* [in] */ const String& string1,
    /* [in] */ const String& string2,
    /* [out] */ Boolean* result)
{
    return RuleBasedCollator::EqualsEx(string1, string2, result);
}

ECode CRuleBasedCollator::GetCollationKey(
    /* [in] */ const String& string,
    /* [out] */ ICollationKey** collationKey)
{
    return RuleBasedCollator::GetCollationKey(string, collationKey);
}

ECode CRuleBasedCollator::GetDecomposition(
    /* [out] */ Int32* decomposition)
{
    return RuleBasedCollator::GetDecomposition(decomposition);
}

ECode CRuleBasedCollator::GetStrength(
    /* [out] */ Int32* strength)
{
    return RuleBasedCollator::GetStrength(strength);
}

ECode CRuleBasedCollator::SetDecomposition(
    /* [in] */ Int32 value)
{
    return RuleBasedCollator::SetDecomposition(value);
}

ECode CRuleBasedCollator::SetStrength(
    /* [in] */ Int32 value)
{
    return RuleBasedCollator::SetStrength(value);
}

ECode CRuleBasedCollator::GetCollationElementIteratorEx(
    /* [in] */ ICharacterIterator* source,
    /* [out] */ ICollationElementIterator** collationElementIterator)
{
    return RuleBasedCollator::GetCollationElementIteratorEx(source, collationElementIterator);
}

ECode CRuleBasedCollator::GetCollationElementIterator(
    /* [in] */ const String& source,
    /* [out] */ ICollationElementIterator** collationElementIterator)
{
    VALIDATE_NOT_NULL(collationElementIterator);
    return RuleBasedCollator::GetCollationElementIterator(source, collationElementIterator);
}

ECode CRuleBasedCollator::GetRules(
    /* [out] */ String* rules)
{
    return RuleBasedCollator::GetRules(rules);
}

ECode CRuleBasedCollator::GetICUCollator(
    /* [out] */ IRuleBasedCollatorICU** icuCollator)
{
    return Collator::GetICUCollator(icuCollator);
}

} // namespace Text
} // namespace Elastos
