
#ifndef __ELASTOS_DROID_WEBKIT_BASE_JAVAHANDLERTHREAD_H__
#define __ELASTOS_DROID_WEBKIT_BASE_JAVAHANDLERTHREAD_H__

// import android.os.Handler;
// import android.os.HandlerThread;

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
    {
    public:
        InnerRunnable(
            /* [in] */ JavaHandlerThread* owner);

        //@Override
        CARAPI Run();

    private:
        JavaHandlerThread* mOwner;
    };

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
