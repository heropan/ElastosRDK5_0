
#ifndef __ELASTOS_DROID_WEBKIT_BASE_THREADUTILS_H__
#define __ELASTOS_DROID_WEBKIT_BASE_THREADUTILS_H__

// import android.os.Handler;
// import android.os.Looper;
// import android.os.Process;

// import java.util.concurrent.Callable;
// import java.util.concurrent.ExecutionException;
// import java.util.concurrent.FutureTask;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

/**
 * Helper methods to deal with threading related tasks.
 */
class ThreadUtils
{
public:
    static CARAPI_(void) SetWillOverrideUiThread();

    static CARAPI_(void) SetUiThread(
        /* [in] */ ILooper* looper);

    /**
     * Run the supplied Runnable on the main thread. The method will block until the Runnable
     * completes.
     *
     * @param r The Runnable to run.
     */
    static CARAPI_(void) RunOnUiThreadBlocking(
        /* [in] */ const IRunnable* r);

    /**
     * Run the supplied Callable on the main thread, wrapping any exceptions in a RuntimeException.
     * The method will block until the Callable completes.
     *
     * @param c The Callable to run
     * @return The result of the callable
     */
    public static <T> T runOnUiThreadBlockingNoException(Callable<T> c);

    /**
     * Run the supplied Callable on the main thread, The method will block until the Callable
     * completes.
     *
     * @param c The Callable to run
     * @return The result of the callable
     * @throws ExecutionException c's exception
     */
    public static <T> T runOnUiThreadBlocking(Callable<T> c);

    /**
     * Run the supplied FutureTask on the main thread. The method will block only if the current
     * thread is the main thread.
     *
     * @param task The FutureTask to run
     * @return The queried task (to aid inline construction)
     */
    public static <T> FutureTask<T> runOnUiThread(FutureTask<T> task);

    /**
     * Run the supplied Callable on the main thread. The method will block only if the current
     * thread is the main thread.
     *
     * @param c The Callable to run
     * @return A FutureTask wrapping the callable to retrieve results
     */
    public static <T> FutureTask<T> runOnUiThread(Callable<T> c);

    /**
     * Run the supplied Runnable on the main thread. The method will block only if the current
     * thread is the main thread.
     *
     * @param r The Runnable to run
     */
    public static CARAPI_(void) runOnUiThread(Runnable r);

    /**
     * Post the supplied FutureTask to run on the main thread. The method will not block, even if
     * called on the UI thread.
     *
     * @param task The FutureTask to run
     * @return The queried task (to aid inline construction)
     */
    public static <T> FutureTask<T> postOnUiThread(FutureTask<T> task);

    /**
     * Post the supplied Runnable to run on the main thread. The method will not block, even if
     * called on the UI thread.
     *
     * @param task The Runnable to run
     */
    public static CARAPI_(void) PostOnUiThread(
        /* [in] */ IRunnable* task);

    /**
     * Post the supplied Runnable to run on the main thread after the given amount of time. The
     * method will not block, even if called on the UI thread.
     *
     * @param task The Runnable to run
     * @param delayMillis The delay in milliseconds until the Runnable will be run
     */
    public static CARAPI_(void) PostOnUiThreadDelayed(
        /* [in] */ IRunnable* task,
        /* [in] */ Int64 delayMillis);

    /**
     * Asserts that the current thread is running on the main thread.
     */
    public static CARAPI_(void) AssertOnUiThread();

    /**
     * @return true iff the current thread is the main (UI) thread.
     */
    public static CARAPI_(Boolean) RunningOnUiThread();

    public static CARAPI_(AutoPtr<ILooper>) GetUiThreadLooper();

    /**
     * Set thread priority to audio.
     */
    //@CalledByNative
    public static CARAPI_(void) SetThreadPriorityAudio(int tid);

private:
    static CARAPI_(AutoPtr<IHandler>) GetUiThreadHandler();

    static const Object sLock;

    static Boolean sWillOverride;

    static AutoPtr<IHandler> sUiThreadHandler;
};

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_BASE_THREADUTILS_H__
