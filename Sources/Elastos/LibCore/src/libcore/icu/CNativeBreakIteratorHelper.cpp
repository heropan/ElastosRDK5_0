
#include "cmdef.h"
#include "CNativeBreakIteratorHelper.h"
#include "NativeBreakIterator.h"

namespace Libcore {
namespace ICU {

ECode CNativeBreakIteratorHelper::GetCharacterInstance(
    /* [in] */ ILocale * where,
    /* [out] */ INativeBreakIterator ** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    AutoPtr<INativeBreakIterator> nbi = NativeBreakIterator::GetCharacterInstance(where);
    *outiter = nbi;
    INTERFACE_ADDREF(*outiter);
    return NOERROR;
}

ECode CNativeBreakIteratorHelper::GetLineInstance(
    /* [in] */ ILocale * where,
    /* [out] */ INativeBreakIterator ** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    AutoPtr<INativeBreakIterator> nbi = NativeBreakIterator::GetLineInstance(where);
    *outiter = nbi;
    INTERFACE_ADDREF(*outiter);
    return NOERROR;
}

ECode CNativeBreakIteratorHelper::GetSentenceInstance(
    /* [in] */ ILocale * where,
    /* [out] */ INativeBreakIterator ** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    AutoPtr<INativeBreakIterator> nbi = NativeBreakIterator::GetSentenceInstance(where);
    *outiter = nbi;
    INTERFACE_ADDREF(*outiter);
    return NOERROR;
}

ECode CNativeBreakIteratorHelper::GetWordInstance(
    /* [in] */ ILocale * where,
    /* [out] */ INativeBreakIterator ** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    AutoPtr<INativeBreakIterator> nbi = NativeBreakIterator::GetWordInstance(where);
    *outiter = nbi;
    INTERFACE_ADDREF(*outiter);
    return NOERROR;
}

} // namespace ICU
} // namespace Libcore
