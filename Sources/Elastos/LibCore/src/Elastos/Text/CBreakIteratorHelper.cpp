
#include "CBreakIteratorHelper.h"
#include "BreakIterator.h"

namespace Elastos {
namespace Text {

ECode CBreakIteratorHelper::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    return BreakIterator::GetAvailableLocales(locales);
}

ECode CBreakIteratorHelper::GetCharacterInstance(
    /* [out] */ IBreakIterator** instance)
{
    return BreakIterator::GetCharacterInstance(instance);
}

ECode CBreakIteratorHelper::GetCharacterInstanceEx(
    /* [in] */ ILocale* where,
    /* [out] */ IBreakIterator** instance)
{
    return BreakIterator::GetCharacterInstance(where,instance);
}

ECode CBreakIteratorHelper::GetLineInstance(
    /* [out] */ IBreakIterator** instance)
{
    return BreakIterator::GetLineInstance(instance);
}

ECode CBreakIteratorHelper::GetLineInstanceEx(
    /* [in] */ ILocale* where,
    /* [out] */ IBreakIterator** instance)
{
    return BreakIterator::GetLineInstance(where,instance);
}

ECode CBreakIteratorHelper::GetSentenceInstance(
    /* [out] */ IBreakIterator** instance)
{
    return BreakIterator::GetSentenceInstance(instance);
}

ECode CBreakIteratorHelper::GetSentenceInstanceEx(
    /* [in] */ ILocale* where,
    /* [out] */ IBreakIterator** instance)
{
    return BreakIterator::GetSentenceInstance(where,instance);
}

ECode CBreakIteratorHelper::GetWordInstance(
    /* [out] */ IBreakIterator** instance)
{
    return BreakIterator::GetWordInstance(instance);
}

ECode CBreakIteratorHelper::GetWordInstanceEx(
    /* [in] */ ILocale* where,
    /* [out] */ IBreakIterator** instance)
{
    return BreakIterator::GetWordInstance(where,instance);
}

} // namespace Text
} // namespace Elastos