#include "elastos/droid/text/style/MetricAffectingSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

/*****************************MetricAffectingSpan::Passthrough*****************************/
PInterface MetricAffectingSpan::Passthrough::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IMetricAffectingSpan*)this;
    }
    else if (riid == EIID_ICharacterStyle) {
        return (ICharacterStyle*)this;
    }
    else if (riid == EIID_IMetricAffectingSpan) {
        return (IMetricAffectingSpan*)this;
    }
    return NULL;
}

UInt32 MetricAffectingSpan::Passthrough::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MetricAffectingSpan::Passthrough::Release()
{
    return ElRefBase::Release();
}

ECode MetricAffectingSpan::Passthrough::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (object == (IInterface*)(IMetricAffectingSpan*)this) {
        *iID = EIID_IMetricAffectingSpan;
    }
    else if (object == (IInterface*)(ICharacterStyle*)this) {
        *iID = EIID_ICharacterStyle;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

MetricAffectingSpan::Passthrough::Passthrough(
    /* [in] */ IMetricAffectingSpan* cs)
{
    mStyle = cs;
}

ECode MetricAffectingSpan::Passthrough::UpdateDrawState(
    /* [in] */ ITextPaint* tp)
{
    return mStyle->UpdateDrawState(tp);
}

ECode MetricAffectingSpan::Passthrough::UpdateMeasureState(
    /* [in] */ ITextPaint* tp)
{
    return mStyle->UpdateMeasureState(tp);
}

ECode MetricAffectingSpan::Passthrough::GetUnderlying(
    /* [in] */ /*IMetricAffectingSpan*/ICharacterStyle** result)
{
    return mStyle->GetUnderlying(result);
}

/*****************************MetricAffectingSpan*****************************/
AutoPtr<IMetricAffectingSpan> MetricAffectingSpan::GetUnderlying()
{
    AutoPtr<IMetricAffectingSpan> mas = (IMetricAffectingSpan*)this;
    return mas;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos