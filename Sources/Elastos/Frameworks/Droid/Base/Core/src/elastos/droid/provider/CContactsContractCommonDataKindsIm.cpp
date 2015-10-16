
#include "elastos/droid/provider/CContactsContractCommonDataKindsIm.h"
#include "elastos/droid/text/TextUtils.h"
#include <elastos/coredef.h>
#include "elastos/droid/R.h"

using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractCommonDataKindsIm::constructor()
{
    return NOERROR;
}

ECode CContactsContractCommonDataKindsIm::GetTypeLabelResource(
    /* [in] */ Int32 type,
    /* [out] */ Int32* resource)
{
    VALIDATE_NOT_NULL(resource);

    switch (type) {
        case TYPE_HOME: *resource = Elastos::Droid::R::string::imTypeHome; return NOERROR;
        case TYPE_WORK: *resource = Elastos::Droid::R::string::imTypeWork; return NOERROR;
        case TYPE_OTHER: *resource = Elastos::Droid::R::string::imTypeOther; return NOERROR;
        default: *resource = Elastos::Droid::R::string::imTypeCustom; return NOERROR;
    }
     return NOERROR;
}

ECode CContactsContractCommonDataKindsIm::GetTypeLabel(
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

ECode CContactsContractCommonDataKindsIm::GetProtocolLabelResource(
    /* [in] */ Int32 type,
    /* [out] */ Int32* resource)
{
    VALIDATE_NOT_NULL(resource);

    switch (type) {
        case PROTOCOL_AIM: *resource = Elastos::Droid::R::string::imProtocolAim; return NOERROR;
        case PROTOCOL_MSN: *resource = Elastos::Droid::R::string::imProtocolMsn; return NOERROR;
        case PROTOCOL_YAHOO: *resource = Elastos::Droid::R::string::imProtocolYahoo; return NOERROR;
        case PROTOCOL_SKYPE: *resource = Elastos::Droid::R::string::imProtocolSkype; return NOERROR;
        case PROTOCOL_QQ: *resource = Elastos::Droid::R::string::imProtocolQq; return NOERROR;
        case PROTOCOL_GOOGLE_TALK: *resource = Elastos::Droid::R::string::imProtocolGoogleTalk; return NOERROR;
        case PROTOCOL_ICQ: *resource = Elastos::Droid::R::string::imProtocolIcq; return NOERROR;
        case PROTOCOL_JABBER: *resource = Elastos::Droid::R::string::imProtocolJabber; return NOERROR;
        case PROTOCOL_NETMEETING: *resource = Elastos::Droid::R::string::imProtocolNetMeeting; return NOERROR;
        default: *resource = Elastos::Droid::R::string::imProtocolCustom; return NOERROR;
    }
    return NOERROR;
}

ECode CContactsContractCommonDataKindsIm::GetProtocolLabel(
    /* [in] */ IResources* res,
    /* [in] */ Int32 type,
    /* [in] */ ICharSequence* label,
    /* [out] */ ICharSequence** lb)
{
    VALIDATE_NOT_NULL(lb);

    if (type == PROTOCOL_CUSTOM && !TextUtils::IsEmpty(label)) {
        *lb = label;
        REFCOUNT_ADD(*lb);
        return NOERROR;
    } else {
        Int32 labelRes;
        FAIL_RETURN(GetProtocolLabelResource(type, &labelRes))
        return res->GetText(labelRes, lb);
    }
    return NOERROR;
}

} //Provider
} //Droid
} //Elastos