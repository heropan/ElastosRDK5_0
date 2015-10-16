
#include "elastos/droid/provider/CContactsContractCommonDataKindsRelation.h"
#include "elastos/droid/text/TextUtils.h"
#include <elastos/coredef.h>
#include "elastos/droid/R.h"

using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractCommonDataKindsRelation::constructor()
{
    return NOERROR;
}

ECode CContactsContractCommonDataKindsRelation::GetTypeLabelResource(
    /* [in] */ Int32 type,
    /* [out] */ Int32* resource)
{
    VALIDATE_NOT_NULL(resource);

    switch (type) {
        case TYPE_ASSISTANT: *resource = Elastos::Droid::R::string::relationTypeAssistant; return NOERROR;
        case TYPE_BROTHER: *resource = Elastos::Droid::R::string::relationTypeBrother; return NOERROR;
        case TYPE_CHILD: *resource = Elastos::Droid::R::string::relationTypeChild; return NOERROR;
        case TYPE_DOMESTIC_PARTNER: *resource = Elastos::Droid::R::string::relationTypeDomesticPartner; return NOERROR;
        case TYPE_FATHER: *resource = Elastos::Droid::R::string::relationTypeFather; return NOERROR;
        case TYPE_FRIEND: *resource = Elastos::Droid::R::string::relationTypeFriend; return NOERROR;
        case TYPE_MANAGER: *resource = Elastos::Droid::R::string::relationTypeManager; return NOERROR;
        case TYPE_MOTHER: *resource = Elastos::Droid::R::string::relationTypeMother; return NOERROR;
        case TYPE_PARENT: *resource = Elastos::Droid::R::string::relationTypeParent; return NOERROR;
        case TYPE_PARTNER: *resource = Elastos::Droid::R::string::relationTypePartner; return NOERROR;
        case TYPE_REFERRED_BY: *resource = Elastos::Droid::R::string::relationTypeReferredBy; return NOERROR;
        case TYPE_RELATIVE: *resource = Elastos::Droid::R::string::relationTypeRelative; return NOERROR;
        case TYPE_SISTER: *resource = Elastos::Droid::R::string::relationTypeSister; return NOERROR;
        case TYPE_SPOUSE: *resource = Elastos::Droid::R::string::relationTypeSpouse; return NOERROR;
        default: *resource = Elastos::Droid::R::string::orgTypeCustom; return NOERROR;
    }
     return NOERROR;
}

ECode CContactsContractCommonDataKindsRelation::GetTypeLabel(
    /* [in] */ IResources* res,
    /* [in] */ Int32 type,
    /* [in] */ ICharSequence* label,
    /* [out] */ ICharSequence** lb)
{
    VALIDATE_NOT_NULL(lb);

    if (type == TYPE_CUSTOM && !TextUtils::IsEmpty(label)) {
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