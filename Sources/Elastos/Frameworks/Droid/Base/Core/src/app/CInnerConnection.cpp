#include "app/CInnerConnection.h"

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

ECode CInnerConnection::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInnerConnection::ToString(
    /* [out] */ String* str)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

}
}
}