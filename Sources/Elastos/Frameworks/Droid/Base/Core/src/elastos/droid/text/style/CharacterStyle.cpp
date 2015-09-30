#include "elastos/droid/text/style/CharacterStyle.h"
#include "elastos/droid/text/style/MetricAffectingSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

/*****************************CharacterStyle::Passthrough*****************************/
PInterface CharacterStyle::Passthrough::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ICharacterStyle*)this;
    }
    else if (riid == EIID_ICharacterStyle) {
        return (ICharacterStyle*)this;
    }
    return NULL;
}

UInt32 CharacterStyle::Passthrough::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CharacterStyle::Passthrough::Release()
{
    return ElRefBase::Release();
}

ECode CharacterStyle::Passthrough::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (object == (IInterface*)(ICharacterStyle*)this) {
        *iID = EIID_ICharacterStyle;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CharacterStyle::Passthrough::Passthrough(
    /* [in] */ ICharacterStyle* cs)
{
    mStyle = cs;
}

ECode CharacterStyle::Passthrough::UpdateDrawState(
    /* [in] */ ITextPaint* tp)
{
    return mStyle->UpdateDrawState(tp);
}

ECode CharacterStyle::Passthrough::GetUnderlying(
    /* [out] */ ICharacterStyle** result)
{
    return mStyle->GetUnderlying(result);
}

/*****************************CharacterStyle*****************************/
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

AutoPtr<ICharacterStyle> CharacterStyle::GetUnderlying()
{
    AutoPtr<ICharacterStyle> cs = (ICharacterStyle*)this->Probe(EIID_ICharacterStyle);
    return cs;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos