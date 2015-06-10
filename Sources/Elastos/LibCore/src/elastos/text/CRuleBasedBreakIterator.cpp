#include "CRuleBasedBreakIterator.h"

namespace Elastos {
namespace Text {

ECode CRuleBasedBreakIterator::First(
    /* [out] */ Int32* postion)
{
    return RuleBasedBreakIterator::First(postion);
}

ECode CRuleBasedBreakIterator::Following(
    /* [in] */ Int32 offset,
    /* [out] */ Int32* postion)
{
    return RuleBasedBreakIterator::Following(offset,postion);
}

ECode CRuleBasedBreakIterator::Last(
    /* [out] */ Int32* postion)
{
    return RuleBasedBreakIterator::Last(postion);
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

ECode CRuleBasedBreakIterator::Preceding(
    /* [in] */ Int32 offset,
    /* [out] */ Int32* postion)
{
    return RuleBasedBreakIterator::Preceding(offset,postion);
}

ECode CRuleBasedBreakIterator::Previous(
    /* [out] */ Int32* postion)
{
    return RuleBasedBreakIterator::Previous(postion);
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

ECode CRuleBasedBreakIterator::Current(
    /* [out] */ Int32 * value)
{
    return RuleBasedBreakIterator::Current(value);
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