
#include "provider/CalendarContractAttendees.h"
#include "net/Uri.h"
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

String CalendarContractAttendees::ATTENDEES_WHERE = String(ICalendarContractAttendeesColumns::EVENT_ID) + String("=?");

ECode CalendarContractAttendees::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://") + ICalendarContract::AUTHORITY + String("/attendees"), uri);
}

ECode CalendarContractAttendees::Query(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int64 eventId,
    /* [in] */ ArrayOf<String>* projection,
    /* [out] */ ICursor** cursor)
{
    AutoPtr<ArrayOf<String> > attArgs = ArrayOf<String>::Alloc(1);
    (*attArgs)[0] = StringUtils::Int64ToString(eventId);
    AutoPtr<IUri> _uri;
    FAIL_RETURN(GetCONTENTURI((IUri**)&_uri))
    return cr->Query(_uri, projection, ATTENDEES_WHERE, attArgs /* selection args */, String(NULL) /* sort order */, cursor);
}

} //Provider
} //Droid
} //Elastos