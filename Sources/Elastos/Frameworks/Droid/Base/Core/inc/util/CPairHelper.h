
#ifndef __ELASTOS_DROID_UTILITY_CPAIRHELPER_H__
#define __ELASTOS_DROID_UTILITY_CPAIRHELPER_H__

#include "_Elastos_Droid_Utility_CPairHelper.h"

namespace Elastos {
namespace Droid {
namespace Utility {

CarClass(CPairHelper)
{
public:
    CARAPI Create(
        /* [in] */ IInterface *A,
        /* [in] */ IInterface *B,
        /* [out] */ IPair **pair);
};

}
}
}

#endif // __ELASTOS_DROID_UTILITY_CPAIRHELPER_H__
