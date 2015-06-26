
#include "CNetPermission.h"

namespace Elastos {
namespace Net {

CAR_INTERFACE_IMPL(CNetPermission, Object, INetPermission)

CAR_OBJECT_IMPL(CNetPermission)

ECode CNetPermission::constructor(
    /* [in] */ const String& name)
{
    //return BasicPermission::Init(name);
}

ECode CNetPermission::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& action)
{
    //return BasicPermission::Init(name, action);
}

} // namespace Net
} // namespace Elastos
