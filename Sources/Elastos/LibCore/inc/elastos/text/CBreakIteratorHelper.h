
#ifndef __ELASTOS_TEXT_CBREAKITERATORHELPER_H__
#define __ELASTOS_TEXT_CBREAKITERATORHELPER_H__

#include "_Elastos_Text_CBreakIteratorHelper.h"

using Elastos::Utility::ILocale;

namespace Elastos {
namespace Text {

CarClass(CBreakIteratorHelper)
{
public:

    CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetCharacterInstance(
        /* [out] */ IBreakIterator** instance);

    CARAPI GetCharacterInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);

    CARAPI GetLineInstance(
        /* [out] */ IBreakIterator** instance);

    CARAPI GetLineInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);

    CARAPI GetSentenceInstance(
        /* [out] */ IBreakIterator** instance);

    CARAPI GetSentenceInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);

    CARAPI GetWordInstance(
        /* [out] */ IBreakIterator** instance);

    CARAPI GetWordInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);
};

} // namespace Text
} // namespace Elastos

#endif // __ELASTOS_TEXT_CBREAKITERATORHELPER_H__
