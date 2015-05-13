
#include "provider/CContactsContractCommonDataKindsSipAddress.h"
#include "text/TextUtils.h"
#include "cmdef.h"
#include "R.h"

using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractCommonDataKindsSipAddress::constructor()
{
    return NOERROR;
}

ECode CContactsContractCommonDataKindsSipAddress::GetTypeLabelResource(
    /* [in] */ Int32 type,
    /* [out] */ Int32* resource)
{
    VALIDATE_NOT_NULL(resource);

    switch (type) {
        case TYPE_HOME: *resource = Elastos::Droid::R::string::sipAddressTypeHome; return NOERROR;
        case TYPE_WORK: *resource = Elastos::Droid::R::string::sipAddressTypeWork; return NOERROR;
        case TYPE_OTHER: *resource = Elastos::Droid::R::string::sipAddressTypeOther; return NOERROR;
        default: *resource = Elastos::Droid::R::string::sipAddressTypeCustom; return NOERROR;
    }
     return NOERROR;
}

ECode CContactsContractCommonDataKindsSipAddress::GetTypeLabel(
    /* [in] */ IResources* res,
    /* [in] */ Int32 type,
    /* [in] */ ICharSequence* label,
    /* [out] */ ICharSequence** lb)
{
    VALIDATE_NOT_NULL(lb);

    if (type == TYPE_CUSTOM && !TextUtils::IsEmpty(label)) {
        *lb = label;
        INTERFACE_ADDREF(*lb);
        return NOERROR;
    } else {
        Int32 labelRes;
        FAIL_RETURN(GetTypeLabelResource(type, &labelRes))
        return res->GetText(labelRes, lb);
    }
    return NOERROR;
}

} //Provider
} //Droid
} //Elastos