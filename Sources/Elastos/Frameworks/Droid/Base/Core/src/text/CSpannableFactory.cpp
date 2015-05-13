
#include "text/CSpannableFactory.h"
#include "text/CSpannableString.h"

namespace Elastos {
namespace Droid {
namespace Text {

ECode CSpannableFactory::NewSpannable(
    /* [in] */ ICharSequence* source,
    /* [out] */ ISpannable** spannable)
{
    VALIDATE_NOT_NULL(spannable);
    return CSpannableString::New(
        source, (ISpannableString**)spannable);
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
