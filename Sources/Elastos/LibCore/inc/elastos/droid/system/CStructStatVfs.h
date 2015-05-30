
#ifndef __CStructStatVfs_H__
#define __CStructStatVfs_H__

#include "coredef.h"
#include "core/Object.h"
#include "_Elastos_Droid_System_CStructStatVfs.h"

using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace System {

/**
 * File information returned by fstatfs(2) and statfs(2).
 *
 * TODO: this should be {@code struct statvfs}, but Bionic doesn't support that yet.
 * @hide until the TODO is fixed.
 */
CarClass(CStructStatVfs) , public Object
{
public:
    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    CARAPI GetBsize(
        /* [out] */ Int64* bsize);

    CARAPI GetBlocks(
        /* [out] */ Int64* blocks);

    CARAPI GetBfree(
        /* [out] */ Int64* bfree);

    CARAPI GetBavail(
        /* [out] */ Int64* bavail);

    CARAPI GetFiles(
        /* [out] */ Int64* files);

    CARAPI GetFfree(
        /* [out] */ Int64* ffree);

    CARAPI GetNamemax(
        /* [out] */ Int64* namemax);

    CARAPI GetFavail(
        /* [out] */ Int64* favail);

    CARAPI GetFsid(
        /* [out] */ Int64* fsid);

    CARAPI GetFlag(
        /* [out] */ Int64* flag);

    CARAPI GetFrsize(
        /* [out] */ Int64* frsize);

    CARAPI constructor(
        /* [in] */ Int64 bsize,
        /* [in] */ Int64 blocks,
        /* [in] */ Int64 bfree,
        /* [in] */ Int64 bavail,
        /* [in] */ Int64 files,
        /* [in] */ Int64 ffree,
        /* [in] */ Int64 namemax,
        /* [in] */ Int64 frsize);

private:
    /** File system block size (used for block counts). */
    Int64 mF_bsize; /*unsigned long*/

    /** Total block count. */
    Int64 mF_blocks; /*fsblkcnt_t*/

    /** Free block count. */
    Int64 mF_bfree; /*fsblkcnt_t*/

    /** Free block count available to non-root. */
    Int64 mF_bavail; /*fsblkcnt_t*/

    /** Total file (inode) count. */
    Int64 mF_files; /*fsfilcnt_t*/

    /** Free file (inode) count. */
    Int64 mF_ffree; /*fsfilcnt_t*/

    /** Maximum filename length. */
    Int64 mF_namemax; /*unsigned long*/

    Int64 mF_favail; /*fsfilcnt_t*/

  /** File system id. */
    Int64 mF_fsid; /*unsigned long*/

  /** Bit mask of ST_* flags. */
    Int64 mF_flag; /*unsigned long*/

    /** Fundamental file system block size. */
    Int64 mF_frsize; /*unsigned long*/
};

} // namespace System
} // namespace Droid
} // namespace Elastos

#endif // __CStructStatVfs_H__
