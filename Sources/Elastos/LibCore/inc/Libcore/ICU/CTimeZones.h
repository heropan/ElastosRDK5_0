
#ifndef __ICU_CTIMEZONES_H__
#define __ICU_CTIMEZONES_H__

#include "cmdef.h"
#include "_CTimeZones.h"

using Elastos::Core::IArrayOf;

namespace Libcore {
namespace ICU {

CarClass(CTimeZones)
{
public:
    CARAPI GetDisplayName(
        /* [in] */ ArrayOf<IArrayOf*>*  zoneStrings,
        /* [in] */ const String& id,
        /* [in] */ Boolean daylight,
        /* [in] */ Int32 style ,
        /* [out] */ String * str);

    CARAPI GetZoneStrings(
        /* [in] */ ILocale * locale,
        /* [out,callee] */ ArrayOf<IArrayOf*>** outarray);

    CARAPI ForLocale(
        /* [in] */ ILocale * locale ,
        /* [out,callee] */ ArrayOf<String>** outarray);
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_CTIMEZONES_H__
