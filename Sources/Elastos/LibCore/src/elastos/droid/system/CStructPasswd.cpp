
#include "CStructPasswd.h"

namespace Elastos {
namespace Droid {
namespace System {
CAR_OBJECT_IMPL(CStructPasswd)

CAR_INTERFACE_IMPL(CStructPasswd, Object, IStructPasswd)

ECode CStructPasswd::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = pw_name;
    return NOERROR;
}

ECode CStructPasswd::GetUid(
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid);

    *uid = pw_uid;
    return NOERROR;
}

ECode CStructPasswd::GetGid(
    /* [out] */ Int32* gid)
{
    VALIDATE_NOT_NULL(gid);

    *gid = pw_gid;
    return NOERROR;
}

ECode CStructPasswd::GetDir(
    /* [out] */ String* dir)
{
    VALIDATE_NOT_NULL(dir);

    *dir = pw_dir;
    return NOERROR;
}

ECode CStructPasswd::GetShell(
    /* [out] */ String* shell)
{
    VALIDATE_NOT_NULL(shell);

    *shell = pw_shell;
    return NOERROR;
}

ECode CStructPasswd::constructor(
    /* [in] */ const String& name,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid,
    /* [in] */ const String& dir,
    /* [in] */ const String& shell)
{
    pw_name = name;
    pw_uid = uid;
    pw_gid = gid;
    pw_dir = dir;
    pw_shell = shell;
    return NOERROR;
}

} // namespace System
} // namespace Droid
} // namespace Elastos
