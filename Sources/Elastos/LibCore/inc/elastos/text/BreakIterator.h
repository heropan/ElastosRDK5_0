#ifndef __ELASTOS_TEXT_BREAKITERATOR_H__
#define __ELASTOS_TEXT_BREAKITERATOR_H__

#include <elastos/core/Object.h>

using Libcore::ICU::INativeBreakIterator;
using Elastos::Utility::ILocale;
using Elastos::Text::ICharacterIterator;

namespace Elastos {
namespace Text {

class BreakIterator : public Object {
public:

    static CARAPI GetAvailableLocales(
        /* [out] */ ArrayOf<ILocale*>** locales);

    static CARAPI GetCharacterInstance(
        /* [out] */ IBreakIterator** instance);

    static CARAPI GetCharacterInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);

    static CARAPI GetLineInstance(
        /* [out] */ IBreakIterator** instance);

    static CARAPI GetLineInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);

    static CARAPI GetSentenceInstance(
        /* [out] */ IBreakIterator** instance);

    static CARAPI GetSentenceInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);

    static CARAPI GetWordInstance(
        /* [out] */ IBreakIterator** instance);

    static CARAPI GetWordInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);

    virtual CARAPI IsBoundary(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean* isBoundary);

    virtual CARAPI Preceding(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* position);

    virtual CARAPI SetText(
        /* [in] */ const String& newText);

    virtual CARAPI Current(
        /* [out] */ Int32* position) = 0;

    virtual CARAPI First(
        /* [out] */ Int32* position) = 0;

    virtual CARAPI Following(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* position) = 0;

    virtual CARAPI GetText(
        /* [out] */ ICharacterIterator** text) = 0;

    virtual CARAPI Last(
        /* [out] */ Int32* position) = 0;

    virtual CARAPI GetNext(
        /* [out] */ Int32* position) = 0;

    virtual CARAPI GetNext(
        /* [in] */ Int32 n,
        /* [out] */ Int32* position) = 0;

    virtual CARAPI Previous(
        /* [out] */ Int32* position) = 0;

    virtual CARAPI SetText(
        /* [in] */ ICharacterIterator* newText) = 0;

protected:
    BreakIterator();

    BreakIterator(
        /* [in] */ INativeBreakIterator* iterator);

public:
    AutoPtr<INativeBreakIterator> mWrapped;
};

} // namespace Text
} // namespace Elastos

#endif //__ELASTOS_TEXT_BREAKITERATOR_H__
