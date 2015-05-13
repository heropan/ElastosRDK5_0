#include "os/CStatFs.h"
#include "stdio.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CStatFs::Restat(
    /* [in] */ const String& path)
{
    return DoStat(path, (IStructStatFs**)&mStat);
}

ECode CStatFs::GetBlockSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    Int64 _size;
    mStat->GetBsize(&_size);
    *size = (Int32)_size;
    return NOERROR;
}

ECode CStatFs::GetBlockCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    Int64 _count;
    mStat->GetBlocks(&_count);
    *count = (Int32)_count;
    return NOERROR;
}

ECode CStatFs::GetFreeBlocks(
    /* [out] */ Int32* blocks)
{
    VALIDATE_NOT_NULL(blocks);
    Int64 _blocks;
    mStat->GetBfree(&_blocks);
    *blocks = (Int32)_blocks;
    return NOERROR;
}

ECode CStatFs::GetAvailableBlocks(
    /* [out] */ Int32* blocks)
{
    VALIDATE_NOT_NULL(blocks);
    Int64 _blocks;
    mStat->GetBavail(&_blocks);
    *blocks = (Int32)_blocks;
    return NOERROR;
}

ECode CStatFs::DoStat(
    /* [in] */ const String& path,
    /* [out] */ IStructStatFs** fs)
{
    VALIDATE_NOT_NULL(fs);

    ECode ec = NOERROR;
    //try {
        AutoPtr<ILibcore> libcore;
        AutoPtr<IStructStatFs> _fs;
        CLibcore::AcquireSingleton((ILibcore**)&libcore);
        AutoPtr<IOs> ios;
        libcore->GetOs((IOs**)&ios);
        ec = ios->Statfs(path, (IStructStatFs**)&_fs);
        *fs = _fs;
        INTERFACE_ADDREF(*fs);
    //} catch (ErrnoException e) {
        if(ec != (ECode)NOERROR)
        {
            //throw new IllegalArgumentException("Invalid path: " + path, e);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    //}
    return NOERROR;
}

ECode CStatFs::constructor(
    /* [in] */ const String& path)
{
    return DoStat(path, (IStructStatFs**)&mStat);
}

} //Os
} //Droid
} //Elastos
