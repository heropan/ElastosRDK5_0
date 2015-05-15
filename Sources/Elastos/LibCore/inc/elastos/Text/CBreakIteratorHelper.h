
#ifndef __CBREAKITERATORHELPER_H__
#define __CBREAKITERATORHELPER_H__

#include "_CBreakIteratorHelper.h"

using Libcore::ICU::ILocale;

namespace Elastos {
namespace Text {

CarClass(CBreakIteratorHelper)
{
public:

    CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetCharacterInstance(
        /* [out] */ IBreakIterator** instance);

    CARAPI GetCharacterInstanceEx(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);

    CARAPI GetLineInstance(
        /* [out] */ IBreakIterator** instance);

    CARAPI GetLineInstanceEx(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);

    CARAPI GetSentenceInstance(
        /* [out] */ IBreakIterator** instance);

    CARAPI GetSentenceInstanceEx(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);

    CARAPI GetWordInstance(
        /* [out] */ IBreakIterator** instance);

    CARAPI GetWordInstanceEx(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);
};

} // namespace Text
} // namespace Elastos

#endif // __CBREAKITERATORHELPER_H__
