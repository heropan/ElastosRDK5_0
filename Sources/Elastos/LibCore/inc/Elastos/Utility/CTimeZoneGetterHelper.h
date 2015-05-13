#ifndef __UTILITY_CTIMEZONEGETTERHELPER_H__
#define __UTILITY_CTIMEZONEGETTERHELPER_H__

#include "_CTimeZoneGetterHelper.h"

namespace Elastos {
namespace Utility {

CarClass(CTimeZoneGetterHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ ITimeZoneGetter** instance);

    CARAPI SetInstance(
        /* [in] */ ITimeZoneGetter* instance);
};

} // namespace Utility
} // namespace Elastos

#endif //__UTILITY_CTIMEZONEGETTERHELPER_H__
