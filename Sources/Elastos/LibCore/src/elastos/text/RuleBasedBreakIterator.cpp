
#include "RuleBasedBreakIterator.h"
#include "StringBuffer.h"

using Elastos::Core::StringBuffer;
using Elastos::Core::ICloneable;

namespace Elastos {
namespace Text {

RuleBasedBreakIterator::~RuleBasedBreakIterator()
{}

ECode RuleBasedBreakIterator::constructor(
    /* [in] */ INativeBreakIterator* iterator)
{
    return BreakIterator::constructor(iterator);
}

ECode RuleBasedBreakIterator::GetCurrent(
    /* [out] */ Int32* position)
{
    return mWrapped->GetCurrent(position);
}

ECode RuleBasedBreakIterator::GetFirst(
    /* [out] */ Int32* position)
{
    return mWrapped->GetFirst(position);
}

ECode RuleBasedBreakIterator::GetFollowing(
    /* [in] */ Int32 offset,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    FAIL_RETURN(ValidateOffset(offset));
    return mWrapped->GetFollowing(offset, position);
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
    return mWrapped->GetText(text);
}

ECode RuleBasedBreakIterator::GetLast(
    /* [out] */ Int32* position)
{
    return mWrapped->GetLast(position);
}

ECode RuleBasedBreakIterator::GetNext(
    /* [out] */ Int32* position)
{
    return mWrapped->GetNext(position);
}

ECode RuleBasedBreakIterator::GetNext(
    /* [in] */ Int32 n,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    return mWrapped->GetNext(n,position);
}

ECode RuleBasedBreakIterator::GetPrevious(
    /* [out] */ Int32* position)
{
    return mWrapped->GetPrevious(position);
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
    newText->GetCurrent(&current);
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
ECode RuleBasedBreakIterator::GetPreceding(
    /* [in] */ Int32 offset,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    FAIL_RETURN(ValidateOffset(offset));
    return mWrapped->GetPreceding(offset, position);
}

//@Override
ECode RuleBasedBreakIterator::Clone(
    /* [out] */ IInterface ** outface)
{
    return ICloneable::Probe(mWrapped)->Clone(outface);
}

} // namespace Text
} // namespace Elastos
