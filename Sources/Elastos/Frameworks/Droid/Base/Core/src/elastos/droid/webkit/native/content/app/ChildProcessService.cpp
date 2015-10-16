
#include "elastos/droid/webkit/native/content/app/ChildProcessService.h"

// import android.app.Service;
// import android.content.Context;
// import android.content.Intent;
// import android.graphics.SurfaceTexture;
// import android.os.Bundle;
// import android.os.IBinder;
// import android.os.ParcelFileDescriptor;
// import android.os.Process;
// import android.os.RemoteException;
// import android.util.Log;
// import android.view.Surface;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.base.library_loader.LibraryLoader;
// import org.chromium.base.library_loader.Linker;
// import org.chromium.base.library_loader.ProcessInitException;
// import org.chromium.content.browser.ChildProcessConnection;
// import org.chromium.content.browser.ChildProcessLauncher;
// import org.chromium.content.common.IChildProcessCallback;
// import org.chromium.content.common.IChildProcessService;

// import java.util.ArrayList;
// import java.util.concurrent.atomic.AtomicReference;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace App {

private static final String MAIN_THREAD_NAME = "ChildProcessMain";
private static final String TAG = "ChildProcessService";
private static AtomicReference<Context> sContext = new AtomicReference<Context>(null);

ChildProcessService::ChildProcessService()
    : mCpuCount(0)
    , mCpuFeatures(0)
    , mLibraryInitialized(FALSE)
    , mIsBound(FALSE)
{
    assert(0);
#if 0
    // Binder object used by clients for this service.
    private final IChildProcessService.Stub mBinder = new IChildProcessService.Stub() {
        // NOTE: Implement any IChildProcessService methods here.
        @Override
        public int setupConnection(Bundle args, IChildProcessCallback callback) {
            mCallback = callback;
            synchronized (mMainThread) {
                // Allow the command line to be set via bind() intent or setupConnection, but
                // the FD can only be transferred here.
                if (mCommandLineParams == null) {
                    mCommandLineParams = args.getStringArray(
                            ChildProcessConnection.EXTRA_COMMAND_LINE);
                }
                // We must have received the command line by now
                assert mCommandLineParams != null;
                mCpuCount = args.getInt(ChildProcessConnection.EXTRA_CPU_COUNT);
                mCpuFeatures = args.getLong(ChildProcessConnection.EXTRA_CPU_FEATURES);
                assert mCpuCount > 0;
                mFileIds = new ArrayList<Integer>();
                mFileFds = new ArrayList<ParcelFileDescriptor>();
                for (int i = 0;; i++) {
                    String fdName = ChildProcessConnection.EXTRA_FILES_PREFIX + i
                            + ChildProcessConnection.EXTRA_FILES_FD_SUFFIX;
                    ParcelFileDescriptor parcel = args.getParcelable(fdName);
                    if (parcel == null) {
                        // End of the file list.
                        break;
                    }
                    mFileFds.add(parcel);
                    String idName = ChildProcessConnection.EXTRA_FILES_PREFIX + i
                            + ChildProcessConnection.EXTRA_FILES_ID_SUFFIX;
                    mFileIds.add(args.getInt(idName));
                }
                Bundle sharedRelros = args.getBundle(Linker.EXTRA_LINKER_SHARED_RELROS);
                if (sharedRelros != null) {
                    Linker.useSharedRelros(sharedRelros);
                    sharedRelros = null;
                }
                mMainThread.notifyAll();
            }
            return Process.myPid();
        }

        @Override
        public void crashIntentionallyForTesting() {
            Process.killProcess(Process.myPid());
        }
    };
#endif
}

AutoPtr<IContext> ChildProcessService::GetContext()
{
    assert(0);
#if 0
    return sContext.get();
#endif
    return NULL;
}

