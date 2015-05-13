
#ifndef __CSTRUCTSTATFS_H__
#define __CSTRUCTSTATFS_H__

#include "_CStructStatFs.h"

namespace Libcore {
namespace IO {

/**
 * File information returned by fstatfs(2) and statfs(2).
 *
 * TODO: this should be {@code struct statvfs}, but Bionic doesn't support that yet.
 * @hide until the TODO is fixed.
 */
CarClass(CStructStatFs)
{
public:
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
    Int64 f_bsize; /*unsigned long*/

    /** Total block count. */
    Int64 f_blocks; /*fsblkcnt_t*/

    /** Free block count. */
    Int64 f_bfree; /*fsblkcnt_t*/

    /** Free block count available to non-root. */
    Int64 f_bavail; /*fsblkcnt_t*/

    /** Total file (inode) count. */
    Int64 f_files; /*fsfilcnt_t*/

    /** Free file (inode) count. */
    Int64 f_ffree; /*fsfilcnt_t*/

    /** Maximum filename length. */
    Int64 f_namemax; /*unsigned long*/

    /** Fundamental file system block size. */
    Int64 f_frsize; /*unsigned long*/
};

} // namespace IO
} // namespace Libcore

#endif // __CSTRUCTSTATFS_H__
