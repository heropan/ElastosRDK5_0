#include "elastos/droid/text/CSpannableStringHelper.h"
#include "elastos/droid/text/CSpannableString.h"

namespace Elastos {
namespace Droid {
namespace Text {

ECode CSpannableStringHelper::ValueOf(
    /* [in] */ ICharSequence* source,
    /* [out] */ ISpannableString** spannedString)
{
    AutoPtr<ISpannableString> span = CSpannableString::ValueOf(source);
    *spannedString = span;
    REFCOUNT_ADD(*spannedString);
    return NOERROR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
