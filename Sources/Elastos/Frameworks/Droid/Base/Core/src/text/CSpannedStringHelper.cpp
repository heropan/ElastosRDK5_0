#include "text/CSpannedStringHelper.h"
#include "text/CSpannedString.h"

namespace Elastos {
namespace Droid {
namespace Text {

ECode CSpannedStringHelper::ValueOf(
    /* [in] */ ICharSequence* source,
    /* [out] */ ISpannedString** spannedString)
{
    AutoPtr<ISpannedString> span = CSpannedString::ValueOf(source);
    *spannedString = span;
    REFCOUNT_ADD(*spannedString);
    return NOERROR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
