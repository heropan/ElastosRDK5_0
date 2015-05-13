
#include "provider/CCalendarContractExtendedProperties.h"

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CCalendarContractExtendedProperties::constructor()
{
    return NOERROR;
}

ECode CCalendarContractExtendedProperties::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return CalendarContractExtendedProperties::GetCONTENTURI(uri);
}

} //Provider
} //Droid
} //Elastos