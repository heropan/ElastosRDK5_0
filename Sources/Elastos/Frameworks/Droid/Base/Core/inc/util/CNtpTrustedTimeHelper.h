#ifndef __CNTPTRUSTEDTIMEHELPER_H__
#define __CNTPTRUSTEDTIMEHELPER_H__

#include "_CNtpTrustedTimeHelper.h"

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

#endif //__CNTPTRUSTEDTIMEHELPER_H__
