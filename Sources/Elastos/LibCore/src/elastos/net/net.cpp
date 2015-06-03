#include "CInet4Address.h"
#include "droid/system/OsConstants.h"

using Elastos::Droid::System::OsConstants;

namespace Elastos {
namespace Net {

CAR_OBJECT_IMPL(CInet4Address)

ECode CInet4Address::constructor(
    /* [in] */ ArrayOf<Byte>* address,
    /* [in] */ const String& name)
{
    return InetAddress::constructor(OsConstants::_AF_INET, address, name);
}

} // namespace Net
} // namespace Elastos