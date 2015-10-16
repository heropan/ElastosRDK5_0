
#include "elastos/droid/provider/CContactsContractCommonDataKindsPhone.h"
#include "elastos/droid/provider/ContactsContractData.h"
#include "elastos/droid/net/Uri.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/R.h"

using Elastos::Droid::Net::Uri;
using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractCommonDataKindsPhone::constructor()
{
    return NOERROR;
}

ECode CContactsContractCommonDataKindsPhone::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContractData::GetCONTENT_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("phones"), uri);
}

ECode CContactsContractCommonDataKindsPhone::GetCONTENT_FILTER_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("filter"), uri);
}

ECode CContactsContractCommonDataKindsPhone::GetDisplayLabel(
    /* [in] */ IContext* context,
    /* [in] */ Int32 type,
    /* [in] */ ICharSequence* label,
    /* [in] */ const ArrayOf<ICharSequence*>& labelArray,
    /* [out] */ ICharSequence** lb)
{
    VALIDATE_NOT_NULL(lb);

    AutoPtr<IResources> res;
    FAIL_RETURN(context->GetResources((IResources**)&res))
    return GetTypeLabel(res, type, label, lb);
}

ECode CContactsContractCommonDataKindsPhone::GetDisplayLabel(
    /* [in] */ IContext* context,
    /* [in] */ Int32 type,
    /* [in] */ ICharSequence* label,
    /* [out] */ ICharSequence** lb)
{
    VALIDATE_NOT_NULL(lb);

    AutoPtr<IResources> res;
    FAIL_RETURN(context->GetResources((IResources**)&res))
    return GetTypeLabel(res, type, label, lb);
}

ECode CContactsContractCommonDataKindsPhone::GetTypeLabelResource(
    /* [in] */ Int32 type,
    /* [out] */ Int32* resource)
{
    VALIDATE_NOT_NULL(resource);

    switch (type) {
        case TYPE_HOME: *resource = Elastos::Droid::R::string::phoneTypeHome; return NOERROR;
        case TYPE_MOBILE: *resource = Elastos::Droid::R::string::phoneTypeMobile; return NOERROR;
        case TYPE_WORK: *resource = Elastos::Droid::R::string::phoneTypeWork; return NOERROR;
        case TYPE_FAX_WORK: *resource = Elastos::Droid::R::string::phoneTypeFaxWork; return NOERROR;
        case TYPE_FAX_HOME: *resource = Elastos::Droid::R::string::phoneTypeFaxHome; return NOERROR;
        case TYPE_PAGER: *resource = Elastos::Droid::R::string::phoneTypePager; return NOERROR;
        case TYPE_OTHER: *resource = Elastos::Droid::R::string::phoneTypeOther; return NOERROR;
        case TYPE_CALLBACK: *resource = Elastos::Droid::R::string::phoneTypeCallback; return NOERROR;
        case TYPE_CAR: *resource = Elastos::Droid::R::string::phoneTypeCar; return NOERROR;
        case TYPE_COMPANY_MAIN: *resource = Elastos::Droid::R::string::phoneTypeCompanyMain; return NOERROR;
        case TYPE_ISDN: *resource = Elastos::Droid::R::string::phoneTypeIsdn; return NOERROR;
        case TYPE_MAIN: *resource = Elastos::Droid::R::string::phoneTypeMain; return NOERROR;
        case TYPE_OTHER_FAX: *resource = Elastos::Droid::R::string::phoneTypeOtherFax; return NOERROR;
        case TYPE_RADIO: *resource = Elastos::Droid::R::string::phoneTypeRadio; return NOERROR;
        case TYPE_TELEX: *resource = Elastos::Droid::R::string::phoneTypeTelex; return NOERROR;
        case TYPE_TTY_TDD: *resource = Elastos::Droid::R::string::phoneTypeTtyTdd; return NOERROR;
        case TYPE_WORK_MOBILE: *resource = Elastos::Droid::R::string::phoneTypeWorkMobile; return NOERROR;
        case TYPE_WORK_PAGER: *resource = Elastos::Droid::R::string::phoneTypeWorkPager; return NOERROR;
        case TYPE_ASSISTANT: *resource = Elastos::Droid::R::string::phoneTypeAssistant; return NOERROR;
        case TYPE_MMS: *resource = Elastos::Droid::R::string::phoneTypeMms; return NOERROR;
        default: *resource = Elastos::Droid::R::string::phoneTypeCustom; return NOERROR;
    }
     return NOERROR;
}

/**
 * Return a {@link CharSequence} that best describes the given type,
 * possibly substituting the given {@link #LABEL} value
 * for {@link #TYPE_CUSTOM}.
 */
ECode CContactsContractCommonDataKindsPhone::GetTypeLabel(
    /* [in] */ IResources* res,
    /* [in] */ Int32 type,
    /* [in] */ ICharSequence* label,
    /* [out] */ ICharSequence** lb)
{
    VALIDATE_NOT_NULL(lb);

    if ((type == TYPE_CUSTOM || type == TYPE_ASSISTANT) && !TextUtils::IsEmpty(label)) {
        *lb = label;
        REFCOUNT_ADD(*lb);
        return NOERROR;
    } else {
        Int32 labelRes;
        FAIL_RETURN(GetTypeLabelResource(type, &labelRes))
        return res->GetText(labelRes, lb);
    }
}

} //Provider
} //Droid
} //Elastos
