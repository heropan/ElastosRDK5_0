
#ifndef __ICU_CNATIVEBREAKITERATORHELPER_H__
#define __ICU_CNATIVEBREAKITERATORHELPER_H__

#include "_CNativeBreakIteratorHelper.h"

namespace Libcore {
namespace ICU {

CarClass(CNativeBreakIteratorHelper)
{
public:
    CARAPI GetCharacterInstance(
        /* [in] */ ILocale * where,
        /* [out] */ INativeBreakIterator ** outiter);

    CARAPI GetLineInstance(
        /* [in] */ ILocale * where,
        /* [out] */ INativeBreakIterator ** outiter);

    CARAPI GetSentenceInstance(
        /* [in] */ ILocale * where,
        /* [out] */ INativeBreakIterator ** outiter);

    CARAPI GetWordInstance(
        /* [in] */ ILocale * where,
        /* [out] */ INativeBreakIterator ** outiter);
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_CNATIVEBREAKITERATORHELPER_H__

