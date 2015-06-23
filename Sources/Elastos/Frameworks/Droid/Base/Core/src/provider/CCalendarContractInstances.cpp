
#include "provider/CCalendarContractInstances.h"
#include "content/CContentUris.h"
#include "net/Uri.h"

using Elastos::Droid::Net::Uri;
using Elastos::Droid::Content::IContentUris;
using Elastos::Droid::Content::CContentUris;

namespace Elastos {
namespace Droid {
namespace Provider {

static AutoPtr<ArrayOf<String> > initWHERECALENDARSARGS()
{
    AutoPtr<ArrayOf<String> > args = ArrayOf<String>::Alloc(1);
    (*args)[0] = String("1");
    return args;
}

const String CCalendarContractInstances::WHERE_CALENDARS_SELECTED = ICalendarContractCalendarColumns::VISIBLE + String("=?");
const AutoPtr<ArrayOf<String> > CCalendarContractInstances::WHERE_CALENDARS_ARGS = initWHERECALENDARSARGS();
const String CCalendarContractInstances::DEFAULT_SORT_ORDER = String("begin ASC");

ECode CCalendarContractInstances::constructor()
{
    return NOERROR;
}

ECode CCalendarContractInstances::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://") + ICalendarContract::AUTHORITY + String("/instances/when"), uri);
}

ECode CCalendarContractInstances::GetCONTENTBYDAYURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://") + ICalendarContract::AUTHORITY + String("/instances/whenbyday"), uri);
}

ECode CCalendarContractInstances::GetCONTENTSEARCHURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://") + ICalendarContract::AUTHORITY + String("/instances/search"), uri);
}

ECode CCalendarContractInstances::GetCONTENTSEARCHBYDAYURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://") + ICalendarContract::AUTHORITY + String("/instances/searchbyday"), uri);
}

ECode CCalendarContractInstances::Query(
    /* [in] */ IContentResolver* cr,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ Int64 begin,
    /* [in] */ Int64 end,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);

    AutoPtr<IUriBuilder> builder;
    AutoPtr<IUri> _uri;
    FAIL_RETURN(GetCONTENTURI((IUri**)&_uri))
    FAIL_RETURN(_uri->BuildUpon((IUriBuilder**)&builder))
    AutoPtr<IContentUris> helper;
    FAIL_RETURN(CContentUris::AcquireSingleton((IContentUris**)&helper))
    FAIL_RETURN(helper->AppendId(builder, begin))
    FAIL_RETURN(helper->AppendId(builder, end))
    AutoPtr<IUri> uri;
    FAIL_RETURN(builder->Build((IUri**)&uri))
    return cr->Query(uri, projection, WHERE_CALENDARS_SELECTED,
            WHERE_CALENDARS_ARGS, DEFAULT_SORT_ORDER, cursor);
}

ECode CCalendarContractInstances::Query(
    /* [in] */ IContentResolver* cr,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ Int64 begin,
    /* [in] */ Int64 end,
    /* [in] */ const String& searchQuery,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);

    AutoPtr<IUri> _uri;
    FAIL_RETURN(GetCONTENTSEARCHURI((IUri**)&_uri))
    AutoPtr<IUriBuilder> builder;
    FAIL_RETURN(_uri->BuildUpon((IUriBuilder**)&builder))
    AutoPtr<IContentUris> helper;
    FAIL_RETURN(CContentUris::AcquireSingleton((IContentUris**)&helper))
    FAIL_RETURN(helper->AppendId(builder, begin))
    FAIL_RETURN(helper->AppendId(builder, end))
    FAIL_RETURN(builder->AppendPath(searchQuery))
    AutoPtr<IUri> uri;
    FAIL_RETURN(builder->Build((IUri**)&uri))
    return cr->Query(uri, projection, WHERE_CALENDARS_SELECTED,
            WHERE_CALENDARS_ARGS, DEFAULT_SORT_ORDER, cursor);
}

} //Provider
} //Droid
} //Elastos