//@Override
ECode ChildProcessService::OnCreate()
{
    assert(0);
#if 0
    Log.i(TAG, "Creating new ChildProcessService pid=" + Process.myPid());
    if (sContext.get() != null) {
        Log.e(TAG, "ChildProcessService created again in process!");
    }
    sContext.set(this);
    super.onCreate();

    mMainThread = new Thread(new Runnable() {
        @Override
        public void run()  {
            try {
                boolean useLinker = Linker.isUsed();

                if (useLinker) {
                    synchronized (mMainThread) {
                        while (!mIsBound) {
                            mMainThread.wait();
                        }
                    }
                    if (mLinkerParams != null) {
                        if (mLinkerParams.mWaitForSharedRelro)
                            Linker.initServiceProcess(mLinkerParams.mBaseLoadAddress);
                        else
                            Linker.disableSharedRelros();

                        Linker.setTestRunnerClassName(mLinkerParams.mTestRunnerClassName);
                    }
                }
                try {
                    LibraryLoader.loadNow(getApplicationContext(), false);
                } catch (ProcessInitException e) {
                    Log.e(TAG, "Failed to load native library, exiting child process", e);
                    System.exit(-1);
                }
                synchronized (mMainThread) {
                    while (mCommandLineParams == null) {
                        mMainThread.wait();
                    }
                }
                LibraryLoader.initialize(mCommandLineParams);
                synchronized (mMainThread) {
                    mLibraryInitialized = true;
                    mMainThread.notifyAll();
                    while (mFileIds == null) {
                        mMainThread.wait();
                    }
                }
                assert mFileIds.size() == mFileFds.size();
                int[] fileIds = new int[mFileIds.size()];
                int[] fileFds = new int[mFileFds.size()];
                for (int i = 0; i < mFileIds.size(); ++i) {
                    fileIds[i] = mFileIds.get(i);
                    fileFds[i] = mFileFds.get(i).detachFd();
                }
                ContentMain.initApplicationContext(sContext.get().getApplicationContext());
                nativeInitChildProcess(sContext.get().getApplicationContext(),
                        ChildProcessService.this, fileIds, fileFds,
                        mCpuCount, mCpuFeatures);
                ContentMain.start();
                nativeExitChildProcess();
            } catch (InterruptedException e) {
                Log.w(TAG, MAIN_THREAD_NAME + " startup failed: " + e);
            } catch (ProcessInitException e) {
                Log.w(TAG, MAIN_THREAD_NAME + " startup failed: " + e);
            }
        }
    }, MAIN_THREAD_NAME);
    mMainThread.start();
#endif

    return E_NOT_IMPLEMENTED;
}

//@Override
ECode ChildProcessService::OnDestroy()
{
    assert(0);
#if 0
    Log.i(TAG, "Destroying ChildProcessService pid=" + Process.myPid());
    super.onDestroy();
    if (mCommandLineParams == null) {
        // This process was destroyed before it even started. Nothing more to do.
        return;
    }
    synchronized (mMainThread) {
        try {
            while (!mLibraryInitialized) {
                // Avoid a potential race in calling through to native code before the library
                // has loaded.
                mMainThread.wait();
            }
        } catch (InterruptedException e) {
            // Ignore
        }
    }
    // Try to shutdown the MainThread gracefully, but it might not
    // have chance to exit normally.
    nativeShutdownMainThread();
#endif

    return E_NOT_IMPLEMENTED;
}

//@Override
ECode ChildProcessService::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    assert(0);
#if 0
    // We call stopSelf() to request that this service be stopped as soon as the client
    // unbinds. Otherwise the system may keep it around and available for a reconnect. The
    // child processes do not currently support reconnect; they must be initialized from
    // scratch every time.
    stopSelf();

    synchronized (mMainThread) {
        mCommandLineParams = intent.getStringArrayExtra(
                ChildProcessConnection.EXTRA_COMMAND_LINE);
        mLinkerParams = null;
        if (Linker.isUsed())
            mLinkerParams = new ChromiumLinkerParams(intent);
        mIsBound = true;
        mMainThread.notifyAll();
    }

    return mBinder;
