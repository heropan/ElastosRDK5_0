
#include "CNetPermission.h"


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
