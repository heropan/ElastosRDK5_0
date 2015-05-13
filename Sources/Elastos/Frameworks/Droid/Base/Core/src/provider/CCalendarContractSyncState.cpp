
#include "provider/CCalendarContractSyncState.h"
#include "provider/CalendarContract.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;
using Elastos::Droid::Provider::ISyncStateContractConstants;

namespace Elastos {
namespace Droid {
namespace Provider {

const String CCalendarContractSyncState::CONTENT_DIRECTORY = ISyncStateContractConstants::CONTENT_DIRECTORY;

ECode CCalendarContractSyncState::constructor()
{
    return NOERROR;
}

ECode CCalendarContractSyncState::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> _uri;
    FAIL_RETURN(CalendarContract::GetCONTENTURI((IUri**)&_uri))
    return Uri::WithAppendedPath(_uri, CONTENT_DIRECTORY, uri);
}

} //Provider
} //Droid
} //Elastos