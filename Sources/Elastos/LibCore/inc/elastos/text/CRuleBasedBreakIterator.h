
#ifndef __ELASTOS_TEXT_CRULEBASEBREAKITERATOR_H__
#define __ELASTOS_TEXT_CRULEBASEBREAKITERATOR_H__

#include "_Elastos_Text_CRuleBasedBreakIterator.h"
#include "RuleBasedBreakIterator.h"

using Libcore::ICU::INativeBreakIterator;
using Elastos::Utility::ILocale;
using Elastos::Text::ICharacterIterator;

namespace Elastos {
namespace Text {

CarClass(CRuleBasedBreakIterator) , public RuleBasedBreakIterator
{
public:

    CARAPI GetFirst(
        /* [out] */ Int32* postion);

    CARAPI GetFollowing(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* postion);

    CARAPI GetLast(
        /* [out] */ Int32* postion);

    CARAPI GetNext(
        /* [out] */ Int32* postion);

    CARAPI GetNext(
        /* [in] */ Int32 n,
        /* [out] */ Int32* postion);

    CARAPI GetPreceding(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* postion);

    CARAPI GetPrevious(
        /* [out] */ Int32* postion);

    CARAPI GetText(
        /* [out] */ ICharacterIterator** text);

    CARAPI IsBoundary(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean* isBoundary);

    CARAPI SetText(
        /* [in] */ const String& newText);

    CARAPI SetText(
        /* [in] */ ICharacterIterator* newText);

    CARAPI GetCurrent(
        /* [out] */ Int32 * value);

    CARAPI Clone(
        /* [out] */ IInterface ** outface);

    CARAPI constructor(
        /* [in] */ INativeBreakIterator * iterator);

};

} // namespace Text
} // namespace Elastos

#endif // __ELASTOS_TEXT_CRULEBASEBREAKITERATOR_H__
