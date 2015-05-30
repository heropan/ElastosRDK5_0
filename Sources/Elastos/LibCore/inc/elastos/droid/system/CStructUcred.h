#ifndef __CSTRUCTUCRED_H__
#define __CSTRUCTUCRED_H__
#include "coredef.h"
#include "core/Object.h"
#include "_Elastos_Droid_System_CStructUcred.h"

using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace System {
CarClass(CStructUcred) , public Object
{
public:
    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    CARAPI GetPid(
        /* [out] */ Int32* pid);

    CARAPI GetUid(
        /* [out] */ Int32* uid);

    CARAPI GetGid(
        /* [out] */ Int32* gid);

private:
    Int32 mPid;
    Int32 mUid;
    Int32 mGid;
};

}// namespace System
}// namespace Droid
}// namespace Elastos
#endif