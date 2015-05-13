
#ifndef __CNETPERMISSION_H__
#define __CNETPERMISSION_H__

#include "_CNetPermission.h"
#include "BasicPermission.h"
#include <elastos/AutoPtr.h>

CarClass(CNetPermission) , public BasicPermission
{
public:
    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& actions);
};

#endif //__CNETPERMISSION_H__
