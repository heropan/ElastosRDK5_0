
#include "CStructStatVfs.h"

namespace Elastos {
namespace Droid {
namespace System {
CAR_OBJECT_IMPL(CStructStatVfs)

CAR_INTERFACE_IMPL(CStructStatVfs, Object, IStructStatVfs)

ECode CStructStatVfs::GetBsize(
    /* [out] */ Int64* bsize)
{
    VALIDATE_NOT_NULL(bsize);

    *bsize = mF_bsize;
    return NOERROR;
}

ECode CStructStatVfs::GetBlocks(
    /* [out] */ Int64* blocks)
{
    VALIDATE_NOT_NULL(blocks);

    *blocks = mF_blocks;
    return NOERROR;
}

ECode CStructStatVfs::GetBfree(
    /* [out] */ Int64* bfree)
{
    VALIDATE_NOT_NULL(bfree);

    *bfree = mF_bfree;
    return NOERROR;
}

ECode CStructStatVfs::GetBavail(
    /* [out] */ Int64* bavail)
{
    VALIDATE_NOT_NULL(bavail);

    *bavail = mF_bavail;
    return NOERROR;
}

ECode CStructStatVfs::GetFiles(
    /* [out] */ Int64* files)
{
    VALIDATE_NOT_NULL(files);

    *files = mF_files;
    return NOERROR;
}

ECode CStructStatVfs::GetFfree(
    /* [out] */ Int64* ffree)
{
    VALIDATE_NOT_NULL(ffree);

    *ffree = mF_ffree;
    return NOERROR;
}

ECode CStructStatVfs::GetNamemax(
    /* [out] */ Int64* namemax)
{
    VALIDATE_NOT_NULL(namemax);

    *namemax = mF_namemax;
    return NOERROR;
}

ECode CStructStatVfs::GetFrsize(
    /* [out] */ Int64* frsize)
{
    VALIDATE_NOT_NULL(frsize);

    *frsize = mF_frsize;
    return NOERROR;
}

ECode CStructStatVfs::GetFavail(
    /* [out] */ Int64* favail)
{
    *favail = mF_favail;
    return NOERROR;
}

ECode CStructStatVfs::GetFsid(
    /* [out] */ Int64* fsid)
{
    *fsid = mF_fsid;
    return NOERROR;
}

ECode CStructStatVfs::GetFlag(
    /* [out] */ Int64* flag)
{
    *flag = mF_flag;
    return NOERROR;
}

ECode CStructStatVfs::constructor(
    /* [in] */ Int64 bsize,
    /* [in] */ Int64 blocks,
    /* [in] */ Int64 bfree,
    /* [in] */ Int64 bavail,
    /* [in] */ Int64 files,
    /* [in] */ Int64 ffree,
    /* [in] */ Int64 namemax,
    /* [in] */ Int64 frsize)
{
    mF_bsize = bsize;
    mF_blocks = blocks;
    mF_bfree = bfree;
    mF_bavail = bavail;
    mF_files = files;
    mF_ffree = ffree;
    mF_namemax = namemax;
    mF_frsize = frsize;
    return NOERROR;
}

} // namespace System
} // namespace Droid
} // namespace Elastos
