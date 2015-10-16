#include "elastos/droid/text/style/CharacterStyle.h"
#include "elastos/droid/text/style/MetricAffectingSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

//===========================================================
// CharacterStyle
//===========================================================
CAR_INTERFACE_IMPL(CharacterStyle, Object, ICharacterStyle)

AutoPtr<ICharacterStyle> CharacterStyle::Wrap(
    /* [in] */ ICharacterStyle* cs)
{
    AutoPtr<IMetricAffectingSpan> mas = IMetricAffectingSpan::Probe(cs);
    if (mas != NULL) {
        AutoPtr<IMetricAffectingSpan> ret = new MetricAffectingSpan::Passthrough(mas);
        return ret;
    } else {
        AutoPtr<ICharacterStyle> ret = new Passthrough(cs);
        return ret;
    }
}

ECode CharacterStyle::GetUnderlying(
    /* [out] */ ICharacterStyle** style)
{
    VALIDATE_NOT_NULL(style)
    *style = THIS_PROBE(ICharacterStyle)
    REFCOUNT_ADD(*style);
    return NOERROR;
}

//===========================================================
// Passthrough
//===========================================================
Passthrough::Passthrough(
    /* [in] */ ICharacterStyle* cs)
{
    mStyle = cs;
}

ECode Passthrough::UpdateDrawState(
    /* [in] */ ITextPaint* tp)
{
    return mStyle->UpdateDrawState(tp);
}

ECode Passthrough::GetUnderlying(
    /* [out] */ ICharacterStyle** result)
{
    return mStyle->GetUnderlying(result);
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos