#ifndef __CSTATFS_H__
#define __CSTATFS_H__

#include "ext/frameworkdef.h"
#include "_CStatFs.h"
#include "Elastos.Core.h"

using Libcore::IO::IStructStatFs;
using Libcore::IO::IOs;
using Libcore::IO::ILibcore;
using Libcore::IO::CLibcore;

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CStatFs)
{
public:
    CARAPI Restat(
        /* [in] */ const String& path);

    CARAPI GetBlockSize(
        /* [out] */ Int32* size);

    CARAPI GetBlockCount(
        /* [out] */ Int32* count);

    CARAPI GetFreeBlocks(
        /* [out] */ Int32* blocks);

    CARAPI GetAvailableBlocks(
        /* [out] */ Int32* blocks);

    CARAPI constructor(
        /* [in] */ const String& path);

private:
    static CARAPI DoStat(
        /* [in] */ const String& path,
        /* [out] */ IStructStatFs** fs);

private:
    AutoPtr<IStructStatFs> mStat;
};

} //Os
} //Droid
} //Elastos

#endif //__CSTATFS_H__
