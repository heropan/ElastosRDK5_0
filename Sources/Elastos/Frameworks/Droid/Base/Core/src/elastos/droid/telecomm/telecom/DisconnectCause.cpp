
#include "Elastos.Droid.Media.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/telecomm/telecom/DisconnectCause.h"
#include "elastos/droid/text/TextUtils.h"

using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Media::IToneGenerator;

namespace Elastos {
namespace Droid {
namespace Telecomm {
namespace Telecom {

//===============================================================
// DisconnectCause::
//===============================================================
CAR_INTERFACE_IMPL_2(DisconnectCause, Object, IDisconnectCause, IParcelable)

ECode DisconnectCause::constructor()
{
    return NOERROR;
}

ECode DisconnectCause::constructor(
    /* [in] */ Int32 code)
{
    return constructor(code, NULL, NULL, String(NULL), IToneGenerator::TONE_UNKNOWN);
}

ECode DisconnectCause::constructor(
    /* [in] */ Int32 code,
    /* [in] */ String reason)
{
    return constructor(code, NULL, NULL, reason, IToneGenerator::TONE_UNKNOWN);
}

ECode DisconnectCause::constructor(
    /* [in] */ Int32 code,
    /* [in] */ ICharSequence* label,
    /* [in] */ ICharSequence* description,
    /* [in] */ String reason)
{
    return constructor(code, label, description, reason, IToneGenerator::TONE_UNKNOWN);
}

ECode DisconnectCause::constructor(
    /* [in] */ Int32 code,
    /* [in] */ ICharSequence* label,
    /* [in] */ ICharSequence* description,
    /* [in] */ String reason,
    /* [in] */ Int32 toneToPlay)
{
    mDisconnectCode = code;
    mDisconnectLabel = label;
    mDisconnectDescription = description;
    mDisconnectReason = reason;
    mToneToPlay = toneToPlay;
    return NOERROR;
}

ECode DisconnectCause::GetCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code)
    *code = mDisconnectCode;
    return NOERROR;
}

ECode DisconnectCause::GetLabel(
    /* [out] */ ICharSequence** result)
{
    VALIDATE_NOT_NULL(result)
    *result = mDisconnectLabel;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode DisconnectCause::GetDescription(
    /* [out] */ ICharSequence** result)
{
    VALIDATE_NOT_NULL(result)
    *result = mDisconnectDescription;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode DisconnectCause::GetReason(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mDisconnectReason;
    return NOERROR;
}

ECode DisconnectCause::GetTone(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mToneToPlay;
    return NOERROR;
}

ECode DisconnectCause::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return NOERROR;
}

ECode DisconnectCause::WriteToParcel(
    /* [in] */ IParcel* destination)
{
    destination->WriteInt32(mDisconnectCode);
    TextUtils::WriteToParcel(mDisconnectLabel, destination);
    TextUtils::WriteToParcel(mDisconnectDescription, destination);
    destination->WriteString(mDisconnectReason);
    destination->WriteInt32(mToneToPlay);
    return NOERROR;
}

ECode DisconnectCause::GetHashCode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    Int32 hc = 0, res = 0;
    res = mDisconnectCode;
    IObject::Probe(mDisconnectLabel)->GetHashCode(&hc);
    res += hc;
    IObject::Probe(mDisconnectDescription)->GetHashCode(&hc);
    res += hc;
    res += mDisconnectReason.GetHashCode();
    res += mToneToPlay;
    *result = res;
    return NOERROR;
}

ECode DisconnectCause::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (IDisconnectCause::Probe(o) != NULL) {
        AutoPtr<IDisconnectCause> d = IDisconnectCause::Probe(o);
        Int32 dCode = 0;
        d->GetCode(&dCode);
        AutoPtr<ICharSequence> dLabel;
        d->GetLabel((ICharSequence**)&dLabel);
        AutoPtr<ICharSequence> dDescription;
        d->GetDescription((ICharSequence**)&dDescription);
        String dReason;
        d->GetReason(&dReason);
        Int32 dTone = 0;
        d->GetTone(&dTone);
        Boolean bReason = mDisconnectReason.Equals(dReason);
        *result = (mDisconnectCode == dCode)
                && Object::Equals(mDisconnectLabel, dLabel)
                && Object::Equals(mDisconnectDescription, dDescription)
                && bReason
                && (mToneToPlay == dTone);
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode DisconnectCause::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    String res("");
    Int32 code = 0;
    GetCode(&code);
    switch (code) {
        case IDisconnectCause::ERROR:
            res = String("ERROR");
            break;
        case IDisconnectCause::LOCAL:
            res = String("LOCAL");
            break;
        case IDisconnectCause::REMOTE:
            res = String("REMOTE");
            break;
        case IDisconnectCause::MISSED:
            res = String("MISSED");
            break;
        case IDisconnectCause::REJECTED:
            res = String("REJECTED");
            break;
        case IDisconnectCause::BUSY:
            res = String("BUSY");
            break;
        case IDisconnectCause::RESTRICTED:
            res = String("RESTRICTED");
            break;
        case IDisconnectCause::OTHER:
            res = String("OTHER");
            break;
        case IDisconnectCause::UNKNOWN:
        default:
            res = String("UNKNOWN");
    }
    String label("");
    if (mDisconnectLabel != NULL) {
        mDisconnectLabel->ToString(&label);
    }
    String description("");
    if (mDisconnectDescription != NULL) {
        mDisconnectDescription->ToString(&description);
    }
    String reason("");
    if (!mDisconnectReason.IsNull()) {
        reason = mDisconnectReason;
    }
    *result = "DisconnectCause [ Code: (";
    *result += res;
    *result += ")";
    *result += " Label: (";
    *result += label;
    *result += ")";
    *result += " Description: (";
    *result += description;
    *result += ")";
    *result += " Reason: (";
    *result += reason;
    *result += ")";
    *result += " Tone: (";
    *result += mToneToPlay;
    *result += ") ]";
    return NOERROR;
}

} // namespace Telecom
} // namespace Telecomm
} // namespace Droid
} // namespace Elastos