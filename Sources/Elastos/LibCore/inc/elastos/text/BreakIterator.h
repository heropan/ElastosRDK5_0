#ifndef __BREAKITERATOR_H__
#define __BREAKITERATOR_H__

#include "cmdef.h"
#include "Elastos.CoreLibrary_server.h"
#include <elastos.h>

using Libcore::ICU::ILocale;
using Libcore::ICU::INativeBreakIterator;
using Elastos::Text::ICharacterIterator;

namespace Elastos {
namespace Text {

class BreakIterator {
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

    virtual CARAPI Next(
        /* [out] */ Int32* position) = 0;

    virtual CARAPI Next(
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

#endif //__BREAKITERATOR_H__
