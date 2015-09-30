
#include "elastos/droid/text/style/CSpellCheckSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

IPARCELABLESPAN_METHODS_IMPL(CSpellCheckSpan, SpellCheckSpan, SpellCheckSpan)

ECode CSpellCheckSpan::SetSpellCheckInProgress(
    /* [in] */ Boolean inProgress)
{
    SpellCheckSpan::SetSpellCheckInProgress(inProgress);
    return NOERROR;
}

ECode CSpellCheckSpan::IsSpellCheckInProgress(
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=SpellCheckSpan::IsSpellCheckInProgress();
    return NOERROR;
}

ECode CSpellCheckSpan::constructor()
{
    Init();
    return NOERROR;
}

ECode CSpellCheckSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

