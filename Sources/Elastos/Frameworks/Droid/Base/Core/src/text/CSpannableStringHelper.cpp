#include "text/CSpannableStringHelper.h"
#include "text/CSpannableString.h"

namespace Elastos {
namespace Droid {
namespace Text {

ECode CSpannableStringHelper::ValueOf(
    /* [in] */ ICharSequence* source,
    /* [out] */ ISpannableString** spannedString)
{
    AutoPtr<ISpannableString> span = CSpannableString::ValueOf(source);
    *spannedString = span;
    INTERFACE_ADDREF(*spannedString);
    return NOERROR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
