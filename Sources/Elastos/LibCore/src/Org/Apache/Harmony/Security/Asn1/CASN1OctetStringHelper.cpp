
#include "CASN1OctetStringHelper.h"
#include "CASN1OctetString.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {


ECode CASN1OctetStringHelper::GetInstance(
    /* [out] */ IASN1Type** instance)
{
    return CASN1OctetString::GetInstance(instance);
}

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org
