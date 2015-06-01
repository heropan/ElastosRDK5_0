
#include "RuleBasedBreakIterator.h"
#include <elastos/StringBuffer.h>

using Elastos::Core::StringBuffer;

namespace Elastos {
namespace Text {

RuleBasedBreakIterator::~RuleBasedBreakIterator()
{}

ECode RuleBasedBreakIterator::Init(
    /* [in] */ INativeBreakIterator* iterator)
{
    mWrapped = iterator;
    return NOERROR;
}

ECode RuleBasedBreakIterator::Current(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    return mWrapped->Current(position);
}

ECode RuleBasedBreakIterator::First(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    return mWrapped->First(position);
}

ECode RuleBasedBreakIterator::Following(
    /* [in] */ Int32 offset,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    FAIL_RETURN(ValidateOffset(offset));
    return mWrapped->Following(offset,position);
}

ECode RuleBasedBreakIterator::ValidateOffset(
    /* [in] */ Int32 offset)
{
    AutoPtr<ICharacterIterator> it;
    mWrapped->GetText((ICharacterIterator **)&it);
    Int32 beginIndex, endIndex;
    it->GetBeginIndex(&beginIndex);
    it->GetEndIndex(&endIndex);
    if (offset < beginIndex || offset > endIndex) {
        StringBuffer bufMsg("Valid range is [");
        Int32 i,j;
        it->GetBeginIndex(&i);
        it->GetEndIndex(&j);
        bufMsg += i;
        bufMsg += " ";
        bufMsg += j;
        bufMsg += "]";
        String message;
        bufMsg.Substring(0, bufMsg.GetLength(),&message);
        //throw new IllegalArgumentException(message);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode RuleBasedBreakIterator::GetText(
    /* [out] */ ICharacterIterator** text)
{
    VALIDATE_NOT_NULL(text);
    return mWrapped->GetText((ICharacterIterator **)&text);
}

ECode RuleBasedBreakIterator::Last(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    return mWrapped->Last(position);
}

ECode RuleBasedBreakIterator::Next(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    return mWrapped->Next(position);
}

ECode RuleBasedBreakIterator::Next(
    /* [in] */ Int32 n,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    return mWrapped->Next(n,position);
}

ECode RuleBasedBreakIterator::Previous(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    return mWrapped->Previous(position);
}

ECode RuleBasedBreakIterator::SetText(
    /* [in] */ const String& newText)
{
    return BreakIterator::SetText(newText);
}

ECode RuleBasedBreakIterator::SetText(
    /* [in] */ ICharacterIterator* newText)
{
    // call a method to check if null pointer
    Char32 current;
    newText->Current(&current);
    return mWrapped->SetText(newText);
}

ECode RuleBasedBreakIterator::IsBoundary(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* isBoundary)
{
    VALIDATE_NOT_NULL(isBoundary);
    FAIL_RETURN(ValidateOffset(offset));
    return mWrapped->IsBoundary(offset,isBoundary);
}

/*
 * (non-Javadoc)
 *
 * @see java.text.BreakIterator#preceding(int)
 */
//@Override
ECode RuleBasedBreakIterator::Preceding(
    /* [in] */ Int32 offset,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    FAIL_RETURN(ValidateOffset(offset));
    return mWrapped->Preceding(offset,position);
}

//@Override
ECode RuleBasedBreakIterator::Clone(
    /* [out] */ IInterface ** outface)
{
    return mWrapped->Clone((INativeBreakIterator **)outface);
}

} // namespace Text
} // namespace Elastos
