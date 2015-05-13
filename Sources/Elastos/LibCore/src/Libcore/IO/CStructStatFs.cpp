
#include "cmdef.h"
#include "CStructStatFs.h"

namespace Libcore {
namespace IO {

ECode CStructStatFs::GetBsize(
    /* [out] */ Int64* bsize)
{
    VALIDATE_NOT_NULL(bsize);

    *bsize = f_bsize;
    return NOERROR;
}

ECode CStructStatFs::GetBlocks(
    /* [out] */ Int64* blocks)
{
    VALIDATE_NOT_NULL(blocks);

    *blocks = f_blocks;
    return NOERROR;
}

ECode CStructStatFs::GetBfree(
    /* [out] */ Int64* bfree)
{
    VALIDATE_NOT_NULL(bfree);

    *bfree = f_bfree;
    return NOERROR;
}

ECode CStructStatFs::GetBavail(
    /* [out] */ Int64* bavail)
{
    VALIDATE_NOT_NULL(bavail);

    *bavail = f_bavail;
    return NOERROR;
}

ECode CStructStatFs::GetFiles(
    /* [out] */ Int64* files)
{
    VALIDATE_NOT_NULL(files);

    *files = f_files;
    return NOERROR;
}

ECode CStructStatFs::GetFfree(
    /* [out] */ Int64* ffree)
{
    VALIDATE_NOT_NULL(ffree);

    *ffree = f_ffree;
    return NOERROR;
}

ECode CStructStatFs::GetNamemax(
    /* [out] */ Int64* namemax)
{
    VALIDATE_NOT_NULL(namemax);

    *namemax = f_namemax;
    return NOERROR;
}

ECode CStructStatFs::GetFrsize(
    /* [out] */ Int64* frsize)
{
    VALIDATE_NOT_NULL(frsize);

    *frsize = f_frsize;
    return NOERROR;
}

ECode CStructStatFs::constructor(
    /* [in] */ Int64 bsize,
    /* [in] */ Int64 blocks,
    /* [in] */ Int64 bfree,
    /* [in] */ Int64 bavail,
    /* [in] */ Int64 files,
    /* [in] */ Int64 ffree,
    /* [in] */ Int64 namemax,
    /* [in] */ Int64 frsize)
{
    f_bsize = bsize;
    f_blocks = blocks;
    f_bfree = bfree;
    f_bavail = bavail;
    f_files = files;
    f_ffree = ffree;
    f_namemax = namemax;
    f_frsize = frsize;
    return NOERROR;
}

} // namespace IO
} // namespace Libcore
