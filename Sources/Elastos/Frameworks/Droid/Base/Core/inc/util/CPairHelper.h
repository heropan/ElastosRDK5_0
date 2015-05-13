
#ifndef __CPAIRHELPER_H__
#define __CPAIRHELPER_H__

#include "_CPairHelper.h"

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

#endif // __CPAIRHELPER_H__
