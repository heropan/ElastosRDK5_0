#include "elastos/droid/text/style/ClickableSpan.h"
#include "elastos/droid/text/CTextPaint.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CAR_INTERFACE_IMPL(ClickableSpan, CharacterStyle, IClickableSpan)

ECode ClickableSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    VALIDATE_NOT_NULL(ds);
    Int32 linkColor;
    ds->SetColor((ds->GetLinkColor(&linkColor), linkColor));
    ds->SetUnderlineText(TRUE);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos