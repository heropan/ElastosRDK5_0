#ifndef __ELASTOS_DROID_OS_SOME_ARGS_H__
#define __ELASTOS_DROID_OS_SOME_ARGS_H__

#include <elastos/core/Object.h>

namespace Elastos {
namespace Droid {
namespace Os {

class SomeArgs
    : public Object
{
public:
    static CARAPI_(AutoPtr<SomeArgs>) Obtain();

    CARAPI Recycle();

private:
    CARAPI_(void) Clear();

    SomeArgs();

public:
    AutoPtr<IInterface> mArg1;
    AutoPtr<IInterface> mArg2;
    AutoPtr<IInterface> mArg3;
    AutoPtr<IInterface> mArg4;

    Int32 mArgi1;
    Int32 mArgi2;
    Int32 mArgi3;
    Int32 mArgi4;
    Int32 mArgi5;
    Int32 mArgi6;

private:
    static const Int32 MAX_POOL_SIZE;

    static AutoPtr<SomeArgs> sPool;
    static Int32 sPoolSize;
    static Object sPoolLock;

    AutoPtr<SomeArgs> mNext;

    Boolean mInPool;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

DEFINE_CONVERSION_FOR(Elastos::Droid::Os::SomeArgs, IInterface)

#endif //__ELASTOS_DROID_OS_SOME_ARGS_H__
