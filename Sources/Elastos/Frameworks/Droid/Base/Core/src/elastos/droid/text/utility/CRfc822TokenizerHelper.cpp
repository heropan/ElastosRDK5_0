#include "ext/frameworkdef.h"
#include "elastos/droid/text/utility/CRfc822TokenizerHelper.h"
#include "elastos/droid/text/utility/CRfc822Tokenizer.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Utility {

ECode CRfc822TokenizerHelper::Tokenize(
    /* [in] */ ICharSequence* text,
    /* [in, out] */ IObjectContainer* ret)
{
    return CRfc822Tokenizer::Tokenize(text, ret);
}

ECode CRfc822TokenizerHelper::Tokenize(
    /* [in] */ ICharSequence* text,
    /* [out, callee] */ ArrayOf<IRfc822Token*>** ret)
{
    VALIDATE_NOT_NULL(ret);
    return CRfc822Tokenizer::Tokenize(text, ret);
}

} // namespace Utility
} // namespace Text
} // namepsace Droid
} // namespace Elastos
