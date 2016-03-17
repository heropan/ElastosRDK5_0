
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/telecomm/telecom/PhoneAccountHandle.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Telecomm {
namespace Telecom {

//===============================================================
// PhoneAccountHandle::
//===============================================================
CAR_INTERFACE_IMPL_2(PhoneAccountHandle, Object, IPhoneAccountHandle, IParcelable)

ECode PhoneAccountHandle::constructor(
    /* [in] */ IComponentName* componentName,
    /* [in] */const String& id)
{
    mComponentName = componentName;
    mId = id;
    return NOERROR;
}

ECode PhoneAccountHandle::GetComponentName(
    /* [out] */ IComponentName** result)
{
    VALIDATE_NOT_NULL(result)
    *result = mComponentName;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode PhoneAccountHandle::GetId(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mId;
    return NOERROR;
}

ECode PhoneAccountHandle::GetHashCode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    Int32 hc = 0;
    IObject::Probe(mComponentName)->GetHashCode(&hc);
    Int32 idHc = mId.GetHashCode();
    *result = hc + idHc;
    return NOERROR;
}

ECode PhoneAccountHandle::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    StringBuilder sb;
    sb.Append(mComponentName);
    sb.Append(", ");
    sb.Append(mId);
    return sb.ToString(result);
}

ECode PhoneAccountHandle::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    if (other != NULL) {
        if (IPhoneAccountHandle::Probe(other) != NULL) {
            AutoPtr<IPhoneAccountHandle> pOther = IPhoneAccountHandle::Probe(other);
            AutoPtr<IComponentName> otherCN, conName;
            pOther->GetComponentName((IComponentName**)&otherCN);
            GetComponentName((IComponentName**)&conName);
            String otherId, id;
            pOther->GetId(&otherId);
            GetId(&id);
            if (Object::Equals(otherCN, conName) && otherId.Equals(id)) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }
    *result = FALSE;
    return NOERROR;
}

ECode PhoneAccountHandle::WriteToParcel(
    /* [in] */ IParcel* out)
{
    IParcelable::Probe(mComponentName)->WriteToParcel(out);
    out->WriteString(mId);
    return NOERROR;
}

ECode PhoneAccountHandle::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    IParcelable::Probe(mComponentName)->ReadFromParcel(in);
    in->ReadString(&mId);
    return NOERROR;
}

} // namespace Telecom
} // namespace Telecomm
} // namespace Droid
} // namespace Elastos