#include "CRuleBasedBreakIterator.h"

namespace Elastos {
namespace Text {

ECode CRuleBasedBreakIterator::GetFirst(
    /* [out] */ Int32* postion)
{
    return RuleBasedBreakIterator::GetFirst(postion);
}

ECode CRuleBasedBreakIterator::GetFollowing(
    /* [in] */ Int32 offset,
    /* [out] */ Int32* postion)
{
    return RuleBasedBreakIterator::GetFollowing(offset,postion);
}

ECode CRuleBasedBreakIterator::GetLast(
    /* [out] */ Int32* postion)
{
    return RuleBasedBreakIterator::GetLast(postion);
}

ECode CRuleBasedBreakIterator::GetNext(
    /* [out] */ Int32* postion)
{
    return RuleBasedBreakIterator::GetNext(postion);
}

ECode CRuleBasedBreakIterator::GetNext(
    /* [in] */ Int32 n,
    /* [out] */ Int32* postion)
{
    return RuleBasedBreakIterator::GetNext(n,postion);
}

ECode CRuleBasedBreakIterator::GetPreceding(
    /* [in] */ Int32 offset,
    /* [out] */ Int32* postion)
{
    return RuleBasedBreakIterator::GetPreceding(offset,postion);
}

ECode CRuleBasedBreakIterator::GetPrevious(
    /* [out] */ Int32* postion)
{
    return RuleBasedBreakIterator::GetPrevious(postion);
}

ECode CRuleBasedBreakIterator::GetText(
    /* [out] */ ICharacterIterator** text)
{
    return RuleBasedBreakIterator::GetText(text);
}

ECode CRuleBasedBreakIterator::IsBoundary(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* isBoundary)
{
    return RuleBasedBreakIterator::IsBoundary(offset,isBoundary);
}

ECode CRuleBasedBreakIterator::SetText(
    /* [in] */ const String& newText)
{
    return RuleBasedBreakIterator::SetText(newText);
}

ECode CRuleBasedBreakIterator::SetText(
    /* [in] */ ICharacterIterator* newText)
{
    return RuleBasedBreakIterator::SetText(newText);
}

ECode CRuleBasedBreakIterator::GetCurrent(
    /* [out] */ Int32 * value)
{
    return RuleBasedBreakIterator::GetCurrent(value);
}

ECode CRuleBasedBreakIterator::Clone(
    /* [out] */ IInterface ** outface)
{
    return RuleBasedBreakIterator::Clone(outface);
}

ECode CRuleBasedBreakIterator::constructor(
    /* [in] */ INativeBreakIterator * iterator)
{
    return RuleBasedBreakIterator::Init(iterator);
}

} // namespace Text
} // namespace Elastos