
#ifndef __CRULEBASEBREAKITERATOR_H__
#define __CRULEBASEBREAKITERATOR_H__

#include "_CRuleBasedBreakIterator.h"
#include "RuleBasedBreakIterator.h"

using Libcore::ICU::ILocale;
using Libcore::ICU::INativeBreakIterator;
using Elastos::Text::ICharacterIterator;

namespace Elastos {
namespace Text {

CarClass(CRuleBasedBreakIterator) , public RuleBasedBreakIterator
{
public:

    CARAPI First(
        /* [out] */ Int32* postion);

    CARAPI Following(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* postion);

    CARAPI Last(
        /* [out] */ Int32* postion);

    CARAPI Next(
        /* [out] */ Int32* postion);

    CARAPI Next(
        /* [in] */ Int32 n,
        /* [out] */ Int32* postion);

    CARAPI Preceding(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* postion);

    CARAPI Previous(
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

    CARAPI Current(
        /* [out] */ Int32 * value);

    CARAPI Clone(
        /* [out] */ IInterface ** outface);

    CARAPI constructor(
        /* [in] */ INativeBreakIterator * iterator);

};

} // namespace Text
} // namespace Elastos

#endif // __CRULEBASEBREAKITERATOR_H__
