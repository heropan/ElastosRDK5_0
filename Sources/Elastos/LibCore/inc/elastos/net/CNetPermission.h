
#ifndef __ELASTOS_NET_CNETPERMISSION_H__
#define __ELASTOS_NET_CNETPERMISSION_H__

#include "_ELASTOS_NET_CNetPermission.h"
#include "BasicPermission.h"
#include <elastos/AutoPtr.h>

CarClass(CNetPermission)
    , public BasicPermission
    , public INetPermission
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& actions);
};

#endif //__ELASTOS_NET_CNETPERMISSION_H__
