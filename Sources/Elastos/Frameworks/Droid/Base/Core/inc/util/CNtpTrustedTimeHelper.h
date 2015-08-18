#ifndef __ELASTOS_DROID_UTILITY_CNTPTRUSTEDTIMEHELPER_H__
#define __ELASTOS_DROID_UTILITY_CNTPTRUSTEDTIMEHELPER_H__

#include "_Elastos_Droid_Utility_CNtpTrustedTimeHelper.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Utility {

CarClass(CNtpTrustedTimeHelper)
{
public:
    CARAPI GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ INtpTrustedTime** ntt);
};

} // namespace Utility
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_UTILITY_CNTPTRUSTEDTIMEHELPER_H__
