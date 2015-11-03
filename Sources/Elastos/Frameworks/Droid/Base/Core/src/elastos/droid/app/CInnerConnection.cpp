#include "elastos/droid/app/CInnerConnection.h"

namespace Elastos{
namespace Droid{
namespace App{

ECode CInnerConnection::Init(
    /* [in] */ LoadedPkg::ServiceDispatcher* sd)
{
    mDispatcher = sd;
    return NOERROR;
}

ECode CInnerConnection::Connected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    LoadedPkg::ServiceDispatcher* sd = mDispatcher;
    if (sd != NULL) {
        sd->Connected(name, service);
    }
    return NOERROR;
}

ECode CInnerConnection::ToString(
    /* [out] */ String* str)
{
    return Object::ToString(str);
}

}
}
}