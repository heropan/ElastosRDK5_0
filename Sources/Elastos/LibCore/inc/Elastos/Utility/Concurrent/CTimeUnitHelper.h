
#ifndef __CTIMEUNITHELPER_H__
#define __CTIMEUNITHELPER_H__

#include "_CTimeUnitHelper.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CTimeUnitHelper)
{
public:
    CARAPI GetNANOSECONDS(
        /* [out] */ ITimeUnit** unit);

    CARAPI GetMICROSECONDS(
        /* [out] */ ITimeUnit** unit);

    CARAPI GetMILLISECONDS(
        /* [out] */ ITimeUnit** unit);

    CARAPI GetSECONDS(
        /* [out] */ ITimeUnit** unit);

    CARAPI GetMINUTES(
        /* [out] */ ITimeUnit** unit);

    CARAPI GetHOURS(
        /* [out] */ ITimeUnit** unit);

    CARAPI GetDAYS(
        /* [out] */ ITimeUnit** unit);
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__CTIMEUNITHELPER_H__
