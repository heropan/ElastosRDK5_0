
#include "provider/CContactsContractCommonDataKindsEvent.h"
#include "R.h"
#include <elastos/coredef.h>

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractCommonDataKindsEvent::constructor()
{
    return NOERROR;
}

ECode CContactsContractCommonDataKindsEvent::GetTypeResource(
    /* [in] */ IInteger32* type,
    /* [out] */ Int32* resource)
{
    VALIDATE_NOT_NULL(resource);

    if (type == NULL) {
        *resource = Elastos::Droid::R::string::eventTypeOther;
        return NOERROR;
    }
    Int32 _type;
    FAIL_RETURN(type->GetValue(&_type))
    switch (_type) {
        case TYPE_ANNIVERSARY: *resource = Elastos::Droid::R::string::eventTypeAnniversary; return NOERROR;
        case TYPE_BIRTHDAY: *resource = Elastos::Droid::R::string::eventTypeBirthday; return NOERROR;
        case TYPE_OTHER: *resource = Elastos::Droid::R::string::eventTypeOther; return NOERROR;
        default: *resource = Elastos::Droid::R::string::eventTypeCustom; return NOERROR;
    }
     return NOERROR;
}

} //Provider
} //Droid
} //Elastos