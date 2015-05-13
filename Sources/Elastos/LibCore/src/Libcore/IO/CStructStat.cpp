
#include "cmdef.h"
#include "CStructStat.h"

namespace Libcore {
namespace IO {

ECode CStructStat::GetDev(
    /* [out] */ Int64* dev)
{
    VALIDATE_NOT_NULL(dev);

    *dev = st_dev;
    return NOERROR;
}

ECode CStructStat::GetIno(
    /* [out] */ Int64* ino)
{
    VALIDATE_NOT_NULL(ino);

    *ino = st_ino;
    return NOERROR;
}

ECode CStructStat::GetMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);

    *mode = st_mode;
    return NOERROR;
}

ECode CStructStat::GetNlink(
    /* [out] */ Int64* nlink)
{
    VALIDATE_NOT_NULL(nlink);

    *nlink = st_nlink;
    return NOERROR;
}

ECode CStructStat::GetUid(
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid);

    *uid = st_uid;
    return NOERROR;
}

ECode CStructStat::GetGid(
    /* [out] */ Int32* gid)
{
    VALIDATE_NOT_NULL(gid);

    *gid = st_gid;
    return NOERROR;
}

ECode CStructStat::GetRdev(
    /* [out] */ Int64* rdev)
{
    VALIDATE_NOT_NULL(rdev);

    *rdev = st_rdev;
    return NOERROR;
}

ECode CStructStat::GetSize(
    /* [out] */ Int64* size)
{
    VALIDATE_NOT_NULL(size);

    *size = st_size;
    return NOERROR;
}

ECode CStructStat::GetAtime(
    /* [out] */ Int64* atime)
{
    VALIDATE_NOT_NULL(atime);

    *atime = st_atime;
    return NOERROR;
}

ECode CStructStat::GetMtime(
    /* [out] */ Int64* mtime)
{
    VALIDATE_NOT_NULL(mtime);

    *mtime = st_mtime;
    return NOERROR;
}

ECode CStructStat::GetCtime(
    /* [out] */ Int64* ctime)
{
    VALIDATE_NOT_NULL(ctime);

    *ctime = st_ctime;
    return NOERROR;
}

ECode CStructStat::GetBlksize(
    /* [out] */ Int64* blksize)
{
    VALIDATE_NOT_NULL(blksize);

    *blksize = st_blksize;
    return NOERROR;
}

ECode CStructStat::GetBlocks(
    /* [out] */ Int64* blocks)
{
    VALIDATE_NOT_NULL(blocks);

    *blocks = st_blocks;
    return NOERROR;
}

ECode CStructStat::constructor(
    /* [in] */ Int64 dev,
    /* [in] */ Int64 ino,
    /* [in] */ Int32 mode,
    /* [in] */ Int64 nlink,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid,
    /* [in] */ Int64 rdev,
    /* [in] */ Int64 size,
    /* [in] */ Int64 atime,
    /* [in] */ Int64 mtime,
    /* [in] */ Int64 ctime,
    /* [in] */ Int64 blksize,
    /* [in] */ Int64 blocks)
{
    st_dev = dev;
    st_ino = ino;
    st_mode = mode;
    st_nlink = nlink;
    st_uid = uid;
    st_gid = gid;
    st_rdev = rdev;
    st_size = size;
    st_atime = atime;
    st_mtime = mtime;
    st_ctime = ctime;
    st_blksize = blksize;
    st_blocks = blocks;
    return NOERROR;
}

} // namespace IO
} // namespace Libcore
