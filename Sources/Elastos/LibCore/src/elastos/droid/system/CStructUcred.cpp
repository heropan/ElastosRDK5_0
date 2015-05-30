#include "CStructoUcred.h"

namespace Elastos {
namespace Droid {
namespace System {
CAR_OBJECT_IMPL(CStructoUcred)

CAR_INTERFACE_IMPL(CStructoUcred, Object, IStructUcred)

ECode CStructoUcred::constructor(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    mPid = pid;
    mUid = uid;
    mGid = gid;
    return NOERROR;
}

ECode CStructoUcred::GetPid(
    /* [out] */ Int32* pid)
{
    *pid = mPid;
    return NOERROR;
}

ECode CStructoUcred::GetUid(
    /* [out] */ Int32* uid)
{
    *uid = mUid;
    return NOERROR;
}

ECode CStructoUcred::GetGid(
    /* [out] */ Int32* gid)
{
    *gid = mGid;
    return NOERROR;
}

}// namespace System
}// namespace Droid
}// namespace Elastos