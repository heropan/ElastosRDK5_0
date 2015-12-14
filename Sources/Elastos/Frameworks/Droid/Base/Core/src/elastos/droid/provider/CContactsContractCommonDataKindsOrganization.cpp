#include "elastos/droid/provider/CContactsContractCommonDataKindsOrganization.h"
#include "elastos/droid/R.h"
#include "elastos/droid/text/TextUtils.h"
// #include <elastos/coredef.h>

using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace Provider {

CAR_SINGLETON_IMPL(CContactsContractCommonDataKindsOrganization)

CAR_INTERFACE_IMPL_3(CContactsContractCommonDataKindsOrganization, Singleton
    , IContactsContractCommonDataKindsOrganization
    , IContactsContractDataColumnsWithJoins
    , IContactsContractCommonDataKindsCommonColumns)

ECode CContactsContractCommonDataKindsOrganization::GetTypeLabelResource(
    /* [in] */ Int32 type,
    /* [out] */ Int32* resource)
{
    VALIDATE_NOT_NULL(resource);

    switch (type) {
        case TYPE_WORK:
            *resource = Elastos::Droid::R::string::orgTypeWork;
            break;
        case TYPE_OTHER:
            *resource = Elastos::Droid::R::string::orgTypeOther;
            break;
        default:
            *resource = Elastos::Droid::R::string::orgTypeCustom;
            break;
    }
    return NOERROR;
}

ECode CContactsContractCommonDataKindsOrganization::GetTypeLabel(
    /* [in] */ IResources* res,
    /* [in] */ Int32 type,
    /* [in] */ ICharSequence* label,
    /* [out] */ ICharSequence** lb)
{
    VALIDATE_NOT_NULL(lb);

    if (type == IContactsContractCommonDataKindsBaseTypes::TYPE_CUSTOM && !TextUtils::IsEmpty(label)) {
        *lb = label;
        REFCOUNT_ADD(*lb);
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