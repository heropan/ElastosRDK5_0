#include "util/CNtpTrustedTimeHelper.h"
#include "util/NtpTrustedTime.h"

namespace Elastos {
namespace Droid {
namespace Utility {

ECode CNtpTrustedTimeHelper::GetInstance(
    /* [in] */ IContext* context,
    /* [out] */ INtpTrustedTime** instance)
{
    return NtpTrustedTime::GetInstance(context, instance);
}

} // namespace Utility
} // namespace Droid
} // namespace Elastos
