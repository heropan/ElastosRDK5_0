
#include "CTimeZones.h"
#include "TimeZones.h"

namespace Libcore {
namespace ICU {

ECode CTimeZones::GetDisplayName(
    /* [in] */ ArrayOf<IArrayOf*>*  zoneStrings,
    /* [in] */ const String& id,
    /* [in] */ Boolean daylight,
    /* [in] */ Int32 style ,
    /* [out] */ String * str)
{
    return TimeZones::GetDisplayName(zoneStrings, id, daylight, style, str);
}

ECode CTimeZones::GetZoneStrings(
    /* [in] */ ILocale * locale,
    /* [out,callee] */ ArrayOf<IArrayOf*> ** outarray)
{
    return TimeZones::GetZoneStrings(locale, outarray);
}

ECode CTimeZones::ForLocale(
    /* [in] */ ILocale * locale ,
    /* [out,callee] */ ArrayOf<String> ** outarray)
{
    return TimeZones::ForLocale(locale, outarray);
}

} // namespace ICU
} // namespace Libcore
