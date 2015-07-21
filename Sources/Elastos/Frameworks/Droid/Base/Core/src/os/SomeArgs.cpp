#include "os/SomeArgs.h"
#include <elastos/core/AutoLock.h>

namespace Elastos {
namespace Droid {
namespace Os {

const Int32 SomeArgs::MAX_POOL_SIZE = 10;
AutoPtr<SomeArgs> SomeArgs::sPool;
Int32 SomeArgs::sPoolSize = 0;
Object SomeArgs::sPoolLock;

SomeArgs::SomeArgs()
    : mArgi1(0)
    , mArgi2(0)
    , mArgi3(0)
    , mArgi4(0)
    , mArgi5(0)
    , mArgi6(0)
    , mInPool(FALSE)
{
    /* do nothing - reduce visibility */
}

AutoPtr<SomeArgs> SomeArgs::Obtain()
{
    AutoLock lock(sPoolLock);

    if (sPoolSize > 0) {
        AutoPtr<SomeArgs> args = sPool;
        sPool = sPool->mNext;
        args->mNext = NULL;
        args->mInPool = FALSE;
        --sPoolSize;
        return args;
    }
    else {
        AutoPtr<SomeArgs> args =  new SomeArgs();
        return args;
    }
}

ECode SomeArgs::Recycle()
{
    if (mInPool) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoLock lock(sPoolLock);

    Clear();
    if (sPoolSize < MAX_POOL_SIZE) {
        mNext = sPool;
        mInPool = TRUE;
        sPool = this;
        ++sPoolSize;
    }
    return NOERROR;
}

void SomeArgs::Clear()
{
    mArg1 = NULL;
    mArg2 = NULL;
    mArg3 = NULL;
    mArg4 = NULL;
    mArgi1 = 0;
    mArgi2 = 0;
    mArgi3 = 0;
    mArgi4 = 0;
    mArgi5 = 0;
    mArgi6 = 0;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
