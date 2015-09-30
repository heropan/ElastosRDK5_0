
#include "elastos/droid/text/CEditableFactory.h"
#include "elastos/droid/text/CSpannableStringBuilder.h"

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
