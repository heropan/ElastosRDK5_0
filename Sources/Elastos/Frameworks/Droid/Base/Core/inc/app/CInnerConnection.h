#ifndef __CINNERCONNECTION_H__
#define __CINNERCONNECTION_H__

#include "_CInnerConnection.h"
#include "app/LoadedPkg.h"

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

#endif //__CINNERCONNECTION_H__
