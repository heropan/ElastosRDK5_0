
#include "provider/CCalendarContractCalendars.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

static AutoPtr<ArrayOf<String> > initSYNCWRITABLECOLUMNS()
{
    AutoPtr<ArrayOf<String> > args = ArrayOf<String>::Alloc(24);

    (*args)[0] = ICalendarContractSyncColumns::ACCOUNT_NAME;
    (*args)[1] = ICalendarContractSyncColumns::ACCOUNT_TYPE;
    (*args)[2] = ICalendarContractSyncColumns::_SYNC_ID;
    (*args)[3] = ICalendarContractSyncColumns::DIRTY;
    (*args)[4] = ICalendarContractCalendarColumns::OWNER_ACCOUNT;
    (*args)[5] = ICalendarContractCalendarColumns::MAX_REMINDERS;
    (*args)[6] = ICalendarContractCalendarColumns::ALLOWED_REMINDERS;
    (*args)[7] = ICalendarContractCalendarColumns::CAN_MODIFY_TIME_ZONE;
    (*args)[8] = ICalendarContractCalendarColumns::CAN_ORGANIZER_RESPOND;
    (*args)[9] = ICalendarContractSyncColumns::CAN_PARTIALLY_UPDATE;
    (*args)[10] = ICalendarContractCalendars::CALENDAR_LOCATION;
    (*args)[11] = ICalendarContractCalendarColumns::CALENDAR_TIME_ZONE;
    (*args)[12] = ICalendarContractCalendarColumns::CALENDAR_ACCESS_LEVEL;
    (*args)[13] = ICalendarContractSyncColumns::DELETED;
    (*args)[14] = ICalendarContractCalendarSyncColumns::CAL_SYNC1;
    (*args)[15] = ICalendarContractCalendarSyncColumns::CAL_SYNC2;
    (*args)[16] = ICalendarContractCalendarSyncColumns::CAL_SYNC3;
    (*args)[17] = ICalendarContractCalendarSyncColumns::CAL_SYNC4;
    (*args)[18] = ICalendarContractCalendarSyncColumns::CAL_SYNC5;
    (*args)[19] = ICalendarContractCalendarSyncColumns::CAL_SYNC6;
    (*args)[20] = ICalendarContractCalendarSyncColumns::CAL_SYNC7;
    (*args)[21] = ICalendarContractCalendarSyncColumns::CAL_SYNC8;
    (*args)[22] = ICalendarContractCalendarSyncColumns::CAL_SYNC9;
    (*args)[23] = ICalendarContractCalendarSyncColumns::CAL_SYNC10;

    return args;
}

AutoPtr<ArrayOf<String> > CCalendarContractCalendars::SYNC_WRITABLE_COLUMNS = initSYNCWRITABLECOLUMNS();

ECode CCalendarContractCalendars::constructor()
{
    return NOERROR;
}

ECode CCalendarContractCalendars::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://") + ICalendarContract::AUTHORITY + String("/calendars"), uri);
}

ECode CCalendarContractCalendars::GetSYNCWRITABLECOLUMNS(
    /* [out, callee] */ ArrayOf<String>** columns)
{
    VALIDATE_NOT_NULL(columns);

    *columns = SYNC_WRITABLE_COLUMNS;
    REFCOUNT_ADD(*columns);
    return NOERROR;
}

} //Provider
} //Droid
} //Elastos