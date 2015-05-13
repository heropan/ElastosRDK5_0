
#ifndef __ICU_TIMEZONES_H__
#define __ICU_TIMEZONES_H__

#include "cmdef.h"
#include "Elastos.Core_server.h"
#include <elastos/HashMap.h>
#include <elastos/Mutex.h>

using Elastos::Core::IArrayOf;
using Elastos::Core::Threading::Mutex;
using Elastos::Utility::HashMap;

namespace Libcore {
namespace ICU {

class TimeZones
{
public:
    class ZoneStringsCache
        : public ElLightRefBase
    {
    public:
        ZoneStringsCache();

        ~ZoneStringsCache();

        CARAPI_(AutoPtr< ArrayOf< AutoPtr< ArrayOf<String> > > >) Create(
            /* [in] */ AutoPtr<ILocale> locale);

    private:
        CARAPI_(void) InternStrings(
            /* [in] */ ArrayOf<StringArray>* result);

        // static final Comparator<String[]> ZONE_STRINGS_COMPARATOR = new Comparator<String[]>() {
        // public int compare(String[] lhs, String[] rhs) {
        //     return lhs[OLSON_NAME].compareTo(rhs[OLSON_NAME]);
        // }

    private:
        HashMap<String, String> mInternTable;
        Mutex mlock;
    };

public:
    static CARAPI GetDisplayName(
        /* [in] */ ArrayOf<IArrayOf*>*  zoneStrings,
        /* [in] */ const String& id,
        /* [in] */ Boolean daylight,
        /* [in] */ Int32 style ,
        /* [out] */ String * str);

    static CARAPI GetZoneStrings(
        /* [in] */ ILocale * locale,
        /* [out,callee] */ ArrayOf<IArrayOf*>** outarray);

    static CARAPI ForLocale(
        /* [in] */ ILocale * locale ,
        /* [out,callee] */ ArrayOf<String>** outarray);

private:
    static CARAPI_(AutoPtr< ArrayOf<String> >) ForCountryCode(
        /* [in] */ const String& countryCode);

    static CARAPI_(AutoPtr< ArrayOf< AutoPtr< ArrayOf<String> > > >) GetZoneStringsImpl(
        /* [in] */ const String& localeName,
        /* [in] */ ArrayOf<String>* timeZoneIds);

    static CARAPI_(AutoPtr< ArrayOf<String> >) GetAvailableTimeZones();

private:
    static AutoPtr<ArrayOf<String> > sAvailableTimeZones;
    static AutoPtr<ZoneStringsCache> sCachedZoneStrings;
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_TIMEZONES_H__
