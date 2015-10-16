
#include "elastos/droid/net/CVpnProfileHelper.h"
#include "elastos/droid/net/CVpnProfile.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Net {

ECode CVpnProfileHelper::Decode(
    /* [in] */ const String& key,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ IVpnProfile** profile)
{
    return CVpnProfile::Decode(key, value, profile);
}

} // namespace Net
} // namespace Internal
} // namespace Droid
} // namespace Elastos