#endif

    return E_NOT_IMPLEMENTED;
}

/**
 * Called from native code to share a surface texture with another child process.
 * Through using the callback object the browser is used as a proxy to route the
 * call to the correct process.
 *
 * @param pid Process handle of the child process to share the SurfaceTexture with.
 * @param surfaceObject The Surface or SurfaceTexture to share with the other child process.
 * @param primaryID Used to route the call to the correct client instance.
 * @param secondaryID Used to route the call to the correct client instance.
 */
//@SuppressWarnings("unused")
//@CalledByNative
void ChildProcessService::EstablishSurfaceTexturePeer(
    /* [in] */ Int32 pid,
    /* [in] */ Object& surfaceObject,
    /* [in] */ Int32 primaryID,
    /* [in] */ Int32 secondaryID)
{
    assert(0);
#if 0
    if (mCallback == null) {
        Log.e(TAG, "No callback interface has been provided.");
        return;
    }

    Surface surface = null;
    boolean needRelease = false;
    if (surfaceObject instanceof Surface) {
        surface = (Surface) surfaceObject;
    } else if (surfaceObject instanceof SurfaceTexture) {
        surface = new Surface((SurfaceTexture) surfaceObject);
        needRelease = true;
    } else {
        Log.e(TAG, "Not a valid surfaceObject: " + surfaceObject);
        return;
    }
    try {
        mCallback.establishSurfacePeer(pid, surface, primaryID, secondaryID);
    } catch (RemoteException e) {
        Log.e(TAG, "Unable to call establishSurfaceTexturePeer: " + e);
        return;
    } finally {
        if (needRelease) {
            surface.release();
        }
    }
#endif
}

//@SuppressWarnings("unused")
//@CalledByNative
AutoPtr<ISurface> ChildProcessService::GetViewSurface(
    /* [in] */ Int32 surfaceId)
{
    assert(0);
#if 0
    if (mCallback == null) {
        Log.e(TAG, "No callback interface has been provided.");
        return null;
    }

    try {
        return mCallback.getViewSurface(surfaceId).getSurface();
    } catch (RemoteException e) {
        Log.e(TAG, "Unable to call establishSurfaceTexturePeer: " + e);
        return null;
    }
#endif

    return NULL;
}

//@SuppressWarnings("unused")
//@CalledByNative
AutoPtr<ISurface> ChildProcessService::GetSurfaceTextureSurface(
    /* [in] */ Int32 primaryId,
    /* [in] */ Int32 secondaryId)
{
    assert(0);
#if 0
    if (mCallback == null) {
        Log.e(TAG, "No callback interface has been provided.");
        return null;
    }

    try {
        return mCallback.getSurfaceTextureSurface(primaryId, secondaryId).getSurface();
    } catch (RemoteException e) {
        Log.e(TAG, "Unable to call getSurfaceTextureSurface: " + e);
        return null;
    }
#endif

    return NULL;
}

/**
 * The main entry point for a child process. This should be called from a new thread since
 * it will not return until the child process exits. See child_process_service.{h,cc}
 *
 * @param applicationContext The Application Context of the current process.
 * @param service The current ChildProcessService object.
 * @param fileIds A list of file IDs that should be registered for access by the renderer.
 * @param fileFds A list of file descriptors that should be registered for access by the
 * renderer.
 */
void ChildProcessService::NativeInitChildProcess(
    /* [in] */ IContext* applicationContext,
    /* [in] */ ChildProcessService* service,
    /* [in] */ ArrayOf<Int32>* extraFileIds,
    /* [in] */ ArrayOf<Int32>* extraFileFds,
    /* [in] */ Int32 cpuCount,
    /* [in] */ Int64 cpuFeatures)
{
}

/**
 * Force the child process to exit.
 */
void ChildProcessService::NativeExitChildProcess()
{
}

void ChildProcessService::NativeShutdownMainThread()
{
}

} // namespace App
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
