
#ifndef __CSTRUCTPASSWD_H__
#define __CSTRUCTPASSWD_H__

#include "_CStructPasswd.h"

namespace Libcore {
namespace IO {

/**
 * Information returned by getpwnam(3) and getpwuid(3). Corresponds to C's
 * {@code struct passwd} from
 * <a href="http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pwd.h.html">&lt;pwd.h&gt;</a>
 */
CarClass(CStructPasswd)
{
public:
    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetUid(
        /* [out] */ Int32* uid);

    CARAPI GetGid(
        /* [out] */ Int32* gid);

    CARAPI GetDir(
        /* [out] */ String* dir);

    CARAPI GetShell(
        /* [out] */ String* schell);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid,
        /* [in] */ const String& dir,
        /* [in] */ const String& shell);

private:
    String pw_name;
    Int32 pw_uid; /* uid_t */
    Int32 pw_gid; /* gid_t */
    String pw_dir;
    String pw_shell;
};

} // namespace IO
} // namespace Libcore

#endif // __CSTRUCTPASSWD_H__
