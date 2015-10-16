#ifndef __ELASTOS_DROID_APP_CINNERCONNECTION_H__
#define __ELASTOS_DROID_APP_CINNERCONNECTION_H__

#include "_Elastos_Droid_App_CInnerConnection.h"
#include "elastos/droid/app/LoadedPkg.h"

namespace Elastos{
namespace Droid{
namespace App{

CarClass(CInnerConnection)
{
public:
    CARAPI Init(
        /* [in] */ LoadedPkg::ServiceDispatcher* sd);

    CARAPI Connected(
        /* [in] */ IComponentName* name,
        /* [in] */ IBinder* service);

    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI ToString(
        /* [out] */ String* str);

public:
    LoadedPkg::ServiceDispatcher* mDispatcher;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CINNERCONNECTION_H__
