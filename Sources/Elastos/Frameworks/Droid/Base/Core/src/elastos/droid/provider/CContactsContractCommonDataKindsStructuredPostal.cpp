
#include "provider/CContactsContractCommonDataKindsStructuredPostal.h"
#include "provider/ContactsContractData.h"
#include "net/Uri.h"
#include "text/TextUtils.h"
#include "R.h"

using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractCommonDataKindsStructuredPostal::constructor()
{
    return NOERROR;
}

ECode CContactsContractCommonDataKindsStructuredPostal::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> _uri;
    FAIL_RETURN(ContactsContractData::GetCONTENT_URI((IUri**)&_uri))
    return Uri::WithAppendedPath(_uri, String("postals"), uri);
}

ECode CContactsContractCommonDataKindsStructuredPostal::GetTypeLabelResource(
    /* [in] */ Int32 type,
    /* [out] */ Int32* resource)
{
    VALIDATE_NOT_NULL(resource);

    switch (type) {
        case TYPE_HOME: *resource = Elastos::Droid::R::string::postalTypeHome; return NOERROR;
        case TYPE_WORK: *resource = Elastos::Droid::R::string::postalTypeWork; return NOERROR;
        case TYPE_OTHER: *resource = Elastos::Droid::R::string::postalTypeOther; return NOERROR;
        default: *resource = Elastos::Droid::R::string::postalTypeCustom; return NOERROR;
    }
    return NOERROR;
}

ECode CContactsContractCommonDataKindsStructuredPostal::GetTypeLabel(
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