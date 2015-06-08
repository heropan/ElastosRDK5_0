
#include "CNetPermission.h"

CAR_INTERFACE_IMPL(CNetPermission, BasicPermission, INetPermission)

CAR_OBJECT_IMPL(CNetPermission)

ECode CNetPermission::constructor(
    /* [in] */ const String& name)
{
    return BasicPermission::Init(name);
}

ECode CNetPermission::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& action)
{
    return BasicPermission::Init(name, action);
}
