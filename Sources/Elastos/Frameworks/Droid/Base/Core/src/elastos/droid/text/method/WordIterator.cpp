#include "elastos/droid/text/method/WordIterator.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/Character.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::CLocaleHelper;
using Libcore::ICU::ILocale;
using Elastos::Core::Character;
using Elastos::Utility::Logging::Logger;
//using Elastos::Text::BreakIterator;
//using Elastos::Text::IBreakIterator

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

const Int32 WordIterator::WINDOW_WIDTH = 50;

WordIterator::WordIterator()
{
    AutoPtr<ILocaleHelper> localeHelp;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelp);
    AutoPtr<ILocale> locale;
    localeHelp->GetDefault((ILocale**)&locale);

    Init(locale.Get());
}

WordIterator::WordIterator(
    /* [in] */ ILocale* locale)
{
    Init(locale);
}

void WordIterator::Init(
    /* [in] */ ILocale* locale)
{
    PRINT_FILE_LINE_EX("TODO");
//    BreakIterator::GetWordInstance(locale, (IBreakIterator**)&mIterator);
}

ECode WordIterator::SetCharSequence(
    /* [in] */ ICharSequence* charSequence,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    mOffsetShift = Elastos::Core::Math::Max(0, start - WINDOW_WIDTH);
    Int32 csLen;
    const Int32 windowEnd = Elastos::Core::Math::Min((charSequence->GetLength(&csLen), csLen), end + WINDOW_WIDTH);

    AutoPtr<ISpannableStringBuilder> ssb = ISpannableStringBuilder::Probe(charSequence);
    if (ssb != NULL) {
        ssb->Substring(mOffsetShift, windowEnd, &mString);
    }
    else {
        AutoPtr<ICharSequence> cs;
        charSequence->SubSequence(mOffsetShift, windowEnd, (ICharSequence**)&cs);
        cs->ToString(&mString);
    }
//    mIterator->SetText(mString);
    return NOERROR;
}

Int32 WordIterator::Preceding(
    /* [in] */ Int32 offset)
{
    Int32 shiftedOffset = offset - mOffsetShift;
    while(TRUE) {
//        mIterator->Preceding(shiftedOffset, &shiftedOffset);
        if (shiftedOffset == /*BreakIterator::DONE*/-1) {
            return /*BreakIterator::DONE*/-1;
        }
        if (IsOnLetterOrDigit(shiftedOffset)) {
            return shiftedOffset + mOffsetShift;
        }
    }
}

Int32 WordIterator::Following(
    /* [in] */ Int32 offset)
{
    Int32 shiftedOffset = offset - mOffsetShift;
    while (TRUE){
//        mIterator->Following(shiftedOffset, &shiftedOffset);
        if (shiftedOffset == /*BreakIterator::DONE*/-1) {
            return /*BreakIterator::DONE*/-1;
        }
        if (IsAfterLetterOrDigit(shiftedOffset)) {
            return shiftedOffset + mOffsetShift;
        }
    }
}

Int32 WordIterator::GetBeginning(
    /* [in] */ Int32 offset)
{
    const Int32 shiftedOffset = offset - mOffsetShift;
    CheckOffsetIsValid(shiftedOffset);

    if (IsOnLetterOrDigit(shiftedOffset)) {
        Boolean bIsBoundary = FALSE;
//        mIterator->IsBoundary(shiftedOffset, &bIsBoundary);
        if (bIsBoundary) {
            return shiftedOffset + mOffsetShift;
        }
        else {
            Int32 preceding = 0;
//            mIterator->Preceding(shiftedOffset, &preceding);
            return preceding + mOffsetShift;
        }
    }
    else {
        if (IsAfterLetterOrDigit(shiftedOffset)) {
            Int32 preceding;
//            mIterator->Preceding(shiftedOffset, &preceding);
            return preceding + mOffsetShift;
        }
    }
    return /*BreakIterator::DONE*/-1;
}

Int32 WordIterator::GetEnd(
    /* [in] */ Int32 offset)
{
    const Int32 shiftedOffset = offset - mOffsetShift;
    CheckOffsetIsValid(shiftedOffset);

    if (IsAfterLetterOrDigit(shiftedOffset)) {
        Boolean bIsBoundary = FALSE;
//        mIterator->IsBoundary(shiftedOffset, &bIsBoundary);
        if (bIsBoundary) {
            return shiftedOffset + mOffsetShift;
        } else {
            Int32 following = 0;
//            mIterator->Following(shiftedOffset, &following);
            return following + mOffsetShift;
        }
    } else {
        if (IsOnLetterOrDigit(shiftedOffset)) {
            Int32 following = 0;
//            mIterator->Following(shiftedOffset, &following);
            return following + mOffsetShift;
        }
    }
    return /*BreakIterator::DONE*/-1;
}

Boolean WordIterator::IsAfterLetterOrDigit(
    /* [in] */ Int32 shiftedOffset)
{
    if (shiftedOffset >= 1 && shiftedOffset <= mString.GetLength()) {
        //Java:    const Int32 codePoint = mString.codePointBefore(shiftedOffset);
        const Int32 codePoint = (Int32)mString.GetChar(shiftedOffset-1);
        if (Character::IsLetterOrDigit(codePoint)){
            return TRUE;
        }
    }
    return FALSE;
}

Boolean WordIterator::IsOnLetterOrDigit(
    /* [in] */ Int32 shiftedOffset)
{
    if (shiftedOffset >= 0 && shiftedOffset < mString.GetLength()) {
        //Java:    final int codePoint = mString.codePointAt(shiftedOffset);
        const Int32 codePoint = (Int32)mString.GetChar(shiftedOffset-1);
        if (Character::IsLetterOrDigit(codePoint)){
            return TRUE;
        }
    }
    return FALSE;
}

void WordIterator::CheckOffsetIsValid(
    /* [in] */ Int32 shiftedOffset)
{
    if (shiftedOffset < 0 || shiftedOffset > mString.GetLength()) {
        /*
        throw new IllegalArgumentException("Invalid offset: " + (shiftedOffset + mOffsetShift) +
                ". Valid range is [" + mOffsetShift + ", " + (mString.length() + mOffsetShift) +
                "]");
        */
        StringBuilder sb("Invalid offset: ");
        sb += StringUtils::ToString(shiftedOffset + mOffsetShift);
        sb += ". Valid range is [";
        sb += StringUtils::ToString(mOffsetShift);
        sb += ", ";
        sb += StringUtils::ToString(mString.GetLength() + mOffsetShift);
        sb += "]";
        Logger::E(String("WordIterator"), sb.ToString());
    }
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
