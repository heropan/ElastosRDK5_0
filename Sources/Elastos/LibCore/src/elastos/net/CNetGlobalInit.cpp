
#include "CNetGlobalInit.h"

extern void InitCNet4Address();

ECode CNetGlobalInit::Initialize()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNetGlobalInit::constructor()
{
    // TODO: Add your code here
    InitCNet4Address();
    return NOERROR;
}

