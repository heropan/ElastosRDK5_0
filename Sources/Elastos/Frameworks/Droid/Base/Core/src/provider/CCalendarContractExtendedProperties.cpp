
#include "provider/CCalendarContractExtendedProperties.h"

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CCalendarContractExtendedProperties::constructor()
{
    return NOERROR;
}

ECode CCalendarContractExtendedProperties::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return CalendarContractExtendedProperties::GetCONTENT_URI(uri);
}

} //Provider
} //Droid
} //Elastos