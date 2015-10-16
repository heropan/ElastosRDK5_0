#include "elastos/droid/utility/CNtpTrustedTimeHelper.h"
#include "elastos/droid/utility/NtpTrustedTime.h"

namespace Elastos {
namespace Droid {
namespace Utility {

CAR_INTERFACE_IMPL(CNtpTrustedTimeHelper, Singleton, INtpTrustedTimeHelper)

CAR_SINGLETON_IMPL(CNtpTrustedTimeHelper)

ECode CNtpTrustedTimeHelper::GetInstance(
    /* [in] */ IContext* context,
    /* [out] */ INtpTrustedTime** instance)
{
    return NtpTrustedTime::GetInstance(context, instance);
}

} // namespace Utility
} // namespace Droid
} // namespace Elastos
