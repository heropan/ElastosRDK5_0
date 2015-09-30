
#include "elastos/droid/text/CAlteredCharSequenceHelper.h"
#include "elastos/droid/text/CAlteredCharSequence.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {

ECode CAlteredCharSequenceHelper::Make(
    /* [in] */ ICharSequence* source,
    /* [in] */ ArrayOf<Char32>* sub,
    /* [in] */ Int32 substart,
    /* [in] */ Int32 subend,
    /* [out] */ IAlteredCharSequence** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IAlteredCharSequence> acs = CAlteredCharSequence::Make(source, sub, substart, subend);
    *ret = acs;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos

