
#include "provider/CalendarContractReminders.h"
#include "net/Uri.h"
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

const String CalendarContractReminders::REMINDERS_WHERE = String(ICalendarContractRemindersColumns::EVENT_ID) + String("=?");

ECode CalendarContractReminders::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://") + ICalendarContract::AUTHORITY + String("/reminders"), uri);
}

ECode CalendarContractReminders::Query(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int64 eventId,
    /* [in] */ ArrayOf<String>* projection,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);

    AutoPtr<ArrayOf<String> > remArgs = ArrayOf<String>::Alloc(1);
    (*remArgs)[0] = StringUtils::ToString(eventId);
    AutoPtr<IUri> uri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&uri))
    return cr->Query(uri, projection, REMINDERS_WHERE, remArgs /*selection args*/, String(NULL) /* sort order */, cursor);
}

} //Provider
} //Droid
} //Elastos