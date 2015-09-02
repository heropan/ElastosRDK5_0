
#ifndef __ELASTOS_DROID_WEBKIT_BASE_JAVAHANDLERTHREAD_H__
#define __ELASTOS_DROID_WEBKIT_BASE_JAVAHANDLERTHREAD_H__

#include "ext/frameworkext.h"

using Elastos::Core::IRunnable;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IHandlerThread;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

/**
 * This class is an internal detail of the native counterpart.
 * It is instantiated and owned by the native object.
 */
//@JNINamespace("base::android")
class JavaHandlerThread
{
private:
    class InnerRunnable
        : public IRunnable
    {
    public:
        InnerRunnable(
            /* [in] */ JavaHandlerThread* owner,
            /* [in] */ const Int64 nativeThread,
            /* [in] */ const Int64 nativeEvent);

        CAR_INTERFACE_DECL();

        //@Override
        CARAPI Run();

    private:
        JavaHandlerThread* mOwner;
        const Int64 mNativeThread;
        const Int64 mNativeEvent;
    };

public:
    CAR_INTERFACE_DECL();

public:
    AutoPtr<IHandlerThread> mThread;

private:
    JavaHandlerThread(
        /* [in] */ String name);

    //@CalledByNative
    static CARAPI_(AutoPtr<JavaHandlerThread>) Create(
        /* [in] */ String name);

    //@CalledByNative
    CARAPI_(void) Start(
        /* [in] */ const Int64 nativeThread,
        /* [in] */ const Int64 nativeEvent);

    CARAPI_(void) NativeInitializeThread(
        /* [in] */ Int64 nativeJavaHandlerThread,
        /* [in] */ Int64 nativeEvent);
};

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_BASE_JAVAHANDLERTHREAD_H__
