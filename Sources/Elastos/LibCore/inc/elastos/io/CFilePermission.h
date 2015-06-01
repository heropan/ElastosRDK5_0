
#ifndef __ELASTOS_IO_CFILEPERMISSION_H__
#define __ELASTOS_IO_CFILEPERMISSION_H__

#include "_Elastos_IO_CFilePermission.h"

namespace Elastos {
namespace IO {

CarClass(CFilePermission)
{
public:
    CARAPI constructor(
        /* [in] */ const String& path,
        /* [in] */ const String& actions);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI NewPermissionCollection(
        /* [out] */ IPermissionCollection** object);

    CARAPI GetActions(
        /* [out] */ String* actions);

    CARAPI Implies(
        /* [in] */ IPermission* p,
        /* [out] */ Boolean* result);
};

} // namespace IO
} // namespace Elastos

#endif //__ELASTOS_IO_CFILEPERMISSION_H__
