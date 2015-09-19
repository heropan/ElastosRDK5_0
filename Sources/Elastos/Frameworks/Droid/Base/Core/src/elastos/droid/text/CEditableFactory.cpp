
#include "text/CEditableFactory.h"
#include "text/CSpannableStringBuilder.h"

namespace Elastos {
namespace Droid {
namespace Text {

ECode CEditableFactory::NewEditable(
    /* [in] */ ICharSequence* source,
    /* [out] */ IEditable** editable)
{
    VALIDATE_NOT_NULL(editable);
    return CSpannableStringBuilder::New(
        source, (ISpannableStringBuilder**)editable);
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
